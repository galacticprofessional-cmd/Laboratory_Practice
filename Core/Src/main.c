#include "../Inc/init.h"
int main(void){
        GPIO_Init_With_My_Macros();
        
        while(1){

            if(BIT_READ(GPIOC_IDR, GPIO_PIN_13)){
                BIT_SET(GPIOB_BSSR, GPIO_PIN_SET_7);

            }
            else{
                BIT_SET(GPIOB_BSSR, GPIO_PIN_RESET_7);
            }

        }



}