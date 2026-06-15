#include "init.h"
#include "interrupt.h"

int main(void)
{
    RCC_init();
    GPIO_init();
    SysTick_init();
    button_interrupt_init();
    TIMER2_init();

    while (1)
    {
        __WFI();
    }
}