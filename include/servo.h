#ifndef __SERVO_H
#define __SERVO_H

#include "pin_config.h"

/* 舵机初始化: TIM1 CH1 @ PA8, 50Hz PWM */
void Servo_Init(void);

/* 设置舵机角度: -45(左) ~ 0(中) ~ +45(右) */
void Servo_SetAngle(int8_t angle);

/* 舵机回正 */
void Servo_Center(void);

#endif /* __SERVO_H */
