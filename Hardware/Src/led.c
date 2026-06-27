#include "led.h"
#include "key.h"


/* LED��ʼ��
void ledInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	 ��ֹJTAʹ��SWD��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	 ��ʼ��LED_BLUE(PB3) LED_RED(PB7)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	 �ر�����LED
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
}
 */

void led_key_test(void)
{
	if (keyScan() == 1)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_SET);
		LED_BLUE_OFF();
		LED_RED_OFF();
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,GPIO_PIN_RESET);
		LED_BLUE_ON();
		LED_RED_ON();
	}
}
