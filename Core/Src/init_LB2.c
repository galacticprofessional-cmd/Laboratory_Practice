#include "init_LB2.h"

#define PWM_MAX 100

void GPIO_Init(void){

    /*------------------------------------------------------------*/
    /* Enable GPIO clocks                                         */
    /*------------------------------------------------------------*/
    SET_BIT(RCC->AHB1ENR,
            RCC_AHB1ENR_GPIOAEN |
            RCC_AHB1ENR_GPIOBEN |
            RCC_AHB1ENR_GPIOCEN);

    /*------------------------------------------------------------*/
    /* Direction pins: PA0, PA1, PA4, PB0                         */
    /* Output, push-pull                                          */
    /*------------------------------------------------------------*/

    /* PA0 */
    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODER0_Msk,
               GPIO_MODER_MODER0_0);

    /* PA1 */
    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODER1_Msk,
               GPIO_MODER_MODER1_0);

    /* PA4 */
    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODER4_Msk,
               GPIO_MODER_MODER4_0);

    /* PB0 */
    MODIFY_REG(GPIOB->MODER,
               GPIO_MODER_MODER0_Msk,
               GPIO_MODER_MODER0_0);

    /*------------------------------------------------------------*/
    /* PWM pins: PC6 (TIM3_CH1), PC7 (TIM3_CH2)                   */
    /* Alternate Function                                         */
    /*------------------------------------------------------------*/

    /* PC6 */
    MODIFY_REG(GPIOC->MODER,
               GPIO_MODER_MODER6_Msk,
               GPIO_MODER_MODER6_1);

    /* PC7 */
    MODIFY_REG(GPIOC->MODER,
               GPIO_MODER_MODER7_Msk,
               GPIO_MODER_MODER7_1);

    /* Set AF2 (TIM3) for PC6 and PC7 */
    MODIFY_REG(GPIOC->AFR[0],
               GPIO_AFRL_AFSEL6_Msk,
               (2U << GPIO_AFRL_AFSEL6_Pos));

    MODIFY_REG(GPIOC->AFR[0],
               GPIO_AFRL_AFSEL7_Msk,
               (2U << GPIO_AFRL_AFSEL7_Pos));

    /* High speed for PWM pins */
    MODIFY_REG(GPIOC->OSPEEDR,
               GPIO_OSPEEDR_OSPEED6_Msk |
               GPIO_OSPEEDR_OSPEED7_Msk,
               GPIO_OSPEEDER_OSPEEDR6 |
               GPIO_OSPEEDER_OSPEEDR7);

    /* No pull-up / pull-down */
    CLEAR_BIT(GPIOC->PUPDR,
              GPIO_PUPDR_PUPD6_Msk |
              GPIO_PUPDR_PUPD7_Msk);

    SET_BIT(GPIOC->MODER, GPIO_MODER_MODER9_1);     
    SET_BIT(GPIOC->OSPEEDR,GPIO_OSPEEDER_OSPEEDR9);  
    MODIFY_REG(GPIOC->AFR[1], GPIO_AFRH_AFSEL9_Msk, 0x00UL);

}


void RCC_Init(void)
{
    /*--------------------------------------------------------------------*/
    /* 1. Reset clock configuration and switch to HSI                     */
    /*--------------------------------------------------------------------*/

    /* Set HSI trimming to default */
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);

    /* Reset CFGR register */
    CLEAR_REG(RCC->CFGR);

    /* Wait until HSI is used as system clock */
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI);

    /*--------------------------------------------------------------------*/
    /* 2. Disable PLL, HSE and CSS before reconfiguration                 */
    /*--------------------------------------------------------------------*/

    /* Disable PLL */
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET);

    /* Disable HSE and CSS */
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET);

    /* Disable HSE bypass */
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

    /*--------------------------------------------------------------------*/
    /* 3. Enable HSE                                                      */
    /*--------------------------------------------------------------------*/

    SET_BIT(RCC->CR, RCC_CR_HSEON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET);

    /*--------------------------------------------------------------------*/
    /* 4. Configure Flash latency BEFORE increasing frequency             */
    /*--------------------------------------------------------------------*/

    MODIFY_REG(FLASH->ACR,
               FLASH_ACR_LATENCY_Msk,
               FLASH_ACR_LATENCY_5WS);

    /*--------------------------------------------------------------------*/
    /* 5. Configure PLL                                                   */
    /*--------------------------------------------------------------------*/

    /* Clear PLL configuration register */
    CLEAR_REG(RCC->PLLCFGR);

    /* Select HSE as PLL source */
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);

    /* PLLM = 4  (8 MHz / 4 = 2 MHz) */
    MODIFY_REG(RCC->PLLCFGR,
               RCC_PLLCFGR_PLLM_Msk,
               (4U << RCC_PLLCFGR_PLLM_Pos));

    /* PLLN = 100 (2 MHz * 100 = 200 MHz) */
    MODIFY_REG(RCC->PLLCFGR,
               RCC_PLLCFGR_PLLN_Msk,
               (100U << RCC_PLLCFGR_PLLN_Pos));

    /* PLLP = 2 (200 MHz / 2 = 100 MHz SYSCLK) */
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk);

    /*--------------------------------------------------------------------*/
    /* 6. Configure AHB and APB prescalers                                */
    /*--------------------------------------------------------------------*/

    MODIFY_REG(RCC->CFGR,
               RCC_CFGR_HPRE_Msk  |
               RCC_CFGR_PPRE1_Msk |
               RCC_CFGR_PPRE2_Msk,
               RCC_CFGR_HPRE_DIV1 |
               RCC_CFGR_PPRE1_DIV2 |  /* APB1 = 50 MHz */
               RCC_CFGR_PPRE2_DIV1);  /* APB2 = 100 MHz */

    /*--------------------------------------------------------------------*/
    /* 7. Enable PLL                                                       */
    /*--------------------------------------------------------------------*/

    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == RESET);

    /*--------------------------------------------------------------------*/
    /* 8. Switch system clock to PLL                                       */
    /*--------------------------------------------------------------------*/

    MODIFY_REG(RCC->CFGR,
               RCC_CFGR_SW_Msk,
               RCC_CFGR_SW_PLL);

    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL);

    /*--------------------------------------------------------------------*/
    /* 9. Enable Clock Security System                                     */
    /*--------------------------------------------------------------------*/

    SET_BIT(RCC->CR, RCC_CR_CSSON);
}


void TIM3_PWM_Init(void)
{
    /*------------------------------------------------------------*/
    /* Enable TIM3 clock                                          */
    /*------------------------------------------------------------*/
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);

    /*------------------------------------------------------------*/
    /* Timer base configuration                                   */
    /*------------------------------------------------------------*/
    TIM3->PSC = 99;          /* 100 MHz / (99+1) = 1 MHz */
    TIM3->ARR = PWM_MAX;     /* 1 MHz / 100 = 10 kHz */

    /*------------------------------------------------------------*/
    /* PWM Mode 1 on CH1 and CH2                                  */
    /*------------------------------------------------------------*/
    MODIFY_REG(TIM3->CCMR1,
               TIM_CCMR1_OC1M_Msk |
               TIM_CCMR1_OC2M_Msk,
               TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |
               TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);

    /* Enable preload */
    SET_BIT(TIM3->CCMR1,
            TIM_CCMR1_OC1PE |
            TIM_CCMR1_OC2PE);

    /*------------------------------------------------------------*/
    /* Enable output compare on CH1 and CH2                       */
    /*------------------------------------------------------------*/
    SET_BIT(TIM3->CCER,
            TIM_CCER_CC1E |
            TIM_CCER_CC2E);

    /* Initial duty cycle = 0 */
    TIM3->CCR1 = 0;
    TIM3->CCR2 = 0;

    /* Enable auto-reload preload */
    SET_BIT(TIM3->CR1, TIM_CR1_ARPE);

    /* Start timer */
    SET_BIT(TIM3->CR1, TIM_CR1_CEN);
}

void Motor_SetSpeed_Left(int16_t speed)
{
    if (speed >= 0)
    {
        SET_BIT(GPIOA->ODR, GPIO_ODR_OD0);   /* IN1 = 1 */
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD1); /* IN2 = 0 */
    }
    else
    {
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD0);
        SET_BIT(GPIOA->ODR, GPIO_ODR_OD1);
        speed = -speed;
    }

    if (speed > PWM_MAX) speed = PWM_MAX;
    TIM3->CCR1 = speed;
}


void Motor_SetSpeed_Right(int16_t speed)
{
    if (speed >= 0)
    {
        SET_BIT(GPIOA->ODR, GPIO_ODR_OD4);   /* IN3 = 1 */
        CLEAR_BIT(GPIOB->ODR, GPIO_ODR_OD0); /* IN4 = 0 */
    }
    else
    {
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD4);
        SET_BIT(GPIOB->ODR, GPIO_ODR_OD0);
        speed = -speed;
    }

    if (speed > PWM_MAX) speed = PWM_MAX;
    TIM3->CCR2 = speed;
}

// void SysTick_Init(void){
//     CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
//     SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
//     SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
//     MODIFY_REG(SysTick->LOAD, SysTick_LOAD_RELOAD_Msk, (180000-1) << SysTick_LOAD_RELOAD_Pos);
//     MODIFY_REG(SysTick->VAL, SysTick_VAL_CURRENT_Msk, (180000-1) << SysTick_VAL_CURRENT_Pos);
//     SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
// }