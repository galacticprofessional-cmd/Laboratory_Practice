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

#define TIME_DELAY 100000
#define TIME_DELAY1 3500

int pressedButton1(void){
    if(READ_BIT(GPIOD->IDR , GPIO_IDR_IDR_2) == 0){
        for(int i = 0; i < TIME_DELAY1; i++){}
        if(READ_BIT(GPIOD->IDR , GPIO_IDR_IDR_2) == 0){
            while(READ_BIT(GPIOD->IDR , GPIO_IDR_IDR_2) == 0){
            }
            return 1;
        }

    }
    return 0;
}

int pressedButton2(void){
    if(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_2) == 0){
        for(int i = 0; i < TIME_DELAY1; i++){}
        if(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_2) == 0){
            while(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_2) == 0){
            }
            return 1;
        }

    }
    return 0;
}

int pressedButton3(void){
    if(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_3) == 0){
        for(int i = 0; i < TIME_DELAY1; i++){}
        if(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_3) == 0){
            while(READ_BIT(GPIOG->IDR , GPIO_IDR_IDR_3) == 0){
            }
            return 1;
        }

    }
    return 0;
}

void TurningOnState(void){
    if(READ_BIT(GPIOF->IDR , GPIO_IDR_IDR_13) == 0){
        SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS13);
    }

    else if(READ_BIT(GPIOE->IDR , GPIO_IDR_IDR_9) == 0){
        SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS9);
    }

    else if(READ_BIT(GPIOE->IDR , GPIO_IDR_IDR_11) == 0){
        SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS11);
    }

    else if(READ_BIT(GPIOF->IDR , GPIO_IDR_IDR_14) == 0){
        SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS14);
    }

    else if(READ_BIT(GPIOE->IDR , GPIO_IDR_IDR_13) == 0){
        SET_BIT(GPIOE->BSRR, GPIO_BSRR_BS13);
    }

    else if(READ_BIT(GPIOF->IDR , GPIO_IDR_IDR_15) == 0){
        SET_BIT(GPIOF->BSRR, GPIO_BSRR_BS15);
    }

}

void TurningOffState(){
    if(READ_BIT(GPIOF->IDR , GPIO_IDR_IDR_15) != 0){
        SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR15);
    }

    else if(READ_BIT(GPIOE->IDR , GPIO_IDR_IDR_13) != 0){
        SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR13);
    }

    else if(READ_BIT(GPIOF->IDR , GPIO_IDR_IDR_14) != 0){
        SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR14);
    }
    else if(READ_BIT(GPIOE->IDR , GPIO_IDR_IDR_11) != 0){
        SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR11);
    }
    else if(READ_BIT(GPIOE->IDR , GPIO_IDR_IDR_9) != 0){
        SET_BIT(GPIOE->BSRR, GPIO_BSRR_BR9);
    }

    else if(READ_BIT(GPIOF->IDR , GPIO_IDR_IDR_13) != 0){
        SET_BIT(GPIOF->BSRR, GPIO_BSRR_BR13);
    }

}

int TheSwitcher(int count){
    return count & 1;
}

int main(void){
    
    GPIO_Init_CMSIS_LB1();
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
}