#include "motor.h"

static TIM_HandleTypeDef htim8;

void Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* 开启时钟 */
    __HAL_RCC_TIM8_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* ---- 方向控制 GPIO ---- */
    /* PB4, PB5 — 电机A方向 */
    GPIO_InitStruct.Pin   = GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* PC12 — 电机B方向1 */
    GPIO_InitStruct.Pin   = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* PD2 — 电机B方向2 */
    GPIO_InitStruct.Pin   = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* 默认方向 = 停止 */
    AIN1 = 0; AIN2 = 0;
    BIN1 = 0; BIN2 = 0;

    /* ---- PWM GPIO: PC6 (CH1), PC7 (CH2) ---- */
    GPIO_InitStruct.Pin   = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* ---- TIM8 时基: 72MHz / 1 / 7200 = 10kHz ---- */
    htim8.Instance               = TIM8;
    htim8.Init.Prescaler         = 0;
    htim8.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim8.Init.Period            = 7199;   /* ARR */
    htim8.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim8.Init.RepetitionCounter = 0;
    htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&htim8);

    /* PWM 通道配置 */
    sConfigOC.OCMode       = TIM_OCMODE_PWM1;
    sConfigOC.Pulse        = 0;    /* 初始占空比 = 0 */
    sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1);  /* PC6 电机B */
    HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2);  /* PC7 电机A */

    /* 启动 PWM */
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
    __HAL_TIM_MOE_ENABLE(&htim8);

    /* 使能 D24A 驱动板 */
    STBY = 1;
}

void Motor_Set(int16_t speed_a, int16_t speed_b)
{
    int16_t pwm;
    uint16_t ccr;

    /* ---- 电机A (PC7 = TIM8_CH2) ---- */
    if (speed_a == 0) {
        AIN1 = 0; AIN2 = 0;
        ccr = 0;
    } else if (speed_a > 0) {
        pwm = speed_a;
        if (pwm > 7199) pwm = 7199;
        AIN1 = 0; AIN2 = 1;   /* 前进 */
        ccr = (uint16_t)pwm;
    } else {
        pwm = -speed_a;
        if (pwm > 7199) pwm = 7199;
        AIN1 = 1; AIN2 = 0;   /* 后退 */
        ccr = (uint16_t)pwm;
    }
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, ccr);

    /* ---- 电机B (PC6 = TIM8_CH1) ---- */
    if (speed_b == 0) {
        BIN1 = 0; BIN2 = 0;
        ccr = 0;
    } else if (speed_b > 0) {
        pwm = speed_b;
        if (pwm > 7199) pwm = 7199;
        BIN1 = 0; BIN2 = 1;   /* 前进 */
        ccr = (uint16_t)pwm;
    } else {
        pwm = -speed_b;
        if (pwm > 7199) pwm = 7199;
        BIN1 = 1; BIN2 = 0;   /* 后退 */
        ccr = (uint16_t)pwm;
    }
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, ccr);
}

void Motor_Stop(void)
{
    AIN1 = 0; AIN2 = 0;
    BIN1 = 0; BIN2 = 0;
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 0);
}
