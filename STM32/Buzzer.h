//=======================================
// 蜂鸣器模块头文件
//=======================================
#ifndef __BUZZER_H
#define __BUZZER_H



//=======================================
// 函数声明
//1. 蜂鸣器初始化
//2. 蜂鸣器开启
//3. 蜂鸣器关闭
//4. 蜂鸣器状态翻转
//5. 蜂鸣器播放音调
//6. 蜂鸣器播放音阶
//7. 设置默认音量
//=======================================
// 函数声明
//=======================================
void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
void Buzzer_Turn(void);
void Buzzer_PlayTone(uint16_t frequency, uint16_t duration, uint8_t volume);
void Buzzer_PlayTone_Default(uint16_t frequency, uint16_t duration);
// note_mode: 0=低音, 1=中音, 2=高音
void Buzzer_PlayNote(uint8_t note, uint8_t note_mode, uint16_t duration, uint8_t volume);
void Buzzer_PlayNote_Default(uint8_t note, uint8_t note_mode, uint16_t duration);
void Buzzer_SetDefaultVolume(uint8_t volume);
// 播放歌曲
void Buzzer_PlayMusic(uint8_t musicIndex);
void Buzzer_PlayMusic_WithVolume(uint8_t musicIndex, uint8_t volume);

#endif
