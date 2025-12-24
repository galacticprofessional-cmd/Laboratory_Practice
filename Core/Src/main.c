#include "init_LB2_hw.h"

int main(void)
{
    GPIO_Init_LEDs();
    GPIO_Init_Button_PD2_Pulldown();
    EXTI_Init_PD2();
    SysTick_Init_1ms();

    while (1)
    {
        Button_Process();
        Blink_Process();
    }
}
