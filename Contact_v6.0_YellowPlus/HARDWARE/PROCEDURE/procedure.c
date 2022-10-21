#include "procedure.h"
#include "motor.h"
#include "delay.h"
#include "exti.h"
#include "pro_support.h"

extern u8 series[4][6],F_TEN,R_TEN,B_TEN,L_TEN,BBL,BBR; //通信传回的数据
extern u32 Target_Speed;
extern char Code_Num; //用于条形码
extern char Our_Code,Enemy_Code;
extern u8 flag_go,DOWN_STATE,UP_STATE; //用于电动推杆
u8 F_test,B_test;
extern u8 flag;
extern u16 EasyFirst,EasyFirstBack,EasySec,EasySecBack,Easy2_1,Easy1_1,Centre,CentreOut;
extern u8 fuckflag;

void MoveToCentr1(void)
{
	Target_Speed = NormalSpeed;
	right();
	F_test = 0;B_test = 0;
	while((!B_TEN)&&(!(F_test&&B_test)))
	{
		if(series[2][4]+series[2][5]>0)
			F_test = 1;
		else if(series[2][0]+series[2][1]>0)
			B_test = 1;
	}
	delay_ms(800); //走出本次的黑线
//	delay_ms(200);
	F_test = 0;B_test = 0;
	while((!B_TEN)&&(!(F_test&&B_test)))
	{
		if(series[2][4]+series[2][5]>0)
			F_test = 1;
		else if(series[2][0]+series[2][1]>0)
			B_test = 1;
	}
	stop();
	delay_ms(200);
}

void MoveToCentr2(void)
{
	fuckflag = 1;
	if(sum_location(0)==0)
		backward();
	while(!sum_location(0))
	{
	}
	stop();
	u8 F_MR,B_MR;
	F_MR = most_right(0);
	B_MR = most_right(2);
	while(F_MR!=B_MR)
	{
		if(F_MR < B_MR)
			clockwise();
		else if(F_MR > B_MR)
			anti_clockwise();
		F_MR = most_right(0);
	  B_MR = most_right(2);
		if(F_MR != B_MR)
				delay_ms(20);
	}
	stop();
	B_MR = most_right(2);
	while(B_MR != 2)
	{
		if(B_MR < 2)
			right();
		else if(B_MR > 2)
			left();
		B_MR = most_right(2);
		if(B_MR != 2)
		  delay_ms(20);
	} 
	stop();
	run_backward(20);
	//////////////////////////////
	F_MR = most_right(0);
	B_MR = most_right(2);
	while(F_MR!=B_MR)
	{
		if(F_MR < B_MR)
			clockwise();
		else if(F_MR > B_MR)
			anti_clockwise();
		F_MR = most_right(0);
	  B_MR = most_right(2);
		if(F_MR != B_MR)
				delay_ms(20);
	}
	stop();
	B_MR = most_right(2);
	while(B_MR != 2)
	{
		if(B_MR < 2)
			right();
		else if(B_MR > 2)
			left();
		B_MR = most_right(2);
		if(B_MR != 2)
		  delay_ms(20);
	} 
	stop();
	delay_ms(800);
	fuckflag =0;
}

void MoveToCentr3(void)
{
		Target_Speed = NormalSpeed;
		pid_Init();
	  Target_Speed = FastSpeed;
	  run_backward(5200); 
}

void MoveToCentr4(void)
{
	Target_Speed = NormalSpeed;
	backward();
	while(!L_TEN)
	{
	}
	stop();
	u8 R_MR,L_MR;
	R_MR = most_right(1);
	L_MR = most_right(3);
	while(L_MR!=R_MR)
	{
		if(L_MR < R_MR)
			anti_clockwise();
		else if(L_MR > R_MR)
			clockwise();
		L_MR = most_right(3);
	  R_MR = most_right(1);
		if(R_MR != L_MR)
				delay_ms(20);
	}
	stop();
	while(!L_TEN)
	{
		if(L_MR <= 2)
			backward();
		else if(L_MR >= 3)
			forward();
		L_MR = most_right(3);
		if(!L_TEN)
		  delay_ms(20);
	} 
	stop();
	left();
}

void EasyPart1(void)
{
	pass_one_line();
	stop();
	PerformBetter();
	SteerControl(5); //正常张开
}

void EasyPart2(void)
{
	Target_Speed = SlowSpeed;
	run_forward(EasyFirst);
	move_lr();
	Target_Speed = SlowSpeed;
	run_forward(10);
	run_backward(EasyFirstBack);
	DOWN_STATE = 0;
	actuator_down();
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
	SteerControl(1); //爪子闭合
}

u8 EasyPart3(void)
{
	Code_Num='3';
	u16 num = 0;
	while(1)
	{
		delay_ms(100);
		num++;
		if((Code_Num != '3') || num > 10)
		  break;
	}
	if(Code_Num==Enemy_Code||Code_Num == '3')
	{
		return 0; //如果识别到敌方冰壶返回0
	}
	else if(Code_Num == Our_Code)
	{
		return 1;
	}
}

void EasyPart4(void)
{
	Target_Speed = NormalSpeed;
	SteerControl(2); //舵机正常打开
	flag_go = 1; //让电动推杆升起来
	actuator_up();
	UP_STATE = 0;
	while(!UP_STATE)
	{
	}
	actuator_stop();
	backward();
	while(!L_TEN)
	{
	}
	stop();
	PerformBetter();
	left();
	delay_ms(800);
//	delay_ms(200);
	pass_one_line();
	stop();
	PerformBetter();
	Target_Speed = SlowSpeed;
	run_forward(EasySec);
	stop();
	move_lr();
	Target_Speed = SlowSpeed;
	run_forward(10);
	run_backward(EasySecBack);
	DOWN_STATE = 0;
	actuator_down();
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
	actuator_stop();
	SteerControl(1); //爪子闭合
}

void EasyPart5(void)
{
	flag_go = 1; //让电动推杆升起来
	actuator_up();
	UP_STATE = 0;
	while(!UP_STATE)
	{
	}
	actuator_stop();
	Target_Speed = NormalSpeed;
	backward();
	while(!L_TEN)
	{
	}
	stop();
	PerformBetter();
	left();
	delay_ms(800); //线走出这一次的黑线
//	delay_ms(200);
	pass_one_line();
	stop();
	PerformBetter();
	Target_Speed = SlowSpeed;
	run_forward(Easy2_1); 
	move_lr();
	DOWN_STATE = 0;
	actuator_down();
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
	actuator_stop();
}

void EasyPart6(void)
{
	flag_go = 1; //让电动推杆升起来
	actuator_up();
	UP_STATE = 0;
	while(!UP_STATE)
	{
	}
	actuator_stop();
	Target_Speed = NormalSpeed;
	backward();
	while(!R_TEN)
	{
	}
	stop();
	PerformBetter1();
	left();
	delay_ms(800);
//	delay_ms(200);
	pass_one_line();
//	while(!F_TEN)
//	{
//	}
	delay_ms(800);
//	delay_ms(200);
	pass_one_line();
//	while(!F_TEN)
//	{
//	}
	stop();
	PerformBetter1();
	Target_Speed = SlowSpeed;
	run_forward(Easy1_1);
	stop();
	move_lr();
	DOWN_STATE = 0;
	actuator_down();
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
}

void EasyPart8(void)
{
	SteerControl(2); //舵机正常张开
	flag_go = 1; //让电动推杆升起来
	actuator_up();
	UP_STATE = 0;
	while(!UP_STATE)
	{
	}
	actuator_stop();
	Target_Speed = NormalSpeed;
	backward();
	while(!R_TEN)
	{
	}
	stop();
	PerformBetter1();
	left();
	delay_ms(800);
//	delay_ms(200);
	pass_one_line();
	stop();
	PerformBetter1();
	SteerControl(3); //张开到非常大
	Target_Speed = SlowSpeed;
	run_forward(Centre);
	move_lr();
	DOWN_STATE = 0;
	actuator_down();
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
	SteerControl(1); //爪子闭合
}

void EasyPart9(void)
{
	flag_go = 1; //让电动推杆升起来
	actuator_up();
	UP_STATE = 0;
	while(!UP_STATE)
	{
	}
	actuator_stop();
	Target_Speed = NormalSpeed;
	backward();
	while(!L_TEN)
	{
	}
	stop();
	PerformBetter1();
	left();
	delay_ms(800); //走出这一次的黑线
//	delay_ms(200);
	pass_one_line();
	stop();
	PerformBetter1();
	Target_Speed = SlowSpeed;
	run_forward(CentreOut);
	move_lr();
	DOWN_STATE = 0;
	actuator_down();
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
}
 
void GetMoreScore_1(float spin)
{
	
	SteerControl(1); //舵机闭合
	Target_Speed = SlowSpeed;
	run_anticlockwise(spin);
	while(flag)
	{
	}
	SteerControl(3); //张开到非常大
	FireTheGoal();
}

void GetMoreScore_2(float spin)
{
	SteerControl(1); //舵机闭合
	Target_Speed = SlowSpeed;
	run_clockwise(spin);
	while(flag)
	{
	}
	SteerControl(3); //舵机张开到非常大
	FireTheGoal();
}

void GetMoreScore_1_1(float spin)
{
	SteerControl(1); //舵机闭合
	Target_Speed = SlowSpeed;
	run_anticlockwise(spin);
	while(flag)
	{
	}
	SteerControl(4); //舵机张开到特殊角度
	FireTheGoal();
}

void BackToNorm_1(float spin)
{
	Target_Speed = SlowSpeed;
	run_clockwise(spin);
	while(flag)
	{
	}
}



void BackToNorm_2(float spin)
{
	Target_Speed = SlowSpeed;
	run_anticlockwise(spin);
	while(flag)
	{
	}
}
