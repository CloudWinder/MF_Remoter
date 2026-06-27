#ifndef __MENUL2_ITEM_H
#define __MENUL2_ITEM_H
#include "stm32f1xx_hal.h"
#include "gui_menu.h"


extern MenuItem_Typedef  controlmodeMenu[];
extern MenuItem_Typedef  flymodeMenu[];
extern MenuItem_Typedef  flyspeedMenu[];
extern MenuItem_Typedef  flipEnableMenu[];
extern MenuItem_Typedef  languageMenu[];
extern MenuItem_Typedef  expModuleMenu[];

extern const uint8_t radio_selected_img[32];
extern const uint8_t radio_unselected_img[32];


void flymodeMenuInit(void);
void controlmodeMenuInit(void);
void languageMenuInit(void);
void flySpeedMenuInit(void);
void flipEnableMenuInit(void);
void expModuleMenuInit(void);

#endif /*__MENUL2_ITEM_H*/

