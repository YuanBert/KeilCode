#include "bsp_rs485x.h"
#include "bsp_gpioControl.h"
#include "bsp_system.h"
#include "stm32Flash.h"
//#define DEBUG  1
RS485ReceiveInfo_t  gRS485AReceiveInfo;
RS485ReceiveInfo_t  gRS485BReceiveInfo;
RS485ReceiveInfo_t  gRS485CReceiveInfo;

uint8_t gNeedFlashCardLastFlag;
uint8_t gNeedFlashCardFlag;
uint8_t gManuallyRegisterFlag;
uint8_t gAutoRegisterFlag;
uint8_t gKA2ActionFlag;
extern uint8_t gKA1Flag;
uint16_t	gKA1TimCnt;
uint16_t	gClearTimeCnt;
uint8_t gRevRS485CAskFlag;
uint8_t gRevRS485ADataFlag;
uint8_t gRevRS485BDelayFlag;

uint16_t gRevRS485ADataLegth;
/* RS485B 权限数据 */
uint8_t gRS485BData[9];

/* RS485C回复数据 */

uint8_t gAckRS485CAskDataBuff[37];
/* 获取485A 协议2 的数据*/
uint8_t gRS485AV2DataBuff[35];



/* 获取数据的异或校验码 */
static uint8_t getXORCode(uint8_t * pDataBuff, uint16_t len)
{
	uint16_t i = 0;
	uint8_t temp = *pDataBuff;
	for(i = 1; i < len; i++)
	{
		temp ^= *(pDataBuff + i);
	}
	return temp;
}
/* 获取校验和 */
static uint8_t getSUMCode(uint8_t * pDataBuff,uint16_t len)
{
  uint16_t i = 0;
  uint8_t retSUM = 0;
  for(i = 0; i < len; i++)
  {
    retSUM += *(pDataBuff + i);
  }
  return retSUM;
  
}

void bsp_RS485xVar_Init(void)
{
	
}

void bsp_RS485A_Receive_IDLE(UART_HandleTypeDef *huart)
{
	uint32_t temp;
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart3);
		HAL_UART_DMAStop(&huart3);
		temp = huart3.hdmarx->Instance->CNDTR;
		gRS485AReceiveInfo.RX_Size = RX_LEN - temp;
		gRS485AReceiveInfo.RX_Flag = 1;
		HAL_UART_Receive_DMA(&huart3,gRS485AReceiveInfo.RX_pData,RX_LEN);
	}
}

void bsp_RS485B_Receive_IDLE(UART_HandleTypeDef *huart)
{
	uint32_t temp;
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		HAL_UART_DMAStop(&huart2);
		temp = huart2.hdmarx->Instance->CNDTR;
		gRS485BReceiveInfo.RX_Size = RX_LEN - temp;
		gRS485BReceiveInfo.RX_Flag = 1;
		HAL_UART_Receive_DMA(&huart2,gRS485BReceiveInfo.RX_pData,RX_LEN);
	}
}

void bsp_RS485C_Receive_IDLE(UART_HandleTypeDef *huart)
{
	uint32_t temp;
	if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		HAL_UART_DMAStop(&huart1);
		temp = huart1.hdmarx->Instance->CNDTR;
		gRS485CReceiveInfo.RX_Size = RX_LEN - temp;
		gRS485CReceiveInfo.RX_Flag = 1;
		HAL_UART_Receive_DMA(&huart1,gRS485CReceiveInfo.RX_pData,RX_LEN);
	}
	//bsp_RS485C_PrashData();
}


void bsp_RS485A_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart3,gRS485AReceiveInfo.RX_pData, RX_LEN);
	gNeedFlashCardFlag = 1; //上电默认为需要刷卡模式
}

void bsp_RS485B_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart2,gRS485BReceiveInfo.RX_pData, RX_LEN);
}

void bsp_RS485C_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
	//HAL_UART_Receive_DMA(&huart1,gRS485CReceiveInfo.RX_pData, RX_LEN);
}

void bsp_RS485A_SendByte(uint8_t *pDataBuff)
{
	HAL_UART_Transmit(&huart3,pDataBuff,1,0xFFFF);
}
void bsp_RS485B_SendByte(uint8_t *pDataBuff)
{
	HAL_UART_Transmit(&huart2,pDataBuff,1,0xFFFF);
}
void bsp_RS485C_SendByte(uint8_t *pDataBuff)
{
   HAL_UART_Transmit(&huart1,pDataBuff,1,0xFFFF);
}

void bsp_RS485A_SendData(uint8_t *pDataBuff,uint16_t size)
{
	HAL_UART_Transmit(&huart3,pDataBuff,size,0xFFFF);
}
void bsp_RS485B_SendData(uint8_t *pDataBuff,uint16_t size)
{
	HAL_UART_Transmit(&huart2,pDataBuff,size,0xFFFF);
}
void bsp_RS485C_SendData(uint8_t *pDataBuff,uint16_t size)
{
	HAL_UART_Transmit(&huart1,pDataBuff,size,0xFFFF);
}


void bsp_RS485A_AckCmd(void)
{
	uint8_t ackBuff[5];
	ackBuff[0] = 0x02;
	ackBuff[1] = 0xC3;
	ackBuff[2] = 0x20;
	ackBuff[3] = 0xE1;
	ackBuff[4] = 0x03;
	bsp_RS485A_SendData(ackBuff, 5);
	
}
void bsp_RS485A_AckCmd_Extern(void)
{
	
}

void bsp_RS485A_PrashData(void)
{
	uint8_t tXorData = 0;
	uint16_t i = 0;
	uint16_t cnt = 0;
	uint8_t tData = 0;
	uint8_t tDataTypeFlag = 0;//0-表示协议2-0，1-表示协议2-1，2-表示协议2-2
	
	if(gRS485AReceiveInfo.RX_Flag)
	{
		gRS485AReceiveInfo.RX_Flag = 0;
		
		if(gRevRS485AGateTypeFlag == gRS485AReceiveInfo.RX_pData[9])
		{
			return;
		}
		/* 遇到火警不进行对RS485A数据接收 */
		if( 0 == gFireFlag)
		{
#ifdef DEBUG
			printf("has the fire alarm ,no ack to RS485A\r\n");
#endif
			return;
		}
		/* 标准协议1 */
		if(0x02 == gRS485AReceiveInfo.RX_pData[0] && 0xC3 == gRS485AReceiveInfo.RX_pData[1])
		{
			if(0x00 != gGroupNumber && gGroupNumber != gRS485AReceiveInfo.RX_pData[7])
			{
#ifdef DEBUG
				printf("组号不匹配\r\n");
#endif
				return;
			}
			
			if(0x00 != gMachineNumber && gMachineNumber != gRS485AReceiveInfo.RX_pData[8])
			{
#ifdef DEBUG
				printf("机器号不匹配\r\n");
#endif
				return;
			}
			
			if(gRevRS485AGateTypeFlag == gRS485AReceiveInfo.RX_pData[9])
			{
#ifdef DEBUG
				printf("主副门不匹配\r\n");
#endif
				return;
			}
			
			if(0x03 != gRS485AReceiveInfo.RX_pData[36])
			{
#ifdef DEBUG
				printf("结束帧错误\r\n");
#endif
				return;
			}
			
			tXorData = getXORCode(gRS485AReceiveInfo.RX_pData, 35);
			if(tXorData != gRS485AReceiveInfo.RX_pData[35])
			{
#ifdef DEBUG
				printf("校验码错误\r\n");
#endif
				return;
			}
			/* 添加对刷卡模式的处理 */
			if(0x88 == gRS485AReceiveInfo.RX_pData[11] && 0x08 == gRS485AReceiveInfo.RX_pData[12] && 0x08 == gRS485AReceiveInfo.RX_pData[13])
			{
				gNeedFlashCardFlag  = 0;  //启动无需刷卡模式
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET); //开启指示灯
        if(2 == gRS485CSendGateTypeFlag)
        {
          for(i = 2; i < 34; i++)
          {
            gAckRS485CAskDataBuff[i] = 0xFF;
          }
        }
        if(1 == gRS485CSendGateTypeFlag)
        {
          for(i = 2; i < 18; i ++)
          {
            gAckRS485CAskDataBuff[i] = 0xFF;
          }
        }
        if(0 == gRS485CSendGateTypeFlag)
        {
          for(i = 18; i < 34; i++)
          {
            gAckRS485CAskDataBuff[i] = 0xFF;
          }
        }
        bsp_CLearAutoRegisterFloorInfo();
				bsp_RS485A_AckCmd();
				return;
			}
			
      if(0x99 == gRS485AReceiveInfo.RX_pData[11] && 0x09 == gRS485AReceiveInfo.RX_pData[12] && 0x09 == gRS485AReceiveInfo.RX_pData[13])
			{
				gNeedFlashCardFlag  = 1;  //启动刷卡模式
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET); //关闭指示灯
				bsp_RS485A_AckCmd();
				return;
			}
      if(0 == gNeedFlashCardFlag && GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4))
      {
        return;
      }
			/* 添加对时间的处理 */
			
			/* 写权限 */
			for(i = 27; i < 35; i++)
			{
				if(gRS485AReceiveInfo.RX_pData[i] > 0)
				{
					cnt++;
				}
			}
			
			if(cnt > 1) //手动登记
			{
				gManuallyRegisterFlag = 1;
				gClearTimeCnt = gCodeSendingTimeCnt;
				//if(0 == gRS485AReceiveInfo.RX_pData[9])
				//{
					
					gAckRS485CAskDataBuff[2] |= (gRS485AReceiveInfo.RX_pData[27] << gFloorOffset);
					gAckRS485CAskDataBuff[3] |= (gRS485AReceiveInfo.RX_pData[28] << gFloorOffset);
					gAckRS485CAskDataBuff[4] |= (gRS485AReceiveInfo.RX_pData[29] << gFloorOffset);
					gAckRS485CAskDataBuff[5] |= (gRS485AReceiveInfo.RX_pData[30] << gFloorOffset);
					gAckRS485CAskDataBuff[6] |= (gRS485AReceiveInfo.RX_pData[31] << gFloorOffset);
					gAckRS485CAskDataBuff[7] |= (gRS485AReceiveInfo.RX_pData[32] << gFloorOffset);
					gAckRS485CAskDataBuff[8] |= (gRS485AReceiveInfo.RX_pData[33] << gFloorOffset);
					gAckRS485CAskDataBuff[9] |= (gRS485AReceiveInfo.RX_pData[34] << gFloorOffset);
				//}
				//else
				//{
					
					gAckRS485CAskDataBuff[18] |= (gRS485AReceiveInfo.RX_pData[27] << gFloorOffset);
					gAckRS485CAskDataBuff[19] |= (gRS485AReceiveInfo.RX_pData[28] << gFloorOffset);
					gAckRS485CAskDataBuff[20] |= (gRS485AReceiveInfo.RX_pData[29] << gFloorOffset);
					gAckRS485CAskDataBuff[21] |= (gRS485AReceiveInfo.RX_pData[30] << gFloorOffset);
					gAckRS485CAskDataBuff[22] |= (gRS485AReceiveInfo.RX_pData[31] << gFloorOffset);
					gAckRS485CAskDataBuff[23] |= (gRS485AReceiveInfo.RX_pData[32] << gFloorOffset);
					gAckRS485CAskDataBuff[24] |= (gRS485AReceiveInfo.RX_pData[33] << gFloorOffset);
					gAckRS485CAskDataBuff[25] |= (gRS485AReceiveInfo.RX_pData[34] << gFloorOffset);
				//}
			}
			else
			{
				for(i = 27; i < 35; i++)
				{
					if(gRS485AReceiveInfo.RX_pData[i] > 0)
					{
						tData = gRS485AReceiveInfo.RX_pData[i];
					}
				}
				cnt = 0;
				for(i = 0; i < 8; i++)
				{
					if(1 == ((tData >> i) & 0x01))
					{
						cnt ++;
					}
				}
				if(cnt > 1) //手动登记
				{
					gManuallyRegisterFlag = 1;
					gClearTimeCnt = gCodeSendingTimeCnt;
					//if(0 == gRS485AReceiveInfo.RX_pData[9])
					//{
						gAckRS485CAskDataBuff[2] |= (gRS485AReceiveInfo.RX_pData[27] << gFloorOffset);
						gAckRS485CAskDataBuff[3] |= (gRS485AReceiveInfo.RX_pData[28] << gFloorOffset);
						gAckRS485CAskDataBuff[4] |= (gRS485AReceiveInfo.RX_pData[29] << gFloorOffset);
						gAckRS485CAskDataBuff[5] |= (gRS485AReceiveInfo.RX_pData[30] << gFloorOffset);
						gAckRS485CAskDataBuff[6] |= (gRS485AReceiveInfo.RX_pData[31] << gFloorOffset);
						gAckRS485CAskDataBuff[7] |= (gRS485AReceiveInfo.RX_pData[32] << gFloorOffset);
						gAckRS485CAskDataBuff[8] |= (gRS485AReceiveInfo.RX_pData[33] << gFloorOffset);
						gAckRS485CAskDataBuff[9] |= (gRS485AReceiveInfo.RX_pData[34] << gFloorOffset);
					//}
					//else
					//{
						gAckRS485CAskDataBuff[18] |= (gRS485AReceiveInfo.RX_pData[27] << gFloorOffset);
						gAckRS485CAskDataBuff[19] |= (gRS485AReceiveInfo.RX_pData[28] << gFloorOffset);
						gAckRS485CAskDataBuff[20] |= (gRS485AReceiveInfo.RX_pData[29] << gFloorOffset);
						gAckRS485CAskDataBuff[21] |= (gRS485AReceiveInfo.RX_pData[30] << gFloorOffset);
						gAckRS485CAskDataBuff[22] |= (gRS485AReceiveInfo.RX_pData[31] << gFloorOffset);
						gAckRS485CAskDataBuff[23] |= (gRS485AReceiveInfo.RX_pData[32] << gFloorOffset);
						gAckRS485CAskDataBuff[24] |= (gRS485AReceiveInfo.RX_pData[33] << gFloorOffset);
						gAckRS485CAskDataBuff[25] |= (gRS485AReceiveInfo.RX_pData[34] << gFloorOffset);
					//}
				}
				else  //自动登记
				{
					//if(0 == gRS485AReceiveInfo.RX_pData[9])
					//{
						gAutoRegisterFlag = 1;
            if(0 == gManuallyRegisterFlag || gClearTimeCnt < 70)
            {
              gClearTimeCnt = 80;
            }
						gAckRS485CAskDataBuff[10] |= (gRS485AReceiveInfo.RX_pData[27] << gFloorOffset);
						gAckRS485CAskDataBuff[11] |= (gRS485AReceiveInfo.RX_pData[28] << gFloorOffset);
						gAckRS485CAskDataBuff[12] |= (gRS485AReceiveInfo.RX_pData[29] << gFloorOffset);
						gAckRS485CAskDataBuff[13] |= (gRS485AReceiveInfo.RX_pData[30] << gFloorOffset);
						gAckRS485CAskDataBuff[14] |= (gRS485AReceiveInfo.RX_pData[31] << gFloorOffset);
						gAckRS485CAskDataBuff[15] |= (gRS485AReceiveInfo.RX_pData[32] << gFloorOffset);
						gAckRS485CAskDataBuff[16] |= (gRS485AReceiveInfo.RX_pData[33] << gFloorOffset);
						gAckRS485CAskDataBuff[17] |= (gRS485AReceiveInfo.RX_pData[34] << gFloorOffset);
					//}
					//else
					//{
						gAckRS485CAskDataBuff[26] |= (gRS485AReceiveInfo.RX_pData[27] << gFloorOffset);
						gAckRS485CAskDataBuff[27] |= (gRS485AReceiveInfo.RX_pData[28] << gFloorOffset);
						gAckRS485CAskDataBuff[28] |= (gRS485AReceiveInfo.RX_pData[29] << gFloorOffset);
						gAckRS485CAskDataBuff[29] |= (gRS485AReceiveInfo.RX_pData[30] << gFloorOffset);
						gAckRS485CAskDataBuff[30] |= (gRS485AReceiveInfo.RX_pData[31] << gFloorOffset);
						gAckRS485CAskDataBuff[31] |= (gRS485AReceiveInfo.RX_pData[32] << gFloorOffset);
						gAckRS485CAskDataBuff[32] |= (gRS485AReceiveInfo.RX_pData[33] << gFloorOffset);
						gAckRS485CAskDataBuff[33] |= (gRS485AReceiveInfo.RX_pData[34] << gFloorOffset);
					//}
				}
			}
			
			
			/* 控制GPIO 闭合继电器KA1 */
			HAL_GPIO_WritePin(KA1_GPIO_Port,KA1_Pin,GPIO_PIN_SET);
			/* 应答RS485A的命令 */
			bsp_RS485A_AckCmd();
			if(1 != cnt)
			{
				gKA1TimCnt = 60;
			}
			gKA2ActionFlag = 1;
			gKA1Flag = 1; //用于定时器打开KA1控制
			gKA1TimCnt = 60;
#ifdef DEBUG
			gAckRS485CAskDataBuff[0] = STX;
			gAckRS485CAskDataBuff[1] = gAddr;
			gAckRS485CAskDataBuff[36] = getXORCode(gAckRS485CAskDataBuff, 36);
			//bsp_RS485C_SendData(gAckRS485CAskDataBuff, 37);
			//bsp_RS485C_SendData(gAckRS485CAskDataBuff, 37);
#endif
			return;
		}
		 /* 标准协议2 */
		if(0x02 == gRS485AReceiveInfo.RX_pData[0] && 0xA0 == gRS485AReceiveInfo.RX_pData[2])
		{
			/* 如果拨码为不应答，则不处理数据，直接返回 0 == gRS485AAckFlag || */
			if(0 == gFireFlag)
			{
#ifdef DEBUG
				printf("拨码为不应答，不应答RS485A\r\n");
#endif
				return ;
			}
#ifdef DEBUG
			printf("标准处理协议2\r\n");
#endif
			cnt = 0;
			gRS485AV2DataBuff[cnt++] = 0x02;
			gRS485AV2DataBuff[cnt++] = gRS485AReceiveInfo.RX_pData[1];
			gRS485AV2DataBuff[cnt++] = gRS485AReceiveInfo.RX_pData[2];
			/* 移除插入值 */
			for(i = 3; i < gRS485AReceiveInfo.RX_Size;)
			{
				if(0x10 == gRS485AReceiveInfo.RX_pData[i])
				{
					gRS485AV2DataBuff[cnt++] = gRS485AReceiveInfo.RX_pData[i + 1];
					i = i + 2;
				}
				else
				{
					gRS485AV2DataBuff[cnt++] = gRS485AReceiveInfo.RX_pData[i];
					i++;
				}
				
				if(26 == cnt && 0xEA == gRS485AV2DataBuff[25])
				{
					tDataTypeFlag = 1;
				}
				if(30 == cnt && 0xE1 == gRS485AV2DataBuff[29])
				{
					tDataTypeFlag = 2;
				}
				if(0 == tDataTypeFlag && 30 == cnt)
				{
#ifdef DEBUG
					printf("RS485A协议2-0 数据帧\r\n");
#endif
					break;
				}
				if(1 == tDataTypeFlag && 31 == cnt)
				{
#ifdef DEBUG
					printf("RS485A协议2-1 数据帧\r\n");
#endif
					break;
				}
				if(2 == tDataTypeFlag && 32 == cnt)
				{
#ifdef DEBUG
					printf("RS485A协议2-2 数据帧\r\n");
#endif
					break;
				}
			}
			
#ifdef DEBUG
				printf("接收到数据帧的数量：%03d\r\n",cnt);
#endif
			
			/* 数据接收错误，直接退出 */
			if(30 != cnt && 0 == tDataTypeFlag)
			{
#ifdef DEBUG
				printf("数据帧数量接收错误--00\r\n");
#endif
				return;
			}
			if(31 != cnt && 1 == tDataTypeFlag)
			{
#ifdef DEBUG
				printf("数据帧数量接收错误--01\r\n");
#endif
				return;
			}
			if(32 != cnt && 2 == tDataTypeFlag)
			{
#ifdef DEBUG
				printf("数据帧数量接收错误--02\r\n");
#endif
				return;
			}
			/* 检测结束位是否正确 */
			if(0x03 != gRS485AV2DataBuff[29] && 0 == tDataTypeFlag)
			{
#ifdef DEBUG
				printf("数据结束位接收错误--00\r\n");
#endif
				return;
			}
			if(0x03 != gRS485AV2DataBuff[30] && 1 == tDataTypeFlag)
			{
#ifdef DEBUG
				printf("数据结束位接收错误--01\r\n");
#endif
				return;
			}
			if(0x03 != gRS485AV2DataBuff[31] && 2 == tDataTypeFlag)
			{
#ifdef DEBUG
				printf("数据结束位接收错误--02\r\n");
#endif
				return;
			}
			
			
			//进行校验
			if(0 == tDataTypeFlag)
			{
				tXorData = getXORCode(gRS485AV2DataBuff,28);
				if(tXorData != gRS485AV2DataBuff[28])
				{
#ifdef DEBUG
					printf("校验位错误--00\r\n");
#endif
					return;
				}
			}
			
			if(1 == tDataTypeFlag)
			{
				tXorData = getXORCode(gRS485AV2DataBuff,29);
				if(tXorData != gRS485AV2DataBuff[29])
				{
#ifdef DEBUG
					printf("校验位错误--01\r\n");
#endif
					return;
				}
			}
			
			if(2 == tDataTypeFlag)
			{
				tXorData = getXORCode(gRS485AV2DataBuff,30);
				if(tXorData != gRS485AV2DataBuff[30])
				{
#ifdef DEBUG
					printf("校验位错误--02\r\n");
#endif
					return;
				}
			}
      
			/* 添加刷卡模式转换 */
			if(0x88 == gRS485AV2DataBuff[16] && 0x08 == gRS485AV2DataBuff[17] && 0x08 == gRS485AV2DataBuff[18])
			{
				gNeedFlashCardFlag  = 0;  //启动无需刷卡模式
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
        if(2 == gRS485CSendGateTypeFlag)
        {
          for(i = 2; i < 34; i++)
          {
            gAckRS485CAskDataBuff[i] = 0xFF;
          }
        }
        if(1 == gRS485CSendGateTypeFlag)
        {
          for(i = 2; i < 18; i ++)
          {
            gAckRS485CAskDataBuff[i] = 0xFF;
          }
        }
        if(0 == gRS485CSendGateTypeFlag)
        {
          for(i = 18; i < 34; i++)
          {
            gAckRS485CAskDataBuff[i] = 0xFF;
          }
        }
        bsp_CLearAutoRegisterFloorInfo();
				return;
			}
			else if(0x99 == gRS485AV2DataBuff[16] && 0x09 == gRS485AV2DataBuff[17] && 0x09 == gRS485AV2DataBuff[18])
			{
				gNeedFlashCardFlag  = 1;  //启动刷卡模式
        if(1 == gFireFlag)
          HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
				return;
			}
      else 
      {
        if(0 == gNeedFlashCardFlag && GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4))
        {
          return ;
        }
      }
			
			
			//进行剩余有效次数的验证
			if(0 == tDataTypeFlag)
			{
				if(0x02 == gRS485AV2DataBuff[25])
				{
					cnt = 0;
					cnt = (gRS485AV2DataBuff[26] << 8) + gRS485AV2DataBuff[27];
					if(0 == cnt)
					{
#ifdef DEBUG
						printf("剩余次数不足--00\r\n");
#endif
						return;
					}
				}
			}
			if(1 == tDataTypeFlag)
			{
				if(0x02 == gRS485AV2DataBuff[26])
				{
					cnt = 0;
					cnt = (gRS485AV2DataBuff[27] << 8) + gRS485AV2DataBuff[28];
					if(0 == cnt)
					{
#ifdef DEBUG
						printf("剩余次数不足--01\r\n");
#endif
						return;
					}
				}
			}
			if(2 == tDataTypeFlag)
			{
				if(0x02 == gRS485AV2DataBuff[26])
				{
					cnt = 0;
					cnt = (gRS485AV2DataBuff[27] << 8) + gRS485AV2DataBuff[28];
					if(0 == cnt)
					{
#ifdef DEBUG
						printf("剩余次数不足--02\r\n");
#endif
						return;
					}
				}
			}
			/* 可以添加对时间的处理 */
			cnt = 0;
			/* 写权限 */
			for(i = 8; i < 16;i++)
			{
				if(gRS485AV2DataBuff[i] > 0)
				{
					cnt++;
				}
			}
			if(cnt > 1)
			{
				gManuallyRegisterFlag = 1;
				gClearTimeCnt = gCodeSendingTimeCnt;
				/* 默认手动登记在主门上 */
//				gAckRS485CAskDataBuff[2] |= (gRS485AV2DataBuff[8] << gFloorOffset);
//				gAckRS485CAskDataBuff[3] |= (gRS485AV2DataBuff[9] << gFloorOffset);
//				gAckRS485CAskDataBuff[4] |= (gRS485AV2DataBuff[10] << gFloorOffset);
//				gAckRS485CAskDataBuff[5] |= (gRS485AV2DataBuff[11] << gFloorOffset);
//				gAckRS485CAskDataBuff[6] |= (gRS485AV2DataBuff[12] << gFloorOffset);
//				gAckRS485CAskDataBuff[7] |= (gRS485AV2DataBuff[13] << gFloorOffset);
//				gAckRS485CAskDataBuff[8] |= (gRS485AV2DataBuff[14] << gFloorOffset);
//				gAckRS485CAskDataBuff[9] |= (gRS485AV2DataBuff[15] << gFloorOffset);
				
				gAckRS485CAskDataBuff[2] |= (gRS485AV2DataBuff[8] << gFloorOffset);
				gAckRS485CAskDataBuff[3] |= (gRS485AV2DataBuff[9] << gFloorOffset);
				gAckRS485CAskDataBuff[4] |= (gRS485AV2DataBuff[10] << gFloorOffset);
				gAckRS485CAskDataBuff[5] |= (gRS485AV2DataBuff[11] << gFloorOffset);
				gAckRS485CAskDataBuff[6] |= (gRS485AV2DataBuff[12] << gFloorOffset);
				gAckRS485CAskDataBuff[7] |= (gRS485AV2DataBuff[13] << gFloorOffset);
				gAckRS485CAskDataBuff[8] |= (gRS485AV2DataBuff[14] << gFloorOffset);
				gAckRS485CAskDataBuff[9] |= (gRS485AV2DataBuff[15] << gFloorOffset);
			//}
			//else
			//{
				gAckRS485CAskDataBuff[18] |= (gRS485AV2DataBuff[8] << gFloorOffset);
				gAckRS485CAskDataBuff[19] |= (gRS485AV2DataBuff[9] << gFloorOffset);
				gAckRS485CAskDataBuff[20] |= (gRS485AV2DataBuff[10] << gFloorOffset);
				gAckRS485CAskDataBuff[21] |= (gRS485AV2DataBuff[11] << gFloorOffset);
				gAckRS485CAskDataBuff[22] |= (gRS485AV2DataBuff[12] << gFloorOffset);
				gAckRS485CAskDataBuff[23] |= (gRS485AV2DataBuff[13] << gFloorOffset);
				gAckRS485CAskDataBuff[24] |= (gRS485AV2DataBuff[14] << gFloorOffset);
				gAckRS485CAskDataBuff[25] |= (gRS485AV2DataBuff[15] << gFloorOffset);
			}
			else
			{
			   for(i = 8; i < 16; i++)
				{
					if(gRS485AV2DataBuff[i] > 0)
					{
						tData =  gRS485AV2DataBuff[i];
						break;
					}
				}
				cnt = 0;
				for(i =0; i < 8; i++)
				{
					if(1 == ((tData >> i) & 0x01))
					{
						cnt++;
					}
				}
				if(cnt > 1)
				{
					gManuallyRegisterFlag = 1;
					gClearTimeCnt = gCodeSendingTimeCnt;
					/* 手动登记到主门 */
					gAckRS485CAskDataBuff[2] |= (gRS485AV2DataBuff[8] << gFloorOffset);
					gAckRS485CAskDataBuff[3] |= (gRS485AV2DataBuff[9] << gFloorOffset);
					gAckRS485CAskDataBuff[4] |= (gRS485AV2DataBuff[10] << gFloorOffset);
					gAckRS485CAskDataBuff[5] |= (gRS485AV2DataBuff[11] << gFloorOffset);
					gAckRS485CAskDataBuff[6] |= (gRS485AV2DataBuff[12] << gFloorOffset);
					gAckRS485CAskDataBuff[7] |= (gRS485AV2DataBuff[13] << gFloorOffset);
					gAckRS485CAskDataBuff[8] |= (gRS485AV2DataBuff[14] << gFloorOffset);
					gAckRS485CAskDataBuff[9] |= (gRS485AV2DataBuff[15] << gFloorOffset);
				//}
				//else
				//{
					gAckRS485CAskDataBuff[18] |= (gRS485AV2DataBuff[8] << gFloorOffset);
					gAckRS485CAskDataBuff[19] |= (gRS485AV2DataBuff[9] << gFloorOffset);
					gAckRS485CAskDataBuff[20] |= (gRS485AV2DataBuff[10] << gFloorOffset);
					gAckRS485CAskDataBuff[21] |= (gRS485AV2DataBuff[11] << gFloorOffset);
					gAckRS485CAskDataBuff[22] |= (gRS485AV2DataBuff[12] << gFloorOffset);
					gAckRS485CAskDataBuff[23] |= (gRS485AV2DataBuff[13] << gFloorOffset);
					gAckRS485CAskDataBuff[24] |= (gRS485AV2DataBuff[14] << gFloorOffset);
					gAckRS485CAskDataBuff[25] |= (gRS485AV2DataBuff[15] << gFloorOffset);
				}
				else
				{
						/* 自动登记到副门 */
					gAutoRegisterFlag = 1;
          if(0 == gManuallyRegisterFlag || gClearTimeCnt < 70)
          {
            gClearTimeCnt = 80; //时间改为0.8s
          }
					gAckRS485CAskDataBuff[10] |= (gRS485AV2DataBuff[8] << gFloorOffset);
					gAckRS485CAskDataBuff[11] |= (gRS485AV2DataBuff[9] << gFloorOffset);
					gAckRS485CAskDataBuff[12] |= (gRS485AV2DataBuff[10] << gFloorOffset);
					gAckRS485CAskDataBuff[13] |= (gRS485AV2DataBuff[11] << gFloorOffset);
					gAckRS485CAskDataBuff[14] |= (gRS485AV2DataBuff[12] << gFloorOffset);
					gAckRS485CAskDataBuff[15] |= (gRS485AV2DataBuff[13] << gFloorOffset);
					gAckRS485CAskDataBuff[16] |= (gRS485AV2DataBuff[14] << gFloorOffset);
					gAckRS485CAskDataBuff[17] |= (gRS485AV2DataBuff[15] << gFloorOffset);
				//}
				//else
				//{
					gAckRS485CAskDataBuff[26] |= (gRS485AV2DataBuff[8] << gFloorOffset);
					gAckRS485CAskDataBuff[27] |= (gRS485AV2DataBuff[9] << gFloorOffset);
					gAckRS485CAskDataBuff[28] |= (gRS485AV2DataBuff[10] << gFloorOffset);
					gAckRS485CAskDataBuff[29] |= (gRS485AV2DataBuff[11] << gFloorOffset);
					gAckRS485CAskDataBuff[30] |= (gRS485AV2DataBuff[12] << gFloorOffset);
					gAckRS485CAskDataBuff[31] |= (gRS485AV2DataBuff[13] << gFloorOffset);
					gAckRS485CAskDataBuff[32] |= (gRS485AV2DataBuff[14] << gFloorOffset);
					gAckRS485CAskDataBuff[33] |= (gRS485AV2DataBuff[15] << gFloorOffset);
				}
			}
			
			/* 应答RS485A的命令 */
			/* 控制GPIO 闭合继电器KA1 */
			HAL_GPIO_WritePin(KA1_GPIO_Port,KA1_Pin,GPIO_PIN_SET);
			/* 应答RS485A的命令 */
			bsp_RS485A_AckCmd();
			if(1 != cnt)
			{
				gKA1TimCnt = 60;
			}
			gKA2ActionFlag = 1;
			gKA1Flag = 1; //用于定时器打开KA1控制
			gKA1TimCnt = 60;
		}
	}
}

void bsp_RS485C_PrashDataA(void)
{
	uint8_t i = 0;
	if(0 == gFireFlag)
	{
		for(i = 2; i < 34; i ++)
		{
			gAckRS485CAskDataBuff[i] = 0xFF;
		}
		bsp_CLearAutoRegisterFloorInfo();
		/* 添加控制KA2的代码*/
		bsp_RS485C_AckData();
		/* 添加控制KA2的代码 */
		
		return;
	}
}

/* 处理RS485B接收到的数据 */
void bsp_RS485B_PrashData(void)
{
  uint8_t tempFloor = 0;
  uint8_t tempOffset = 0;
  uint8_t i = 0;
  if(gRS485BReceiveInfo.RX_Flag)
  {
    gRS485BReceiveInfo.RX_Flag = 0;
    if(0 == gFireFlag || 0 == gNeedFlashCardFlag)
    {
#ifdef DEBUG
        printf("there is a Fire Alarm flag!\r\n");
#endif
      return;
    }
    if(0x55 != gRS485BReceiveInfo.RX_pData[0] || 0xFF != gRS485BReceiveInfo.RX_pData[1])
    {
      return ;
    }
    //0xA8类指令
    if(0xA8 == gRS485BReceiveInfo.RX_pData[3])
    {
        /* 添加校验 */
         if(gRS485BReceiveInfo.RX_pData[19] != getSUMCode(gRS485BReceiveInfo.RX_pData,19))
         {
           return;
         }
        /* 处理源楼层 */
        if(gRS485BReceiveInfo.RX_pData[7] >> 7)//负楼层
        {
          tempFloor = gNagivateFloorOffset - (gRS485BReceiveInfo.RX_pData[7] & 0x7F);
          gAckRS485CAskDataBuff[10] |= (0x01 << tempFloor);
          gAckRS485CAskDataBuff[26] |= (0x01 << tempFloor);
        }
        else{
          tempFloor = (gRS485BReceiveInfo.RX_pData[7] + gNagivateFloorOffset - 1) / 8;
          tempOffset = (gRS485BReceiveInfo.RX_pData[7] + gNagivateFloorOffset - 1) % 8;
          gAckRS485CAskDataBuff[10 + tempFloor] |= (0x01 << tempOffset);
          gAckRS485CAskDataBuff[26 + tempFloor] |= (0x01 << tempOffset);
        }
        
        gAutoRegisterFlag = 1;
        if(0 == gManuallyRegisterFlag || gClearTimeCnt < 70)
        {
          gClearTimeCnt = 80;
        }
      
        /* 处理目标楼层 */
        gRS485BData[0] |= (gRS485BReceiveInfo.RX_pData[10] << (gNagivateFloorOffset - 1));//处理负楼层
        for(i = 0; i < 8; i++)
        {
          gRS485BData[i] |= (gRS485BReceiveInfo.RX_pData[i + 11] << gNagivateFloorOffset);
        }
        gRevRS485BDelayFlag = 1;
        bsp_RS485B_AckCmd(gRS485BReceiveInfo.RX_pData[3],gRS485BReceiveInfo.RX_pData[4]);
        return;
    }
    else if(0xA9 == gRS485BReceiveInfo.RX_pData[3])
    {
      /* 添加校验 */
      if(gRS485BReceiveInfo.RX_pData[12] != getSUMCode(gRS485BReceiveInfo.RX_pData,12))
      {
        return;
      }
      if(gRS485BReceiveInfo.RX_pData[5] >> 7)//负楼层
      {
        tempFloor = gNagivateFloorOffset - (gRS485BReceiveInfo.RX_pData[5] & 0x7F);
        gAckRS485CAskDataBuff[10] |= (0x01 << tempFloor);
        gAckRS485CAskDataBuff[26] |= (0x01 << tempFloor);
      }
      else{
        tempFloor = (gRS485BReceiveInfo.RX_pData[5] + gNagivateFloorOffset - 1) / 8;
        tempOffset = (gRS485BReceiveInfo.RX_pData[5] + gNagivateFloorOffset - 1) % 8;
        gAckRS485CAskDataBuff[10 + tempFloor] |= (0x01 << tempOffset);
        gAckRS485CAskDataBuff[26 + tempFloor] |= (0x01 << tempOffset);
      }
      gAutoRegisterFlag = 1;
      
      if(0x00 != gRS485BReceiveInfo.RX_pData[9])
      {
        if(gRS485BReceiveInfo.RX_pData[9] >> 7)//负楼层
        {
          tempFloor = gNagivateFloorOffset - (gRS485BReceiveInfo.RX_pData[9] & 0x7F);
          gRS485BData[0] |= (0x01 << tempFloor);
        }
        else{
          tempFloor = (gRS485BReceiveInfo.RX_pData[9] + gNagivateFloorOffset) / 8;
          tempOffset = (gRS485BReceiveInfo.RX_pData[9] + gNagivateFloorOffset - 1) % 8;
          gRS485BData[tempFloor] |= (0x01 << tempOffset);
        }
        gRevRS485BDelayFlag = 1;
      }
       bsp_RS485B_AckCmd(gRS485BReceiveInfo.RX_pData[3],gRS485BReceiveInfo.RX_pData[4]);
    }
  }
}

/* 处理RS485C接收到的数据 */
void bsp_RS485C_PrashData(void)
{
	uint8_t tXorData = 0;
	uint8_t i = 0;
	if(gRS485CReceiveInfo.RX_Flag)
	{
	  gRS485CReceiveInfo.RX_Flag = 0;
		
		/* 检测是否由火警，如果有火警则进行火警处理，开发所有楼层 */
	  if(0 == gFireFlag)
	  {
		  for(i = 2; i < 34; i ++)
		  {
			  gAckRS485CAskDataBuff[i] = 0xFF;
		  }
		  bsp_CLearAutoRegisterFloorInfo();
		  /* 添加控制KA2的代码*/
		  HAL_GPIO_WritePin(KA2_GPIO_Port,KA2_Pin,GPIO_PIN_SET);
		  bsp_RS485C_AckData();
		  /* 添加控制KA2的代码 */
		  HAL_GPIO_WritePin(KA2_GPIO_Port,KA2_Pin,GPIO_PIN_RESET);
		  return;
	  }
	  
	  if(STX == gRS485CReceiveInfo.RX_pData[0])
	  {
		   //地址过滤是否启用
		   if(gAddr != gRS485CReceiveInfo.RX_pData[1])
		   {
				return;
		   }
		   tXorData = getXORCode(gRS485CReceiveInfo.RX_pData,4);
		   if(tXorData != gRS485CReceiveInfo.RX_pData[4])
		   {
				return;
		   }
		   gRevRS485CAskFlag = 1;//标记表示接收到RS485C的查询指令
	  } 
	}
	
	if(gRevRS485CAskFlag)
	{
		/* 清除RS485C查询标记位 */
		gRevRS485CAskFlag = 0;
		/* 添加控制KA2的代码 */
		HAL_GPIO_WritePin(KA2_GPIO_Port,KA2_Pin,GPIO_PIN_SET);
		bsp_RS485C_AckData();
		HAL_Delay(1);
		bsp_RS485C_AckData();
		/* 添加控制KA2的代码 */
		HAL_GPIO_WritePin(KA2_GPIO_Port,KA2_Pin,GPIO_PIN_RESET);
		//bsp_CLearAutoRegisterFloorInfo();
	}
}

void bsp_RS485B_AckCmd(uint8_t cmd, uint8_t tID)
{
  uint8_t AckData[8];
  uint8_t i = 0;
  
  AckData[0] = 0x55;
  AckData[1] = 0xFF;
  AckData[2] = 0x03;
  AckData[3] = 0xAA;
  AckData[4] = tID;
  AckData[5] = cmd;
  AckData[6] = 0x01;
  
  for(i = 0; i < 7; i++)
  {
    AckData[7] += AckData[i];
  }
  bsp_RS485B_SendData(AckData,8);
}

void bsp_RS485C_AckData(void)
{
	gAckRS485CAskDataBuff[0] = STX;
	gAckRS485CAskDataBuff[1] = gAddr;
	gAckRS485CAskDataBuff[36] = getXORCode(gAckRS485CAskDataBuff, 36);
	bsp_RS485C_SendData(gAckRS485CAskDataBuff, 37);
}


/* 清空全部登记信息 */
void bsp_ClearAllRegisterFloorInfo(void)
{
	uint8_t i = 0;
	for(i = 2; i < 34; i ++)
	{
		gAckRS485CAskDataBuff[i] = 0;
	}
	bsp_gpioControlGetSW45Data();
}

/* 清楚自动登记信息 */
void bsp_CLearAutoRegisterFloorInfo(void)
{
	uint8_t i = 0;
	for(i = 10; i < 18; i++)
	{
		gAckRS485CAskDataBuff[i] = 0;
	}
	for( i = 26; i < 34; i++)
	{
		gAckRS485CAskDataBuff[i] = 0;
	}
}




