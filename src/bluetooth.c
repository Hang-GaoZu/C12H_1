#include "bluetooth.h"
#include "car_control.h"
#include "pin_config.h"

/* huart2 定义在 main.c 中 */
extern UART_HandleTypeDef huart2;

void Bluetooth_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 开启时钟 */
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* PA2 = USART2_TX (复用推挽) */
    GPIO_InitStruct.Pin   = GPIO_PIN_2;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA3 = USART2_RX (浮空输入) */
    GPIO_InitStruct.Pin  = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 配置: 9600 baud, 8N1 */
    huart2.Instance          = USART2;
    huart2.Init.BaudRate     = 9600;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);

    /* 使能 RX 中断 */
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

    /* NVIC 配置 */
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/* 蓝牙命令解析 — 在 USART2 中断中调用 */
void Bluetooth_RxCallback(uint8_t byte)
{
    switch (byte) {
        case '1':   Car_SetState(CAR_BACKWARD);  break;
        case '2':   Car_SetState(CAR_FORWARD);   break;
        case '3':   Car_SetState(CAR_TURN_LEFT); break;
        case '4':   Car_SetState(CAR_TURN_RIGHT);break;
        case '5':   Car_SetState(CAR_HORN);      break;
        case '6':   Car_SetState(CAR_LIGHT);     break;
        default:    Car_SetState(CAR_STOP);      break;
    }
}
