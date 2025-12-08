#include "../Inc/init_LB1_Macros.h"

void GPIO_Init_Macros_LB1(void){

    BIT_SET     (RCC_AHB1ENR, RCC_GPIOD_EN | RCC_GPIOG_EN | RCC_GPIOE_EN | RCC_GPIOF_EN);

    BIT_SET     (GPIOF_MODER,      MODER13_0);          //Настройка пина PF13 на вывод, регистр MODER
    BIT_CLEAR   (GPIOF_OTYPER,     OTYPER13);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    BIT_SET     (GPIOF_OSPEEDR,    OSPEEDR13_0);     //Настройка скорости работы вывода PF13, регистр OSPEEDR
    BIT_SET     (GPIOF_BSRR,       BSRR_BR13);                //Предварительное выключение светодиода, регистр BSR, бит BR13

    BIT_SET     (GPIOE_MODER,      MODER9_0);          //Настройка пина PE9 на вывод, регистр MODER
    BIT_CLEAR   (GPIOE_OTYPER,     OTYPER9);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    BIT_SET     (GPIOE_OSPEEDR,    OSPEEDR9_0);     //Настройка скорости работы вывода PE9, регистр OSPEEDR
    BIT_SET     (GPIOE_BSRR,       BSRR_BR9);                //Предварительное выключение светодиода, регистр BSR, бит BR9

    BIT_SET     (GPIOE_MODER,      MODER11_0);          //Настройка пина PE11 на вывод, регистр MODER
    BIT_CLEAR   (GPIOE_OTYPER,     OTYPER11);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    BIT_SET     (GPIOE_OSPEEDR,    OSPEEDR11_0);     //Настройка скорости работы вывода PE11, регистр OSPEEDR
    BIT_SET     (GPIOE_BSRR,       BSRR_BR11);                //Предварительное выключение светодиода, регистр BSR, бит BR11

    BIT_SET     (GPIOF_MODER,      MODER14_0);          //Настройка пина PF14 на вывод, регистр MODER
    BIT_CLEAR   (GPIOF_OTYPER,     OTYPER14);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    BIT_SET     (GPIOF_OSPEEDR,    OSPEEDR14_0);     //Настройка скорости работы вывода PF14, регистр OSPEEDR
    BIT_SET     (GPIOF_BSRR,       BSRR_BR14);                //Предварительное выключение светодиода, регистр BSR, бит BR14

    BIT_SET     (GPIOE_MODER,      MODER13_0);          //Настройка пина PE13 на вывод, регистр MODER
    BIT_CLEAR   (GPIOE_OTYPER,     OTYPER13);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    BIT_SET     (GPIOE_OSPEEDR,    OSPEEDR13_0);     //Настройка скорости работы вывода PE13, регистр OSPEEDR
    BIT_SET     (GPIOE_BSRR,       BSRR_BR13);                //Предварительное выключение светодиода, регистр BSR, бит BR13

    BIT_SET     (GPIOF_MODER,      MODER15_0);          //Настройка пина PF15 на вывод, регистр MODER
    BIT_CLEAR   (GPIOF_OTYPER,     OTYPER15);              //Настройка режима работы выxода на push-pull, регистр OTYPER
    BIT_SET     (GPIOF_OSPEEDR,    OSPEEDR15_0);     //Настройка скорости работы вывода PF15, регистр OSPEEDR
    BIT_SET     (GPIOF_BSRR,       BSRR_BR15);                //Предварительное выключение светодиода, регистр BSR, бит BR15

}


int pressedButton1(void){
    if(BIT_READ(GPIOD_IDR, IDR_2) == 0){
        for(int i = 0; i < TIME_DELAY1; i++){}
        if(BIT_READ(GPIOD_IDR, IDR_2) == 0){
            while(BIT_READ(GPIOD_IDR, IDR_2) == 0){
            }
            return 1;
        }

    }
    return 0;
}

int pressedButton2(void){
    if(BIT_READ(GPIOG_IDR, IDR_2) == 0){
        for(int i = 0; i < TIME_DELAY1; i++){}
        if(BIT_READ(GPIOG_IDR, IDR_2) == 0){
            while(BIT_READ(GPIOG_IDR, IDR_2) == 0){
            }
            return 1;
        }

    }
    return 0;
}

int pressedButton3(void){
    if(BIT_READ(GPIOG_IDR, IDR_3) == 0){
        for(int i = 0; i < TIME_DELAY1; i++){}
        if(BIT_READ(GPIOG_IDR, IDR_3) == 0){
            while(BIT_READ(GPIOG_IDR, IDR_3) == 0){
            }
            return 1;
        }

    }
    return 0;
}

void TurningOnState(void){
    if(BIT_READ(GPIOF_IDR, IDR_13) == 0){
        BIT_SET(GPIOF_BSRR, BSRR_BS13);
    }

    else if(BIT_READ(GPIOE_IDR, IDR_9) == 0){
        BIT_SET(GPIOE_BSRR, BSRR_BS9);
    }

    else if(BIT_READ(GPIOE_IDR, IDR_11) == 0){
        BIT_SET(GPIOE_BSRR, BSRR_BS11);
    }

    else if(BIT_READ(GPIOF_IDR, IDR_14) == 0){
        BIT_SET(GPIOF_BSRR, BSRR_BS14);
    }

    else if(BIT_READ(GPIOE_IDR, IDR_13) == 0){
        BIT_SET(GPIOE_BSRR, BSRR_BS13);
    }

    else if(BIT_READ(GPIOF_IDR, IDR_15) == 0){
        BIT_SET(GPIOF_BSRR, BSRR_BS15);
    }

}

void TurningOffState(){
    if(BIT_READ(GPIOF_IDR, IDR_15) != 0){
        BIT_SET(GPIOF_BSRR, BSRR_BR15);
    }

    else if(BIT_READ(GPIOE_IDR, IDR_13) != 0){
        BIT_SET(GPIOE_BSRR, BSRR_BR13);
    }

    else if(BIT_READ(GPIOF_IDR, IDR_14) != 0){
        BIT_SET(GPIOF_BSRR, BSRR_BR14);
    }
    else if(BIT_READ(GPIOE_IDR, IDR_11) != 0){
        BIT_SET(GPIOE_BSRR, BSRR_BR11);
    }
    else if(BIT_READ(GPIOE_IDR, IDR_9) != 0){
        BIT_SET(GPIOE_BSRR, BSRR_BR9);
    }

    else if(BIT_READ(GPIOF_IDR, IDR_13) != 0){
        BIT_SET(GPIOF_BSRR, BSRR_BR13);
    }

}

int TheSwitcher(int count){
    return count & 1;
}
