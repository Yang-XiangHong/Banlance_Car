#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "HW504.h"
#include "usart.h"
#include "control.h"
#include "mpu6050.h"
#include "inv_mpu.h"

uint8_t rec_dat;

uint16_t L_X, L_Y , R_X, R_Y;
uint8_t SW;

int main(void)
{
	OLED_Init();
	HW504_Init();
	uart_init(9600);
	
	OLED_Clear();

	OLED_ShowString(1, 1, "X:");
	OLED_ShowString(2, 1, "Y:");
	OLED_ShowString(3, 1, "SW:");
	
	OLED_ShowString(1, 9, "X:");
	OLED_ShowString(2, 9, "Y:");
	OLED_ShowString(3, 9, "SW:");
//	
	OLED_ShowString(4, 1, "Flag:");
	

	while (1)
	{
		SendData(flag);
			
		JoyStick_Order();
		
		L_X = HW504_L_X();
		L_Y = HW504_L_Y();
		
		R_X = HW504_R_X();
		SW = HW504_R_SW();
		R_Y = HW504_R_Y();
		
		OLED_ShowNum(1,3, L_X, 1);
		OLED_ShowNum(2,3, L_Y, 1);
		OLED_ShowNum(3,5, SW, 1);
		
		OLED_ShowNum(1,11, R_X, 1);
		OLED_ShowNum(2,11, R_Y, 1);
		OLED_ShowNum(3,12, SW, 1);
		
		OLED_ShowHexNum(4,9, flag, 1);
		
	}
}
