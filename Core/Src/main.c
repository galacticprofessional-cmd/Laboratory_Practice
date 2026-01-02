// LB1

#include "../Inc/init_LB1.h"

int main(void){
    
    GPIO_Init_CMSIS_LB1();


    int count = 0;

    while(1){

        if(pressedButton3()){
            count++;
        }
        
        int key = TheSwitcher(count);
     
        if (key == 0){
            if(pressedButton1()){
                TurningOnState();
            }
            else if(pressedButton2()){
                TurningOffState();

            }
        }
        else if(key == 1){
            if(pressedButton1()){
                TurningOffState();

            }
            else if(pressedButton2()){
                TurningOnState();

            }
        }

    }
    

}