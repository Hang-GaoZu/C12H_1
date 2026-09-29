#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f1xx_hal.h"

/* 蓝牙初始化: USART2 中断接收 */
void Bluetooth_Init(void);

/* 蓝牙命令解析（在 USART2 中断中调用） */
void Bluetooth_RxCallback(uint8_t byte);

#endif /* __BLUETOOTH_H */
