#include "key.h"

		
//����IO��ʼ������
/*void keyInit(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = KEY_S2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = KEY_S1 | KEY_L;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = KEY_R;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}*/

////������������
////���ذ���ֵ
////mode:0,��֧��������;1,֧��������;
////0��û���κΰ�������
////1��KEY0����
////2��KEY1����
////3��KEY2���� 
////4��KEY3���� WK_UP
////ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
//u8 KEY_Scan(u8 mode)
//{	 
//	static u8 key_up=1;//�������ɿ���־
//	if(mode)key_up=1;  //֧������		  
//	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
//	{
//		delay_ms(10);//ȥ���� 
//		key_up=0;
//		if(KEY0==0)return KEY0_PRES;
//		else if(KEY1==0)return KEY1_PRES;
//		else if(KEY2==0)return KEY2_PRES;
//		else if(WK_UP==1)return WKUP_PRES;
//	}else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	    
// 	return 0;// �ް�������
//}

uint8_t keyScan(void)
{

	uint8_t key_val = 0; //���巵��ֵ����
	/*��ȡ���ŽǶ�״̬*/
	if(	READ_KEY_L() == GPIO_PIN_RESET ||
    	READ_KEY_R() == GPIO_PIN_RESET ||
    	READ_KEY_J1() == GPIO_PIN_RESET ||
    	READ_KEY_J2() == GPIO_PIN_RESET) //����������ʱ����ƽΪ0
	{
		HAL_Delay(10);//����
		if(	READ_KEY_L() == GPIO_PIN_RESET ||
    		READ_KEY_R() == GPIO_PIN_RESET ||
    		READ_KEY_J1() == GPIO_PIN_RESET ||
    		READ_KEY_J2() == GPIO_PIN_RESET) //��ⰴ���������
		{
			key_val =1;
		}
	}
	return key_val;

}


