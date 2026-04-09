//=======================================
// 热敏传感器模块头文件
//=======================================
#ifndef __TEMP_SENSOR_H
#define __TEMP_SENSOR_H

#include "stm32f10x.h"

#define TEMP_READ_TIMES  10

//=======================================
// 函数声明
//1. 热敏传感器初始化
//2. 读取ADC值
//3. 读取平均ADC值
//4. 读取温度值
//=======================================
// 函数声明
//=======================================
void TempSensor_Init(void);
uint16_t Temp_ADC_Read(void);
uint16_t Temp_Average_Data(void);
int16_t Temp_GetValue(void);

#endif
