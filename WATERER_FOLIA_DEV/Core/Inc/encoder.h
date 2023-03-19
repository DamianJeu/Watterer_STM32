/*
 * encoder.h
 *
 *  Created on: 4 lis 2022
 *      Author: Damia
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "main.h"

#define PULSE 1


/**
 * @brief Encoder_T structure
 */
typedef struct
{
	GPIO_TypeDef *GPIO_button;
	uint16_t GPIO_Pin_button;
	TIM_HandleTypeDef *htim;
	uint32_t Channel;
	uint16_t *Timer;
	void (*BtnPressed) (void);
	void (*ValueUp) (void);
	void (*ValueDown) (void);

} Encoder_T;

void Encoder_Init(Encoder_T *enco, GPIO_TypeDef *GPIO_btn, uint16_t GPIO_PinBtn,
		TIM_HandleTypeDef *htim, uint32_t Ch, uint16_t *timer);
uint16_t Encoder_Move(Encoder_T *enco);
void Encoder_Pressed(Encoder_T *enco);
void EncoderRegisterCallback(Encoder_T *enco, void (*Btn) (void), void (*Up) (void), void (*Down) (void));
uint16_t Encoder_Event(Encoder_T *enco);


#endif /* INC_ENCODER_H_ */
