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

void GPIO_Init_CMSIS1(void){
    // SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);

    // SET_BIT(GPIOD->MODER, GPIO_MODER_MODER2_0);        //Настройка пина PD2 на вывод, регистр MODER
    // CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT14);         //Настройка режима работы выxода на push-pull, регистр OTYPER
    // SET_BIT(GPIOB->OSPEEDR,GPIO_OSPEEDER_OSPEEDR14_0);  //Настройка скорости работы вывода PB14, регистр OSPEEDR
    // SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);               //Предварительное выключение светодиода, регистр BSR, бит BR7
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOGEN);

    SET_BIT(GPIOB->MODER, GPIO_MODER_MODER14_0);        //Настройка пина PB14 на вывод, регистр MODER
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT14);         //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT(GPIOB->OSPEEDR,GPIO_OSPEEDER_OSPEEDR14_0);  //Настройка скорости работы вывода PB14, регистр OSPEEDR
    SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);

    SET_BIT(GPIOD->MODER, GPIO_MODER_MODER14_0);        //Настройка пина PD14 на вывод, регистр MODER
    CLEAR_BIT(GPIOD->OTYPER, GPIO_OTYPER_OT14);         //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT(GPIOD->OSPEEDR,GPIO_OSPEEDER_OSPEEDR14_0);  //Настройка скорости работы вывода PD14, регистр OSPEEDR
    SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR14);

    SET_BIT(GPIOD->MODER, GPIO_MODER_MODER15_0);        //Настройка пина PD15 на вывод, регистр MODER
    CLEAR_BIT(GPIOD->OTYPER, GPIO_OTYPER_OT15);         //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT(GPIOD->OSPEEDR,GPIO_OSPEEDER_OSPEEDR15_0);  //Настройка скорости работы вывода PD15, регистр OSPEEDR
    SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR15);


}
