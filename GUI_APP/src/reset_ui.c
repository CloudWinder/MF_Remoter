#include "reset_ui.h"
#include "windows.h"
#include "scrollbar.h"
#include "joystick.h"
#include "keyTask.h"
#include "display.h"
#include "messagebox.h"
#include "config_param.h"


static bool isInit;

//���öԻ���
MessageBox_Typedef ResetSystemMessageBox={
0,
0,	
128,
64,
"Reset",	
"System will be reset?",
"OK",
"Cancel"
};

const uint8_t *resetSystemLanguage[4][3]={
{
"����",
"Reset",
"����",
},
{
"ȷ���ָ�ΪĬ�����ã�",
"Restore the default Settings?",
"�_���֏͞�Ĭ�J�O�ã�",
},
{
"ȷ��",
"OK",
"�_��",
},
{
"ȡ��",
"Cancel",
"ȡ��",
}
};

/*��ʾ���öԻ���*/
static void msgboxDispalyInit(void)
{
	if(isInit) return;
	
	ResetSystemMessageBox.title = (uint8_t*)resetSystemLanguage[0][configParam.language];
	ResetSystemMessageBox.msg = (uint8_t*)resetSystemLanguage[1][configParam.language];
	ResetSystemMessageBox.ok = (uint8_t*)resetSystemLanguage[2][configParam.language];
	ResetSystemMessageBox.cancel = (uint8_t*)resetSystemLanguage[3][configParam.language];
	GUI_MessageBoxDraw(&ResetSystemMessageBox);
	
	isInit = true;
}

/*���ý���*/
void reset_ui(void)
{
	enum dir_e joystick2;
	static int timeout;
	
	msgboxDispalyInit();
	joystick2 = getJoystick2Dir(0);
	if(joystick2 != CENTER)
	{
		timeout = 0;
	}
	switch(joystick2)
	{
		case RIGHT:  //ROLL����
			GUI_MessageBoxButtonStatus(&ResetSystemMessageBox, 0);
			break;
			
		case LEFT:  //ROLL����
			GUI_MessageBoxButtonStatus(&ResetSystemMessageBox, 1);
			break;
		default :break;
	}
	
	uint8_t keyState = getKeyState();
	if(keyState == KEY_J2_SHORT_PRESS)//����ҡ�˼�ִ�в˵���Ӧ�Ķ���
	{	
		if(GUI_MessageBoxResult(&ResetSystemMessageBox) == 1)
		{
			configParamReset();
			__ASM volatile("cpsid i");
			NVIC_SystemReset();	/*����*/
		}
		isInit = false;
		setShow_ui(MAIN_UI);
	}
	if(timeout++ > 100)//��ʱ�˳��˵�
	{
		timeout = 0;
		isInit = false;
		setShow_ui(MAIN_UI);
	}
}
