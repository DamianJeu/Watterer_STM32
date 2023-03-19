/*
 * menu_callback.c
 *
 *  Created on: 4 mar 2023
 *      Author: Damia
 */
#include <stdio.h>
#include "menu.h"
#include "cfg.h"
#include "main.h"
#include "encoder.h"

extern Encoder_T enco;
static uint8_t id = 0;

char buffer[30];

/*****************************Calibration Values*****************************/
static void Ch1LowCalibValUp(void)
{
	uint16_t temp = GetCh1_LowCalibVal();
	temp++;
	SetCh1_LowCalibVal(temp);
	display.ch1LowNew = temp;

}

static void Ch1LowCalibValDown(void)
{
	uint16_t temp = GetCh1_LowCalibVal();
	if (temp)
		temp--;
	SetCh1_LowCalibVal(temp);
	display.ch1LowNew = temp;

}

static void Ch2LowCalibValUp(void)
{
	uint16_t temp = GetCh2_LowCalibVal();
	temp++;
	SetCh2_LowCalibVal(temp);
	display.ch2LowNew = temp;

}

static void Ch2LowCalibValDown(void)
{
	uint16_t temp = GetCh2_LowCalibVal();
	if (temp)
		temp--;
	SetCh2_LowCalibVal(temp);
	display.ch2LowNew = temp;

}

static void Ch1HighCalibValUp(void)
{
	uint16_t temp = GetCh1_HighCalibVal();
	temp++;
	SetCh1_HighCalibVal(temp);
	display.ch1HighNew = temp;

}

static void Ch1HighCalibValDown(void)
{
	uint16_t temp = GetCh1_HighCalibVal();
	if (temp)
		temp--;
	SetCh1_HighCalibVal(temp);
	display.ch1HighNew = temp;

}

static void Ch2HighCalibValUp(void)
{
	uint16_t temp = GetCh2_HighCalibVal();
	temp++;
	SetCh2_HighCalibVal(temp);
	display.ch2HighNew = temp;

}

static void Ch2HighCalibValDown(void)
{
	uint16_t temp = GetCh2_HighCalibVal();
	if (temp)
		temp--;
	SetCh2_HighCalibVal(temp);
	display.ch2HighNew = temp;

}

/**************************TooDry Values*************************************/

static void CH1_TooDryValUp(void)
{
	uint16_t temp = GetCh1_TooDryVal();
	temp++;
	SetCh1_TooDryVal(temp);
	display.ch1TooDry = temp;
}

static void CH1_TooDryValDown(void)
{
	uint16_t temp = GetCh1_TooDryVal();
	if (temp)
		temp--;
	SetCh1_TooDryVal(temp);
	display.ch1TooDry = temp;
}

static void CH2_TooDryValUp(void)
{
	uint16_t temp = GetCh2_TooDryVal();
	temp++;
	SetCh2_TooDryVal(temp);
	display.ch2TooDry = temp;
}

static void CH2_TooDryValDown(void)
{
	uint16_t temp = GetCh2_TooDryVal();
	if (temp)
		temp--;
	SetCh2_TooDryVal(temp);
	display.ch2TooDry = temp;
}

/**************************Display OnOff*****************************************/

static void DisplayONOFFUp(void)
{
	uint16_t temp = GetDisp_TimeONOFF();
	temp++;
	SetDisp_TimeONOFF(temp);
	display.displayTime = temp;
}

static void DisplayONOFFDown(void)
{
	uint16_t temp = GetDisp_TimeONOFF();
	if (temp)
		temp--;
	SetDisp_TimeONOFF(temp);
	display.displayTime = temp;
}

void Exit(void)
{
	EncoderRegisterCallback(&enco, Menu_Enter, Menu_Next, Menu_Prev);
}

void Change_CH1_LowCalibVal(void)
{

	if (!id || id != 1)
	{
		EncoderRegisterCallback(&enco, Exit, Ch1LowCalibValUp,
				Ch1LowCalibValDown);
		id = 1;
	}

	//Encoder_Event(&enco);
}

void Change_CH1_HighCalibVal(void)
{

	if (!id || id != 2)
	{
		EncoderRegisterCallback(&enco, Exit, Ch1HighCalibValUp,
				Ch1HighCalibValDown);
		id = 2;
	}

	//Encoder_Event(&enco);

}

void Change_CH2_LowCalibVal(void)
{

	if (!id || id != 3)
	{
		EncoderRegisterCallback(&enco, Exit, Ch2LowCalibValUp,
				Ch2LowCalibValDown);
		id = 3;
	}

	//Encoder_Event(&enco);
}

void Change_CH2_HighCalibVal(void)
{

	if (!id || id != 4)
	{
		EncoderRegisterCallback(&enco, Exit, Ch2HighCalibValUp,
				Ch2HighCalibValDown);
		id = 4;
	}

	//Encoder_Event(&enco);
}

void Change_CH1_TooDryVal(void)
{

	if (!id || id != 5)
	{
		EncoderRegisterCallback(&enco, Exit, CH1_TooDryValUp,
				CH1_TooDryValDown);
		id = 5;
	}

	//Encoder_Event(&enco);
}

void Change_CH2_TooDryVal(void)
{

	if (!id || id != 6)
	{
		EncoderRegisterCallback(&enco, Exit, CH2_TooDryValUp,
				CH2_TooDryValDown);
		id = 6;
	}

	//Encoder_Event(&enco);
}

void Change_DisplayONOFF(void)
{

	if (!id || id != 7)
	{
		EncoderRegisterCallback(&enco, Exit, DisplayONOFFUp, DisplayONOFFDown);
		id = 7;
	}

	//Encoder_Event(&enco);
}

void Force_PumpONOFF(void)
{

	T_pumpStatus tmp = Get_PumpStatus();
	if (tmp == PumpOff)
		Set_PumpStatus(PumpOn);
	else
		Set_PumpStatus(PumpOff);

	display.pump = Get_PumpStatus();

}

