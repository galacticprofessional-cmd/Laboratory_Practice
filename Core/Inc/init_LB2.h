#ifndef INIT_LB2_H
#define INIT_LB2_H

#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include <stdint.h>

/* ===================== PWM ===================== */
#define PWM_MAX        200
#define BASE_SPEED     70

/* ===================== PID ===================== */
#define KP             25
#define KD             0
#define KI             0

/* ===================== GLOBAL VARIABLES ===================== */
extern volatile uint8_t sensor_left;
extern volatile uint8_t sensor_center;
extern volatile uint8_t sensor_right;

extern volatile int8_t line_error;

/* ===================== INIT ===================== */
void RCC_Init(void);
void GPIO_Init(void);
void TIM3_PWM_Init(void);
void LineSensors_GPIO_Init(void);

/* ===================== SENSORS ===================== */
void LineSensors_Read(void);
void Line_CalcError(void);

/* ===================== MOTORS ===================== */
void Motor_SetSpeed_Left(int16_t speed);
void Motor_SetSpeed_Right(int16_t speed);
void Robot_Move(void);

#endif
