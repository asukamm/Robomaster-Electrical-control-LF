#include <stdio.h>
#include <stdint.h>
#define GPIO_PIN_0  ((uint16_t)0x0001)
#define GPIO_PIN_1  ((uint16_t)0x0002)
#define GPIO_PIN_2  ((uint16_t)0x0004)
#define GPIO_PIN_3  ((uint16_t)0x0008)

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

int has_pin(const GPIO_InitTypeDef *p,uint16_t pin){	
	if ((p->Pin & pin)== pin) {
	return 1;
}
	return 0;
}
void add_pin(GPIO_InitTypeDef *p,uint16_t pin)
{
	p->Pin |= pin;
}
void remove_pin(GPIO_InitTypeDef *p,uint16_t pin)
{
	p->Pin &= ~pin;
}
int main ()
{
	system("chcp 65001");
	 GPIO_InitTypeDef g;
	 g.Pin = 0;
	 add_pin(&g, GPIO_PIN_0);
    add_pin(&g, GPIO_PIN_2);
    printf("添加PIN0和PIN2后: 0x%04X\n", g.Pin);
    if (has_pin(&g, GPIO_PIN_0))
	 {
        printf("包含 PIN0\n");
    } 
	else 
	{
        printf("不包含 PIN0\n");
    }

    if (has_pin(&g, GPIO_PIN_1)) 
	{
        printf("包含 PIN1\n");
    } 
	else{
        printf("不包含 PIN1\n");
		 
    }
    remove_pin(&g, GPIO_PIN_0);
    printf("移除PIN0后的Pin值: 0x%04X\n", g.Pin);
    
     return 0;
}
