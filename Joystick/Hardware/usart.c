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
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (int8_t) ch;      
	return ch;
}


/* 发送字节函数 */
void send_byte(uint8_t byte)
{
	USART_SendData(USART1, byte);
	/* 等待发送完成 */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}
/* 发送字符串函数 */
void send_string(uint8_t *str)
{
	while(*str != '\0')
	{
		send_byte(*str++);
	}
}
/* 发送数组函数 */
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
	
	data[0] = 0xa3;		//帧头
	data[1] = 0xb3;		//帧头
	data[3] = 0xc3;		//帧尾
	
	while(i<4)
	{
		USART_SendData(USART1, data[i++]);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);	//等待发送完成，避免发送过快
	}
}

void send_pack(void)
{
	send_byte(0xa3);	/* 帧头 */
	send_byte(0xb3);	/* 帧头 */
	
	send_byte(flag);
	
	send_byte(0xc3);	/* 帧尾 */
}

void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
	
} 


