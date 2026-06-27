#include <stdlib.h>
#include "joystick.h"
#include "my_adc.h"
#include "config_param.h"


//ҡ���м���������ֵ��ADCֵ��
#define MID_DB_THRUST		150	
#define MID_DB_YAW			300	
#define MID_DB_PITCH		150
#define MID_DB_ROLL			150

//ҡ��������������ֵ��ADCֵ��
#define DB_RANGE			10

//��ȡҡ�˷���ʱ�������м�ķ�Χֵ��ADCֵ��
#define DIR_MID_THRUST		800
#define DIR_MID_YAW			800
#define DIR_MID_PITCH		800
#define DIR_MID_ROLL		800


static bool isInit;
static joystickParam_t* jsParam;

/*ȥ����������*/
int deadband(int value, const int threshold)
{
	if (abs(value) < threshold)
	{
		value = 0;
	}
	else if (value > 0)
	{
		value -= threshold;
	}
	else if (value < 0)
	{
		value += threshold;
	}
	return value;
}

/*ҡ�˳�ʼ��*/
void joystickInit(void)
{
	if(isInit) return;
	Adc_Init();
	jsParam = &configParam.jsParam;
	isInit = true;
}

/*��ȡҡ��ADCֵ*/
void getFlyDataADCValue(joystickFlyui16_t *adcValue)
{
	adcValue->thrust = getAdcValue(ADC_THRUST);
	adcValue->roll = getAdcValue(ADC_ROLL);
	adcValue->pitch = getAdcValue(ADC_PITCH);
	adcValue->yaw = getAdcValue(ADC_YAW);
}

/*ADCֵת���ɷɿ����ݰٷֱ�*/
void ADCtoFlyDataPercent(joystickFlyf_t *percent)
{
	int16_t adcValue;
	
	//THRUST
	adcValue = getAdcValue(ADC_THRUST) - jsParam->thrust.mid;
	adcValue = deadband(adcValue,MID_DB_THRUST);
	if(adcValue>=0)
		percent->thrust = (float)adcValue/(jsParam->thrust.range_pos-MID_DB_THRUST-DB_RANGE);
	else
		percent->thrust = (float)adcValue/(jsParam->thrust.range_neg-MID_DB_THRUST-DB_RANGE);
	
	//ROLL
	adcValue = getAdcValue(ADC_ROLL) - jsParam->roll.mid;
	adcValue = deadband(adcValue, MID_DB_ROLL);
	if(adcValue >= 0)
		percent->roll = (float)adcValue/(jsParam->roll.range_pos-MID_DB_ROLL-DB_RANGE);
	else
		percent->roll = (float)adcValue/(jsParam->roll.range_neg-MID_DB_ROLL-DB_RANGE);
	
	//PITCH
	adcValue = getAdcValue(ADC_PITCH) - jsParam->pitch.mid;
	adcValue = deadband(adcValue, MID_DB_PITCH);
	if(adcValue >= 0)
		percent->pitch = (float)adcValue/(jsParam->pitch.range_pos-MID_DB_PITCH-DB_RANGE);
	else
		percent->pitch = (float)adcValue/(jsParam->pitch.range_neg-MID_DB_PITCH-DB_RANGE);
	
	//YAW
	adcValue = getAdcValue(ADC_YAW) - jsParam->yaw.mid;
	adcValue = deadband(adcValue, MID_DB_YAW);
	if(adcValue >= 0)
		percent->yaw = (float)adcValue/(jsParam->yaw.range_pos-MID_DB_YAW-DB_RANGE);
	else
		percent->yaw = (float)adcValue/(jsParam->yaw.range_neg-MID_DB_YAW-DB_RANGE);
}

/*��ȡҡ��1����*/
/*mode:0,��֧��������;1,֧��������*/
enum dir_e getJoystick1Dir(uint8_t mode)
{
	enum dir_e ret=CENTER;
	joystickFlyui16_t adcValue;
	static bool havebackToCenter = true;
	
	getFlyDataADCValue(&adcValue);
	if(mode) havebackToCenter = true;
	if(havebackToCenter == true)//ҡ�˻ص����м�λ��
	{
		if(adcValue.thrust > (jsParam->thrust.mid+DIR_MID_THRUST))
			ret = FORWARD;
		else if(adcValue.thrust < (jsParam->thrust.mid-DIR_MID_THRUST))
			ret = BACK;
		
		if(ret==BACK && adcValue.yaw>(jsParam->yaw.mid+DIR_MID_YAW))
			ret = BACK_RIGHT;
		else if(ret==BACK && adcValue.yaw<(jsParam->yaw.mid-DIR_MID_YAW))
			ret = BACK_LEFT;
		else if(adcValue.yaw > (jsParam->yaw.mid+DIR_MID_YAW))
			ret = RIGHT;
		else if(adcValue.yaw < (jsParam->yaw.mid-DIR_MID_YAW))
			ret = LEFT;
		
		havebackToCenter = false;//ҡ���뿪���м�λ��
		if(ret == CENTER)//ҡ����Ȼ���м�λ��
			havebackToCenter = true;
	}
	else if( adcValue.thrust >= (jsParam->thrust.mid-DIR_MID_THRUST) &&
			 adcValue.thrust <= (jsParam->thrust.mid+DIR_MID_THRUST) &&
			 adcValue.yaw >= (jsParam->yaw.mid-DIR_MID_YAW) &&
			 adcValue.yaw <= (jsParam->yaw.mid+DIR_MID_YAW) 
		   )//ҡ���뿪���м�λ�ã����ڲ�ѯҡ���Ƿ����
	{
		havebackToCenter = true;
		ret = CENTER;
	}
	
	return ret;
}

/*��ȡҡ��2����*/
/*mode:0,��֧��������;1,֧��������*/
enum dir_e getJoystick2Dir(uint8_t mode)
{
	enum dir_e ret = CENTER;
	joystickFlyui16_t adcValue;
	static bool havebackToCenter = true;
	
	getFlyDataADCValue(&adcValue);
	if(mode) havebackToCenter = true;
	if(havebackToCenter == true)//ҡ�˻ص����м�λ��
	{	
		if(adcValue.pitch > (jsParam->pitch.mid+DIR_MID_PITCH))
			ret = FORWARD;
		else if(adcValue.pitch < (jsParam->pitch.mid-DIR_MID_PITCH))
			ret = BACK;
		
		if(ret==BACK && adcValue.roll>(jsParam->roll.mid+DIR_MID_ROLL))
			ret = BACK_RIGHT;
		else if(ret==BACK && adcValue.roll<(jsParam->roll.mid-DIR_MID_ROLL))
			ret = BACK_LEFT;
		else if(adcValue.roll>(jsParam->roll.mid+DIR_MID_ROLL))
			ret = RIGHT;
		else if(adcValue.roll<(jsParam->roll.mid-DIR_MID_ROLL))
			ret = LEFT;

		havebackToCenter = false;//ҡ���뿪���м�λ��
		if(ret == CENTER)//ҡ����Ȼ���м�λ��
			havebackToCenter = true;
	}
	else if( adcValue.pitch >= (jsParam->pitch.mid-DIR_MID_PITCH) &&
			 adcValue.pitch <= (jsParam->pitch.mid+DIR_MID_PITCH) &&
			 adcValue.roll >= (jsParam->roll.mid-DIR_MID_ROLL) &&
			 adcValue.roll <= (jsParam->roll.mid+DIR_MID_ROLL) 
		   )//ҡ���뿪���м�λ�ã����ڲ�ѯҡ���Ƿ����
	{
		havebackToCenter = true;
		ret = CENTER;
	}
	
	return ret;
}

