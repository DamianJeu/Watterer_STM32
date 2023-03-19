/*
 * cfg.c
 *
 *  Created on: 4 mar 2023
 *      Author: Damia
 */

#include <string.h>
#include "main.h"
#include "cfg.h"
#include "eeprom.h"

uint16_t VirtAddVarTab[NB_OF_VAR];
uint16_t readEEPROMvalues[NB_OF_VAR];
uint16_t VarDataTab[NB_OF_VAR];

/**
 * @brief Device Init structure definition
 */
typedef struct
{

	//CH1 calibration value------------
	uint16_t ch1_LowCalibVal;
	uint16_t ch1_HighCalibVal;
	//CH2 calibration value------------
	uint16_t ch2_LowCalibVal;
	uint16_t ch2_HighCalibVal;
	//CH1 Dry value
	uint16_t ch1_TooDry;
	uint16_t ch2_TooDry;
	//DISPLAY OFF after no action
	uint16_t dispONOFF;
	T_pumpStatus pumpONOFF;

} T_deviceCFG;

static T_deviceCFG DeviceConfig;

/**
 * @brief  Init device config
 * @param  none
 * @retval none
 */
void Cfg_Init(void)
{

	if (EE_Init() != HAL_OK)
	{
		Error_Handler();
	}

	for (uint8_t VarIndex = 1; VarIndex <= NB_OF_VAR; VarIndex++)
	{
		VirtAddVarTab[VarIndex - 1] = VarIndex;
	}

	for (uint8_t VarIndex = 0; VarIndex < NB_OF_VAR; VarIndex++)
	{
		if ((EE_ReadVariable(VirtAddVarTab[VarIndex],
				&readEEPROMvalues[VarIndex])) != HAL_OK)
		{
			Error_Handler();
		}
	}

	DeviceConfig.ch1_LowCalibVal = readEEPROMvalues[0];
	DeviceConfig.ch2_LowCalibVal = readEEPROMvalues[1];
	DeviceConfig.ch1_HighCalibVal = readEEPROMvalues[2];
	DeviceConfig.ch2_HighCalibVal = readEEPROMvalues[3];

//	DeviceConfig.ch1_LowCalibVal=1000;
//	DeviceConfig.ch2_LowCalibVal=1000;
//	DeviceConfig.ch1_HighCalibVal=1890;
//	DeviceConfig.ch2_HighCalibVal=1890;

	DeviceConfig.ch1_TooDry = readEEPROMvalues[4];
	DeviceConfig.ch2_TooDry = readEEPROMvalues[5];

	DeviceConfig.dispONOFF = readEEPROMvalues[6];

}

/**
 * @brief  T_deviceCFG Setter (LowCalib CH1).
 * @param  uint16_t field value
 * @retval none
 */
void SetCh1_LowCalibVal(uint16_t x)
{

	DeviceConfig.ch1_LowCalibVal = x;

}

/**
 * @brief  T_deviceCFG Setter (HighCalib CH1).
 * @param  uint16_t field value
 * @retval none
 */
void SetCh1_HighCalibVal(uint16_t x)
{

	DeviceConfig.ch1_HighCalibVal = x;

}

/**
 * @brief  T_deviceCFG Setter (LowCalib CH2).
 * @param  uint16_t field value
 * @retval none
 */
void SetCh2_LowCalibVal(uint16_t x)
{

	DeviceConfig.ch2_LowCalibVal = x;

}

/**
 * @brief  T_deviceCFG Setter (HighCalib CH2).
 * @param  uint16_t field value
 * @retval none
 */
void SetCh2_HighCalibVal(uint16_t x)
{

	DeviceConfig.ch2_HighCalibVal = x;

}

/**
 * @brief  T_deviceCFG Setter (TooDry CH1).
 * @param  uint16_t field value
 * @retval none
 */
void SetCh1_TooDryVal(uint16_t x)
{

	DeviceConfig.ch1_TooDry = x;

}

/**
 * @brief  T_deviceCFG Setter (TooDry CH2).
 * @param  uint16_t field value
 * @retval none
 */
void SetCh2_TooDryVal(uint16_t x)
{

	DeviceConfig.ch2_TooDry = x;

}
/**
 * @brief  T_deviceCFG Setter (DisplayOFF).
 * @param  uint16_t field value
 * @retval none
 */
void SetDisp_TimeONOFF(uint16_t x)
{

	DeviceConfig.dispONOFF = x;

}
/**
 * @brief  T_deviceCFG Setter.
 * @param  T_pumpStatus field value (PumpOn, PumpOff)
 * @retval none
 */
void Set_PumpStatus(T_pumpStatus x)
{

	DeviceConfig.pumpONOFF = x;

}

/**
 * @brief  T_deviceCFG Getter (LowCalib CH1).
 * @param  none
 * @retval uint16_t field value
 */
uint16_t GetCh1_LowCalibVal(void)
{

	return DeviceConfig.ch1_LowCalibVal;

}

/**
 * @brief  T_deviceCFG Getter (HighCalib CH1).
 * @param  none
 * @retval uint16_t field value
 */
uint16_t GetCh1_HighCalibVal(void)
{

	return DeviceConfig.ch1_HighCalibVal;

}
/**
 * @brief  T_deviceCFG Getter (LowCalib CH2).
 * @param  none
 * @retval uint16_t field value
 */
uint16_t GetCh2_LowCalibVal(void)
{

	return DeviceConfig.ch2_LowCalibVal;

}
/**
 * @brief  T_deviceCFG Getter (HighCalib CH2).
 * @param  none
 * @retval uint16_t field value
 */
uint16_t GetCh2_HighCalibVal(void)
{

	return DeviceConfig.ch2_HighCalibVal;

}

/**
 * @brief  T_deviceCFG Getter (TooDry CH1).
 * @param  none
 * @retval uint16_t field value
 */
uint16_t GetCh1_TooDryVal(void)
{

	return DeviceConfig.ch1_TooDry;

}
/**
 * @brief  T_deviceCFG Getter (TooDry CH2).
 * @param  none
 * @retval uint16_t field value
 */
uint16_t GetCh2_TooDryVal(void)
{

	return DeviceConfig.ch2_TooDry;

}
/**
 * @brief  T_deviceCFG Getter (DisplayOFF).
 * @param  none
 * @retval uint16_t field value
 */
uint16_t GetDisp_TimeONOFF(void)
{

	return DeviceConfig.dispONOFF;

}

/**
 * @brief  T_deviceCFG Getter (DisplayOFF).
 * @param  none
 * @retval T_pumpStatus field value (PumpOn, PumpOff)
 */
T_pumpStatus Get_PumpStatus(void)
{

	return DeviceConfig.pumpONOFF;

}

/**
 * @brief  EEPROM save callback
 * @param  none
 * @retval none
 */
void EEPROM_Save_CallBack(void)
{

	VarDataTab[0] = DeviceConfig.ch1_LowCalibVal;
	VarDataTab[1] = DeviceConfig.ch2_LowCalibVal;
	VarDataTab[2] = DeviceConfig.ch1_HighCalibVal;
	VarDataTab[3] = DeviceConfig.ch2_HighCalibVal;
	VarDataTab[4] = DeviceConfig.ch1_TooDry;
	VarDataTab[5] = DeviceConfig.ch2_TooDry;
	VarDataTab[6] = DeviceConfig.dispONOFF;

	for (uint8_t VarIndex = 0; VarIndex < NB_OF_VAR; VarIndex++)
	{
		/* Sequence 1 */
		if ((EE_WriteVariable(VirtAddVarTab[VarIndex], VarDataTab[VarIndex]))
				!= HAL_OK)
		{
			Error_Handler();
		}
	}

	for (uint8_t i = 0; i < 3; i++)
	{
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
		HAL_Delay(15);
		HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
		HAL_Delay(15);
	}

}
