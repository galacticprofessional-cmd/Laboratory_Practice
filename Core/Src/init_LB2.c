#include "init_LB2.h"

void ITR_Init(void){
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);

    SET_BIT(SYSCFG->EXTICR[3], SYSCFG_EXTICR4_EXTI12_PC);
    SET_BIT(EXTI->IMR, EXTI_IMR_IM12);
    SET_BIT(EXTI->RTSR, EXTI_RTSR_TR12);
    CLEAR_BIT(EXTI->FTSR, EXTI_FTSR_TR12);
    NVIC_SetPriority(EXTI15_10_IRQn, 
                        NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0)); 
    NVIC_EnableIRQ(EXTI15_10_IRQn); 
}

void GPIO_Init_CMSIS_LB2(void){
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);

    SET_BIT(GPIOC->MODER, GPIO_MODER_MODER9_1);        //Настройка пина PC9 на вывод, на альтернативный режим
    SET_BIT(GPIOC->OSPEEDR,GPIO_OSPEEDER_OSPEEDR9);  //Настройка скорости работы вывода PC, регистр OSPEEDR
    MODIFY_REG(GPIOC->AFR[1], GPIO_AFRH_AFSEL9_Msk, 0x00UL);     
    

    SET_BIT(GPIOA->MODER, GPIO_MODER_MODER8_1);        //Настройка пина PC9 на вывод, на альтернативный режим
    SET_BIT(GPIOC->OSPEEDR,GPIO_OSPEEDER_OSPEEDR8);  //Настройка скорости работы вывода PC, регистр OSPEEDR
    CLEAR_BIT(GPIOA->AFR[1], GPIO_AFRH_AFSEL8);  

    SET_BIT     (GPIOB->MODER,      GPIO_MODER_MODER7_0);          //Настройка пина PF13 на вывод, регистр MODER
    CLEAR_BIT   (GPIOB->OTYPER,     GPIO_OTYPER_OT7);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT     (GPIOB->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR7_0);     //Настройка скорости работы вывода PF13, регистр OSPEEDR
    SET_BIT     (GPIOB->BSRR,       GPIO_BSRR_BR7);                //Предварительное выключение светодиода, регистр BSR, бит BR13


}


void RCC_Init(void){
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U); 
    CLEAR_REG(RCC->CFGR); 
    while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET); 
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON); 
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET); 
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON); 
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET); 
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

    // SET_BIT(RCC->CR, RCC_CR_HSEON); //Запускаем внешний кварцевый резонатор 
    // while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET); //Ждём пока он запустится 
    // CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); //Сбросим бит байпаса в 0, если вдруг там что-то лежит 
    // SET_BIT(RCC->CR, RCC_CR_CSSON); //Запустим Clock detector 

    SET_BIT(RCC->CR, RCC_CR_HSEON);
    while(READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET);
    SET_BIT(RCC->CR, RCC_CR_CSSON);

    CLEAR_REG(RCC->PLLCFGR);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSE);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_2);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLN_5 |
                          RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_8);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_0);
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_0 | RCC_PLLCFGR_PLLQ_1 |
                            RCC_PLLCFGR_PLLQ_2 | RCC_PLLCFGR_PLLQ_3);

    SET_BIT(RCC->CFGR, RCC_CFGR_SW_1);
    SET_BIT(RCC->CFGR, RCC_CFGR_HPRE_DIV1);
    SET_BIT(RCC->CFGR, RCC_CFGR_PPRE1_DIV4);
    SET_BIT(RCC->CFGR, RCC_CFGR_PPRE2_DIV2);
    SET_BIT(RCC->CFGR, RCC_CFGR_MCO1);
    SET_BIT(RCC->CFGR, RCC_CFGR_MCO1PRE_2 | RCC_CFGR_MCO1PRE_1);
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_MCO2);
    SET_BIT(RCC->CFGR, RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO2PRE_1);


    SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_5WS); 

    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) == RESET);

}