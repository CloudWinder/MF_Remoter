#include "menuL2_item.h"
#include "menuL3_item.h"
#include "menu_ui.h"
#include "windows.h"
#include "scrollbar.h"
#include "joystick.h"
#include "keyTask.h"
#include "display.h"
#include "OLED.h"
#include "config_param.h"
#include "24l01.h"
#include "menuL1_item.h"
#include "atkp.h"


/*�˵�����*/
#define  CONTROLMODE_MENU_NUM   3
#define  FLYMODE_MENU_NUM    	2
#define  FLYSPEED_MENU_NUM		3
#define  FLIPENABLE_MENU_NUM	2
#define  LANGUAGE_MENU_NUM    	3
#define	 EXPMODULE_MENU_NUM		4

MenuItem_Typedef  controlmodeMenu[CONTROLMODE_MENU_NUM];
MenuItem_Typedef  flymodeMenu[FLYMODE_MENU_NUM];
MenuItem_Typedef  flyspeedMenu[FLYSPEED_MENU_NUM];
MenuItem_Typedef  flipEnableMenu[FLIPENABLE_MENU_NUM];
MenuItem_Typedef  languageMenu[LANGUAGE_MENU_NUM];
MenuItem_Typedef  expModuleMenu[EXPMODULE_MENU_NUM];

/****** ѡ��ͼ�� ���� ����ʽ ˳��(��λ��ǰ) *****/
const uint8_t radio_selected_img[32]={
0x00,0x00,0x03,0xC0,0x0C,0x30,0x10,0x08,0x11,0x88,0x23,0xC4,0x27,0xE4,0x27,0xE4,
0x23,0xC4,0x11,0x88,0x10,0x08,0x0C,0x30,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
};

/****** ��ѡ��ͼ�� ���� ����ʽ ˳��(��λ��ǰ) *****/
const uint8_t  radio_unselected_img[32]={
0x00,0x00,0x03,0xC0,0x0C,0x30,0x10,0x08,0x10,0x08,0x20,0x04,0x20,0x04,0x20,0x04,
0x20,0x04,0x10,0x08,0x10,0x08,0x0C,0x30,0x03,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
};

const uint8_t* controlmodeLanguage[3][CONTROLMODE_MENU_NUM]={
{
"����ģʽ",
"�ֶ�ģʽ",
"����ģʽ",
},
{
"Althold Mode",
"Manual Mode",
"3Dhold Mode",
},
{
"����ģʽ",
"�ք�ģʽ",
"���cģʽ",
},
};
const uint8_t* flymodeLanguage[3][FLYMODE_MENU_NUM]={
{
"��ͷģʽ",
"��ͷģʽ",
},
{
"Headless",
"X-mode",
},
{
"�o�^ģʽ",
"���^ģʽ",
},
};
const uint8_t* flySpeedLanguage[3][FLYSPEED_MENU_NUM]={
{
"����ģʽ",
"����ģʽ",
"����ģʽ",
},
{
"Low speed",
"Medium speed",
"High speed",
},
{
"����ģʽ",
"����ģʽ",
"����ģʽ",
}	
};
const uint8_t* flipEnableLanguage[3][FLIPENABLE_MENU_NUM]={
{
"����",
"�ر�",
},
{
"Enable",
"Disable",
},
{
"�_��",
"�P�]",
}	
};
const uint8_t* languageChoiceLanguage[3][LANGUAGE_MENU_NUM]={
{
"��������",
"English",
"��������"
},
{
"��������",
"English",
"��������"
},
{
"��������",
"English",
"��������"
}	
};
const uint8_t* expModuleLanguage[3][EXPMODULE_MENU_NUM]={
{
" δ����ģ�飡",
"1.RGB�ƻ�",
"1.����ͷģ��",
"1.����ģ��",
},
{
"No mudule found!",
"1.RGB_LED Ring",
"1.Camera module",
"1.opFlow module",
},
{
" δ�l�Fģ�K��",
"1.RGB���h",
"1.�z���^ģ�K",
"1.����ģ�K",
}	
};

static void setCtrlmodeAlthold(void)
{
	exitMenu();
	configParam.flight.ctrl = ALTHOLD_MODE;
}

static void setCtrlmodeManual(void)
{
	exitMenu();
	configParam.flight.ctrl = MANUAL_MODE;
}

static void setCtrlmode3Dhold(void)
{
	exitMenu();
	configParam.flight.ctrl = THREEHOLD_MODE;
}

/*��ʼ������ģʽ�˵�*/
void controlmodeMenuInit(void)
{	
	for(int i=0; i<CONTROLMODE_MENU_NUM; i++)
	{
		controlmodeMenu[i].menuItemCount = CONTROLMODE_MENU_NUM;
		controlmodeMenu[i].isSelect = false;
		controlmodeMenu[i].icoSelected = radio_selected_img;
		controlmodeMenu[i].icoUnselected = radio_unselected_img;
		controlmodeMenu[i].title = controlmodeLanguage[configParam.language][i];
		controlmodeMenu[i].parentMenu = mainMenu;
		controlmodeMenu[i].childrenMenu = NULL;
	}
	controlmodeMenu[0].Function = setCtrlmodeAlthold;
	controlmodeMenu[1].Function = setCtrlmodeManual;
	controlmodeMenu[2].Function = setCtrlmode3Dhold;
	switch(configParam.flight.ctrl)
	{
		case ALTHOLD_MODE:
			controlmodeMenu[0].isSelect = true;
			break;
		case MANUAL_MODE:
			controlmodeMenu[1].isSelect = true;
			break;
		case THREEHOLD_MODE:
			controlmodeMenu[2].isSelect = true;
	}
}

static void setFlymodeHeadless(void)
{
	exitMenu();
	configParam.flight.mode = HEAD_LESS;
}

static void setFlymodeXmode(void)
{
	exitMenu();
	configParam.flight.mode = X_MODE;
}

/*��ʼ������ģʽ�˵�*/
void flymodeMenuInit(void)
{	
	for(int i=0; i<FLYMODE_MENU_NUM; i++)
	{
		flymodeMenu[i].menuItemCount = FLYMODE_MENU_NUM;
		flymodeMenu[i].isSelect = false;
		flymodeMenu[i].icoSelected = radio_selected_img;
		flymodeMenu[i].icoUnselected = radio_unselected_img;
		flymodeMenu[i].title = flymodeLanguage[configParam.language][i];
		flymodeMenu[i].Function = NULL;
		flymodeMenu[i].parentMenu = mainMenu;
		flymodeMenu[i].childrenMenu = NULL;
	}
	flymodeMenu[0].Function = setFlymodeHeadless;
	flymodeMenu[1].Function = setFlymodeXmode;
	switch(configParam.flight.mode)
	{
		case HEAD_LESS:
			flymodeMenu[0].isSelect = true;
			break;
		case X_MODE:
			flymodeMenu[1].isSelect = true;
			break;
	}
}

static void setFlightLowSpeed(void)
{
	exitMenu();
	configParam.flight.speed = LOW_SPEED;
}

static void setFlightMidSpeed(void)
{
	exitMenu();
	configParam.flight.speed = MID_SPEED;
}

static void setFlightHighSpeed(void)
{
	exitMenu();
	configParam.flight.speed = HIGH_SPEED;
}

/*��ʼ�������ٶȲ˵�*/
void flySpeedMenuInit(void)
{	
	for(int i=0; i<FLYSPEED_MENU_NUM; i++)
	{
		flyspeedMenu[i].menuItemCount = FLYSPEED_MENU_NUM;
		flyspeedMenu[i].isSelect = false;
		flyspeedMenu[i].icoSelected = radio_selected_img;
		flyspeedMenu[i].icoUnselected = radio_unselected_img;
		flyspeedMenu[i].title = flySpeedLanguage[configParam.language][i];
		flyspeedMenu[i].Function = NULL;
		flyspeedMenu[i].parentMenu = mainMenu;
		flyspeedMenu[i].childrenMenu = NULL;
	}
	flyspeedMenu[0].Function = setFlightLowSpeed;
	flyspeedMenu[1].Function = setFlightMidSpeed;
	flyspeedMenu[2].Function = setFlightHighSpeed;
	switch(configParam.flight.speed)
	{
		case LOW_SPEED:
			flyspeedMenu[0].isSelect = true;
			break;
		case MID_SPEED:
			flyspeedMenu[1].isSelect = true;
			break;
		case HIGH_SPEED:
			flyspeedMenu[2].isSelect = true;
			break;
	}
}

static void setFlipEnable(void)
{
	exitMenu();
	configParam.flight.flip = FLIP_ENABLE;
}

static void setFlipDisable(void)
{
	exitMenu();
	configParam.flight.flip = FLIP_DISABLE;
}

/*��ʼ������ʹ�ܲ˵�*/
void flipEnableMenuInit(void)
{	
	for(int i=0; i<FLIPENABLE_MENU_NUM; i++)
	{
		flipEnableMenu[i].menuItemCount = FLIPENABLE_MENU_NUM;
		flipEnableMenu[i].isSelect = false;
		flipEnableMenu[i].icoSelected = radio_selected_img;
		flipEnableMenu[i].icoUnselected = radio_unselected_img;
		flipEnableMenu[i].title = flipEnableLanguage[configParam.language][i];
		flipEnableMenu[i].parentMenu = mainMenu;
		flipEnableMenu[i].childrenMenu = NULL;
	}
	flipEnableMenu[0].Function = setFlipEnable;
	flipEnableMenu[1].Function = setFlipDisable;
	switch(configParam.flight.flip)
	{
		case FLIP_ENABLE:
			flipEnableMenu[0].isSelect = true;
			break;
		case FLIP_DISABLE:
			flipEnableMenu[1].isSelect = true;
			break;
	}
}


static void setLanguageSimpleChinese(void)
{
	exitMenu();
	configParam.language = SIMPLE_CHINESE;
	mainMenuInit();
	flymodeMenuInit();
	controlmodeMenuInit();
	languageMenuInit();
	flySpeedMenuInit();
	flipEnableMenuInit();
	expModuleMenuInit();
	ledringMenuInit();
	cameraMenuInit();
	vl53lxxMenuInit();
}

static void setLanguageEnglish(void)
{
	exitMenu();
	configParam.language = ENGLISH;
	mainMenuInit();
	flymodeMenuInit();
	controlmodeMenuInit();
	languageMenuInit();
	flySpeedMenuInit();
	flipEnableMenuInit();
	expModuleMenuInit();
	ledringMenuInit();
	cameraMenuInit();
	vl53lxxMenuInit();
}

static void setLanguageComplexChinese(void)
{
	exitMenu();
	configParam.language = COMPLEX_CHINESE;
	mainMenuInit();
	flymodeMenuInit();
	controlmodeMenuInit();
	languageMenuInit();
	flySpeedMenuInit();
	flipEnableMenuInit();
	expModuleMenuInit();
	ledringMenuInit();
	cameraMenuInit();
	vl53lxxMenuInit();
}

/*��ʼ������ѡ��˵�*/
void languageMenuInit(void)
{	
	for(int i=0; i<LANGUAGE_MENU_NUM; i++)
	{
		languageMenu[i].menuItemCount = LANGUAGE_MENU_NUM;
		languageMenu[i].isSelect = false;
		languageMenu[i].icoSelected = radio_selected_img;
		languageMenu[i].icoUnselected = radio_unselected_img;
		languageMenu[i].title = languageChoiceLanguage[configParam.language][i];
		languageMenu[i].parentMenu = mainMenu;
		languageMenu[i].childrenMenu = NULL;
	}
	languageMenu[0].Function = setLanguageSimpleChinese;
	languageMenu[1].Function = setLanguageEnglish;
	languageMenu[2].Function = setLanguageComplexChinese;
	
	switch(configParam.language)
	{
		case SIMPLE_CHINESE:
			languageMenu[0].isSelect = true;
			break;
		case ENGLISH:
			languageMenu[1].isSelect = true;
			break;
		case COMPLEX_CHINESE:
			languageMenu[2].isSelect = true;
	}
}

/*��ʼ����չģ��˵�*/
void expModuleMenuInit(void)
{	
	for(int i=0; i<EXPMODULE_MENU_NUM; i++)
	{
		expModuleMenu[i].menuItemCount = 1;
		expModuleMenu[i].isSelect = true;
		expModuleMenu[i].icoSelected = finger_img;
		expModuleMenu[i].icoUnselected = NULL;
		expModuleMenu[i].title = expModuleLanguage[configParam.language][i];
		expModuleMenu[i].Function = NULL;
		expModuleMenu[i].parentMenu = mainMenu;
		expModuleMenu[i].childrenMenu = NULL;
	}
	expModuleMenu[1].childrenMenu = ledringMenu;
	expModuleMenu[1].Function = gotoNextMenu;
	
	expModuleMenu[2].childrenMenu = cameraMenu;
	expModuleMenu[2].Function = gotoNextMenu;
	
	expModuleMenu[3].childrenMenu = opflowMenu;
	expModuleMenu[3].Function = gotoNextMenu;
}
