/*
 * waterer_math.c
 *
 *  Created on: 12 mar 2023
 *      Author: Damia
 */

#include <waterer_math.h>
#include "main.h"


/**
 * @brief  ADC_init
 * @param  uint16_t maxVal - maximum expected value
 * @param  uint16_t minVal - minimum expected value
 * @retval uint16_t value (0-100)
 */
uint16_t Calibration(uint16_t minVal, uint16_t maxVal, uint16_t rawValue)
{

	float a;
	float b;

	a = 100.0 /(maxVal-minVal);
	b = ( a*(-1.0) )*minVal;

	if((uint16_t) 100.0-(a*rawValue+b)>100)return 100;
	else return (uint16_t) 100.0-(a*rawValue+b);


	return 0;
}
