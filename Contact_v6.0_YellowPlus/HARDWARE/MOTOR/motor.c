#include "motor.h"
#include "stdlib.h"


//轮子采用‘X’型安装方法


extern u32 target[4],Target_Speed;
u32 count[4],out[4],last_out[4];
int error[4],p=0,last_err[4];
extern u8 fuckflag;

void pid_Init(void)
{
	if(!fuckflag)
	{
		TIM_SetCompare1(TIM10,0);
		TIM_SetCompare1(TIM11,0);
		TIM_SetCompare1(TIM13,0);
		TIM_SetCompare2(TIM12,0);
	}
	
	int k=0;
	for(k=0;k<4;k++)
	{
		target[k]=Target_Speed;
		count[k]=0;
		last_out[k]=0;
		out[k]=0;
		error[k]=0;
		last_err[k] = 0;
	}
}
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

float kp = 0.6, kd = 0.5;
u32 amount = 0;
extern u32 end;
extern u8 flag;

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		//amount += count[2];
		amount += count[3];  //tzh
		for(p=0;p<4;p++)
		{ 
			if(flag == 1 && amount > end)
			{
				flag = 0;
				stop();
			} //用于进行一定距离的行走
			error[p]=target[p]-count[p];
			if(error[p]<-50) error[p]=-50; //用于error的限值
			if(error[p]>50)error[p]=50;
			out[p]=(u32)((int)(last_out[p])+(int)(kp*error[p])+(int)(kd*(last_err[p]-error[p])));
		  if(Target_Speed > 60)
			{
				if(out[p]>Target_Speed+200) out[p]=Target_Speed + 200;
				if(p == 1)
					if(out[p]>Target_Speed+150) out[p]=Target_Speed +150;
			}
			else
			{
				if(out[p]>Target_Speed+60) out[p]=Target_Speed + 60;
			}
			if(out[p]<(int)(0.2*Target_Speed)) out[p]=(int)(0.2*Target_Speed);
			last_out[p]=out[p];
			count[p]=0;
			last_err[p] = error[p];
			if(fuckflag)
				out[p]=60;
			
		}
		TIM_SetCompare1(TIM10,out[0]);
		TIM_SetCompare1(TIM11,out[1]);
		TIM_SetCompare1(TIM13,out[2]);
		TIM_SetCompare2(TIM12,out[3]);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

void forward(void)
{
	pid_Init();
	if(Target_Speed <= 40)
	{
		kp = 0.4;
		kd  = 0.15;
		target[0] += 3;
		target[1] += 2;
		target[2] += 1;
		target[3] += 2;
	}
	LF1=1;LF2=0;
	RF1=1;RF2=0;
	LB1=1;LB2=0;
	RB1=1;RB2=0;
}


void backward(void)
{
	pid_Init();
	if(Target_Speed > 60)
	{
		target[0] += 2;
		target[1] += 1;
		target[2] += 1;
		target[3] += 3;
	}
	if(Target_Speed > 100)
	{
		target[0] += -4;
		target[1] += 3;
		target[2] += -4;
		target[3] += 7;
	}
	LF1=0;LF2=1;
	RF1=0;RF2=1;
	LB1=0;LB2=1;
	RB1=0;RB2=1;
}

void right(void)
{
	pid_Init();
	if(Target_Speed > 60)
	{
		target[0] += -2;
		target[1] += 2;
		target[2] += 2;
		target[3] += -2;
	}
	LF1=1;LF2=0;
	RF1=0;RF2=1;
	LB1=0;LB2=1;
	RB1=1;RB2=0;
}

void left(void)
{
	pid_Init();
	if(Target_Speed > 60)
	{
		target[0] += 0;
		target[1] += 2;
		target[2] += 6;
		target[3] += 0;
	}
	LF1=0;LF2=1;
	RF1=1;RF2=0;
	LB1=1;LB2=0;
	RB1=0;RB2=1;
}

void anti_clockwise(void)
{
	pid_Init();
	LF1=0;LF2=1;
	RF1=1;RF2=0;
	LB1=0;LB2=1;
	RB1=1;RB2=0;
}

void clockwise(void)
{
	pid_Init();
	LF1=1;LF2=0;
	RF1=0;RF2=1;
	LB1=1;LB2=0;
	RB1=0;RB2=1;
}

void stop(void)
{
	LF1=0;LF2=0;
	RF1=0;RF2=0;
	LB1=0;LB2=0;
	RB1=0;RB2=0;
}


u32 end = 0;
u8 flag = 0;
void run_forward(u16 distance)
{
	flag = 1;
	amount = 0;
	float num = distance * 32.5 / 6.0;
	end = amount + (u32)(num);
	forward();
	while(flag)
	{
	}
}

void run_backward(u16 distance)
{
	flag = 1;
	amount = 0;
	float num = distance * 32.5 / 6.0;
	end = amount + (u32)(num);
	backward();
	while(flag)
	{
	}
}

void run_clockwise(float distance)
{
	fuckflag = 1;
	flag = 1;
	amount = 0;
	float num = distance * 420.0 / 9.0;
	end = amount + (u32)(num);
	clockwise();
	fuckflag = 0;
}

void run_anticlockwise(float distance)
{
	fuckflag = 1;
	flag = 1;
	amount = 0;
	float num = distance * 420.0 / 9.0;
	end = amount + (u32)(num);
	anti_clockwise();
	fuckflag = 0;
}
