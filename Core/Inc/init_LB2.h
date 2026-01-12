#ifndef INIT_LB2_H
#define INIT_LB2_H

#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F411RE/stm32f411xe.h"

        
void GPIO_Init_CMSIS_LB2(void);
void ITR_Init(void);
void RCC_Init(void);
void SysTick_Init(void);

#endif 