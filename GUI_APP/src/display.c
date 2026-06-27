#include "display.h"
#include "main_ui.h"
#include "trim_ui.h"
#include "debug_ui.h"
#include "menu_ui.h"
#include "match_ui.h"
#include "joystickCalib_ui.h"
#include "menuL1_item.h"
#include "menuL2_item.h"
#include "menuL3_item.h"
#include "reset_ui.h"
/*FreeRtos includes*/
#include "FreeRTOS.h"
#include "task.h"


static enum ui_e show_ui = MAIN_UI;

/*������ʾ����*/
void setShow_ui(enum ui_e ui)
{
	show_ui = ui;
	GUI_ClearSCR();
}

/*��ʾ����*/
void displayTask(void* param)
{
	while(1)
	{
		vTaskDelay(50);
		switch(show_ui)
		{
			case MAIN_UI:
				main_ui();
				break;
			case TRIM_UI:
				trim_ui();
				break;
			case MENU_UI:
				Menu_Run();
				break;
			case DEBUG_UI:
				debug_ui();
				break;
			case JOYSTICK_CALIB_UI:
				joystickCalib_ui();
				break;
			case MATCH_UI:
				match_ui();
				break;
			case RESET_UI:
				reset_ui();
				break;
			default:break;
		}
		GUI_Refresh();
	}
}

/*������ʾ��ʼ��*/
void displayInit(void)
{
	//�˵���ʼ��
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


