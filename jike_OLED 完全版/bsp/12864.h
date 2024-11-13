#ifndef __BSP_12864_H
#define __BSP_12864_H
 
#include "i2c.h"
 
#define I2C_X           hi2c2
 
#define OLED_ADDR       0x78
#define CMD_CTRL        0X00
#define DATA_CTRL       0X40
 
void OLED_Write_Cmd(uint8_t cmd);
void OLED_Write_Data(uint8_t data);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_full(void);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_ShowStr(unsigned char x,unsigned y,unsigned char ch[],unsigned TextSize);
void OLED_32x32_Chinese(int x,int y,int font);
void OLED_16x16_Chinese(int x,int y,int font);



/*------------ÏÔ´æº¯Êý--------------------------------------*/
void OLED_DrawData(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t *data, uint8_t mode);
void OLED_DrawPoint2(uint8_t x, uint8_t y, uint8_t c);
void OLED_GRAM_Reset(void);
void OLED_GRAM_Refresh(void);
void OLED_GRAM_full(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t c);


#endif /* __BSP_12864_H */
