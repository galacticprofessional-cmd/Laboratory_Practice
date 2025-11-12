#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"

extern volatile uint8_t btnCount, flag;
extern volatile uint32_t global_systick_tim_count;
extern volatile uint16_t second_tim_count;

#endif