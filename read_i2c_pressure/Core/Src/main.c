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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BMP280_ADDR 0x77 << 1

// Oversampling definitions
#define OSRS_OFF    	0x00
#define OSRS_1      	0x01
#define OSRS_2      	0x02
#define OSRS_4      	0x03
#define OSRS_8      	0x04
#define OSRS_16     	0x05

// MODE Definitions
#define MODE_SLEEP      0x00
#define MODE_FORCED     0x01
#define MODE_NORMAL     0x03

// IIR Filter Coefficients
#define IIR_OFF     	0x00
#define IIR_2       	0x01
#define IIR_4       	0x02
#define IIR_8       	0x03
#define IIR_16      	0x04

// Standby time
#define T_SB_05     0x00
#define T_SB_625     0x01
#define T_SB_125     0x02
#define T_SB_250     0x03
#define T_SB_500     0x04
#define T_SB_1000     0x05
#define T_SB_2000   0x06
#define T_SB_4000   0x07

// REGISTERS DEFINITIONS
#define ID_REG  		0xD0
#define RESET_REG		0xE0
#define STATUS_REG      0xF3
#define CTRL_MEAS_REG   0xF4
#define CONFIG_REG      0xF5
#define PRESS_MSB_REG   0xF7
#define TEMP_MSB_REG 	0xFA
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;

ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c2;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim15;

/* USER CODE BEGIN PV */
// static const uint8_t HUM_ADDR = 0x44 << 1;
// static const uint8_t LIS3DH_ADDR = 0x18 << 1;
// static const uint8_t DISPLAY_ADDR = 0x3C << 1;
// static const uint8_t TEMP_ADDR = 0x48 << 1;
// static const uint8_t BMP280_ADDR 0x77 << 1
// static const uint8_t AHT20_ADDR 0x38 << 1

int32_t p_raw, t_raw;
float Pressure, Temp;
int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
uint16_t dig_T1, dig_P1;
int32_t t_fine;
uint8_t chip_id;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM15_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

static int trim_read();
static int BMP280_read_raw();
//static uint32_t BMP280_compensate_P_int64(int32_t adc_P);
static uint32_t BMP280_compensate_P_int32(int32_t adc_P);
static int32_t BMP280_compensate_T(int32_t adc_T);
void BMP280_measure(float *pressure, float *temp);
int BMP280_config(uint8_t osrs_t, uint8_t osrs_p, uint8_t mode, uint8_t t_sb, uint8_t filter);
void BMP280_wakeup();
//void BMP280_PrintCalibration(void);
//void BMP280_CheckID(void);

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
  MX_I2C2_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  MX_TIM15_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Initialize leds */
  BSP_LED_Init(LED_GREEN);

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN BSP */

  /* -- Sample board code to send message over COM1 port ---- */
  printf("Welcome to STM32 world !\n\r");

  /* -- Sample board code to switch on leds ---- */
  BSP_LED_On(LED_GREEN);
  BMP280_config(OSRS_2, OSRS_16, MODE_NORMAL, T_SB_05, IIR_16);
  /* USER CODE END BSP */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


//    uint8_t bufferI2c[10] = {0};

//  BMP280_CheckID();
//  trim_read();
//  BMP280_PrintCalibration();

  while (1)
  {

/* -- Sample board code to toggle leds ---- */
       BSP_LED_Toggle(LED_GREEN);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
       BMP280_measure(&Pressure, &Temp);

       printf("Temperature: %.2f C,\r\nPressure: %.2f Pa\r\n", Temp, Pressure);
       HAL_Delay(30000); // print every 30sec
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  /* I2C2_EV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
  /* I2C2_ER_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.GainCompensation = 0;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T15_TRGO;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x40B285C2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 0;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 65535;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */



static int trim_read(){
	uint8_t trimdata[25] ={0};

	// read calibration registers 0x88 - 0x9F
	if (HAL_I2C_Mem_Read(&hi2c2, BMP280_ADDR, 0x88, 1, trimdata, 25, HAL_MAX_DELAY) != HAL_OK)
	        return 1;

	// temperature coefficients
	dig_T1 = (uint16_t)(trimdata[1] << 8 | trimdata[0]);
	dig_T2 = (int16_t)(trimdata[3] << 8 | trimdata[2]);
	dig_T3 = (int16_t)(trimdata[5] << 8 | trimdata[4]);

	// Pressure coefficients
	dig_P1 = (uint16_t)(trimdata[7] << 8 | trimdata[6]);
	dig_P2 = (int16_t)(trimdata[9] << 8 | trimdata[8]);
	dig_P3 = (int16_t)(trimdata[11] << 8 | trimdata[10]);
	dig_P4 = (int16_t)(trimdata[13] << 8 | trimdata[12]);
	dig_P5 = (int16_t)(trimdata[15] << 8 | trimdata[14]);
	dig_P6 = (int16_t)(trimdata[17] << 8 | trimdata[16]);
	dig_P7 = (int16_t)(trimdata[19] << 8 | trimdata[18]);
	dig_P8 = (int16_t)(trimdata[21] << 8 | trimdata[20]);
	dig_P9 = (int16_t)(trimdata[23] << 8 | trimdata[22]);

	return 0;
}

int BMP280_config (uint8_t osrs_t, uint8_t osrs_p, uint8_t mode, uint8_t t_sb, uint8_t filter){

	trim_read();

	uint8_t datawr;
	uint8_t datacheck = 0;

	// reset the device
	datawr = 0xB6;
	if (HAL_I2C_Mem_Write(&hi2c2, BMP280_ADDR, RESET_REG, 1, &datawr, 1, HAL_MAX_DELAY) != HAL_OK) {
		return 1;
	}
	HAL_Delay(100);

	// write the standby time and iir filter coefficient to 0xF5
	datawr = (t_sb << 5) | (filter << 2);
	HAL_I2C_Mem_Read(&hi2c2, BMP280_ADDR, CONFIG_REG, 1, &datacheck, 1, HAL_MAX_DELAY);
	HAL_Delay(100);

	// write the pressure  and temp oversampling with mode to 0xF4
	datacheck = (osrs_t << 5) | (osrs_p << 2) | mode;
	HAL_I2C_Mem_Write(&hi2c2, BMP280_ADDR, CTRL_MEAS_REG, 1, &datacheck, 1, HAL_MAX_DELAY);
	return 0;
}


static int BMP280_read_raw() {

	uint8_t buf[6];

	// check the chip id
	HAL_I2C_Mem_Read(&hi2c2, BMP280_ADDR, ID_REG, 1, &chip_id, 1, HAL_MAX_DELAY);
		if (chip_id == 0x58){
		// calculate the raw data
		HAL_I2C_Mem_Read(&hi2c2, BMP280_ADDR, PRESS_MSB_REG, 1, buf, 6, HAL_MAX_DELAY);
		p_raw = (buf[0]<<12)|(buf[1]<<4)|(buf[2]>>4);
		t_raw = (buf[3]<<12)|(buf[4]<<4)|(buf[5]>>4);
		return 0;
		}
		else return -1;
}


//uint32_t BMP280_compensate_P_int64(int32_t adc_P)
//{
//	int64_t var1, var2, p;
//	var1 = ((int64_t)t_fine) - 128000;
//	var2 = var1 * var1 * (int64_t)dig_P6;
//	var2 = var2 + ((var1*(int64_t)dig_P5)<<17);
//	var2 = var2 + (((int64_t)dig_P4)<<35);
//	var1 = ((var1 * var1 * (int64_t)dig_P3)>>8) + ((var1 * (int64_t)dig_P2)<<12);
//	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)dig_P1)>>33;
//
//	if (var1 == 0)
//	{
//		return 0; // avoid exception caused by division by zero
//	}
//
//	p = 1048576 - adc_P;
//
//	p = (((p<<31) - var2) * 3125) / var1;
//	var1 = (((int64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
//	var2 = (((int64_t)dig_P8) * p) >> 19;
//
//	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
//
//	return (uint32_t)p;
//}

static uint32_t BMP280_compensate_P_int32(int32_t adc_P)
{
	int32_t var1, var2;
	uint32_t p;
	var1 = (((int32_t)t_fine)>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)dig_P6);
	var2 = var2 + ((var1*((int32_t)dig_P5))<<1);
	var2 = (var2>>2)+(((int32_t)dig_P4)<<16);
	var1 = (((dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)dig_P2) *var1)>>1))>>18;
	var1 =((((32768+var1))*((int32_t)dig_P1))>>15);
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p = (((uint32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;
	if (p < 0x80000000)
	{
		p = (p << 1) / ((uint32_t)var1);
	}
	else
	{
		p = (p / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)dig_P8))>>13;
	p = (uint32_t)((int32_t)p + ((var1 + var2 + dig_P7) >> 4));
	return p;
}


int32_t BMP280_compensate_T(int32_t adc_T)
{
    int32_t var1, var2;
    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
              ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
            ((int32_t)dig_T3)) >> 14;

    t_fine = var1 + var2;
    return (t_fine * 5 + 128) >> 8;   // temperature in °C * 100
}

void BMP280_wakeup() {
	uint8_t datawr = 0;
	// read the register
	HAL_I2C_Mem_Read(&hi2c2, BMP280_ADDR, CTRL_MEAS_REG, 1, &datawr, 1, HAL_MAX_DELAY);

	// modify the data with the forced mode
	datawr = datawr | MODE_FORCED;

	// write the new data to the register
	HAL_I2C_Mem_Write(&hi2c2, BMP280_ADDR, CTRL_MEAS_REG, 1, &datawr, 1, HAL_MAX_DELAY);
	HAL_Delay(100);
}


void BMP280_measure(float *pressure, float *temp)
{
	if (BMP280_read_raw() == 0){

	*temp = BMP280_compensate_T(t_raw) / 100.0f;
    *pressure = (BMP280_compensate_P_int32(p_raw));
	} else {
		*temp = 0;
		*pressure = 0;
	}

}

//void BMP280_PrintCalibration(void)
//{
//    printf("dig_T1 = %u\r\n", dig_T1);
//    printf("dig_T2 = %d\r\n", dig_T2);
//    printf("dig_T3 = %d\r\n", dig_T3);
//
//    printf("dig_P1 = %u\r\n", dig_P1);
//    printf("dig_P2 = %d\r\n", dig_P2);
//    printf("dig_P3 = %d\r\n", dig_P3);
//    printf("dig_P4 = %d\r\n", dig_P4);
//    printf("dig_P5 = %d\r\n", dig_P5);
//    printf("dig_P6 = %d\r\n", dig_P6);
//    printf("dig_P7 = %d\r\n", dig_P7);
//    printf("dig_P8 = %d\r\n", dig_P8);
//    printf("dig_P9 = %d\r\n", dig_P9);
//}
//
//void BMP280_CheckID(void)
//{
//    uint8_t id = 0;
//
//    if (HAL_I2C_Mem_Read(&hi2c2, BMP280_ADDR, 0xD0, 1, &id, 1, HAL_MAX_DELAY) == HAL_OK)
//        printf("BMP280 ID register = 0x%02X\r\n", id);
//    else
//        printf("I2C read failed at address 0x%02X\r\n", BMP280_ADDR);
//}

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
