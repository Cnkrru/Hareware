#include "stm32f10x.h"
#include "RTC.h"
#include <time.h>

//=======================================
// 全局变量
//=======================================
// 全局时间数组，分别为年、月、日、时、分、秒
uint16_t RTC_Time[] = {2026, 3, 14, 12, 0, 0};
static uint8_t rtcInitFlag = 0;

//=======================================
// 内部RTC自动初始化函数
// 功能：自动初始化RTC，包括时钟配置和第一次设置
//=======================================
static void RTC_Init(void)
{
    if (rtcInitFlag == 0)
    {
        rtcInitFlag = 1;
        // 开启时钟
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);     // 开启PWR的时钟
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);     // 开启BKP的时钟
        
        // 备份寄存器访问使能
        PWR_BackupAccessCmd(ENABLE);                            // 使用PWR开启对备份寄存器的访问
        
        if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)           // 通过写入备份寄存器的标志位，判断RTC是否是第一次配置
        {
            RCC_LSEConfig(RCC_LSE_ON);                           // 开启LSE时钟
            while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);   // 等待LSE准备就绪
            
            RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);              // 选择RTCCLK来源为LSE
            RCC_RTCCLKCmd(ENABLE);                               // RTCCLK使能
            
            RTC_WaitForSynchro();                                // 等待同步
            RTC_WaitForLastTask();                               // 等待上一次操作完成
            
            RTC_SetPrescaler(32768 - 1);                         // 设置RTC预分频器，预分频后的计数频率为1Hz
            RTC_WaitForLastTask();                               // 等待上一次操作完成
            
            RTC_SetTime();                                     // 设置时间，调用此函数，全局数组里时间值刷新到RTC硬件电路
            
            BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);            // 在备份寄存器写入自己规定的标志位，用于判断RTC是不是第一次执行配置
        }
        else                                                     // RTC不是第一次配置
        {
            RTC_WaitForSynchro();                                // 等待同步
            RTC_WaitForLastTask();                               // 等待上一次操作完成
        }
    }
}

//=======================================
// RTC设置时间
// 功能：将全局数组里的时间值刷新到RTC硬件电路
//=======================================
void RTC_SetTime(void)
{
    RTC_Init();  // 自动初始化RTC
    time_t time_cnt;                                           // 定义秒计数器数据类型
    struct tm time_date;                                       // 定义日期时间数据类型
    
    time_date.tm_year = RTC_Time[0] - 1900;                  // 将数组的时间赋值给日期时间结构体
    time_date.tm_mon = RTC_Time[1] - 1;
    time_date.tm_mday = RTC_Time[2];
    time_date.tm_hour = RTC_Time[3];
    time_date.tm_min = RTC_Time[4];
    time_date.tm_sec = RTC_Time[5];
    
    time_cnt = mktime(&time_date);               // 调用mktime函数，将日期时间转换为秒计数器格式
    
    RTC_SetCounter(time_cnt);                                  // 将秒计数器写入到RTC的CNT中
    RTC_WaitForLastTask();                                     // 等待上一次操作完成
}

//=======================================
// RTC读取时间
// 功能：将RTC硬件电路里的时间值刷新到全局数组
//=======================================
void RTC_ReadTime(void)
{
    RTC_Init();  // 自动初始化RTC
    time_t time_cnt;                                           // 定义秒计数器数据类型
    struct tm time_date;                                       // 定义日期时间数据类型
    
    time_cnt = RTC_GetCounter();                 // 读取RTC的CNT，获取当前的秒计数器
    
    time_date = *localtime(&time_cnt);                         // 使用localtime函数，将秒计数器转换为日期时间格式
    
    RTC_Time[0] = time_date.tm_year + 1900;                  // 将日期时间结构体赋值给数组的时间
    RTC_Time[1] = time_date.tm_mon + 1;
    RTC_Time[2] = time_date.tm_mday;
    RTC_Time[3] = time_date.tm_hour;
    RTC_Time[4] = time_date.tm_min;
    RTC_Time[5] = time_date.tm_sec;
}

//=======================================
// 获取时间戳
// 功能：获取当前RTC的秒计数器值
// 返回：当前时间戳（秒计数器值）
//=======================================
uint32_t RTC_GetTimestamp(void)
{
    RTC_Init();  // 自动初始化RTC
    return RTC_GetCounter();
}

//=======================================
// 时间戳改写接口
// 功能：通过时间戳设置RTC时间
// 参数：timestamp - 要设置的时间戳
//=======================================
void RTC_TimestampWrite(uint32_t timestamp)
{
    RTC_Init();  // 自动初始化RTC
    RTC_SetCounter(timestamp);
    RTC_WaitForLastTask();
    
    // 更新全局数组
    time_t time_cnt = timestamp;
    struct tm time_date = *localtime(&time_cnt);
    
    RTC_Time[0] = time_date.tm_year + 1900;
    RTC_Time[1] = time_date.tm_mon + 1;
    RTC_Time[2] = time_date.tm_mday;
    RTC_Time[3] = time_date.tm_hour;
    RTC_Time[4] = time_date.tm_min;
    RTC_Time[5] = time_date.tm_sec;
}

//=======================================
// 获取星期几
// 功能：获取当前星期几
// 返回：星期几（1-7，1表示周日）
//=======================================
uint8_t RTC_GetWeekday(void)
{
    RTC_Init();  // 自动初始化RTC
    time_t time_cnt = RTC_GetCounter();
    struct tm time_date = *localtime(&time_cnt);
    
    return time_date.tm_wday + 1;  // tm_wday范围是0-6，0表示周日，转换为1-7
}

//=======================================
// 标准时间格式
// 功能：获取标准格式的时间字符串
// 参数：buffer - 存储时间字符串的缓冲区
//=======================================
void RTC_StandardTime(char* buffer)
{
    RTC_Init();  // 自动初始化RTC
    RTC_ReadTime();
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", 
            RTC_Time[0], RTC_Time[1], RTC_Time[2], 
            RTC_Time[3], RTC_Time[4], RTC_Time[5]);
}

//=======================================
// 时间戳自增
// 功能：将RTC时间戳自增1秒
//=======================================
void RTC_IncrementTimestamp(void)
{
    RTC_Init();  // 自动初始化RTC
    uint32_t timestamp = RTC_GetCounter();  // 获取当前时间戳
    timestamp++;  // 自增1秒
    RTC_SetCounter(timestamp);  // 写回RTC
    RTC_WaitForLastTask();  // 等待操作完成
    
    // 更新全局时间数组
    time_t time_cnt = timestamp;
    struct tm time_date = *localtime(&time_cnt);
    
    RTC_Time[0] = time_date.tm_year + 1900;
    RTC_Time[1] = time_date.tm_mon + 1;
    RTC_Time[2] = time_date.tm_mday;
    RTC_Time[3] = time_date.tm_hour;
    RTC_Time[4] = time_date.tm_min;
    RTC_Time[5] = time_date.tm_sec;
}
