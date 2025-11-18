#ifndef INIT_H
#define INIT_H

#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F429ZI/stm32f429xx.h"

#define TIME_DELAY1 3500

        
// #define RCC_AHB1ENR         *(uint32_t *)(0x40023800UL + 0x30UL)

// #define RCC_GPIOB_EN        0x02UL
// #define RCC_GPIOC_EN        0x04UL

// #define GPIOB_MODER         *(uint32_t *)(0x40020400UL + 0x00UL) 
// #define GPIOB_OTYPER        *(uint32_t *)(0x40020400UL + 0x04UL) 
// #define GPIOB_OSPEEDR       *(uint32_t *)(0x40020400UL + 0x08UL) 
// #define GPIOB_BSSR          *(uint32_t *)(0x40020400UL + 0x18UL) 

// #define GPIOC_IDR           *(uint32_t *)(0x40020800UL + 0x10UL) 

// #define GPIO_PIN_OUT_7      0x4000UL
// #define GPIO_OFF            0x00UL
// #define GPIO_PIN_MED_7      0x4000UL
// #define GPIO_PIN_RESET_7    0x800000UL
// #define GPIO_PIN_SET_7      0x80UL
// #define GPIO_PIN_13         0x2000UL

// #define BIT_SET(REG, BIT)   ((REG) |= (BIT))
// #define BIT_READ(REG, BIT)  ((REG) & (BIT))

void GPIO_Init_Memory1(void);
void GPIO_Init_Using_My_Macros(void);
void GPIO_Init_CMSIS_LB1(void);
int pressedButton1(void);
int pressedButton2(void);
int pressedButton3(void);
void TurningOnState(void);
void TurningOffState(void);
int TheSwitcher(int count);

#endif 