#ifndef __24L01_H
#define __24L01_H
#include <stdint.h>
#include "stm32f1xx_hal.h"


/***********************NRF SPI�Ĵ���ָ��*********************************/
#define CMD_R_REG       	0x00  // ���Ĵ���ָ��
#define CMD_W_REG       	0x20  // д�Ĵ���ָ��
#define CMD_RX_PL_WID   	0x60  // ��ȡ�������ݳ���
#define CMD_R_RX_PAYLOAD 	0x61  // ��ȡ��������ָ��
#define CMD_W_TX_PAYLOAD  	0xA0  // д��������ָ��
#define CMD_FLUSH_TX      	0xE1  // ��ϴ���� FIFOָ��
#define CMD_FLUSH_RX      	0xE2  // ��ϴ���� FIFOָ��
#define CMD_REUSE_TX_PL   	0xE3  // �����ظ�װ������ָ��
#define CMD_NOP           	0xFF  // ��������������ȡstatus�Ĵ���

#define CMD_W_ACK_PAYLOAD(P)  (0xA8|(P&0x0F))//PRXģʽװ��PAYLOAD��ʹ��Pͨ��
#define CMD_W_TX_PAYLOAD_NO_ACK 0xB0				 //PTXģʽдNO ACK����

//***********************NRF �Ĵ�����ַ***********************************//
#define REG_CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define REG_EN_AA           0x01  // �Զ�Ӧ��������
#define REG_EN_RXADDR       0x02  // �����ŵ�����
#define REG_SETUP_AW        0x03  // �շ���ַ��������
#define REG_SETUP_RETR      0x04  // �Զ��ط���������
#define REG_RF_CH           0x05  // ����Ƶ������
#define REG_RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define REG_STATUS    		0x07  // ״̬�Ĵ���
#define REG_OBSERVE_TX      0x08  // ���ͼ�⹦��
#define REG_RPD             0x09  // ���չ��ʼ��           
#define REG_RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define REG_RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define REG_RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define REG_RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define REG_RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define REG_RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define REG_TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define REG_RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define REG_RX_PW_P1        0x12  // ����Ƶ��1�������ݳ���
#define REG_RX_PW_P2        0x13  // ����Ƶ��2�������ݳ���
#define REG_RX_PW_P3        0x14  // ����Ƶ��3�������ݳ���
#define REG_RX_PW_P4        0x15  // ����Ƶ��4�������ݳ���
#define REG_RX_PW_P5        0x16  // ����Ƶ��5�������ݳ���
#define REG_FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
#define REG_DYNPD 			0x1C  // ��̬���ݰ�����
#define REG_FEATURE 		0x1D  // �ص�Ĵ���

/* STATUS�Ĵ�������λ */
#define BIT_RX_DR (1<<6)
#define BIT_TX_DS (1<<5)
#define BIT_MAX_RT (1<<4)

#define  NRF_SPI  			&hspi1
#define  NRF_CE_H()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET)
#define  NRF_CE_L()			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET)

#define  SPI2_CSN_H()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define  SPI2_CSN_L()		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)

extern SPI_HandleTypeDef hspi1;

enum nrfMode
{
	PTX_MODE,
	PRX_MODE,
};	

enum nrfPower
{
	POWER_M18dBm,
	POWER_M12dBm,
	POWER_M6dBm,
	POWER_0dBm,
};

enum nrfRate
{
	DATA_RATE_250K,
	DATA_RATE_1M,
	DATA_RATE_2M,
};

typedef enum 
{
	IDLE    = 0,
	MAX_RT 	= 1,
	TX_DS 	= 2,
	RX_DR 	= 3,
}nrfEvent_e;
	
/* ��ʼ��NRF24L01���� */
void nrfInit(enum nrfMode model);	

/* ���nrf�Ƿ�ͨѶ���� */
ErrorStatus nrf_check(void);

/* �������ݰ�(PTXģʽ) */
void nrf_txPacket(uint8_t *tx_buf,uint8_t len);

/* ����NO_ACK���ݰ�(PTXģʽ) */
void nrf_txPacketNoACK(uint8_t *tx_buf,uint8_t len);

/* ����ACK���ݰ�������0ͨ��(PRXģʽ) */
void nrf_txPacket_AP(uint8_t *tx_buf,uint8_t len);

/* ����NO_ACK���ݰ�(PTXģʽ) */
void nrf_sendPacketNoACK(uint8_t *sendBuf,uint8_t len);
	
/* �������ݰ������ȴ�����ACK (PTXģʽ) */
uint8_t nrf_sendPacketWaitACK(uint8_t *sendBuf ,uint8_t len ,
							uint8_t *ackBuf  ,uint8_t *acklen);
/* ��ѯ�¼����������ݰ� */
nrfEvent_e nrf_checkEventandRxPacket(uint8_t *ackBuf, uint8_t *acklen);

/*����nrf�жϻص�����*/
void nrf_setIterruptCallback(void(*cb)(void));

/**************************NRF24L01+���ú���***********************************/
void nrf_setAddress(uint64_t address);
void nrf_setChannel(uint8_t channel);
void nrf_setDataRate(enum nrfRate dataRate);
void nrf_setPower(enum nrfPower power);
void nrf_setArd(void);
void nrf_setArc(uint8_t arc);
uint8_t nrf_getRpd(void);
uint8_t nrf_getTxRetry(void);
void nrf_txPacket(uint8_t *tx_buf,uint8_t len);
uint8_t nrf_rxPacket(uint8_t *rx_buf);
#endif


