#include "control.h"

int flag = 0x00;

void JoyStick_Order(void)
{
	if(L_X == 2 &&L_Y < 2)	//上
	{
		flag = 0x01;
	}
	else if(L_X == 2 &&L_Y > 2)	//下
	{
		flag = 0x02;
	}
	else if(L_X > 2 &&L_Y == 2)	//左
	{
		flag = 0x03;
	}
	else if(L_X < 2 &&L_Y == 2)	//右
	{
		flag = 0x04;
	}
	
	if(R_X == 2 &&R_Y < 2)	//上
	{
		flag = 0x05;
	}
	else if(R_X == 2 &&R_Y > 2)	//下
	{
		flag = 0x06;
	}
	else if(R_X > 2 &&R_Y == 2)	//左
	{
		flag = 0x07;
	}
	else if(R_X < 2 &&R_Y == 2)	//右
	{
		flag = 0x08;
	}
	else if(L_X == 2 &&L_Y == 2)
	{
		flag = 0x00;
	}
	
}

