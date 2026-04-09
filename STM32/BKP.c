#include "BKP.h"

//=======================================
// 备份寄存器初始化
// 1.十个备份寄存器，每个寄存器16位，2字节
// 2.每个寄存器可以存两个数字/字母
// 3.使用BKP读写数据应该注意每个寄存器的地址，否则数据会存储到错误的地方
// 4.相邻BKP寄存器地址差四位
// BKP_DR1  0x40006C04
// BKP_DR2  0x40006C08
// BKP_DR3  0x40006C0C
// BKP_DR4  0x40006C10
// BKP_DR5  0x40006C14
// BKP_DR6  0x40006C18
// BKP_DR7  0x40006C1C
// BKP_DR8  0x40006C20
// BKP_DR9  0x40006C24
// BKP_DR10 0x40006C28
//=======================================

//=======================================
// 初始化备份寄存器
// 功能：开启PWR和BKP时钟，使能备份寄存器访问
//=======================================
void BKP_Init(void)
{
    // 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);    // 开启PWR的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);    // 开启BKP的时钟
    
    // 备份寄存器访问使能
    PWR_BackupAccessCmd(ENABLE);                            // 使用PWR开启对备份寄存器的访问
}

//=======================================
// 向备份寄存器写入数据
// 功能：向指定备份寄存器写入数据
// 参数：reg - 寄存器编号（1-10）
//       data - 要写入的数据
// 返回：无
//=======================================
void BKP_WriteData(uint8_t reg, uint16_t data)
{
    // 验证寄存器编号
    if (reg >= 1 && reg <= 10)
    {
        BKP_WriteBackupRegister(BKP_DR1 + ((reg - 1) * 4), data);    // 写入数据到指定备份寄存器
    }
}

//=======================================
// 从备份寄存器读取数据
// 功能：从指定备份寄存器读取数据
// 参数：reg - 寄存器编号（1-10）
//       data - 存储读取数据的指针
// 返回：无
//=======================================
void BKP_ReadData(uint8_t reg, uint16_t *data)
{
    // 验证寄存器编号
    if (reg >= 1 && reg <= 10)
    {
        *data = BKP_ReadBackupRegister(BKP_DR1 + ((reg - 1) * 4));    // 从指定备份寄存器读取数据
    }
}

//=======================================
// 向备份寄存器写入数组
// 功能：向多个备份寄存器写入数组数据
// 参数：array - 要写入的数组指针
//       length - 数组长度（最大10）
// 返回：无
//=======================================
void BKP_WriteArray(uint16_t *array, uint8_t length)
{
    uint8_t i;
    
    // 限制数组长度，最多10个备份寄存器
    if (length > 10)
    {
        length = 10;
    }
    
    // 依次写入数据到备份寄存器
    for (i = 0; i < length; i++)
    {
        BKP_WriteBackupRegister(BKP_DR1 + (i * 4), array[i]);
    }
}

//=======================================
// 从备份寄存器读取数组
// 功能：从多个备份寄存器读取数组数据
// 参数：array - 存储读取数据的数组指针
//       length - 数组长度（最大10）
// 返回：无
//=======================================
void BKP_ReadArray(uint16_t *array, uint8_t length)
{
    uint8_t i;
    
    // 限制数组长度，最多10个备份寄存器
    if (length > 10)
    {
        length = 10;
    }
    
    // 依次从备份寄存器读取数据
    for (i = 0; i < length; i++)
    {
        array[i] = BKP_ReadBackupRegister(BKP_DR1 + (i * 4));
    }
}