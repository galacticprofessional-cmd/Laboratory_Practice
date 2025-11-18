// #include "init_LB2.h"
// #include "interrupt.h"

// volatile uint8_t btnCount = 0, flag = 0;
// volatile uint32_t global_systick_tim_count = 0;

// volatile uint32_t second = 0;
// volatile uint16_t second_tim_count;


// int main(void){
    
//     RCC_Init();
//     ITR_Init();
//     GPIO_Init_CMSIS_LB2();


//     while(1){
//         if(second_tim_count >= 1000){
//             second++;
//         }
//         if (second % 2){
//             SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
//         }
//         else{
//             SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7);
//         }

//         // if(flag){
//         //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS7);
//         // }else{
//         //     SET_BIT(GPIOB->BSRR, GPIO_BSRR_BR7);
//         // }
//     }
// }



// LB1

#include "../Inc/init_LB1.h"



void ledOn(int n){
    switch(n){
        case 0:
            SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS13); // PF13 ON
            break;
        case 1:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS9);  // PE9 ON
            break;
        case 2:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS11); // PE11 ON
            break;
        case 3:
            SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS14); // PF14 ON
            break;
        case 4:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS13); // PE13 ON
            break;
        case 5:
            SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS15); // PF15 ON
            break;
    }
}

void ledOff(int n){
    switch(n){
        case 0:
            SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR13); // PF13 OFF
            break;
        case 1:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR9);  // PE9 OFF
            break;
        case 2:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR11); // PE11 OFF
            break;
        case 3:
            SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR14); // PF14 OFF
            break;
        case 4:
            SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR13); // PE13 OFF
            break;
        case 5:
            SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR15); // PF15 OFF
            break;
    }
}

int changeSpeed(int speedIndex){
    switch(speedIndex){
        case 0:
            return 0;

        case 1:
            return 500000;

        case 2:
            return 200000;

        case 3:
            return 80000;

        case 4:
            return 30000;
    }

    return 0;
}

void blinkLed(int ledNum, int *ledState){
    if (*ledState == 1){
        ledOff(ledNum);
        *ledState = 0;
    } else {
        ledOn(ledNum);
        *ledState = 1;
    }
}

int main(void){
    
    GPIO_Init_CMSIS_LB1();

    // LB1 main task 
    // int count = 0;

    // while(1){

    //     if(pressedButton3() == 1){
    //         count++;
    //     }
        
    //     int key = TheSwitcher(count);
     
    //     if (key == 0){
    //         if(pressedButton1() == 1){
    //             TurningOnState();
    //         }
    //         else if(pressedButton2() == 1){
    //             TurningOffState();

    //         }
    //     }
    //     else if(key == 1){
    //         if(pressedButton1() == 1){
    //             TurningOffState();

    //         }
    //         else if(pressedButton2() == 1){
    //             TurningOnState();

    //         }
    //     }

    // }

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