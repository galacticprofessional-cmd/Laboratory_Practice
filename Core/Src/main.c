// LB1

#include "../Inc/init_LB1.h"

int main(void){
    
    GPIO_Init_CMSIS_LB1();
    GPIO_Init_Macros_LB1();
    GPIO_Init_Memory_LB1();


    int count = 0;

    while(1){

        if(pressedButton3() == 1){
            count++;
        }
        
        int key = TheSwitcher(count);
     
        if (key == 0){
            if(pressedButton1() == 1){
                TurningOnState();
            }
            else if(pressedButton2() == 1){
                TurningOffState();

            }
        }
        else if(key == 1){
            if(pressedButton1() == 1){
                TurningOffState();

            }
            else if(pressedButton2() == 1){
                TurningOnState();

            }
        }

    }

    // LB1 extra task 

    int currentLed = -1;
    int ledState   = 1;       
    int speedIndex = 0;        
    int delayValue = 0;

    while(1){
        if (pressedButton1() == 1) {  
            if (currentLed != -1) {
                ledOff(currentLed);
            }

            currentLed++;          
            if (currentLed >= 6) { 
                currentLed = 0;    
            }

            ledOn(currentLed);
            ledState = 1;
            speedIndex = 0;
            delayValue = 0;
        }

        if (pressedButton2() == 1) {  
            if (currentLed != -1) {
                ledOff(currentLed);
            }

            currentLed--;          
            if (currentLed < 0) { 
                currentLed = 5;    
            }

            ledOn(currentLed);
            ledState = 1;
            speedIndex = 0;
            delayValue = 0;
        }

    if (pressedButton3() == 1) {
        speedIndex++;
        if (speedIndex > 4) speedIndex = 0;  

        delayValue = changeSpeed(speedIndex);
    }


    if (currentLed != -1) {
        if (speedIndex == 0) {
            ledOn(currentLed);
            ledState = 1;
        } else {
            blinkLed(currentLed, &ledState);

            for (volatile int i = 0; i < delayValue; i++){}
        }
    }
    }
}