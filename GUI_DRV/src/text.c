#include <string.h>
#include "text.h"	
#include "oled.h"
#include "font.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//������ʾ��������	   
//��������:2015/1/20
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
 
#define  LCD_WIDTH		128
#define  LCD_HIGH		64

 const char* font1212_table = {
"����ģʽ�����ٶȷ���ʹ��ҡ��У׼\
ƥ����������ѡ�������˳���չ���w\
�L�u�U���S�Z�x��U�K�����ֶ����\
�c��ͷ�Пo�^���п����ر��_���P�]\
�����ķ�δ���֣��ƻ���������l�F\
���h�z����һȦ�ɂ����²����ֻ���\
����ʾ���ѡ�ͨ��ʧ������ң���١�\
��Ո�b��ȷ�ָ�ΪĬ���裿�_�͞��J\
�Oȡ����ɫ������̬��Ӧ������˸��\
����Ͳ����ˮyԇ�B���W�q��늼�"
};
 
 const char* font2424_table = {
"����ң�b���S"
};

//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//f_w:�ֿ�
//f_h:�ָ�
//mode:0,������ʾ,1,������ʾ
static void show_font(uint16_t x,uint16_t y,const uint8_t *font,uint8_t f_w,uint8_t f_h,uint8_t mode)
{
	const char* font_table;
	uint16_t fontSeq;
	uint8_t temp,t,t1;
	uint16_t y0=y; 
	uint8_t csize=(f_h/8+((f_h%8)?1:0))*f_w;//�õ����ɷֱ��ַ���ռ���ֽ��� 

	if(f_w==12 && f_h==12)
		font_table = font1212_table;
	else if(f_w==24 && f_h==24)
		font_table = font2424_table;
	else return;/*û�е��ֿ�*/
	for(fontSeq=0; fontSeq<strlen(font_table)/2; fontSeq++)/*����font_table��Ӧ�ֿ�������±�*/
	{
		if(font_table[2*fontSeq]==font[0] && font_table[2*fontSeq+1]==font[1])
			break;
	}
	if(fontSeq >= strlen(font_table)/2) return;/*font_table��û��font����*/

	for(t=0;t<csize;t++)
	{   												   
		if(f_w==12 && f_h==12)
			temp = font_1212[fontSeq][t];/*����font_1212�ֿ�*/
		else if(f_w==24 && f_h==24)
			temp = font_2424[fontSeq][t];/*����font_2424�ֿ�*/
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)oled_drawPoint(x,y,mode);
			else oled_drawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==f_h)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}  
}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//f_w:�ֿ�
//f_h:�ָ�
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void show_str(uint16_t x,uint16_t y,const uint8_t*str,uint8_t f_w,uint8_t f_h,uint8_t mode)
{					
	uint16_t x0=x;
	uint16_t y0=y;							  	  
    uint8_t bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//���� 
	        else               //�ַ�
	        {      
                if(x>(x0+LCD_WIDTH-f_h/2))//����
				{				   
					y+=f_h;
					x=x0;	   
				}							    
		        if(y>(y0+LCD_HIGH-f_h))break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=f_h;
					x=x0;
		            str++; 
		        }  
		        else oled_showChar(x,y,*str,f_w/2,f_h,mode);//��Ч����д�� 
				str++; 
		        x+=f_h/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }else//���� 
        {     
            bHz=0;//�к��ֿ�    
            if(x>(x0+LCD_WIDTH-f_h))//����
			{	    
				y+=f_h+2;//2Ϊ�м��
				x=x0;		  
			}
	        if(y>(y0+LCD_HIGH-f_h))break;//Խ�緵��  						     
	        show_font(x,y,str,f_w,f_h,mode); //��ʾ�������,������ʾ 
	        str+=2; 
	        x+=f_h;//��һ������ƫ��	    
        }						 
    }   
}  			 		 
//��ָ�����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ���			  
void show_str_mid(uint16_t x,uint16_t y,const uint8_t*str,uint8_t f_w,uint8_t f_h,uint8_t mode,uint16_t len)
{
	uint16_t strlenth=0;
   	strlenth=strlen((char*)str);
	strlenth*=f_h/2;
	if(strlenth>len)show_str(x,y,str,f_w,f_h,mode);
	else
	{
		strlenth=(len-strlenth)/2;
	    show_str(strlenth+x,y,str,f_w,f_h,mode);
	}
}   

