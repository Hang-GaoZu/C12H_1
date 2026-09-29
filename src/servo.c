#include "servo.h"

static TIM_HandleTypeDef htim1;

void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* 开启时钟 */
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* PA8 — TIM1_CH1 复用推挽输出 */
    GPIO_InitStruct.Pin   = SERVO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SERVO_PORT, &GPIO_InitStruct);

    /* TIM1 时基配置: 72MHz / 72 / 20000 = 50Hz */
    htim1.Instance               = SERVO_TIM;
    htim1.Init.Prescaler         = SERVO_PWM_PRESCALER;
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim1.Init.Period            = SERVO_PWM_PERIOD;
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&htim1);

    /* PWM 通道配置 */
    sConfigOC.OCMode       = TIM_OCMODE_PWM1;
    sConfigOC.Pulse        = SERVO_PWM_CENTER;  /* 初始中点 */
    sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, SERVO_TIM_CHANNEL);

    /* 启动 PWM (TIM1 是高级定时器，需要使能 MOE) */
    HAL_TIM_PWM_Start(&htim1, SERVO_TIM_CHANNEL);
    __HAL_TIM_MOE_ENABLE(&htim1);
}

void Servo_SetAngle(int8_t angle)
{
    /* 限幅 -45 ~ +45 */
    if (angle < -45) angle = -45;
    if (angle > 45)  angle = 45;

    /* 线性映射: angle(-45~+45) → CCR(1000~2000) */
    /* CCR = 1500 + angle * 1000 / 90 ≈ 1500 + angle * 11 */
    int16_t ccr = SERVO_PWM_CENTER + (int16_t)angle * 1000 / 90;

    /* 安全限幅 */
    if (ccr < 500)  ccr = 500;
    if (ccr > 2500) ccr = 2500;

    __HAL_TIM_SET_COMPARE(&htim1, SERVO_TIM_CHANNEL, (uint16_t)ccr);
}

void Servo_Center(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, SERVO_TIM_CHANNEL, SERVO_PWM_CENTER);
}
