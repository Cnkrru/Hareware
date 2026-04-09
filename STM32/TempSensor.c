#include "stm32f10x.h"
#include "Delay.h"
#include <math.h>
#include "TempSensor.h"

//=======================================
// 热敏传感器初始化
// 包含GPIO初始化和ADC初始化
//=======================================
void TempSensor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}

//=======================================
// 读取ADC值
// 返回值：ADC转换值
//=======================================
uint16_t Temp_ADC_Read(void)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

    return ADC_GetConversionValue(ADC1);
}

//=======================================
// 读取平均ADC值
// 返回值：10次平均ADC值
//=======================================
uint16_t Temp_Average_Data(void)
{
    uint32_t tempData = 0;
    uint8_t i;

    for (i = 0; i < TEMP_READ_TIMES; i++)
    {
        tempData += Temp_ADC_Read();
        Delay_ms(5);
    }

    tempData /= TEMP_READ_TIMES;
    return (uint16_t)tempData;
}

//=======================================
// 读取温度值
// 返回值：温度值 (单位: 0.1℃)
//=======================================
int16_t Temp_GetValue(void)
{
    float voltage = 0;
    float R = 0;
    float Temp = 0;

    voltage = Temp_Average_Data();
    voltage = voltage / 4096 * 3.3f;

    R = voltage / (3.3f - voltage) * 10000;

    Temp = 1 / (log(R / 10000) / 3950 + 1 / (273.15 + 25)) - 273.15;

    return (int16_t)(Temp * 10);
}
