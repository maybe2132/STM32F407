/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "12864.h"
#include "font.h"
#include "flash.h"
#include "lvgl.h" 
#include "lv_port_disp_template.h"
#define DATA_BU_SIZE 1024  // 用户定义的缓冲区大小
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t rs485_buff[256];
#define RXBUFFERSIZE                 100
uint8_t rx_len;

extern uint8_t Chinses_16[];
extern uint8_t Chinses_16_2[];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int len;
	void str_chinese(int x,int y,char str[])
	{
		int n=0;
		char data_temp[50];
		strcpy(data_temp,str);
		const char s[2] = "-";
		char *token;
		// 获取第一个子字符串
		token = strtok(str, s);
		// 继续获取其他的子字符串
		while(token != NULL) {
		printf("%s\n", token);
		DZ_Show_GB2312(x+16*n,y,token);
		n++;
		if(x+16*n>112)
		{
			OLED_Set_Pos(0,y+=2);
			n=0;
		}
		token = strtok(NULL, s);
		}
	}
	

void lv_demo_button(void)
{
    lv_style_t style;

    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_white());
    lv_style_set_outline_width(&style, 2);

    lv_obj_set_style_bg_color(lv_scr_act(),   lv_color_white(), 0);

    lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 20, 20);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_style(btn,&style,LV_STATE_DEFAULT);

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_set_style_text_color(label,lv_color_black(),0);
    lv_obj_center(label);
}

lv_color_t c;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
/* USER CODE BEGIN 1 */
/* USER CODE END 1 */


  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
		if(HAL_UART_Receive_IT(&huart2,rs485_buff,1)!=HAL_OK);//接收中断使能
		//使能定时器中断
		    HAL_TIM_Base_Start_IT(&htim6);
		HAL_Delay(100);
		OLED_Init();
		
		OLED_NewFrame();
		OLED_ShowFrame();
		OLED_Set_Pos(64,4);
		OLED_Test();
//		lv_init();
//		lv_port_disp_init();
		HAL_Delay(100);
#if	0 //测试程序
		str_chinese(0,0,"上-变-频-器-辨-识-磁-极-位-置-出-错-网-络-芯-片");
		str_chinese(0,2,"驱-动-机-输-入-缺-相");
		str_chinese(0,4,"欠-变-频-器-电-压-报-警");
		str_chinese(0,6,"运-行-反-馈");
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		HAL_Delay(1000);

//		lv_demo_button();
    /* USER CODE BEGIN 3 */
//		for(uint8_t i=0;i<64;i++)
//		{
//				OLED_NewFrame();
//				OLED_DrawPoint(i*2,i,1);
//				OLED_ShowFrame();
//		}




		/* Infinite loop */
/* USER CODE BEGIN WHILE */
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart== &huart2)
		HAL_UART_Receive_IT(&huart2,rs485_buff,RXBUFFERSIZE);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		HAL_UART_Receive_IT(&huart2,rs485_buff,1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
			HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
//			lv_tick_inc(1);
//			lv_task_handler();
}

 #ifdef __GNUC__
     #define PUTCHAR_PROTOTYPE int _io_putchar(int ch)
 #else
     #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
 #endif /* __GNUC__*/
 
 /******************************************************************
     *@brief  Retargets the C library printf  function to the USART.
     *@param  None
     *@retval None
 ******************************************************************/
 PUTCHAR_PROTOTYPE
 {	
     HAL_UART_Transmit(&huart2, (uint8_t *)&ch,1,0xFFFF);
     return ch;
 }
 


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
