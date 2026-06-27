
#ifndef  GUI_CONFIG_H
#define  GUI_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef NULL
	#define NULL 0
#endif


extern uint8_t const  DCB2HEX_TAB[8];


/* ������ɫ��������(���������ݽṹ) */
#define  TCOLOR	  uint8_t 					//�����޸ģ�ԭ����	uint8_t_t 

/* ����LCM�������� */
#define  GUI_LCM_XMAX		128				/* ����Һ��x��������� */
#define  GUI_LCM_YMAX		64				/* ����Һ��y��������� */

/* ����ǰ��ɫ������ɫ����������ASCII�롢���֡����ڡ���ɫλͼ��ʾ */
extern TCOLOR	disp_color;
extern TCOLOR	back_color;

/****************************************************************************
* ���ƣ�GUI_CmpColor()
* ���ܣ��ж���ɫֵ�Ƿ�һ�¡�
* ��ڲ�����color1		��ɫֵ1
*		   color2		��ɫֵ2
* ���ڲ���������1��ʾ��ͬ������0��ʾ����ͬ��
* ˵����������ɫ����TCOLOR�����ǽṹ���ͣ�������Ҫ�û���д�ȽϺ�����
****************************************************************************/
#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )

/****************************************************************************
* ���ƣ�GUI_CopyColor()
* ���ܣ���ɫֵ���ơ�
* ��ڲ�����color1		Ŀ����ɫ����
*		   color2		Դ��ɫ����
* ���ڲ�������
* ˵����������ɫ����TCOLOR�����ǽṹ���ͣ�������Ҫ�û���д���ƺ�����
****************************************************************************/
#define  GUI_CopyColor(color1, color2) 	*color1 = color2


/* �п��ȵ�ֱ�ߺ��� */
#define   GUI_LineWith_EN		0
/* ��Բ���� */
#define   GUI_CircleX_EN		1
/* ��Բ���� */
#define   GUI_EllipseX_EN		0
/* ��亯�� */
#define   GUI_FloodFill_EN		1
/* ���ϼ����½ڵ����(��亯��) */
#define   DOWNP_N				20
#define   UPP_N					20
/* Բ������ */
#define   GUI_ArcX_EN			1
/* ���κ��� */
#define   GUI_Pieslice_EN		0


/* ���ڹ��� */
#define   GUI_WINDOW_EN			1

/* ͼ��˵� */
#define  GUI_MenuIco_EN			0	

/* �����˵� */
#define  GUI_MenuDown_EN		0


/* 5*7���� */
#define  FONT5x7_EN				0

/* 6*12����*/
#define  FONT6x12_EN     		0

/* 8*8���� */
#define  FONT8x8_EN				0

/* 8*16���� */
#define  FONT8x16_EN			0

/* 12*12����*/
#define  FONT12x12_EN    		0

/* 12*24����*/
#define  FONT12x24_EN     		0

/* 16*32���� */
#define  FONT16x32_EN			0

/* 14*56����*/
#define  FONT14x40_EN     		0

/* 24*32���� */
#define  FONT24x24_EN			0

/* 24*48���� */
#define  FONT24x48_EN			0

/* 24*54����*/
#define  FONT24x54_EN     		0

/* ������ʾ���� */
#define  GUI_PutHZ_EN			0


/* ��ɫͼ����ʾ���� */
#define	 GUI_LoadPic_EN			0

/* ��ɫת�� */
#define  CONVERTCOLOR_EN		0


#endif
