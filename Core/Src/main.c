#include "stdint.h"
#include "init_LB2.h"
#include "init_LB1.h"

/* ------------------- Константы ------------------- */

#define LED_COUNT           6U
#define BTN_DEBOUNCE_MS     100U      // антидребезг
#define PRESS_2S_MS         2000U     // граница 2 секунды
#define PRESS_4S_MS         4000U     // для 3-й функции, позже

/* Частоты мигания:
 * 0: 0.3 Гц  → T ≈ 3.33 c → T/2 ≈ 1667 мс
 * 1: 1.5 Гц  → T ≈ 0.67 c → T/2 ≈ 333 мс
 * 2: 2.2 Гц  → T ≈ 0.45 c → T/2 ≈ 227 мс
 */
static const uint32_t g_blinkHalfPeriods[3] = {1667U, 333U, 227U};

/* ------------------- Глобальные переменные ------------------- */

/* Время от SysTick в мс */
volatile uint32_t g_ms = 0;

/* События от EXTI по кнопке PD2 */
volatile uint32_t g_btn_last_irq_ms = 0;
volatile uint8_t  g_btn_press_event  = 0;   // фронт нажатия (0->1)
volatile uint8_t  g_btn_release_event = 0;  // фронт отпускания (1->0)

/* Состояние нажатия */
volatile uint8_t  g_btn_is_pressed = 0;
volatile uint32_t g_btn_press_start_ms = 0;

/* Светодиоды */
int currentLed = -1;            // 0..5, -1 = ещё не выбран
uint8_t blinkModeEnabled = 1;   // пока всегда мигаем (3-ю функцию добавим позже)
uint8_t blinkFreqIndex = 0;     // 0..2
uint8_t blinkLedIsOn = 0;       // 0=выкл, 1=горит
uint32_t lastBlinkTime_ms = 0;  // время последнего переключения

/* ------------------- SysTick ------------------- */

void SysTick_Handler(void)
{
    g_ms++;
}

/* ------------------- Светодиоды ------------------- */
/* Светодиоды: PF13, PE9, PE11, PF14, PE13, PF15 */

void LED_On_Index(int n)
{
    switch (n)
    {
    case 0: SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS13); break;
    case 1: SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS9);  break;
    case 2: SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS11); break;
    case 3: SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS14); break;
    case 4: SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS13); break;
    case 5: SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS15); break;
    default: break;
    }
}

void LED_Off_Index(int n)
{
    switch (n)
    {
    case 0: SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR13); break;
    case 1: SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR9);  break;
    case 2: SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR11); break;
    case 3: SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR14); break;
    case 4: SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR13); break;
    case 5: SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR15); break;
    default: break;
    }
}

void LED_AllOff(void)
{
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR13 | GPIO_BSRR_BR14 | GPIO_BSRR_BR15);
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR9  | GPIO_BSRR_BR11 | GPIO_BSRR_BR13);
}

/* 1-я функция: следующий светодиод, предыдущий гаснет, циклично 0..5 */
void LED_Next(void)
{
    if (currentLed >= 0)
    {
        LED_Off_Index(currentLed);
    }

    currentLed++;
    if (currentLed >= (int)LED_COUNT)
    {
        currentLed = 0;
    }

    LED_On_Index(currentLed);
    blinkLedIsOn = 1;
    lastBlinkTime_ms = g_ms;      // старт отсчёта мигания для нового светодиода
}

/* ------------------- EXTI2 (PD2, pull-down) ------------------- */

void EXTI2_IRQHandler(void)
{
    /* Проверяем, что сработала линия 2 */
    if (READ_BIT(EXTI->PR, EXTI_PR_PR2) != 0U)
    {
        uint32_t now = g_ms;

        /* Антидребезг по времени */
        if ((now - g_btn_last_irq_ms) >= BTN_DEBOUNCE_MS)
        {
            g_btn_last_irq_ms = now;

            /* Смотрим текущее состояние PD2:
             * 0 → отпущена, 1 → нажата.
             * Pull-down: в покое 0, при нажатии 1.
             */
            if (READ_BIT(GPIOD->IDR, GPIO_IDR_IDR_2) != 0U)
            {
                /* фронт 0->1: кнопку НАЖАЛИ */
                g_btn_press_event = 1U;
            }
            else
            {
                /* фронт 1->0: кнопку ОТПУСТИЛИ */
                g_btn_release_event = 1U;
            }
        }

        /* Сбрасываем флаг прерывания по линии 2 */
        SET_BIT(EXTI->PR, EXTI_PR_PR2);
    }
}

/* ------------------- ИНИЦИАЛИЗАЦИЯ ------------------- */

void GPIO_Init_LEDs(void)
{
    /* Тактирование портов E и F */
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN);
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);

    /* PF13, PF14, PF15 — выход */
    SET_BIT(GPIOF->MODER, GPIO_MODER_MODER13_0);
    SET_BIT(GPIOF->MODER, GPIO_MODER_MODER14_0);
    SET_BIT(GPIOF->MODER, GPIO_MODER_MODER15_0);

    /* PE9, PE11, PE13 — выход */
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODER9_0);
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODER11_0);
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODER13_0);

    LED_AllOff();
}

void GPIO_Init_Button_PD2_Pulldown(void)
{
    /* Тактирование порта D */
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);

    /* PD2 как вход */
    CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODER2);

    /* Pull-down: PUPDR2 = 10b */
    CLEAR_BIT(GPIOD->PUPDR, GPIO_PUPDR_PUPDR2);
    SET_BIT(GPIOD->PUPDR, GPIO_PUPDR_PUPDR2_1);
}

void EXTI_Init_PD2(void)
{
    /* Включаем SYSCFG */
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);

    /* Привязка EXTI2 к PD2 */
    MODIFY_REG(SYSCFG->EXTICR[0],
               SYSCFG_EXTICR1_EXTI2,
               SYSCFG_EXTICR1_EXTI2_PD);

    /* Линия 2 не маскирована */
    SET_BIT(EXTI->IMR, EXTI_IMR_MR2);

    /* Срабатывание по обоим фронтам: 0->1 и 1->0 */
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR2);  // rising
    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR2);  // falling

    /* Очистить возможный старый флаг */
    SET_BIT(EXTI->PR, EXTI_PR_PR2);

    /* Разрешаем прерывание EXTI2 в NVIC */
    NVIC_EnableIRQ(EXTI2_IRQn);
}

void SysTick_Init_1ms(void)
{
    /* Обновляем SystemCoreClock на всякий случай */
    SystemCoreClockUpdate();

    /* Останавливаем SysTick */
    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

    uint32_t reload = SystemCoreClock / 1000U - 1U;

    WRITE_REG(SysTick->LOAD, reload);
    WRITE_REG(SysTick->VAL, 0U);

    /* Источник – такт ядра, прерывания включены, таймер включен */
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}

/* ------------------- ОБРАБОТКА КНОПКИ (1-я + 2-я функции) ------------------- */

void Button_Process(void)
{
    uint32_t now = g_ms;

    /* Событие: кнопку нажали */
    if (g_btn_press_event)
    {
        g_btn_press_event = 0U;
        g_btn_is_pressed = 1U;
        g_btn_press_start_ms = now;
    }

    /* Событие: кнопку отпустили */
    if (g_btn_release_event)
    {
        g_btn_release_event = 0U;

        if (g_btn_is_pressed)
        {
            g_btn_is_pressed = 0U;

            uint32_t press_time = now - g_btn_press_start_ms;

            if (press_time < PRESS_2S_MS)
            {
                /* 1-я функция: кратковременное нажатие → следующий светодиод */
                LED_Next();
            }
            else if (press_time < PRESS_4S_MS)
            {
                /* 2-я функция: удержание около 2 секунд → смена частоты мигания */
                if (blinkModeEnabled)
                {
                    blinkFreqIndex++;
                    if (blinkFreqIndex >= 3)
                        blinkFreqIndex = 0;

                    /* Чтобы смена частоты не давала "скачок" по фазе */
                    lastBlinkTime_ms = now;
                }
            }
            else
            {
                /* Здесь будет 3-я функция (4 секунды), пока ничего не делаем */
            }
        }
    }
}

/* ------------------- МИГАНИЕ (только если включен режим мигания) ------------------- */

void Blink_Process(void)
{
    if (!blinkModeEnabled)
        return;

    if (currentLed < 0)
        return;  // ещё ни один светодиод не выбран

    uint32_t now = g_ms;
    uint32_t interval = g_blinkHalfPeriods[blinkFreqIndex];

    if ((now - lastBlinkTime_ms) >= interval)
    {
        lastBlinkTime_ms = now;

        if (blinkLedIsOn)
        {
            LED_Off_Index(currentLed);
            blinkLedIsOn = 0;
        }
        else
        {
            LED_On_Index(currentLed);
            blinkLedIsOn = 1;
        }
    }
}

/* ------------------- MAIN ------------------- */

int main(void)
{
    GPIO_Init_LEDs();
    GPIO_Init_Button_PD2_Pulldown();
    EXTI_Init_PD2();
    SysTick_Init_1ms();

    while (1)
    {
        Button_Process();  // обрабатываем 1-ю и 2-ю функции по кнопке
        Blink_Process();   // мигаем с текущей частотой
    }
}