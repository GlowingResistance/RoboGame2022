#include "contact.h"

//返回检测级别
//0：先图形检测，再巡线驱动
//1：
//
u8 Contact_Control(u8 state)
{
	if(state == 0)//传输错误
	{
		stop();
		return 0;
	}
		
	if(state>0 && state<4)//巡线运动
	{
		switch(state)
		{
			case 1:
				forward();
				break;
			case 2:
				clockwise();
				break;
			case 3:
				anti_clockwise();
				break;
		}
		return 1;//最低级别
	}
	else return 0;
}
