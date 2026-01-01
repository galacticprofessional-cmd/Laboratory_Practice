#include "init_LB2_hw.h"

/* ===================== ТАБЛИЦА ЧАСТОТ ===================== */

static const uint32_t g_blinkHalfPeriods[3] = {1667U, 333U, 227U};

/* ===================== ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ===================== */

volatile uint32_t g_ms = 0;

volatile uint32_t g_btn_last_irq_ms    = 0;
volatile uint8_t  g_btn_press_event    = 0;
volatile uint8_t  g_btn_release_event  = 0;
volatile uint8_t  g_btn_is_pressed     = 0;
volatile uint32_t g_btn_press_start_ms = 0;

/* MCU Viewer */
volatile uint32_t mcu_btn_press_count    = 0;
volatile uint32_t mcu_action_short_count = 0;
volatile uint32_t mcu_action_2s_count    = 0;
volatile uint32_t mcu_action_4s_count    = 0;

/* LEDs */
volatile int      currentLed       = -1;
volatile uint8_t  blinkModeEnabled = 1;
volatile uint8_t  blinkFreqIndex   = 0;
volatile uint8_t  blinkLedIsOn     = 0;
volatile uint32_t lastBlinkTime_ms = 0;

/* ===================== SysTick ===================== */

void SysTick_Handler(void)
{
    g_ms++;
}

/* ===================== EXTI ===================== */

void EXTI2_IRQHandler(void)
{
    if (READ_BIT(EXTI->PR, EXTI_PR_PR2))
    {
        uint32_t now = g_ms;

        if ((now - g_btn_last_irq_ms) >= BTN_DEBOUNCE_MS)
        {
            g_btn_last_irq_ms = now;

            if (READ_BIT(GPIOD->IDR, GPIO_IDR_IDR_2))
            {
                g_btn_press_event = 1;
                mcu_btn_press_count++;
            }
            else
            {
                g_btn_release_event = 1;
            }
        }
        SET_BIT(EXTI->PR, EXTI_PR_PR2);
    }
}

/* ===================== LED ===================== */

void LED_On_Index(int n)
{
    switch (n)
    {
        case 0: SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS8);  break;
        case 1: SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS10); break;
        case 2: SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS4);  break;
        case 3: SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS5);  break;
        case 4: SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS3);  break;
        case 5: SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS10); break;
        case 6: SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5);  break;
    }
}

void LED_Off_Index(int n)
{
    switch (n)
    {
        case 0: SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR8);  break;
        case 1: SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR10); break;
        case 2: SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR4);  break;
        case 3: SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR5);  break;
        case 4: SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR3);  break;
        case 5: SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR10); break;
        case 6: SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5);  break;
    }
}

void LED_AllOff(void)
{
    SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR8 | GPIO_BSRR_BR10 | GPIO_BSRR_BR5);
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR3 | GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BR10);
}

void LED_Next(void)
{
    if (currentLed >= 0)
        LED_Off_Index(currentLed);

    currentLed = (currentLed + 1) % LED_COUNT;

    LED_On_Index(currentLed);
    blinkLedIsOn = 1;
    lastBlinkTime_ms = g_ms;
}

/* ===================== INIT ===================== */

void GPIO_Init_LEDs(void)
{
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);

    SET_BIT(GPIOA->MODER, GPIO_MODER_MODER8_0 |
                          GPIO_MODER_MODER10_0|
                          GPIO_MODER_MODER5_0);

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODER3_0  |
                          GPIO_MODER_MODER4_0 |
                          GPIO_MODER_MODER5_0 |
                          GPIO_MODER_MODER10_0);

    // OTYPER не настраивается, потому что по умолчанию GPIO работает в push-pull,
    // а для обычного светодиода открытый сток (open-drain) не требуется.

    // OSPEEDR не настраивается, потому что скорость вывода не влияет на работу светодиода
    // и по умолчанию стоит безопасное значение с минимальными помехами.

    LED_AllOff();
}

void GPIO_Init_Button_PD2_Pulldown(void)
{
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);

    CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODER2);
    CLEAR_BIT(GPIOD->PUPDR, GPIO_PUPDR_PUPDR2);
    SET_BIT(GPIOD->PUPDR, GPIO_PUPDR_PUPDR2_1);
}

void EXTI_Init_PD2(void)
{
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);

    MODIFY_REG(SYSCFG->EXTICR[0],
               SYSCFG_EXTICR1_EXTI2,
               SYSCFG_EXTICR1_EXTI2_PD);

    SET_BIT(EXTI->IMR, EXTI_IMR_MR2);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR2);
    SET_BIT(EXTI->FTSR, EXTI_FTSR_TR2);
    SET_BIT(EXTI->PR, EXTI_PR_PR2);

    NVIC_EnableIRQ(EXTI2_IRQn);
}

void SysTick_Init_1ms(void)
{
    SystemCoreClockUpdate();

    SysTick->LOAD = SystemCoreClock / 1000 - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
}

/* ===================== ЛОГИКА ===================== */

void Button_Process(void)
{
    uint32_t now = g_ms;

    if (g_btn_press_event)
    {
        g_btn_press_event = 0;
        g_btn_is_pressed = 1;
        g_btn_press_start_ms = now;
    }

    if (g_btn_release_event)
    {
        g_btn_release_event = 0;
        g_btn_is_pressed = 0;

        uint32_t dt = now - g_btn_press_start_ms;

        if (dt < PRESS_2S_MS)
        {
            mcu_action_short_count++;
            LED_Next();
        }
        else if (dt < PRESS_4S_MS && blinkModeEnabled)
        {
            mcu_action_2s_count++;
            blinkFreqIndex = (blinkFreqIndex + 1) % 3;
            lastBlinkTime_ms = now;
        }
        else
        {
            mcu_action_4s_count++;
            blinkModeEnabled ^= 1;
            lastBlinkTime_ms = now;
        }
    }
}

void Blink_Process(void)
{
    if (!blinkModeEnabled || currentLed < 0)
        return;

    uint32_t now = g_ms;
    uint32_t period = g_blinkHalfPeriods[blinkFreqIndex];

    if ((now - lastBlinkTime_ms) >= period)
    {
        lastBlinkTime_ms = now;
        blinkLedIsOn ^= 1;

        if (blinkLedIsOn)
            LED_On_Index(currentLed);
        else
            LED_Off_Index(currentLed);
    }
}
