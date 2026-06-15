#include "interrupt.h"
#include "motor.h"
#include "sensors.h"

/* ===================== НАСТРОЙКИ СКОРОСТЕЙ ===================== */
/* Все значения — скважность ШИМ 0..1000 (ARR таймера = 999).      */

#define BASE_SPEED   400   // базовая скорость движения по прямой
#define MAX_SPEED    650   // потолок на одно колесо (защита от перелёта duty)
#define TURN_SPEED   300   // скорость доворота при ПОЛНОЙ потере линии

/* ===================== КОЭФФИЦИЕНТЫ ПИД ======================= */
/* СТАРТОВЫЕ значения. Их ОБЯЗАТЕЛЬНО подбирать под свой робот     */
/* (см. процедуру настройки в ответе).                            */

#define KP    50    // пропорциональный: насколько резко реагирует на смещение
#define KI     0    // интегральный: НАЧНИ СТРОГО С НУЛЯ
#define KD   200    // дифференциальный: гасит раскачку/перелёт через линию

#define I_LIMIT 150  // анти-windup: ограничение интегральной части

#define MIN_PRESS_DURATION 50  // антидребезг кнопки, мс (тиков SysTick)

/* ===================== ВНУТРЕННЕЕ СОСТОЯНИЕ =================== */

static volatile uint32_t ticks = 0;
static volatile uint32_t press_time = 0;
static volatile uint8_t  button_permission = 0;
static volatile uint8_t  tim2_flag = 0;

/* Память ПИД-регулятора */
static volatile int32_t last_error = 0;
static volatile int32_t integral   = 0;

/* Счётчик перекрёстков */
static volatile uint8_t cross_count = 0;
static volatile uint8_t is_on_cross = 0;

static void led_toggle(void)
{
    GPIOC->ODR ^= GPIO_ODR_ODR_2;
}

void SysTick_Handler(void)
{
    static uint32_t led_counter = 0;
    led_counter++;
    uint32_t current_interval = (button_permission == 1) ? 100U : 500U;

    if (led_counter >= current_interval)
    {
        led_toggle();
        led_counter = 0;
    }
    ticks++;
}

uint32_t get_ticks(void)
{
    return ticks;
}

void button_interrupt_init(void)
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
    MODIFY_REG(SYSCFG->EXTICR[13 / 4], 0xFU << ((13 % 4) * 4), 0x2U << ((13 % 4) * 4));
    SET_BIT(EXTI->IMR, EXTI_IMR_IM13);
    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR13);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR13);
    NVIC_SetPriority(EXTI15_10_IRQn, 2);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void)
{
    if (READ_BIT(EXTI->PR, EXTI_PR_PR13) != 0)
    {
        if (READ_BIT(GPIOC->IDR, GPIO_IDR_IDR_13) == 0)
        {
            press_time = ticks;
        }
        else
        {
            uint32_t release_time = ticks;
            if ((release_time - press_time) >= MIN_PRESS_DURATION)
            {
                button_permission = !button_permission;

                /* При перезапуске сбрасываем счётчики И память регулятора */
                if (button_permission == 1)
                {
                    cross_count = 0;
                    is_on_cross = 0;
                    last_error  = 0;
                    integral    = 0;
                }
            }
        }
        SET_BIT(EXTI->PR, EXTI_PR_PR13);
    }
}

uint8_t get_button_permission(void)
{
    return button_permission;
}

void TIM2_IRQHandler(void)
{
    if (READ_BIT(TIM2->SR, TIM_SR_UIF))
    {
        CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
        tim2_flag = 1;

        if (button_permission == 0)
        {
            left_motor(0);
            right_motor(0);
            return;
        }

        uint8_t s_left   = read_right_sensor();
        uint8_t s_middle = read_middle_sensor();
        uint8_t s_right  = read_left_sensor();

        /* === 1. ПЕРЕКРЁСТКИ И ФИНИШ === */
        if (s_left && s_right)
        {
            if (is_on_cross == 0)
            {
                cross_count++;
                is_on_cross = 1;
            }

            if (cross_count >= 4)
            {
                left_motor(0);
                right_motor(0);
                button_permission = 0;
                return;
            }

            /* Пробиваем перекрёсток прямо, регулятор обнуляем */
            last_error = 0;
            integral   = 0;
            left_motor(BASE_SPEED);
            right_motor(BASE_SPEED);
            return;
        }
        else if (!s_left && !s_right)
        {
            is_on_cross = 0;
        }

        /* === 2. ПОЛНАЯ ПОТЕРЯ ЛИНИИ (угол 90°, разрыв) === */
        uint8_t active = s_left + s_middle + s_right;
        if (active == 0)
        {
            /* Доворачиваем на месте в сторону последнего известного отклонения */
            if (last_error < 0)        { left_motor(-TURN_SPEED); right_motor(TURN_SPEED);  }
            else if (last_error > 0)   { left_motor(TURN_SPEED);  right_motor(-TURN_SPEED); }
            else                       { left_motor(0);           right_motor(0);           }
            return;
        }

        /* === 3. ПИД-РЕГУЛЯТОР ПО ПОЛОЖЕНИЮ ЛИНИИ === */
        /* Взвешенный "центр тяжести" линии: левый = -2, центр = 0, правый = +2 */
        int32_t weighted = (int32_t)(-2) * s_left + (int32_t)(2) * s_right;
        int32_t error    = weighted / (int32_t)active;   /* -2, -1, 0, +1, +2 */

        integral += error;
        if (integral >  I_LIMIT) integral =  I_LIMIT;     /* анти-windup */
        if (integral < -I_LIMIT) integral = -I_LIMIT;

        int32_t derivative = error - last_error;
        int32_t u = KP * error + KI * integral + KD * derivative;
        last_error = error;

        /* Симметричный доворот вокруг базовой скорости */
        int32_t l = BASE_SPEED + u;
        int32_t r = BASE_SPEED - u;

        /* Ограничение (по линии назад не сдаём — иначе сорвёшься с трассы) */
        if (l > MAX_SPEED) l = MAX_SPEED;
        if (l < 0)         l = 0;
        if (r > MAX_SPEED) r = MAX_SPEED;
        if (r < 0)         r = 0;

        left_motor((int16_t)l);
        right_motor((int16_t)r);
    }
}

uint8_t get_tim2_flag(void)
{
    return tim2_flag;
}

void set_tim2_flag(uint8_t flag)
{
    tim2_flag = flag;
}