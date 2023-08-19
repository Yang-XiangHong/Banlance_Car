#include "stm32f10x.h"                  // Device header
#include "AD.h"
#include "Delay.h"

/**************
引脚连接：

VRX - A0

VRY - A1

SW - PB0

5V- 5V

GND - GND

by: 普普通通

***************/

void HW504_Init(void)
{
	AD_Init();
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
	
}

uint8_t HW504_R_SW(void)
{
	uint8_t KeyNum = 0;
	
	KeyNum = GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_0);
	
	return KeyNum;
}

uint16_t HW504_L_X(void)
{
	uint16_t DataX = AD_GetValue(ADC_Channel_2);
	return DataX;
	
}

uint16_t HW504_L_Y(void)
{
	uint16_t DataY = AD_GetValue(ADC_Channel_3);
	return DataY;
}

uint16_t HW504_R_X(void)
{
	uint16_t DataX = AD_GetValue(ADC_Channel_0);
	return DataX;
	
}

uint16_t HW504_R_Y(void)
{
	uint16_t DataY = AD_GetValue(ADC_Channel_1);
	return DataY;
}


