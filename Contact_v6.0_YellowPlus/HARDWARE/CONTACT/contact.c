#include "contact.h"

//���ؼ�⼶��
//0����ͼ�μ�⣬��Ѳ������
//1��
//
u8 Contact_Control(u8 state)
{
	if(state == 0)//�������
	{
		stop();
		return 0;
	}
		
	if(state>0 && state<4)//Ѳ���˶�
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
		return 1;//��ͼ���
	}
	else return 0;
}
