#include <stdio.h>
#include "main_ui.h"
#include "display.h"
#include "font.h"
#include "joystick.h"
#include "keyTask.h"
#include "beep.h"
#include "config_param.h"
#include "oled.h"
#include "atkp.h"
#include "radiolink.h"
#include "my_adc.h"
#include "menuL1_item.h"
#include "text.h"
#include "remoter_ctrl.h"
#include "gui_basic.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"


#define  LOW_POWER  	3.25f
#define  FULL_POWER		4.15f

static bool RCLock = true;
static bool trimFlag = false;
static bool flipFlag = false;
static bool isMFSelfTestPass = true;

static bool RCLowPower = false;
static bool RCLowPowerOld = false;
static bool MFLowPower = false;
static bool MFLowPowerOld = false;
static bool isMFCanFly = false;
static bool isMFCanFlyOld = false;

static int8_t batPercent;
static MiniFlyMsg_t msg;
static TickType_t dispalyBatTime;
static TickType_t getMFMsgTime;
static enum expModuleID moduleID = NO_MODULE;

extern float plane_bat;//���᷵�صĵ���
extern uint8_t rssi;//���᷵�ص������ź�ֵ


static void flight(void)
{
	static uint8_t flipBeepTimeOut;
	
	oled_fill(0,24,127,63,0);
	
	/*ң��������������*/
	if(getJoystick1Dir(1)==BACK_LEFT && getJoystick2Dir(1)==BACK_RIGHT)
	{
		RCLock = false;
	}
	if(getJoystick1Dir(1)==BACK_RIGHT && getJoystick2Dir(1)==BACK_LEFT)
	{
		RCLock = true;
	}
	
	/*��ʾ����ģʽ����ͷ������ģʽ*/
	GUI_Circle(20,43,16,1);
	GUI_Circle(20,43,17,1);
	if(configParam.flight.ctrl == ALTHOLD_MODE)
		oled_showChar(14,30,'H',12,24,1);
	else if(configParam.flight.ctrl == MANUAL_MODE)
		oled_showChar(14,30,'M',12,24,1);
	else 
		oled_showChar(14,30,'T',12,24,1);
	oled_showPicture(49,43-15,bmp_lock[RCLock],30,30);
	oled_showPicture(94,43-15,bmp_flightMode[configParam.flight.mode],30,30);
	GUI_Circle(94+15,43,16,1);
	GUI_Circle(94+15,43,17,1);
	
	/*��������*/
	uint8_t keyState = getKeyState();
	if(keyState == KEY_J1_SHORT_PRESS)//һ����ɡ�����
	{
		if( RCLock==false && isMFCanFly==true &&
			(configParam.flight.ctrl == ALTHOLD_MODE || 
			configParam.flight.ctrl == THREEHOLD_MODE)
		  )
		{
			sendRmotorCmd(CMD_FLIGHT_LAND, NULL);
		}
	}
	else if(keyState == KEY_L_SHORT_PRESS)//����ͣ��
	{
		sendRmotorCmd(CMD_EMER_STOP, NULL);
	}
	else if(keyState == KEY_J2_SHORT_PRESS)//����΢������
	{
		if( RCLock==false && isMFCanFly==true)
		{
			trimFlag = true;
			runBeepAcktion(TRIM_BEEP);
			setShow_ui(TRIM_UI);
		}
	}
	else if(keyState == KEY_R_LONG_PRESS)//���Խ���
	{
		//if(RCLock == false)
		{
			setShow_ui(DEBUG_UI);
		}
	}
	else if(keyState == KEY_R_SHORT_PRESS)//һ������
	{
		if( RCLock==false && isMFCanFly==true && 
			MFLowPower==false && configParam.flight.flip==FLIP_ENABLE  
			&& configParam.flight.ctrl != THREEHOLD_MODE) /*��������ģʽ���رտշ�*/
		{
			flipFlag = !flipFlag;
			flipBeepTimeOut = 0;
			if(flipFlag == true)
				runBeepAcktion(FLIP_BEEP);
			else
				stopBeepAcktion(FLIP_BEEP);
		}
	}
	else if(keyState == KEY_J2_LONG_PRESS)
	{
		if(RCLock == true)
		{
			setShow_ui(MENU_UI);
		}
	}
	
	/*�¼�����*/
	if(flipFlag == true)
	{
		enum dir_e flipDir = getJoystick2Dir(0);
		
		if(flipDir != CENTER)
		{
			flipFlag = false;
			stopBeepAcktion(FLIP_BEEP);
			sendRmotorCmd(CMD_FLIP, flipDir);
		}
		else if(++flipBeepTimeOut>30)
		{
			flipFlag = false;
			stopBeepAcktion(FLIP_BEEP);
			flipBeepTimeOut = 0;
		}
	}
}

void main_ui(void)
{
	uint8_t rssi_level;
	static float remoterBat;
	
	/*�����ٷֱ���ʾ*/
	if(xTaskGetTickCount() >= dispalyBatTime+1000)/*ң��������1000ms����һ��*/
	{
		dispalyBatTime = xTaskGetTickCount();
		remoterBat = ((float)getAdcValue(ADC_BAT)/4095.0)*3*2;
		if(remoterBat < LOW_POWER)
			RCLowPower = true;
		else 
			RCLowPower = false;	
	}
	if(RCLock == true)/*��ʾң��������*/
		batPercent = (remoterBat-LOW_POWER)/(FULL_POWER-LOW_POWER)*100;
	else/*��ʾ�������*/
		batPercent = (plane_bat-LOW_POWER)/(FULL_POWER-LOW_POWER)*100;
	batPercent = limit(batPercent, 0, 99);
	
	/*�ź���ʾ*/
	if(rssi < 80) rssi_level = 5;/*�ź�ֵС80db,�ź�Ϊ5��*/
	else if(rssi < 85) rssi_level = 4;
	else if(rssi < 90) rssi_level = 3;
	else if(rssi < 95) rssi_level = 2;
	else if(rssi >= 95) rssi_level = 1;
	if(radioinkConnectStatus() == false) rssi_level = 0;
		
	/*��ʾͼƬ*/
	oled_showPicture(0,0,bmp_rssi[rssi_level],24,22);
	oled_showPicture(24,0,bmp_logo,80,22);
	oled_showPicture(106,4,bmp_battery[batPercent*12/99],10,16);
	oled_showChar(119,13,'%',6,8,1);
	oled_showNum(116,4,batPercent,2,6,8);
	
	/*�����Լ�*/
	if(isMFSelfTestPass == true)
	{
		flight();/*�Լ�ɹ��ɷ���*/
	}
	else/*�Լ������ʾ����������*/
	{
		char str[22];
		oled_fill(0,24,127,63,0);
		if(msg.mpu_selfTest == true)
			oled_showString(0,24,(uint8_t*)"MPU9250 CHECK [OK]",6,12);
		else
			oled_showString(0,24,(uint8_t*)"MPU9250 CHECK [FAIL]",6,12);
			
		if(msg.baro_slfTest == true)
			oled_showString(0,38,(uint8_t*)"BARO CHECK [OK]",6,12);
		else
			oled_showString(0,38,(uint8_t*)"BARO CHECK [FAIL]",6,12);
		sprintf(str,"MINIFLY FIRMWARE V%1d.%1d",msg.version/10,msg.version%10);
		oled_showString(0,52,(uint8_t*)str,6,12);
	}
	
	/*�͵�������*/
	if(RCLowPower!=RCLowPowerOld || MFLowPower!=MFLowPowerOld)
	{
		if(RCLowPower==true || MFLowPower==true)
			runBeepAcktion(LOWPOWER_BEEP);
		else 
			stopBeepAcktion(LOWPOWER_BEEP);
		RCLowPowerOld = RCLowPower;
		MFLowPowerOld = MFLowPower;
	}
	if(MFLowPower==true && radioinkConnectStatus()==false)
	{
		MFLowPower = false;
	}
	
	/*����ɷ�����ʾ����*/
	if(isMFCanFly != isMFCanFlyOld)
	{
		if(isMFCanFly == true)
		{
			runBeepAcktion(CANFLY_BEEP);
		}
		isMFCanFlyOld = isMFCanFly;
	}
	/*500ms��ȡһ��MFMsg*/
	if(xTaskGetTickCount() >= getMFMsgTime+500)
	{
		getMFMsgTime = xTaskGetTickCount();
		sendRmotorCmd(CMD_GET_MSG, NULL);
	}
	
	/*ͨ��ʧ��������չģ��˵�ΪNO_MODULE*/
	if(radioinkConnectStatus() == false)
	{
		expModuleMenuSet(NO_MODULE);
	}
}

static uint8_t reReceiveTimes = 3;	/*�ؽ��մ���*/

/*����MSG����*/
void miniFlyMsgACKProcess(atkp_t* p)
{
	msg = *(MiniFlyMsg_t*)(p->data+1);
	if(msg.mpu_selfTest==false || msg.baro_slfTest==false) 
	{
		isMFSelfTestPass = false;
	}
	else
	{
		isMFSelfTestPass = true;
	}
	isMFCanFly = msg.isCanFly;
	MFLowPower = msg.isLowpower;
	moduleID = msg.moduleID;
	moduleID = moduleID;
	expModuleMenuSet(msg.moduleID);
	
	if(isMFCanFly == true)	/*У׼ͨ��֮�����΢��ֵ*/
	{
		if(reReceiveTimes > 0)
		{
			reReceiveTimes--;
			configParam.trim.roll = msg.trimRoll;	/*roll΢��*/
			configParam.trim.pitch = msg.trimPitch;	/*pitch΢��*/
		}
	}else
	{
		reReceiveTimes = 3;	/*�ؽ��մ���*/
	}	
}

/*RCLock����*/
void setRCLock(bool set)
{
	RCLock = set;
}
bool getRCLock(void)
{
	return RCLock;
}

/*trimFlag����*/
void setTrimFlag(bool set)
{
	trimFlag = set;
}
bool getTrimFlag(void)
{
	return trimFlag;
}

/*MFLowPower����*/
void setMFLowPower(bool set)
{
	MFLowPower = set;
}
bool getMFLowPower(void)
{
	return MFLowPower;
}

/*isMFCanFly����*/
void setIsMFCanFly(bool set)
{
	isMFCanFly = set;
}
bool getIsMFCanFly(void)
{
	return isMFCanFly;
}
