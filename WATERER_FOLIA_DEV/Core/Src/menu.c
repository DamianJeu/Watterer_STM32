/*
 * menu.c
 *
 *  Created on: 1 lis 2022
 *      Author: Damia
 *      Sources: http://www.embeddeddev.pl/menu-na-lcd-wprowadzenie/
 */
#include <string.h>
#include <stdio.h>
#include "main.h"

#include "menu.h"
#include "cfg.h"
#include "higrometer.h"
#include "menu_callback.h"
#include "../../OLED2/fonts.h"
#include "../../OLED2/ssd1306.h"

extern uint8_t menuOk;

T_DisplayVal display;

/**
 * @brief Menu list structure and dependency
 */
menu_t menu1 =
{ "CALIBRATION", 0, NULL, &menu2, &menu6, &sub_menu1_CH1, NULL, NULL };
menu_t sub_menu1_CH1 =

{ "CH1-CALIBRATION", 0, NULL, &sub_menu1_CH2, &sub_menu1_EXIT,
		&sub_menu1_CH1_RAW, &menu1, NULL };
menu_t sub_menu1_CH1_RAW =
{ "RAW VAL:", 1, &display.ch1rawVal, &sub_menu1_CH1_CALIB, &sub_menu1_CH1_SAVE,
		NULL, &sub_menu1_CH1,
		NULL };
menu_t sub_menu1_CH1_CALIB =
{ "Calib. value:", 1, &display.ch1Low, &sub_menu1_CH1_LOWNEW,
		&sub_menu1_CH1_RAW, NULL, &sub_menu1_CH1,
		NULL };
menu_t sub_menu1_CH1_LOWNEW =
{ "LOW NEW:", 1, &display.ch1LowNew, &sub_menu1_CH1_HIGHNEW,
		&sub_menu1_CH1_CALIB, NULL, &sub_menu1_CH1, Change_CH1_LowCalibVal };
menu_t sub_menu1_CH1_HIGHNEW =
{ "HIGH NEW:", 1, &display.ch1HighNew, &sub_menu1_CH1_SAVE,
		&sub_menu1_CH1_LOWNEW, NULL, &sub_menu1_CH1, Change_CH1_HighCalibVal };
menu_t sub_menu1_CH1_SAVE =
		{ "EXIT", 0, NULL, NULL, &sub_menu1_CH1_HIGHNEW, NULL, &sub_menu1_CH1,
				Menu_Back };
menu_t sub_menu1_CH2 =

{ "CH2-CALIBRATION", 0, NULL, &sub_menu1_EXIT, &sub_menu1_CH1,
		&sub_menu1_CH2_RAW, &menu1, NULL };
menu_t sub_menu1_CH2_RAW =
{ "RAW VAL:", 1, &display.ch2rawVal, &sub_menu1_CH2_CALIB, &sub_menu1_CH2_SAVE,
		NULL, &sub_menu1_CH2,
		NULL };
menu_t sub_menu1_CH2_CALIB =
{ "Calib. value:", 1, &display.ch2Low, &sub_menu1_CH2_LOWNEW,
		&sub_menu1_CH2_RAW, NULL, &sub_menu1_CH2,
		NULL };
menu_t sub_menu1_CH2_LOWNEW =
{ "LOW NEW:", 1, &display.ch2LowNew, &sub_menu1_CH2_HIGHNEW,
		&sub_menu1_CH2_CALIB, NULL, &sub_menu1_CH2, Change_CH2_LowCalibVal };
menu_t sub_menu1_CH2_HIGHNEW =
{ "HIGH NEW:", 1, &display.ch2HighNew, &sub_menu1_CH2_SAVE,
		&sub_menu1_CH2_LOWNEW, NULL, &sub_menu1_CH2, Change_CH2_HighCalibVal };
menu_t sub_menu1_CH2_SAVE =
		{ "EXIT", 0, NULL, NULL, &sub_menu1_CH2_HIGHNEW, NULL, &sub_menu1_CH2,
				Menu_Back };
menu_t sub_menu1_EXIT =
{ "EXIT", 0, NULL, NULL, &sub_menu1_CH2, NULL, &menu1, Menu_Back };

menu_t menu2 =
{ "TOO DRY", 0, NULL, &menu3, &menu1, &sub_menu2_CH1_TOODRY, NULL, NULL };
menu_t sub_menu2_CH1_TOODRY =
{ "CH1:", 1, &display.ch1TooDry, &sub_menu2_CH2_TOODRY, &sub_menu2_EXIT, NULL,
		&menu2, Change_CH1_TooDryVal };
menu_t sub_menu2_CH2_TOODRY =
{ "CH2:", 1, &display.ch2TooDry, &sub_menu2_EXIT, &sub_menu2_CH1_TOODRY, NULL,
		&menu2, Change_CH2_TooDryVal };
menu_t sub_menu2_EXIT =
{ "EXIT", 0, NULL, NULL, &sub_menu2_CH2_TOODRY, NULL, &menu2, Menu_Back };

menu_t menu3 =
{ "DISPLAY", 0, NULL, &menu4, &menu2, &sub_menu3_DISP_OFF, NULL, NULL };
menu_t sub_menu3_DISP_OFF =
{ "ON/OFF:", 1, &display.displayTime, &sub_menu3_DISP_EXIT,
		&sub_menu3_DISP_EXIT, NULL, &menu3, Change_DisplayONOFF };
menu_t sub_menu3_DISP_EXIT =
{ "EXIT", 0, NULL, NULL, &sub_menu3_DISP_OFF, NULL, &menu3, Menu_Back };

menu_t menu4 =
{ "FORCE", 0, NULL, &menu5, &menu3, &sub_menu4_START_PUMP, NULL, NULL };
menu_t sub_menu4_START_PUMP =
{ "START PUMP:", 1, (uint16_t*) &display.pump, &sub_menu4_EXIT, &sub_menu4_EXIT,
		NULL, &menu4, Force_PumpONOFF };
menu_t sub_menu4_EXIT =
{ "EXIT", 0, NULL, NULL, &sub_menu4_START_PUMP, NULL, &menu4, Menu_Back };

menu_t menu5 =
{ "SAVE EEPROM", 0, NULL, &menu6, &menu4, NULL, NULL, EEPROM_Save_CallBack };

menu_t menu6 =
{ "EXIT", 0, NULL, NULL, &menu5, NULL, NULL, Menu_Exit };

menu_t *currentPointer = &menu1;

uint8_t menu_index;
uint8_t lcd_row_pos;
uint8_t lcd_row_pos_level_1, lcd_row_pos_level_2;

/**
 * @brief  Menu_Refresh
 * @param  none
 * @retval none
 */
void Menu_Refresh(void)
{

	menu_t *temp;
	uint8_t i;
	uint8_t center;
	uint8_t len = 0;

	char tempBuffer[20];
	//char buffer2[10];

	for (uint16_t i = 0; i < 1024; i++)
	{
		SSD1306_Buffer[i] = 0;
	}

	if (currentPointer->parent)
	{

		temp = (currentPointer->parent)->child;

		center = (( LCD_COLS - (strlen((temp->parent)->name))) * 5) >> 1;

		SSD1306_GotoXY((center - 1), 0);
		SSD1306_Putc(' ', &Font_7x10, 1);

		SSD1306_Puts((temp->parent)->name, &Font_7x10, 0);

		SSD1306_Putc(' ', &Font_7x10, 1);

	}
	else
	{

		temp = &menu1;
		center = (LCD_COLS * 5 - 4) >> 1;
		SSD1306_GotoXY((center - 1), 0);

		//buf_str_XY(center-1,0," MENU ");
		SSD1306_Puts(" MENU ", &Font_7x10, 0);
	}

	for (i = 0; i != menu_index - lcd_row_pos; i++)
	{
		temp = temp->next;
	}

	for (i = 1; i < LCD_ROWS; i++)
	{

		SSD1306_GotoXY(0, i * 11);
		if (temp == currentPointer)
			SSD1306_Putc('>', &Font_7x10, 0);
		else
			SSD1306_Putc(' ', &Font_7x10, 1);

		SSD1306_GotoXY(7, i * 11);
		if (temp->valueFlag && temp->value != NULL)
		{
			len = strlen(temp->name);
			memcpy(tempBuffer, temp->name, len);
			sprintf_v5((tempBuffer + len), "^ ", (sizeof(tempBuffer) - len), 0,
					*(temp->value));
			SSD1306_Puts(tempBuffer, &Font_7x10, 1);

		}
		else
			SSD1306_Puts(temp->name, &Font_7x10, 1);

		temp = temp->next;
		if (!temp)
			break;

	}
	SSD1306_UpdateScreen();
}

/**
 * @brief  Menu_Get_Level
 * @param  menu_t * Element addres
 * @retval uint8_t Level number
 */
static uint8_t Menu_Get_Level(menu_t *q)
{

	menu_t *temp = q;
	uint8_t i = 0;

	if (!q->parent)
		return 0;

	while (temp->parent != NULL)
	{
		temp = temp->parent;
		i++;
	}

	return i;
}

/**
 * @brief  Menu_Get_Index
 * @param  menu_t * Element addres
 * @retval uint8_t item number
 */
static uint8_t Menu_Get_Index(menu_t *q)
{

	menu_t *temp;
	uint8_t i = 0;

	if (q->parent)
		temp = (q->parent)->child;
	else
		temp = &menu1;

	while (temp != q)
	{
		temp = temp->next;
		i++;
	}

	return i;
}

/**
 * @brief  Menu_Enter (Next lvl in menu)
 * @param  none
 * @retval none
 */
void Menu_Enter(void)
{

	if (currentPointer->menu_function)
		currentPointer->menu_function();
	else

	if (currentPointer->child)
	{

		switch (Menu_Get_Level(currentPointer))
		{
		case 0:
			lcd_row_pos_level_1 = lcd_row_pos;
			break;

		case 1:
			lcd_row_pos_level_2 = lcd_row_pos;
			break;
		}

		menu_index = 0;
		lcd_row_pos = 0;

		currentPointer = currentPointer->child;

		Menu_Refresh();
	}
}

/**
 * @brief  Menu_Next (Next index in menu)
 * @param  none
 * @retval none
 */
void Menu_Next(void)
{

	if (currentPointer->next)
	{

		currentPointer = currentPointer->next;
		menu_index++;
		if (++lcd_row_pos > LCD_ROWS - 2)
			lcd_row_pos = LCD_ROWS - 2;
	}
	else
	{
		menu_index = 0;
		lcd_row_pos = 0;

		if (currentPointer->parent)
			currentPointer = (currentPointer->parent)->child;
		else
			currentPointer = &menu1;
	}

	Menu_Refresh();

}

/**
 * @brief  Menu_Back (Previous index in menu)
 * @param  none
 * @retval none
 */
void Menu_Prev(void)
{

	currentPointer = currentPointer->prev;

	if (menu_index)
	{
		menu_index--;
		if (lcd_row_pos > 0)
			lcd_row_pos--;
	}
	else
	{
		menu_index = Menu_Get_Index(currentPointer);

		if (menu_index >= LCD_ROWS - 2)
			lcd_row_pos = LCD_ROWS - 2;
		else
			lcd_row_pos = menu_index;
	}

	Menu_Refresh();
}

/**
 * @brief  Menu_Back (back one level in menu)
 * @param  none
 * @retval none
 */
void Menu_Back(void)
{

	if (currentPointer->parent)
	{

		switch (Menu_Get_Level(currentPointer))
		{
		case 1:
			lcd_row_pos = lcd_row_pos_level_1;
			break;

		case 2:
			lcd_row_pos = lcd_row_pos_level_2;
			break;
		}

		currentPointer = currentPointer->parent;
		menu_index = Menu_Get_Index(currentPointer);

		Menu_Refresh();

	}
}

/**
 * @brief  Menu_Refresh
 * @param  none
 * @retval none
 */
void Menu_Exit(void)
{
	menuOk = 0;
	currentPointer = &menu1;
	lcd_row_pos = 0;
	menu_index = 0;
	Menu_Refresh();
	SSD1306_Clear();

}

