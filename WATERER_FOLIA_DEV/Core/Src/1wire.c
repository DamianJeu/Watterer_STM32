/*
 * 1wire.c
 *
 *  Created on: Oct 23, 2022
 *      Author: Damia
 */

#include "1wire.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;

HAL_StatusTypeDef wire_init(oneWire_t *oneWire, TIM_HandleTypeDef *htim,
		GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	if (oneWire != NULL)
	{
		oneWire->htim = htim;
		oneWire->GPIOx = GPIOx;
		oneWire->GPIO_Pin = GPIO_Pin;

		return HAL_OK;
	}
	return HAL_ERROR;
}

//it needs to be thought out
static void delay_us(oneWire_t *oneWire, uint32_t us)
{
	//uint16_t time = us;

	__HAL_TIM_SET_COUNTER(oneWire->htim, 0);
	while (__HAL_TIM_GET_COUNTER(oneWire->htim) < us);

}

HAL_StatusTypeDef wire_reset(oneWire_t *oneWire)
{
	int rc;

	HAL_GPIO_WritePin(oneWire->GPIOx, oneWire->GPIO_Pin, GPIO_PIN_RESET);
	delay_us(oneWire, 480);
	HAL_GPIO_WritePin(oneWire->GPIOx, oneWire->GPIO_Pin, GPIO_PIN_SET);
	delay_us(oneWire, 70);
	rc = HAL_GPIO_ReadPin(oneWire->GPIOx, oneWire->GPIO_Pin);
	delay_us(oneWire, 410);

	if (rc == 0)
		return HAL_OK;
	else
		return HAL_ERROR;
}

static int read_bit(oneWire_t *oneWire)
{
	int rc;
	HAL_GPIO_WritePin(oneWire->GPIOx, oneWire->GPIO_Pin, GPIO_PIN_RESET);
	delay_us(oneWire, 6);
	HAL_GPIO_WritePin(oneWire->GPIOx, oneWire->GPIO_Pin, GPIO_PIN_SET);
	delay_us(oneWire, 9);
	rc = HAL_GPIO_ReadPin(oneWire->GPIOx, oneWire->GPIO_Pin);
	delay_us(oneWire, 55);
	return rc;
}

uint8_t wire_read(oneWire_t *oneWire)
{
	uint8_t value = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		value >>= 1;
		if (read_bit(oneWire))
			value |= 0x80;
	}
	return value;
}

static void write_bit(oneWire_t *oneWire, int value)
{
	if (value)
	{
		HAL_GPIO_WritePin(oneWire->GPIOx, oneWire->GPIO_Pin, GPIO_PIN_RESET);
		delay_us(oneWire, 6);
		HAL_GPIO_WritePin(oneWire->GPIOx, oneWire->GPIO_Pin, GPIO_PIN_SET);
		delay_us(oneWire, 64);
	}
	else
	{
		HAL_GPIO_WritePin(oneWire->GPIOx, oneWire->GPIO_Pin, GPIO_PIN_RESET);
		delay_us(oneWire, 60);
		HAL_GPIO_WritePin(oneWire->GPIOx, oneWire->GPIO_Pin, GPIO_PIN_SET);
		delay_us(oneWire, 10);
	}
}

void wire_write(oneWire_t *oneWire, uint8_t byte)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		write_bit(oneWire, byte & 0x01);
		byte >>= 1;
	}
}

static uint8_t byte_crc(uint8_t crc, uint8_t byte)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		uint8_t b = crc ^ byte;
		crc >>= 1;
		if (b & 0x01)
			crc ^= 0x8c;
		byte >>= 1;
	}
	return crc;
}

uint8_t wire_crc(const uint8_t *data, int len)
{
	int i;
	uint8_t crc = 0;

	for (i = 0; i < len; i++)
		crc = byte_crc(crc, data[i]);

	return crc;
}
