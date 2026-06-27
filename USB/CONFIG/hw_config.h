#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H
#include "platform_config.h"
#include "usb_type.h"
#include "stm32f1xx_hal.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//USB-hw_config ����	   
//��������:2015/1/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
  
 
#define USB_USART_TXFIFO_SIZE   256	//USB���⴮�ڷ���FIFO��С		
#define USB_USART_REC_LEN	 	256	//USB���ڽ��ջ���������ֽ���

//����һ��USB USART FIFO�ṹ��
typedef struct  
{										    
	uint8_t  buffer[USB_USART_TXFIFO_SIZE];	//buffer
	uint16_t writeptr;						//дָ��
	uint16_t readptr;						//��ָ��
}_usb_usart_fifo;

extern _usb_usart_fifo uu_txfifo;		//USB���ڷ���FIFO
extern _usb_usart_fifo uu_rxfifo;		//USB���ڽ���FIFO

extern uint8_t  USB_USART_RX_BUF[USB_USART_REC_LEN]; //���ջ���,���USB_USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USB_USART_RX_STA;   					//����״̬���	
 
//USBͨ�ô��뺯������
void usb_vcp_init(void);
int usbGetDataWithTimout(uint8_t *c);
void usbsendData(uint8_t* data, uint16_t length);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void USB_Port_Set(uint8_t enable);
void IntToUnicode (uint32_t value,uint8_t *pbuf,uint8_t len);
void Get_SerialNum(void);

//��ͬUSB�������ӵĺ������� 
bool USART_Config(void);
void USB_To_USART_Send_Data(uint8_t* data_buffer, uint8_t Nb_bytes);
void USART_To_USB_Send_Data(void);
void USB_SendData(uint8_t data);
void usb_printf(char* fmt,...); 

#endif  


