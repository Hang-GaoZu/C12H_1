#include "car_control.h"
#include "motor.h"
#include "servo.h"
#include "oled.h"
#include "pin_config.h"

/* 全局运动状态 */
volatile CarState g_car_state = CAR_STOP;

/* 运动参数 */
#define MOTOR_SPEED_FULL     3600    /* 约50%占空比 */
#define MOTOR_SPEED_HALF     1800    /* 转向内侧减速 */

/* 当前显示的状态文字，用于判断是否需要刷新OLED */
static CarState last_display_state = CAR_STOP;

void Car_Init(void)
{
    /* LED 初始 */
    LED = 0;

    /* OLED 初始化 */
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, "BT Car Ready", 12);
    OLED_ShowString(0, 2, "Waiting cmd...", 12);

    /* 舵机初始化并回正 */
    Servo_Init();
    Servo_Center();

    /* 电机初始化并停止 */
    Motor_Init();
    Motor_Stop();

    g_car_state = CAR_STOP;
    last_display_state = CAR_STOP;
}

void Car_SetState(CarState state)
{
    g_car_state = state;
}

void Car_Stop(void)
{
    g_car_state = CAR_STOP;
    Motor_Stop();
    Servo_Center();
    LED = 0;
}

void Car_Update(void)
{
    uint16_t base_speed = MOTOR_SPEED_FULL;

    switch (g_car_state) {
    case CAR_STOP:
        Motor_Stop();
        Servo_Center();
        LED = 0;
        break;

    case CAR_FORWARD:
        Motor_Set(base_speed, base_speed);
        Servo_Center();
        LED = 1;
        break;

    case CAR_BACKWARD:
        Motor_Set(-base_speed, -base_speed);
        Servo_Center();
        LED = 1;
        break;

    case CAR_TURN_LEFT:
        /* 舵机左转 + 内侧(左)电机减速 */
        Motor_Set(MOTOR_SPEED_HALF, base_speed);
        Servo_SetAngle(-30);   /* 左转30度，需实际调试 */
        LED = 1;
        break;

    case CAR_TURN_RIGHT:
        /* 舵机右转 + 内侧(右)电机减速 */
        Motor_Set(base_speed, MOTOR_SPEED_HALF);
        Servo_SetAngle(30);    /* 右转30度，需实际调试 */
        LED = 1;
        break;

    case CAR_HORN:
        /* 蜂鸣器后续加装，先只显示 */
        break;

    case CAR_LIGHT:
        /* 灯光后续加装，先只显示 */
        LED = !LED;
        break;

    default:
        g_car_state = CAR_STOP;
        break;
    }

    /* OLED 显示更新（仅在状态变化时刷新，减少闪烁） */
    if (g_car_state != last_display_state) {
        last_display_state = g_car_state;

        OLED_Clear();

        /* 第0行: 标题 */
        OLED_ShowString(0, 0, "== BT Car ==", 12);

        /* 第2行: 当前指令 */
        OLED_ShowString(0, 2, "Cmd: ", 12);
        switch (g_car_state) {
        case CAR_STOP:      OLED_ShowString(48, 2, "STOP", 12);    break;
        case CAR_FORWARD:   OLED_ShowString(48, 2, "FORWARD", 12); break;
        case CAR_BACKWARD:  OLED_ShowString(48, 2, "BACKWARD", 12);break;
        case CAR_TURN_LEFT: OLED_ShowString(48, 2, "LEFT", 12);    break;
        case CAR_TURN_RIGHT:OLED_ShowString(48, 2, "RIGHT", 12);   break;
        case CAR_HORN:      OLED_ShowString(48, 2, "HORN", 12);    break;
        case CAR_LIGHT:     OLED_ShowString(48, 2, "LIGHT", 12);   break;
        default:            OLED_ShowString(48, 2, "---", 12);     break;
        }
    }
}
