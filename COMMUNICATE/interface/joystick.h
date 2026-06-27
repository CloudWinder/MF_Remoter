#ifndef __JOYSTICK_H
#define __JOYSTICK_H
#include "stm32f1xx_hal.h"
#include <stdbool.h>


#define MAX(a,b) a>=b? a:b
#define MIN(a,b) a<=b? a:b

enum dir_e
{
	CENTER,
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
	BACK_LEFT,
	BACK_RIGHT,
};

//ҡ�˵��������̲���
struct range_s
{
	int16_t range_pos;	//ҡ��������
	uint16_t mid;
	int16_t range_neg;	//ҡ�˸�����
};

//ҡ�����̲���
typedef struct
{
	struct range_s thrust;
	struct range_s roll;
	struct range_s pitch;
	struct range_s yaw;
}joystickParam_t;

//�ɿ����ݽṹ
typedef struct 
{
	float roll;
	float pitch;
	float yaw;
	float thrust;
}joystickFlyf_t;

//�ɿ����ݽṹ
typedef struct 
{
	uint16_t roll;
	uint16_t pitch;
	uint16_t yaw;
	uint16_t thrust;
}joystickFlyui16_t;


void joystickInit(void);

void getFlyDataADCValue(joystickFlyui16_t *adcValue);

void ADCtoFlyDataPercent(joystickFlyf_t *flyDataPercent);

void joystcikParamSet(joystickParam_t set);

enum dir_e getJoystick1Dir(uint8_t mode);

enum dir_e getJoystick2Dir(uint8_t mode);


#endif /*__JOYSTICK_H*/
