#include "control.h"

float Med_Angle = 0 ;        //机械中值

float 
    Vertical_Kp = 550,        //直立环Kp
    Vertical_Kd = 1.5;        //直立环Kd
float 
    Velocity_Kp = 60,        //速度环Kp
    Velocity_Ki = 0.3;        //速度环Ki
float
	Tur_Kp = 30,
	Tur_Kd = 0.3;

/*
直立环PD：Kp*Ek + Kd*Ek_D 
Ek：偏差   Ek_D：偏差的微分
入口：期望角度、真实角度、真实角速度
*/
int	Vertical_Ring_PD(float Angle,float Gyro)
{
	float Bias;
	int balance;
	Bias=Angle-Med_Angle;
	balance=Vertical_Kp*Bias+ Gyro*Vertical_Kd;
	
	return balance;
}

/**************************************************************************************************************
*函数名:Vertical_speed_PI()
*功能；速度环PI控制
*形参:(int encoder_left):左轮编码器值/(int encoder_right):编码器右轮的值/(float Angle):x轴角度值
*返回值:
**************************************************************************************************************/

int Vertical_speed_PI(int encoder_left,int encoder_right,float Movement )
{
	static float Velocity,Encoder_Least,Encoder;
	static float Encoder_Integral;
	Encoder_Least =(encoder_left+encoder_right)-0;    //获取最新速度偏差=测量速度（左右编码器之和）-目标速度（此处为零）
	Encoder *= 0.8f;																	//一阶低通滤波器 ，上次的速度占85%
	Encoder += Encoder_Least*0.2f;                   //一阶低通滤波器， 本次的速度占15% 
	Encoder_Integral +=Encoder;                       //积分出位移 积分时间：10ms
	Encoder_Integral=Encoder_Integral-Movement; 
	
	if(Encoder_Integral>10000)  	Encoder_Integral=10000;           //积分限幅
	if(Encoder_Integral<-10000)	  Encoder_Integral=-10000;            //积分限幅

	Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;      //速度控制
	
	Turn_Off(pitch);
	
	return Velocity;
}

/**************************************************************************************************************
*函数名:Vertical_turn_PD()
*功能:转向环PD
*形参:无  CCD小于34左转、CCD大于64右转。 yaw = z轴陀螺仪数值
*返回值:无
***************************************************************************************************************/
int Vertical_turn_PD(u8 CCD,short yaw)
{
		float Turn;     
    float Bias;	  
	  Bias=CCD-64;
	  Turn=Bias*Tur_Kp+yaw*Tur_Kd;
	  return Turn;
}

