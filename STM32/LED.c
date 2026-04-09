#include "stm32f10x.h"

//=======================================
// LED模块初始化
//=======================================
void LED_model_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);								
			
}

//=======================================
// LED1点亮
//=======================================
void LED_ON(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_7);		//设置PB7引脚为高电平
}

//=======================================
// LED熄灭
//=======================================
void LED_OFF(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);		//设置PB7引脚为低电平
}