#ifndef  MENU_H
#define  MENU_H
#include <stdbool.h>
#include "gui_config.h"

//����ͼ��˵�32X32
typedef struct MenuItem
{  	
	int8_t cursorPosition;			//�˵���ʾʱ���λ��
	uint8_t menuItemCount;			//�˵�������Ŀ��
	bool isSelect;					//��ʾ�˵���״̬ �Ƿ�ѡ��
	const uint8_t *icoSelected;		//�˵���Ŀѡ�е�ICO���ݵ�ַ
	const uint8_t *icoUnselected;	//�˵���Ŀûѡ�е�ICO���ݵ�ַ
	const uint8_t *title;			//�˵���Ŀ����
	void (*Function)(void); 		//�˵���Ӧ�Ķ���
	struct MenuItem* parentMenu;	//��һ���˵�
	struct MenuItem* childrenMenu;	//��һ���˵�
}MenuItem_Typedef;


uint8_t  GUI_MenuIcoDraw(MenuItem_Typedef *ico);
void GUI_MenuItem_Selected(uint32_t x, uint32_t y, MenuItem_Typedef *item, bool isSelect);
uint8_t  GUI_MenuItemDraw(uint32_t x, uint32_t y, MenuItem_Typedef *item);


#endif


