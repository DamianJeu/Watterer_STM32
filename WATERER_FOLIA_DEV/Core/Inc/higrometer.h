/*
 * higrometer.h
 *
 *  Created on: 11 lis 2022
 *      Author: Damia
 */

#ifndef INC_HIGROMETER_H_
#define INC_HIGROMETER_H_

#include "adc.h"

#define  SAMPLE_BUF_SIZE 20
#define NUM_OF_SAMPLE 128


void HigroMeasure(void);
uint16_t Get_Data_CH1(void);
uint16_t Get_Data_CH2(void);
void HigroInit(ADC_HandleTypeDef* hadc);

#endif /* INC_HIGROMETER_H_ */
