#include "oled.h"
#include "oledfont.h"
#include "stm32f1xx_hal.h"

/* --- Pin helpers --------------------------------------------------------- */
/* PB13 = SCK, PB12 = MOSI (MOSI), PA5 = DC, PC4 = RST */
#define SCK(v)   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, v ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define MOSI(v)  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, v ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define DC(v)     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, v ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define RST(v)    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, v ? GPIO_PIN_SET : GPIO_PIN_RESET)

static void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
    DC(cmd ? 1 : 0);
    for (int8_t i = 7; i >= 0; i--) {
        SCK(0);
        MOSI((dat >> i) & 1);
        SCK(1);
        SCK(0);
    }
}

void OLED_Init(void)
{
    GPIO_InitTypeDef g = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    g.Mode  = GPIO_MODE_OUTPUT_PP;
    g.Pull  = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_HIGH;

    g.Pin = GPIO_PIN_12 | GPIO_PIN_13;   /* MOSI + SCK */
    HAL_GPIO_Init(GPIOB, &g);

    g.Pin = GPIO_PIN_5;   /* DC */
    HAL_GPIO_Init(GPIOA, &g);

    g.Pin = GPIO_PIN_4;     /* RST */
    HAL_GPIO_Init(GPIOC, &g);

    /* hardware reset */
    RST(0);
    HAL_Delay(200);
    RST(1);
    HAL_Delay(200);

    /* SSD1306 init sequence */
    OLED_WR_Byte(0xAE, OLED_CMD);
    OLED_WR_Byte(0xD5, OLED_CMD); OLED_WR_Byte(0x80, OLED_CMD);
    OLED_WR_Byte(0xA8, OLED_CMD); OLED_WR_Byte(0x3F, OLED_CMD);
    OLED_WR_Byte(0xD3, OLED_CMD); OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0x40, OLED_CMD);
    OLED_WR_Byte(0x8D, OLED_CMD); OLED_WR_Byte(0x14, OLED_CMD);
    OLED_WR_Byte(0x20, OLED_CMD); OLED_WR_Byte(0x02, OLED_CMD);
    OLED_WR_Byte(0xA1, OLED_CMD);
    OLED_WR_Byte(0xC8, OLED_CMD);
    OLED_WR_Byte(0xDA, OLED_CMD); OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0x81, OLED_CMD); OLED_WR_Byte(0xEF, OLED_CMD);
    OLED_WR_Byte(0xD9, OLED_CMD); OLED_WR_Byte(0xF1, OLED_CMD);
    OLED_WR_Byte(0xDB, OLED_CMD); OLED_WR_Byte(0x30, OLED_CMD);
    OLED_WR_Byte(0xA4, OLED_CMD);
    OLED_WR_Byte(0xA6, OLED_CMD);
    OLED_WR_Byte(0xAF, OLED_CMD);  /* display on */
    OLED_Clear();
}

void OLED_Clear(void)
{
    for (uint8_t i = 0; i < 8; i++) {
        OLED_WR_Byte(0xB0 + i, OLED_CMD);
        OLED_WR_Byte(0x01, OLED_CMD);
        OLED_WR_Byte(0x10, OLED_CMD);
        for (uint16_t n = 0; n < 128; n++)
            OLED_WR_Byte(0, OLED_DATA);
    }
}

void OLED_SetPos(uint8_t x, uint8_t y)
{
    OLED_WR_Byte(0xB0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xF0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0F) | 0x01, OLED_CMD);
}

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch, uint8_t size)
{
    uint8_t c = ch - ' ';
    OLED_SetPos(x, y);
    for (uint8_t i = 0; i < size; i++)
        OLED_WR_Byte(oled_asc2_1206[c][i], OLED_DATA);
    OLED_SetPos(x, y + 1);
    for (uint8_t i = 0; i < size; i++)
        OLED_WR_Byte(oled_asc2_1206[c][i + size], OLED_DATA);
}

void OLED_ShowString(uint8_t x, uint8_t y, const char *str, uint8_t size)
{
    while (*str) {
        OLED_ShowChar(x, y, *str, size);
        x += size / 2;
        str++;
    }
}