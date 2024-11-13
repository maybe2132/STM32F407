#include "esp8266.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include <stdio.h>
#include "rtc.h"
extern	uint8_t usart3[1024];
extern	uint8_t usart3_flag;
extern	uint16_t usart3_cnt;
char * pbuff=NULL;
uint16_t pbuff_cnt=0;

uint8_t eps8266SendAtcmd(char * buff,char *str)
{
	int i,j;

	for(i=0;i<5;i++)
	{
		USART3_SendData((uint8_t *)buff,strlen(buff));
		for(j=0;j<3000;j++)
			{
				if(usart3_flag)
					{	                
						usart3_flag=0;
					if(strstr((char *)usart3,str))
					{
						printf("[ok]\n");
						usart3_cnt=0;
						return 0;
					}
				}
				delay_time(1);
			}
	}
	printf("[error]\n");
	return 1;
}

uint8_t ESP8266_STA_TCPClientInit(ESP8266_InfoTypeDef espInfo)
{
	char buf[100];

	printf("[1/7]�˳�͸�������.........................................");
	eps8266SendAtcmd("+++","OK");
	delay_time(260);
	if(eps8266SendAtcmd("AT\r\n","OK"))
		return 1;
	printf("[2/7]����WIFI����ģʽΪSTA..................................");
	if(eps8266SendAtcmd("AT+CWMODE=1\r\n", "OK"))
		
		return 2;
	printf("[3/7]����AP�ȵ�.............................................");
	sprintf(buf, "AT+CWJAP_DEF=\"%s\",\"%s\"\r\n", espInfo.ssid, espInfo.pwd); /* ��̬ƴ�ӹؼ���Ϣ */
    if (eps8266SendAtcmd(buf,"OK"))
		return 3;
	printf("[4/7]����������.............................................");
    if (eps8266SendAtcmd("AT+CIPMUX=0\r\n", "OK"))
		return 4;
    printf("[5/7]���ӷ�����.............................................");
    sprintf(buf, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", espInfo.ip, espInfo.port);
    if (eps8266SendAtcmd(buf, "OK"))
		return 5;
    printf("[6/7]����͸��ģʽ...........................................");
    if (eps8266SendAtcmd("AT+CIPMODE=1\r\n", "OK"))
		return 6;
    printf("[7/7]��ʼ͸��..............................................."); /* �˺�����������ֳ�����,�Ͽ��Զ����� */  
    if (eps8266SendAtcmd("AT+CIPSEND\r\n", ">"))
		return 7;

	printf("**************************\n");
    printf("�ȵ����ƣ�%s\n", espInfo.ssid);
    printf("�ȵ����룺%s\n", espInfo.pwd);
    printf("������ip��ַ��%s\n", espInfo.ip);
    printf("�������˿ںţ�%d\n", espInfo.port);
    printf("**************************\n");
	return 0;
}


void data_processing(char *source ,weather *data)
{
	uint16_t i=0;
	pbuff=strstr(source,"name");
	if(pbuff!=NULL)
	{
		for(i=7;pbuff[i]!='"';i++)
		{
			data->name[i-7]=pbuff[i];
		}
	}
	pbuff=strstr(source,"temperature");
	if(pbuff!=NULL)
	{
		for(i=14;pbuff[i]!='"';i++)
		{
			data->temperature[i-14]=pbuff[i];
		}
	}
	pbuff=strstr(source,"text");
	if(pbuff!=NULL)
	{
		for(i=7;pbuff[i]!='"';i++)
		{
			data->text[i-7]=pbuff[i];
		}
	}		
}
void esp8266_Time_proscessing(char * source,RTC_TT *rtc_time)
{
	char *pbuff=NULL;
	uint16_t i=0;
	//Date: Fri, 07 Jul 2023 06:46:52 GMT
	pbuff=strstr(source,"Date:");
	if(pbuff!=NULL)
	{
		for(i=6;pbuff[i]!=',';i++)
		{
			rtc_time->week[i-6]=pbuff[i];
		}
	}
	pbuff=strstr(source,"Date:");
	if(pbuff!=NULL)
	{
		for(i=11;pbuff[i]!=' ';i++)
		{
			rtc_time->day[i-11]=pbuff[i];
		}
	}
	pbuff=strstr(source,"Date:");	
	if(pbuff!=NULL)
	{
		for(i=14;pbuff[i]!=' ';i++)
		{
			rtc_time->month[i-14]=pbuff[i];
		}
		
	}	
pbuff=strstr(source,"Date:");
	if(pbuff!=NULL)
	{
		for(i=18;pbuff[i]!=' ';i++)
		{
			rtc_time->year[i-18]=pbuff[i];
		}
	}
pbuff=strstr(source,"Date:");
	if(pbuff!=NULL)
	{
			for(i=23;i<24;i++)
			{
				rtc_time->hour[i-23]=pbuff[i];
			}	
	}
pbuff=strstr(source,"Date:");
	if(pbuff!=NULL)
	{
		for(i=26;i<28;i++)
		{
			rtc_time->min[i-26]=pbuff[i];
		}
	}
pbuff=strstr(source,"Date:");
	if(pbuff!=NULL)
	{
		for(i=29;i<31;i++)
		{
			rtc_time->sec[i-29]=pbuff[i];
		}
	}
pbuff=strstr(source,"Date:");
	if(pbuff!=NULL)
	{
		for(i=23;i<25;i++)
		{
			rtc_time->hour[i-23]=pbuff[i];
		}
	}
}
