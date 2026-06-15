#include "init.h"

/**
 * @brief Инициализация портов для датчиков, кнопки, светодиода и двигателей
 * @note
 *  Датчики линии: PA0, PA1, PA4
 *  Направление моторов: PA5, PA6, PA7, PB6
 *  PWM: PB3, PB10
 *  Кнопка: PC13
 *  Светодиод: PC2
 */
void GPIO_init(void)
{
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);

    /* Датчики линии */
    CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER4);
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPDR0 | GPIO_PUPDR_PUPDR1 | GPIO_PUPDR_PUPDR4);

    /* Выходы управления направлением моторов */
    CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER6);

    SET_BIT(GPIOA->MODER, GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODER6_0);

    /* Начальное состояние: моторы выключены */
    WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BR5 | GPIO_BSRR_BR6 | GPIO_BSRR_BR7);
    WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BR6);

    /* PWM-пины: PB3 и PB10 -> Alternate Function TIM2 */
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER3 | GPIO_MODER_MODER10);
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODER3_1 | GPIO_MODER_MODER10_1);

    MODIFY_REG(GPIOB->AFR[0], 0xFU << (3U * 4U), 1U << (3U * 4U));
    MODIFY_REG(GPIOB->AFR[1], 0xFU << ((10U - 8U) * 4U), 1U << ((10U - 8U) * 4U));

    /* Кнопка PC13 */
    CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODER13);
    MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPDR13, GPIO_PUPDR_PUPDR13_0); /* pull-up */

    /* Светодиод PC2 */
    CLEAR_BIT(GPIOC->MODER, GPIO_MODER_MODER2);
    SET_BIT(GPIOC->MODER, GPIO_MODER_MODER2_0);
    CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR2);
    WRITE_REG(GPIOC->BSRR, GPIO_BSRR_BR_2);
}

/**
 * @brief Настройка тактирования контроллера на частоту 84 МГц
 * @retval None
 */
void RCC_init(void)
{
    SET_BIT(RCC->CR, RCC_CR_HSEON);

    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0)
        ;

    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
    MODIFY_REG(PWR->CR, PWR_CR_VOS, PWR_CR_VOS_1);

    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2WS);

    /* (8 / 4) * 84 / 2 = 84 MHz */
    MODIFY_REG(RCC->PLLCFGR,
               RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLSRC,
               (4U << RCC_PLLCFGR_PLLM_Pos) |
                   (84U << RCC_PLLCFGR_PLLN_Pos) |
                   (0U << RCC_PLLCFGR_PLLP_Pos) |
                   (4U << RCC_PLLCFGR_PLLQ_Pos) |
                   RCC_PLLCFGR_PLLSRC_HSE);

    SET_BIT(RCC->CR, RCC_CR_PLLON);

    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0)
        ;

    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);

    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);

    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
        ;

    SystemCoreClockUpdate();
}

/**
 * @brief Настройка TIMER2 для PWM
 * @note Частота PWM 1 кГц. Частота таймера 1 МГц
 * @retval None
 */
void TIMER2_init(void)
{
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);

    WRITE_REG(TIM2->PSC, 84U - 1U);
    WRITE_REG(TIM2->ARR, 1000U - 1U);

    WRITE_REG(TIM2->CCR2, 0U);
    WRITE_REG(TIM2->CCR3, 0U);

    MODIFY_REG(TIM2->CCMR1, TIM_CCMR1_OC2M | TIM_CCMR1_OC2PE,
               TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE);

    MODIFY_REG(TIM2->CCMR2, TIM_CCMR2_OC3M | TIM_CCMR2_OC3PE,
               TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE);

    SET_BIT(TIM2->CCER, TIM_CCER_CC2E | TIM_CCER_CC3E);

    SET_BIT(TIM2->CR1, TIM_CR1_ARPE);

    WRITE_REG(TIM2->EGR, TIM_EGR_UG);

    SET_BIT(TIM2->DIER, TIM_DIER_UIE);
    NVIC_SetPriority(TIM2_IRQn, 2);
    NVIC_EnableIRQ(TIM2_IRQn);

    SET_BIT(TIM2->CR1, TIM_CR1_CEN);
}

/**
 * @brief Настройка системного таймера
 */
void SysTick_init(void)
{
    WRITE_REG(SysTick->LOAD, (SystemCoreClock / 1000U) - 1U);
    WRITE_REG(SysTick->VAL, 0U);
    WRITE_REG(SysTick->CTRL, 0U);

    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}