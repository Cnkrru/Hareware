//=======================================
// Flash模块头文件
//=======================================
#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"

//=======================================
// Flash扇区定义
//=======================================
#define FLASH_PAGE_SIZE         0x400    // 1KB每页
#define FLASH_START_ADDRESS     0x08000000
#define FLASH_END_ADDRESS       0x08080000    // 512KB Flash
#define FLASH_PAGE_COUNT        ((FLASH_END_ADDRESS - FLASH_START_ADDRESS) / FLASH_PAGE_SIZE)

//=======================================
// 函数声明
//1. 读取一个32位的字
//2. 读取一个16位的半字
//3. 读取一个8位的字节
//4. 全擦除
//5. 页擦除
//6. 编程字
//7. 编程半字
//8. 批量读取字节
//9. 批量写入字节
//10. 写入缓冲区
//11. 获取页地址
//12. 获取页号
//13. 检查地址是否有效
//14. 计算CRC32校验
//15. 验证数据
//16. 擦除多个页
//17. 带校验的字编程
//18. 带校验的半字编程
//19. 保存数据到闪存
//20. 清除所有有效数据
//21. 读取指定索引的数据
//22. 写入指定索引的数据
//23. 批量读取数据
//24. 批量写入数据
//25. 保存指定范围的数据
//26. 备份数据到指定地址
//27. 从指定地址恢复数据
//28. 验证数据完整性
//29. 查找指定数据
//30. 填充指定范围的数据
//31. 交换两个索引的数据
//=======================================
// 函数声明
//=======================================
uint32_t Flash_ReadWord(uint32_t Address);
uint16_t Flash_ReadHalfWord(uint32_t Address);
uint8_t Flash_ReadByte(uint32_t Address);
void Flash_EraseAllPages(void);
void Flash_ErasePage(uint32_t PageAddress);
void Flash_ProgramWord(uint32_t Address, uint32_t Data);
void Flash_ProgramHalfWord(uint32_t Address, uint16_t Data);
void Flash_ReadBytes(uint32_t address, uint8_t* buffer, uint32_t length);
void Flash_WriteBytes(uint32_t address, uint8_t* buffer, uint32_t length);
void Flash_WriteBuffer(uint32_t address, uint8_t* buffer, uint32_t length);
uint32_t Flash_GetPageAddress(uint8_t page);
uint8_t Flash_GetPageNumber(uint32_t address);
uint8_t Flash_CheckAddress(uint32_t address);
uint32_t Flash_CalculateCRC32(uint8_t* buffer, uint32_t length);
uint8_t Flash_VerifyData(uint32_t address, uint8_t* buffer, uint32_t length);
void Flash_ErasePages(uint8_t startPage, uint8_t endPage);
void Flash_ProgramWordWithCheck(uint32_t address, uint32_t data, uint8_t* success);
void Flash_ProgramHalfWordWithCheck(uint32_t address, uint16_t data, uint8_t* success);

//=======================================
// 存储功能
//=======================================
#define FLASH_STORE_START_ADDRESS    0x0800FC00    // 存储的起始地址
#define FLASH_STORE_COUNT            512           // 存储数据的个数

extern uint16_t Flash_StoreData[FLASH_STORE_COUNT];

void Flash_StoreSave(void);
void Flash_StoreClear(void);
uint16_t Flash_StoreRead(uint16_t index);
void Flash_StoreWrite(uint16_t index, uint16_t value);
void Flash_StoreReadBatch(uint16_t startIndex, uint16_t* buffer, uint16_t count);
void Flash_StoreWriteBatch(uint16_t startIndex, uint16_t* buffer, uint16_t count);
void Flash_StoreSaveRange(uint16_t startIndex, uint16_t count);
void Flash_StoreBackup(uint32_t backupAddress);
void Flash_StoreRestore(uint32_t backupAddress);
uint8_t Flash_StoreVerify(void);
int16_t Flash_StoreFind(uint16_t value);
void Flash_StoreFill(uint16_t startIndex, uint16_t count, uint16_t value);
void Flash_StoreSwap(uint16_t index1, uint16_t index2);

#endif
