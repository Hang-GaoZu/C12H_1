#ifndef __PIN_CONFIG_H
#define __PIN_CONFIG_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_cortex.h"
#include "stm32f1xx_hal_flash.h"
#include "stm32f1xx_hal_pwr.h"

/*============================================================
 *  Bitband GPIO 操作宏
 *  移植自参考代码 sys.h，兼容 GCC 和 Keil
 *  用法: PBout(4)=1;   PCin(3);
 *============================================================*/
#define BITBAND(addr, bitnum)  ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr)         *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

/* GPIO ODR 地址 */
#define GPIOA_ODR_Addr    (GPIOA_BASE + 0x0C)
#define GPIOB_ODR_Addr    (GPIOB_BASE + 0x0C)
#define GPIOC_ODR_Addr    (GPIOC_BASE + 0x0C)
#define GPIOD_ODR_Addr    (GPIOD_BASE + 0x0C)

/* GPIO IDR 地址 */
#define GPIOA_IDR_Addr    (GPIOA_BASE + 0x08)
#define GPIOB_IDR_Addr    (GPIOB_BASE + 0x08)
#define GPIOC_IDR_Addr    (GPIOC_BASE + 0x08)
#define GPIOD_IDR_Addr    (GPIOD_BASE + 0x08)

/* 输出操作 */
#define PAout(n)          BIT_ADDR(GPIOA_ODR_Addr, n)
#define PBout(n)          BIT_ADDR(GPIOB_ODR_Addr, n)
#define PCout(n)          BIT_ADDR(GPIOC_ODR_Addr, n)
#define PDout(n)          BIT_ADDR(GPIOD_ODR_Addr, n)

/* 输入操作 */
#define PAin(n)           BIT_ADDR(GPIOA_IDR_Addr, n)
#define PBin(n)           BIT_ADDR(GPIOB_IDR_Addr, n)
#define PCin(n)           BIT_ADDR(GPIOC_IDR_Addr, n)
#define PDin(n)           BIT_ADDR(GPIOD_IDR_Addr, n)


/*============================================================
 *  引脚定义 — 根据 CLAUDE.md 引脚分配表
 *============================================================*/

/* ---------- D24A 使能 ---------- */
#define STBY              PCout(2)

/* ---------- 电机A ---------- */
/* PWM: PC7 = TIM8_CH2 */
#define MOTOR_A_PWM_PIN       GPIO_PIN_7
#define MOTOR_A_PWM_PORT      GPIOC
#define MOTOR_A_PWM_TIM       TIM8
#define MOTOR_A_PWM_CHANNEL   TIM_CHANNEL_2
/* 方向: PB4=A_DIR1, PB5=A_DIR2 */
#define AIN1              PBout(4)
#define AIN2              PBout(5)

/* ---------- 电机B ---------- */
/* PWM: PC6 = TIM8_CH1 */
#define MOTOR_B_PWM_PIN       GPIO_PIN_6
#define MOTOR_B_PWM_PORT      GPIOC
#define MOTOR_B_PWM_TIM       TIM8
#define MOTOR_B_PWM_CHANNEL   TIM_CHANNEL_1
/* 方向: PC12=B_DIR1, PD2=B_DIR2 */
#define BIN1              PCout(12)
#define BIN2              PDout(2)


/* ---------- 舵机 (TIM1_CH1 PA8) ---------- */
#define SERVO_PIN             GPIO_PIN_8
#define SERVO_PORT            GPIOA
#define SERVO_TIM             TIM1
#define SERVO_TIM_CHANNEL     TIM_CHANNEL_1

/* 舵机PWM参数: 72MHz/72/20000 = 50Hz */
#define SERVO_PWM_PRESCALER   (72 - 1)
#define SERVO_PWM_PERIOD      (20000 - 1)   /* 20ms = 50Hz */
#define SERVO_PWM_CENTER      1500           /* 1.5ms 中点 */
#define SERVO_PWM_LEFT        1000           /* 1.0ms 左转 */
#define SERVO_PWM_RIGHT       2000           /* 2.0ms 右转 */


/* ---------- LED ---------- */
#define LED                   PBout(14)


#endif /* __PIN_CONFIG_H */
