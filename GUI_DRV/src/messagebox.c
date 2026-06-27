#include "messagebox.h"
#include "gui_basic.h"
#include "string.h"
#include "button.h"
#include "stm32f1xx_hal.h"
#include "text.h"

//�õ��ַ����ĳ���
uint16_t getMessageBoxStringlen(uint8_t *str)
{
	uint16_t strlenth=0;
	strlenth=strlen((const char*)str);//�ֽ���
	strlenth*=6;//һ���ֽ�ˮƽ6����(12x12����)
	return strlenth;
}

Button_Typedef btn_ok_messagebox={
20,
20,
48,
15,
"OK",
1
};

Button_Typedef btn_cancel_messagebox={
20,
20,
48,
15,
"cancel",
1
};


//����Ϣ��
uint8_t GUI_MessageBoxDraw(MessageBox_Typedef* messagebox)
{
	/* �������ˣ������������Χ���򷵻�0 */
	if( ( (messagebox->with)<20 ) || ( (messagebox->hight)<20 ) ) return(0);// ���ȡ��߶ȼ�飬������С����
	if( (messagebox->x + messagebox->with ) > GUI_LCM_XMAX ) return(0);		// ���ڿ����Ƿ����
	if( (messagebox->y + messagebox->hight ) > GUI_LCM_YMAX ) return(0);	// ���ڸ߶��Ƿ����

	/* ��ʼ������ */
	GUI_RectangleFill(messagebox->x, messagebox->y, messagebox->x + messagebox->with - 1, messagebox->y + messagebox->hight - 1, 0);//����
	GUI_Rectangle(messagebox->x, messagebox->y, messagebox->x + messagebox->with - 1, messagebox->y + messagebox->hight - 1, 1);// ������(�߿�)
	GUI_HLine(messagebox->x, messagebox->y + 14, messagebox->x + messagebox->with - 1, disp_color);		// �������±߿�	 

	//��������
	GUI_RectangleFill(messagebox->x, messagebox->y, messagebox->x + messagebox->with - 1, messagebox->y + 14-1, 1);
	if(getMessageBoxStringlen(messagebox->title)>(messagebox->with-2))
	{  	
		return 0;//��ʾ�ı��ⳬ���˴���Ŀ���
	} 
	//��ʾ����
	show_str_mid(messagebox->x, messagebox->y+1,messagebox->title,12,12,0,messagebox->with);


	//ȷ����ť
	btn_ok_messagebox.state = 0;
	btn_ok_messagebox.title = messagebox->ok;
	btn_ok_messagebox.x = messagebox->x+5;
	btn_ok_messagebox.y = messagebox->y+messagebox->hight-1 -btn_ok_messagebox.height -3;
	GUI_DrawButton(&btn_ok_messagebox);

	//ȡ����ť	  
	btn_cancel_messagebox.state = 1;
	btn_cancel_messagebox.title = messagebox->cancel;
	btn_cancel_messagebox.x = messagebox->x+ messagebox->with -btn_cancel_messagebox.width-1-5;
	btn_cancel_messagebox.y = messagebox->y+messagebox->hight-1 -btn_cancel_messagebox.height-3;	 
	GUI_DrawButton(&btn_cancel_messagebox); 

	//��ʾ��Ϣ
	show_str(messagebox->x+6, messagebox->y+1+16,messagebox->msg, 12,12,1);
	return(1);
}

//���ð�ť״̬
void GUI_MessageBoxButtonStatus(MessageBox_Typedef* messagebox, uint8_t status)
{	
	uint8_t ok_status;
	uint8_t cancel_status;

	if(status)
	{
		ok_status =1;
		cancel_status  =0;			
	}else
	{
		ok_status =0;
		cancel_status  =1;	
	}
	//ȷ����ť
	GUI_Button_Selected(&btn_ok_messagebox,ok_status);

	//ȡ����ť	  
	GUI_Button_Selected(&btn_cancel_messagebox,cancel_status);

}

//�õ���ǰѡ����
uint8_t GUI_MessageBoxResult(MessageBox_Typedef* messagebox)
{
	return btn_ok_messagebox.state;	
}

//ɾ����Ϣ��
uint8_t GUI_MessageBoxDelete(MessageBox_Typedef* messagebox)
{	
   if( ( (messagebox->with)<20 ) || ( (messagebox->hight)<20 ) ) return(0);		// ���ȡ��߶ȼ�飬������С����
   if( (messagebox->x + messagebox->with ) > GUI_LCM_XMAX ) return(0);			// ���ڿ����Ƿ����
   if( (messagebox->y + messagebox->hight ) > GUI_LCM_YMAX ) return(0);			// ���ڸ߶��Ƿ����
    
   /* �������� */
   GUI_RectangleFill(messagebox->x, messagebox->y, messagebox->x + messagebox->with - 1, messagebox->y + messagebox->hight - 1, back_color);
   return(1);
}

//MessageBox_Typedef MessageBox={
//0,
//0,	
//128,
//64,
//"reset",	
//"system will be resetni hao a1234?",
//"OK",
//"Cancel"
//};


////������Ϣ��
//void test_MessageBox(void)
//{
// GUI_MessageBoxDraw(&MessageBox);
// GUI_Refresh();
// delay_ms(2000);
// GUI_MessageBoxButtonStatus(&MessageBox, 1);	
// GUI_Refresh();
// delay_ms(2000);
// GUI_MessageBoxButtonStatus(&MessageBox, 0);		
// GUI_Refresh();
// delay_ms(2000);	
//GUI_MessageBoxDelete(&MessageBox);	
//	
//	
//}


