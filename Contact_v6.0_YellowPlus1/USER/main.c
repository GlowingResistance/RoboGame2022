#include "stm32f4xx.h"
#include "delay.h"//��ʱ�������ڵ�ͷ�ļ�
#include "usart.h"
#include "gpio.h"
#include "pwm.h"
#include "motor.h"
#include "usart3.h"
#include "exti.h"
#include "key.h"
#include "led.h"
#include "timer.h"
#include "procedure.h"
#include "pro_support.h"
#include "pro_buff.h"


u32 target[4],Target_Speed = 80; //pid��ʼ��
extern u8 UP_STATE,DOWN_STATE,flag_go,flag; //΢������
extern u8 L_TEN,R_TEN,F_TEN,B_TEN,series[4][6]; //ͨ��
extern char Code_Num; //������
char Our_Code='1',Enemy_Code='0';

//���ܻ��޸ĵĲ�����
u16 EasyFirst = 450,EasyFirstBack = 50;
u16 EasySec = 450,EasySecBack = 50;
u16 Easy2_1 = 400,Easy1_1 = 430,Centre = 400,CentreOut = 430; //Easy2_1�����ڶ���λ�ĵ��Ѷȱ�����һ��λ����ʱ��ǰ������
u16 HardGet = 360,HardPush = 400;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();  //��ʼ����ʱ����
	
	//USART3������f103ͨ��
	USART1_Init(115200);//��ʼ�����ڲ�����Ϊ115200 
	USART3_Init(115200);//������Ѳ��ģ��ͨ�ţ���ȡ��Ӧ����
	gpio_Init();
	EXTIX_Init();       //��ʼ���ⲿ�ж�����
	/*���ֿ���*/
	TIM4_Cap_Init(0XFFFF,84-1); //��ƵΪ1MHZ
	TIM3_Int_Init(500-1,8400-1); //ÿ50ms����һ���ж�
	/*TIM10_PF6*/
	TIM10_PWM_Init(500-1,84-1); //Ƶ��Ϊ4k
	TIM_SetCompare1(TIM10,0);
	/*TIM11_PF7*/
	TIM11_PWM_Init(500-1,84-1);
	TIM_SetCompare1(TIM11,0);
	/*TIM13_PF8*/
	TIM13_PWM_Init(500-1,84-1);
	TIM_SetCompare1(TIM13,0);
	/*TIM12_PB15*/
	TIM12_PWM_Init(500-1,84-1);
	TIM_SetCompare2(TIM12,0);
	
	/*�綯�Ƹ�*/
	/*TIM9_CH1---PE5*/ 
	TIM9_PWM_Init(250-1,84-1); //Ƶ��Ϊ8k �綯�Ƹ�
	TIM_SetCompare1(TIM9,0);
	/*TIM5_CH2 CH3---PA1 PA2*/
	
	/*���*/
	TIM5_PWM_Init(4000-1,420-1); //���
	TIM_SetCompare2(TIM5,Steer2_Close);
	TIM_SetCompare3(TIM5,Steer3_Close);
	
	delay_ms(1000); //��ֹ�����ܷ�
	delay_ms(1000);
	delay_ms(1000);

	
	//procedure����
	u8 code = 0;
	MoveToCentr1();
  MoveToCentr2();
	MoveToCentr3();
	MoveToCentr4(); 
	
	/*���Ѷ�ȡ����*/
	EasyPart1();  //ǰ������һ��ȡ������û��ǰ��
	EasyPart2(); //ץ����һ������
	code = EasyPart3();
	if(code == 0) //�ǵз�����
	{
		EasyPart4(); //�ƶ���ȡ�����Ѷȵĵڶ�������
		EasyPart5(); //�ߵ���һ��������׼������
	}
	else if(code == 1)
	{
		EasyPart6(); //�ߵ���һ��������׼������
	}
	GetMoreScore_1_1(4); //�ƶ��Ƕ�׼������
	BackToNorm_1(4);  //����ת����
	EasyPart8();  //�ߵ��м��Ǹ���������ȡ
	EasyPart9(); //���м�ı����ϵ��ڶ��鷢����
	GetMoreScore_2(5.5); 
	BackToNorm_2(5);

	/*���Ѷ�ȡ����*/
	HardPart1(); //�����ֱ�������һ�����Ѷ�ȡ����
	code = HardPart2();  //ʶ�����
	if(code == 1)
	{
		HardPart3(); //�ӵ�һ�����Ѷ�ȡ�����˶����ڶ��鷢����
		GetMoreScore_2(5.5); //�����һ�����Ѷȵĺ�
		BackToNorm_2(5);
		HardPart4(); //�ӵڶ��鷢�����˶����ڶ������Ѷ�ȡ����
		code = HardPart2(); //ʶ�����
		if(code == 1)
		{
			HardPart5(); //�ӵڶ������Ѷ�ȡ�����˶�����һ��������
			GetMoreScore_1(4); //����ڶ������Ѷȵĺ�
			BackToNorm_1(4);
		}
		else if(code == 0)
		{
			HardPart6(); //�ӵڶ������Ѷ�ȡ�����˶������������Ѷ�ȡ����
			HardPart7(); //�ӵ��������Ѷ�ȡ�����˶�����һ��������
			GetMoreScore_1(4); //����ڶ������Ѷȵĺ�
			BackToNorm_1(4);
		}
	}
	else if(code == 0) //ȡ���з�����
	{
		HardPart6(); //��һ�����Ѷ�ȡ�����˶����ڶ������Ѷ�ȡ����
		HardPart8(); //�ӵڶ������Ѷ�ȡ�����˶����ڶ���������
		GetMoreScore_2(5.5); //�����һ�����Ѷȵĺ�
		BackToNorm_2(5);
		HardPart9(); //�ӵڶ����������˶������������Ѷ�ȡ����
	  HardPart7(); //�ӵ��������Ѷ�ȡ�����˶�����һ��������
		GetMoreScore_1(4); //����ڶ������Ѷȵĺ�
		BackToNorm_1(4);
	}
	UP_STATE = 0;
	flag_go = 1;
	actuator_up();
	while(!UP_STATE)
	{
	}
	actuator_stop();
	
  while(1) 
	{
		
	}
}

