//=======================================
// 计数传感器模块头文件
//=======================================
#ifndef __COUNTSENSOR_H__
#define __COUNTSENSOR_H__

//=======================================
// 全局变量
//=======================================
// 全局变量声明
extern unsigned short g_count_min;  // 计数最小值
extern unsigned short g_count_max;  // 计数最大值

//=======================================
// 函数声明
//1. 计数传感器初始化
//2. 获取计数传感器的计数值
//3. 重置计数
//4. 中断内层函数
//=======================================
// 函数声明
//=======================================
void CountSensor_Init(void);
unsigned short CountSensor_Get(void);
void CountSensor_Reset(void);
void CountSensor_InterruptInner(void);
#endif
