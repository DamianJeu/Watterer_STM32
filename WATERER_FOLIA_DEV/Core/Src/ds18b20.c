/*
 * ds18b20.c
 *
 *  Created on: 27 paź 2022
 *      Author: Damia
 */

#include <string.h>
#include <stdlib.h>
#include "ds18b20.h"
#include "1wire.h"

#define DS18B20_SCRATCHPAD_SIZE    9

#define DS18B20_READ_ROM           0x33
#define DS18B20_MATCH_ROM          0x55
#define DS18B20_SKIP_ROM           0xCC

#define DS18B20_CONVERT_T          0x44
#define DS18B20_READ_SCRATCHPAD    0xBE




HAL_StatusTypeDef ds18b20_init(ds18b20_T *ds, oneWire_t *wire)
{

	ds->oneWire = wire;
	return wire_init( wire, wire->htim, wire->GPIOx, wire->GPIO_Pin);

}


HAL_StatusTypeDef ds18b20_read_address(ds18b20_T *ds) //uint8_t* rom_code
{

  int i;
  uint8_t crc;

  if (wire_reset(ds->oneWire) != HAL_OK)
    return HAL_ERROR;

  wire_write(ds->oneWire, DS18B20_READ_ROM);

  for (i = 0; i < DS18B20_ROM_CODE_SIZE; i++)
    ds->rom[i] = wire_read( ds->oneWire );

  crc = wire_crc(ds->rom, DS18B20_ROM_CODE_SIZE - 1);
  if (ds->rom[DS18B20_ROM_CODE_SIZE - 1] == crc)
    return HAL_OK;
  else
    return HAL_ERROR;
}


static HAL_StatusTypeDef send_cmd(ds18b20_T *ds, uint8_t cmd)
{
  int i;

  if (wire_reset(ds->oneWire) != HAL_OK)
    return HAL_ERROR;

  if (!ds->rom) {
    wire_write(ds->oneWire, DS18B20_SKIP_ROM);
  } else {
    wire_write(ds->oneWire, DS18B20_MATCH_ROM);
    for (i = 0; i < DS18B20_ROM_CODE_SIZE; i++)
      wire_write(ds->oneWire, ds->rom[i]);
  }
  wire_write(ds->oneWire, cmd);
  return HAL_OK;
}



HAL_StatusTypeDef ds18b20_start_measure(ds18b20_T *ds)
{
  return send_cmd(ds, DS18B20_CONVERT_T);
}


static HAL_StatusTypeDef ds18b20_read_scratchpad(ds18b20_T *ds, uint8_t* scratchpad)
{
  int i;
  uint8_t crc;

  if (send_cmd(ds, DS18B20_READ_SCRATCHPAD) != HAL_OK)
    return HAL_ERROR;

  for (i = 0; i < DS18B20_SCRATCHPAD_SIZE; i++)
    scratchpad[i] = wire_read(ds->oneWire);

  crc = wire_crc(scratchpad, DS18B20_SCRATCHPAD_SIZE - 1);
  if (scratchpad[DS18B20_SCRATCHPAD_SIZE - 1] == crc)
    return HAL_OK;
  else
    return HAL_ERROR;
}


HAL_StatusTypeDef ds18b20_get_temp(ds18b20_T *ds, int16_t *dec, int16_t * frac)
{
  uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];
  int16_t temp;

  if(HAL_ERROR==ds18b20_read_scratchpad(ds, scratchpad))return HAL_ERROR;
  memcpy(&temp, &scratchpad[0], sizeof(temp));
  *dec=temp/16;
  if(frac!=NULL)*frac=((temp*100)/16)%100;


  return HAL_OK;
  //return temp / 16.0f;
}


