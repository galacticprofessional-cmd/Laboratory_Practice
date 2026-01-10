#ifndef INIT_LB2_H
#define INIT_LB2_H

#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F411RE/stm32f411xe.h"

        
void GPIO_Init(void);
void RCC_Init(void);
void TIM3_PWM_Init(void);
void Motor_SetSpeed_Left(int16_t speed);
void Motor_SetSpeed_Right(int16_t speed);

#endif 