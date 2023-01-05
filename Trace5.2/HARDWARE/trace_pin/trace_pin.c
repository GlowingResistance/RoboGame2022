#include "trace_pin.h"

void Trace_ReadPin_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	//PB3,PB4��λ��Ĭ��ΪJTAG���Թ��ܣ��ֶ��رգ�ҪԤ�ȿ�����ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	//ע����single�˿�ֻ��ʹ����������ˣ���⵽����ʱ����Ӧ�˿�Ϊ�͵�ƽ��Ӧ�۲�0�ĸ�����λ�á�
	
	
//PB
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|	\
																GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13| \
																GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
//PA	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|  \
																GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|	\
																GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

}


