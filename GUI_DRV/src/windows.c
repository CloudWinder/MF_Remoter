
#include "windows.h"
#include "gui_basic.h"
#include "string.h"
#include "text.h"

#if  GUI_WINDOW_EN==1

//�õ��ַ����ĳ���
uint16_t getWindowStringlen(const unsigned char *str)
{
	uint16_t strlenth=0;
	strlenth=strlen((const char*)str);
	strlenth*=6;

	return strlenth;
}

/****************************************************************************
* ���ƣ�GUI_WindowsDraw()
* ���ܣ���ʾ���ڡ������ṩ�Ĵ��ڲ������л����ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
uint8_t  GUI_WindowsDraw(WINDOWS *win)
{  
	/* �������ˣ������������Χ���򷵻�0 */
	if( ( (win->width)<20 ) || ( (win->height)<20 ) ) return(0);	// ���ȡ��߶ȼ�飬������С����
	if( (win->x + win->width ) > GUI_LCM_XMAX ) return(0);			// ���ڿ����Ƿ����
	if( (win->y + win->height ) > GUI_LCM_YMAX ) return(0);			// ���ڸ߶��Ƿ����

	/* ��ʼ������ */
	GUI_RectangleFill(win->x, win->y, win->x + win->width - 1, win->y + win->height - 1, 0);// �����겻������ô���ڣ�
	GUI_Rectangle(win->x, win->y, win->x + win->width - 1, win->y + win->height - 1, 1);	// �����ڱ߿�
	GUI_HLine(win->x, win->y + 14, win->x + win->width - 1, disp_color);					// ������Ŀ���±߿�	 

	//��������
	GUI_RectangleFill(win->x, win->y, win->x + win->width - 1, win->y + 14-1, 1);
	if(getWindowStringlen(win->title)>(win->width-2))
	{  
		return 0;//��ʾ�ı��ⳬ���˴���Ŀ���
	}	
	show_str_mid(win->x, win->y+1, win->title,12,12,0,win->width);//��ʾ����
	return(1);
}


/****************************************************************************
* ���ƣ�GUI_WindowsHide()
* ���ܣ��������ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ���������0��ʾ����ʧ�ܣ�����1��ʾ�����ɹ�
****************************************************************************/
uint8_t GUI_WindowsDelete(WINDOWS *win)
{  
	/* �������ˣ������������Χ���򷵻�0 */
	if( ( (win->width)<20 ) || ( (win->height)<20 ) ) return(0);	// ���ȡ��߶ȼ�飬������С����
	if( (win->x + win->width ) > GUI_LCM_XMAX ) return(0);			// ���ڿ����Ƿ����
	if( (win->y + win->height ) > GUI_LCM_YMAX ) return(0);			// ���ڸ߶��Ƿ����

	/* �������� */
	GUI_RectangleFill(win->x, win->y, win->x + win->width - 1, win->y + win->height - 1, back_color);
	return(1);
}


/****************************************************************************
* ���ƣ�GUI_WindowsClr()
* ���ܣ��������ڡ�
* ��ڲ�����win		���ھ��
* ���ڲ�������
* ˵����ʹ�ô˺���ǰҪ��ʹ��GUI_WindowsDraw()�����ڻ�����
****************************************************************************/
void GUI_WindowsClr(WINDOWS *win)
{ 
	uint8_t  x0, y0;
	uint8_t  x1, y1;

	/* ����Ҫ���������� */
	x0 = win->x + 1;
	x1 = win->x + win->width - 2;
	y0 = win->y + 14;
	y1 = win->y + win->height - 2;

	/* ʹ��������ʵ������ */
	GUI_RectangleFill(x0, y0, x1, y1, back_color);
}


WINDOWS winxp=
{
.x = 0,
.y = 0,	
.width = 128,
.height = 64,
.itemsperpage = 3,
.topitem = 0,
.title = "XBOS",
};

//���Դ���
void test_window(void)
{
	GUI_WindowsDraw(&winxp);	
}


#endif


