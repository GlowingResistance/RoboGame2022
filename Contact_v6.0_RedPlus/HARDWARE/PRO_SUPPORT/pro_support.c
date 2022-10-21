#include "procedure.h"
#include "motor.h"
#include "delay.h"
#include "exti.h"
#include "pro_support.h"

extern u8 series[4][6],F_TEN,R_TEN,B_TEN,L_TEN,BBL,BBR; //通信传回的数据
extern char Code_Num; //用于条形码
extern char Our_Code,Enemy_Code;
extern u8 flag_go,DOWN_STATE,UP_STATE; //用于电动推杆
u8 fuckflag = 0;
extern u32 Target_Speed;


u8 sum_location(u8 i)
{
	u8 d;
	int loc_sum;
	loc_sum = 0;
	for(d=0;d<6;d++)
	{
		loc_sum += series[i][d];
	}
	if(loc_sum == 0) return 0;
	else if(loc_sum != 0) return 1;
}

u8 most_right(u8 i) //从小的一端开始数
{
	u8 d;
	for(d=0;d<6;d++)
	{
		if(series[i][d]!=0)
			return d;
	}
	if(d == 6)
	{
		if(i == 1 || i == 3)
		{
			if(sum_location(0) == 1) return 5;
			else if(sum_location(2) == 1) return 0;
			else return 5;
		}
		if(i == 0 || i == 2)
		{
			if(sum_location(1) == 1) return 0;
			else if(sum_location(3) == 1) return 5;
			else return 5;
		}
	}
}

u8 most_front(u8 i) //从大的一端开始数
{
	u8 d;
	for(d=5;d>0;d--)
	{
		if(series[i][d]!=0)
			return d;
	}
	return 0;
}

u8 NumOfLight(u8 i) //数亮的个数
{
	u8 d;
	u8 Num = 0;
	for(d=0;d<6;d++)
	{
		if(series[i][d] != 0)
			Num ++;
	}
	return Num;
}

void SteerControl(u8 i)  //进行舵机的控制
{ 
	if(i == 1) //加紧
	{
		TIM_SetCompare2(TIM5,3715);
	  TIM_SetCompare3(TIM5,3710);
	}
	else if(i == 2) //正常张开
	{
		TIM_SetCompare2(TIM5,3675);
	  TIM_SetCompare3(TIM5,3740);
	}
	else if(i == 3) //张开到非常大
	{
		TIM_SetCompare2(TIM5,3640);
	  TIM_SetCompare3(TIM5,3780);
	}
	else if(i == 4) //发射冰壶的特殊角度
	{
		TIM_SetCompare2(TIM5,3670);
	  TIM_SetCompare3(TIM5,3740);
	}
	else if(i == 5) //发射冰壶的特殊角度
	{
		TIM_SetCompare2(TIM5,3670);
	  TIM_SetCompare3(TIM5,3735);
	}
//	u8 r;
//	for(r = 0; r < 1; r++) //舵机的机械响应速度比较慢
//	{
		delay_ms(500);
//	}
} 

void FireTheGoal(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
	delay_ms(500);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
  delay_ms(4000);
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	delay_ms(500);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	delay_ms(4000);
}

void PerformBetter(void)
{
	fuckflag = 1;
	u8 R_MM,L_MM;
	R_MM = most_right(1);
	L_MM = most_right(3);
	while(R_MM!=L_MM)
	{
		if(R_MM < L_MM)
			clockwise();
		else if(R_MM > L_MM)
			anti_clockwise();
		R_MM = most_right(1);
	  L_MM = most_right(3);
		if(R_MM != L_MM)
				delay_ms(20);
	}
	stop();
	u8 F_MM;
	F_MM = most_right(0);
	while(!F_TEN)
	{
		if(F_MM <= 2)
			right();
		else if(F_MM >= 3)
			left();
		F_MM = most_right(0);
		if(!F_TEN)
		  delay_ms(20);
	} 
	stop();
	u8 L_MS;
	L_MS = most_right(3);
	while(!L_TEN)
	{
		if(L_MS <= 2)
			backward();
		else if(L_MS >= 3)
			forward();
		L_MS = most_right(3);
		if(!L_TEN)
			delay_ms(20);
	}
	stop();
	fuckflag = 0;
}

void PerformBetter1(void)
{
	fuckflag = 1;
	u8 R_MM,L_MM;
	R_MM = most_right(1);
	L_MM = most_right(3);
	while(R_MM!=L_MM)
	{
		if(R_MM < L_MM)
			clockwise();
		else if(R_MM > L_MM)
			anti_clockwise();
		R_MM = most_right(1);
	  L_MM = most_right(3);
		if(R_MM != L_MM)
				delay_ms(20);
	}
	stop();
	u8 F_MM;
	F_MM = most_right(0);
	while(!F_TEN)
	{
		if(F_MM <= 2)
			right();
		else if(F_MM >= 3)
			left();
		F_MM = most_right(0);
		if(!F_TEN)
		  delay_ms(20);
	} 
	stop();
	u8 L_MS;
	L_MS = most_right(3);
	while(L_MS != 4)
	{
		if(L_MS < 4)
			backward();
		else if(L_MS > 4)
			forward();
		L_MS = most_right(3);
		if(L_MS != 4)
			delay_ms(20);
	}
	stop();
	fuckflag = 0;
}

//此函数中会将目标速度改为80
void move_lr()
{
	Target_Speed = 80;
	fuckflag = 1;
	u8 ff_mm = most_right(0);
	while(!F_TEN)
	{
		if(ff_mm <= 2)
			right();
		else if(ff_mm >= 3)
			left();
		if(!F_TEN)
			delay_ms(20);
	}
	stop();
	fuckflag = 0;
}

void pass_one_line(void)
{
	u8 F_test,B_test;
	F_test = 0;B_test = 0;
	while((!F_TEN)&&(!(F_test&&B_test)))
	{
		if(series[0][4]+series[0][5]>0)
			F_test = 1;
		else if(series[0][0]+series[0][1]>0)
			B_test = 1;
	}
}

void pass_one_line_hard(void)
{
	u8 R_test = 0,L_test = 0;
	while((!L_TEN)&&(!(R_test&&L_test)))
	{
		if(series[3][4]+series[3][5]>0)
			R_test = 1;
		else if(series[3][0]+series[3][1]>0)
			L_test = 1;
	}
}