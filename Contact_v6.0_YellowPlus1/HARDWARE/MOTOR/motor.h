#ifndef __MOTOR_H
#define __MOTOR_H

//#define Target_Speed  80   //设置轮子目标转速 <450;值越大，转速越慢

#include "sys.h"
#include "led.h"
#include "usart.h"
#include "pwm.h"
#include "gpio.h"

void pid_Init(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void forward(void);
void backward(void);
void left(void);
void right(void);
void clockwise(void);
void anti_clockwise(void);
void stop(void);
void run_forward(u16 distance);
void run_backward(u16 distance);
void run_clockwise(float distance);
void run_anticlockwise(float distance);
void Vehicle_Run(int16_t x,int16_t y,int16_t w);

#endif
