//=======================================
// 串口模块头文件
//=======================================
#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

//=======================================
// 全局变量
//=======================================
//全局变量
extern uint8_t Serial_RxFlag;
extern uint8_t Serial_RxData;
extern uint8_t Serial_TxPacket[4];
extern uint8_t Serial_RxPacket[4];
extern uint8_t Serial_Rx_Data_Group[4];

//=======================================
// 函数声明
//1. 串口初始化
//2. 发送一个字节
//3. 发送数组
//4. 发送字符串
//5. 发送数字
//6. 格式化发送
//7. 获取接收标志
//8. 获取接收数据
//9. 发送数据包
//10. 处理接收数据
//=======================================
// 函数声明
//=======================================
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
void Serial_SendPacket(void);
void Serial_Receive_Data(uint8_t Data);

#endif