//=======================================
// 舵机模块头文件
//=======================================
#ifndef __SERVO_H
#define __SERVO_H

//=======================================
// 全局变量声明
//=======================================
extern int Now_Angle;
extern int Next_Angle;
extern int Servo_Change_Speed;

//=======================================
// 函数声明
//1. 舵机初始化
//2. 舵机设置角度
//3. 舵机当前角度设置
//4. 舵机下一个角度设置
//5. 舵机速度设置   
//6. 舵机角度差判断
//7. 舵机平滑角度变化
//8. 舵机当前角度读取
//=======================================
// 函数声明
//=======================================
void Servo_Init(void);                
void Servo_SetAngle(float Angle);      
void Now_Angle_Set(int angle);        
void Next_Angle_Set(int angle);       
void Speed_Set(int speed);                         
void Smooth_Change(void);              
int Now_Angle_Read(void);             



#endif

