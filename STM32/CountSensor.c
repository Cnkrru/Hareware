#include "stm32f10x.h"
#include "misc.h"

// 静态变量
static unsigned short CountSensor_Count = 0;
unsigned short g_count_min = 0;
unsigned short g_count_max = 9999;

//=======================================
// 计数传感器初始化
//=======================================
void CountSensor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

//=======================================
// 获取计数传感器的计数值
//=======================================
unsigned short CountSensor_Get(void)
{
    return CountSensor_Count;
}

//=======================================
// 重置计数
//=======================================
void CountSensor_Reset(void)
{
    CountSensor_Count = g_count_min;
}

//=======================================
// 中断内层函数
//=======================================
void CountSensor_InterruptInner(void)
{
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0)
    {
        if(CountSensor_Count < g_count_max)
        {
            CountSensor_Count++;
        }
        else
        {
            CountSensor_Count = g_count_min;
        }
    }
}

//=======================================
// EXTI9_5外部中断函数
//=======================================
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line8) == SET)
    {
        CountSensor_InterruptInner();
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}
