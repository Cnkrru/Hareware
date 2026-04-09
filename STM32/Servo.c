#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//=======================================
// 全局变量部分
//=======================================
#define Angle_Max 180
#define Angle_Min 0
int Now_Angle=0;
int Next_Angle=90;
int Servo_Change_Speed=10;

//=======================================
// 函    数：舵机初始化
//=======================================
void Servo_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						

	/*配置时钟源*/
	TIM_InternalClockConfig(TIM4);		//选择TIM4为内部时钟，若不调用此函数，TIM默认也为内部时钟

	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;					//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;			//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;			//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM4的时基单元

	/*输出比较初始化*/ 
	TIM_OCInitTypeDef TIM_OCInitStructure;							//定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);                         //结构体初始化，若结构体没有完整赋值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;						//初始的CCR值
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);                        //将结构体变量交给TIM_OC1Init，配置TIM4的输出比较通道1

	/*TIM使能*/
	TIM_Cmd(TIM4, ENABLE);			//使能TIM4，定时器开始运行
}

//=======================================
// 函    数：舵机设置角度
// 参    数：Angle 要设置的舵机角度，范围：0~180
//=======================================
void Servo_SetAngle(float Angle)
{
	TIM_SetCompare1(TIM4, Angle / 180 * 2000 + 500);	//设置占空比
}

//=======================================
// 函    数：舵机当前角度设置（包含角度判断）
// 参    数：angle 要设置的当前角度
//=======================================
void Now_Angle_Set(int angle)
{
    if(angle>Angle_Max) 
    {
        Now_Angle=Angle_Max;
    }
    else if(angle<Angle_Min)
    {
        Now_Angle=Angle_Min;
    }
    else
    {
        Now_Angle=angle;
    }
}

//=======================================
// 函    数：舵机下一个角度设置（包含角度判断）
// 参    数：angle 要设置的下一个角度
//=======================================
void Next_Angle_Set(int angle)
{
    if(angle>Angle_Max) 
    {
        Next_Angle=Angle_Max;
    }
    else if(angle<Angle_Min)
    {
        Next_Angle=Angle_Min;
    }
    else
    {
        Next_Angle=angle;
    }
}

//=======================================
// 函    数：舵机速度设置
// 参    数：speed 要设置的速度(数值越小越快
//=======================================
void Speed_Set(int Delay_time)
{
    if(Delay_time>0)
    {
        Servo_Change_Speed=Delay_time;
    }
}

//=======================================
// 函    数：舵机平滑角度变化
//=======================================
void Smooth_Change(void)
{
    int Temp = Next_Angle - Now_Angle;
    int Difference = Temp > 0 ? Temp : -Temp;

    if(Now_Angle<Next_Angle)
    {
        for(int i=0;i<Difference;i++)
        {
            Now_Angle+=1;
            Servo_SetAngle(Now_Angle);
            Delay_ms(Servo_Change_Speed);
        }
    }
    else if(Now_Angle>Next_Angle)
    {
        for(int i=0;i<Difference;i++)
        {
            Now_Angle-=1;
            Servo_SetAngle(Now_Angle);
            Delay_ms(Servo_Change_Speed);
        }
    }
}

//=======================================
// 函    数：舵机当前角度读取
// 返回值  ：当前角度
//=======================================
int Now_Angle_Read(void)
{
    return Now_Angle;
}

//=======================================
//已实现算法
//1.PWM与舵机角度映射
//2.平滑转动
//=======================================


//=======================================
// 算法：梯形速度变化
// 平滑加速,匀速运动,平滑减速
// 加速与减速过程用平滑转动算法就行
//=======================================
