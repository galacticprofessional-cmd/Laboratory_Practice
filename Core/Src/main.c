#include "stdint.h"
#include "init_LB2.h"
#include "init_LB1.h"


/* ---------------- Глобальные переменные ---------------- */

volatile uint8_t button_short_press_flag = 0;  // флаг: было короткое нажатие

int currentLed = -1;   // -1 = пока ни один не горит

/* ---------------- Работа со светодиодами ---------------- */
/* Светодиоды: PF13, PE9, PE11, PF14, PE13, PF15 (как в ЛР1) */

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
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR13);
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR14);
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR15);

    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR9);
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR11);
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR13);
}

/* Каждое вызов — следующая лампа, предыдущая гаснет, цикл 0..5 */
void LED_Next(void)
{
    if (currentLed >= 0)
    {
        LED_Off_Index(currentLed);
    }

    currentLed++;
    if (currentLed >= 6)
    {
        currentLed = 0;
    }

    LED_On_Index(currentLed);
}

/* ---------------- Обработчик прерывания EXTI2 (кнопка PD2) ---------------- */

void EXTI2_IRQHandler(void)
{
    /* Проверяем, что действительно сработала линия 2 */
    if (READ_BIT(EXTI->PR, EXTI_PR_PR2) != 0U)
    {
        /* Небольшая задержка для антидребезга (в стиле ЛР1) */
        for (volatile int i = 0; i < 50000; i++) {}

        /* Ещё раз читаем вход PD2 – если всё ещё 1, значит кнопка нажата */
        if (READ_BIT(GPIOD->IDR, GPIO_IDR_IDR_2) != 0U)
        {
            /* Ждём отпускания, чтобы не ловить многократно */
            while (READ_BIT(GPIOD->IDR, GPIO_IDR_IDR_2) != 0U) {}

            /* Сообщаем main(), что было короткое нажатие */
            button_short_press_flag = 1;
        }

        /* Сбрасываем флаг прерывания по линии 2 */
        SET_BIT(EXTI->PR, EXTI_PR_PR2);
    }
}

/* ---------------- Инициализация GPIO для светодиодов ---------------- */

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

    /* На всякий случай выключим все светодиоды */
    LED_AllOff();
}

/* ---------------- Инициализация кнопки PD2 (вход, pull-down) ---------------- */

void GPIO_Init_Button_PD2(void)
{
    /* Тактирование порта D */
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);

    /* PD2 как вход: MODER2 = 00 */
    CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODER2);

    /* Pull-down: PUPDR2 = 10b */
    CLEAR_BIT(GPIOD->PUPDR, GPIO_PUPDR_PUPDR2);
    SET_BIT(GPIOD->PUPDR, GPIO_PUPDR_PUPDR2_1);

    /* На входе OTYPER и OSPEEDR можно не трогать, они важны для выхода */
}

/* ---------------- Инициализация EXTI2 на PD2 ---------------- */

void EXTI_Init_PD2(void)
{
    /* Включаем тактирование SYSCFG */
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);

    /* Привязать EXTI2 к порту D:
       SYSCFG_EXTICR1, поле EXTI2[3:0] */
    MODIFY_REG(SYSCFG->EXTICR[0],
               SYSCFG_EXTICR1_EXTI2,
               SYSCFG_EXTICR1_EXTI2_PD);

    /* Разрешаем прерывание от линии 2 */
    SET_BIT(EXTI->IMR, EXTI_IMR_MR2);

    /* Срабатывание по фронту 0→1 (нажатие, т.к. pull-down) */
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR2);
    CLEAR_BIT(EXTI->FTSR, EXTI_FTSR_TR2);

    /* Очистим возможный старый флаг */
    SET_BIT(EXTI->PR, EXTI_PR_PR2);

    /* Разрешаем прерывание EXTI2 в NVIC */
    NVIC_EnableIRQ(EXTI2_IRQn);
}

/* ---------------- MAIN ---------------- */

int main(void)
{
    GPIO_Init_LEDs();
    GPIO_Init_Button_PD2();
    EXTI_Init_PD2();

    while (1)
    {
        /* Если обработчик прерывания сообщил о коротком нажатии */
        if (button_short_press_flag)
        {
            button_short_press_flag = 0;
            LED_Next();   // реализуем 1-ю функцию задания
        }
    }
}