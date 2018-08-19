#ifndef __BSP_GPIOCONTROL_H
#define __BSP_GPIOCONTROL_H
#include "stm32f1xx_hal.h"
#include "main.h"
#include "gpio.h"

extern uint8_t  gSW24Flag;
extern uint8_t  gFireFlag;
extern uint8_t  gGroupNumber;
extern uint8_t  gMachineNumber;
extern uint8_t  gAddr;
extern uint8_t  gFloorOffset;
extern uint8_t  gCodeSendingTime;
extern uint32_t gCodeSendingTimeCnt;
extern uint8_t  gRS485AAckFlag;
extern uint8_t  gRevRS485AGateTypeFlag;
extern uint8_t  gRS485CSendGateTypeFlag;
extern uint8_t  gNagivateFloorOffset;

void bsp_gpioControl_Init(void);
void bsp_gpioControlGetFireFlag(void);
void bsp_gpioControlGetSW45Data(void);
uint8_t bsp_gpioControlGetGroupNumber(void);
uint8_t bsp_gpioControlGetMachineNumber(void);
uint8_t bsp_gpioControlGetAddr(void);
uint8_t bsp_gpioControlGetFloorOffset(void);
uint8_t bsp_gpioControlGetNagitaveFloorOffset(void);
uint8_t bsp_gpioControlGetCodeSendingTime(void);

void bsp_gpioControlGetRS485CSendGateTypeFlag(void);


#endif
