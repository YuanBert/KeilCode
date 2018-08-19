#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

#include "stm32f1xx_hal.h"
#include "main.h"

uint32_t EnterCritical(void)
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}

void ExitCritical(uint32_t status)
{
	__set_PRIMASK(status);
}


#endif
