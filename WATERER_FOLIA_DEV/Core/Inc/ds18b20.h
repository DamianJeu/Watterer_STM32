/*
 * ds18b20.h
 *
 *  Created on: 27 paź 2022
 *      Author: Damia
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "main.h"
#include "1wire.h"

#define DS18B20_ROM_CODE_SIZE		8

typedef struct
{
	uint8_t rom[DS18B20_ROM_CODE_SIZE];
	uint16_t temp;
	oneWire_t * oneWire;

}ds18b20_T;

// Inicjalizacja 1-wire
// return - HAL_OK/HAL_ERROR
HAL_StatusTypeDef ds18b20_init(ds18b20_T *ds, oneWire_t *wire);

// Odczytaj adres czujnika i policz CRC
// rom_code - miejsce do zapisu odczytanych danych
// return - HAL_OK/HAL_ERROR
HAL_StatusTypeDef ds18b20_read_address(ds18b20_T *ds);

// Rozpocznij pomiar temperatury
// rom_code - adres czujnika lub NULL
// return - HAL_OK/HAL_ERROR
HAL_StatusTypeDef ds18b20_start_measure(ds18b20_T *ds);

// Pobierz temperaturę
// rom_code - adres czujnika lub NULL
// return - temperatura w stopniach Celsjusza
HAL_StatusTypeDef ds18b20_get_temp(ds18b20_T *ds, int16_t *dec, int16_t * frac);


#endif /* INC_DS18B20_H_ */
