#include "motor.h"

static int16_t clamp_speed(int16_t speed)
{
    if (speed > 999)
        return 999;
    if (speed < -999)
        return -999;
    return speed;
}

/**
 * @brief Управление левым мотором.
 * @note
 *  Направление: PA5, PA6
 *  PWM: TIM2_CH3 (PB10)
 * @param speed Скорость от -999 до 999
 */
void left_motor(int16_t speed)
{
    speed = clamp_speed(speed);

    if (speed > 0)
    {
        WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BR_5 | GPIO_BSRR_BS_6);
        WRITE_REG(TIM2->CCR3, (uint16_t)speed);
    }
    else if (speed < 0)
    {
        speed = -speed;
        WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS_5 | GPIO_BSRR_BR_6);
        WRITE_REG(TIM2->CCR3, (uint16_t)speed);
    }
    else
    {
        /* Плавная остановка без жесткого торможения */
        WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BR_5 | GPIO_BSRR_BR_6);
        WRITE_REG(TIM2->CCR3, 0U);
    }
}

/**
 * @brief Управление правым мотором.
 * @note
 *  Направление: PA7, PB6
 *  PWM: TIM2_CH2 (PB3)
 * @param speed Скорость от -999 до 999
 */
void right_motor(int16_t speed)
{
    speed = clamp_speed(speed);

    if (speed > 0)
    {
        WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS_7);
        WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BR_6);
        WRITE_REG(TIM2->CCR2, (uint16_t)speed);
    }
    else if (speed < 0)
    {
        speed = -speed;
        WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BR_7);
        WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BS_6);
        WRITE_REG(TIM2->CCR2, (uint16_t)speed);
    }
    else
    {
        /* Плавная остановка без жесткого торможения */
        WRITE_REG(GPIOA->BSRR, GPIO_BSRR_BS_7);
        WRITE_REG(GPIOB->BSRR, GPIO_BSRR_BR_6);
        WRITE_REG(TIM2->CCR2, 0U);
    }
}