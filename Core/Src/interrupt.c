#include "interrupt.h"

extern volatile uint8_t btnCount = 0, flag = 0;

void EXTI15_10_IRQHandler(){
    SET_BIT(EXTI->PR, EXTI_PR_PR12);
    btnCount++;
    flag = !flag;

}