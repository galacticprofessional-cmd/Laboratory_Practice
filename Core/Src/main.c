#include "init_LB2.h"

int main(void)
{
    RCC_Init();
    GPIO_Init();
    TIM3_PWM_Init();

    while (1)
    {
        Motor_SetSpeed_Left(100);
        Motor_SetSpeed_Right(100);
    }
}
