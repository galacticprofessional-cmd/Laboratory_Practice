#include "../Inc/init.h"

void GPIO_Init_Memory(void){
        *(uint32_t *)(0x40023800UL + 0x30UL) |= 0x02UL;         //Включение тактирования на переферии GPIOB и GPIOC, регистр AHB1ENR
        *(uint32_t *)(0x40020400UL + 0x00UL) |= 0x4000UL;       //Настройка пина PB7 на вывводб регистр MODER
        *(uint32_t *)(0x40020400UL + 0x04UL) |= 0x00UL;         //Настройка режима работы выxода на push-pull, регистр OTYPER
        *(uint32_t *)(0x40020400UL + 0x08UL) |= 0x4000UL;       //Настройка скорости вывода PB7, регистр OSPEEDR
        *(uint32_t *)(0x40020400UL + 0x18UL) |= 0x800000UL;     //Предварительное выключение светодиода, регистр BSR, бит BR7
}

void GPIO_Init_With_My_Macros(void){
    RCC_AHB1ENR |= RCC_GPIOB_EN | RCC_GPIOC_EN;     //Включение тактирования на переферии GPIOB и GPIOC, регистр AHB1ENR

    BIT_SET(GPIOB_MODER,    GPIO_PIN_OUT_7);        //Настройка пина PB7 на вывод, регистр MODER
    BIT_SET(GPIOB_OTYPER,   GPIO_OFF);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    BIT_SET(GPIOB_OSPEEDR,  GPIO_PIN_MED_7);        //Настройка скорости вывода PB7, регистр OSPEEDR
    BIT_SET(GPIOB_BSSR,     GPIO_PIN_RESET_7);      //Предварительное выключение светодиода, регистр BSR, бит BR7

}

void GPIO_Init_CMSIS(void){
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODER14_0);        //Настройка пина PB14 на вывод, регистр MODER
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT14);         //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT(GPIOB->OSPEEDR,GPIO_OSPEEDER_OSPEEDR14_0);  //Настройка скорости работы вывода PB14, регистр OSPEEDR
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);               //Предварительное выключение светодиода, регистр BSR, бит BR7


}

