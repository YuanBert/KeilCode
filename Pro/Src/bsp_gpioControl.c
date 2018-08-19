#include "bsp_gpioControl.h"
#include "bsp_rs485x.h"

#define DEBUG
/* ��ʾ�Ƿ�ʹ�� 0 - ��ʾ�ر����ⱨ��ģʽ 1 - �ָ�ԭ״̬ */
uint8_t gSW24Flag;
/* ��ʾ�Ƿ�ʹ�� 0 - ��ʾ��Ӧ��RS485A 1 - ��ʾӦ��RS485A  */
uint8_t gRS485AAckFlag;

/* ��ʾ�Ƿ�Ҫ���� 0- ���� 1-���� */
uint8_t gRevRS485AGateTypeFlag;

/* ��ʾRS485C Ҫ���͵Ĵ������� 0 - ���� 1-���� 2-������ͬʱ */
uint8_t gRS485CSendGateTypeFlag;

/* �𾯱��λ 0-�����𾯱��� 1-�޻𾯱��� */
uint8_t  gFireFlag;

/* ��� */
uint8_t  gGroupNumber;

/* ������� */
uint8_t  gMachineNumber;

/* ��ַ */
uint8_t  gAddr;

/* ƫ������ */
uint8_t  gFloorOffset;

/* ��ƫ������ */
uint8_t  gNagivateFloorOffset;

/* ���ݱ���ʱ������ */
uint8_t  gCodeSendingTime;

/* ���ݱ���ʱ������� */
uint32_t gCodeSendingTimeCnt;

/* gpio���ƶ˿ڳ�ʼ�� */
void bsp_gpioControl_Init(void)
{
	uint8_t i;
  //HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
	/* ��ȡ�𾯱���״̬ */
	gFireFlag = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5);
	/* ��ȡ���Ÿ��ű��λ */
	gRevRS485AGateTypeFlag = HAL_GPIO_ReadPin(SW1_5_GPIO_Port,SW1_5_Pin);
	/* ��ȡ�Ƿ����RS485A���ݱ��λ */
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15))
	{
		//Ӧ��
		gRS485AAckFlag = 1;
	}
	else{
		//��Ӧ��
		gRS485AAckFlag = 0;
	}
	/* ��ȡ�ظ����� ���ŵı��λ */
	bsp_gpioControlGetRS485CSendGateTypeFlag();
	/* ��ȡ��� */
	gGroupNumber = bsp_gpioControlGetGroupNumber();
	/* ��ȡ������� */
	gMachineNumber = bsp_gpioControlGetMachineNumber();
	/* ��ȡ��ַ */
	gAddr =  bsp_gpioControlGetAddr();
	/* ��ȡ¥��ƫ������ */
	gFloorOffset = bsp_gpioControlGetFloorOffset();
  /* ��ȡ��¥��ƫ������ */
  gNagivateFloorOffset =  bsp_gpioControlGetNagitaveFloorOffset();
	/* ��ȡ���ݱ���ʱ�� */
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
	/* ��ȡ������Ϣ */
	bsp_gpioControlGetSW45Data();
  if(GPIO_PIN_SET == gFireFlag)
  {
    //HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    /* �ǻ�ģʽ*/
    bsp_ClearAllRegisterFloorInfo();
    printf("no fire alarmr\r\n");
  }
  else
  {
    //HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
        /* ���ڻ�ģʽ */
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

/* ��ȡ�𾯱��λ */
void bsp_gpioControlGetFireFlag(void)
{
	gFireFlag = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5);
}

/* ��ȡ��� */
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

/* ��ȡ������ */
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


/* ��ȡ��ַ ��1 - 4 */
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

/* ��ȡ¥��ƫ������ */
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

/* ��ȡ��¥��ƫ������ */
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

/* ��ȡ���ݱ���ʱ�� */
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


/* ��ȡ�ظ���������Ϣ */
void bsp_gpioControlGetRS485CSendGateTypeFlag(void)
{
	   if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW2_1_GPIO_Port,SW2_1_Pin))
	   {
		   //ֻ���͸��Ŵ���
		   gRS485CSendGateTypeFlag = 0;
	   }
	   else{
		   //ֻ�������Ŵ���
			gRS485CSendGateTypeFlag = 1;
	   }
	   
	   if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(SW2_2_GPIO_Port,SW2_2_Pin))
	   {
			//ͬʱ��Ӧ�����Ų�ѯ
		   gRS485CSendGateTypeFlag = 2;
	   }
}

void bsp_gpioControlGetSW45Data(void)
{
	/* ���� */
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
