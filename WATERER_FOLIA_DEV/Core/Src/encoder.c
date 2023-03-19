/*
 * encoder.c
 *
 *  Created on: 4 lis 2022
 *      Author: Damia
 */

#include "encoder.h"

//void (*BtnPressed) (void);
//void (*ValueUp) (void);
//void (*ValueDown) (void);

/**
 * @brief  Register encoder callback
 * @param  Encoder_T instance
 * @param  void (*Btn)(void) - called when buton pressed
 * @param  void (*Up)(void) - called when encoder increase
 * @param  void (*Down)(void) - called when encoder decrease
 * @retval none
 */
void EncoderRegisterCallback(Encoder_T *enco, void (*Btn)(void),
		void (*Up)(void), void (*Down)(void))
{

	enco->BtnPressed = Btn;
	enco->ValueUp = Up;
	enco->ValueDown = Down;

}

/**
 * @brief  Encoder init
 * @param  Encoder_T instance
 * @param  GPIO_TypeDef GPIO buttoon
 * @param  uint16_t pin number
 * @param 	TIM_HandleTypeDef  counter
 * @param 	uint32_t counter channel
 * @param	uint16_t software timer
 * @retval none
 */
void Encoder_Init(Encoder_T *enco, GPIO_TypeDef *GPIO_btn, uint16_t GPIO_PinBtn,
		TIM_HandleTypeDef *htim, uint32_t Ch, uint16_t *timer)
{
	enco->GPIO_button = GPIO_btn;
	enco->GPIO_Pin_button = GPIO_PinBtn;
	enco->htim = htim;
	enco->Channel = Ch;
	enco->Timer = timer;

	HAL_TIM_Encoder_Start(htim, Ch);
	__HAL_TIM_SetCounter(enco->htim, 1000);

}

/**
 * @brief  Encoder move detection
 * @param  Encoder_T instance
 * @retval uint16_t encoder value
 */
uint16_t Encoder_Move(Encoder_T *enco)
{

	static uint16_t lastEnco;
	uint16_t presentEnco;

	//uint16_t tmpPtr = enco->Timer;

	presentEnco = __HAL_TIM_GetCounter(enco->htim);

	if (!(*enco->Timer))
	{
		lastEnco = 1000;
		__HAL_TIM_SetCounter(enco->htim, 1000);
	}

	if ((presentEnco > lastEnco))
	{
		if (!(presentEnco % PULSE))
		{
			if (enco->ValueUp != NULL)
			{
				enco->ValueUp();
			}
		}

		//*tmpPtr = 2000;
		*enco->Timer = 2000;

	}
	else if ((presentEnco < lastEnco))
	{
		if (!(presentEnco % PULSE))
		{
			if (enco->ValueDown != NULL)
			{
				enco->ValueDown();
			}
		}
		*enco->Timer = 2000;
	}

	lastEnco = presentEnco;

	return presentEnco;

}

/**
 * @brief  Encoder button pressed
 * @param  Encoder_T instance
 * @retval none
 */
void Encoder_Pressed(Encoder_T *enco)
{

	static uint8_t key_next_lock = 0;

	if (!key_next_lock
			&& !(HAL_GPIO_ReadPin(enco->GPIO_button, enco->GPIO_Pin_button)))
	{

		key_next_lock = 1;

		if (enco->BtnPressed != NULL)
			enco->BtnPressed();

	}
	else if (key_next_lock
			&& (HAL_GPIO_ReadPin(enco->GPIO_button, enco->GPIO_Pin_button)))
	{
		(key_next_lock < 10) ? (key_next_lock++) : (key_next_lock = 0);

	}

}

/**
 * @brief  Encoder Event
 * @param  Encoder_T instance
 * @retval uint16_t encoder value
 */
uint16_t Encoder_Event(Encoder_T *enco)
{
	uint16_t tmp;

	Encoder_Pressed(enco);
	tmp = Encoder_Move(enco);

	return tmp;

}

