/*
 * higrometer.c
 *
 *  Created on: 11 lis 2022
 *      Author: Damia
 */

#include "higrometer.h"

static uint8_t adcStart = 0;
static uint32_t aveCh1, aveCh2;
static uint16_t adcBuffer[SAMPLE_BUF_SIZE];
static ADC_HandleTypeDef* InternalHadc;

/**
 * @brief  ADC_init
 * @param  ADC_HandleTypeDef*
 * @retval none
 */
void HigroInit(ADC_HandleTypeDef* hadc)
{
	InternalHadc = hadc;
}


/**
 * @brief  Start ADC conversion with DMA
 * @param  none
 * @retval none
 */
void HigroMeasure(void)
{

	if (!adcStart)
	{
		uint64_t sum1 = 0, sum2 = 0;

		for (uint16_t i = 0; i < SAMPLE_BUF_SIZE; i++)
		{
			if (!(i % 2))
			{
				sum1 += adcBuffer[i];
			}
			else
				sum2 += adcBuffer[i];

		}

		/*********FILTER*********/
		aveCh1 *= NUM_OF_SAMPLE;
		aveCh2 *= NUM_OF_SAMPLE;

		aveCh1 +=  (sum1 / SAMPLE_BUF_SIZE);
		aveCh2 +=  (sum2 / SAMPLE_BUF_SIZE);

		aveCh1 /= (NUM_OF_SAMPLE+1);
		aveCh2 /= (NUM_OF_SAMPLE +1);

		adcStart = 1;
		HAL_ADC_Start_DMA(InternalHadc, (uint32_t*) adcBuffer, SAMPLE_BUF_SIZE);
	}

}


/**
 * @brief  ADC CH1 Getter
 * @param  uint16_t ADC CH1 Value
 * @retval none
 */
uint16_t Get_Data_CH1(void)
{

	return aveCh1;

}

/**
 * @brief  ADC CH2 Getter
 * @param  uint16_t ADC CH2 Value
 * @retval none
 */
uint16_t Get_Data_CH2(void)
{

	return aveCh2;

}


/**
 * @brief  ADC Completed
 * @param  ADC_HandleTypeDef
 * @retval none
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	adcStart = 0;
	if(hadc->Instance==InternalHadc->Instance)HAL_ADC_Stop_DMA(hadc);
}

