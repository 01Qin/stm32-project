/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "alldata.h"
#include "OLED.h"
#include "mcp4728.h"
#include "NTC.h"
#include "jsonParser.h"
#include "TCA9555.h"
#include "DAC7578.h"
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
uint32_t encoderCNT = 0;
uint16_t encoder = 0;

uint8_t i2cTxBuffer[16] = {0};
uint8_t i2cRxBuffer[16] = {0};

dacChannelConfig dacConfig1 = {0};
dacChannelConfig dacConfig2 = {0};

//kTester_type allData = {0};
const int16_t tempOffset[8] = {0}; // {3, -1, -4, 0 , 0, 3, -4, 10};		// MCP4728 is not so accurate, we need some offset correction, but DAC7578 is really cool!

uint16_t dummyCNT = 0;
uint8_t dummyDir = 0;

#define RX_BUFFER_SIZE 128
uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t rxWorkBuffer[RX_BUFFER_SIZE + 1];

#define TX_BUFFER_SIZE 2048
uint8_t txBuffer[TX_BUFFER_SIZE];

char response[2048] = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void print(const char *format, ...){


    va_list args;
    va_start(args, format);

    // Format string into tx_buffer safely
    int len = vsnprintf((char *)txBuffer, TX_BUFFER_SIZE, format, args);

    va_end(args);

    // Send over UART using DMA
    HAL_UART_Transmit_DMA(&huart2, (uint8_t*)txBuffer, len);
}

void setPwmOut(uint8_t pwm){
	  TIM1->CCR2 = (uint32_t)pwm;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	memcpy(rxWorkBuffer, &rxBuffer[0], Size);
	memset(rxBuffer, 0, sizeof(rxBuffer));
	rxWorkBuffer[Size + 1] = 0;					// Null-terminate for printf or other

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rxBuffer, RX_BUFFER_SIZE);

	jsonParse(rxWorkBuffer, (char*) &response, sizeof(response));

	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)response, strlen(response));

//	// For debug only
//	HAL_UART_Transmit_DMA(&huart2, (uint8_t*)rxWorkBuffer, Size);
}

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

  /* Reset of all peripherals, Initialises the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialise all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //Init Encoder
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_2);
  htim2.Instance->CNT = 0;

  //Init PWM measurements
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);   // main channel
  HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_2);   // indirect channel

  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);   // main channel
  HAL_TIM_IC_Start(&htim4, TIM_CHANNEL_2);   // indirect channel

  HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1);   // main channel
  HAL_TIM_IC_Start(&htim8, TIM_CHANNEL_2);   // indirect channel

  //Test PWM
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  setPwmOut(50);

  uint32_t timeTickNow = HAL_GetTick();

  /* USER CODE END 2 */

  /* Initialise LEDs */
  BSP_LED_Init(LED_GREEN);

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
////Find addresses
//  print("Scan slaves on i2c1:\r\n");
//  uint8_t dummy = 0;
//  HAL_StatusTypeDef retValue = 0;
//  for(int i=1; i<128; i++){
//	  retValue = HAL_I2C_Master_Transmit(&hi2c1, i<<1, &dummy, 0, 10);
//	  if(HAL_BUSY == retValue) print("0x%02X - Busy\r\n", i);
//	  if(HAL_OK == retValue) print("0x%02X - OK\r\n", i);
//  }
//
//  print("--------------------\r\nScan slaves on i2c3:\r\n");
//  retValue = 0;
//  for(int i=1; i<128; i++){
//	  retValue = HAL_I2C_Master_Transmit(&hi2c3, i<<1, &dummy, 0, 10);
//	  if(HAL_BUSY == retValue) print("0x%02X - Busy\r\n", i);
//	  if(HAL_OK == retValue) print("0x%02X - OK\r\n", i);
//  }
//  print("--------------------\r\n");

  //MCP4728 DAC Initialisation
//  MCP4728_Init(&hi2c1, dacConfig1);
//  dacConfig1.channelVref = 0x00;
//  MCP4728_Write_VRef_Select(&hi2c1, dacConfig1);
//
//  MCP4728_Init(&hi2c3, dacConfig2);
//  dacConfig2.channelVref = 0x00;
//  MCP4728_Write_VRef_Select(&hi2c3, dacConfig2);

  //TCA9555 GPIO-expander Initialisation
  TCA9555_All_Output_Init(&hi2c1, TCA9555_ADD_0);

  //Display Initialisation
  HAL_Delay(100);
  OLED_Init(Display);
  HAL_Delay(10);
  OLED_CLS(Display);
  HAL_Delay(10);
  OLED_P8x16Str(Display, 0, 4, "MaKE sOme FuN!;)");
  HAL_Delay(2000);
  OLED_CLS(Display);
  HAL_Delay(10);

  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rxBuffer, RX_BUFFER_SIZE);

  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  BSP_LED_Toggle(LED_GREEN);

	  // Check encoder / Set the value between 0 and 100
	  encoderCNT = htim2.Instance->CNT;
	  if(encoderCNT >= 6000) {
		  htim2.Instance->CNT = 0;
		  encoderCNT = 0;
	  }
	  if(encoderCNT > 220 && encoderCNT < 6000){
		  htim2.Instance->CNT = 220;
		  encoderCNT = 220;
	  }
	  encoder = encoderCNT;

//	  int tempEncoder = encoder - 50;
	  uint16_t tempPwm = encoder % 101;
	  setPwmOut(tempPwm);


	  // Collecting data
	  allData.dacChannelValues[0] =  ntc_to_adc(allData.tempValues[0]) + tempOffset[0];
	  allData.dacChannelValues[1] =  ntc_to_adc(allData.tempValues[1]) + tempOffset[1];
	  allData.dacChannelValues[2] =  ntc_to_adc(allData.tempValues[2]) + tempOffset[2];
	  allData.dacChannelValues[3] =  ntc_to_adc(allData.tempValues[3]) + tempOffset[3];
	  allData.dacChannelValues[4] =  ntc_to_adc(allData.tempValues[4]) + tempOffset[4];
	  allData.dacChannelValues[5] =  ntc_to_adc(allData.tempValues[5]) + tempOffset[5];
	  allData.dacChannelValues[6] =  ntc_to_adc(allData.tempValues[6]) + tempOffset[6];
	  allData.dacChannelValues[7] =  ntc_to_adc(allData.tempValues[7]) + tempOffset[7];

	  allData.pwmValues[0] = whatDutyIs(0);
	  allData.pwmValues[1] = whatDutyIs(1);
	  allData.pwmValues[2] = whatDutyIs(2);

	  allData.freqValues[0] = whatFrequencyIs(0);
	  allData.freqValues[1] = whatFrequencyIs(1);
	  allData.freqValues[2] = whatFrequencyIs(2);

	  // Format PWM data if it is needed -> If the data is too old, the input is a DC-like signal
	  timeTickNow = HAL_GetTick();
	  if((timeTickNow - whatLastCaptureIs(0)) > 100){
	        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);

	        if (state == GPIO_PIN_SET)
	        {
	        	allData.pwmValues[0] = 100;
	        	allData.freqValues[0] = 0;
	        }
	        else
	        {
	        	allData.pwmValues[0] = 0;
	        	allData.freqValues[0] = 0;
	        }

	  }
	  if((timeTickNow - whatLastCaptureIs(1)) > 100){
	        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);

	        if (state == GPIO_PIN_SET)
	        {
	        	allData.pwmValues[1] = 100;
	        	allData.freqValues[1] = 0;
	        }
	        else
	        {
	        	allData.pwmValues[1] = 0;
	        	allData.freqValues[1] = 0;
	        }

	  }
	  if((timeTickNow - whatLastCaptureIs(2)) > 100){
	        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11);

	        if (state == GPIO_PIN_SET)
	        {
	        	allData.pwmValues[2] = 100;
	        	allData.freqValues[2] = 0;
	        }
	        else
	        {
	        	allData.pwmValues[2] = 0;
	        	allData.freqValues[2] = 0;
	        }

	  }

	  // Update analog outputs
	  DAC7578_Write_AllChannel(&hi2c1, DAC7578_ADD_2, allData.dacChannelValues);

	  // Update digital outputs
	  TCA9555_Write_AllOutput(&hi2c1, TCA9555_ADD_0, allData.digitOutValues);

	  // Send to display
	  OLED_PrintValueI(Display, 86, 7, encoder);
	  OLED_PrintValueI(Display, 44, 7, tempPwm);
	  OLED_PrintValueI(Display, 1, 7, allData.dacChannelValues[0]);

	  OLED_P6x8Str(Display, 8, 0, "T1:"); OLED_PrintValueI3(Display, 26, 0, allData.tempValues[0]); OLED_P6x8Str(Display, 50, 0, "C");
	  OLED_P6x8Str(Display, 8, 1, "T2:"); OLED_PrintValueI3(Display, 26, 1, allData.tempValues[1]); OLED_P6x8Str(Display, 50, 1, "C");
	  OLED_P6x8Str(Display, 8, 2, "T3:"); OLED_PrintValueI3(Display, 26, 2, allData.tempValues[2]); OLED_P6x8Str(Display, 50, 2, "C");
	  OLED_P6x8Str(Display, 8, 3, "T4:"); OLED_PrintValueI3(Display, 26, 3, allData.tempValues[3]); OLED_P6x8Str(Display, 50, 3, "C");

	  OLED_P6x8Str(Display, 72, 0, "T5:"); OLED_PrintValueI3(Display, 90, 0, allData.tempValues[4]); OLED_P6x8Str(Display, 114, 0, "C");
	  OLED_P6x8Str(Display, 72, 1, "T6:"); OLED_PrintValueI3(Display, 90, 1, allData.tempValues[5]); OLED_P6x8Str(Display, 114, 1, "C");
	  OLED_P6x8Str(Display, 72, 2, "T7:"); OLED_PrintValueI3(Display, 90, 2, allData.tempValues[6]); OLED_P6x8Str(Display, 114, 2, "C");
	  OLED_P6x8Str(Display, 72, 3, "T8:"); OLED_PrintValueI3(Display, 90, 3, allData.tempValues[7]); OLED_P6x8Str(Display, 114, 3, "C");

	  #define SHIFT 8
	  OLED_PrintValueH1(Display, 0 + SHIFT, 4, allData.digitOutValues[0]);
	  OLED_PrintValueH1(Display, 6 + SHIFT, 4, allData.digitOutValues[1]);
	  OLED_PrintValueH1(Display, 12 + SHIFT, 4, allData.digitOutValues[2]);
	  OLED_PrintValueH1(Display, 18 + SHIFT, 4, allData.digitOutValues[3]);

	  OLED_PrintValueH1(Display, 27 + SHIFT, 4, allData.digitOutValues[4]);
	  OLED_PrintValueH1(Display, 33 + SHIFT, 4, allData.digitOutValues[5]);
	  OLED_PrintValueH1(Display, 39 + SHIFT, 4, allData.digitOutValues[6]);
	  OLED_PrintValueH1(Display, 45 + SHIFT, 4, allData.digitOutValues[7]);

	  OLED_PrintValueH1(Display, 54 + SHIFT, 4, allData.digitOutValues[8]);
	  OLED_PrintValueH1(Display, 60 + SHIFT, 4, allData.digitOutValues[9]);
	  OLED_PrintValueH1(Display, 66 + SHIFT, 4, allData.digitOutValues[10]);
	  OLED_PrintValueH1(Display, 72 + SHIFT, 4, allData.digitOutValues[11]);

	  OLED_PrintValueH1(Display, 81 + SHIFT, 4, allData.digitOutValues[12]);
	  OLED_PrintValueH1(Display, 87 + SHIFT, 4, allData.digitOutValues[13]);
	  OLED_PrintValueH1(Display, 93 + SHIFT, 4, allData.digitOutValues[14]);
	  OLED_PrintValueH1(Display, 99 + SHIFT, 4, allData.digitOutValues[15]);

//	  // For debug only
//	  OLED_P6x8Str(Display, 8, 4, (char *)rxWorkBuffer);

	  OLED_P6x8Str(Display, 8, 5, "PWM(%):"); OLED_PrintValueUC(Display, 50, 5, allData.pwmValues[0]);
	  	  	  	  	  	  	  	  	  	  	  OLED_PrintValueUC(Display, 74, 5, allData.pwmValues[1]);
	  	  	  	  	  	  	  	  	  	  	  OLED_PrintValueUC(Display, 98, 5, allData.pwmValues[2]);

	  OLED_P6x8Str(Display, 8, 6, "F(kHz):"); OLED_PrintValueUC(Display, 50, 6, (allData.freqValues[0]+500)/1000);
											  OLED_PrintValueUC(Display, 74, 6, (allData.freqValues[1]+500)/1000);
											  OLED_PrintValueUC(Display, 98, 6, (allData.freqValues[2]+500)/1000);


//	  HAL_Delay(10);
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/**
  * @}
  */

/**
  * @}
  */

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
#ifdef USE_FULL_ASSERT
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
