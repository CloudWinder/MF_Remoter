#ifndef __USB_LINK_H
#define __USB_LINK_H
#include <stdint.h>
#include <stdbool.h>
#include "atkp.h"


void usblinkInit(void);
bool usblinkSendPacket(const atkp_t *p);
bool usblinkSendPacketBlocking(const atkp_t *p);
bool usblinkReceivePacket(atkp_t *p);
bool usblinkReceivePacketBlocking(atkp_t *p);
void usblinkTxTask(void* param);
void usblinkRxTask(void *param);


#endif /*usblink.h*/

