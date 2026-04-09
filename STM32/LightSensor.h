//=======================================
// 光照传感器模块头文件
//=======================================
#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

#include "stm32f10x.h"

#define LDR_READ_TIMES  10

//=======================================
// 函数声明
//1. 光敏传感器初始化(PA8)
//2. 读取ADC值
//3. 读取平均ADC值
//4. 读取光照强度(勒克斯)
//5. 读取光照强度百分比
//=======================================
// 函数声明
//=======================================
void LightSensor_Init(void);
uint16_t Light_ADC_Read(void);
uint16_t Light_Average_Data(void);
uint16_t Light_LuxData(void);
uint8_t Light_LuxPercentage(void);

#endif
