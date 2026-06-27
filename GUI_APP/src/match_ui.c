#include <stdio.h>
#include <string.h>
#include "match_ui.h"
#include "display.h"
#include "keyTask.h"
#include "atkp.h"
#include "oled.h"
#include "joystick.h"
#include "remoter_ctrl.h"
#include "radiolink.h"
#include "beep.h"
#include "main_ui.h"
#include "atkp.h"
#include "24l01.h"
#include "config_param.h"
#include "text.h"
#include "font.h"

/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"


static enum matchStep 
{
	CHECK_ADDR,
	GET_CONFIG_COMMANDER,
	SET_CONFIG_COMMANDER,
	SET_REMOTOR_CONFIG,
	CHECK_STATUS,
}step = CHECK_ADDR;

static bool isConfigRecv = false;
static radioConfig_t radio;


const uint8_t* remoterLanguage[3] = 
{
"ң��",
"   R",
"�b��",
};

const uint8_t* miniFlyLanguage[3] = 
{
"����",
"F",
"���S",
};

const uint8_t* noteMsgLanguage[3][4] = 
{
{
"��ʾ����ƥ�䣡",
"��ʾ��ƥ���С���",
"  ͨ��ʧ�ܣ���������ң�غ�������ƥ�䡣",
},
{
"NOTE: Already matched!",
"NOTE: Matching.....",
"Connect fail!Please reset remoter and MiniFly before matching.",
},
{
"��ʾ����ƥ�䣡",
"��ʾ��ƥ���С���",
"  ͨ��ʧ����Ո�������b�غ����S��ƥ�䡣",
},
};

const uint8_t* resultLanguage[3][2] = 
{
{
"ƥ��ɹ���",
"ƥ��ʧ�ܣ�",
},
{
"Match Success",
"Match Fail",
},
{
"ƥ��ɹ���",
"ƥ��ʧ����",
},
};

/*��ȡ��������ָ��*/
static void getRadioConfigCommander(void)
{
	atkp_t p;
	p.msgID = DOWN_RADIO;
	p.dataLen = 1;
	p.data[0] = D_RADIO_GET_CONFIG;
	radiolinkSendPacket(&p);
}
/*������������ָ��*/
static void setRadioConfigCommander(radioConfig_t* set)
{
	atkp_t p;
	p.msgID = DOWN_RADIO;
	p.dataLen = sizeof(radioConfig_t)+1;
	p.data[0] = D_RADIO_SET_CONFIG;
	memcpy(&p.data[1], set, sizeof(radioConfig_t));
	nrf_sendPacketNoACK((uint8_t*)&p, p.dataLen+2);
}

bool isRadioAddrDefault(void)
{
	return(	configParam.radio.addressHigh == ((uint64_t)RADIO_ADDRESS>>32) &&
			configParam.radio.addressLow == (RADIO_ADDRESS & 0xFFFFFFFFULL));
}

void setMatchRadioConfig(radioConfig_t* set)
{
	isConfigRecv = true;
	radio = *set;
}

void match_ui(void)
{
	static bool startMatch = false;
	static uint8_t timeOut;
	uint64_t addr;
	
	show_str(0,0,remoterLanguage[configParam.language],24,24,1);
	oled_showPicture(48,0,bmp_remoter2fly,32,24);
	show_str(78,0,miniFlyLanguage[configParam.language],24,24,1);
	switch(step)
	{
		case CHECK_ADDR:
			if(radioinkConnectStatus() == true)
			{
				if(isRadioAddrDefault()==true)
				{
					show_str(0,28,noteMsgLanguage[configParam.language][1],12,12,1);
					step = GET_CONFIG_COMMANDER;
					startMatch = true;
				}
				else
				{
					show_str(0,28,noteMsgLanguage[configParam.language][0],12,12,1);
				}
			}
			else
			{
				show_str(0,28,noteMsgLanguage[configParam.language][2],12,12,1);
			}
			break;

		case GET_CONFIG_COMMANDER:
			if(isConfigRecv == true)
			{
				isConfigRecv = false;
				step = SET_CONFIG_COMMANDER;
			}
			else
			{
				getRadioConfigCommander();
			}
			break;
			
		case SET_CONFIG_COMMANDER:
			radiolinkEnable(DISABLE);
			setRadioConfigCommander(&radio);
			radiolinkEnable(ENABLE);
			step = SET_REMOTOR_CONFIG;
			break;
			
		case SET_REMOTOR_CONFIG:
			nrf_setChannel(radio.channel);
			nrf_setDataRate(radio.dataRate);
			addr = (uint64_t)radio.addressHigh<<32 | radio.addressLow;
			nrf_setAddress(addr);
			step = CHECK_STATUS;
			break;
		
		case CHECK_STATUS:
			if(radioinkConnectStatus()==true)
			{
				startMatch = false;
				configParam.radio = radio;
				writeConfigParamToFlash();
				show_str_mid(0,51,resultLanguage[configParam.language][0],12,12,1,128);
			}
			break;
	}
	
	timeOut++;
	
	if(startMatch==true && timeOut>10)
	{
		startMatch = false;
		show_str_mid(0,51,resultLanguage[configParam.language][1],12,12,1,128);
	}
	if(timeOut>30)
	{
		step = CHECK_ADDR;
		timeOut = 0;
		setShow_ui(MAIN_UI);
	}
}


