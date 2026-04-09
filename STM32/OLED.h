//=======================================
// OLED模块头文件
//=======================================
#ifndef __OLED_H
#define __OLED_H

//=======================================
// 函数声明
//1. OLED初始化
//2. OLED清屏
//3. OLED显示一个字符
//4. OLED显示字符串
//5. OLED显示数字（十进制，正数）
//6. OLED显示数字（十进制，带符号数）
//7. OLED显示数字（十六进制，正数）
//8. OLED显示数字（二进制，正数）
//9. 绘制点
//10. 绘制直线
//11. 绘制圆
//12. OLED显示汉字
//13. OLED显示位图
//14. 显示日期（格式：YYYY-MM-DD）
//15. 显示时间（格式：HH:MM:SS）
//16. 显示温度（温度: XX°C）
//17. 显示湿度（湿度: XX%）
//18. 显示光强（光强: XX%）
//19. 显示初始化提示（初始化中……）
//=======================================
// 函数声明
//=======================================
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char Color);
void OLED_Draw_Line(unsigned char X1, unsigned char Y1, unsigned char X2, unsigned char Y2, unsigned char Color);
void OLED_Draw_Circle(unsigned char X, unsigned char Y, unsigned char Radius, unsigned char Color);
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num, uint8_t mode);
void OLED_ShowBMP(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *bmp, uint8_t mode);
void OLED_ShowDate(uint8_t Line, uint8_t Column, char *DateTime);
void OLED_ShowTime(uint8_t Line, uint8_t Column, char *DateTime);
void OLED_ShowTemperature(uint8_t Line, uint8_t Column, uint8_t temp_data);
void OLED_ShowHumidity(uint8_t Line, uint8_t Column, uint8_t humidity_data);
void OLED_ShowLight(uint8_t Line, uint8_t Column, uint8_t lux_data);
void OLED_ShowInit(void);

#endif
