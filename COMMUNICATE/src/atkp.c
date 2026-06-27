#include <string.h>
#include "atkp.h"
#include "radiolink.h"
#include "usblink.h"
#include "config_param.h"
#include "trim_ui.h"
#include "beep.h"
#include "main_ui.h"
#include "match_ui.h"
#include "remoter_ctrl.h"

/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"


float plane_yaw,plane_roll,plane_pitch;
float plane_bat;
uint8_t rssi;

/*atkp����*/
static void atkpAnalyze(atkp_t *p)
{
	if(p->msgID == UP_STATUS)
	{
		plane_roll = (int16_t)(*(p->data+0)<<8)|*(p->data+1);
		plane_roll = plane_roll/100;
		plane_pitch = (int16_t)(*(p->data+2)<<8)|*(p->data+3);
		plane_pitch = plane_pitch/100;
		plane_yaw = (int16_t)(*(p->data+4)<<8)|*(p->data+5);
		plane_yaw = plane_yaw/100;
	}
	else if(p->msgID == UP_POWER)
	{
		plane_bat = (int16_t)(*(p->data+0)<<8)|*(p->data+1);
		plane_bat = plane_bat/100;
	}
	else if(p->msgID == UP_REMOTOR)	
	{
		switch(p->data[0])
		{
			case ACK_MSG:
				miniFlyMsgACKProcess(p);
				break;
		}
	}
	else if(p->msgID == UP_RADIO)
	{
		radioConfig_t radio;
		switch(p->data[0])
		{
			case U_RADIO_RSSI:
				rssi = p->data[1];
				break;
			
			case U_RADIO_CONFIG:
				memcpy(&radio, p->data+1, sizeof(radio));
				setMatchRadioConfig(&radio);
				break;
		}
	}
}

/*�����������ݴ�������*/
void radiolinkDataProcessTask(void *param) 
{
	atkp_t p;
	while(1)
	{
		radiolinkReceivePacketBlocking(&p); /*���������ϴ������ݣ������������ݺ�ң��������*/
		atkpAnalyze(&p);
		usblinkSendPacket(&p);	/*�ѽ��յ����������ݷ��͵���λ��*/
		vTaskDelay(1);
	}
}

/*USB�������ݴ�������*/
void usblinkDataProcessTask(void *param)
{
	atkp_t p;
	while(1)
	{
		usblinkReceivePacketBlocking(&p);	/*������λ�����͵�����*/
		atkpAnalyze(&p);
		radiolinkSendPacket(&p);
	}
}

