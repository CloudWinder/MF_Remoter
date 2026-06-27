#include <string.h>
#include <stdio.h>
#include "joystickCalib_ui.h"
#include "keyTask.h"
#include "display.h"
#include "joystick.h"
#include "text.h"
#include "config_param.h"
#include "oled.h"


const unsigned char* stepLanguage1[3]={
{"1.ҡ������ҡ��һȦ"},
{"1.Rock two joysticks   a circle"},
{"1.�u�Ӄɂ��u�UһȦ"},
};

const unsigned char* stepLanguage2[3]={
{"2.��KEY_R��һ��"},
{"2.Press KEY_R next"},
{"2.��KEY_R��һ��"},
};

const unsigned char* stepLanguage3[3]={
{"3.����ҡ�˻���"},
{"3.Keep joysticks on    center"},
{"3.���֓u�U����"},
};

const unsigned char* stepLanguage4[3]={
{"4.��KEY_R���"},
{"4.Press KEY_R finish"},
{"4.��KEY_R���"},
};

/*ҡ��У׼����*/
void joystickCalib_ui(void)
{
	static uint8_t steps;
	char str[30];
	static int16_t max[4],min[4] = {1000,1000,1000,1000};
	uint8_t y;
	joystickParam_t jsParam;
	joystickFlyui16_t adcValue;
	
	getFlyDataADCValue(&adcValue);
	if(steps==0)
	{
		oled_fill(0,0,127,38,0);
		y = 14;
		if(configParam.language == ENGLISH) 
			y = 26;
		show_str(0,0,stepLanguage1[configParam.language],12,12,1);
		show_str(0,y,stepLanguage2[configParam.language],12,12,1);
					
		//roll
		max[0] = MAX(max[0],adcValue.roll);
		min[0] = MIN(min[0],adcValue.roll);	
		//pitch
		max[1] = MAX(max[1],adcValue.pitch);
		min[1] = MIN(min[1],adcValue.pitch);
		//yaw
		max[2] = MAX(max[2],adcValue.yaw);
		min[2] = MIN(min[2],adcValue.yaw);
		//thrust
		max[3] = MAX(max[3],adcValue.thrust);
		min[3] = MIN(min[3],adcValue.thrust);
	}
	if(steps==1)
	{
		oled_fill(0,0,127,38,0);
		y = 14;
		if(configParam.language == ENGLISH) 
			y = 26;
		show_str(0,0,stepLanguage3[configParam.language],12,12,1);
		show_str(0,y,stepLanguage4[configParam.language],12,12,1);
		
		jsParam.roll.mid = adcValue.roll;
		jsParam.pitch.mid = adcValue.pitch;
		jsParam.yaw.mid = adcValue.yaw;
		jsParam.thrust.mid = adcValue.thrust;
		
		jsParam.roll.range_pos = max[0] - jsParam.roll.mid;
		jsParam.roll.range_neg = jsParam.roll.mid - min[0];	
		
		jsParam.pitch.range_pos = max[1] - jsParam.pitch.mid;
		jsParam.pitch.range_neg = jsParam.pitch.mid - min[1];
		
		jsParam.yaw.range_pos = max[2] - jsParam.yaw.mid;
		jsParam.yaw.range_neg = jsParam.yaw.mid - min[2];
		
		jsParam.thrust.range_pos = max[3] - jsParam.thrust.mid;
		jsParam.thrust.range_neg = jsParam.thrust.mid - min[3];
	}
	
	memset(str,0,22);
	sprintf(str,"THR:%4d    PIT:%4d",adcValue.thrust,adcValue.pitch);
	oled_showString(0,44,(uint8_t*)str,6,8);
	memset(str,0,22);
	sprintf(str,"YAW:%4d    ROL:%4d",adcValue.yaw,adcValue.roll);
	oled_showString(0,54,(uint8_t*)str,6,8);	
	
	uint8_t keyState = getKeyState();
	if(keyState == KEY_R_SHORT_PRESS)/*�̰�KEY_R������һ��*/
	{
		steps++;
		if(steps==2)
		{
			steps=0;
			configParam.jsParam = jsParam;
			writeConfigParamToFlash();
			for(uint8_t i=0; i<4; i++)
			{
				min[i] = 1000;
				max[i] = 1000;
			}
			setShow_ui(MAIN_UI);
		}
	}
	else if(keyState == KEY_L_SHORT_PRESS)/*�̰�KEY_L�˳�У׼*/
	{
		steps=0;
		setShow_ui(MAIN_UI);
	}
}
