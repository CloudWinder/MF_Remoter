#ifndef __BEEP_H
#define __BEEP_H	 
#include "stm32f1xx_hal.h"
#include <stdbool.h>


#define BEEP_ON() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET)
#define BEEP_OFF() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET)
#define BEEP_TOGGLE() HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14)

typedef struct 
{
	bool isActive;
	bool isLoop;
	uint16_t waitMS;
}beepSeq_t;


enum beepAction
{
	TRIM_BEEP = 0,
	FLIP_BEEP,
	LOWPOWER_BEEP,
	CANFLY_BEEP,
};


void beepInit(void);
void runBeepAcktion(enum beepAction runaction);
void stopBeepAcktion(enum beepAction runaction);
#endif
