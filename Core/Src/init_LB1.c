#include "../Inc/init_LB1.h"

void GPIO_Init_Memory1(void){
//         *(uint32_t *)() |=     //Включение тактирования на переферии GPIOB и GPIOC, регистр AHB1ENR
//         *(uint32_t *)() |=     //Настройка пина PB7 на вывводб регистр MODER
//         *(uint32_t *)() |=     //Настройка режима работы выxода на push-pull, регистр OTYPER
//         *(uint32_t *)() |=     //Настройка скорости вывода PB7, регистр OSPEEDR
//         *(uint32_t *)() |=     //Предварительное выключение светодиода, регистр BSR, бит BR7
}

void GPIO_Init_Using_My_Macros(void){
//     RCC_AHB1ENR |= RCC_GPIOB_EN | RCC_GPIOC_EN;     //Включение тактирования на переферии GPIOB и GPIOC, регистр AHB1ENR

//     BIT_SET();        //Настройка пина PB7 на вывод, регистр MODER
//     BIT_SET();              //Настройка режима работы выxода на push-pull, регистр OTYPER
//     BIT_SET();        //Настройка скорости вывода PB7, регистр OSPEEDR
//     BIT_SET();      //Предварительное выключение светодиода, регистр BSR, бит BR7

}

void GPIO_Init_CMSIS_LB1(void){

    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN);

    SET_BIT     (GPIOF->MODER,      GPIO_MODER_MODER13_0);          //Настройка пина PF13 на вывод, регистр MODER
    CLEAR_BIT   (GPIOF->OTYPER,     GPIO_OTYPER_OT13);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT     (GPIOF->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR13_0);     //Настройка скорости работы вывода PF13, регистр OSPEEDR
    SET_BIT     (GPIOF->BSRR,       GPIO_BSRR_BR13);                //Предварительное выключение светодиода, регистр BSR, бит BR13

    SET_BIT     (GPIOE->MODER,      GPIO_MODER_MODER9_0);          //Настройка пина PE9 на вывод, регистр MODER
    CLEAR_BIT   (GPIOE->OTYPER,     GPIO_OTYPER_OT9);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT     (GPIOE->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR9_0);     //Настройка скорости работы вывода PE9, регистр OSPEEDR
    SET_BIT     (GPIOE->BSRR,       GPIO_BSRR_BR9);                //Предварительное выключение светодиода, регистр BSR, бит BR9

    SET_BIT     (GPIOE->MODER,      GPIO_MODER_MODER11_0);          //Настройка пина PE11 на вывод, регистр MODER
    CLEAR_BIT   (GPIOE->OTYPER,     GPIO_OTYPER_OT11);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT     (GPIOE->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR11_0);     //Настройка скорости работы вывода PE11, регистр OSPEEDR
    SET_BIT     (GPIOE->BSRR,       GPIO_BSRR_BR11);                //Предварительное выключение светодиода, регистр BSR, бит BR11

    SET_BIT     (GPIOF->MODER,      GPIO_MODER_MODER14_0);          //Настройка пина PF14 на вывод, регистр MODER
    CLEAR_BIT   (GPIOF->OTYPER,     GPIO_OTYPER_OT14);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT     (GPIOF->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR14_0);     //Настройка скорости работы вывода PF14, регистр OSPEEDR
    SET_BIT     (GPIOF->BSRR,       GPIO_BSRR_BR14);                //Предварительное выключение светодиода, регистр BSR, бит BR14

    SET_BIT     (GPIOE->MODER,      GPIO_MODER_MODER13_0);          //Настройка пина PE13 на вывод, регистр MODER
    CLEAR_BIT   (GPIOE->OTYPER,     GPIO_OTYPER_OT13);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT     (GPIOE->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR13_0);     //Настройка скорости работы вывода PE13, регистр OSPEEDR
    SET_BIT     (GPIOE->BSRR,       GPIO_BSRR_BR13);                //Предварительное выключение светодиода, регистр BSR, бит BR13

    SET_BIT     (GPIOF->MODER,      GPIO_MODER_MODER15_0);          //Настройка пина PF15 на вывод, регистр MODER
    CLEAR_BIT   (GPIOF->OTYPER,     GPIO_OTYPER_OT15);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT     (GPIOF->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR15_0);     //Настройка скорости работы вывода PF15, регистр OSPEEDR
    SET_BIT     (GPIOF->BSRR,       GPIO_BSRR_BR15);                //Предварительное выключение светодиода, регистр BSR, бит BR15

}
