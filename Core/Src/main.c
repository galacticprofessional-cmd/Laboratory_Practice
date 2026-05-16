#include "kursach.h"

int main(void)
{
    /* --- INIT --- */
    RCC_Init();
    GPIO_Init();
    ADC_Init();

    while (1)
    {
        /* 1. Калибровка (всегда доступна) */
        Calibration_Process();

        /* 2. Чтение ADC */
        LineSensors_ReadADC();

        /* 3. Обработка */
        LineSensors_Process();

        if (sensor_center)
        {
            Motor_Forward();
        }
        else
        {
            Motor_Stop();
        }

    }
}