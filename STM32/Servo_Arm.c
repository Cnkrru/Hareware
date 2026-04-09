#include "stm32f10x.h"
#include "Delay.h"

//=======================================
// 全局变量部分
//=======================================
#define ANGLE_MAX 180
#define ANGLE_MIN 0
int Arm_Now_Angle[4] = {0, 0, 0, 0}; // 四个舵机的当前角度
int Arm_Next_Angle[4] = {0, 0, 0, 0}; // 四个舵机的目标角度
int Arm_Change_Time = 10; // 角度变化的时间间隔

//=======================================
// 函    数：舵机初始化
//=======================================
void Servo_Arm_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//配置时钟源
	TIM_InternalClockConfig(TIM4);		//选择TIM4为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	//时基单元初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;					//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;			//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;			//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM4的时基单元

	//输出比较初始化
	TIM_OCInitTypeDef TIM_OCInitStructure;							//定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);                         //结构体初始化，若结构体没有完整赋值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;						//初始的CCR值
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);                        //将结构体变量交给TIM_OC1Init，配置TIM4的输出比较通道1

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;						//初始的CCR值
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);                        //将结构体变量交给TIM_OC2Init，配置TIM4的输出比较通道2

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;						//初始的CCR值
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);                        //将结构体变量交给TIM_OC3Init，配置TIM4的输出比较通道3

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;						//初始的CCR值
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);                        //将结构体变量交给TIM_OC4Init，配置TIM4的输出比较通道4
    
	//TIM使能
	TIM_Cmd(TIM4, ENABLE);			//使能TIM4，定时器开始运行
}

//=======================================
// 函    数：舵机设置角度
// 参    数：index - 舵机索引（0-3）
// 参    数：angle - 角度值（0~180）
//=======================================
void Servo_Arm_Set_Angle(int index, int angle)
{
    if(index < 0 || index > 3)
    {
        return;
    }
    
    // 角度范围检查
    if(angle > ANGLE_MAX)
    {
        angle = ANGLE_MAX;
    }
    else if(angle < ANGLE_MIN)
    {
        angle = ANGLE_MIN;
    }
    
    // 设置角度
    switch(index)
    {
        case 0:
            TIM_SetCompare1(TIM4, (int)(angle / 180.0 * 2000 + 500));	//设置占空比
            break;
        case 1:
            TIM_SetCompare2(TIM4, (int)(angle / 180.0 * 2000 + 500));	//设置占空比
            break;
        case 2:
            TIM_SetCompare3(TIM4, (int)(angle / 180.0 * 2000 + 500));	//设置占空比
            break;
        case 3:
            TIM_SetCompare4(TIM4, (int)(angle / 180.0 * 2000 + 500));	//设置占空比	
            break;
    }
    
    // 更新当前角度
    Arm_Now_Angle[index] = angle;
}

//=======================================
// 函    数：舵机速度设置
// 参    数：time - 时间间隔（ms）
//=======================================
void Servo_Arm_SetChangeTime(int time)
{
    if(time > 0)
    {
        Arm_Change_Time = time;
    }
}

//=======================================
// 函    数：获取舵机当前角度
// 参    数：index - 舵机索引（0-3）
// 返    回：当前角度
//=======================================
int Servo_Arm_GetNowAngle(short index)
{
    if(index < 0 || index > 3)
    {
        return -1;
    }
    return Arm_Now_Angle[index];
}

//=======================================
// 函    数：设置舵机当前角度
// 参    数：index - 舵机索引（0-3）
// 参    数：angle - 角度值（0~180）
//=======================================
void Servo_Arm_SetNowAngle(short index, int angle)
{
    if(index < 0 || index > 3)
    {
        return;
    }
    
    // 角度范围检查
    if(angle > ANGLE_MAX)
    {
        angle = ANGLE_MAX;
    }
    else if(angle < ANGLE_MIN)
    {
        angle = ANGLE_MIN;
    }
    
    Arm_Now_Angle[index] = angle;
}

//=======================================
// 函    数：设置舵机目标角度
// 参    数：index - 舵机索引（0-3）
// 参    数：angle - 角度值（0~180）
//=======================================
void Servo_Arm_SetNextAngle(short index, int angle)
{
    if(index < 0 || index > 3)
    {
        return;
    }
    
    // 角度范围检查
    if(angle > ANGLE_MAX)
    {
        angle = ANGLE_MAX;
    }
    else if(angle < ANGLE_MIN)
    {
        angle = ANGLE_MIN;
    }
    
    Arm_Next_Angle[index] = angle;
}

//=======================================
// 函    数：单个舵机平滑角度变化
// 参    数：index - 舵机索引（0-3）
//=======================================
void Servo_Arm_Smooth_Change(short index)
{
    if(index < 0 || index > 3)
    {
        return;
    }
    
    if(Arm_Now_Angle[index] == Arm_Next_Angle[index])
    {
        return;
    }
    
    int Temp = Arm_Next_Angle[index] - Arm_Now_Angle[index];
    int Difference = Temp > 0 ? Temp : -Temp;

    if(Arm_Now_Angle[index] < Arm_Next_Angle[index])
    {
        for(int i = 0; i < Difference; i++)
        {
            Arm_Now_Angle[index]++;
            Servo_Arm_Set_Angle(index, Arm_Now_Angle[index]);
            Delay_ms(Arm_Change_Time);
        }
    }
    else if(Arm_Now_Angle[index] > Arm_Next_Angle[index])
    {
        for(int i = 0; i < Difference; i++)
        {
            Arm_Now_Angle[index]--;
            Servo_Arm_Set_Angle(index, Arm_Now_Angle[index]);
            Delay_ms(Arm_Change_Time);
        }
    }
}

//=======================================
// 函    数：单个舵机平滑角度变化（直接指定目标角度）
// 参    数：index - 舵机索引（0-3）
// 参    数：nextAngle - 目标角度（0~180）
//=======================================
void Servo_Arm_Smooth_Change_Angle(short index, int nextAngle)
{
    if(index < 0 || index > 3)
    {
        return;
    }
    
    // 角度范围检查
    if(nextAngle > ANGLE_MAX)
    {
        nextAngle = ANGLE_MAX;
    }
    else if(nextAngle < ANGLE_MIN)
    {
        nextAngle = ANGLE_MIN;
    }
    
    // 获取当前角度
    int nowAngle = Servo_Arm_GetNowAngle(index);
    if(nowAngle == -1)
    {
        return;
    }
    
    // 设置当前角度和目标角度
    Servo_Arm_SetNowAngle(index, nowAngle);
    Servo_Arm_SetNextAngle(index, nextAngle);
    
    // 执行平滑角度变化
    Servo_Arm_Smooth_Change(index);
}

//=======================================
// 函    数：所有舵机返回初始状态
//=======================================
void Servo_Arm_AllBackInitState(void)
{
    for(int i = 0; i < 4; i++)
    {
        Servo_Arm_SetNextAngle(i, 90);
        Servo_Arm_Smooth_Change(i);
    }
}
