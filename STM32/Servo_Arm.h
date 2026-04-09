#ifndef __SERVO_ARM_H
#define __SERVO_ARM_H

//=======================================
// 全局变量声明
//=======================================
extern int Arm_Now_Angle[4];
extern int Arm_Next_Angle[4];
extern int Arm_Change_Time;

// 函    数：舵机初始化
void Servo_Arm_Init(void);

// 函    数：舵机设置角度
// 参    数：index - 舵机索引（0-3）
// 参    数：angle - 角度值（0~180）
void Servo_Arm_Set_Angle(int index, int angle);

// 函    数：舵机速度设置
// 参    数：time - 时间间隔（ms）
void Servo_Arm_SetChangeTime(int time);

// 函    数：获取舵机当前角度
// 参    数：index - 舵机索引（0-3）
// 返    回：当前角度
int Servo_Arm_GetNowAngle(short index);

// 函    数：设置舵机当前角度
// 参    数：index - 舵机索引（0-3）
// 参    数：angle - 角度值（0~180）
void Servo_Arm_SetNowAngle(short index, int angle);

// 函    数：设置舵机目标角度
// 参    数：index - 舵机索引（0-3）
// 参    数：angle - 角度值（0~180）
void Servo_Arm_SetNextAngle(short index, int angle);

// 函    数：单个舵机平滑角度变化
// 参    数：index - 舵机索引（0-3）
void Servo_Arm_Smooth_Change(short index);

// 函    数：单个舵机平滑角度变化（直接指定目标角度）
// 参    数：index - 舵机索引（0-3）
// 参    数：nextAngle - 目标角度（0~180）
void Servo_Arm_Smooth_Change_Angle(short index, int nextAngle);

// 函    数：所有舵机返回初始状态
void Servo_Arm_AllBackInitState(void);

#endif