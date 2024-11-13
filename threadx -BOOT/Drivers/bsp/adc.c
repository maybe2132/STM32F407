#include "main.h"
#include "adc.h"
/*
*****************************************************************************
										获得ADC值
																		//获得ADC值
																		//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
																		//返回值:转换结果
******************************************************************************
*/

uint16_t GET_ADC(ADC_HandleTypeDef hadc,uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC_ChanConf;
    
    ADC_ChanConf.Channel=ch;                                   //通道
    ADC_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc,&ADC_ChanConf);        //通道配置

    HAL_ADC_Start(&hadc);                               //开启ADC
    
    HAL_ADC_PollForConversion(&hadc,10);                //轮询转换
 
    return (uint16_t)HAL_ADC_GetValue(&hadc);            //返回最近一次ADC1规则组的转换结果
}
/*
*****************************************************************************
										获取指定通道的转换值
																		//获取指定通道的转换值，取times次,然后平均 
																		//times:获取次数
																		//返回值:通道ch的times次转换结果平均值
******************************************************************************
*/

uint16_t GET_ADC_AVERAGE(ADC_HandleTypeDef hadc,uint32_t ch,uint8_t times)
{
    uint32_t temp_val=0;
    uint8_t t;
    for(t=0;t<times;t++)	
    {
        temp_val+=GET_ADC(hadc,ch);
        HAL_Delay(5);
    }
    return temp_val/times;
}

