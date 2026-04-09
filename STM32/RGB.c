//=======================================
// RGB LED模块 (PWM调光)
//=======================================
// 引脚连接：
//   PA8  -> R (红色) - TIM1_CH1
//   PA9  -> G (绿色) - TIM1_CH2
//   PA10 -> B (蓝色) - TIM1_CH3
//=======================================
// 函数说明：
// 1. RGB_Init       - 初始化RGB模块(PWM)
// 2. RGB_Red       - 红色PWM调光 (0-255)
// 3. RGB_Green     - 绿色PWM调光 (0-255)
// 4. RGB_Blue      - 蓝色PWM调光 (0-255)
// 5. RGB_SetColor  - 设置RGB颜色 (0-255)
//=======================================

#include "stm32f10x.h"
#include "RGB.h"

//=======================================
// RGB模块初始化(PWM)
//=======================================
void RGB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = 255 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 900 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

//=======================================
// 红色PWM调光
//=======================================
void RGB_Red(uint8_t duty)
{
    TIM_SetCompare1(TIM1, duty);
}

//=======================================
// 绿色PWM调光
//=======================================
void RGB_Green(uint8_t duty)
{
    TIM_SetCompare2(TIM1, duty);
}

//=======================================
// 蓝色PWM调光
//=======================================
void RGB_Blue(uint8_t duty)
{
    TIM_SetCompare3(TIM1, duty);
}

//=======================================
// 设置RGB颜色
//=======================================
void RGB_SetColor(uint8_t r, uint8_t g, uint8_t b)
{
    RGB_Red(r);
    RGB_Green(g);
    RGB_Blue(b);
}
