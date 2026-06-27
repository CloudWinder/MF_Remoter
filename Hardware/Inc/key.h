#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f1xx_hal.h"
   	 

#define KEY_L  		GPIO_PIN_11
#define KEY_R  		GPIO_PIN_15
#define KEY_S1   	GPIO_PIN_10
#define KEY_S2   	GPIO_PIN_8

#define READ_KEY_L()  	HAL_GPIO_ReadPin(GPIOB,KEY_L)	//��ȡ�󰴼�
#define READ_KEY_R() 	HAL_GPIO_ReadPin(GPIOC,KEY_R)	//��ȡ�Ұ���
#define READ_KEY_J1()  	HAL_GPIO_ReadPin(GPIOB,KEY_S1)	//��ȡҡ��1����
#define READ_KEY_J2()  	HAL_GPIO_ReadPin(GPIOA,KEY_S2)	//��ȡҡ��2����

uint8_t keyScan(void);


//IO��ʼ��
//void keyInit(void);

 //����ɨ�躯��		
//void KEY_Scan(void);

#endif


