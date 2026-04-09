#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"
//=======================================
// 全局变量部分
//=======================================
#define Speed_Max 100
#define Speed_Min -100
int8_t Motor_Now_Speed = 0;
int8_t Motor_Next_Speed = 0;
int Motor_Change_Time=100;

//==============================================================================
// 函    数：直流电机初始化
// 参    数：无
// 返 回 值：无
//==============================================================================
void Motor_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//开启TIM2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		// 开启AFIO时钟，用于引脚复用
	
	// 禁用JTAG，保留SWD功能，释放PB3和PB4作为普通IO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
    //电机引脚（使用Motor_car模块的一号电机引脚：B3, B4）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//将PB3和PB4引脚初始化为推挽输出	
	//驱动模块的PWM引脚（使用Motor_car模块的一号电机PWM引脚：PA0）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							
	
	//配置时钟源
	TIM_InternalClockConfig(TIM2);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	//时基单元初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;					//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                 //计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;               //预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元
	
	//输出比较初始化
	TIM_OCInitTypeDef TIM_OCInitStructure;									//定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);                         //结构体初始化，若结构体没有完整赋值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;									//初始的CCR值
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);                        //将结构体变量交给TIM_OC1Init，配置TIM2的输出比较通道1
	
	//TIM使能
	TIM_Cmd(TIM2, ENABLE);			//使能TIM2，定时器开始运行
}

//=======================================
// 函    数：电机PWM设置（最内层函数）
// 参    数：Speed - 速度值
//=======================================
void Motor_PWM_Set(int8_t Speed)
{
	TIM_SetCompare1(TIM2, Speed);		//PWM设置为速度值
}

//=======================================
// 函    数：电机控制
// 参    数：Speed - 速度值
// 参    数：Arah - 方向，1为正转，0为反转
//=======================================
void Motor_Control(uint8_t Arah, int8_t Speed)
{
	if(Arah == 1) // 正转
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);	//PB3置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);	//PB4置低电平，设置方向为正转
	}
	else if(Arah == 0) // 反转
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_3);	//PB3置低电平
		GPIO_SetBits(GPIOB, GPIO_Pin_4);	//PB4置高电平，设置方向为反转
	}
	Motor_PWM_Set(Speed);		//PWM设置为速度值
}

//=======================================
// 函    数：电机速度设置
// 参    数：Speed - 速度值
//=======================================
void Motor_Set_Speed(int8_t Speed)
{
	if(Speed > 0)
	{
		Motor_Control(1, Speed);
	}
	else if(Speed < 0)
	{
		Motor_Control(0, -Speed);
	}
}

//=======================================
// 函    数：电机当前速度设置（包含速度判断）
// 参    数：speed 要设置的当前速度
//=======================================
void Motor_NowSpeed_Set(int8_t speed)
{
    if(speed > Speed_Max)
    {
        Motor_Now_Speed = Speed_Max;
    }
    else if(speed < Speed_Min)
    {
        Motor_Now_Speed = Speed_Min;
    }
    else
    {
        Motor_Now_Speed = speed;
    }
}

//=======================================
// 函    数：电机下一个速度设置（包含速度判断）
// 参    数：speed 要设置的下一个速度
//=======================================
void Motor_NextSpeed_Set(int8_t speed)
{
    if(speed > Speed_Max)
    {
        Motor_Next_Speed = Speed_Max;
    }
    else if(speed < Speed_Min)
    {
        Motor_Next_Speed = Speed_Min;
    }
    else
    {
        Motor_Next_Speed = speed;
    }
}

void Motor_Change_Time_Set(int8_t motor_delay)
{
    Motor_Change_Time = motor_delay;
}
//=======================================
// 函    数：电机平滑速度变化
//=======================================
void Motor_Smooth_Change(void)
{
    int Temp = Motor_Next_Speed - Motor_Now_Speed;
    int Difference = Temp > 0 ? Temp : -Temp;
    
    if(Motor_Now_Speed < Motor_Next_Speed)
    {
        for(int i = 0; i < Difference; i++)
        {
            Motor_Now_Speed++;
            if(Motor_Now_Speed > 0)
            {
                Motor_Control(1, Motor_Now_Speed);  
            }
            else if(Motor_Now_Speed < 0)
            {
                Motor_Control(0, -Motor_Now_Speed);
            }
            Delay_ms(Motor_Change_Time);
        }
    }
    else if(Motor_Now_Speed > Motor_Next_Speed)
    {
        for(int i = 0; i < Difference; i++)
        {
            Motor_Now_Speed--;
            if(Motor_Now_Speed > 0)
            {
                Motor_Control(1, Motor_Now_Speed);
            }
            else if(Motor_Now_Speed < 0)
            {
                Motor_Control(0, -Motor_Now_Speed);
            }
            Delay_ms(Motor_Change_Time);
        }
    }
}

//=======================================
// 函    数：电机当前速度读取
// 返回值  ：当前速度
//=======================================
int8_t Motor_NowSpeed_Read(void)
{
    return Motor_Now_Speed;
}

//=======================================
// 函    数：电机平滑停止
//=======================================
void Motor_Stop(void)
{   
    int state=Motor_NowSpeed_Read();
    Motor_NowSpeed_Set(state);
    Motor_NextSpeed_Set(0);
    Motor_Smooth_Change();
}
