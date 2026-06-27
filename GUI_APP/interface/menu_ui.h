#ifndef __GUI_MENU_H
#define __GUI_MENU_H
#include <stdint.h>
#include "gui_basic.h"
#include "gui_menu.h"
#include "windows.h"
#include "scrollbar.h"


extern WINDOWS MenuWindow;
extern Scrollbar_Typedef MenuScrollbar;

void menu_test(void);
void Menu_Run(void);//���в˵�
void MenuHookFun(uint8_t key);//���Ӻ���
void exitMenu(void);
void gotoNextMenu(void);
void gotoLastMenu(void);


#endif /*__GUI_MENU_H*/
