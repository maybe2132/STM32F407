#include "main.h"
#include "12864.h"
#include "i2c.h"
#include "font.h"
/**
  * @brief �� oled д������
  */
void OLED_Write_Cmd(uint8_t cmd)
{
    uint8_t temp = cmd;
			HAL_I2C_Mem_Write(&I2C_X, OLED_ADDR, CMD_CTRL, I2C_MEMADD_SIZE_8BIT, &temp, 1,0xffff);
}
 
/**
  * @brief �� oled д������
  */
void OLED_Write_Data(uint8_t data)
{
    uint8_t temp = data;
    HAL_I2C_Mem_Write(&I2C_X, OLED_ADDR, DATA_CTRL, I2C_MEMADD_SIZE_8BIT, &temp, 1, 0xFFFF);
}
 
/**
  * @brief ���� oled д������ 
  * @param x    �����꣺0 ~ 127
  * @param y    �����꣺0 ~ 63
  */
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
    OLED_Write_Cmd(0xb0 + y);                 // д��ҳ��ַ
    OLED_Write_Cmd((x & 0x0f));               // д���еĵ�ַ (�Ͱ��ֽ�)
    OLED_Write_Cmd(((x & 0xf0) >> 4) | 0x10); // д���еĵ�ַ(�߰��ֽ�)
}
 
/**
  * @brief oled ����
  */
void OLED_Clear(void)
{
    unsigned char page, row;
    for (page = 0; page < 8; page++)
    {
        OLED_Write_Cmd(0xb0 + page); // ��0 ~ 7ҳ����д��
        OLED_Write_Cmd(0x00);        // ��λ�е�ַ
        OLED_Write_Cmd(0x10);        // ��λ�е�ַ
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
        OLED_Write_Cmd(0xb0 + page); // ��0 ~ 7ҳ����д��
        OLED_Write_Cmd(0x00);        // ��λ�е�ַ
        OLED_Write_Cmd(0x10);        // ��λ�е�ַ
        for (row = 0; row < 128; row++)
        {
            OLED_Write_Data(0XFF);
        }
    }
}
 

const unsigned char  OLED_init_cmd[25]=
{
  
      0xAE,//�ر���ʾ
	
       0xD5,//����ʱ�ӷ�Ƶ����,��Ƶ��
       0x80,  //[3:0],��Ƶ����;[7:4],��Ƶ��

       0xA8,//��������·��
       0X3F,//Ĭ��(1/64)
	
       0xD3,//������ʾƫ��
       0X00,//Ĭ��Ϊ0
	
       0x40,//������ʾ��ʼ�� [5:0],����.
	
       0x8D,//��ɱ�����
       0x10,//bit2������/�ر�
       0x20,//�����ڴ��ַģʽ
       0x02,//[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
       0xA1,//���ض�������,bit0:0,0->0;1,0->127;  A1
	
       0xC8,//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� (C0 ��ת��ʾ) C8
	   
       0xDA,//����COMӲ����������
       0x12,//[5:4]����  
	   
       0x81,//�Աȶ�����
       0xf0,//1~255;Ĭ��0X7F (��������,Խ��Խ��)
	   
       0xD9,//����Ԥ�������
       0x71,//[3:0],PHASE 1;[7:4],PHASE 2;
	   
       0xDB,//����VCOMH ��ѹ����
       0x00,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	   
       0xA4,//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	   
       0xA6,//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ 
       
       0xAF,//������ʾ   
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
  * @brief �� OLED �������л���
  */
void OLED_ON(void)
{
    OLED_Write_Cmd(0X8D); // ���õ�ɱ�
    OLED_Write_Cmd(0X14); // ������ɱ�
    OLED_Write_Cmd(0XAF); // OLED ����
}
 
/**
  * @brief ��OLED����
  */
void OLED_OFF(void)
{
    OLED_Write_Cmd(0X8D); // ���õ�ɱ�
    OLED_Write_Cmd(0X10); // �رյ�ɱ�
    OLED_Write_Cmd(0XAE); // OLED ����
}
 
/**
  * @brief ��ʾ bsp_font.h �е�ASCII�ַ� ����Ϊ Consolas
  * @param x��  ��ʼ�������꣺0 ~ 127
  *        y:   ��ʼ��ҳ���꣺0 ~ 7
* @param	font: �����ַ�
  */
void OLED_16x16_Chinese(int x,int y,int font)
{
	int n,m=0;
	switch(font)
	{
		case 1:															//ʢ
					for(n=0;n<2;n++)
						{
							OLED_Set_Pos(x,y+n);
							for(int i=0;i<16;i++)
							OLED_Write_Data(chinese_16x16_1[i+16*n]);
						}
			break;
		case 2:																//��
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
		case 1:																//ʢ
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
			case 2:																//��
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
			case 1://��ʾ�Ĵ�СΪ6*8
			{
				while(ch[j] != '\0')
				{
				   c = ch[j] - 32;//��Сд֮���ת��
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
			case 2://��ʾ�Ĵ�СΪ8*16
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
					 for(i = 0;i<8;i++)			//���������ѭ����ʾ
					 OLED_Write_Data( F8X16[c*16+i] ); //����������ʾH���ϰ벿��
					 OLED_Set_Pos(x,y+1);
					 for(i = 0;i<8;i++)			//������ʾH���°벿��
					 OLED_Write_Data( F8X16[c*16+i+8] );
					 x+=8;//��ת����һҳ
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
    //�߶Ȳ�ȫ
    while (h % 8) h++;
    for (i = 0; i < w * h / 8; i++)
    {
        //����һ���ֽ�
        byte = data[i];
        for (j = 0; j < 8; j++)
        {
            //����һλ
            if ((byte >> j) & 0x01)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            //��ƫ��
            y++;
        }
        //��ƫ��
        x++;
        //�жϵ�ǰҳ�������Ƿ�д�꣬�л���һҳ
        if (x - x0 == w)
        {
            x = x0;
            y0 += 8;
        }
        y = y0;
    }
}
/**
  * @brief ��ʾ bsp_font.h �еĺ��֣����壩
  * @param x��  ��ʼ�������꣺0 ~ 127
  *        y:   ��ʼ��ҳ���꣺0 ~ 7
  * @param n��  �������ֿ��е�����
//  */

#define OLED_HEIGHT 64
#define OLED_WIDTH	128
/* 1.��������ʾ��RAM�ȴ���Դ� */
uint8_t OLED_GRAM[OLED_HEIGHT / 8][OLED_WIDTH];

/* 2.���Դ��е�����λ�û���һ���� */
//(x,y)���� - x:0~127 y:0~63
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t c)
{
    uint8_t pageAddr = y / 8;
    uint8_t colAddr = x;
    uint8_t bitAddr = y % 8;
    //��ַ����
    if (x < OLED_WIDTH && y < OLED_HEIGHT)
    {
        if (c)
            OLED_GRAM[pageAddr][colAddr] |= 1 << bitAddr;
        else
            OLED_GRAM[pageAddr][colAddr] &= ~(1 << bitAddr);
    }
}

/* 2.��OLED�е�����λ�û���һ���� */
//(x,y)���� - x:0~127 y:0~63
void OLED_DrawPoint2(uint8_t x, uint8_t y, uint8_t c)
{
    uint8_t pageAddr = y / 8;
    uint8_t colAddr = x;
    uint8_t bitAddr = y % 8;
	OLED_Set_Pos(x,y);
    //��ַ����
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
/* 3.���Դ��е�������������д����ʾ��RAM�� */
void OLED_GRAM_Refresh(void)
{
    uint8_t i, j;
    //д������Ļ������
    for (j = 0; j < OLED_HEIGHT / 8; j++)
    {
        //дһ��ҳ������
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

