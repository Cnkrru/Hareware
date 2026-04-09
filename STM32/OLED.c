//=======================================
// OLED模块
// 软件I2C驱动
// 引脚配置：
// OLED     STM32
// VCC  ->  3.3V
// GND  ->  GND
// SCL  ->  PB10
// SDA  ->  PB11
//=======================================
// 中文字符范围限制
// 行：1-8
// 列：1-4
//=======================================
#include "stm32f10x.h"
#include "OLED_Font.h"

//=======================================
// 引脚配置
//=======================================
#define OLED_W_SCL(x)        GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))
#define OLED_W_SDA(x)        GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)(x))

//=======================================
// 引脚初始化
//=======================================
void OLED_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

//=======================================
// I2C开始
//=======================================
void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

//=======================================
// I2C停止
//=======================================
void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

//=======================================
// I2C发送一个字节
//=======================================
void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA(!!(Byte & (0x80 >> i)));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1);    //额外的一个时钟，不处理应答信号
    OLED_W_SCL(0);
}

//=======================================
// OLED写命令
//=======================================
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);        //从机地址
    OLED_I2C_SendByte(0x00);        //写命令
    OLED_I2C_SendByte(Command); 
    OLED_I2C_Stop();
}

//=======================================
// OLED写数据
//=======================================
void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78);        //从机地址
    OLED_I2C_SendByte(0x40);        //写数据
    OLED_I2C_SendByte(Data);
    OLED_I2C_Stop();
}

//=======================================
// OLED设置光标位置
//=======================================
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);                //设置Y位置
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));    //设置X位置高4位
    OLED_WriteCommand(0x00 | (X & 0x0F));        //设置X位置低4位
}

//=======================================
// OLED清屏
//=======================================
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        for(i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

//=======================================
// OLED显示一个字符
//=======================================
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);        //设置光标位置在上半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);            //显示上半部分内容
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);    //设置光标位置在下半部分
    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);        //显示下半部分内容
    }
}

//=======================================
// OLED显示字符串
//=======================================
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

//=======================================
// OLED次方函数
//=======================================
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

//=======================================
// OLED显示数字（十进制，正数）
//=======================================
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)                        
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

//=======================================
// OLED显示数字（十进制，带符号数）
//=======================================
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++)                        
    {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

//=======================================
// OLED显示数字（十六进制，正数）
//=======================================
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)                        
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        }
        else
        {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

//=======================================
// OLED显示数字（二进制，正数）
//=======================================
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)                        
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

//=======================================
// OLED初始化
//=======================================
void OLED_Init(void)
{
    uint32_t i, j;
    
    for (i = 0; i < 1000; i++)            //上电延时
    {
        for (j = 0; j < 1000; j++);
    }
    
    OLED_I2C_Init();            //端口初始化
    
    OLED_WriteCommand(0xAE);    //关闭显示
    
    OLED_WriteCommand(0xD5);    //设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);
    
    OLED_WriteCommand(0xA8);    //设置多路复用率
    OLED_WriteCommand(0x3F);
    
    OLED_WriteCommand(0xD3);    //设置显示偏移
    OLED_WriteCommand(0x00);
    
    OLED_WriteCommand(0x40);    //设置显示开始行
    
    OLED_WriteCommand(0xA1);    //设置左右方向，0xA1正常 0xA0左右反置
    
    OLED_WriteCommand(0xC8);    //设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(0xDA);    //设置COM引脚硬件配置
    OLED_WriteCommand(0x12);
    
    OLED_WriteCommand(0x81);    //设置对比度控制
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9);    //设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);    //设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4);    //设置整个显示打开/关闭

    OLED_WriteCommand(0xA6);    //设置正常/倒转显示

    OLED_WriteCommand(0x8D);    //设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF);    //开启显示
        
    OLED_Clear();                //OLED清屏
}

//=======================================
// 绘制点
//=======================================
void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char Color)
{
    unsigned char page = y / 8;           // 计算页地址
    unsigned char column = x;             // 列地址
    unsigned char bit = y % 8;            // 位地址
    
    OLED_SetCursor(page, column);         // 设置光标位置
    
    if (Color)
    {
        OLED_WriteData(0x01 << bit);      // 写入1
    }
    else
    {
        OLED_WriteData(0x00);             // 写入0
    }
}

//=======================================
// 绘制直线
//=======================================
void OLED_Draw_Line(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char Color)
{
    int dx = X2 - X1;                     // X方向差值
    int dy = Y2 - Y1;                     // Y方向差值
    int sx = (dx > 0) ? 1 : -1;           // X方向步进
    int sy = (dy > 0) ? 1 : -1;           // Y方向步进
    int err = dx - dy;                    // 误差值
    int e2;                               // 误差值的2倍
    
    unsigned char x = X1;                 // 当前X坐标
    unsigned char y = Y1;                 // 当前Y坐标
    
    while(1)
    {
        OLED_DrawPoint(x, y, Color);  // 绘制当前点
        
        if(x == X2 && y == Y2)            // 到达终点
        {
            break;
        }
        
        e2 = 2 * err;                     // 计算2倍误差值
        
        if(e2 > -dy)                      // X方向步进
        {
            err -= dy;                    // 更新误差值
            x += sx;                      // 更新X坐标
        }
        
        if(e2 < dx)                       // Y方向步进
        {
            err += dx;                    // 更新误差值
            y += sy;                      // 更新Y坐标
        }
    }
}

//=======================================
// 绘制圆
//=======================================
void OLED_Draw_Circle(unsigned char X, unsigned char Y, unsigned char Radius, unsigned char Color)
{
    int x = 0;
    int y = Radius;
    int d = 3 - 2 * Radius;
    
    while(x <= y)
    {
        OLED_DrawPoint(X + x, Y + y, Color);
        OLED_DrawPoint(X - x, Y + y, Color);
        OLED_DrawPoint(X + x, Y - y, Color);
        OLED_DrawPoint(X - x, Y - y, Color);
        OLED_DrawPoint(X + y, Y + x, Color);
        OLED_DrawPoint(X - y, Y + x, Color);
        OLED_DrawPoint(X + y, Y - x, Color);
        OLED_DrawPoint(X - y, Y - x, Color);
        
        if(d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

//=======================================
// OLED显示汉字（支持正反显示）
//参数 ：Line   - 行位置，1~4（每行对应16像素高度）
//参数 ：Column - 列位置，1~8（每列对应16像素宽度）
//参数 ：Num    - 字库中的汉字索引（0,1,...）
//参数 ：mode   - 显示模式，1-正常显示，0-反向显示
//=======================================
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num, uint8_t mode)  
{
    uint8_t i;
    uint8_t page_start = (Line - 1) * 2;   // 计算起始页（汉字占2页）
    uint8_t col_start = (Column - 1) * 16; // 计算起始列（汉字占16列）

    // 显示上半部分（前16字节）
    OLED_SetCursor(page_start, col_start);
    for (i = 0; i < 16; i++) 
    {
        uint8_t data = OLED_F10x16[Num][i];
        OLED_WriteData(mode ? data : ~data); // 根据mode取反
    }

    // 显示下半部分（后16字节）
    OLED_SetCursor(page_start + 1, col_start);
    for (i = 16; i < 32; i++) 
    {
        uint8_t data = OLED_F10x16[Num][i];
        OLED_WriteData(mode ? data : ~data); // 根据mode取反
    }
}

//=======================================
// OLED显示位图（支持任意位置和尺寸）
//参数 ：x     - 起始列 (0~127)
//参数 ：y     - 起始页 (0~7)
//参数 ：width - 图像宽度（像素，必须为8的倍数）
//参数 ：height - 图像高度（像素，必须为8的倍数）
//参数 ：bmp   - 位图数据数组，大小为 (width/8)*height 字节
//参数 ：mode  - 显示模式，1-正常显示，0-反向显示
//=======================================
// void OLED_ShowBMP(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bmp, uint8_t mode) 
// {
//     uint8_t page_end = y + height / 8;
//     uint8_t col_end = x + width;
//     uint16_t index = 0;
//     
//     for (uint8_t page = y; page < page_end; page++) 
//     {
//         OLED_SetCursor(page, x);
//         for (uint8_t col = x; col < col_end; col++) 
//         {
//             uint8_t data = bmp[index++];
//             OLED_WriteData(mode ? data : ~data); // 根据mode取反
//         }
//     }
// }

//=======================================
// 显示日期（格式：YYYY-MM-DD）
// 参    数：Line - 行号（1-8）
// 参    数：Column - 列号（1-16）
// 参    数：DateTime - 完整的日期时间字符串（格式：YYYY-MM-DD HH:MM:SS）
//=======================================
void OLED_ShowDate(uint8_t Line, uint8_t Column, char *DateTime)
{
    char date_part[11]; // 存储年月日部分
    
    // 分割年月日部分（前10个字符）
    for(int i = 0; i < 10; i++) {
        date_part[i] = DateTime[i];
    }
    date_part[10] = '\0'; // 结束符
    
    OLED_ShowString(Line, Column, date_part);
}

//=======================================
// 显示时间（格式：HH:MM:SS）
// 参    数：Line - 行号（1-8）
// 参    数：Column - 列号（1-16）
// 参    数：DateTime - 完整的日期时间字符串（格式：YYYY-MM-DD HH:MM:SS）
//=======================================
void OLED_ShowTime(uint8_t Line, uint8_t Column, char *DateTime)
{
    char time_part[9];  // 存储时分秒部分
    
    // 分割时分秒部分（跳过空格，从第11个字符开始）
    for(int i = 0; i < 8; i++) {
        time_part[i] = DateTime[i + 11];
    }
    time_part[8] = '\0'; // 结束符
    
    OLED_ShowString(Line, Column, time_part);
}

//=======================================
// 显示温度（温度: XX°C）
// 参    数：Line - 行号（1-4）
// 参    数：Column - 列号（1-8）
// 参    数：temp_data - 温度值
//=======================================
void OLED_ShowTemperature(uint8_t Line, uint8_t Column, uint8_t temp_data)
{
    // 显示"温"字（索引13）
    OLED_ShowCN(Line, Column, 13, 1);
    // 显示"度"字（索引14）
    OLED_ShowCN(Line, Column + 1, 14, 1);
    // 显示冒号
    OLED_ShowChar(Line, Column + 4 , ':');
    // 显示温度值
    OLED_ShowNum(Line, Column + 5, temp_data, 2);
    // 显示摄氏度符号
    OLED_ShowChar(Line, Column + 7, 'C');
}

//=======================================
// 显示湿度（湿度: XX%）
// 参    数：Line - 行号（1-4）
// 参    数：Column - 列号（1-8）
// 参    数：humidity_data - 湿度值
//=======================================
void OLED_ShowHumidity(uint8_t Line, uint8_t Column, uint8_t humidity_data)
{
    // 显示"湿"字（索引15）
    OLED_ShowCN(Line, Column, 15, 1);
    // 显示"度"字（索引14）
    OLED_ShowCN(Line, Column + 1, 14, 1);
    // 显示冒号
    OLED_ShowChar(Line, Column + 4 , ':');
    // 显示湿度值
    OLED_ShowNum(Line, Column + 5, humidity_data, 2);
    // 显示百分号
    OLED_ShowChar(Line, Column + 7, '%');
}

//=======================================
// 显示光强（光强: XX%）
// 参    数：Line - 行号（1-4）
// 参    数：Column - 列号（1-8）
// 参    数：lux_data - 光强值
//=======================================
void OLED_ShowLight(uint8_t Line, uint8_t Column, uint8_t lux_data)
{
    // 显示"光"字（索引16）
    OLED_ShowCN(Line, Column, 16, 1);
    // 显示"强"字（索引18）
    OLED_ShowCN(Line, Column + 1, 18, 1);
    // 显示冒号
    OLED_ShowChar(Line, Column + 4 , ':');
    // 显示光强值
    OLED_ShowNum(Line, Column + 5, lux_data, 2);
    // 显示百分号
    OLED_ShowChar(Line, Column + 7, '%');
}

//=======================================
// 显示初始化提示（初始化中……）
//=======================================
void OLED_ShowInit(void)
{
    OLED_Clear();
    OLED_ShowCN(2, 1, 0, 1); // 显示"初"字（索引0）
    OLED_ShowCN(2, 2, 1, 1); // 显示"始"字（索引1）
    OLED_ShowCN(2, 3, 2, 1); // 显示"中"字（索引3）
    OLED_ShowCN(2, 4, 3, 1); // 显示"中"字（索引3）
    OLED_ShowChar(2, 9, '.'); // 显示点号
    OLED_ShowChar(2, 10, '.'); // 显示点号
    OLED_ShowChar(2, 11, '.'); // 显示点号
    OLED_ShowChar(2, 12, '.'); // 显示点号
    OLED_ShowChar(2, 13, '.'); // 显示点号
    OLED_ShowChar(2, 14, '.'); // 显示点号
}