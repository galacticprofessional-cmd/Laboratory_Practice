#include "interrupt.h"

volatile uint32_t global_systick_tim_count = 0;

uint8_t FLAG_DELAY;
uint32_t delay_count;

// volatile uint8_t btnCount = 0, flag = 0;

void EXTI15_10_IRQHandler(){
    SET_BIT(EXTI->PR, EXTI_PR_PR12);
    btnCount++;
    flag = !flag;

}

void Systick_Handler(void){
    global_systick_tim_count++;
    // second_tim_count;
    // if (FLAG_DELAY){
    // delay_count++;

    // }
}

void delay(uint32_t del){
    FLAG_DELAY = 1;
    while(del >= delay_count){}
    delay_count = 0;
    FLAG_DELAY =0;
}