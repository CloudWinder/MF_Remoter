#ifndef __MENUL3_ITEM_H
#define __MENUL3_ITEM_H
#include <stdint.h>
#include "gui_menu.h"


extern MenuItem_Typedef  ledringMenu[];
extern MenuItem_Typedef  cameraMenu[];
extern MenuItem_Typedef  opflowMenu[];

void ledringMenuInit(void);
void cameraMenuInit(void);
void vl53lxxMenuInit(void);

#endif /*__MENUL3_ITEM_H*/

