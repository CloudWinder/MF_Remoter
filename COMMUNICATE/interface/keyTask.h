#ifndef __KEY_TASK_H
#define __KEY_TASK_H	 
#include "stm32f1xx_hal.h"
#include "key.h"


//����״̬
#define KEY_L_SHORT_PRESS	1	
#define KEY_L_LONG_PRESS	2
#define KEY_R_SHORT_PRESS	3	
#define KEY_R_LONG_PRESS	4	
#define KEY_J1_SHORT_PRESS	5	
#define KEY_J1_LONG_PRESS	6
#define KEY_J2_SHORT_PRESS	7
#define KEY_J2_LONG_PRESS	8

void keyTask(void* param);
uint8_t getKeyState(void);

#endif


