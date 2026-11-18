# include <stdio.h>
# include <stdint.h>
#include<stdlib.h>

typedef struct {

	uint8_t id;
	uint8_t on;
	uint32_t delay_ms;
}LED;

int main() 
{
	system("chcp 65001");
	LED led = {1,1,500};
	printf("初始： id=%d",led.id);
	printf("  on=%d",led.on);
	printf("  delay_ms=%u\n",led.delay_ms);
	
	LED *p = &led;
	p->id = 2;
	p->on = 0;
	p->delay_ms = 1000;
	printf("修改后： id=%d",led.id);
	printf("  on=%d",led.on);
	printf("  delay_ms=%u",led.delay_ms);
	
	
	
	
	
	return 0;
} // 简答：led.id 是通过结构体访问变量，p->id是用指针指向变量地址访问.因为led.id和（*p).id这一解指针访问等价，解指针和p->id等价，因此三者都等价。