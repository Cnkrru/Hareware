#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Buzzer_music.h"
#include "Buzzer.h"

// 默认音量定义（用户可以直接修改这里的默认值）
#define BUZZER_DEFAULT_VOLUME 5


//=======================================
// 蜂鸣器初始化
//=======================================
void Buzzer_Init(void)
{
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	// GPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);							//将PB15引脚初始化为推挽输出
	
	// 设置GPIO初始化后的默认电平
	GPIO_SetBits(GPIOB, GPIO_Pin_15);								//设置PB15引脚为高电平
}

//=======================================
// 蜂鸣器开启
//=======================================
void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);		//设置PB15引脚为低电平
}

//=======================================
// 蜂鸣器关闭
//=======================================
void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_15);		//设置PB15引脚为高电平
}

//=======================================
// 蜂鸣器状态翻转
//=======================================
void Buzzer_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_15);								//则设置PB15引脚为高电平
	}
	else																			//否则，即当前引脚输出高电平
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);								//则设置PB15引脚为低电平
	}
}

//=======================================
// 蜂鸣器播放音调
// 参    数：frequency - 频率（Hz）
// 参    数：duration - 持续时间（ms）
// 参    数：volume - 音量（0-100）
//=======================================
void Buzzer_PlayTone(uint16_t frequency, uint16_t duration, uint8_t volume)
{
    if (frequency == 0) {
        Delay_ms(duration);
        return;
    }
    
    // 限制音量范围
    if (volume > 100) volume = 100;
    if (volume < 0) volume = 0;
    
    uint32_t period = 1000000 / frequency; // 周期（微秒）
    uint32_t onTime = (period * volume) / 100; // 导通时间
    uint32_t offTime = period - onTime; // 关闭时间
    uint32_t endTime = duration * 1000; // 转换为微秒
    uint32_t startTime = 0;
    
    while (startTime < endTime) {
        Buzzer_ON();
        Delay_us(onTime);
        Buzzer_OFF();
        Delay_us(offTime);
        startTime += period;
    }
}

//=======================================
// 蜂鸣器播放音阶
// 参    数：note - 音符索引（0=C, 1=D, 2=E, 3=F, 4=G, 5=A, 6=B）
// 参    数：note_mode - 音阶模式（0=低, 1=中, 2=高）
// 参    数：duration - 持续时间（ms）
// 参    数：volume - 音量（0-100）
//=======================================
void Buzzer_PlayNote(uint8_t note, uint8_t note_mode, uint16_t duration, uint8_t volume)
{
    if (note < 7) {
        switch (note_mode) {
            case 0:
                Buzzer_PlayTone(LowNotes[note], duration, volume);
                break;
            case 1:
                Buzzer_PlayTone(MidNotes[note], duration, volume);
                break;
            case 2:
                Buzzer_PlayTone(HighNotes[note], duration, volume);
                break;
            default:
                break;
        }
    }
}

//=======================================
// 蜂鸣器播放音调（使用默认音量）
// 参    数：frequency - 频率（Hz）
// 参    数：duration - 持续时间（ms）
//=======================================
void Buzzer_PlayTone_Default(uint16_t frequency, uint16_t duration)
{
    Buzzer_PlayTone(frequency, duration, BUZZER_DEFAULT_VOLUME);
}

//=======================================
// 蜂鸣器播放音阶（使用默认音量）
// 参    数：note - 音符索引（0=C, 1=D, 2=E, 3=F, 4=G, 5=A, 6=B）
// 参    数：note_mode - 音阶模式（0=低, 1=中, 2=高）
// 参    数：duration - 持续时间（ms）
//=======================================
void Buzzer_PlayNote_Default(uint8_t note, uint8_t note_mode, uint16_t duration)
{
    Buzzer_PlayNote(note, note_mode, duration, BUZZER_DEFAULT_VOLUME);
}

//=======================================
// 播放歌曲（指定音量）
// 参    数：musicIndex - 乐谱索引
// 参    数：volume - 音量（0-100）
//=======================================
void Buzzer_PlayMusic_WithVolume(uint8_t musicIndex, uint8_t volume)
{
    switch (musicIndex) {
        case 0:
        {
            // 播放打上花火
            uint32_t i;
            for (i = 0; i < sizeof(HanabiMusic) / sizeof(MusicNote); i++) {
                Buzzer_PlayNote(
                    HanabiMusic[i].note,
                    HanabiMusic[i].note_mode,
                    HanabiMusic[i].duration,
                    volume
                );
                // 短暂延迟，让音符之间有间隔
                Delay_ms(50);
            }
            break;
        }
        default:
            // 未知乐谱索引
            break;
    }
}

//=======================================
// 播放歌曲（使用默认音量）
// 参    数：musicIndex - 乐谱索引
//=======================================
void Buzzer_PlayMusic(uint8_t musicIndex)
{
    Buzzer_PlayMusic_WithVolume(musicIndex, BUZZER_DEFAULT_VOLUME);
}
