#include "../Inc/init.h"

#define TIME_DELAY 1500

int main(void){
        GPIO_Init_With_My_Macros();
        GPIO_Init_CMSIS();
        
        while(1){
            if(BIT_READ(GPIOC_IDR, GPIO_PIN_13)){
                BIT_SET(GPIOB_BSSR, GPIO_PIN_SET_7);
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR14);


            }
            else{
                BIT_SET(GPIOB_BSSR, GPIO_PIN_RESET_7);
                SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS14);

            }

        }
}