#ifndef __DISPLAY_H
#define __DISPLAY_H


//��������
enum ui_e
{
	MAIN_UI,
	TRIM_UI,
	MENU_UI,
	DEBUG_UI,
	JOYSTICK_CALIB_UI,
	MATCH_UI,
	RESET_UI,
};

void setShow_ui(enum ui_e ui);
void displayTask(void* param);
void displayInit(void);


#endif /*__DISPLAY_H*/
