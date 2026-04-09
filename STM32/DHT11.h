//=======================================
// DHT11模块头文件
//=======================================
#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

//=======================================
// 函数声明
//1. DHT11模块初始化
//2. 设置DHT11引脚模式
//3. 复位DHT11
//4. 等待DHT11的回应
//5. 从DHT11读取一个位
//6. 从DHT11读取一个字节
//7. 从DHT11读取一次数据
//=======================================
// 函数声明
//=======================================
void DHT11_Init(void);
void DHT11_SetMode(uint8_t mode);
void DHT11_Reset(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_ReadBit(void);
uint8_t DHT11_ReadByte(void);
uint8_t DHT11_ReadData(uint8_t *temp, uint8_t *humi);

#endif /* __DHT11_H */
