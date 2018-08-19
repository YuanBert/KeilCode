#ifndef __BSP_RS485x_H
#define __BSP_RS485x_H

#include "stm32f1xx_hal.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"

#define CLEARTIMESUM	1000 //1000ms的延时
#define RX_LEN			512
#define STX				0x5A

#define YEAR		0x88
#define MONTH		0x08
#define DAY			0x08

typedef struct{
	uint8_t  RX_Flag:1;
	uint16_t RX_Size;
	uint8_t  RX_pData[RX_LEN];
}RS485ReceiveInfo_t,*pRS485ReceiveInfo_t;

extern uint8_t gRevRS485BDelayFlag;
extern uint8_t gNeedFlashCardLastFlag;
extern uint8_t gNeedFlashCardFlag;
extern uint8_t gAutoRegisterFlag;
extern uint8_t gManuallyRegisterFlag;
extern uint8_t gKA2ActionFlag;
extern uint16_t		gKA1TimCnt;
extern uint16_t		gClearTimeCnt;
extern uint8_t		gRevRS485ADataFlag;
extern uint8_t		gRevRS485CAskFlag;
extern uint8_t		gAckRS485CAskDataBuff[37];
extern uint8_t gRS485BData[9];

extern RS485ReceiveInfo_t  gRS485AReceiveInfo;
extern RS485ReceiveInfo_t  gRS485BReceiveInfo;
extern RS485ReceiveInfo_t  gRS485CReceiveInfo;

void bsp_RS485xVar_Init(void);

void bsp_RS485A_Init(void);
void bsp_RS485B_Init(void);
void bsp_RS485C_Init(void);

void bsp_RS485A_SendByte(uint8_t *pDataBuff);
void bsp_RS485B_SendByte(uint8_t *pDataBuff);
void bsp_RS485C_SendByte(uint8_t *pDataBuff);

void bsp_RS485A_SendData(uint8_t *pDataBuff,uint16_t size);
void bsp_RS485B_SendData(uint8_t *pDataBuff,uint16_t size);
void bsp_RS485C_SendData(uint8_t *pDataBuff,uint16_t size);

void bsp_RS485C_PrashDataA(void);

void bsp_RS485A_PrashData(void);
void bsp_RS485B_PrashData(void);
void bsp_RS485C_PrashData(void);

void bsp_RS485A_AckCmd(void);
void bsp_RS485A_AckCmd_Extern(void);
void bsp_RS485B_AckCmd(uint8_t cmd, uint8_t tID);
void bsp_RS485C_AckData(void);


/* 清空登记信息 */
void bsp_ClearAllRegisterFloorInfo(void);

/* 清楚自动登记信息 */
void bsp_CLearAutoRegisterFloorInfo(void);

//void bsp_RS485B_AckCmd(void);
//void bsp_RS485C_AckCmd(void);

#endif
