#include "Flash.h"

//=======================================
// Flash参数
// 1.扇区大小
//  - 0-3扇区的大小为1KB
//  - 4-11扇区的大小为8KB
//  - 12扇区的大小为4KB


//=======================================

//=======================================
// 读取一个32位的字
// 功能：读取指定地址的32位数据
// 参数：Address - 读取地址
// 返回：读取的数据
//=======================================
uint32_t Flash_ReadWord(uint32_t Address)
{
    return *((__IO uint32_t *)(Address));                  // 使用指针访问指定地址下的数据并返回
}

//=======================================
// 读取一个16位的半字
// 功能：读取指定地址的16位数据
// 参数：Address - 读取地址
// 返回：读取的数据
//=======================================
uint16_t Flash_ReadHalfWord(uint32_t Address)
{
    return *((__IO uint16_t *)(Address));                  // 使用指针访问指定地址下的数据并返回
}

//=======================================
// 读取一个8位的字节
// 功能：读取指定地址的8位数据
// 参数：Address - 读取地址
// 返回：读取的数据
//=======================================
uint8_t Flash_ReadByte(uint32_t Address)
{
    return *((__IO uint8_t *)(Address));                   // 使用指针访问指定地址下的数据并返回
}

//=======================================
// 全擦除
// 功能：擦除所有Flash页
//=======================================
void Flash_EraseAllPages(void)
{
    FLASH_Unlock();                                         // 解锁
    FLASH_EraseAllPages();                                   // 全擦除
    FLASH_Lock();                                           // 加锁
}

//=======================================
// 页擦除
// 功能：擦除指定Flash页
// 参数：PageAddress - 页地址
//=======================================
void Flash_ErasePage(uint32_t PageAddress)
{
    FLASH_Unlock();                                         // 解锁
    FLASH_ErasePage(PageAddress);                           // 页擦除
    FLASH_Lock();                                           // 加锁
}

//=======================================
// 编程字
// 功能：向指定地址写入32位数据
// 参数：Address - 写入地址
//       Data - 写入数据
//=======================================
void Flash_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Unlock();                                         // 解锁
    FLASH_ProgramWord(Address, Data);                        // 编程字
    FLASH_Lock();                                           // 加锁
}

//=======================================
// 编程半字
// 功能：向指定地址写入16位数据
// 参数：Address - 写入地址
//       Data - 写入数据
//=======================================
void Flash_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Unlock();                                         // 解锁
    FLASH_ProgramHalfWord(Address, Data);                   // 编程半字
    FLASH_Lock();                                           // 加锁
}

//=======================================
// 拓展功能
//=======================================

//=======================================
// 批量读取字节
// 功能：批量读取指定地址的字节数据
// 参数：address - 起始地址
//       buffer - 存储缓冲区
//       length - 读取长度
//=======================================
void Flash_ReadBytes(uint32_t address, uint8_t* buffer, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        buffer[i] = Flash_ReadByte(address + i);
    }
}

//=======================================
// 批量写入字节
// 功能：批量向指定地址写入字节数据
// 参数：address - 起始地址
//       buffer - 数据缓冲区
//       length - 写入长度
//=======================================
void Flash_WriteBytes(uint32_t address, uint8_t* buffer, uint32_t length)
{
    FLASH_Unlock();
    
    for (uint32_t i = 0; i < length; i++) {
        // 字节写入需要转换为半字写入
        if (i % 2 == 0 && i < length - 1) {
            // 小端模式：低字节在前
            uint16_t data = (buffer[i + 1] << 8) | buffer[i];
            FLASH_ProgramHalfWord(address + i, data);
            i++;
        } else {
            // 单个字节时，低字节有效，高字节补0
            uint16_t data = buffer[i];
            FLASH_ProgramHalfWord(address + i, data);
        }
    }
    
    FLASH_Lock();
}

//=======================================
// 写入缓冲区
// 功能：自动处理对齐和擦除，写入缓冲区数据
// 参数：address - 起始地址
//       buffer - 数据缓冲区
//       length - 写入长度
//=======================================
void Flash_WriteBuffer(uint32_t address, uint8_t* buffer, uint32_t length)
{
    uint32_t startPage = address / FLASH_PAGE_SIZE;
    uint32_t endPage = (address + length - 1) / FLASH_PAGE_SIZE;
    
    FLASH_Unlock();
    
    // 擦除需要的页
    for (uint32_t page = startPage; page <= endPage; page++) {
        FLASH_ErasePage(FLASH_START_ADDRESS + page * FLASH_PAGE_SIZE);
    }
    
    // 写入数据
    for (uint32_t i = 0; i < length; i++) {
        if (i % 2 == 0 && i < length - 1) {
            // 小端模式：低字节在前
            uint16_t data = (buffer[i + 1] << 8) | buffer[i];
            FLASH_ProgramHalfWord(address + i, data);
            i++;
        } else {
            // 单个字节时，低字节有效，高字节补0
            uint16_t data = buffer[i];
            FLASH_ProgramHalfWord(address + i, data);
        }
    }
    
    FLASH_Lock();
}

//=======================================
// 获取页地址
// 功能：根据页号获取页地址
// 参数：page - 页号
// 返回：页地址
//=======================================
uint32_t Flash_GetPageAddress(uint8_t page)
{
    if (page < FLASH_PAGE_COUNT) {
        return FLASH_START_ADDRESS + page * FLASH_PAGE_SIZE;
    }
    return 0;
}

//=======================================
// 获取页号
// 功能：根据地址获取页号
// 参数：address - 地址
// 返回：页号
//=======================================
uint8_t Flash_GetPageNumber(uint32_t address)
{
    if (address >= FLASH_START_ADDRESS && address < FLASH_END_ADDRESS) {
        return (address - FLASH_START_ADDRESS) / FLASH_PAGE_SIZE;
    }
    return 0xFF;
}

//=======================================
// 检查地址是否有效
// 功能：检查地址是否在Flash范围内
// 参数：address - 地址
// 返回：1-有效，0-无效
//=======================================
uint8_t Flash_CheckAddress(uint32_t address)
{
    return (address >= FLASH_START_ADDRESS && address < FLASH_END_ADDRESS) ? 1 : 0;
}

//=======================================
// 计算CRC32校验
// 功能：计算数据的CRC32校验值
// 参数：buffer - 数据缓冲区
//       length - 数据长度
// 返回：CRC32校验值
//=======================================
uint32_t Flash_CalculateCRC32(uint8_t* buffer, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF;
    uint32_t polynomial = 0xEDB88320;
    
    for (uint32_t i = 0; i < length; i++) {
        crc ^= buffer[i];
        for (uint32_t j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ polynomial;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return ~crc;
}

//=======================================
// 验证数据
// 功能：验证Flash中的数据是否与缓冲区一致
// 参数：address - 起始地址
//       buffer - 数据缓冲区
//       length - 验证长度
// 返回：1-验证通过，0-验证失败
//=======================================
uint8_t Flash_VerifyData(uint32_t address, uint8_t* buffer, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        if (Flash_ReadByte(address + i) != buffer[i]) {
            return 0;
        }
    }
    return 1;
}

//=======================================
// 擦除多个页
// 功能：擦除指定范围的Flash页
// 参数：startPage - 起始页号
//       endPage - 结束页号
//=======================================
void Flash_ErasePages(uint8_t startPage, uint8_t endPage)
{
    if (startPage > endPage || endPage >= FLASH_PAGE_COUNT) {
        return;
    }
    
    FLASH_Unlock();
    
    for (uint8_t page = startPage; page <= endPage; page++) {
        FLASH_ErasePage(FLASH_START_ADDRESS + page * FLASH_PAGE_SIZE);
    }
    
    FLASH_Lock();
}

//=======================================
// 带校验的字编程
// 功能：向指定地址写入32位数据并校验
// 参数：address - 写入地址
//       data - 写入数据
//       success - 校验结果
//=======================================
void Flash_ProgramWordWithCheck(uint32_t address, uint32_t data, uint8_t* success)
{
    FLASH_Unlock();
    FLASH_ProgramWord(address, data);
    FLASH_Lock();
    
    *success = (Flash_ReadWord(address) == data) ? 1 : 0;
}

//=======================================
// 带校验的半字编程
// 功能：向指定地址写入16位数据并校验
// 参数：address - 写入地址
//       data - 写入数据
//       success - 校验结果
//=======================================
void Flash_ProgramHalfWordWithCheck(uint32_t address, uint16_t data, uint8_t* success)
{
    FLASH_Unlock();
    FLASH_ProgramHalfWord(address, data);
    FLASH_Lock();
    
    *success = (Flash_ReadHalfWord(address) == data) ? 1 : 0;
}

//=======================================
// 存储功能
//=======================================

//=======================================
// 全局变量
//=======================================
// 存储数据数组
uint16_t Flash_StoreData[FLASH_STORE_COUNT];
static uint8_t flashStoreInitFlag = 0;

//=======================================
// 内部存储模块自动初始化函数
// 功能：自动初始化存储模块
//=======================================
static void Flash_StoreAutoInit(void)
{
    if (flashStoreInitFlag == 0) {
        flashStoreInitFlag = 1;
        // 判断是不是第一次使用
        if (Flash_ReadHalfWord(FLASH_STORE_START_ADDRESS) != 0xA5A5) {
            Flash_ErasePage(FLASH_STORE_START_ADDRESS);
            Flash_ProgramHalfWord(FLASH_STORE_START_ADDRESS, 0xA5A5);
            for (uint16_t i = 1; i < FLASH_STORE_COUNT; i++) {
                Flash_ProgramHalfWord(FLASH_STORE_START_ADDRESS + i * 2, 0x0000);
            }
        }
        
        // 上电时，将闪存数据加载回SRAM数组
        for (uint16_t i = 0; i < FLASH_STORE_COUNT; i++) {
            Flash_StoreData[i] = Flash_ReadHalfWord(FLASH_STORE_START_ADDRESS + i * 2);
        }
    }
}

//=======================================
// 保存数据到闪存
// 功能：将存储数据保存到Flash
//=======================================
void Flash_StoreSave(void)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    Flash_ErasePage(FLASH_STORE_START_ADDRESS);
    for (uint16_t i = 0; i < FLASH_STORE_COUNT; i++) {
        Flash_ProgramHalfWord(FLASH_STORE_START_ADDRESS + i * 2, Flash_StoreData[i]);
    }
}

//=======================================
// 清除所有有效数据
// 功能：清除所有存储数据
//=======================================
void Flash_StoreClear(void)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    for (uint16_t i = 1; i < FLASH_STORE_COUNT; i++) {
        Flash_StoreData[i] = 0x0000;
    }
    Flash_StoreSave();
}

//=======================================
// 读取指定索引的数据
// 功能：读取指定索引的存储数据
// 参数：index - 索引
// 返回：存储的数据
//=======================================
uint16_t Flash_StoreRead(uint16_t index)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    if (index < FLASH_STORE_COUNT) {
        return Flash_StoreData[index];
    }
    return 0;
}

//=======================================
// 写入指定索引的数据
// 功能：写入数据到指定索引
// 参数：index - 索引
//       value - 数据值
//=======================================
void Flash_StoreWrite(uint16_t index, uint16_t value)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    if (index < FLASH_STORE_COUNT) {
        Flash_StoreData[index] = value;
    }
}

//=======================================
// 批量读取数据
// 功能：批量读取存储数据
// 参数：startIndex - 起始索引
//       buffer - 存储缓冲区
//       count - 读取数量
//=======================================
void Flash_StoreReadBatch(uint16_t startIndex, uint16_t* buffer, uint16_t count)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    for (uint16_t i = 0; i < count && (startIndex + i) < FLASH_STORE_COUNT; i++) {
        buffer[i] = Flash_StoreData[startIndex + i];
    }
}

//=======================================
// 批量写入数据
// 功能：批量写入存储数据
// 参数：startIndex - 起始索引
//       buffer - 数据缓冲区
//       count - 写入数量
//=======================================
void Flash_StoreWriteBatch(uint16_t startIndex, uint16_t* buffer, uint16_t count)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    for (uint16_t i = 0; i < count && (startIndex + i) < FLASH_STORE_COUNT; i++) {
        Flash_StoreData[startIndex + i] = buffer[i];
    }
}

//=======================================
// 保存指定范围的数据
// 功能：保存指定范围的存储数据
// 参数：startIndex - 起始索引
//       count - 保存数量
//=======================================
void Flash_StoreSaveRange(uint16_t startIndex, uint16_t count)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    Flash_ErasePage(FLASH_STORE_START_ADDRESS);
    for (uint16_t i = 0; i < FLASH_STORE_COUNT; i++) {
        Flash_ProgramHalfWord(FLASH_STORE_START_ADDRESS + i * 2, Flash_StoreData[i]);
    }
}

//=======================================
// 备份数据到指定地址
// 功能：将存储数据备份到指定地址
// 参数：backupAddress - 备份地址
//=======================================
void Flash_StoreBackup(uint32_t backupAddress)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    Flash_ErasePage(backupAddress);
    for (uint16_t i = 0; i < FLASH_STORE_COUNT; i++) {
        Flash_ProgramHalfWord(backupAddress + i * 2, Flash_StoreData[i]);
    }
}

//=======================================
// 从指定地址恢复数据
// 功能：从指定地址恢复存储数据
// 参数：backupAddress - 备份地址
//=======================================
void Flash_StoreRestore(uint32_t backupAddress)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    for (uint16_t i = 0; i < FLASH_STORE_COUNT; i++) {
        Flash_StoreData[i] = Flash_ReadHalfWord(backupAddress + i * 2);
    }
    Flash_StoreSave();
}

//=======================================
// 验证数据完整性
// 功能：验证存储数据的完整性
// 返回：1-验证通过，0-验证失败
//=======================================
uint8_t Flash_StoreVerify(void)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    if (Flash_ReadHalfWord(FLASH_STORE_START_ADDRESS) != 0xA5A5) {
        return 0;
    }
    
    for (uint16_t i = 0; i < FLASH_STORE_COUNT; i++) {
        if (Flash_ReadHalfWord(FLASH_STORE_START_ADDRESS + i * 2) != Flash_StoreData[i]) {
            return 0;
        }
    }
    
    return 1;
}

//=======================================
// 查找指定数据
// 功能：查找指定的存储数据
// 参数：value - 要查找的数据
// 返回：找到的索引，-1表示未找到
//=======================================
int16_t Flash_StoreFind(uint16_t value)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    for (uint16_t i = 1; i < FLASH_STORE_COUNT; i++) {
        if (Flash_StoreData[i] == value) {
            return i;
        }
    }
    return -1;
}

//=======================================
// 填充指定范围的数据
// 功能：填充指定范围的存储数据
// 参数：startIndex - 起始索引
//       count - 填充数量
//       value - 填充值
//=======================================
void Flash_StoreFill(uint16_t startIndex, uint16_t count, uint16_t value)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    for (uint16_t i = 0; i < count && (startIndex + i) < FLASH_STORE_COUNT; i++) {
        Flash_StoreData[startIndex + i] = value;
    }
}

//=======================================
// 交换两个索引的数据
// 功能：交换两个索引的存储数据
// 参数：index1 - 第一个索引
//       index2 - 第二个索引
//=======================================
void Flash_StoreSwap(uint16_t index1, uint16_t index2)
{
    Flash_StoreAutoInit();  // 自动初始化存储模块
    if (index1 < FLASH_STORE_COUNT && index2 < FLASH_STORE_COUNT) {
        uint16_t temp = Flash_StoreData[index1];
        Flash_StoreData[index1] = Flash_StoreData[index2];
        Flash_StoreData[index2] = temp;
    }
}
