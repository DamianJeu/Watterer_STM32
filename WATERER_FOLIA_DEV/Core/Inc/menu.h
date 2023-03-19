/*
 * menu.h
 *
 *  Created on: 1 lis 2022
 *      Author: Damia
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "main.h"
#include "menu.h"
#include "cfg.h"
#include "menu_callback.h"

#define LCD_ROWS 5
#define LCD_COLS 18

typedef struct menu_struct menu_t;

struct menu_struct {

	const char * name;
	uint8_t valueFlag;
	uint16_t * value;
	menu_t * next;
	menu_t * prev;
	menu_t * child;
	menu_t * parent;
	void (*menu_function)(void);

};

typedef struct
{
	uint16_t ch1rawVal;
	uint16_t ch1Low;
	uint16_t ch1LowNew;
	uint16_t ch1High;
	uint16_t ch1HighNew;

	uint16_t ch2rawVal;
	uint16_t ch2Low;
	uint16_t ch2LowNew;
	uint16_t ch2High;
	uint16_t ch2HighNew;

	uint16_t ch1TooDry;
	uint16_t ch2TooDry;

	uint16_t displayTime;

	T_pumpStatus pump;

}T_DisplayVal;

extern T_DisplayVal display;

extern menu_t menu1;
extern	menu_t sub_menu1_CH1;
extern		menu_t sub_menu1_CH1_RAW;
extern		menu_t sub_menu1_CH1_CALIB;
extern		menu_t sub_menu1_CH1_LOWNEW;
extern		menu_t sub_menu1_CH1_HIGHNEW;
extern		menu_t sub_menu1_CH1_SAVE;
extern	menu_t sub_menu1_CH2;
extern		menu_t sub_menu1_CH2_RAW;
extern		menu_t sub_menu1_CH2_CALIB;
extern		menu_t sub_menu1_CH2_LOWNEW;
extern		menu_t sub_menu1_CH2_HIGHNEW;
extern		menu_t sub_menu1_CH2_SAVE;
extern	menu_t sub_menu1_EXIT;

extern menu_t menu2;
extern	menu_t sub_menu2_CH1_TOODRY;
extern	menu_t sub_menu2_CH2_TOODRY;
extern	menu_t sub_menu2_EXIT;

extern menu_t menu3;
extern	menu_t sub_menu3_DISP_OFF;
extern	menu_t sub_menu3_DISP_EXIT;

extern menu_t menu4;
extern	menu_t sub_menu4_START_PUMP;
extern	menu_t sub_menu4_EXIT;

extern menu_t menu5;
extern menu_t menu6;




void Menu_Refresh(void);
void Menu_Next(void);
void Menu_Prev(void);
void Menu_Enter(void);
void Menu_Back(void);
void Menu_Exit (void);

//void ( *menu_exit)(void);


#endif /* INC_MENU_H_ */
