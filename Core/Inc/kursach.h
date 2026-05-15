#ifndef KURSACH_H
#define KURSACH_H

#include "../../CMSIS/Devices/STM32F4xx/Inc/stm32f4xx.h"
#include "../../CMSIS/Devices/STM32F4xx/Inc/STM32F411RE/stm32f411xe.h"

/* ===== GLOBAL VARIABLES ===== */

extern volatile uint16_t adc_left, adc_center, adc_right;

extern uint16_t min_left, max_left;
extern uint16_t min_center, max_center;
extern uint16_t min_right, max_right;

extern uint16_t thr_left, thr_center, thr_right;

extern uint8_t sensor_left, sensor_center, sensor_right;

/* --- КАЛИБРОВКА --- */
extern uint8_t calib_stage;
extern uint8_t sensor_id;

/* --- ДОБАВИЛ (из source) --- */
extern uint8_t calibration_busy;
extern uint8_t active_sensor;


/* ===== INIT ===== */
void GPIO_Init(void);
void RCC_Init(void);
void ADC_Init(void);

/* ===== ADC ===== */
uint16_t ADC_Read(uint8_t channel);
void LineSensors_ReadADC(void);

/* ===== CALIBRATION ===== */
void Calibration_Process(void);

/* ===== PROCESS ===== */
void LineSensors_Process(void);

/* ===== MOTORS ===== */
void Motor_Stop(void);
void Motor_Forward(void);

#endif