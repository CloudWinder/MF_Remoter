#ifndef __MENUL1_ITEM_H
#define __MENUL1_ITEM_H
#include "stm32f1xx_hal.h"
#include "gui_menu.h"
#include "remoter_ctrl.h"


extern MenuItem_Typedef  mainMenu[];
extern const uint8_t finger_img[32];

void mainMenuInit(void);
void expModuleMenuSet(enum expModuleID id);

#endif /*__MENUL1_ITEM_H*/
