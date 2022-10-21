#include "usart3.h"
#include "BinaryToHex.h"
#include "usart.h"


u8 series[4][6],low,notes,y,j,F_TEN,R_TEN,B_TEN,L_TEN,BBL,BBR;
u8 important_data = B00000000;
u16 contact_data;
extern char Code_Num;

void USART3_Init(u32 bound)
{
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART2时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //复用为USART2-TX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //复用为USART2-RX
	
	//USART2端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA9，PA10

   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
	{
		contact_data=USART_ReceiveData(USART3);
		low=contact_data & B11111111;
		notes=(low & B11100000)>>6;
		if(notes == B00000000)
			y = 0;
		else if(notes == B00000001)
			y = 1;
		else if(notes == B00000010)
			y = 2;
		else if(notes == B00000011)
			y = 3;
		series[y][0]=(low & B00100000)>>5; //0存储最高位
		series[y][1]=(low & B00010000)>>4;
		series[y][2]=(low & B00001000)>>3;
		series[y][3]=(low & B00000100)>>2;
		series[y][4]=(low & B00000010)>>1;
		series[y][5]=(low & B00000001);
		if(y == 0) F_TEN = series[y][2]&series[y][3];
		else if(y == 1) R_TEN = series[y][2]&series[y][3];
		else if(y == 2) B_TEN = series[y][2]&series[y][3];
		else if(y == 3) L_TEN = series[y][2]&series[y][3];
  }
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
} 
