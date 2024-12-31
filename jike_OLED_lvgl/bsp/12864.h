#ifndef __BSP_12864_H
#define __BSP_12864_H
 
#include "i2c.h"
 #include "lvgl.h" 
#include "lv_port_disp_template.h"
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
void LCD_DisplayChinese(uint16_t x,uint16_t y,uint16_t size,char *Font);
void DZ_Show_GB2312(uint8_t x,uint8_t y,uint8_t hz[]);

/*------------ÏÔ´æº¯Êý--------------------------------------*/
void OLED_DrawData(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t *data, uint8_t mode);
void OLED_DrawPoint2(uint8_t x, uint8_t y, lv_color_t *c);
void OLED_GRAM_Reset(void);
void OLED_GRAM_Refresh(void);
void OLED_GRAM_full(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t c);
void OLED_NewFrame();
void OLED_ShowFrame();
void OLED_Test();
#endif /* __BSP_12864_H */
