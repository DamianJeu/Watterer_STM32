/*
 * cfg.h
 *
 *  Created on: 4 mar 2023
 *      Author: Damia
 */

#ifndef INC_CFG_H_
#define INC_CFG_H_

#include "main.h"

typedef enum {PumpOn, PumpOff}T_pumpStatus;



void Cfg_Init(void);
void EEPROM_Save_CallBack(void);


//Settes
void SetCh1_LowCalibVal(uint16_t x);
void SetCh1_HighCalibVal(uint16_t x);
void SetCh2_LowCalibVal(uint16_t x);
void SetCh2_HighCalibVal(uint16_t x);
void SetCh1_TooDryVal(uint16_t x);
void SetCh2_TooDryVal(uint16_t x);
void SetDisp_TimeONOFF(uint16_t x);
void Set_PumpStatus(T_pumpStatus x);

//Getters
uint16_t GetCh1_LowCalibVal(void);
uint16_t GetCh1_HighCalibVal(void);
uint16_t GetCh2_LowCalibVal(void);
uint16_t GetCh2_HighCalibVal(void);
uint16_t GetCh1_TooDryVal(void);
uint16_t GetCh2_TooDryVal(void);
uint16_t GetDisp_TimeONOFF(void);
T_pumpStatus Get_PumpStatus(void);



#endif /* INC_CFG_H_ */
