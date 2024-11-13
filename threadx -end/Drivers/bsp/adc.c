#include "main.h"
#include "adc.h"
/*
*****************************************************************************
										���ADCֵ
																		//���ADCֵ
																		//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
																		//����ֵ:ת�����
******************************************************************************
*/

uint16_t GET_ADC(ADC_HandleTypeDef hadc,uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC_ChanConf;
    
    ADC_ChanConf.Channel=ch;                                   //ͨ��
    ADC_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //����ʱ��
    ADC_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&hadc,&ADC_ChanConf);        //ͨ������

    HAL_ADC_Start(&hadc);                               //����ADC
    
    HAL_ADC_PollForConversion(&hadc,10);                //��ѯת��
 
    return (uint16_t)HAL_ADC_GetValue(&hadc);            //�������һ��ADC1�������ת�����
}
/*
*****************************************************************************
										��ȡָ��ͨ����ת��ֵ
																		//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
																		//times:��ȡ����
																		//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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

