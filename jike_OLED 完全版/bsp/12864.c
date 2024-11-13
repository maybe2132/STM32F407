#include "main.h"
#include "12864.h"
#include "i2c.h"
#include "font.h"
/**
  * @brief 向 oled 写入命令
  */
void OLED_Write_Cmd(uint8_t cmd)
{
    uint8_t temp = cmd;
			HAL_I2C_Mem_Write(&I2C_X, OLED_ADDR, CMD_CTRL, I2C_MEMADD_SIZE_8BIT, &temp, 1,0xffff);
}
 
/**
  * @brief 向 oled 写入数据
  */
void OLED_Write_Data(uint8_t data)
{
    uint8_t temp = data;
    HAL_I2C_Mem_Write(&I2C_X, OLED_ADDR, DATA_CTRL, I2C_MEMADD_SIZE_8BIT, &temp, 1, 0xFFFF);
}
 
/**
  * @brief 设置 oled 写入坐标 
  * @param x    列坐标：0 ~ 127
  * @param y    行坐标：0 ~ 63
  */
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
    OLED_Write_Cmd(0xb0 + y);                 // 写入页地址
    OLED_Write_Cmd((x & 0x0f));               // 写入列的地址 (低半字节)
    OLED_Write_Cmd(((x & 0xf0) >> 4) | 0x10); // 写入列的地址(高半字节)
}
 
/**
  * @brief oled 清屏
  */
void OLED_Clear(void)
{
    unsigned char page, row;
    for (page = 0; page < 8; page++)
    {
        OLED_Write_Cmd(0xb0 + page); // 从0 ~ 7页依次写入
        OLED_Write_Cmd(0x00);        // 低位列地址
        OLED_Write_Cmd(0x10);        // 高位列地址
        for (row = 0; row < 128; row++)
        {
            OLED_Write_Data(0X00);
        }
    }
}
 void OLED_full(void)
{
    unsigned char page, row;
    for (page = 0; page < 8; page++)
    {
        OLED_Write_Cmd(0xb0 + page); // 从0 ~ 7页依次写入
        OLED_Write_Cmd(0x00);        // 低位列地址
        OLED_Write_Cmd(0x10);        // 高位列地址
        for (row = 0; row < 128; row++)
        {
            OLED_Write_Data(0XFF);
        }
    }
}
 

const unsigned char  OLED_init_cmd[25]=
{
  
      0xAE,//关闭显示
	
       0xD5,//设置时钟分频因子,震荡频率
       0x80,  //[3:0],分频因子;[7:4],震荡频率

       0xA8,//设置驱动路数
       0X3F,//默认(1/64)
	
       0xD3,//设置显示偏移
       0X00,//默认为0
	
       0x40,//设置显示开始行 [5:0],行数.
	
       0x8D,//电荷泵设置
       0x10,//bit2，开启/关闭
       0x20,//设置内存地址模式
       0x02,//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
       0xA1,//段重定义设置,bit0:0,0->0;1,0->127;  A1
	
       0xC8,//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 (C0 翻转显示) C8
	   
       0xDA,//设置COM硬件引脚配置
       0x12,//[5:4]配置  
	   
       0x81,//对比度设置
       0xf0,//1~255;默认0X7F (亮度设置,越大越亮)
	   
       0xD9,//设置预充电周期
       0x71,//[3:0],PHASE 1;[7:4],PHASE 2;
	   
       0xDB,//设置VCOMH 电压倍率
       0x00,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	   
       0xA4,//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	   
       0xA6,//设置显示方式;bit0:1,反相显示;0,正常显示 
       
       0xAF,//开启显示   
};

void OLED_Init(void)
  {
		HAL_Delay(100);
    unsigned char i;
    for(i=0;i<25;i++)
      {
        OLED_Write_Cmd(OLED_init_cmd[i]);
      }
  }
 
/**
  * @brief 将 OLED 从休眠中唤醒
  */
void OLED_ON(void)
{
    OLED_Write_Cmd(0X8D); // 设置电荷泵
    OLED_Write_Cmd(0X14); // 开启电荷泵
    OLED_Write_Cmd(0XAF); // OLED 唤醒
}
 
/**
  * @brief 让OLED休眠
  */
void OLED_OFF(void)
{
    OLED_Write_Cmd(0X8D); // 设置电荷泵
    OLED_Write_Cmd(0X10); // 关闭电荷泵
    OLED_Write_Cmd(0XAE); // OLED 休眠
}
 
/**
  * @brief 显示 bsp_font.h 中的ASCII字符 字体为 Consolas
  * @param x：  起始点列坐标：0 ~ 127
  *        y:   起始点页坐标：0 ~ 7
* @param	font: 中文字符
  */
void OLED_16x16_Chinese(int x,int y,int font)
{
	int n,m=0;
	switch(font)
	{
		case 1:															//盛
					for(n=0;n<2;n++)
						{
							OLED_Set_Pos(x,y+n);
							for(int i=0;i<16;i++)
							OLED_Write_Data(chinese_16x16_1[i+16*n]);
						}
			break;
		case 2:																//剑
					for(n=0;n<2;n++)
						{
							OLED_Set_Pos(x,y+n);
							for(int i=0;i<16;i++)
							OLED_Write_Data(chinese_16x16_2[i+16*n]);
						}
			break;
		case 3:
			break;
			case 4:
			break;
	}
			
}
void OLED_32x32_Chinese(int x,int y,int font)
{
	int n,m=0;
	switch(font)
	{
		case 1:																//盛
			for(n=0;n<4;n++)
						{
							OLED_Set_Pos(x,y+n);
							for(int i=0;i<16;i++)
							OLED_Write_Data(chinese_16x16_3[i+32*n]);
						}

				for(m=0;m<4;m++)
						{
							OLED_Set_Pos(x+16,y+m);
							for(int i=0;i<16;i++)
							OLED_Write_Data(chinese_16x16_3[i+32*m+16]);
						}
			break;
			case 2:																//剑
			for(n=0;n<4;n++)
						{
							OLED_Set_Pos(x,y+n);
							for(int i=0;i<16;i++)
							OLED_Write_Data(chinese_16x16_4[i+32*n]);
						}

				for(m=0;m<4;m++)
						{
							OLED_Set_Pos(x+16,y+m);
							for(int i=0;i<16;i++)
							OLED_Write_Data(chinese_16x16_4[i+32*m+16]);
						}
			break;
			case 3:
				for(n=0;n<4;n++)
						{
							OLED_Set_Pos(x,y+n);
							for(int i=0;i<16;i++)
							OLED_Write_Data(BMP1[i+32*n]);
						}

				for(m=0;m<4;m++)
						{
							OLED_Set_Pos(x+16,y+m);
							for(int i=0;i<16;i++)
							OLED_Write_Data(BMP1[i+32*m+16]);
						}
				break;
			case 4:
				for(n=0;n<4;n++)
						{
							OLED_Set_Pos(x,y+n);
							for(int i=0;i<16;i++)
							OLED_Write_Data(image_32x32_3[i+32*n]);
						}

				for(m=0;m<4;m++)
						{
							OLED_Set_Pos(x+16,y+m);
							for(int i=0;i<16;i++)
							OLED_Write_Data(image_32x32_3[i+32*m+16]);
						}
				break;
	}
			
}
void OLED_ShowStr(unsigned char x,unsigned y,unsigned char ch[],unsigned TextSize)
{
    unsigned char c = 0,i = 0,j = 0;
	  switch(TextSize)
		{
			case 1://显示的大小为6*8
			{
				while(ch[j] != '\0')
				{
				   c = ch[j] - 32;//大小写之间的转化
					if(x>126)
					{
					    x= 0;
						  y++;
					
					}
					OLED_Set_Pos(x,y);
					for(i=0;i<6;i++)
           OLED_Write_Data( ascii_6x8[c][i] );
					x+=6 ;
					j++;
					
					}
				
				}break;
			case 2://显示的大小为8*16
			{
			   while(ch[j] !='\0')
				 {
						 c= ch[j] - 32;
					 if(x >120)
					 {
							 x = 0;
							 y++ ;
					 }
					 OLED_Set_Pos(x,y);
					 for(i = 0;i<8;i++)			//这里分两次循环显示
					 OLED_Write_Data( F8X16[c*16+i] ); //比如这是显示H的上半部分
					 OLED_Set_Pos(x,y+1);
					 for(i = 0;i<8;i++)			//这是显示H的下半部分
					 OLED_Write_Data( F8X16[c*16+i+8] );
					 x+=8;//跳转到下一页
					 j++;
			 }
				 
		}break;
			
		}
}		  
void OLED_DrawData(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t *data, uint8_t mode)
{
    uint16_t i, j;
    uint8_t x0 = x;
    uint8_t y0 = y;
    uint8_t byte;
    //高度补全
    while (h % 8) h++;
    for (i = 0; i < w * h / 8; i++)
    {
        //绘制一个字节
        byte = data[i];
        for (j = 0; j < 8; j++)
        {
            //绘制一位
            if ((byte >> j) & 0x01)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            //行偏移
            y++;
        }
        //列偏移
        x++;
        //判断当前页的数据是否写完，切换下一页
        if (x - x0 == w)
        {
            x = x0;
            y0 += 8;
        }
        y = y0;
    }
}
/**
  * @brief 显示 bsp_font.h 中的汉字（宋体）
  * @param x：  起始点列坐标：0 ~ 127
  *        y:   起始点页坐标：0 ~ 7
  * @param n：  汉字在字库中的索引
//  */

#define OLED_HEIGHT 64
#define OLED_WIDTH	128
/* 1.构建和显示屏RAM等大的显存 */
uint8_t OLED_GRAM[OLED_HEIGHT / 8][OLED_WIDTH];

/* 2.在显存中的任意位置绘制一个点 */
//(x,y)坐标 - x:0~127 y:0~63
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t c)
{
    uint8_t pageAddr = y / 8;
    uint8_t colAddr = x;
    uint8_t bitAddr = y % 8;
    //地址过滤
    if (x < OLED_WIDTH && y < OLED_HEIGHT)
    {
        if (c)
            OLED_GRAM[pageAddr][colAddr] |= 1 << bitAddr;
        else
            OLED_GRAM[pageAddr][colAddr] &= ~(1 << bitAddr);
    }
}

/* 2.在OLED中的任意位置绘制一个点 */
//(x,y)坐标 - x:0~127 y:0~63
void OLED_DrawPoint2(uint8_t x, uint8_t y, uint8_t c)
{
    uint8_t pageAddr = y / 8;
    uint8_t colAddr = x;
    uint8_t bitAddr = y % 8;
	OLED_Set_Pos(x,y);
    //地址过滤
    if (x < OLED_WIDTH && y < OLED_HEIGHT)
    {
        if (c)
				{
            OLED_GRAM[pageAddr][colAddr] |= 1 << bitAddr;
						OLED_Write_Data(OLED_GRAM[pageAddr][colAddr]);
        }
					else
            OLED_GRAM[pageAddr][colAddr] &= ~(1 << bitAddr);
    }
}
/* 3.将显存中的数据依次连续写入显示屏RAM中 */
void OLED_GRAM_Refresh(void)
{
    uint8_t i, j;
    //写整个屏幕的数据
    for (j = 0; j < OLED_HEIGHT / 8; j++)
    {
        //写一整页的数据
        OLED_Set_Pos(j, 0);
        for (i = 0; i < OLED_WIDTH; i++)
            OLED_Write_Data(OLED_GRAM[j][i]);
    }
}

void OLED_GRAM_Reset(void)
{
    uint8_t i, j;
    for (i = 0; i < OLED_HEIGHT / 8; i++)
    {
        for (j = 0; j < OLED_WIDTH; j++)
            OLED_GRAM[i][j] = 0x00;
    }
}
void OLED_GRAM_full(void)
{
    uint8_t i, j;
    for (i = 0; i < OLED_HEIGHT / 8; i++)
    {
        for (j = 0; j < OLED_WIDTH; j++)
            OLED_GRAM[i][j] = 0xFF;
    }
}

