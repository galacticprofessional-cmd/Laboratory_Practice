#include "stdint.h"
#include "init_LB2.h"
#include "init_LB1.h"

#define LED_COUNT           6U
#define BTN_DEBOUNCE_MS     100U      // антидребезг
#define PRESS_2S_MS         2000U     // граница ~2 секунд
#define PRESS_4S_MS         4000U     // граница ~4 секунд

/* Частоты мигания:
 * 0: 0.3 Гц  → T ≈ 3.33 c → T/2 ≈ 1667 мс
 * 1: 1.5 Гц  → T ≈ 0.67 c → T/2 ≈ 333 мс
 * 2: 2.2 Гц  → T ≈ 0.45 c → T/2 ≈ 227 мс
 */
static const uint32_t g_blinkHalfPeriods[3] = {1667U, 333U, 227U};


volatile uint32_t g_ms = 0;

volatile uint32_t g_btn_last_irq_ms = 0;
volatile uint8_t  g_btn_press_event   = 0;   // фронт нажатия (0->1)
volatile uint8_t  g_btn_release_event = 0;   // фронт отпускания (1->0)

volatile uint8_t  g_btn_is_pressed = 0;
volatile uint32_t g_btn_press_start_ms = 0;

int      currentLed       = -1;  
uint8_t  blinkModeEnabled = 1;   
uint8_t  blinkFreqIndex   = 0;   
uint8_t  blinkLedIsOn     = 0;   
uint32_t lastBlinkTime_ms = 0;   


void SysTick_Handler(void)
{
    g_ms++;
}


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
    blinkLedIsOn     = 1;
    lastBlinkTime_ms = g_ms;   // начальная точка для мигания
}


void EXTI2_IRQHandler(void)
{
    if (READ_BIT(EXTI->PR, EXTI_PR_PR2) != 0U)
    {
        uint32_t now = g_ms;

        if ((now - g_btn_last_irq_ms) >= BTN_DEBOUNCE_MS)
        {
            g_btn_last_irq_ms = now;

            if (READ_BIT(GPIOD->IDR, GPIO_IDR_IDR_2) != 0U)
            {
                g_btn_press_event = 1U;
            }
            else
            {
                g_btn_release_event = 1U;
            }
        }

        SET_BIT(EXTI->PR, EXTI_PR_PR2);
    }
}


void GPIO_Init_LEDs(void)
{
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN);
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN);

    SET_BIT(GPIOF->MODER, GPIO_MODER_MODER13_0);
    SET_BIT(GPIOF->MODER, GPIO_MODER_MODER14_0);
    SET_BIT(GPIOF->MODER, GPIO_MODER_MODER15_0);

    SET_BIT(GPIOE->MODER, GPIO_MODER_MODER9_0);
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODER11_0);
    SET_BIT(GPIOE->MODER, GPIO_MODER_MODER13_0);

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

    CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

    uint32_t reload = SystemCoreClock / 1000U - 1U;

    WRITE_REG(SysTick->LOAD, reload);
    WRITE_REG(SysTick->VAL, 0U);

    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}


void Button_Process(void)
{
    uint32_t now = g_ms;

    if (g_btn_press_event)
    {
        g_btn_press_event   = 0U;
        g_btn_is_pressed    = 1U;
        g_btn_press_start_ms = now;
    }

    if (g_btn_release_event)
    {
        g_btn_release_event = 0U;

        if (g_btn_is_pressed)
        {
            g_btn_is_pressed = 0U;

            uint32_t press_time = now - g_btn_press_start_ms;

            if (press_time < PRESS_2S_MS)
            {
                LED_Next();
            }
            else if (press_time < PRESS_4S_MS)
            {
                if (blinkModeEnabled)
                {
                    blinkFreqIndex++;
                    if (blinkFreqIndex >= 3)
                        blinkFreqIndex = 0;

                    lastBlinkTime_ms = now;
                }
            }
            else
            {

                if (blinkModeEnabled)
                {
                    blinkModeEnabled = 0;

                    LED_AllOff();
                    if (currentLed >= 0)
                    {
                        LED_On_Index(currentLed);  
                        blinkLedIsOn = 1;
                    }
                }
                else
                {
                    blinkModeEnabled = 1;

                    if (currentLed >= 0)
                    {
                        LED_On_Index(currentLed);
                        blinkLedIsOn     = 1;
                        lastBlinkTime_ms = now;
                    }
                }
            }
        }
    }
}


void Blink_Process(void)
{
    if (!blinkModeEnabled)
        return;           

    if (currentLed < 0)
        return;       

    uint32_t now      = g_ms;
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


int main(void)
{
    GPIO_Init_LEDs();
    GPIO_Init_Button_PD2_Pulldown();
    EXTI_Init_PD2();
    SysTick_Init_1ms();

    while (1)
    {
        Button_Process();  
        Blink_Process();   
    }
}