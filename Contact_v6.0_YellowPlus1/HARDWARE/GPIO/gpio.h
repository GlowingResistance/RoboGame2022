#ifndef __GPIO_H
#define __GPIO_H
#include "sys.h"

void gpio_Init(void); //GPIO≥ı ºªØ		 	
#define LF1 PEout(9)	
#define LF2 PEout(8)		 
#define RF1 PFout(2)	
#define RF2 PFout(3)		 
#define LB1 PDout(10)	
#define LB2 PFout(4)		
#define RB1 PDout(9)	
#define RB2 PDout(8)	

#define LA1 PCout(9)	
#define LA2 PEout(7)		 
#define RA1 PEout(3)	
#define RA2 PEout(15) 

#endif
