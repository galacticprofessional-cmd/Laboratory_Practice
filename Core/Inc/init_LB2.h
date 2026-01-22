#ifndef INIT_LB2_H
#define INIT_LB2_H

#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F411RE/stm32f411xe.h"


// extern volatile uint8_t sensor_left;   // PA6
// extern volatile uint8_t sensor_right;  // PA7

// extern volatile int8_t line_error;     // для будущего алгоритма.

extern volatile uint16_t line_sensor_adc;


        
void GPIO_Init(void);
void RCC_Init(void);
void TIM3_PWM_Init(void);
void LineSensors_GPIO_Init(void);
void LineSensor_ADC_Init(void);
void LineSensor_Update(void);
uint16_t LineSensor_ReadFiltered(void);
void Motor_SetSpeed_Left(int16_t speed);
void Motor_SetSpeed_Right(int16_t speed);

#endif 