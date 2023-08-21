#include "timer.h"
#include "oled.h"

int Balance_Pwm , Velocity_Pwm , Turn_Pwm;
int Movement = 0;
int Turn_Speed = 64;

extern int temp;

void TIM5_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM5, TIM_IT_Update ,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设
							 
}

void TIM5_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		mpu_dmp_get_data(&pitch, &roll, &yaw);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
		Encoder_Left=Read_Encoder(2);                            //读取编码器的值，保证输出极性一致
		Encoder_Right=Read_Encoder(3);                          //读取编码器的值

		Balance_Pwm =Vertical_Ring_PD(roll,gyrox);   							//===直立环PID控制	
		Velocity_Pwm=Vertical_speed_PI(Encoder_Left,Encoder_Right,Movement);       //===速度环PID控制	 
		Turn_Pwm =Vertical_turn_PD(Turn_Speed,gyroz); 		//===转向环PID控制
		Moto1=Balance_Pwm-Velocity_Pwm+Turn_Pwm;  	            	//===计算左轮电机最终PWM
		Moto2=Balance_Pwm-Velocity_Pwm-Turn_Pwm;      //===计算右轮电机最终PWM
		Xianfu_Pwm();  									  //===PWM限幅
		Turn_Off(roll);			 			  //===检查角度以及电压是否正常
		Set_Pwm(Moto1,Moto2);                                     //===赋值给PWM寄存器  

		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}
