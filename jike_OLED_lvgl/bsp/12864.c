#include "main.h"
#include "12864.h"
#include "i2c.h"
#include "font.h"
#include <stdlib.h>
/**
  * @brief �� oled д������
  */
void OLED_Write_Cmd(uint8_t cmd)
{
			
			HAL_I2C_Mem_Write(&I2C_X, OLED_ADDR, CMD_CTRL, I2C_MEMADD_SIZE_8BIT, &cmd, 1,0xffff);
}
 
/**
  * @brief �� oled д������
  */
void OLED_Write_Data(uint8_t data)
{
    HAL_I2C_Mem_Write(&I2C_X, OLED_ADDR, DATA_CTRL, I2C_MEMADD_SIZE_8BIT, &data, 1, 0xFFFF);
}
 

void OLED_SendCmd(uint8_t cmd)
{
	uint8_t sendbuffer[2];
	sendbuffer[0]=0x00;
	sendbuffer[1]=cmd;
	HAL_I2C_Mem_Write(&I2C_X, OLED_ADDR, DATA_CTRL, I2C_MEMADD_SIZE_8BIT, sendbuffer, 2, 0xFFFF);

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
 


#define OLED_HEIGHT 64
#define OLED_WIDTH	128
/* 1.��������ʾ��RAM�ȴ���Դ� */
uint8_t OLED_GRAM[OLED_HEIGHT / 8][OLED_WIDTH];


void OLED_NewFrame(){
	memset(OLED_GRAM,0,sizeof(OLED_GRAM));
}


void OLED_ShowFrame(){
		OLED_Set_Pos(0,0);
uint8_t sendBuffer[129];
	sendBuffer[0]=0x40;
	for(uint8_t i=0;i<8;i++)
	{
		for(uint8_t j=0;j<128;j++)
		{
			sendBuffer[j+1]=OLED_GRAM[i][j];
		}
	OLED_Set_Pos(0,i);
	HAL_I2C_Master_Transmit(&I2C_X, OLED_ADDR, sendBuffer, sizeof(sendBuffer),0xffff);
	}
}
void OLED_Test(){
	
	uint8_t sendBuffer[]={0X40,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,};
	HAL_I2C_Master_Transmit(&I2C_X, OLED_ADDR, sendBuffer, sizeof(sendBuffer),0xffff);
}

/* 2.���Դ��е�����λ�û���һ���� */
//(x,y)���� - x:0~127 y:0~63
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t c)
{
		if(x>=128||y>=64)return;
		OLED_GRAM[y/8][x] |= 0x01 << (y%8);
}
















#if 1
/* 2.��OLED�е�����λ�û���һ���� */
//(x,y)���� - x:0~127 y:0~63
void OLED_DrawPoint2(uint8_t x, uint8_t y, lv_color_t *c)
{
				OLED_NewFrame();
				OLED_DrawPoint(x,y,1);
				OLED_ShowFrame();
}
#else
void OLED_DrawPoint2(uint8_t x, uint8_t y, uint8_t *c)
{
    uint8_t pageAddr = y / 8;
    uint8_t colAddr = x;
    uint8_t bitAddr = y % 8;
	OLED_Set_Pos(x,y);
    //��ַ����
    if (x < OLED_WIDTH && y < OLED_HEIGHT)
    {

            OLED_GRAM[pageAddr][colAddr] |= 1 << bitAddr;
						OLED_Write_Data(OLED_GRAM[pageAddr][colAddr]);
        
    }
}
#endif
///* 3.���Դ��е�������������д����ʾ��RAM�� */
//void OLED_GRAM_Refresh(void)
//{
//    uint8_t i, j;
//    //д������Ļ������
//    for (j = 0; j < OLED_HEIGHT / 8; j++)
//    {
//        //дһ��ҳ������
//        OLED_Set_Pos(j, 0);
//        for (i = 0; i < OLED_WIDTH; i++)
//            OLED_Write_Data(OLED_GRAM[j][i]);
//    }
//}

//void OLED_GRAM_Reset(void)
//{
//    uint8_t i, j;
//    for (i = 0; i < OLED_HEIGHT / 8; i++)
//    {
//        for (j = 0; j < OLED_WIDTH; j++)
//            OLED_GRAM[i][j] = 0x00;
//    }
//}
//void OLED_GRAM_full(void)
//{
//	
//    uint8_t i, j;
//    for (i = 0; i < OLED_HEIGHT / 8; i++)
//    {
//        for (j = 0; j < OLED_WIDTH; j++)
//            OLED_GRAM[i][j] = 0xFF;
//    }
//}

#include "stdio.h"

uint8_t locate_HZ(uint8_t hz[])
{
	uint8_t i;
	for(i=0;i<HZK_length;i++)
	{
		if(hz[0]==hz_library[i].hz[0])
			if(hz[1]==hz_library[i].hz[1])
			{
				break;
			}
	}
	if(i==HZK_length)
	{
		return 255;
	}
	else
	{
		return i;
	}
	return 0;
}
#include <string.h>
void DZ_Show_GB2312(uint8_t x,uint8_t y,uint8_t hz[])
{
	int len = strlen(hz)/2;
	uint8_t i,num;
  uint8_t *p;	
	for(int m=0;m<len;m++)
	{
		num=locate_HZ(hz);
	}
	num=locate_HZ(hz);//Ѱ������
	if(num==255)//û�ҵ����֣���ʾһ��Բ
	{
		printf("��\n");
	}
	else//�ҵ����֣���ʾ����
	{
		p=hz_library[num].Data;
		OLED_Set_Pos(x,y);
		for(i=0;i<15;i++)//�ϰ���
		{
			OLED_Write_Data(*p);
			p++;
		}
		OLED_Set_Pos(x,y+1);
		for(i=0;i<15;i++)//�°���
		{
			OLED_Write_Data(*p);
			p++;
		}
	}
}
