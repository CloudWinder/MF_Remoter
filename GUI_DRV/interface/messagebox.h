#ifndef __MESSAGEBOX_H
#define __MESSAGEBOX_H

#include "gui_config.h"

/* ���崰�����ݽṹ */
typedef  struct
{  
	uint32_t  x;		// ����λ��(���Ͻǵ�x����)
	uint32_t  y;		// ����λ��(���Ͻǵ�y����)

	uint32_t  with;		// ���ڿ���
	uint32_t  hight;	// ���ڸ߶�

	uint8_t   *title;	// ���������ָ�� 
	uint8_t   *msg;		//��Ϣ
	uint8_t   *ok;		//ȷ��
	uint8_t   *cancel;	//ȡ��
} MessageBox_Typedef;


uint8_t GUI_MessageBoxDelete(MessageBox_Typedef* messagebox);
uint8_t GUI_MessageBoxDraw(MessageBox_Typedef* messagebox);
void GUI_MessageBoxButtonStatus(MessageBox_Typedef* messagebox, uint8_t status);
uint8_t GUI_MessageBoxResult(MessageBox_Typedef* messagebox);
void test_MessageBox(void);


#endif


