 #include "init_LB2.h"

int main(void)
{
    RCC_Init();                 // Тактирование (HSE + PLL 100 МГц)
    GPIO_Init();                // Настройка GPIO для моторов и ШИМ
    TIM3_PWM_Init();            // Настройка ШИМ на TIM3 (PC6, PC7)
    LineSensors_GPIO_Init();    // Настройка пинов датчиков линии (PA6, PA7, PB1)

    while (1)
    {
        LineSensors_Read();     // Опрос датчиков
        Line_CalcError();       // Вычисление ошибки положения линии
        Robot_Move();           // Расчёт и установка скоростей моторов

        // Небольшая задержка для стабильности (~10 мс при 100 МГц)
        for (volatile uint32_t i = 0; i < 500000; i++);
    }
}
