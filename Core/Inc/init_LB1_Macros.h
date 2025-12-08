#ifndef INIT_LB1_MACROS_H
#define INIT_LB1_MACROS_H

#include "stdint.h"

#define TIME_DELAY1 3500


#define RCC_AHB1ENR         *(uint32_t *)(0x40023800UL + 0x30UL)

#define RCC_GPIOD_EN        0x0UL << 3U
#define RCC_GPIOG_EN        0x1UL << 6U
#define RCC_GPIOE_EN        0x1UL << 4U
#define RCC_GPIOF_EN        0x1UL << 5U

#define GPIOE_MODER         *(uint32_t *)(0x40021000UL + 0x00UL) 
#define GPIOE_OTYPER        *(uint32_t *)(0x40021000UL + 0x04UL) 
#define GPIOE_OSPEEDR       *(uint32_t *)(0x40021000UL + 0x08UL) 
#define GPIOE_BSRR          *(uint32_t *)(0x40021000UL + 0x18UL)

#define GPIOF_MODER         *(uint32_t *)(0x40021400UL + 0x00UL) 
#define GPIOF_OTYPER        *(uint32_t *)(0x40021400UL + 0x04UL) 
#define GPIOF_OSPEEDR       *(uint32_t *)(0x40021400UL + 0x08UL) 
#define GPIOF_BSRR          *(uint32_t *)(0x40021400UL + 0x18UL)


#define MODER9_0            0x00040000UL
#define OTYPER9             0x1UL << 9U
#define OSPEEDR9_0          0x1UL << 18U
#define BSRR_BR9            0x1UL << 25U
#define BSRR_BS9            0x1UL << 9U

#define MODER11_0           0x00400000UL
#define OTYPER11            0x1UL << 11U
#define OSPEEDR11_0         0x1UL << 22U
#define BSRR_BR11           0x1UL << 27U
#define BSRR_BS11           0x1UL << 11U

#define MODER13_0           0x04000000UL
#define OTYPER13            0x1UL << 13U
#define OSPEEDR13_0         0x1UL << 26U
#define BSRR_BR13           0x1UL << 29U
#define BSRR_BS13           0x1UL << 13U

#define MODER14_0           0x10000000UL
#define OTYPER14            0x1UL << 14U
#define OSPEEDR14_0         0x1UL << 28U
#define BSRR_BR14           0x1UL << 30U
#define BSRR_BS14           0x1UL << 14U

#define MODER15_0           0x40000000UL
#define OTYPER15            0x1UL << 15U
#define OSPEEDR15_0         0x1UL << 30U
#define BSRR_BR15           0x1UL << 31U
#define BSRR_BS15           0x1UL << 15U


#define GPIOD_IDR           *(uint32_t *)(0x40020C00UL + 0x04UL)
#define GPIOG_IDR           *(uint32_t *)(0x40021800UL + 0x04UL)
#define GPIOF_IDR           *(uint32_t *)(0x40021400UL + 0x04UL)
#define GPIOE_IDR           *(uint32_t *)(0x40021000UL + 0x04UL)



#define IDR_2               0x1UL << 2U
#define IDR_3               0x1UL << 3U
#define IDR_9               0x1UL << 9U
#define IDR_11              0x1UL << 11U
#define IDR_13              0x1UL << 13U
#define IDR_14              0x1UL << 14U
#define IDR_15              0x1UL << 15U



#define BIT_SET(REG, BIT)   ((REG) |= (BIT))
#define BIT_CLEAR(REG, BIT)   ((REG) &= ~(BIT))
#define BIT_READ(REG, BIT)  ((REG) & (BIT))

#endif 