#include "exti.h"
#include "delay.h" 
#include "gpio.h" 
#include "motor.h"
#include "pwm.h"

u8 UP_STATE=0,DOWN_STATE=1;
u8 flag_go=0;

//右上：PD0 右下：PD3

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

//外部中断初始化程序
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	/*配置EXTI_Line0*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);//PD0 连接到中断线0
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	
	/*配置EXTI_Line3*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);//PD3 连接到中断线3

	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置

}

//外部中断0服务程序  对应控制的是右上的行程开关
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
	EXTI_ClearITPendingBit(EXTI_Line0);//清除LINE0上的中断标志位  
}


//外部中断3服务程序  对应控制的是右下的行程开关
void EXTI3_IRQHandler(void)
{
	delay_ms(10);	//消抖
	DOWN_STATE=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3);
	if(DOWN_STATE==1)
	{
		
		DOWN_STATE=1;
		delay_ms(1000);
		actuator_stop();
	}
	if(DOWN_STATE==0)
	{
		DOWN_STATE=0;
	}
	EXTI_ClearITPendingBit(EXTI_Line3);//清除LINE3上的中断标志位  
}
