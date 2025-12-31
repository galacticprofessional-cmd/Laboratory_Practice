#include "../Inc/init.h"

__attribute__((used)) volatile uint32_t heartbeat = 0;
__attribute__((used)) volatile uint32_t ledOn = 0;
__attribute__((used)) volatile uint32_t buttonCPushed = 0;
__attribute__((used)) volatile uint32_t buttonDPushed = 0;
volatile uint8_t g_btn_event = 0;
volatile uint32_t g_btn_timer = 0;


float a;
float b; 
float c;

#define TIME_DELAY1 3500

int pressedButton_PC13(void)
{
    if (READ_BIT(GPIOC->IDR, GPIO_IDR_IDR_13) == 0)
    {
        for (volatile int i = 0; i < TIME_DELAY1; i++) {}

        if (READ_BIT(GPIOC->IDR, GPIO_IDR_IDR_13) == 0)
        {
            while (READ_BIT(GPIOC->IDR, GPIO_IDR_IDR_13) == 0)
            {
                // ждём отпускания

            }
            return 1;

        }
    }
    return 0;
}

int pressedButton_PD2(void)
{
    if (READ_BIT(GPIOD->IDR, GPIO_IDR_IDR_2) == 0)
    {
        for (volatile int i = 0; i < TIME_DELAY1; i++) {}

        if (READ_BIT(GPIOD->IDR, GPIO_IDR_IDR_2) == 0)
        {
            while (READ_BIT(GPIOD->IDR, GPIO_IDR_IDR_2) == 0)
            {
                // ждём отпускания

            }
            return 1;

        }
    }
    return 0;
}

void Wait(void)
{
    if (g_btn_timer > 0)
    {
        g_btn_timer--;
        if (g_btn_timer == 0)
        {
            g_btn_event = 0;
        }
    }
}

int main(void)
{
    GPIO_Init_CMSIS();
    while (1)
    {

        if(pressedButton_PD2()){
            g_btn_event = 1;
            g_btn_timer = 60000;
        }

        if(g_btn_event){
            SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5);
        }else{
            SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5);
        }
        Wait();
        // if (g_btn_event){
        //     for (volatile int i = 0; i < 60000; i++) {}
        //     g_btn_event = 0;
        // }
    //     SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5);
    //     ledOn = 1; 

    //     heartbeat = 0;

    //     while(heartbeat < 60000){
    //         heartbeat++;
    //         c = a/b;
    //     }

    //     SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5);
    //     ledOn = 0; 
    //     heartbeat = 0;
    //     while(heartbeat < 60000){
    //         heartbeat++;
    //         c = a/b;
    //     }
    //     buttonCPushed = 0;
    //     buttonDPushed = 1;
    }
}