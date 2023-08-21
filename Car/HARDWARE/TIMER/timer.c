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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM5, TIM_IT_Update ,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx����
							 
}

void TIM5_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		mpu_dmp_get_data(&pitch, &roll, &yaw);
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
		Encoder_Left=Read_Encoder(2);                            //��ȡ��������ֵ����֤�������һ��
		Encoder_Right=Read_Encoder(3);                          //��ȡ��������ֵ

		Balance_Pwm =Vertical_Ring_PD(roll,gyrox);   							//===ֱ����PID����	
		Velocity_Pwm=Vertical_speed_PI(Encoder_Left,Encoder_Right,Movement);       //===�ٶȻ�PID����	 
		Turn_Pwm =Vertical_turn_PD(Turn_Speed,gyroz); 		//===ת��PID����
		Moto1=Balance_Pwm-Velocity_Pwm+Turn_Pwm;  	            	//===�������ֵ������PWM
		Moto2=Balance_Pwm-Velocity_Pwm-Turn_Pwm;      //===�������ֵ������PWM
		Xianfu_Pwm();  									  //===PWM�޷�
		Turn_Off(roll);			 			  //===���Ƕ��Լ���ѹ�Ƿ�����
		Set_Pwm(Moto1,Moto2);                                     //===��ֵ��PWM�Ĵ���  

		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}
