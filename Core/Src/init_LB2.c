#include "init_LB2.h"

/* ===================== GLOBALS ===================== */
volatile uint8_t sensor_left;
volatile uint8_t sensor_center;
volatile uint8_t sensor_right;

volatile int8_t line_error;

/* ===================== RCC ===================== */
void RCC_Init(void)
{
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);
    CLEAR_REG(RCC->CFGR);

    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI);

    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY));

    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

    SET_BIT(RCC->CR, RCC_CR_HSEON);
    while (!(RCC->CR & RCC_CR_HSERDY));

    MODIFY_REG(FLASH->ACR,
               FLASH_ACR_LATENCY_Msk,
               FLASH_ACR_LATENCY_5WS);

    CLEAR_REG(RCC->PLLCFGR);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_Msk, 4U << RCC_PLLCFGR_PLLM_Pos);
    MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, 100U << RCC_PLLCFGR_PLLN_Pos);
    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk);

    MODIFY_REG(RCC->CFGR,
               RCC_CFGR_HPRE_Msk  |
               RCC_CFGR_PPRE1_Msk |
               RCC_CFGR_PPRE2_Msk,
               RCC_CFGR_HPRE_DIV1 |
               RCC_CFGR_PPRE1_DIV2 |
               RCC_CFGR_PPRE2_DIV1);

    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while (!(RCC->CR & RCC_CR_PLLRDY));

    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_PLL);
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL);

    SET_BIT(RCC->CR, RCC_CR_CSSON);
}

/* ===================== GPIO ===================== */
void GPIO_Init(void)
{
    SET_BIT(RCC->AHB1ENR,
            RCC_AHB1ENR_GPIOAEN |
            RCC_AHB1ENR_GPIOBEN |
            RCC_AHB1ENR_GPIOCEN);

    /* Motor direction pins */
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER0_Msk, GPIO_MODER_MODER0_0);
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER1_Msk, GPIO_MODER_MODER1_0);
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER4_Msk, GPIO_MODER_MODER4_0);
    MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER0_Msk, GPIO_MODER_MODER0_0);

    /* PWM pins PC6, PC7 */
    MODIFY_REG(GPIOC->MODER,
               GPIO_MODER_MODER6_Msk | GPIO_MODER_MODER7_Msk,
               GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);

    MODIFY_REG(GPIOC->AFR[0],
               GPIO_AFRL_AFSEL6_Msk | GPIO_AFRL_AFSEL7_Msk,
               (2U << GPIO_AFRL_AFSEL6_Pos) |
               (2U << GPIO_AFRL_AFSEL7_Pos));

    MODIFY_REG(GPIOC->OSPEEDR,
               GPIO_OSPEEDR_OSPEED6_Msk | GPIO_OSPEEDR_OSPEED7_Msk,
               GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);
}

/* ===================== TIM3 PWM ===================== */
void TIM3_PWM_Init(void)
{
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);

    TIM3->PSC = 99;
    TIM3->ARR = PWM_MAX;

    MODIFY_REG(TIM3->CCMR1,
               TIM_CCMR1_OC1M_Msk | TIM_CCMR1_OC2M_Msk,
               TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |
               TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);

    SET_BIT(TIM3->CCMR1, TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
    SET_BIT(TIM3->CCER, TIM_CCER_CC1E | TIM_CCER_CC2E);

    TIM3->CCR1 = 200;
    TIM3->CCR2 = 200;

    SET_BIT(TIM3->CR1, TIM_CR1_ARPE | TIM_CR1_CEN);
}

/* ===================== LINE SENSORS (DO) ===================== */
void LineSensors_GPIO_Init(void)
{
    CLEAR_BIT(GPIOA->MODER,
              GPIO_MODER_MODER6_Msk |
              GPIO_MODER_MODER7_Msk);

    CLEAR_BIT(GPIOB->MODER,
              GPIO_MODER_MODER1_Msk);

    CLEAR_BIT(GPIOA->PUPDR,
              GPIO_PUPDR_PUPD6_Msk |
              GPIO_PUPDR_PUPD7_Msk);

    CLEAR_BIT(GPIOB->PUPDR,
              GPIO_PUPDR_PUPD1_Msk);
}

void LineSensors_Read(void)
{
    sensor_left   = (GPIOA->IDR & GPIO_IDR_ID6) ? 1 : 0;
    sensor_center = (GPIOA->IDR & GPIO_IDR_ID7) ? 1 : 0;
    sensor_right  = (GPIOB->IDR & GPIO_IDR_ID1) ? 1 : 0;
}

void Line_CalcError(void)
{
    if (sensor_center == 0)
        line_error = 0;
    else if (sensor_left == 0)
        line_error = -1;
    else if (sensor_right == 0)
        line_error = 1;
    else
        line_error = 0;
}

/* ===================== MOTORS ===================== */
void Motor_SetSpeed_Left(int16_t speed)
{
    if (speed >= 0)
    {
        SET_BIT(GPIOA->ODR, GPIO_ODR_OD0);
        CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD1);
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
        SET_BIT(GPIOA->ODR, GPIO_ODR_OD4);
        CLEAR_BIT(GPIOB->ODR, GPIO_ODR_OD0);
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

void Robot_Move(void)
{
    int16_t correction = KP * line_error;

    Motor_SetSpeed_Left(BASE_SPEED - correction);
    Motor_SetSpeed_Right(BASE_SPEED + correction);
}
