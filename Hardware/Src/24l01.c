#include "24l01.h"


/* �û����ò�����Ĭ�ϣ� */
#define DEFAULT_ADDR			0x123456789AULL
#define DEFAULT_CHANNEL 	 	2
#define DEFAULT_DATARATE  		DATA_RATE_250K
#define DEFAULT_POWR 			POWER_0dBm

uint64_t nrfAddress = DEFAULT_ADDR;
static void (*interruptCb)(void) = 0;

/***************************NRF24L01+��������***********************************/


static uint8_t SPI_RWByte(SPI_HandleTypeDef* hspi,uint8_t byte)
{			
  uint8_t d_read,d_send=byte;
  if(HAL_SPI_TransmitReceive(hspi,&d_send,&d_read,1,0xFF)!=HAL_OK)
  {
    d_read=0xFF;
  }
  return d_read;
}

/* д�Ĵ��� */
static uint8_t writeReg(uint8_t reg,uint8_t value)
{
	uint8_t status;	
	SPI2_CSN_L();                	 	
	status=SPI_RWByte(NRF_SPI,reg|CMD_W_REG);
	SPI_RWByte(NRF_SPI , value); 	
	SPI2_CSN_H();                 	  
	return status;       					
}

/* ���Ĵ��� */ 
static uint8_t readReg(uint8_t reg)
{
	uint8_t reg_val;	    
 	SPI2_CSN_L();         		 			
	SPI_RWByte(NRF_SPI,reg|CMD_R_REG);
	reg_val=SPI_RWByte(NRF_SPI,0xA5);
	SPI2_CSN_H();     								    
	return reg_val;    						
}	

/* �������� */
static uint8_t readBuf(uint8_t cmd,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,i;	       
	SPI2_CSN_L();            
	status=SPI_RWByte(NRF_SPI,cmd);   	   
	for(i=0;i<len;i++)
		pBuf[i]=SPI_RWByte(NRF_SPI,0XFF);
	SPI2_CSN_H();
	return status;
}

/* д������ */
static uint8_t writeBuf(uint8_t cmd,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,i;	    
	SPI2_CSN_L();          
	status=SPI_RWByte(NRF_SPI,cmd);
	for(i=0;i<len;i++)
		SPI_RWByte(NRF_SPI,*pBuf++);  
	SPI2_CSN_H();  			 	
	return status;  
}

/* �������ݰ�(PTXģʽ) */
void nrf_txPacket(uint8_t *tx_buf,uint8_t len)
{	
	NRF_CE_L();	
	writeBuf(CMD_W_TX_PAYLOAD,tx_buf,len);
	NRF_CE_H();		 
}

/* ����NO_ACK���ݰ�(PTXģʽ) */
void nrf_txPacketNoACK(uint8_t *tx_buf,uint8_t len)
{	
	NRF_CE_L();		 
	writeBuf(CMD_W_TX_PAYLOAD_NO_ACK,tx_buf,len);
	NRF_CE_H();		
}

/* ����ACK���ݰ�������0ͨ��(PRXģʽ) */
void nrf_txPacket_AP(uint8_t *tx_buf,uint8_t len)
{	
	NRF_CE_L();		 	
	writeBuf(CMD_W_ACK_PAYLOAD(0),tx_buf,len);
	NRF_CE_H();		 
}
/* ����NO_ACK���ݰ�(PTXģʽ) */
void nrf_sendPacketNoACK(uint8_t *sendBuf,uint8_t len)
{	
	while((readReg(REG_STATUS)&0x01)!=0);	/* �ȴ�TX_FIFO��Ϊfull */
	nrf_txPacketNoACK(sendBuf,len);			/* ����NO_ACK���ݰ� */
}

/**************************NRF24L01+���ú���***********************************/
/* ���÷�����յ�ַ�������շ���ַһ�� */
void nrf_setAddress(uint64_t address)
{
	writeBuf(CMD_W_REG |REG_RX_ADDR_P0,(uint8_t*)&address,5);//����ʹ��P0�ڵ�
	writeBuf(CMD_W_REG |REG_TX_ADDR,(uint8_t*)&address,5); 		
}

/* ����Ƶ��ͨ��,channel:0~125 */
void nrf_setChannel(uint8_t channel)
{
	if(channel<=125) 
		writeReg(REG_RF_CH, channel);
}

/* ���ô������ʣ�dr:0->250KHz��1->1MHz��2->2MHz�� */
void nrf_setDataRate(enum nrfRate dataRate)
{
	uint8_t reg_rf = readReg(REG_RF_SETUP);
	reg_rf &= ~((1<<5)|(1<<3));/* ���ԭ������ */
	switch(dataRate)
	{
		case DATA_RATE_250K:
			reg_rf |= 0x20;
			break;
		case DATA_RATE_1M:
			reg_rf |= 0x00;
			break;
		case DATA_RATE_2M:
			reg_rf |= 0x08;
			break;	
	}
	writeReg(REG_RF_SETUP,reg_rf); 	
}

/* ���÷��书��,power: 0->-18dB  1->-12dB  2->-6dB  3->0dB */
void nrf_setPower(enum nrfPower power)
{
	uint8_t reg_rf = readReg(REG_RF_SETUP);
	reg_rf &= 0xF8;/* ���ԭ�蹦�� */
	switch(power)
	{
		case POWER_M18dBm:
			reg_rf |= 0x00;
			break;
		case POWER_M12dBm:
			reg_rf |= 0x02;
			break;
		case POWER_M6dBm:
			reg_rf |= 0x04;
			break;
		case POWER_0dBm:
			reg_rf |= 0x07;
			break;	
	}
	writeReg(REG_RF_SETUP,reg_rf);
}

/* �����ط�ʱ�������������ʼ��շ��ֽڴ�С���� */
/* ��ϸ˵���ο�nrf24l01.datasheet��P34. */
void nrf_setArd(void)
{
	uint8_t reg_rf,reg_retr;
	reg_rf=readReg(REG_RF_SETUP);
	reg_retr=readReg(REG_SETUP_RETR);
	if(!(reg_rf&0x20))	/* ���ʲ���250K(�Ĵ���0x20) */
		reg_retr|= 1<<4;/* (1+1)*250=500us,�ڽ���32�ֽ�ʱ */
	else
		reg_retr|= 5<<4;/* (5+1)*250=1500us,�ڽ���32�ֽ�ʱ */
	writeReg(REG_SETUP_RETR,reg_retr);
}

/* �����ط�������arc:0~15 */
void nrf_setArc(uint8_t arc)
{
	uint8_t reg_retr;
	if(arc>15)
		return;
	reg_retr=readReg(REG_SETUP_RETR);
	reg_retr|= arc;
	writeReg(REG_SETUP_RETR,reg_retr);
}

/* ��ȡ���չ��ʼ�� */
uint8_t nrf_getRpd(void)
{
   return readReg(REG_RPD);
}

/* ��ȡ�ط�ʧ�ܴ��� */
uint8_t nrf_getTxRetry(void)
{
   return readReg(REG_OBSERVE_TX)&0x0F;
}


/* ��ʼ��NRF24L01���� */
/* model: PTX_MODE��PRX_MODE */
void nrfInit(enum nrfMode model)
{
	nrf_setAddress(nrfAddress);
	nrf_setChannel(DEFAULT_CHANNEL);
	nrf_setDataRate(DEFAULT_DATARATE);
	nrf_setPower(DEFAULT_POWR);
	nrf_setArd();
	nrf_setArc(3);
	if(model==PRX_MODE)
	{
		writeReg(REG_CONFIG, 0x0f);   		/* IRQ�շ�����жϿ���,16λCRC,PRX */
		writeReg(REG_DYNPD,0x01);			/* ʹ��RX_P0��̬����PLAYLOAD */
		writeReg(REG_FEATURE,0x06);			/* ʹ�ܶ�̬����PLAYLOAD������ACK PLAYLOAD */
		
		writeReg(REG_EN_AA,0x01); 			/* ʹ��ͨ��0���Զ�Ӧ�� */	
		
		writeReg(CMD_FLUSH_TX,0xff);		/* ��ϴTX_FIFO */
		writeReg(CMD_FLUSH_RX,0xff);
	}
	else							 	
	{
		writeReg(REG_CONFIG, 0x0e);   		/* IRQ�շ�����жϿ���,16λCRC,PTX */
		writeReg(REG_DYNPD,0x01);			/* ʹ��RX_P0��̬����PLAYLOAD */
		writeReg(REG_FEATURE,0x07);			/* ʹ�ܶ�̬���ȡ�ACK PLAYLOAD���͡�W_TX_PAYLOAD_NOACK */
		
		writeReg(CMD_FLUSH_TX,0xff);		/* ��ϴTX_FIFO */
		writeReg(CMD_FLUSH_RX,0xff);
	}
}

/* ���MCU��24l01�Ƿ�ͨѶ���� */
/* ������д�������ַ�Ƿ�һ�� */
ErrorStatus nrf_check(void)
{ 
	uint64_t addr = 0;
	
	writeBuf(CMD_W_REG |REG_TX_ADDR,(uint8_t*)&nrfAddress,5); 
	readBuf(CMD_R_REG|REG_TX_ADDR,(uint8_t*)&addr,5); 
	if(nrfAddress==addr)
		return SUCCESS;
	else
		return ERROR;
}

/* �������ݰ������ذ�����len */
uint8_t nrf_rxPacket(uint8_t *rx_buf)
{	
	uint8_t rx_len = readReg(CMD_RX_PL_WID);
	if(rx_len>0 && rx_len<33)
	{
		NRF_CE_L();	
		readBuf(CMD_R_RX_PAYLOAD,rx_buf,rx_len);
		NRF_CE_H();
	}
	else 
		rx_len = 0;
	writeReg(CMD_FLUSH_RX,0xff);/* ��ϴRX_FIFO */
	return rx_len;		
}

/* ��ѯ�¼����������ݰ� */
nrfEvent_e nrf_checkEventandRxPacket(uint8_t *ackBuf, uint8_t *acklen)
{
	nrfEvent_e nrfEvent = IDLE;
	*acklen = 0;
	uint8_t status = readReg(REG_STATUS);/*���¼���־�Ĵ���*/
	if(status&BIT_MAX_RT)/*�ط�ʧ��*/
	{
		writeReg(CMD_FLUSH_TX,0xff);
		nrfEvent =  MAX_RT;
	}
	else if(status&BIT_RX_DR)/*�������ݵ�RX_FIFO*/
	{
		*acklen =  nrf_rxPacket(ackBuf);
		nrfEvent = RX_DR;
	}
	else if(status&BIT_TX_DS)/*����������TX_FIFO�ɹ�*/
	{
		nrfEvent = TX_DS;
	}
	writeReg(REG_STATUS,0x70);/*�����־*/
	uint8_t status1 = readReg(REG_STATUS);/*���¼���־�Ĵ���*/
	status1 = status1;
	return nrfEvent;
}

/* �������ݰ������ȴ�����ACK(PTXģʽ) */
/* ����ֵ��1�ɹ���0ʧ��*/
uint8_t nrf_sendPacketWaitACK(uint8_t *sendBuf, uint8_t len, uint8_t *ackBuf, uint8_t *acklen)
{ 
	if(len==0) return 0;
	nrf_txPacket(sendBuf,len);
	while((readReg(REG_STATUS)&0x70) == 0);/* �ȴ��¼� */
	nrfEvent_e nrfEvent = nrf_checkEventandRxPacket(ackBuf, acklen);
	if(nrfEvent == MAX_RT)
		return 0;
	return 1;
}


/*�ⲿ�жϷ�����*/
void nrf_setIterruptCallback(void(*cb)(void))
{
	interruptCb = cb;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_13)
  {
  		if(interruptCb)
		{
			interruptCb();
		}
  }
}

