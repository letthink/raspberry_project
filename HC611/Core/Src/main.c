/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
float ChangeDistance(unsigned long); //距离计算函数（用户函数）
int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t*)&ch,1,0xffff);
	return ch;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
    uint32_t capture_Buf[3] = {0};   //存放计数值
    uint8_t capture_Cnt = 0;    //状态标志位
    uint32_t high_time;   //高电平时间
		
		unsigned long TIM3_Count = 0;
		unsigned Count = 0;
		float Distance;
		
		uint8_t RxData[1];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t buf[]="Hi this one is first important one\r\n";
	uint16_t len_buf =sizeof(buf); 
	uint8_t CapIndex = 0;
	uint8_t CapValReady = 1;
	float USData = 0;
	uint16_t CapVal = 0;
	uint8_t TxData[12] = "";
	uint8_t ts1[] = "test1";
	double dis = 10;
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	//HAL_UART_Transmit(&huart1,"Hi this one is first important one\r\n",len_buf,100);//注意长度问题，对于长度尽可能保证不长也不少的情况
	HAL_GPIO_WritePin(BuzzerB7_GPIO_Port,BuzzerB7_Pin,GPIO_PIN_SET);
	HAL_Delay(150);
	HAL_GPIO_WritePin(BuzzerB7_GPIO_Port,BuzzerB7_Pin,GPIO_PIN_RESET);
	//HAL_UART_Transmit(&huart1,"\r\n",8,100);
	//HAL_UART_Transmit(&huart1,"0x0D，0x0A",6,100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_GPIO_TogglePin(LEDB5_GPIO_Port,LEDB5_Pin);
		HAL_Delay(100);
		HAL_UART_Receive_IT(&huart1,(uint8_t*)RxData,1);
		/*
		if(CapValReady == 1)
		{
			HAL_UART_Transmit(&huart1,ts1,sizeof(ts1),100);//注意长度问题，对于长度尽可能保证不长也不少的情况
			//25.5cm-350cm
			USData = CapVal * 34000/1000000/2;//声速34000cm/s TIM3Freq：1000000Hz
			TxData[0] = USData;
			if(USData <= 350 && USData >= 25.5)
			{
				HAL_UART_Transmit(&huart1,TxData,sizeof(USData),100);
				HAL_UART_Transmit(&huart1,ts1,sizeof(ts1),100);//注意长度问题，对于长度尽可能保证不长也不少的情况
			}
			__HAL_TIM_DISABLE(&htim3);
			HAL_Delay(500);
			__HAL_TIM_SET_COUNTER(&htim3,0);//设置定时器3计数值为0
			__HAL_TIM_ENABLE(&htim3);
			CapValReady = 0;
			USTrig();
			HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);//定时器3通道1输入捕获使能
			HAL_UART_Transmit(&huart1,"test3",sizeof(ts1),100);//注意长度问题，对于长度尽可能保证不长也不少的情况
		}
			*/
			
			/*switch(capture_Cnt){
				case 0:
						capture_Cnt++;
					__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_BOTHEDGE);
					HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
					break;
				case 3:
					high_time = capture_Buf[1]-capture_Buf[0];
					HAL_UART_Transmit(&huart1,(uint8_t*)high_time,1,0xffff);
				
				HAL_Delay(1000);
				capture_Cnt = 0;
				break;
				
			}*/
			
			HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_SET);
			HAL_Delay(20/1000);
			HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_RESET);
			
			TIM3_Count = 0;
			
			while(HAL_GPIO_ReadPin(ECHO_GPIO_Port,ECHO_Pin)==0);
			HAL_TIM_Base_Start_IT(&htim3);
			
			while(HAL_GPIO_ReadPin(ECHO_GPIO_Port,ECHO_Pin)==1);
			HAL_TIM_Base_Stop_IT(&htim3);
			
			Count = TIM3_Count;
			
			Distance = ChangeDistance(Count);
			
			//通过stm距离，考虑到数据的丢失，选择通过32直接判断
			/*
			if(Distance < 45000 && Distance > 25)
			{	
				printf("%.2fcm\n",Distance/10);
			}
			else if(Distance >= 45000)
			{
				printf("Distance too far\n");
			}
			else
			{
				printf("Distance too close\n");
			}
			
			if(Distance < 800)
			{
				HAL_GPIO_WritePin(LEDE7_GPIO_Port,LEDE7_Pin,GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(LEDE7_GPIO_Port,LEDE7_Pin,GPIO_PIN_SET);
			}
			*/
			
			//距离的判断标志，简化信息传递
			/*
			if(Distance < 800)
			{
				printf("%s","01");
				printf("\r");
				HAL_GPIO_WritePin(LEDE7_GPIO_Port,LEDE7_Pin,GPIO_PIN_RESET);
				HAL_Delay(100); //发送过于频繁，树莓派来不及接收
			}
			else
			{
				printf("%s","02");
				HAL_GPIO_WritePin(LEDE7_GPIO_Port,LEDE7_Pin,GPIO_PIN_SET);
				HAL_Delay(500);
			}
			*/
			HAL_Delay(900); //900ms是目前测试，不会导致数据超发的（重叠的周期）最短周期
			printf("%f",Distance/10.0); //或者对数据长度进行限制，再在树莓派上通过算法摘取合适的数据计入，需要纠错功能
		
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	
	if(TIM1 == htim ->Instance)
	{
	
		switch(capture_Cnt){
			case 1:
				capture_Buf[0] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);
				capture_Cnt++;
				break;
			case 2:
				capture_Buf[1] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);
				HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1);
			  capture_Cnt++;
		}
		
	}	
	
	
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	
	if(TIM5 == htim->Instance)
	{
		switch(capture_Cnt){
			case 1:
				capture_Buf[0] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//获取当前的捕获值.
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
				capture_Cnt++;
				break;
			case 2:
				capture_Buf[1] = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//获取当前的捕获值.
				HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1); //停止捕获   或者: __HAL_TIM_DISABLE(&htim5);
				capture_Cnt++;    
		}
	
	}
	
}
*/

float ChangeDistance(unsigned long Count)
{

	float Distance;
	
	Distance = Count*340*2/2000.0;
	
	return Distance;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef*huart)
{
	UNUSED(huart);
	if(RxData[0] =='1')
	{
		HAL_GPIO_WritePin(BuzzerB7_GPIO_Port,BuzzerB7_Pin,GPIO_PIN_SET);
	}
	else if(RxData[0] == '2')
	{
		HAL_GPIO_WritePin(BuzzerB7_GPIO_Port,BuzzerB7_Pin,GPIO_PIN_RESET);
	}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
