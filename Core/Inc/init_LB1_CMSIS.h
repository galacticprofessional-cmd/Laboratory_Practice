#ifndef INIT_LB1_CMSIS_H
#define INIT_LB1_CMSIS_H

#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"

#define TIME_DELAY1 3500

void GPIO_Init_Memory1(void);
void GPIO_Init_Using_My_Macros(void);
void GPIO_Init_CMSIS_LB1(void);
int pressedButton1(void);
int pressedButton2(void);
int pressedButton3(void);
void TurningOnState(void);
void TurningOffState(void);
int TheSwitcher(int count);

// Extra task functions

void ledOn(int n);
void ledOff(int n);
int changeSpeed(int speedIndex);
void blinkLed(int ledNum, int *ledState);

#endif 