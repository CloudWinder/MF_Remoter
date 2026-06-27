#ifndef __COMMANDER_H
#define __COMMANDER_H
#include <stdbool.h>
#include "atkp.h"
#include "joystick.h"


/*ң���������*/
typedef enum 
{
	REMOTOR_CMD,
	REMOTOR_DATA,
}remoterType_e;

/*��������*/
#define  CMD_GET_MSG		0x01	/*��ȡ������Ϣ���Լ죩*/
#define  CMD_GET_CANFLY		0x02	/*��ȡ�����Ƿ��ܷ�*/
#define  CMD_FLIGHT_LAND	0x03	/*��ɡ�����*/
#define  CMD_EMER_STOP		0x04	/*����ͣ��*/
#define  CMD_FLIP			0x05	/*4D����*/
#define  CMD_POWER_MODULE	0x06	/*�򿪹ر���չģ���Դ*/
#define  CMD_LEDRING_EFFECT	0x07	/*����RGB�ƻ�Ч��*/
#define  CMD_POWER_VL53LXX	0x08	/*�򿪹رռ���*/

/*���б���*/
#define  ACK_MSG			0x01	/*���᷵�ص���Ϣ*/

/*��չģ������ö��*/
enum expModuleID
{
	NO_MODULE,
	LED_RING,
	WIFI_CAMERA,
	OPTICAL_FLOW,
	MODULE1,
};

/*ң�����ݽṹ*/
typedef __packed struct
{
	float roll;      
	float pitch;  
	float yaw;      
	float thrust;
	float trimPitch;
	float trimRoll;
	uint8_t ctrlMode;
	bool flightMode;
	bool RCLock;
} remoterData_t;

typedef __packed struct
{
	uint8_t version;
	bool mpu_selfTest;
	bool baro_slfTest;
	bool isCanFly;
	bool isLowpower;
	enum expModuleID moduleID;
	
	float trimRoll;		/*roll΢��*/
	float trimPitch;	/*pitch΢��*/
} MiniFlyMsg_t;

void commanderTask(void* param);
joystickFlyf_t getFlyControlData(void);
float limit(float value,float min, float max);
void sendRmotorCmd(uint8_t cmd, uint8_t data);
void sendRmotorData(uint8_t *data, uint8_t len);

#endif /*__COMMANDER_H*/
