#include "init_LB2.h"

int main(void)
{
    RCC_Init();
    // GPIO_Init();
    // TIM3_PWM_Init();
    // LineSensors_GPIO_Init();
    LineSensor_ADC_Init();

    while (1)
    {
        // Motor_SetSpeed_Left(100);
        // Motor_SetSpeed_Right(100);
        LineSensor_Update();
        line_sensor_adc = LineSensor_ReadFiltered();



    }
}
