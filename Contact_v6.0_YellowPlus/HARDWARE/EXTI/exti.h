#ifndef __EXTI_H
#define __EXTI_H	 
#include "sys.h"  	

void actuator_up(void);
void actuator_down(void);
void actuator_stop(void);
void EXTIX_Init(void);
#define Steer2_Open 3680
#define Steer3_Open 3740
#define Steer2_Close 3715
#define Steer3_Close 3710
#define Steer2_VeryOpen 3640
#define Steer3_VeryOpen 3780

#endif

