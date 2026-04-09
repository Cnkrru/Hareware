#ifndef __MOTOR_CAR_H
#define __MOTOR_CAR_H

//=======================================
// 全局变量声明
//=======================================
extern int8_t Now_Speed[4];
extern int8_t Next_Speed[4];
extern int Change_Time;

// 速度范围定义
#define SPEED_MAX 100
#define SPEED_MIN -100

// 函    数：四轮小车初始化
void Motor_Car_Init(void);

// 函    数：单个电机控制
// 参    数：index - 电机索引（0-3）
// 参    数：speed - 速度值（-100~100）
void Motor_Car_Control(int index, int8_t speed);

// 函    数：设置速度变化时间
// 参    数：time - 时间间隔（ms）
void Motor_Car_SetChangeTime(int time);

// 函    数：设置单个电机当前速度
// 参    数：index - 电机索引（0-3）
// 参    数：speed - 速度值
void Motor_Car_Set_NowSpeed(int index, int8_t speed);

// 函    数：设置单个电机目标速度
// 参    数：index - 电机索引（0-3）
// 参    数：speed - 速度值
void Motor_Car_Set_NextSpeed(int index, int8_t speed);

// 函    数：获取单个电机当前速度
// 参    数：index - 电机索引（0-3）
// 返    回：当前速度
int8_t Motor_Car_GetNowSpeed(int index);

// 函    数：小车平滑速度变化
void Motor_Car_Smooth_Go(void);

// 函    数：小车前进
// 参    数：speed - 速度值（0~100）
void Motor_Car_Forward(int8_t speed);

// 函    数：小车后退
// 参    数：speed - 速度值（0~100）
void Motor_Car_Backward(int8_t speed);

// 函    数：小车左转
// 参    数：speed - 速度值（0~100）
void Motor_Car_Left(int8_t speed);

// 函    数：小车右转
// 参    数：speed - 速度值（0~100）
void Motor_Car_Right(int8_t speed);

// 函    数：小车停止
void Motor_Car_Stop(void);

#endif