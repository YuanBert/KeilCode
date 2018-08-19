#include "bsp_gpioControl.h"
#include "bsp_rs485x.h"

#define DEBUG
/* 表示是否使用 0 - 表示关闭特殊报警模式 1 - 恢复原状态 */
uint8_t gSW24Flag;
/* 表示是否使用 0 - 表示不应答RS485A 1 - 表示应答RS485A  */
uint8_t gRS485AAckFlag;

/* 表示是否要接收 0- 副门 1-主门 */
uint8_t gRevRS485AGateTypeFlag;

/* 表示RS485C 要发送的代码类型 0 - 副门 1-主门 2-主副门同时 */
uint8_t gRS485CSendGateTypeFlag;

/* 火警标记位 0-发生火警报警 1-无火警报警 */
uint8_t  gFireFlag;

/* 组号 */
uint8_t  gGroupNumber;

/* 机器编号 */
uint8_t  gMachineNumber;

/* 地址 */
uint8_t  gAddr;

/* 偏置设置 */
uint8_t  gFloorOffset;

/* 负偏置设置 */
uint8_t  gNagivateFloorOffset;

/* 数据保持时间设置 */
uint8_t  gCodeSendingTime;

/* 数据保持时间计数器 */
uint32_t gCodeSendingTimeCnt;

/* gpio控制端口初始化 */
void bsp_gpioControl_Init(void)
{
	uint8_t i;
  //HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
	/* 获取火警报警状态 */
	gFireFlag = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5);
	/* 获取主门副门标记位 */
	gRevRS485AGateTypeFlag = HAL_GPIO_ReadPin(SW1_5_GPIO_Port,SW1_5_Pin);
	/* 获取是否忽略RS485A数据标记位 */
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15))
	{
		//应答
		gRS485AAckFlag = 1;
	}
	else{
		//不应答
		gRS485AAckFlag = 0;
	}
	/* 获取回复主门 副门的标记位 */
	bsp_gpioControlGetRS485CSendGateTypeFlag();
	/* 获取组号 */
	gGroupNumber = bsp_gpioControlGetGroupNumber();
	/* 获取机器编号 */
	gMachineNumber = bsp_gpioControlGetMachineNumber();
	/* 获取地址 */
	gAddr =  bsp_gpioControlGetAddr();
	/* 获取楼层偏置设置 */
	gFloorOffset = bsp_gpioControlGetFloorOffset();
  /* 获取负楼层偏置设置 */
  gNagivateFloorOffset =  bsp_gpioControlGetNagitaveFloorOffset();
	/* 获取数据保持时间 */
	gCodeSendingTime = bsp_gpioControlGetCodeSendingTime();
	
	if(0 == gCodeSendingTime)
	{
		gCodeSendingTimeCnt = 120;
	}
	if(1 == gCodeSendingTime)
	{
		gCodeSendingTimeCnt = 1000;
	}
	if(2 == gCodeSendingTime)
	{
		gCodeSendingTimeCnt = 3000;
	}
	if(3 == gCodeSendingTime)
	{
		gCodeSendingTimeCnt = 6000;
	}
	/* 获取拨码信息 */
	bsp_gpioControlGetSW45Data();
  if(GPIO_PIN_SET == gFireFlag)
  {
    //HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    /* 非火警模式*/
    bsp_ClearAllRegisterFloorInfo();
    printf("no fire alarmr\r\n");
  }
  else
  {
    //HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
        /* 处在火警模式 */
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
  }
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	/* printf Debug Info */
#ifdef DEBUG
	printf("\r\nfire info is %02d\r\n",gFireFlag);
	printf("gRevRS485AGateTypeFlag is %02d\r\n",gRevRS485AGateTypeFlag);
	printf("gRS485AAckFlag is %02d\r\n",gRS485AAckFlag);
	printf("gRS485CSendGateTypeFlag is %02d\r\n",gRS485CSendGateTypeFlag);
	printf("gGroupNumber is %02d\r\n",gGroupNumber);
	printf("gMachineNumber is %02d\r\n",gMachineNumber);
	printf("gAddr is %d\r\n",gAddr);
	printf("gFloorOffset is %02d\r\n",gFloorOffset);
  printf("gNagivateFloorOffset is %02d\r\n",gNagivateFloorOffset);
	printf("gCodeSendingTimeCnt is %02d\r\n",gCodeSendingTimeCnt);
	printf("gAckRS485CAskDataBuff[2] is %03d\r\n",gAckRS485CAskDataBuff[2]);//,gAckRS485CAskDataBuff[3],gAckRS485CAskDataBuff[4]);
	printf("gAckRS485CAskDataBuff[3] is %03d\r\n",gAckRS485CAskDataBuff[3]);
	printf("gAckRS485CAskDataBuff[4] is %03d\r\n",gAckRS485CAskDataBuff[4]);
	printf("gAckRS485CAskDataBuff[18] is %03d\r\n",gAckRS485CAskDataBuff[18]);
	printf("gAckRS485CAskDataBuff[19] is %03d\r\n",gAckRS485CAskDataBuff[19]);
	printf("gAckRS485CAskDataBuff[20] is %03d\r\n",gAckRS485CAskDataBuff[20]);
#endif
}

/* 获取火警标记位 */
void bsp_gpioControlGetFireFlag(void)
{
	gFireFlag = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5);
}

/* 获取组号 */
uint8_t bsp_gpioControlGetGroupNumber(void)
{
	uint8_t number = 0;
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW1_1_GPIO_Port,SW1_1_Pin))
	{
		number = 1;
	}
	
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW1_2_GPIO_Port,SW1_2_Pin))
	{
		 number += (1 << 1);
	}
	
	return number;
}

/* 获取机器号 */
uint8_t bsp_gpioControlGetMachineNumber(void)
{
	uint8_t number= 0;
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW1_3_GPIO_Port,SW1_3_Pin))
	{
		number = 1;
	}
	
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW1_4_GPIO_Port,SW1_4_Pin))
	{
		 number += (1 << 1);
	}
	return number;
}


/* 获取地址 ：1 - 4 */
uint8_t bsp_gpioControlGetAddr(void)
{
	uint8_t number= 0;
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW1_7_GPIO_Port,SW1_7_Pin))
	{
		number = 1;
	}
	
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW1_8_GPIO_Port,SW1_8_Pin))
	{
		 number += (1 << 1);
	}
	return number + 1;
}

/* 获取楼层偏置设置 */
uint8_t bsp_gpioControlGetFloorOffset(void)
{
	uint8_t offset = 0;
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW2_4_GPIO_Port,SW2_4_Pin))
	{
		offset = 1;
	}
	
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW2_5_GPIO_Port,SW2_5_Pin))
	{
		 offset += (1 << 1);
	}
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW2_6_GPIO_Port,SW2_6_Pin))
	{
		offset += (1 << 2);
	}
	
	return offset;
}

/* 获取负楼层偏置设置 */
uint8_t bsp_gpioControlGetNagitaveFloorOffset(void)
{
  uint8_t offset = 0;
  if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10))
	{
		offset = 1;
	}
	
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9))
	{
		 offset += (1 << 1);
	}
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13))
	{
		offset += (1 << 2);
	}
	
	return offset; 

}

/* 获取数据保持时间 */
uint8_t bsp_gpioControlGetCodeSendingTime(void)
{
	uint8_t number = 0;
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW2_7_GPIO_Port,SW2_7_Pin))
	{
		number = 1;
	}
	
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW2_8_GPIO_Port,SW2_8_Pin))
	{
		 number += (1 << 1);
	}
	return number;
}


/* 获取回复主副门信息 */
void bsp_gpioControlGetRS485CSendGateTypeFlag(void)
{
	   if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW2_1_GPIO_Port,SW2_1_Pin))
	   {
		   //只发送副门代码
		   gRS485CSendGateTypeFlag = 0;
	   }
	   else{
		   //只发送主门代码
			gRS485CSendGateTypeFlag = 1;
	   }
	   
	   if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW2_2_GPIO_Port,SW2_2_Pin))
	   {
			//同时相应主副门查询
		   gRS485CSendGateTypeFlag = 2;
	   }
}

void bsp_gpioControlGetSW45Data(void)
{
	/* 主门 */
	if(gRevRS485AGateTypeFlag)
	{
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_1_GPIO_Port,SW4_1_Pin))
		{
			gAckRS485CAskDataBuff[2] |= 0x01;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_2_GPIO_Port,SW4_2_Pin))
		{
			gAckRS485CAskDataBuff[2] |= 0x02;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_3_GPIO_Port,SW4_3_Pin))
		{
			gAckRS485CAskDataBuff[2] |= 0x04;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_4_GPIO_Port,SW4_4_Pin))
		{
			gAckRS485CAskDataBuff[2] |= 0x08;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_5_GPIO_Port,SW4_5_Pin))
		{
			gAckRS485CAskDataBuff[2] |= 0x10;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_6_GPIO_Port,SW4_6_Pin))
		{
			gAckRS485CAskDataBuff[2] |= 0x20;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_7_GPIO_Port,SW4_7_Pin))
		{
			gAckRS485CAskDataBuff[2] |= 0x40;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_8_GPIO_Port,SW4_8_Pin))
		{
			gAckRS485CAskDataBuff[2] |= 0x80;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_9_GPIO_Port,SW4_9_Pin))
		{
			gAckRS485CAskDataBuff[3] |= 0x01;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_10_GPIO_Port,SW4_10_Pin))
		{
			gAckRS485CAskDataBuff[3] |= 0x02;
		}
		/**********/
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_1_GPIO_Port,SW5_1_Pin))
		{
			gAckRS485CAskDataBuff[3] |= 0x04;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_2_GPIO_Port,SW5_2_Pin))
		{
			gAckRS485CAskDataBuff[3] |= 0x08;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_3_GPIO_Port,SW5_3_Pin))
		{
			gAckRS485CAskDataBuff[3] |= 0x10;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_4_GPIO_Port,SW5_4_Pin))
		{
			gAckRS485CAskDataBuff[3] |= 0x20;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_5_GPIO_Port,SW5_5_Pin))
		{
			gAckRS485CAskDataBuff[3] |= 0x40;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_6_GPIO_Port,SW5_6_Pin))
		{
			gAckRS485CAskDataBuff[3] |= 0x80;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_7_GPIO_Port,SW5_7_Pin))
		{
			gAckRS485CAskDataBuff[4] |= 0x01;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_8_GPIO_Port,SW5_8_Pin))
		{
			gAckRS485CAskDataBuff[4] |= 0x02;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_9_GPIO_Port,SW5_9_Pin))
		{
			gAckRS485CAskDataBuff[4] |= 0x04;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_10_GPIO_Port,SW5_10_Pin))
		{
			gAckRS485CAskDataBuff[4] |= 0x08;
		}
		
	}
	else{
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_1_GPIO_Port,SW4_1_Pin))
		{
			gAckRS485CAskDataBuff[18] |= 0x01;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_2_GPIO_Port,SW4_2_Pin))
		{
			gAckRS485CAskDataBuff[18] |= 0x02;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_3_GPIO_Port,SW4_3_Pin))
		{
			gAckRS485CAskDataBuff[18] |= 0x04;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_4_GPIO_Port,SW4_4_Pin))
		{
			gAckRS485CAskDataBuff[18] |= 0x08;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_5_GPIO_Port,SW4_5_Pin))
		{
			gAckRS485CAskDataBuff[18] |= 0x10;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_6_GPIO_Port,SW4_6_Pin))
		{
			gAckRS485CAskDataBuff[18] |= 0x20;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_7_GPIO_Port,SW4_7_Pin))
		{
			gAckRS485CAskDataBuff[18] |= 0x40;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_8_GPIO_Port,SW4_8_Pin))
		{
			gAckRS485CAskDataBuff[18] |= 0x80;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_9_GPIO_Port,SW4_9_Pin))
		{
			gAckRS485CAskDataBuff[19] |= 0x01;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW4_10_GPIO_Port,SW4_10_Pin))
		{
			gAckRS485CAskDataBuff[19] |= 0x02;
		}
		/**********/
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_1_GPIO_Port,SW5_1_Pin))
		{
			gAckRS485CAskDataBuff[19] |= 0x04;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_2_GPIO_Port,SW5_2_Pin))
		{
			gAckRS485CAskDataBuff[19] |= 0x08;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_3_GPIO_Port,SW5_3_Pin))
		{
			gAckRS485CAskDataBuff[19] |= 0x10;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_4_GPIO_Port,SW5_4_Pin))
		{
			gAckRS485CAskDataBuff[19] |= 0x20;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_5_GPIO_Port,SW5_5_Pin))
		{
			gAckRS485CAskDataBuff[19] |= 0x40;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_6_GPIO_Port,SW5_6_Pin))
		{
			gAckRS485CAskDataBuff[19] |= 0x80;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_7_GPIO_Port,SW5_7_Pin))
		{
			gAckRS485CAskDataBuff[20] |= 0x01;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_8_GPIO_Port,SW5_8_Pin))
		{
			gAckRS485CAskDataBuff[20] |= 0x02;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_9_GPIO_Port,SW5_9_Pin))
		{
			gAckRS485CAskDataBuff[20] |= 0x04;
		}
		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW5_10_GPIO_Port,SW5_10_Pin))
		{
			gAckRS485CAskDataBuff[20] |= 0x08;
		}
	}
}
