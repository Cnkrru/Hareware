//=======================================
// 备份寄存器模块头文件
//=======================================
#ifndef __BKP_H
#define __BKP_H

#include "stm32f10x.h"

//=======================================
// 函数声明
//1. 备份寄存器初始化
//2. 向备份寄存器写入数据
//3. 从备份寄存器读取数据
//4. 向备份寄存器写入数组
//5. 从备份寄存器读取数组
//=======================================
// 函数声明
//=======================================
void BKP_Init(void);
void BKP_WriteData(uint8_t reg, uint16_t data);
void BKP_ReadData(uint8_t reg, uint16_t *data);
void BKP_WriteArray(uint16_t *array, uint8_t length);
void BKP_ReadArray(uint16_t *array, uint8_t length);

#endif