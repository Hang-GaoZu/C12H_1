#include "stm32f1xx_hal.h"
#include "car_control.h"
#include "bluetooth.h"

/* huart2 定义在这里，bluetooth.c 中 extern 引用 */
UART_HandleTypeDef huart2;

void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* 依次初始化 LED、OLED、舵机、电机 */
    Car_Init();

    /* 初始化蓝牙（USART2 中断接收） */
    Bluetooth_Init();

    while (1) {
        /* 主循环：根据蓝牙指令执行运动并刷新 OLED */
        Car_Update();
        HAL_Delay(10);
    }
}

/* USART2 中断服务函数：接收蓝牙单个字节命令 */
void USART2_IRQHandler(void)
{
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET) {
        uint8_t rx_byte = (uint8_t)(huart2.Instance->DR & 0x00FF);
        Bluetooth_RxCallback(rx_byte);
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};

    /* HSE 8MHz * PLL9 = 72MHz */
    osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc.HSEState       = RCC_HSE_ON;
    osc.PLL.PLLState   = RCC_PLL_ON;
    osc.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    osc.PLL.PLLMUL     = RCC_PLL_MUL9;
    HAL_RCC_OscConfig(&osc);

    clk.ClockType       = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
    clk.AHBCLKDivider   = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider  = RCC_HCLK_DIV2;
    clk.APB2CLKDivider  = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2);
}