#include "init_LB2.h"
#include "interrupt.h"

#define POROG 2000

int main(void)
{
    RCC_Init();
    SysTick_Init();
    GPIO_Init_CMSIS_LB2();
    
    while (1)
    {
        

    }
}
