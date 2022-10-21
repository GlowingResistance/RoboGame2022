#include "stm32f4xx.h"
#include "delay.h"//延时函数所在的头文件
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


u32 target[4],Target_Speed = 80; //pid初始化
extern u8 UP_STATE,DOWN_STATE,flag_go,flag; //微动开关
extern u8 L_TEN,R_TEN,F_TEN,B_TEN,series[4][6]; //通信
extern char Code_Num; //条形码
char Our_Code='0',Enemy_Code='1';

//可能会修改的参数：
u16 EasyFirst = 450,EasyFirstBack = 50;
u16 EasySec = 450,EasySecBack = 50;
u16 Easy2_1 = 400,Easy1_1 = 430,Centre = 400,CentreOut = 430; //Easy2_1代表在二号位的低难度冰壶在一号位发射时的前进距离
u16 HardGet = 380,HardPush = 400;

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();  //初始化延时函数
	
	//USART3用于与f103通信
	USART1_Init(115200);//初始化串口波特率为115200 
	USART3_Init(115200);//用于与巡线模块通信，获取对应数据
	gpio_Init();
	EXTIX_Init();       //初始化外部中断输入
	/*麦轮控制*/
	TIM4_Cap_Init(0XFFFF,84-1); //分频为1MHZ
	TIM3_Int_Init(500-1,8400-1); //每50ms发生一次中断
	/*TIM10_PF6*/
	TIM10_PWM_Init(500-1,84-1); //频率为4k
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
	
	/*电动推杆*/
	/*TIM9_CH1---PE5*/ 
	TIM9_PWM_Init(250-1,84-1); //频率为8k 电动推杆
	TIM_SetCompare1(TIM9,0);
	/*TIM5_CH2 CH3---PA1 PA2*/
	
	/*舵机*/
	TIM5_PWM_Init(4000-1,420-1); //舵机
	TIM_SetCompare2(TIM5,Steer2_Close);
	TIM_SetCompare3(TIM5,Steer3_Close);
	
	delay_ms(1000); //防止程序跑飞
	delay_ms(1000);
	delay_ms(1000);

	
	//procedure流程
	u8 code = 0;
	MoveToCentr1();
  MoveToCentr2();
	MoveToCentr3();
	MoveToCentr4(); 
	
	/*低难度取壶区*/
	EasyPart1();  //前进到第一个取壶区还没往前走
	EasyPart2(); //抓到第一个冰壶
	code = EasyPart3();
	if(code == 0) //是敌方冰壶
	{
		EasyPart4(); //移动夹取到低难度的第二个冰壶
		EasyPart5(); //走到第一个发射区准备发射
	}
	else if(code == 1)
	{
		EasyPart6(); //走到第一个发射区准备发射
	}
	GetMoreScore_1_1(4); //移动角度准备发射
	BackToNorm_1(4);  //重新转回来
	EasyPart8();  //走到中间那个冰壶并夹取
	EasyPart9(); //将中间的冰壶拖到第二块发射区
	GetMoreScore_2(5.5); 
	BackToNorm_2(5);

	/*高难度取壶区*/
	HardPart1(); //向后走直到到达第一个高难度取壶区
	code = HardPart2();  //识别冰壶
	if(code == 1)
	{
		HardPart3(); //从第一个高难度取壶区运动到第二块发射区
		GetMoreScore_2(5.5); //发射第一个高难度的壶
		BackToNorm_2(5);
		HardPart4(); //从第二块发射区运动到第二个高难度取壶区
		code = HardPart2(); //识别冰壶
		if(code == 1)
		{
			HardPart5(); //从第二个高难度取壶区运动到第一个发射区
			GetMoreScore_1(4); //发射第二个高难度的壶
			BackToNorm_1(4);
		}
		else if(code == 0)
		{
			HardPart6(); //从第二个高难度取壶区运动到第三个高难度取壶区
			HardPart7(); //从第三个高难度取壶区运动到第一个发射区
			GetMoreScore_1(4); //发射第二个高难度的壶
			BackToNorm_1(4);
		}
	}
	else if(code == 0) //取到敌方冰壶
	{
		HardPart6(); //从一个高难度取壶区运动到第二个高难度取壶区
		HardPart8(); //从第二个高难度取壶区运动到第二个发射区
		GetMoreScore_2(5.5); //发射第一个高难度的壶
		BackToNorm_2(5);
		HardPart9(); //从第二个发射区运动到第三个高难度取壶区
	  HardPart7(); //从第三个高难度取壶区运动到第一个发射区
		GetMoreScore_1(4); //发射第二个高难度的壶
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

