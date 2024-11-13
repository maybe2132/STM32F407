#ifndef __ADC_H__
#define __ADC_H__
#include "main.h"

#endif
extern uint16_t GET_ADC(ADC_HandleTypeDef hadc,uint32_t ch);
extern uint16_t GET_ADC_AVERAGE(ADC_HandleTypeDef hadc,uint32_t ch,uint8_t times);