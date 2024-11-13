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
#include "sdio.h"

//FatFs
#include "diskio.h"
#include "ff.h"
FATFS *fs[FF_VOLUMES];
FRESULT res_sd;
/*
*****************************************************************************
											串口重定向
******************************************************************************
*/
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


#define NVIC_APP_FLASH ((uint32_t)0x08010000) //flash起始地址



/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

void sys_msr_msp(uint32_t addr)
{
    __set_MSP(addr);    /* 设置栈顶地址 */
}


/*
*****************************************************************************
								控制蓝色LED任务
******************************************************************************
*/
#define BLOCK_SIZE   512                            //一个块的字节数
#define NUMBERS_PER_CHUNK 512  						//多少个数据存一次
#define INT_SIZE 1             						//一个数值占几个字节
#define BUFFER_SIZE (NUMBERS_PER_CHUNK * INT_SIZE)  //存一次sd卡的数组的大小（其实就是512）
 
uint8_t buffer_TX[NUMBERS_PER_CHUNK];  // 用于暂时存储需要发送到SD卡的数据
uint8_t buffer_RX[NUMBERS_PER_CHUNK];  // 用于接收从SD卡中读取来的数据	 

void App_Led_BlueTask()
{
//	uint16_t blockNum = 0;               // 起始块号
//	 
///*--------------------------SD卡写测试----------------------------------*/
//for (int chunk = 0; chunk < 2; chunk++) 
//{
//    // 填充缓冲区
//    for (int i = 0; i < NUMBERS_PER_CHUNK; i++)
//    {
//        buffer_TX[i] = i/2;
////		printf("%d ",buffer_TX[i]); 
//    }
// 
//    // 计算写入的块数量
//    uint32_t blocksToWrite = BUFFER_SIZE / BLOCK_SIZE; //（512*1=512）
//			 
//			
//    // 将数据写入SD卡
//    if (HAL_SD_WriteBlocks(&hsd, (uint8_t *)buffer_TX, blockNum, 1, 10000) == HAL_OK) 
//	{
//	    while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER);  //处于传输状态退出
//        printf("\r\nWriteBlocks Successfully\r\n");
//    }
//	else
//    {
//        printf("WriteBlocks Failed\n");
//    }
//        blockNum += 1;  // 更新块号以写入下一个段
//}
////	res_sd = f_mount(fs[0],"1:",1);
//	printf("res_sd=%d",res_sd);
	while(1)
	{
		tx_thread_sleep(1000);
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);

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
