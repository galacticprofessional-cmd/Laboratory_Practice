#ifndef INIT_LB1_H
#define INIT_LB1_H

#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F411RE/stm32f411xe.h"

extern volatile uint32_t mcu_btn1_count;
extern volatile uint32_t mcu_btn2_count;
extern volatile uint32_t mcu_btn3_count;

extern volatile uint8_t  mcu_btn1_event;
extern volatile uint8_t  mcu_btn2_event;
extern volatile uint8_t  mcu_btn3_event;




#define GPIOE_MODER   (*(volatile uint32_t *)(0x40021000UL + 0x00UL))
#define GPIOE_OTYPER  (*(volatile uint32_t *)(0x40021000UL + 0x04UL))
#define GPIOE_OSPEEDR (*(volatile uint32_t *)(0x40021000UL + 0x08UL))
#define GPIOE_BSRR    (*(volatile uint32_t *)(0x40021000UL + 0x18UL))

#define GPIOF_MODER   (*(volatile uint32_t *)(0x40021400UL + 0x00UL))
#define GPIOF_OTYPER  (*(volatile uint32_t *)(0x40021400UL + 0x04UL))
#define GPIOF_OSPEEDR (*(volatile uint32_t *)(0x40021400UL + 0x08UL))
#define GPIOF_BSRR    (*(volatile uint32_t *)(0x40021400UL + 0x18UL))

#define MODER11_0           0x00400000UL
#define OTYPER11            0x1UL << 11U
#define OSPEEDR11_0         0x1UL << 22U
#define BSRR_BR11           0x1UL << 27U
#define BSRR_BS11           0x1UL << 11U

#define MODER14_0           0x10000000UL
#define OTYPER14            0x1UL << 14U
#define OSPEEDR14_0         0x1UL << 28U
#define BSRR_BR14           0x1UL << 30U
#define BSRR_BS14           0x1UL << 14U

#define BIT_SET(REG, BIT)   ((REG) |= (BIT))
#define BIT_CLEAR(REG, BIT) ((REG) &= ~(BIT))
#define BIT_READ(REG, BIT)  ((REG) & (BIT))


#define TIME_DELAY1 3500

void GPIO_Init_CMSIS_LB1(void);
int pressedButton1(void);
int pressedButton2(void);
int pressedButton3(void);
void TurningOnState(void);
void TurningOffState(void);
int TheSwitcher(int count);


#endif 