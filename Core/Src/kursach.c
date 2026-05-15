#include "kursach.h"

/* ===================== GLOBALS ===================== */
volatile uint16_t adc_left, adc_center, adc_right;

uint16_t min_left = 4095, max_left = 0;
uint16_t min_center = 4095, max_center = 0;
uint16_t min_right = 4095, max_right = 0;

uint16_t thr_left, thr_center, thr_right;

uint8_t sensor_left, sensor_center, sensor_right;

/* --- КАЛИБРОВКА --- */
uint8_t calib_stage = 0;
uint8_t sensor_id = 0;

/* --- NEW (added, not replacing anything) --- */
uint8_t calibration_busy = 0;
uint8_t active_sensor = 255;


/* ===================== GPIO ===================== */
void GPIO_Init(void)
{
    SET_BIT(RCC->AHB1ENR,
            RCC_AHB1ENR_GPIOAEN |
            RCC_AHB1ENR_GPIOBEN);

    /* RESET */
    CLEAR_BIT(GPIOA->ODR,
              GPIO_ODR_OD5 |
              GPIO_ODR_OD6 |
              GPIO_ODR_OD7 |
              GPIO_ODR_OD8);

    CLEAR_BIT(GPIOB->ODR, GPIO_ODR_OD6);

    /* MOTORS */
    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODER5_Msk |
               GPIO_MODER_MODER6_Msk |
               GPIO_MODER_MODER7_Msk,
               GPIO_MODER_MODER5_0 |
               GPIO_MODER_MODER6_0 |
               GPIO_MODER_MODER7_0);

    MODIFY_REG(GPIOB->MODER,
               GPIO_MODER_MODER6_Msk,
               GPIO_MODER_MODER6_0);

    /* LED PA8 */
    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODER8_Msk,
               GPIO_MODER_MODER8_0);

    /* --- UPDATED: 3 buttons --- */
    CLEAR_BIT(GPIOB->MODER,
              GPIO_MODER_MODER10_Msk |
              GPIO_MODER_MODER4_Msk |
              GPIO_MODER_MODER5_Msk);

    MODIFY_REG(GPIOB->PUPDR,
               GPIO_PUPDR_PUPD10_Msk |
               GPIO_PUPDR_PUPD4_Msk |
               GPIO_PUPDR_PUPD5_Msk,
               GPIO_PUPDR_PUPD10_0 |
               GPIO_PUPDR_PUPD4_0 |
               GPIO_PUPDR_PUPD5_0);

    /* PUSH-PULL */
    CLEAR_BIT(GPIOA->OTYPER,
              GPIO_OTYPER_OT5 |
              GPIO_OTYPER_OT6 |
              GPIO_OTYPER_OT7 |
              GPIO_OTYPER_OT8);

    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT6);

    /* NO PULL */
    CLEAR_BIT(GPIOA->PUPDR,
              GPIO_PUPDR_PUPD5_Msk |
              GPIO_PUPDR_PUPD6_Msk |
              GPIO_PUPDR_PUPD7_Msk |
              GPIO_PUPDR_PUPD8_Msk);

    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPD6_Msk);
}


/* ===================== RCC ===================== */
void RCC_Init(void)
{
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);
    CLEAR_REG(RCC->CFGR);

    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI);

    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET);

    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET);

    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

    SET_BIT(RCC->CR, RCC_CR_HSEON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET);

    MODIFY_REG(FLASH->ACR,
               FLASH_ACR_LATENCY_Msk,
               FLASH_ACR_LATENCY_5WS);

    CLEAR_REG(RCC->PLLCFGR);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);

    MODIFY_REG(RCC->PLLCFGR,
               RCC_PLLCFGR_PLLM_Msk,
               (4U << RCC_PLLCFGR_PLLM_Pos));

    MODIFY_REG(RCC->PLLCFGR,
               RCC_PLLCFGR_PLLN_Msk,
               (100U << RCC_PLLCFGR_PLLN_Pos));

    CLEAR_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk);

    MODIFY_REG(RCC->CFGR,
               RCC_CFGR_HPRE_Msk |
               RCC_CFGR_PPRE1_Msk |
               RCC_CFGR_PPRE2_Msk,
               RCC_CFGR_HPRE_DIV1 |
               RCC_CFGR_PPRE1_DIV2 |
               RCC_CFGR_PPRE2_DIV1);

    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == RESET);

    MODIFY_REG(RCC->CFGR,
               RCC_CFGR_SW_Msk,
               RCC_CFGR_SW_PLL);

    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL);

    SET_BIT(RCC->CR, RCC_CR_CSSON);
}


/* ===================== MOTOR ===================== */
void Motor_Forward(void)
{
    SET_BIT(GPIOA->ODR, GPIO_ODR_OD5);
    CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD6);

    SET_BIT(GPIOB->ODR, GPIO_ODR_OD6);
    CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD7);
}

void Motor_Stop(void)
{
    CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD5);
    CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD6);
    CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD7);
    CLEAR_BIT(GPIOB->ODR, GPIO_ODR_OD6);
}


/* ===================== ADC ===================== */
void ADC_Init(void)
{
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);

    MODIFY_REG(GPIOA->MODER,
               GPIO_MODER_MODER0_Msk |
               GPIO_MODER_MODER1_Msk |
               GPIO_MODER_MODER4_Msk,
               GPIO_MODER_MODER0_Msk |
               GPIO_MODER_MODER1_Msk |
               GPIO_MODER_MODER4_Msk);

    ADC1->CR1 = 0;
    ADC1->CR2 = 0;
    ADC1->SMPR1 = 0;
    ADC1->SMPR2 = 0;
    ADC1->SQR1 = 0;
    ADC1->SQR2 = 0;
    ADC1->SQR3 = 0;

    MODIFY_REG(ADC1->SMPR2,
               ADC_SMPR2_SMP0_Msk |
               ADC_SMPR2_SMP1_Msk |
               ADC_SMPR2_SMP4_Msk,
               ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP0_1 |
               ADC_SMPR2_SMP1_2 | ADC_SMPR2_SMP1_1 |
               ADC_SMPR2_SMP4_2 | ADC_SMPR2_SMP4_1);

    SET_BIT(ADC1->CR2, ADC_CR2_ADON);
}

uint16_t ADC_Read(uint8_t channel)
{
    MODIFY_REG(ADC1->SQR3, ADC_SQR3_SQ1_Msk, channel);
    SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
    while (!(ADC1->SR & ADC_SR_EOC));
    return ADC1->DR;
}

void LineSensors_ReadADC(void)
{
    adc_left   = ADC_Read(0);
    adc_center = ADC_Read(1);
    adc_right  = ADC_Read(4);
}


/* ===================== BUTTONS (NEW) ===================== */
uint8_t Btn_Left()   { return !(GPIOB->IDR & GPIO_IDR_ID10); }
uint8_t Btn_Center() { return !(GPIOB->IDR & GPIO_IDR_ID4); }
uint8_t Btn_Right()  { return !(GPIOB->IDR & GPIO_IDR_ID5); }


/* ===================== DELAY ===================== */
void delay(volatile uint32_t t)
{
    while (t--);
}




/* ===================== CALIBRATION ===================== */
void Calibration_Process(void)
{
    static uint8_t last = 0;

    LineSensors_ReadADC();

    uint8_t btn =
        Btn_Left() ||
        Btn_Center() ||
        Btn_Right();

    if (btn && !last)
    {
        uint16_t val;

        /* какой датчик нажали */
        if (Btn_Left()) sensor_id = 0;
        else if (Btn_Center()) sensor_id = 1;
        else if (Btn_Right()) sensor_id = 2;

        /* значение датчика */
        if (sensor_id == 0) val = adc_left;
        else if (sensor_id == 1) val = adc_center;
        else val = adc_right;

        /* ---------- MIN ---------- */
        if (calib_stage == 0)
        {
            if (sensor_id == 0) min_left = val;
            else if (sensor_id == 1) min_center = val;
            else min_right = val;

            calib_stage = 1;
        }

        /* ---------- MAX ---------- */
        else
        {
            if (sensor_id == 0)
            {
                max_left = val;
                thr_left = (min_left + max_left) / 2;
            }
            else if (sensor_id == 1)
            {
                max_center = val;
                thr_center = (min_center + max_center) / 2;
            }
            else
            {
                max_right = val;
                thr_right = (min_right + max_right) / 2;
            }

            calib_stage = 0;
        }
    }

    last = btn;
}


/* ===================== PROCESS ===================== */
void LineSensors_Process(void)
{
    if (max_left > min_left)
        sensor_left = (adc_left > thr_left);
    else
        sensor_left = (adc_left < thr_left);

    if (max_center > min_center)
        sensor_center = (adc_center > thr_center);
    else
        sensor_center = (adc_center < thr_center);

    if (max_right > min_right)
        sensor_right = (adc_right > thr_right);
    else
        sensor_right = (adc_right < thr_right);
}