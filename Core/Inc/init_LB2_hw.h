#ifndef INIT_LB2_HW
#define INIT_LB2_HW

#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F411RE/stm32f411xe.h"

/* ===================== КОНСТАНТЫ ===================== */

#define LED_COUNT           6U
#define BTN_DEBOUNCE_MS     100U
#define PRESS_2S_MS         2000U
#define PRESS_4S_MS         4000U

/* ===================== ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ===================== */

/* Таймер */
extern volatile uint32_t g_ms;

/* Кнопка PD2 */
extern volatile uint32_t g_btn_last_irq_ms;
extern volatile uint8_t  g_btn_press_event;
extern volatile uint8_t  g_btn_release_event;
extern volatile uint8_t  g_btn_is_pressed;
extern volatile uint32_t g_btn_press_start_ms;

/* MCU Viewer */
extern volatile uint32_t mcu_btn_press_count;
extern volatile uint32_t mcu_action_short_count;
extern volatile uint32_t mcu_action_2s_count;
extern volatile uint32_t mcu_action_4s_count;

/* Светодиоды */
extern volatile int      currentLed;
extern volatile uint8_t  blinkModeEnabled;
extern volatile uint8_t  blinkFreqIndex;
extern volatile uint8_t  blinkLedIsOn;
extern volatile uint32_t lastBlinkTime_ms;

/* ===================== ФУНКЦИИ ===================== */

/* Init */
void GPIO_Init_LEDs(void);
void GPIO_Init_Button_PD2_Pulldown(void);
void EXTI_Init_PD2(void);
void SysTick_Init_1ms(void);

/* LED */
void LED_On_Index(int n);
void LED_Off_Index(int n);
void LED_AllOff(void);
void LED_Next(void);

/* Logic */
void Button_Process(void);
void Blink_Process(void);

#endif
