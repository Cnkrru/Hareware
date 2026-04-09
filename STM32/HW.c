#include "HW.h"

//=======================================
// 红外循迹传感器初始化
//=======================================
void HW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//=======================================
// 获取红外传感器数据
// 返回值：传感器状态 (0: 未检测到, 1: 检测到)
//=======================================
uint16_t HW_GetData(void)
{
    uint16_t HW_Data;
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == Bit_RESET)
    {
        HW_Data = 0;
    }
    else
    {
        HW_Data = 1;
    }

    return HW_Data;
}
