//=======================================
// W25Q64模块头文件
//=======================================
#ifndef __W25Q64_H
#define __W25Q64_H

#include "stm32f10x.h"

#define W25Q64_DUMMY_BYTE      0xFF

#define W25Q64_WRITE_ENABLE     0x06
#define W25Q64_WRITE_DISABLE   0x04
#define W25Q64_READ_STATUS_REG1 0x05
#define W25Q64_READ_STATUS_REG2 0x35
#define W25Q64_WRITE_STATUS_REG 0x01
#define W25Q64_READ_DATA       0x03
#define W25Q64_FAST_READ       0x0B
#define W25Q64_FAST_READ_DUAL   0x3B
#define W25Q64_PAGE_PROGRAM    0x02
#define W25Q64_BLOCK_ERASE_64K 0xD8
#define W25Q64_BLOCK_ERASE_32K 0x52
#define W25Q64_SECTOR_ERASE    0x20
#define W25Q64_CHIP_ERASE      0xC7
#define W25Q64_POWER_DOWN      0xB9
#define W25Q64_RELEASE_POWER_DOWN 0xAB
#define W25Q64_MANUFACTURER_ID  0x90
#define W25Q64_DEVICE_ID        0xAB
#define W25Q64_JEDEC_ID         0x9F

#define W25Q64_CS_LOW()        GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define W25Q64_CS_HIGH()       GPIO_SetBits(GPIOB, GPIO_Pin_12)

//=======================================
// 函数声明
//1. W25Q64初始化
//2. 读取ID
//3. 读取数据
//4. 写入数据
//5. 扇区擦除
//6. 块擦除
//7. 芯片擦除
//8. 等待空闲
//9. 读取状态寄存器
//=======================================
// 函数声明
//=======================================
void W25Q64_Init(void);
uint32_t W25Q64_ReadID(void);
void W25Q64_ReadData(uint32_t addr, uint8_t *buf, uint32_t len);
void W25Q64_PageProgram(uint32_t addr, uint8_t *buf, uint32_t len);
void W25Q64_SectorErase(uint32_t addr);
void W25Q64_BlockErase64K(uint32_t addr);
void W25Q64_BlockErase32K(uint32_t addr);
void W25Q64_ChipErase(void);
void W25Q64_WaitBusy(void);
uint8_t W25Q64_ReadStatusReg(uint8_t reg);

#endif
