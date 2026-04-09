//=======================================
// RGB模块头文件
//=======================================
#ifndef __RGB_H
#define __RGB_H

#include "stm32f10x.h"

//=======================================
// 函数声明
//1. RGB模块初始化
//2. 设置红色亮度
//3. 设置绿色亮度
//4. 设置蓝色亮度
//5. 设置颜色
//=======================================
// 函数声明
//=======================================
void RGB_Init(void);
void RGB_Red(uint8_t duty);
void RGB_Green(uint8_t duty);
void RGB_Blue(uint8_t duty);
void RGB_SetColor(uint8_t r, uint8_t g, uint8_t b);

#endif
