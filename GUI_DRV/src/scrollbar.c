#include "scrollbar.h"
#include "gui_basic.h"
#include "stm32f1xx_hal.h"
#include "oled.h"

#define SCROLL_BTN_WIDTH  10  //��ť�Ŀ���
#define SCROLL_BTN_HEIGHT 10	//��ť�ĸ߶�

//�ϰ�ť
const unsigned char scroll_btn_up[8]={
0x02,0x0E,0x3E,0x7E,0x7E,0x3E,0x0E,0x02,
};

//�°�ť
const unsigned char scroll_btn_down[8]={
0x40,0x70,0x7C,0x7E,0x7E,0x7C,0x70,0x40,
};

//���ù�������λ��
void GUI_Scrollbar_SetPos(Scrollbar_Typedef *scbx)
{	
	uint16_t scbpos;  //������ƫ��
	uint16_t scblen;  //���������Թ����ĳ���
	uint16_t x,y,xlen,ylen;
//	uint8_t scbplen;
//	
//	scbplen = SCROLL_BTN_HEIGHT;  //�õ���ť�ĸ߶�
	
	scblen=scbx->height-2*SCROLL_BTN_HEIGHT-scbx->scbbarlen;//�õ������������ռ�ĳ���
	x=scbx->x+1;                  //��������ʼ����x
	y=scbx->y+SCROLL_BTN_HEIGHT;  //��������ʼ����y
	xlen=scbx->width-2-1;         //���������� -1
	ylen=scblen+scbx->scbbarlen-1;//���������� -1
	
	//���������
	GUI_RectangleFill(x ,y,x+xlen,y+ylen, 0);
	if(scbx->totalitems <= scbx->itemsperpage)//����Ŀ����һҳ��ʾ��
	{
		scbpos=0; //ƫ��0
	}
	else //��ֹһҳ
	{
		scbpos=(scbx->topitem*scblen)/(scbx->totalitems-1);//���㵱ǰ������ƫ�Ƶ�λ��
	}
	
	//����������
	GUI_RectangleFill(x ,y+scbpos,x+xlen,y+scbpos+scbx->scbbarlen, 1);
	//GUI_Refresh();	//ˢ����Ļ	
}


//����һ��������
void GUI_ScrollbarDraw(Scrollbar_Typedef *scbx)
{
	uint32_t scbactlen=0;//���������Թ����Ŀռ䳤��
	if(scbx==NULL) return;
	if(scbx->width<SCROLL_BTN_WIDTH) return;
	if(scbx->height<2*SCROLL_BTN_HEIGHT) return;
	
	//����߿�
	GUI_Rectangle(scbx->x, scbx->y, scbx->x+scbx->width-1, scbx->y+scbx->height-1, 1);
	//������������ť
	oled_showPicture(scbx->x+(scbx->width-8)/2, scbx->y+1, (uint8_t *)scroll_btn_up, 8, 8);
	oled_showPicture(scbx->x+(scbx->width-8)/2, scbx->y+scbx->height-9, (uint8_t *)scroll_btn_down, 8, 8);	
	//����ť�߿�
	GUI_Line(scbx->x, scbx->y+9, scbx->x+scbx->width-1, scbx->y+9,1);
	GUI_Line(scbx->x, scbx->y+scbx->height-10, scbx->x+scbx->width-1, scbx->y+scbx->height-10,1);
	
	
	//�ɹ������������Ŀռ䷶Χ
	scbactlen=scbx->height -2*SCROLL_BTN_HEIGHT;
	
	//ÿҳ������Ŀ��������Ŀ�� ������Ŀ�������ɹ����Ŀؼ�
	if(scbx->itemsperpage >= scbx->totalitems)scbx->scbbarlen=scbactlen;
	//ÿҳ������ĿС������Ŀ�� 
	else scbx->scbbarlen=(scbactlen*scbx->itemsperpage)/scbx->totalitems;//�õ��������ĳ���
	//���ù�������λ��
	GUI_Scrollbar_SetPos(scbx);
}


//ɾ��һ��������
void GUI_ScrollbarDelete(Scrollbar_Typedef *scbx)
{
  GUI_RectangleFill(scbx->x, scbx->y, scbx->x+scbx->width-1, scbx->y+scbx->height-1, 0);

}

Scrollbar_Typedef Scrollbar=
{
118,
14,
10,
50,
	
9,
3,
9,
0
};

//���Թ���������
void test_scrollbar(void){

	GUI_ScrollbarDraw(&Scrollbar);
	GUI_Refresh();								//ˢ����Ļ
	HAL_Delay(2000);
	Scrollbar.topitem = 1;
	GUI_Scrollbar_SetPos(&Scrollbar);
	GUI_Refresh();								//ˢ����Ļ
	HAL_Delay(2000);
//	GUI_ScrollbarDraw(&Scrollbar);
	
}


