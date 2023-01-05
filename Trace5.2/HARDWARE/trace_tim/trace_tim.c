#include "trace_tim.h"

extern uint16_t State[2];

void Trace_TIM2_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //�ڳ�ʼ�� TIM2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); //�ܳ�ʼ�� NVIC �Ĵ���
	
	TIM_Cmd(TIM2, ENABLE); 
}

u8 num2And3_6to5(u8 code)
{
	u8 s2 = (code >>2) & 0x01;
	u8 s3 = (code >>3) & 0x01;
	u8 final2_3 = (s2 & s3)<< 2;
	s2 = code >> 4;
	s3 = code & 0x03;
	code = (s2<<3) | final2_3 | s3; 
	return code;
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
//		u16 State[2];
//		u8 SendData[5] = {0};
//		u8 temp;
//		
//		State[0] = GPIO_ReadInputData(GPIOA);
//		State[1] = GPIO_ReadInputData(GPIOB);
//		
//		SendData[3] |= (State[0]&0x003f );
//		SendData[1] |= ((State[0]>>6) & 0x003f );
//		SendData[0] |= (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) | \
//									 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)<<1 | \
//									 (State[1]&0x000f) << 2);
//		SendData[2] |= ((State[1]>>4) & 0x003f);
//		SendData[4] |= (State[1]>>12);
//		
//		for(temp = 0;temp<5;temp ++)
//		{
//			SendData[temp] = num2And3_6to5(SendData[temp])&0x1f;
//			SendData[temp] |= temp<<5;
//		}

//		temp = 0;
////		USART_SendData(USART3,0xff);
//		while(temp < 5)//��������5����λ����
//		{
//			USART_SendData(USART3,SendData[temp]);
//			while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != SET);//����ӵ������ֹ�ϴ�����δ��ȫ����
//			temp++;
//		}
//		USART_SendData(USART3,State[0]);

		u8 SendData[4] = {0};
		//����λ���ڴ��ѭ����ţ�����λ���ڴ��ѭ����Ϣ
		u8 temp;

		SendData[3] |= (State[0]&0x003f );
		SendData[1] |= ((State[0]>>6) & 0x003f );
		SendData[0] |= (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) | \
									 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)<<1 | \
									 (State[1]&0x000f) << 2);
		SendData[2] |= ((State[1]>>4) & 0x003f);
		//SendData[4] |= (State[1]>>12);
		
		for(temp = 0;temp<4;temp ++)
		{
			//SendData[temp] = num2And3_6to5(SendData[temp])&0x1f;
			SendData[temp] |= temp<<6;
		}

		temp = 0;
//		USART_SendData(USART3,0xff);
		while(temp < 4)//��������4����λ����
		{
			USART_SendData(USART3,SendData[temp]);
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC) != SET);//����ӵ������ֹ�ϴ�����δ��ȫ����
			temp++;
		}
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

