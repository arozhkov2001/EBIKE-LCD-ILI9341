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
#include "adc.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include <stdio.h>
#include "snow_tiger.h"
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
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef DateToUpdate = {0};
uint16_t adc = 0;
uint16_t j = 255;
uint16_t i;
uint16_t percent = 0;
uint16_t tempPercent;
uint16_t tempClock;
char trans_str[64] = {0,};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int16_t map (int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_SPI2_Init();
  MX_ADC1_Init();
  /* Run the ADC calibration */
    if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    {
      /* Calibration Error */
      Error_Handler();
    }
  /* USER CODE BEGIN 2 */
  ILI9341_Init();//initial driver setup to drive ili9341
  ILI9341_Fill_Screen(BLACK);
  		//ILI9341_Draw_Image((const char*)lcd_tft, SCREEN_HORIZONTAL_1);
  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
  		ILI9341_Draw_Text("20:85", 50, 80, WHITE, 3, BLUE);
  		ILI9341_DrawText("20:85", BIGFONT, 50, 100, WHITE, BLUE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
clock();
checkBattery();
ILI9341_Draw_Horizontal_Line(0,24,320,CYAN);
ILI9341_Draw_Horizontal_Line(0,25,320,CYAN);


}


//ILI9341_Draw_Text(j, 50, 60, WHITE, 1, BLACK);


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int16_t map (int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
  // if input is smaller/bigger than expected return the min/max out ranges value
  if (x < in_min)
    return out_min;
  else if (x > in_max)
    return out_max;

  // map the input to the output range.
  // round up if mapping bigger ranges to smaller ranges
  else  if ((in_max - in_min) > (out_max - out_min))
    return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
  // round down if mapping smaller ranges to bigger ranges
  else
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void checkBattery()
{
	HAL_ADC_Start(&hadc1); // запускаем преобразование сигнала АЦП
	          HAL_ADC_PollForConversion(&hadc1, 100); // ожидаем окончания преобразования
	          adc = HAL_ADC_GetValue(&hadc1); // читаем полученное значение в переменную adc
	          //HAL_ADC_Stop(&hadc1); // останавливаем АЦП
	          j = map(adc, 0, 4096, 256, 290);
	          percent = map(adc, 0, 4096, 0, 100);
	         if(j<=273 && j>=260){i=ORANGE;};
	         if(j>273){i= GREEN;}
	         if(j<260){i= RED;}
	snprintf(trans_str, 63, "%d\n", percent);
	ILI9341_Draw_Hollow_Rectangle_Coord(252, 4, 291, 18, WHITE);
			ILI9341_Draw_Hollow_Rectangle_Coord(253, 5, 290, 17, WHITE);
			ILI9341_Draw_Filled_Rectangle_Coord(292, 8, 295, 15, WHITE);
	if(tempPercent!=percent){
		ILI9341_Draw_Filled_Rectangle_Coord(254, 6, 290, 17, i);
		ILI9341_Draw_Filled_Rectangle_Coord(j, 17, 290, 6, BLACK);
		tempPercent=percent;
		ILI9341_DrawText(trans_str, FONT3, 216, 5, WHITE, BLACK);
		ILI9341_DrawText("%", FONT3, 237, 5, WHITE, BLACK);
	}
}
void clock()
{
	 HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BCD
	 if(sTime.Minutes<10){
		 snprintf(trans_str, 62, "%d:0%d:%d\n", sTime.Hours, sTime.Minutes, sTime.Seconds);
	 }
	 if(sTime.Seconds<10){
		 snprintf(trans_str, 62, "%d:%d:0%d\n", sTime.Hours, sTime.Minutes, sTime.Seconds);
		 }

	 if(sTime.Hours<10){
		 snprintf(trans_str, 62, "0%d:%d:%d\n", sTime.Hours, sTime.Minutes, sTime.Seconds);
			 }

	 if((sTime.Hours<10)&&(sTime.Minutes<10)){
	 		 snprintf(trans_str, 62, "0%d:0%d:%d\n", sTime.Hours, sTime.Minutes, sTime.Seconds);
	 			 }
		         //snprintf(trans_str, 62, "%d:%d:%d\n", sTime.Hours, sTime.Minutes, sTime.Seconds);
		         //ILI9341_DrawText(trans_str, 10, 10, WHITE, 2, BLACK);
		         if(tempClock!=sTime.Seconds){
		        	 tempClock=sTime.Seconds;
		        	 ILI9341_DrawText(trans_str, FONT4, 0, 0, WHITE, BLACK);}
		         else {

		         }
		        //  HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
		       //  snprintf(trans_str, 63, "Date %d-%d-20%d\n", DateToUpdate.Date, DateToUpdate.Month, DateToUpdate.Year);
		       //  ILI9341_Draw_Text(trans_str, 10, 30, WHITE, 2, BLACK);
		         //HAL_Delay(1000);
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
