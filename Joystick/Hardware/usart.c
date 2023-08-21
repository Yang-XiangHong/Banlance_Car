#include "usart.h"
#include "stdio.h"
#include "oled.h"

extern int flag;

#define DATA_HEAD	0xEF
#define DATA_TAIL	0xFE

extern uint8_t dat_Rxd;
extern uint8_t dat;

void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (int8_t) ch;      
	return ch;
}


/* �����ֽں��� */
void send_byte(uint8_t byte)
{
	USART_SendData(USART1, byte);
	/* �ȴ�������� */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
/* �����ַ������� */
void send_string(uint8_t *str)
{
	while(*str != '\0')
	{
		send_byte(*str++);
	}
}
/* �������麯�� */
void send_buf(uint8_t *buf, uint16_t len)
{
	uint16_t i;
	for(i=0; i<len; i++)
	{
		send_byte(buf[i]);
	}
}

void SendData(int rec_data)
{
	int i = 0;
	int data[4];
	
	data[2] = rec_data;
	
	data[0] = 0xa3;		//֡ͷ
	data[1] = 0xb3;		//֡ͷ
	data[3] = 0xc3;		//֡β
	
	while(i<4)
	{
		USART_SendData(USART1, data[i++]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);	//�ȴ�������ɣ����ⷢ�͹���
	}
}

void send_pack(void)
{
	send_byte(0xa3);	/* ֡ͷ */
	send_byte(0xb3);	/* ֡ͷ */
	
	send_byte(flag);
	
	send_byte(0xc3);	/* ֡β */
}

void uart_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	
} 


