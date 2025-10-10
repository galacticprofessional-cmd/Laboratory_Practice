#include "../Inc/init_LB1.h"

#define TIME_DELAY 1000000

int main(void){
        GPIO_Init_CMSIS1();
        
        while(1){
            if(READ_BIT(GPIOD->IDR , GPIO_IDR_IDR_2) != 0){
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS14);
                for(int i = 1; i <= TIME_DELAY; i++){}
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS14);
                for(int i = 1; i <= TIME_DELAY; i++){}
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BS15);
            }
            else if(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_2) != 0){
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);
                for(int i = 1; i <= TIME_DELAY; i++){}
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR14);
                for(int i = 1; i <= TIME_DELAY; i++){}
                SET_BIT(GPIOD->BSRR, GPIO_BSRR_BR15);
            }

        }
}