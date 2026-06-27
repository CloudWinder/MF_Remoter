#include <math.h>
#include <stdio.h>
#include <string.h>
#include "trim_ui.h"
#include "display.h"
#include "keyTask.h"
#include "oled.h"
#include "joystick.h"
#include "remoter_ctrl.h"
#include "radiolink.h"
#include "beep.h"
#include "main_ui.h"
#include "config_param.h"


#define TRIM_SCALE 	5
#define PI			(3.14159265)
#define DEG2RAD		0.017453293f	/* ��ת���� ��/180 */

extern float plane_yaw;

double mx_sin(double rad)
{   
	double sine;
	if (rad < 0)
		sine = rad*(1.27323954f + 0.405284735f * rad);
	else
		sine = rad * (1.27323954f - 0.405284735f * rad);
	if (sine < 0)
		sine = sine*(-0.225f * (sine + 1) + 1);
	else
		sine = sine * (0.225f *( sine - 1) + 1);
	return sine;
}

double my_sin(double rad)
{
	int8_t flag = 1;

	if (rad >= PI)
	{
		rad -= PI;
		flag = -1;
	}

	return mx_sin(rad) * flag;
}

float my_cos(double rad)
{
	int8_t flag = 1;
	rad += PI / 2.0;

	if (rad >= PI)
	{
		flag = -1;
		rad -= PI;
	}
	return my_sin(rad)*flag;
}

void trim_ui(void)
{
	char str[22];
	static uint8_t timeOut;
	joystickFlyf_t  percent;
	
	oled_showString(22,0,(uint8_t*)"TRIM",6,12);
	memset(str,0,22);
	sprintf(str,"TRIM_PIT:%5.2f",configParam.trim.pitch);
	oled_showString(20,20,(uint8_t*)str,6,8);
	memset(str,0,22);
	sprintf(str,"TRIM_ROL:%5.2f",configParam.trim.roll);
	oled_showString(20,40,(uint8_t*)str,6,8);

	ADCtoFlyDataPercent(&percent);
	
	if(configParam.flight.mode == HEAD_LESS)//��ͷģʽ
	{	
		float yawRad = plane_yaw * DEG2RAD;
		//float cosy = cosf(yawRad);
		//float siny = sinf(yawRad);
		float cosy = my_cos(yawRad);
		float siny = my_sin(yawRad);
		float originalPitch =  percent.pitch/TRIM_SCALE;
		float originalRoll = percent.roll/TRIM_SCALE;
		configParam.trim.roll += (originalRoll * cosy - originalPitch * siny);
		configParam.trim.pitch += (originalPitch * cosy + originalRoll * siny);
	}
	else//��ͷģʽ
	{
		configParam.trim.pitch += percent.pitch/TRIM_SCALE;
		configParam.trim.roll += percent.roll/TRIM_SCALE;
	}
	configParam.trim.pitch = limit(configParam.trim.pitch, -5.0, 5.0);
	configParam.trim.roll = limit(configParam.trim.roll, -5.0, 5.0);
	
	// ��������
	uint8_t keyState = getKeyState();
	if(keyState==KEY_J2_SHORT_PRESS || (++timeOut>60))//�̰�KEY_R��ʱ�˳�΢������
	{
		timeOut = 0;
		setTrimFlag(false);/*���΢����־*/
		stopBeepAcktion(TRIM_BEEP);/*�رշ�����*/
		setShow_ui(MAIN_UI);
	}
}

