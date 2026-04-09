//=======================================
// 电机模块头文件
//=======================================
#ifndef __MOTOR_H
#define __MOTOR_H

//=======================================
// 全局变量声明
//=======================================
extern int8_t Motor_Now_Speed;
extern int8_t Motor_Next_Speed;
extern int Motor_Change_Time;

//=======================================
// 函数声明
//1. 电机初始化
//2. 电机PWM设置（最内层函数）
//3. 电机控制
//4. 电机速度设置
//5. 电机当前速度设置
//6. 电机下一个速度设置
//7. 电机平滑速度变化
//8. 电机当前速度读取
//9. 电机停止
//=======================================
// 函数声明
//=======================================
void Motor_Init(void);
void Motor_PWM_Set(int8_t Speed);
void Motor_Control(uint8_t Arah, int8_t Speed);
void Motor_Set_Speed(int8_t Speed);
void Motor_NowSpeed_Set(int8_t speed);
void Motor_NextSpeed_Set(int8_t speed);
void Motor_Smooth_Change(void);
int8_t Motor_NowSpeed_Read(void);
void Motor_Stop(void);

#endif
