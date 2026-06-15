#include "sensors.h"

/**
 * @brief Чтение датчика, подключенного к PA0
 * @returns 1 если черная линия, иначе 0
 */
uint8_t read_left_sensor(void)
{
    return READ_BIT(GPIOA->IDR, GPIO_IDR_IDR_0) ? 1U : 0U;
}

/**
 * @brief Чтение центрального датчика, подключенного к PA1
 * @returns 1 если черная линия, иначе 0
 */
uint8_t read_middle_sensor(void)
{
    return READ_BIT(GPIOA->IDR, GPIO_IDR_IDR_1) ? 1U : 0U;
}

/**
 * @brief Чтение датчика, подключенного к PA4
 * @returns 1 если черная линия, иначе 0
 */
uint8_t read_right_sensor(void)
{
    return READ_BIT(GPIOA->IDR, GPIO_IDR_IDR_4) ? 1U : 0U;
}