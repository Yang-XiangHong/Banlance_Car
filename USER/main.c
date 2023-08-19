#include "led.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "motor.h"
#include "oled.h"
#include "pwm.h"
#include "timer.h"
#include "encoder.h"
#include "mpu6050.h"
#include "inv_mpu.h"

int Moto1=0, Moto2=0;
float pitch, roll, yaw;
short accx, accy, accz;
short gyrox, gyroy, gyroz;
short Encoder_Right, Encoder_Left;

uint8_t CMD;

int temp;

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(9600);
	delay_init();	    	 //延时函数初始化	
	Motor_Init();
	OLED_Init();
	LED_Init();
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	
	MPU_Init();
	while(mpu_dmp_init())
	{
		OLED_ShowString(2, 2, "MPU6050 error!", 12);
		delay_ms(200);
		OLED_Clear();
		delay_ms(200);
	}
	TIM1_PWM_Init(7199, 0);
	TIM5_Int_Init(71, 9999);
	OLED_Clear();
	while(1)
	{	
		OLED_ShowString(2, 2, "roll:", 12);
		OLED_Float(50, 2, roll, 1);
	}
}

