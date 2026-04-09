//=======================================
// RTC模块头文件
//=======================================
#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"

//=======================================
// 全局变量
//=======================================
// 全局时间数组，分别为年、月、日、时、分、秒
extern uint16_t RTC_Time[];

//=======================================
// 函数声明
//1. RTC设置时间
//2. RTC读取时间
//3. 获取时间戳
//4. 时间戳改写接口
//5. 获取星期几
//6. 标准时间格式
//7. 时间戳自增
//=======================================
// 函数声明
//=======================================
void RTC_SetTime(void);
void RTC_ReadTime(void);
uint32_t RTC_GetTimestamp(void);
void RTC_TimestampWrite(uint32_t timestamp);
uint8_t RTC_GetWeekday(void);
void RTC_StandardTime(char* buffer);
void RTC_IncrementTimestamp(void);

#endif