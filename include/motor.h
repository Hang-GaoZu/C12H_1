#ifndef __MOTOR_H
#define __MOTOR_H

#include "pin_config.h"

/* 电机初始化: TIM8 CH1+CH2 PWM + 方向GPIO */
void Motor_Init(void);

/* 设置电机速度
 * speed_a, speed_b: -7199 ~ +7199
 *   >0 前进, <0 后退, =0 停止
 */
void Motor_Set(int16_t speed_a, int16_t speed_b);

/* 停止两个电机 */
void Motor_Stop(void);

#endif /* __MOTOR_H */
