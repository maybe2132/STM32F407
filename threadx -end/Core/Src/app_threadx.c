/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "stdio.h"
#include "usart.h"

int fputc(int c, FILE *stream)
{
    while (!(USART2->SR & 1<<7)){}  //等待”发送缓冲器“空
    USART2->DR = c;
    while (!(USART2->SR & 1<<6)){}  //等待“发送完成”
    return c;
}
/*
*****************************************************************************
											任务优先级
******************************************************************************
*/
#define APP_START_TASK_PRIO 2
#define APP_LED_TASK_PRIO 2
#define APP_KEY_C_LED_TASK_PRIO 2



/*
*****************************************************************************
											任务栈大小
******************************************************************************
*/
#define APP_SRART_TASK_SIZE 4096
#define APP_LED_TASK_SIZE 1024
/*
*****************************************************************************
											静态全局变量
******************************************************************************
*/
static TX_THREAD AppStartTaskTCB;
static uint64_t AppStartTask[APP_SRART_TASK_SIZE/8];

static TX_THREAD AppLedTaskTCB;
static uint64_t AppLedTask[APP_LED_TASK_SIZE/8];

static TX_THREAD AppLedRedTaskTCB;
static uint64_t AppLedRedTask[APP_LED_TASK_SIZE/8];

static TX_THREAD AppKey_C_LEDTaskTCB;
static uint64_t AppKey_C_LedTask[APP_LED_TASK_SIZE/8];

static TX_THREAD AppData_ProcessingTaskTCB;
static uint64_t AppDataProcessingTask[APP_SRART_TASK_SIZE/8];

static TX_THREAD AppData_Text3TaskTCB;
static uint64_t AppData_Text3Task[APP_SRART_TASK_SIZE/8];

static TX_THREAD Appprogressbar_TCB;
static uint64_t AppprogressbarTask[APP_SRART_TASK_SIZE/8];
Serial_Data Message_ID;
TX_QUEUE MessageQueues1;
uint8_t Rarr[100];
uint8_t Send_flag=0;
char led_flag=1;
uint8_t flash_data[10];																	//FLASH读取缓存
extern uint16_t GET_ADC_AVERAGE(ADC_HandleTypeDef hadc,uint32_t ch,uint8_t times);
extern ADC_HandleTypeDef hadc1;
float Sec_flag=0;																		//程序执行第二遍时清楚之前的数据
char progressbar_buff[1000];
int progressbar_cnt=0;
int bar_flag=0;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint32_t count1,count2,count3=0;
/*
*****************************************************************************
										数组清零函数
******************************************************************************
*/
void ClearStr(char *buff)
{
	uint16_t i=0;
	for(i=0;i<SIZE(Message_ID.Message);i++)
	{
		buff[i]=0;
	}
}


/*
*****************************************************************************
										读取CPUID
******************************************************************************
*/
 void Get_CPU_ID(uint32_t CpuID[])
{
	//read CPU ID,stm32lxx,add+4
	CpuID[0] = *(__IO uint32_t *)(0X1FFF7A10);
	CpuID[1] = *(__IO uint32_t *)(0X1FFF7A14);
	CpuID[2] = *(__IO uint32_t *)(0X1FFF7A23);
}

/*
*****************************************************************************
								外部中断控制按键
******************************************************************************
*/
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		if(GPIO_Pin == GPIO_PIN_0)
		{
			printf("button1--ok\n");
		}
		else if(GPIO_Pin ==GPIO_PIN_1)	
		{
			printf("button2--ok\n");
		}
		else if(GPIO_Pin ==GPIO_PIN_4)
		{
			printf("button3--ok\n");
			printf("progressbar:%s\n",progressbar_buff);
		}
}


/*
*****************************************************************************
								控制蓝色LED任务
******************************************************************************
*/
void my_event_flags_set_notify(TX_EVENT_FLAGS_GROUP *group_ptr);
void App_Led_BlueTask()
{
	while(1)
	{

//				Get_CPU_ID();
				tx_thread_sleep(1000);	
				HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);


	}
}

/*
*****************************************************************************
								测试任务2任务
******************************************************************************
*/



void App_Led_RedTask()
{
		char led[]="led";
		char led_ok[]="led_ok";
		char *p_led=NULL;
		char *p_ok=NULL;		
		char key_ok[]="key_ok";
		char *p_key=NULL;	
  while (1)
		{	
			count2++;
			p_led=strstr(Message_ID.Message,led);
			p_ok=strstr(Message_ID.Message,led_ok);
			p_key=strstr(Message_ID.Message,key_ok);
			if(p_led)
			{
				if(led_flag)									//防止LED一直闪烁
				{
					HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_5);
					tx_thread_sleep(500);
				}
			}
			if(p_ok)		//上位机发送LED测试成功信号
			{
				led_flag=0;
				for( int k= 0; k < SIZE(Rarr); k++)
					{
							Rarr[k]=0;
					}
				ClearStr(Message_ID.Message);	
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,1);	
				printf("LED测试完成\n");
			}
			if(p_key)		//上位机发送按键测试成功信号
			{
				printf("\n\n\n");
				printf("--------TEXT3 START---------\n");
					for( int k= 0; k < SIZE(Rarr); k++)
					{
							Rarr[k]=0;
					}
					ClearStr(Message_ID.Message);	
				tx_thread_resume(&AppData_Text3TaskTCB);
				tx_thread_suspend(&AppLedRedTaskTCB);	//挂起测试2

			}
			tx_thread_sleep(50);
		}
	
}
/*
*****************************************************************************
								测试任务3任务
******************************************************************************
*/


void App_TEXT3_Task()
{

		uint32_t CPUID[3];
		uint16_t temp_adc_value[2];
		float AD_Value[2];
		char *p_write=NULL;
		char *p_read=NULL;
		char *p_ID=NULL;
		char *p_text3=NULL;
		char *p_value=NULL;
		char read_sn[]="read_sn";
		char read_id[]="read_id";
		char text3_end[]="text3_end";
		char value[]="read_value";
		uint8_t i,j,k,n=0;
		while(1)
		{	
				count3++;
				p_read=strstr(Message_ID.Message,read_sn);
				p_ID=strstr(Message_ID.Message,read_id);
				p_value=strstr(Message_ID.Message,value);
				p_text3=strstr(Message_ID.Message,text3_end);
				if(p_read!=NULL)													//读取FLASH
				{	
						ReadFlashData(ADDR_FLASH_SECTOR_2,flash_data,10);
						printf("TEXT3_SN:");
						for(n=0;n<SIZE(flash_data);n++)
						{
							printf("%X",flash_data[n]);
						}
						printf("\n");		
						ClearStr(Message_ID.Message);
				}	
				if(p_ID!=NULL)														//读取ID
				{				
						Get_CPU_ID(CPUID);	
						printf("TEXT3_ID:%x-%x-%x\n",CPUID[0],CPUID[1],CPUID[2]);
						ClearStr(Message_ID.Message);				
				}				
				if(p_value!=NULL)													//读取电压
				{
						temp_adc_value[0]=GET_ADC_AVERAGE(hadc1,ADC_CHANNEL_16,10);
						AD_Value[0]=(temp_adc_value[0]*3.3)/4096;
						temp_adc_value[1]=GET_ADC_AVERAGE(hadc1,ADC_CHANNEL_17,10);
						AD_Value[1]=(temp_adc_value[1]*3.3)/4096;
						printf("TEXT3_VALUE:%.1f\n",AD_Value[1]);
						ClearStr(Message_ID.Message);																															
				}				
	/*********************清空****************************/
					p_value=NULL;
					p_read=NULL;
					p_write=NULL;
					p_ID=NULL;
					for( k= 0; k < SIZE(Rarr); k++)
					{
							Rarr[k]=0;
					}
					tx_thread_sleep(50);	
	/*************************************************/
	/*************任务3结束*******************************/
					if(p_text3)
					{
					for( k= 0; k < SIZE(Rarr); k++)
						{
								Rarr[k]=0;
								ClearStr(Message_ID.Message);
						}
					
						Sec_flag=5;
						p_text3=NULL;
						tx_thread_resume(&AppData_ProcessingTaskTCB);//唤醒测试1
						tx_thread_suspend(&AppData_Text3TaskTCB);	//挂起测试3
					}
		}			
}
/*
*****************************************************************************
							数据处理任务（测试任务1）
******************************************************************************
*/


int y=0;
void App_Data_PrpcessingTask()
{
	uint32_t CPUID[3];
	uint16_t temp_adc_value[2];
	float AD_Value[2];
	char *p_write=NULL;
	char *p_read=NULL;
	char *p_ID=NULL;
	char *p_text1=NULL;
	char *p_value=NULL;
	char *p_bar=NULL;
	char w_progressbar[]="w_bar";
	char write_sn[]="write";
	char read_sn[]="read_sn";
	char read_id[]="read_id";
	char value[]="read_value";
	char text[]="text1_end";
	uint8_t i,j,n=0;

	while(1)
	{		
		
			count1++;
			ClearStr(Message_ID.Mcu_id);
			p_read=strstr(Message_ID.Message,read_sn);
			p_write=strstr(Message_ID.Message,write_sn);
			p_ID=strstr(Message_ID.Message,read_id);
			p_value=strstr(Message_ID.Message,value);
			p_text1=strstr(Message_ID.Message,text);
			p_bar=strstr(Message_ID.Message,w_progressbar);
			if(Sec_flag)
			{
					for( int k= 0; k < SIZE(Rarr); k++)
					{
							Rarr[k]=0;
							ClearStr(Message_ID.Message);
					}

					Sec_flag--;
					p_text1=NULL;
					p_value=NULL;
					p_ID=NULL;
					p_write=NULL;
					p_read=NULL;
			}
			if(p_write!=NULL&&Send_flag)							//写FLASH地址
			{	
				for(i=0;i<strlen(Message_ID.Message)-5;i++)
				{
					Message_ID.Mcu_id[j++]=Message_ID.Message[i+5];
				}	
				
				Send_flag=0;
				j=0;
				WriteFlashData(ADDR_FLASH_SECTOR_2,(uint8_t *)Message_ID.Mcu_id, SIZE(Message_ID.Mcu_id));
				ClearStr(Message_ID.Message);
			}
			if(p_read!=NULL)													//读取FLASH
			{	
				printf("--------TEXT1 START---------\n");
				ReadFlashData(ADDR_FLASH_SECTOR_2,flash_data,10);
				printf("CPU_SN:");
				for(n=0;n<SIZE(flash_data);n++)
				{
					printf("%X",flash_data[n]);
				}
				printf("\n");		
				ClearStr(Message_ID.Message);
			}	
			if(p_ID!=NULL)														//读取ID
			{				
				Get_CPU_ID(CPUID);	
				printf("CPU_ID:%x-%x-%x\n",CPUID[0],CPUID[1],CPUID[2]);
				ClearStr(Message_ID.Message);				
			}				
			if(p_value!=NULL)													//读取电压
			{
				temp_adc_value[0]=GET_ADC_AVERAGE(hadc1,ADC_CHANNEL_16,10);
				AD_Value[0]=temp_adc_value[0]*3.3/4096;
				temp_adc_value[1]=GET_ADC_AVERAGE(hadc1,ADC_CHANNEL_17,10);
				AD_Value[1]=temp_adc_value[1]*3.3/4096;
				printf("ADC1_16:%1.f\n",AD_Value[0]);
				printf("TEXT1_VALUE:%.1f\n",AD_Value[1]);
				ClearStr(Message_ID.Message);	
				tx_thread_sleep(100);
				
			}

			
/********判断测试1是否结束********/
			if(p_text1!=NULL)
			{
				led_flag=1;																//检测LED标志位
				printf("\n\n\n");
				printf("--------TEXT2 START---------\n");
				p_value=NULL;
				p_read=NULL;
				p_write=NULL;
				p_ID=NULL;
				for( int k= 0; k < SIZE(Rarr); k++)
				{
						Rarr[k]=0;
				}		
				ClearStr(Message_ID.Message);	
				tx_thread_resume(&AppLedRedTaskTCB);//唤醒测试2
				tx_thread_suspend(&AppData_ProcessingTaskTCB);//挂起测试1

			}
/*********************清空****************************/
				p_value=NULL;
				p_read=NULL;
				p_write=NULL;
				p_ID=NULL;
				for( int k= 0; k < SIZE(Rarr); k++)
				{
						Rarr[k]=0;
				}
				
/*************************************************/
				tx_thread_sleep(50);
	}			
}

/*
*****************************************************************************
							下位机接收任务
******************************************************************************
*/
void App_progressbar_Task()
{
	while(1)
	{
		if(Message_ID.Message[0]=='5'&&Message_ID.Message[1]=='A')
		{
			for(int y=0;y<strlen(Message_ID.Message)-2;y++)
			{
				progressbar_buff[progressbar_cnt++]=Message_ID.Message[y+2];
				if(progressbar_cnt>=999)
				{
					progressbar_cnt=0;
					break;
				}
			}
			ClearStr(Message_ID.Message);
		}
		if(Message_ID.Message[0]=='A'&&Message_ID.Message[1]=='5')
		{
			printf("写入的数据为：%s\n",	progressbar_buff);
			for( int k= 0; k < SIZE(Rarr); k++)
				{
						Rarr[k]=0;
				}
				
			ClearStr(Message_ID.Message);
		}
	tx_thread_sleep(50);
	}
}

/*
*****************************************************************************
											创建任务
******************************************************************************
*/

uint32_t MessageQueuesBuf1[100]; /* 定义消息队列缓冲1 */
void AppTaskStart()
{
//	_tx_thread_terminate(&AppLedTaskTCB);//终止起始任务
	_tx_thread_create(&AppLedTaskTCB,
										"LED BLUE TASK",
										App_Led_BlueTask,
										0,
										&AppLedTask[0],
										APP_LED_TASK_SIZE,
										20,
										20,
										0,
										TX_AUTO_START);
	_tx_thread_create(&AppLedRedTaskTCB,
										"TEXT2",
										App_Led_RedTask,
										0,	
										&AppLedRedTask[0],
										APP_LED_TASK_SIZE,
										10,
										10,
										0,
										TX_DONT_START);
	_tx_thread_create(&AppData_ProcessingTaskTCB,
										"Data Processing",
										App_Data_PrpcessingTask,
										0,
										&AppDataProcessingTask[0],
										APP_SRART_TASK_SIZE,
										6u,
										6u,
										0,
										TX_AUTO_START);

	_tx_thread_create(&AppData_Text3TaskTCB,
										"TEXT 3 ",
										App_TEXT3_Task,
										0,
										&AppData_Text3Task[0],
										APP_SRART_TASK_SIZE,
										11,
										11,
										0,
										TX_DONT_START);
	_tx_thread_create(&Appprogressbar_TCB,
										"Progressbar TASK",
										App_progressbar_Task,
										0,
										&AppprogressbarTask[0],
										APP_SRART_TASK_SIZE,
										5,
										5,
										0,
										TX_AUTO_START);

}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
Serial_Data message;
/*
*****************************************************************************
										串口中断回调函数
******************************************************************************
*/
 void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
 {
	 Send_flag=1;
	 uint16_t i,j=0;
	 for(i=0;i<SIZE(Rarr);i++)
	 {
			Message_ID.Message[j++]=Rarr[i];
	 }
		HAL_UARTEx_ReceiveToIdle_IT(&huart2,Rarr,20);
 }

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  (void)byte_pool;
	Message_ID.Message[0]=0;
	Message_ID.ReceiveID=0;
	Message_ID.SendID=0;
	_tx_thread_create(&AppStartTaskTCB,
										"APP START TASK",
										AppTaskStart,
										0,
										&AppStartTask[0],
										APP_SRART_TASK_SIZE,
										1,
										1,
										TX_NO_TIME_SLICE,
										TX_AUTO_START);
	/* 创建消息队列1 */
	tx_queue_create(&MessageQueues1, 
									"MessageFID", 
									1,                         /* 每次消息队列发送的数据大小，单位32bit，范围1-16 */
									(VOID *)MessageQueuesBuf1, 
									sizeof(MessageQueues1));   /* 消息队列大小，单位字节 */
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
