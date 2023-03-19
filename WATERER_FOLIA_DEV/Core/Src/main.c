/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "../../OLED2/fonts.h"
#include "../../OLED2/ssd1306.h"
#include "eeprom.h"
#include "ds18b20.h"
#include "encoder.h"
#include "menu.h"
#include "higrometer.h"
#include "cfg.h"
#include "waterer_math.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SEPARATOR '^'
#define ZERO_SING '@'
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

char buf[30];
uint16_t Ch1, Ch2, raw1, raw2;
float ch1Volt, ch2Volt;
uint16_t softTimer1, softTimer2;
uint16_t eepromValue, encoVal = 100;
int16_t tmpC;
uint32_t addres;
uint8_t menuOk;

ds18b20_T ds1;
oneWire_t oneWire1;
Encoder_T enco;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void EncoINC(void);
void EncoDEC(void);
void TogLed(void);
void Menu_In(void);
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
	HAL_FLASH_Unlock();
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_ADC1_Init();
	MX_TIM3_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	/* USER CODE BEGIN 2 */

	MX_I2C1_Init();
	SSD1306_Init();
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start(&htim2);
	Encoder_Init(&enco, ENCO_BTN_GPIO_Port, ENCO_BTN_Pin, &htim3,
	TIM_CHANNEL_ALL, &softTimer2);
	HigroInit(&hadc1);

	Cfg_Init();

	display.ch1LowNew = GetCh1_LowCalibVal();
	display.ch1HighNew = GetCh1_HighCalibVal();
	display.ch2LowNew = GetCh2_LowCalibVal();
	display.ch2HighNew = GetCh2_HighCalibVal();
	display.ch1TooDry = GetCh1_TooDryVal();
	display.ch2TooDry = GetCh2_TooDryVal();
	display.displayTime = GetDisp_TimeONOFF();

	wire_init(&oneWire1, &htim2, DS_GPIO_Port, DS_Pin);
	ds18b20_init(&ds1, &oneWire1);
	ds18b20_read_address(&ds1);
	menuOk = 0;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		HigroMeasure();
		Encoder_Event(&enco);

		raw1 = Get_Data_CH1();
		raw2 = Get_Data_CH2();

		Ch1 = Calibration(GetCh1_LowCalibVal(), GetCh1_HighCalibVal(), raw1);
		Ch2 = Calibration(GetCh2_LowCalibVal(), GetCh2_HighCalibVal(), raw2);

		if (!softTimer1)
		{
			/*MENU SCREEN DISPLAY*/
			if (menuOk & (1 << 0))
			{

				display.ch1Low = Ch1;
				display.ch2Low = Ch2;
				display.ch1rawVal = raw1;
				display.ch2rawVal = raw2;
				Menu_Refresh();

				if (!(menuOk & (1 << 1)))
				{
					EncoderRegisterCallback(&enco, Menu_Enter, Menu_Next,
							Menu_Prev);
					menuOk |= (1 << 1);
					menuOk &= ~(1 << 2);
				}

			}
			/*MAIN SCREEN DISPLAY*/
			else if (!(menuOk & (1 << 0)))
			{

				SSD1306_GotoXY(0, 0);
				sprintf_v5(buf, "Temp:^C  ", sizeof(buf), 0, tmpC);
				SSD1306_Puts(buf, &Font_7x10, 1);

				SSD1306_GotoXY(0, 10);
				sprintf_v5(buf, "CH0:^% ,CH1:^%      ", sizeof(buf), 0, Ch1,
						Ch2);
				SSD1306_Puts(buf, &Font_7x10, 1);

				SSD1306_DrawFilledRectangle(0, 20, 120, 10, 0);
				SSD1306_DrawFilledRectangle(0, 20, Ch1, 10, 1);

				SSD1306_DrawFilledRectangle(0, 40, 120, 10, 0);
				SSD1306_DrawFilledRectangle(0, 40, Ch2, 10, 1);

				SSD1306_GotoXY(0, 52);
				sprintf_v5(buf, "EEPROM=^, EN:^  ", sizeof(buf), 0, eepromValue,
						encoVal);
				SSD1306_Puts(buf, &Font_7x10, 1);

				ds18b20_start_measure(&ds1);
				ds18b20_get_temp(&ds1, &tmpC, NULL);

				SSD1306_UpdateScreen();

				if (!(menuOk & (1 << 2)))
				{
					EncoderRegisterCallback(&enco, Menu_In, EncoINC, EncoDEC);
					menuOk |= (1 << 2);
					menuOk &= ~(1 << 1);
				}

			}

			softTimer1 = 33;
		}

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{ 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{ 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit =
	{ 0 };

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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}

	/** Enables the Clock Security System
	 */
	HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if (softTimer1)
		softTimer1--;

	if (softTimer2)
		softTimer2--;

}

void Menu_In(void)
{
	menuOk |= (1 << 0);
}

char* sprintf_v5(char *source, char *txt, uint8_t buf_size, uint8_t zeroEnable,
		unsigned int value, ...)
{
	uint8_t i = 0;
	uint8_t tmp = 0;
	va_list arg;
	uint32_t var = value;

	memset(source, 0x00, (buf_size));

	va_start(arg, value);

	while (*(txt + i))
	{
		if ((*(txt + i)) != SEPARATOR && (*(txt + i)) != ZERO_SING)
		{
			*(source + tmp) = *(txt + i);
			i++;
			tmp++;
		}
		else if ((*(txt + i)) == SEPARATOR)
		{
			char buf[20];
			int j = 0;
			i += 1;

			itoa(var, buf, 10);

			while (*(buf + j))
			{
				*(source + tmp) = *(buf + j);
				j++;
				tmp++;
			}
			var = va_arg(arg, unsigned int);

		}
		else if ((*(txt + i)) == ZERO_SING)
		{
			i += 1;
			*(source + tmp) = '.';
			tmp += 1;
			if (zeroEnable)
			{
				*(source + tmp) = '0';
				tmp += 1;
			}
		}
	}
	va_end(arg);
	return source;
}

void EncoINC(void)
{
	encoVal++;
}

void EncoDEC(void)
{
	if (encoVal)
		encoVal--;
}

void ToggLed(void)
{

	HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);

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
