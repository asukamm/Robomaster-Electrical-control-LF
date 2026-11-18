#include <stdio.h>
#include <stdint.h>

typedef enum {
    GPIO_MODE_INPUT     = 0,  
    GPIO_MODE_OUTPUT_PP = 1,  
    GPIO_MODE_OUTPUT_OD = 2,  
} GPIOMode;
 
typedef enum {
    GPIO_SPEED_LOW  = 0,
	GPIO_SPEED_MED = 1,
	GPIO_SPEED_HIGH = 2,
}  GPIOSpeed ;
 
typedef struct {
    uint16_t  Pin;  
    GPIOMode  Mode;
    GPIOSpeed Speed;
} GPIO_InitTypeDef;

void X(GPIO_InitTypeDef g, GPIOSpeed s){
	g.Speed = s;
}
void Y(GPIO_InitTypeDef *p,GPIOSpeed s){
	p->Speed = s;
}
int main ()
{
	GPIO_InitTypeDef speed = { 0,1,0};
	X(speed,2);
	printf("传值调用后：Speed=%d\n",speed.Speed);
	Y(&speed,2);
	printf("传址调用后：Speed=%d",speed.Speed);
	
	
	
	
	
	
	
	
	return 0;	
}
