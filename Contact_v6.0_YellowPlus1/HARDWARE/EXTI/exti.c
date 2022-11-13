#include "exti.h"
#include "delay.h" 
#include "gpio.h" 
#include "motor.h"
#include "pwm.h"

u8 UP_STATE=0,DOWN_STATE=1;
u8 flag_go=0;

//���ϣ�PD0 ���£�PD3

void actuator_down(void)
{
	LA1=1;LA2=0;
	RA1=1;RA2=0;
}

void actuator_up(void)
{
	LA1=0;LA2=1;
	RA1=0;RA2=1;
}

void actuator_stop(void)
{
	LA1=0;LA2=0;
	RA1=0;RA2=0;
}

//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

	/*����EXTI_Line0*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);//PD0 ���ӵ��ж���0
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	
	/*����EXTI_Line3*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);//PD3 ���ӵ��ж���3

	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����

}

//�ⲿ�ж�0�������  ��Ӧ���Ƶ������ϵ��г̿���
void EXTI0_IRQHandler(void)
{
	UP_STATE=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0);
	if(flag_go)
	{
		flag_go=0;
		UP_STATE=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0);
		if(UP_STATE==1)
	 {
		  actuator_stop();
	 }
	}
	EXTI_ClearITPendingBit(EXTI_Line0);//���LINE0�ϵ��жϱ�־λ  
}


//�ⲿ�ж�3�������  ��Ӧ���Ƶ������µ��г̿���
void EXTI3_IRQHandler(void)
{
	delay_ms(10);	//����
	DOWN_STATE=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3);
	if(DOWN_STATE==1)
	{
		
		DOWN_STATE=1;
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		actuator_stop();
	}
	if(DOWN_STATE==0)
	{
		DOWN_STATE=0;
	}
	EXTI_ClearITPendingBit(EXTI_Line3);//���LINE3�ϵ��жϱ�־λ  
}
