#include "procedure.h"
#include "motor.h"
#include "delay.h"
#include "exti.h"
#include "pro_support.h"

extern u8 series[4][6],F_TEN,R_TEN,B_TEN,L_TEN,BBL,BBR; //ͨ�Ŵ��ص�����
extern u32 Target_Speed;
extern char Code_Num; //����������
extern char Our_Code,Enemy_Code;
extern u8 flag_go,DOWN_STATE,UP_STATE; //���ڵ綯�Ƹ�
u8 R_test,L_test;
extern u8 flag;
extern u16 HardGet,HardPush;

void HardPart1(void)
{
	SteerControl(1); //����պ�
	UP_STATE = 0;
	flag_go = 1; /////////////////////////////////////////
	actuator_up();
	while(!UP_STATE)
	{
	}
	actuator_stop();
	Target_Speed = NormalSpeed;
	backward();
	pass_one_line_hard();
	stop();
	PerformBetter();
	clockwise();
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter();
	SteerControl(3); //����ſ����ǳ���
	Target_Speed = SlowSpeed;
	run_forward(HardGet);
	move_lr();
	SteerControl(1); //����պ�
}

u8 HardPart2(void)
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
		return 0; //���ʶ�𵽵з���������0
	}
	else if(Code_Num == Our_Code)
	{
		return 1;
	}
}

void HardPart3(void)
{
	Target_Speed = NormalSpeed;
	backward();
	pass_one_line_hard();
	stop();
	PerformBetter();
	anti_clockwise();
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter();
	Target_Speed = SlowSpeed;
	run_forward(HardPush);
	move_lr();
	DOWN_STATE = 0;
	actuator_down();
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
}

void HardPart4(void)
{
	SteerControl(1);
	UP_STATE = 0;
	flag_go = 1; 
	actuator_up();
	while(!UP_STATE)
	{
	}
	actuator_stop();
	Target_Speed = NormalSpeed;
	backward();
	pass_one_line_hard();
	stop();
	clockwise();
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	delay_ms(800); //�߳��˴κ���
	delay_ms(500);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter();
	left();
	delay_ms(800);
//	delay_ms(200);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter();
	SteerControl(3);
	Target_Speed = SlowSpeed;
	run_forward(HardGet);
	move_lr();
	SteerControl(1);
}

void HardPart5(void)
{
	Target_Speed = NormalSpeed;
	backward();
	pass_one_line_hard();
	stop();
	anti_clockwise();
	delay_ms(800);
	while(!F_TEN)
	{
	}
	delay_ms(800); //�߳��˴κ���
	delay_ms(500);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter1();
	right();
	delay_ms(800);
//	delay_ms(200);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter1();
	Target_Speed = SlowSpeed;
	run_forward(HardPush);
	move_lr();
	DOWN_STATE = 0;
	actuator_down();
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
}

void HardPart6(void)
{
	SteerControl(2); //��������ſ�
	Target_Speed = NormalSpeed;
	backward();
	pass_one_line_hard();
	stop();
	PerformBetter();
	left();
	delay_ms(800);
//	delay_ms(200);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter();
	SteerControl(3); //����ſ����ǳ���
	Target_Speed = SlowSpeed;
	run_forward(HardGet);
	move_lr();
	SteerControl(1); //����պ�
}

void HardPart7(void)
{
	Target_Speed = NormalSpeed;
	backward();
	pass_one_line_hard();
	stop();
	PerformBetter();
	anti_clockwise();
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter1();
	Target_Speed = SlowSpeed;
	run_forward(HardPush); 
	stop();
	move_lr();
	actuator_down();
	DOWN_STATE = 0;
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
}

void HardPart8(void)
{
	Target_Speed = NormalSpeed;
	backward();
	pass_one_line_hard();
	stop();
	PerformBetter();
	clockwise();
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter1();
	left();
	delay_ms(800);
//	delay_ms(200);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter1();
	Target_Speed = SlowSpeed;
	run_forward(HardPush); 
	move_lr();
	actuator_down();
	DOWN_STATE = 0;
	while(!DOWN_STATE)
	{
	}
	delay_ms(500);
}

void HardPart9(void)
{
	SteerControl(1); //����պ�
	flag_go = 1;
	UP_STATE = 0;
	actuator_up();
	while(!UP_STATE)
	{
	}
	actuator_stop();
	Target_Speed = NormalSpeed;
	backward();
	pass_one_line_hard();
	stop();
	PerformBetter();
	clockwise();
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	delay_ms(800);
	delay_ms(500);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter();
	left();
	delay_ms(800);
//	delay_ms(200);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter();
	left();
	delay_ms(800);
//	delay_ms(200);
	while(!F_TEN)
	{
	}
	stop();
	PerformBetter();
	SteerControl(3);
	Target_Speed = SlowSpeed;
	run_forward(HardGet); 
	move_lr();
	SteerControl(1);
}

