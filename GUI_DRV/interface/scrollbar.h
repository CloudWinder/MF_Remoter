#ifndef  __SCROLLBAR_H
#define  __SCROLLBAR_H

#include "gui_config.h"

//����һ������������
typedef  struct
{
 uint32_t x;             
 uint32_t y;
 uint32_t width;
 uint32_t height;

 uint32_t totalitems;  	  	//����Ŀ��
 uint32_t itemsperpage;		//ÿҳ��ʾ����Ŀ��
 uint32_t topitem;			//��˵���Ŀ
 uint32_t scbbarlen;		//��������	
	
}Scrollbar_Typedef;

void test_scrollbar(void);

void GUI_ScrollbarDelete(Scrollbar_Typedef *scbx);
void GUI_Scrollbar_SetPos(Scrollbar_Typedef *scbx);
void GUI_ScrollbarDraw(Scrollbar_Typedef *scbx);


#endif


