/*
 * 1wire.h
 *
 *  Created on: Oct 23, 2022
 *      Author: Damia
 */

#ifndef INC_1WIRE_H_
#define INC_1WIRE_H_

#include "main.h"

typedef struct
{
	TIM_HandleTypeDef *htim;
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;

} oneWire_t;

//one wire init
HAL_StatusTypeDef wire_init(oneWire_t *oneWire, TIM_HandleTypeDef *htim,
		GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
//one wire reset
HAL_StatusTypeDef wire_reset(oneWire_t *oneWire);
//one wire read
uint8_t wire_read(oneWire_t *oneWire);
//one wire write
void wire_write(oneWire_t *oneWire, uint8_t byte);
//write crc
uint8_t wire_crc(const uint8_t* data, int len);
//delay


#endif /* INC_1WIRE_H_ */
