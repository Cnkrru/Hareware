#include "stm32f10x.h"
#include "DHT11.h"
#include "Delay.h"

//=======================================
// DHT11模块初始化
//=======================================
void DHT11_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_5);    
}

//=======================================
// 设置DHT11引脚模式
// 参数：mode - 0为输入模式，1为输出模式
//=======================================
void DHT11_SetMode(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if (mode)
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    }
    else
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    }
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//=======================================
// 复位DHT11
//=======================================
void DHT11_Reset(void)
{
    DHT11_SetMode(1);
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    Delay_ms(20); // 约20ms
    GPIO_SetBits(GPIOA, GPIO_Pin_5);
    Delay_us(13); // 约13us
}

//=======================================
// 等待DHT11的回应
// 返回值：0-存在，1-未检测到
//=======================================
uint8_t DHT11_Check(void)
{
    uint8_t retry = 0;
    DHT11_SetMode(0);
    
    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    
    if (retry >= 100) return 1;
    
    retry = 0;
    while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) && retry < 100)    
    {
        retry++;
        Delay_us(1);
    }
    
    if (retry >= 100) return 1;
    
    return 0;
}

//=======================================
// 从DHT11读取一个位
// 返回值：1/0
//=======================================
uint8_t DHT11_ReadBit(void)
{
    uint8_t retry = 0;
    
    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    
    retry = 0;
    while (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    
    Delay_us(40); // 40us延时
    
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)) return 1;
    else return 0;
}

//=======================================
// 从DHT11读取一个字节
// 返回值：读到的数据
//=======================================
uint8_t DHT11_ReadByte(void)
{
    uint8_t i, dat = 0;
    
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_ReadBit();
    }
    
    return dat;
}

//=======================================
// 从DHT11读取一次数据
// 参数：temp - 温度值指针(范围:0~50°)
//       humi - 湿度值指针(范围:20%~90%)
// 返回值：0-正常，1-读取失败
//=======================================
uint8_t DHT11_ReadData(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;
    
    DHT11_Reset();
    
    if (DHT11_Check() == 0)
    {
        for (i = 0; i < 5; i++)
        {
            buf[i] = DHT11_ReadByte();
        }
        
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
        }
    }
    else return 1;
    
    return 0;
}
