#ifndef __CAR_CONTROL_H
#define __CAR_CONTROL_H

#include "stm32f1xx_hal.h"

/* 小车运动状态 */
typedef enum {
    CAR_STOP = 0,
    CAR_FORWARD,
    CAR_BACKWARD,
    CAR_TURN_LEFT,
    CAR_TURN_RIGHT,
    CAR_HORN,        /* 鸣笛 */
    CAR_LIGHT        /* 亮灯 */
} CarState;

/* 全局运动状态，由蓝牙中断修改，主循环读取 */
extern volatile CarState g_car_state;

/* 小车初始化（依次初始化 OLED、舵机、电机、蓝牙） */
void Car_Init(void);

/* 主循环调用：根据 g_car_state 执行运动 + 更新 OLED */
void Car_Update(void);

/* 直接设置运动状态（供蓝牙回调调用） */
void Car_SetState(CarState state);

/* 紧急停止 */
void Car_Stop(void);

#endif /* __CAR_CONTROL_H */
