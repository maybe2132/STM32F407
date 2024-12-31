#ifndef OLED_OLEDFONT_H_
#define OLED_OLEDFONT_H_
#include "main.h"
extern uint8_t ascii_6x8[92][6];
extern  unsigned char F6x8[][6];
extern  unsigned char F8X16[];
extern 	uint8_t ascii_8x16[95][16];
extern  unsigned char Hzk[][32];
extern unsigned char BMP1[];
extern uint8_t chinese_16x16_1[];
extern uint8_t chinese_16x16_2[];
extern uint8_t chinese_16x16_3[];
extern uint8_t chinese_16x16_4[];
extern unsigned char BMP1[];
extern uint8_t image_32x32_1[];
extern uint8_t image_32x32_3[]; 
#define HZK_length 100

//extern unsigned char BMP2[].........
typedef struct
{
char hz[2];//ºº×Ö×Ö·û
unsigned char Data[30];//ºº×ÖµãÕó
}HZ_Struct;
extern HZ_Struct hz_library[HZK_length];
#endif /* OLED_OLEDFONT_H_ */

