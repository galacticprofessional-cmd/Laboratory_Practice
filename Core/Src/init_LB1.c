#include "../Inc/init_LB1.h"

void GPIO_Init_CMSIS_LB1(void){

    SET_BIT     (RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN);

    SET_BIT     (GPIOF->MODER,      GPIO_MODER_MODER13_0);          //Настройка пина PF13 на вывод, регистр MODER
    CLEAR_BIT   (GPIOF->OTYPER,     GPIO_OTYPER_OT13);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT     (GPIOF->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR13_0);     //Настройка скорости работы вывода PF13, регистр OSPEEDR
    SET_BIT     (GPIOF->BSRR,       GPIO_BSRR_BR13);                //Предварительное выключение светодиода, регистр BSR, бит BR13

    SET_BIT     (GPIOE->MODER,      GPIO_MODER_MODER9_0);          //Настройка пина PE9 на вывод, регистр MODER
    CLEAR_BIT   (GPIOE->OTYPER,     GPIO_OTYPER_OT9);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    SET_BIT     (GPIOE->OSPEEDR,    GPIO_OSPEEDER_OSPEEDR9_0);     //Настройка скорости работы вывода PE9, регистр OSPEEDR
    SET_BIT     (GPIOE->BSRR,       GPIO_BSRR_BR9);                //Предварительное выключение светодиода, регистр BSR, бит BR9

}

void GPIO_Init_Macros_LB1(void){

    BIT_SET     (GPIOE_MODER,      MODER11_0);                      //Настройка пина PE11 на вывод, регистр MODER
    BIT_CLEAR   (GPIOE_OTYPER,     OTYPER11);                       //Настройка режима работы выxода на push-pull, регистр OTYPER
    BIT_SET     (GPIOE_OSPEEDR,    OSPEEDR11_0);                    //Настройка скорости работы вывода PE11, регистр OSPEEDR
    BIT_SET     (GPIOE_BSRR,       BSRR_BR11);                      //Предварительное выключение светодиода, регистр BSR, бит BR11

    BIT_SET     (GPIOF_MODER,      MODER14_0);                      //Настройка пина PF14 на вывод, регистр MODER
    BIT_CLEAR   (GPIOF_OTYPER,     OTYPER14);                       //Настройка режима работы выxода на push-pull, регистр OTYPER
    BIT_SET     (GPIOF_OSPEEDR,    OSPEEDR14_0);                    //Настройка скорости работы вывода PF14, регистр OSPEEDR
    BIT_SET     (GPIOF_BSRR,       BSRR_BR14);                      //Предварительное выключение светодиода, регистр BSR, бит BR14

}

void GPIO_Init_Memory_LB1(void){
    
    *(uint32_t *)(0x40021000UL + 0x00UL) |= 0x04000000UL;          //Настройка пина PE13 на вывод, регистр MODER           
    *(uint32_t *)(0x40021000UL + 0x04UL) &= ~(0x1UL << 13U);       //Настройка режима работы выxода на push-pull, регистр OTYPER
    *(uint32_t *)(0x40021000UL + 0x08UL) |= (0x1UL << 26U);        //Настройка скорости работы вывода PE13, регистр OSPEEDR
    *(uint32_t *)(0x40021000UL + 0x18UL) |= (0x1UL << 29U);        //Предварительное выключение светодиода, регистр BSR, бит BR13

    *(uint32_t *)(0x40021400UL + 0x00UL) |= (0x40000000UL);        //Настройка пина PF15 на вывод, регистр MODER
    *(uint32_t *)(0x40021400UL + 0x04UL) &= ~(0x1UL << 15U);       //Настройка режима работы выxода на push-pull, регистр OTYPER
    *(uint32_t *)(0x40021400UL + 0x08UL) |= (0x1UL << 30U);        //Настройка скорости работы вывода PF15, регистр OSPEEDR
    *(uint32_t *)(0x40021400UL + 0x18UL) |= (0x1UL << 31U);        //Предварительное выключение светодиода, регистр BSR, бит BR15

}


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

void TurningOffState(void){

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


// Extra task functions


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

