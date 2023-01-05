#include "stm32f10x.h"
#include "sys.h"
#include "trace_tim.h"
#include "usart3.h"
#include "delay.h"
#include "trace_pin.h"

uint16_t State[2]={0};
int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART3_Init(115200); //PB10->TX    PB11->RX
	Trace_ReadPin_GPIO_Init();
	Trace_TIM2_Init(49999,71); //50ms
	while(1)
	{
//		Trace_Detect();
		State[0] = GPIO_ReadInputData(GPIOA);
		State[1] = GPIO_ReadInputData(GPIOB);
//		NumDisplay(state);//注意Trace_tim.c中state值的变化
	}
}
