#include "control.h"

int flag = 6;

void JoyStick_Order(void)
{
	if((L_X > 2500 &&L_Y < 1000)||(L_X < 1500 &&L_Y < 1000))	//上
	{
		flag = 0x01;
	}
	else if((L_X > 2500 &&L_Y > 2500)||(L_X < 1500 &&L_Y > 2500))	//下
	{
		flag = 0x02;
	}
	else if(R_X > 2500)	//下
	{
		flag = 0x03;
	}
	else if(R_X < 1500)	//下
	{
		flag = 0x04;
	}
	if(R_X > 1900&&R_Y < 1000)
		flag = 0x05;
	if(R_X > 1900&&R_Y > 3000)
		flag = 0x06;
}

