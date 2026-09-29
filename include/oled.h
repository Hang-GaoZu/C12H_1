#ifndef OLED_H
#define OLED_H

#include "stm32f1xx_hal.h"

#define OLED_CMD   0
#define OLED_DATA 1

void OLED_Init(void);
void OLED_Clear(void);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, const char *p, uint8_t size);

#endif