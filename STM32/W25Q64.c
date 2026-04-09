//=======================================
// W25Q64模块 (SPI Flash)
//=======================================
// 引脚连接：
//   PB12 -> CS (片选)
//   PB13 -> SCK (时钟)
//   PB14 -> MISO (数据输出)
//   PB15 -> MOSI (数据输入)
//=======================================
// 容量：64Mbit (8MB)
// 页大小：256字节
// 扇区大小：4KB
// 块大小：64KB/32KB
//=======================================
// 函数说明：
// 1. W25Q64_Init          - 初始化W25Q64
// 2. W25Q64_ReadID       - 读取芯片ID
// 3. W25Q64_ReadData     - 读取数据
// 4. W25Q64_PageProgram   - 页编程(写入)
// 5. W25Q64_SectorErase   - 扇区擦除(4KB)
// 6. W25Q64_BlockErase64K - 块擦除(64KB)
// 7. W25Q64_BlockErase32K - 块擦除(32KB)
// 8. W25Q64_ChipErase    - 芯片擦除
// 9. W25Q64_WaitBusy     - 等待空闲
// 10. W25Q64_ReadStatusReg - 读取状态寄存器
//=======================================

#include "stm32f10x.h"
#include "W25Q64.h"

//=======================================
// W25Q64初始化
//=======================================
void W25Q64_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI2, &SPI_InitStructure);

    SPI_Cmd(SPI2, ENABLE);

    W25Q64_CS_HIGH();
}

//=======================================
// SPI发送接收一个字节
//=======================================
static uint8_t SPI_SendByte(uint8_t dat)
{
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI2, dat);

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI2);
}

//=======================================
// 读取芯片ID
//=======================================
uint32_t W25Q64_ReadID(void)
{
    uint32_t temp = 0;

    W25Q64_CS_LOW();

    SPI_SendByte(0x9F);
    temp = SPI_SendByte(0xFF) << 16;
    temp |= SPI_SendByte(0xFF) << 8;
    temp |= SPI_SendByte(0xFF);

    W25Q64_CS_HIGH();

    return temp;
}

//=======================================
// 读取数据
//=======================================
void W25Q64_ReadData(uint32_t addr, uint8_t *buf, uint32_t len)
{
    W25Q64_CS_LOW();

    SPI_SendByte(0x03);
    SPI_SendByte((addr >> 16) & 0xFF);
    SPI_SendByte((addr >> 8) & 0xFF);
    SPI_SendByte(addr & 0xFF);

    while (len--)
    {
        *buf = SPI_SendByte(0xFF);
        buf++;
    }

    W25Q64_CS_HIGH();
}

//=======================================
// 页编程(写入)
//=======================================
void W25Q64_PageProgram(uint32_t addr, uint8_t *buf, uint32_t len)
{
    W25Q64_WaitBusy();

    W25Q64_CS_LOW();
    SPI_SendByte(0x06);
    W25Q64_CS_HIGH();

    W25Q64_CS_LOW();
    SPI_SendByte(0x02);
    SPI_SendByte((addr >> 16) & 0xFF);
    SPI_SendByte((addr >> 8) & 0xFF);
    SPI_SendByte(addr & 0xFF);

    while (len--)
    {
        SPI_SendByte(*buf);
        buf++;
    }

    W25Q64_CS_HIGH();

    W25Q64_WaitBusy();
}

//=======================================
// 扇区擦除(4KB)
//=======================================
void W25Q64_SectorErase(uint32_t addr)
{
    W25Q64_WaitBusy();

    W25Q64_CS_LOW();
    SPI_SendByte(0x06);
    W25Q64_CS_HIGH();

    W25Q64_CS_LOW();
    SPI_SendByte(0x20);
    SPI_SendByte((addr >> 16) & 0xFF);
    SPI_SendByte((addr >> 8) & 0xFF);
    SPI_SendByte(addr & 0xFF);
    W25Q64_CS_HIGH();

    W25Q64_WaitBusy();
}

//=======================================
// 块擦除(64KB)
//=======================================
void W25Q64_BlockErase64K(uint32_t addr)
{
    W25Q64_WaitBusy();

    W25Q64_CS_LOW();
    SPI_SendByte(0x06);
    W25Q64_CS_HIGH();

    W25Q64_CS_LOW();
    SPI_SendByte(0xD8);
    SPI_SendByte((addr >> 16) & 0xFF);
    SPI_SendByte((addr >> 8) & 0xFF);
    SPI_SendByte(addr & 0xFF);
    W25Q64_CS_HIGH();

    W25Q64_WaitBusy();
}

//=======================================
// 块擦除(32KB)
//=======================================
void W25Q64_BlockErase32K(uint32_t addr)
{
    W25Q64_WaitBusy();

    W25Q64_CS_LOW();
    SPI_SendByte(0x06);
    W25Q64_CS_HIGH();

    W25Q64_CS_LOW();
    SPI_SendByte(0x52);
    SPI_SendByte((addr >> 16) & 0xFF);
    SPI_SendByte((addr >> 8) & 0xFF);
    SPI_SendByte(addr & 0xFF);
    W25Q64_CS_HIGH();

    W25Q64_WaitBusy();
}

//=======================================
// 芯片擦除
//=======================================
void W25Q64_ChipErase(void)
{
    W25Q64_WaitBusy();

    W25Q64_CS_LOW();
    SPI_SendByte(0x06);
    W25Q64_CS_HIGH();

    W25Q64_CS_LOW();
    SPI_SendByte(0xC7);
    W25Q64_CS_HIGH();

    W25Q64_WaitBusy();
}

//=======================================
// 等待空闲
//=======================================
void W25Q64_WaitBusy(void)
{
    while ((W25Q64_ReadStatusReg(1) & 0x01) == 0x01);
}

//=======================================
// 读取状态寄存器
//=======================================
uint8_t W25Q64_ReadStatusReg(uint8_t reg)
{
    uint8_t status;

    W25Q64_CS_LOW();

    if (reg == 1)
    {
        SPI_SendByte(0x05);
    }
    else
    {
        SPI_SendByte(0x35);
    }

    status = SPI_SendByte(0xFF);
    W25Q64_CS_HIGH();

    return status;
}
