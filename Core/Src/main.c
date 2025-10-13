#include "../Inc/init_LB1.h"

#define TIME_DELAY 1000000

void TurningOnState(void){
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS13);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS9);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS11);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS14);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS13);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS15);
}

void TurningOffState(void){
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR13);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR9);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR11);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR14);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR13);
    for(int i = 1; i <= TIME_DELAY; i++){}
    SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR15);
}
int TheSwitcher(int count){
    int key;
    if(count % 2 == 0){
        key = 0;
    }else{
        key = 1;
    }
    return key;
}

int main(void){
    
    GPIO_Init_CMSIS_LB1();
    int count = 0;

    while(1){
        int key = TheSwitcher(count);
        
        if(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_3) != 0){
            count++;
        }

        if (key == 0){
            if(READ_BIT(GPIOD->IDR , GPIO_IDR_IDR_2) != 0){
                TurningOnState();
            }
            else if(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_2) != 0){
                TurningOffState();
            }
        }
        else if(key == 1){
            if(READ_BIT(GPIOD->IDR , GPIO_IDR_IDR_2) != 0){
                TurningOffState();
            }
            else if(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_2) != 0){
                TurningOnState();
            }            
        }

    }
}