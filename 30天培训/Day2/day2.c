#include<stdio.h>
#define GPIO_PIN_0 ((uint16_t)0x0001)
#define GPIO_PIN_1 ((uint16_t)0x0002)
#define GPIO_PIN_2 ((uint16_t)0x0004)
#define GPIO_PIN_3 ((uint16_t)0x0008)
#define GPIO_PIN_4 ((uint16_t)0x0010)
#define GPIO_PIN_5 ((uint16_t)0x0020)
#define GPIO_PIN_6 ((uint16_t)0x0040)
#define GPIO_PIN_7 ((uint16_t)0x0080)
#define GPIO_PIN_8 ((uint16_t)0x0100)
#define GPIO_PIN_9 ((uint16_t)0x0200)
#define GPIO_PIN_10 ((uint16_t)0x0400)
#define GPIO_PIN_11 ((uint16_t)0x0800)
#define GPIO_PIN_12 ((uint16_t)0x1000)
#define GPIO_PIN_13 ((uint16_t)0x2000)
#define GPIO_PIN_14 ((uint16_t)0x4000)
#define GPIO_PIN_15 ((uint16_t)0x8000)
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef struct{
	uint8_t id;
	uint8_t on;
	uint32_t delay_ms;
}LED;

typedef enum{
	GPIO_MODE_INPUT = 0,
	GPIO_MODE_OUTPUT_PP = 1,
	GPIO_MODE_OUTPUT_OD = 2,
}GPIOMode; 

typedef enum{
	GPIO_SPEED_LOW = 0,
	GPIO_SPEED_MED = 1,
	GPIO_SPEED_HIGH = 2,
}GPIOSpeed;

typedef struct{
	uint16_t Pin;
	GPIOMode Mode;
	GPIOSpeed Speed;
}GPIO_InitTypeDef;

void set_speed_by_value(GPIO_InitTypeDef g, GPIOSpeed s){
	g.Speed = s;
}

void set_speed_by_ptr(GPIO_InitTypeDef *p, GPIOSpeed s){
	p->Speed = s;
}

int has_pin(const GPIO_InitTypeDef *p, uint16_t pin){
	return (p->Pin & pin) == pin;
}

void add_pin(GPIO_InitTypeDef *p, uint16_t pin){
	p->Pin |= pin;
}

void remove_pin(GPIO_InitTypeDef *p, uint16_t pin){
	if(p->Pin & pin == 1){
		p->Pin &= ~pin;
	}
}

void GPIO_Init(GPIO_InitTypeDef *init){
	printf("[GPIO_Init] Pin=0x%04x, Mode=", init->Pin);
	switch (init->Mode){
	case GPIO_MODE_INPUT:
		printf("输入(INPUT)");
		break;
	case GPIO_MODE_OUTPUT_PP:
		printf("推挽输出(OUTPUT_PP)");
		break;
	case GPIO_MODE_OUTPUT_OD:
		printf("开漏输出(OUTPUT_OD)");
		break;
	}
	printf(", Speed=");
	switch(init->Speed){
	case GPIO_SPEED_LOW:
		printf("低速(LOW)");
		break; 
	case GPIO_SPEED_MED:
		printf("中速(MED)");
		break; 
	case GPIO_SPEED_HIGH:
		printf("高速(HIGH)");
		break; 
	}
	printf("\n");
}

int main(){
	printf("========== 第 1 题 ==========\n");
	LED led;
	led.id = 1;
	led.on = 1;
	led.delay_ms = 500;
	printf("初始:id=%u on=%u delay_ms=%u\n", led.id, led.on, led.delay_ms);
	
	LED *p = &led;
	p->id = 2;
	p->on = 0;
	p->delay_ms = 1000;
	printf("修改后:id=%u on=%u delay_ms=%u\n", p->id, p->on, p->delay_ms);
	printf("等价验证:led.id=%u, (*p).id=%u, p->id=%u  （三者相同）\n", led.id, (*p).id, p->id);
	
	printf("\n========== 第 2 题 ==========\n");
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	set_speed_by_value(GPIO_InitStructure, GPIO_SPEED_HIGH);
	printf("传值调用后:Speed=%d（期望 LOW=0，未被修改）\n", GPIO_InitStructure.Speed);
	set_speed_by_ptr(&GPIO_InitStructure, GPIO_SPEED_HIGH);
	printf("传址调用后:Speed=%d（期望 HIGH=2，已修改）\n", GPIO_InitStructure.Speed);
	
	printf("\n========== 第 3 题 ==========\n");
	add_pin(&GPIO_InitStructure, GPIO_PIN_0);
	add_pin(&GPIO_InitStructure, GPIO_PIN_2);
	printf("添加PIN0、PIN2后:Pin=");
	printf("0x%04x\n", GPIO_InitStructure.Pin);
	printf("是否含PIN0:"); 
	if(has_pin(&GPIO_InitStructure, GPIO_PIN_0) == 1){
		printf("是\n");
	}else{
		printf("否\n");
	} 
	printf("是否含PIN1:");
	if(has_pin(&GPIO_InitStructure, GPIO_PIN_1) == 1){
		printf("是\n");
	}else{
		printf("否\n");
	}
	printf("移除PIN0后:Pin=");
	remove_pin(&GPIO_InitStructure, GPIO_PIN_0);
	printf("0x%04x\n", GPIO_InitStructure.Pin);
	printf("是否含PIN0:");
	if(has_pin(&GPIO_InitStructure, GPIO_PIN_0) == 1){
		printf("是\n");
	}else{
		printf("否\n");
	}
	
	printf("\n========== 第 4 题 ==========\n");
	printf("初始化配置:\n");
	GPIO_InitStructure.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(&GPIO_InitStructure);
	printf("修改后配置:\n");
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_Init(&GPIO_InitStructure);
	printf("\n========== 第 5 题 ==========\n");
	LED leds[4] = {
        {1, 1, 100},
        {2, 0, 200},
        {3, 0, 300},
        {4, 0, 400}
    };
    for (p = leds; p < leds + 4; p++) {
        printf("LED%u: on=%u delay_ms=%u\n", p->id, p->on, p->delay_ms);
    }
    p = leds + 2;
    p->on = 1;
    printf("打开 3 号灯后: LED%u on=%u\n", p->id, p->on);
}

