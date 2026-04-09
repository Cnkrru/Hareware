#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//=======================================
// 按键初始化
//=======================================
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOB的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PA9引脚初始化为上拉输入
}

//=======================================
// 获取按键值
// 返回值：按键键码，0表示无按键按下
//=======================================
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;		//定义变量，默认键码值为0
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0)			//读PA9输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(40);														//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == 0);	//等待按键松手
		Delay_ms(40);														//延时消抖
		KeyNum = 1;														//置键码为1
	}
	return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}
