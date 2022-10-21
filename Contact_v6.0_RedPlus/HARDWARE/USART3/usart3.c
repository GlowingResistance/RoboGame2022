#include "usart3.h"
#include "BinaryToHex.h"
#include "usart.h"


u8 series[4][6],low,notes,y,j,F_TEN,R_TEN,B_TEN,L_TEN,BBL,BBR;
u8 important_data = B00000000;
u16 contact_data;
extern char Code_Num;

void USART3_Init(u32 bound)
{
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART2ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //����ΪUSART2-TX
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //����ΪUSART2-RX
	
	//USART2�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

void USART3_IRQHandler(void)                	//����3�жϷ������
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
		series[y][0]=(low & B00100000)>>5; //0�洢���λ
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
