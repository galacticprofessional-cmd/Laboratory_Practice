#include "../Inc/init_LB1.h"

volatile uint32_t mcu_btn1_count = 0;
volatile uint32_t mcu_btn2_count = 0;
volatile uint32_t mcu_btn3_count = 0;

volatile uint8_t  mcu_btn1_event = 0;
volatile uint8_t  mcu_btn2_event = 0;
volatile uint8_t  mcu_btn3_event = 0;



void GPIO_Init_CMSIS_LB1(void){

    SET_BIT     (RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);

    SET_BIT     (GPIOA->MODER,      GPIO_MODER_MODER8_0);         
    CLEAR_BIT   (GPIOA->OTYPER,     GPIO_OTYPER_OT8);             
    SET_BIT     (GPIOA->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR8_0);    
    SET_BIT     (GPIOA->BSRR,       GPIO_BSRR_BR8);               

    SET_BIT     (GPIOB->MODER,      GPIO_MODER_MODER10_0);        
    CLEAR_BIT   (GPIOB->OTYPER,     GPIO_OTYPER_OT10);            
    SET_BIT     (GPIOB->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR10_0);   
    SET_BIT     (GPIOB->BSRR,       GPIO_BSRR_BR10);
    
    SET_BIT     (GPIOB->MODER,      GPIO_MODER_MODER3_0);        
    CLEAR_BIT   (GPIOB->OTYPER,     GPIO_OTYPER_OT3);            
    SET_BIT     (GPIOB->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR3_0);   
    SET_BIT     (GPIOB->BSRR,       GPIO_BSRR_BR3);

    SET_BIT     (GPIOB->MODER,      GPIO_MODER_MODER4_0);        
    CLEAR_BIT   (GPIOB->OTYPER,     GPIO_OTYPER_OT4);            
    SET_BIT     (GPIOB->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR4_0);   
    SET_BIT     (GPIOB->BSRR,       GPIO_BSRR_BR4);

    SET_BIT     (GPIOB->MODER,      GPIO_MODER_MODER5_0);        
    CLEAR_BIT   (GPIOB->OTYPER,     GPIO_OTYPER_OT5);            
    SET_BIT     (GPIOB->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR5_0);   
    SET_BIT     (GPIOB->BSRR,       GPIO_BSRR_BR5);

    SET_BIT     (GPIOA->MODER,      GPIO_MODER_MODER10_0);            
    CLEAR_BIT   (GPIOA->OTYPER,     GPIO_OTYPER_OT10);                
    SET_BIT     (GPIOA->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR10_0);       
    SET_BIT     (GPIOA->BSRR,       GPIO_BSRR_BR10);
    
    // --- PD2: input + pull-up ---
    CLEAR_BIT(GPIOD->MODER, GPIO_MODER_MODE2_Msk);      // 00 input
    CLEAR_BIT(GPIOD->PUPDR, GPIO_PUPDR_PUPD2_Msk);
    SET_BIT  (GPIOD->PUPDR, GPIO_PUPDR_PUPD2_0);        // 01 pull-up

    // --- PA0: input + pull-up ---
    CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODE0_Msk);      // 00 input
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD0_Msk);
    SET_BIT  (GPIOA->PUPDR, GPIO_PUPDR_PUPD0_0);        // 01 pull-up

    // --- PA1: input + pull-up ---
    CLEAR_BIT(GPIOA->MODER, GPIO_MODER_MODE1_Msk);      // 00 input
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD1_Msk);
    SET_BIT  (GPIOA->PUPDR, GPIO_PUPDR_PUPD1_0);        // 01 pull-up

}


int pressedButton1(void){

    if(READ_BIT(GPIOD->IDR , GPIO_IDR_IDR_2) == 0){
        for(int i = 0; i < TIME_DELAY1; i++){}
        if(READ_BIT(GPIOD->IDR , GPIO_IDR_IDR_2) == 0){
            while(READ_BIT(GPIOD->IDR , GPIO_IDR_IDR_2) == 0){}
            mcu_btn1_count++;
            mcu_btn1_event = 1;
            return 1;
        }

    }
    return 0;
}

int pressedButton2(void){

    if(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_0) == 0){
        for(int i = 0; i < TIME_DELAY1; i++){}
        if(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_0) == 0){
            while(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_0) == 0){}
            mcu_btn2_count++;            
            mcu_btn2_event = 1;            
            return 1;
        }

    }
    return 0;
}

int pressedButton3(void){

    if(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_1) == 0){
        for(int i = 0; i < TIME_DELAY1; i++){}
        if(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_1) == 0){
            while(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_1) == 0){}
            mcu_btn3_count++;
            mcu_btn3_event = 1;
            return 1;
        }

    }
    return 0;
}

void TurningOnState(void){

    if(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_8) == 0){
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS8);
    }

    else if(READ_BIT(GPIOB->IDR , GPIO_IDR_IDR_10) == 0){
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS10);
    }

    else if(READ_BIT(GPIOB->IDR , GPIO_IDR_IDR_4) == 0){
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS4);
    }

    else if(READ_BIT(GPIOB->IDR , GPIO_IDR_IDR_5) == 0){
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS5);
    }

    else if(READ_BIT(GPIOB->IDR , GPIO_IDR_IDR_3) == 0){
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS3);
    }

    else if(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_10) == 0){
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS10);
    }

}

void TurningOffState(void){

    if(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_8) != 0){
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR8);
    }

    else if(READ_BIT(GPIOB->IDR , GPIO_IDR_IDR_10) != 0){
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR10);
    }

    else if(READ_BIT(GPIOB->IDR , GPIO_IDR_IDR_4) != 0){
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR4);
    }

    else if(READ_BIT(GPIOB->IDR , GPIO_IDR_IDR_5) != 0){
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR5);
    }

    else if(READ_BIT(GPIOB->IDR , GPIO_IDR_IDR_3) != 0){
        SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR3);
    }

    else if(READ_BIT(GPIOA->IDR , GPIO_IDR_IDR_10) != 0){
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR10);
    }

}

int TheSwitcher(int count){

    return count & 1;

}
