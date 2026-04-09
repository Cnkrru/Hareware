#include "stm32f10x.h"
#include "Motor_car.h"
#include "Delay.h"

//=======================================
// 全局变量部分
//=======================================
#define SPEED_MAX 100
#define SPEED_MIN -100

// 电机速度变量
int8_t Now_Speed[4] = {0, 0, 0, 0}; // 四个电机的当前速度
int8_t Next_Speed[4] = {0, 0, 0, 0}; // 四个电机的目标速度
int Change_Time = 50; // 速度变化的时间间隔



//=======================================
// 函    数：四轮小车初始化
//=======================================
void Motor_Car_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			// 开启AFIO时钟，用于引脚复用
	
	// 禁用JTAG，保留SWD功能，释放PB3和PB4作为普通IO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
    
	// 电机1方向引脚（B3, B4）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	// 电机2方向引脚（A11, A12）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	// 电机3方向引脚（B12, B13）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	// 电机4方向引脚（B14, B15）
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	// PWM引脚（PA0-PA3）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	//配置时钟源
	TIM_InternalClockConfig(TIM2);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	//时基单元初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元
	
	//输出比较初始化
	TIM_OCInitTypeDef TIM_OCInitStructure;								//定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);                         //结构体初始化，若结构体没有完整赋值

	// 电机1 PWM（TIM2 CH1）
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	// 电机2 PWM（TIM2 CH2）
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	// 电机3 PWM（TIM2 CH3）
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
		
	// 电机4 PWM（TIM2 CH4）
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	
	//TIM使能
	TIM_Cmd(TIM2, ENABLE);		//使能TIM2，定时器开始运行
}

//=======================================
// 函    数：单个电机控制
// 参    数：index - 电机索引（0-3）
// 参    数：speed - 速度值（-100~100）
//=======================================
void Motor_Car_Control(int index, int8_t speed)
{
    if(index < 0 || index > 3)
    {
        return;
    }
    
    // 速度范围检查
    if(speed > SPEED_MAX) 
    {
        speed = SPEED_MAX;
    }
    else if(speed < SPEED_MIN) 
    {
        speed = SPEED_MIN;
    }
    
    // 设置方向和速度
    switch(index)
    {
        case 0: // 电机1（左前轮）
            if(speed > 0)
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_3);
                GPIO_ResetBits(GPIOB, GPIO_Pin_4);
                TIM_SetCompare1(TIM2, speed); // 使用TIM2
            }
            else if(speed < 0)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_3);
                GPIO_SetBits(GPIOB, GPIO_Pin_4);
                TIM_SetCompare1(TIM2, -speed); // 使用TIM2
            }
            else
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4);
                TIM_SetCompare1(TIM2, 0); // 停止
            }
            break;
            
        case 1: // 电机2（右前轮）
            if(speed > 0)
            {
                GPIO_SetBits(GPIOA, GPIO_Pin_11);
                GPIO_ResetBits(GPIOA, GPIO_Pin_12);
                TIM_SetCompare2(TIM2, speed); // 使用TIM2
            }
            else if(speed < 0)
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_11);
                GPIO_SetBits(GPIOA, GPIO_Pin_12);
                TIM_SetCompare2(TIM2, -speed); // 使用TIM2
            }
            else
            {
                GPIO_ResetBits(GPIOA, GPIO_Pin_11 | GPIO_Pin_12);
                TIM_SetCompare2(TIM2, 0); // 停止
            }
            break;
            
        case 2: // 电机3（左后轮）
            if(speed > 0)
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_12);
                GPIO_ResetBits(GPIOB, GPIO_Pin_13);
                TIM_SetCompare3(TIM2, speed); // 使用TIM2
            }
            else if(speed < 0)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_12);
                GPIO_SetBits(GPIOB, GPIO_Pin_13);
                TIM_SetCompare3(TIM2, -speed); // 使用TIM2
            }
            else
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
                TIM_SetCompare3(TIM2, 0); // 停止
            }
            break;
            
        case 3: // 电机4（右后轮）
            if(speed > 0)
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_14);
                GPIO_ResetBits(GPIOB, GPIO_Pin_15);
                TIM_SetCompare4(TIM2, speed); // 使用TIM2
            }
            else if(speed < 0)
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_14);
                GPIO_SetBits(GPIOB, GPIO_Pin_15);
                TIM_SetCompare4(TIM2, -speed); // 使用TIM2
            }
            else
            {
                GPIO_ResetBits(GPIOB, GPIO_Pin_14 | GPIO_Pin_15);
                TIM_SetCompare4(TIM2, 0); // 停止
            }
            break;
    }
}

//=======================================
// 函    数：设置速度变化时间
// 参    数：time - 时间间隔（ms）
//=======================================
void Motor_Car_SetChangeTime(int time)
{
    if(time > 0)
    {
        Change_Time = time;
    }
}

//=======================================
// 函    数：设置单个电机当前速度
// 参    数：index - 电机索引（0-3）
// 参    数：speed - 速度值
//=======================================
void Motor_Car_Set_NowSpeed(int index, int8_t speed)
{
    if(index < 0 || index > 3)
    {
        return;
    }
    
    if(speed > SPEED_MAX) 
    {
        speed = SPEED_MAX;
    }
    else if(speed < SPEED_MIN) 
    {
        speed = SPEED_MIN;
    }
    
    Now_Speed[index] = speed;
}

//=======================================
// 函    数：设置单个电机目标速度
// 参    数：index - 电机索引（0-3）
// 参    数：speed - 速度值
//=======================================
void Motor_Car_Set_NextSpeed(int index, int8_t speed)
{
    if(index < 0 || index > 3)
    {
        return;
    }
    
    if(speed > SPEED_MAX) 
    {
        speed = SPEED_MAX;
    }
    else if(speed < SPEED_MIN) 
    {
        speed = SPEED_MIN;
    }
    
    Next_Speed[index] = speed;
}

//=======================================
// 函    数：获取单个电机当前速度
// 参    数：index - 电机索引（0-3）
// 返    回：当前速度
//=======================================
int8_t Motor_Car_GetNowSpeed(int index)
{
    if(index < 0 || index > 3)
    {
        return 0;
    }
    return Now_Speed[index];
}

//=======================================
// 函    数：小车平滑速度变化
//=======================================
void Motor_Car_Smooth_Go(void)
{
    // 处理所有四个电机
    for(int i=0; i<4; i++)
    {
        int Temp = Next_Speed[i] - Now_Speed[i];
        int Difference = Temp > 0 ? Temp : -Temp;
        
        if(Now_Speed[i] < Next_Speed[i])
        {
            for(int j=0; j<Difference; j++) 
            {
                Now_Speed[i]++;
                Motor_Car_Control(i, Now_Speed[i]);
                Delay_ms(Change_Time);
            }
        }
        else if(Now_Speed[i] > Next_Speed[i])
        {
            for(int j=0; j<Difference; j++) 
            {
                Now_Speed[i]--;
                Motor_Car_Control(i, Now_Speed[i]);
                Delay_ms(Change_Time);
            }
        }
    }
}

//=======================================
// 函    数：小车前进（四电机版本）
// 参    数：speed - 速度值（0~100）
//=======================================
void Motor_Car_Forward(int8_t speed)
{
    for(int i=0; i<4; i++)
    {
        Now_Speed[i]=Motor_Car_GetNowSpeed(i);
        Next_Speed[i] = speed;
    }
    Motor_Car_Smooth_Go();
}

//=======================================
// 函    数：小车后退（四电机版本）
// 参    数：speed - 速度值（0~100）
//=======================================
void Motor_Car_Backward(int8_t speed)
{
    for(int i=0; i<4; i++)
    {
        Now_Speed[i]=Motor_Car_GetNowSpeed(i);
        Next_Speed[i] = -speed;
    }
    Motor_Car_Smooth_Go();
}

//=======================================
// 函    数：小车左转（四电机版本）
// 参    数：speed - 速度值（0~100）
//=======================================
void Motor_Car_Left(int8_t speed)
{
    // 左电机反转，右电机正转
    for(int i=0; i<4; i++)
    {
        Now_Speed[i]=Motor_Car_GetNowSpeed(i);
    }
    Next_Speed[0] = -speed; // 左前轮
    Next_Speed[1] = speed;  // 右前轮
    Next_Speed[2] = -speed; // 左后轮
    Next_Speed[3] = speed;  // 右后轮
    Motor_Car_Smooth_Go();
}

//=======================================
// 函    数：小车右转（四电机版本）
// 参    数：speed - 速度值（0~100）
//=======================================
void Motor_Car_Right(int8_t speed)
{
    // 左电机正转，右电机反转
    for(int i=0; i<4; i++)
    {
        Now_Speed[i]=Motor_Car_GetNowSpeed(i);
    }
    Next_Speed[0] = speed;  // 左前轮
    Next_Speed[1] = -speed; // 右前轮
    Next_Speed[2] = speed;  // 左后轮
    Next_Speed[3] = -speed; // 右后轮
    Motor_Car_Smooth_Go();
}

//=======================================
// 函    数：小车停止（四电机版本）
//=======================================
void Motor_Car_Stop(void)
{
    for(int i=0; i<4; i++)
    {
        Now_Speed[i]=Motor_Car_GetNowSpeed(i);
        Next_Speed[i] = 0;
    }
    Motor_Car_Smooth_Go();
}

// 函    数：小车前进（二电机版本，暂时注释掉）
// 参    数：speed - 速度值（0~100）
//=======================================
/*
void Motor_Car_Forward(int8_t speed)
{
    Next_Speed[0] = speed; // 左前轮
    Next_Speed[1] = speed; // 右前轮
    Motor_Car_Smooth_Go();
}

//=======================================
// 函    数：小车后退（二电机版本，暂时注释掉）
// 参    数：speed - 速度值（0~100）
//=======================================
void Motor_Car_Backward(int8_t speed)
{
    Next_Speed[0] = -speed; // 左前轮
    Next_Speed[1] = -speed; // 右前轮
    Motor_Car_Smooth_Go();
}

//=======================================
// 函    数：小车左转（二电机版本，暂时注释掉）
// 参    数：speed - 速度值（0~100）
//=======================================
void Motor_Car_Left(int8_t speed)
{
    // 左电机反转，右电机正转
    Next_Speed[0] = -speed; // 左前轮
    Next_Speed[1] = speed;  // 右前轮
    Motor_Car_Smooth_Go();
}

//=======================================
// 函    数：小车右转（二电机版本，暂时注释掉）
// 参    数：speed - 速度值（0~100）
//=======================================
void Motor_Car_Right(int8_t speed)
{
    // 左电机正转，右电机反转
    Next_Speed[0] = speed;  // 左前轮
    Next_Speed[1] = -speed; // 右前轮
    Motor_Car_Smooth_Go();
}

//=======================================
// 函    数：小车停止（二电机版本，暂时注释掉）
//=======================================
void Motor_Car_Stop(void)
{
    Next_Speed[0] = 0; // 左前轮
    Next_Speed[1] = 0; // 右前轮
    Motor_Car_Smooth_Go();
}
*/