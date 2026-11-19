# include <stdio.h>


typedef struct {
	
	int on;
	int delay_ms;
}LED;

int main ()
{
	system("chcp 65001");
	LED leds[4] = {{1,100},{0,200},{0,300},{0,400}};
	LED *p = leds;
	for(;p<leds + 4;p++)
	{
		printf("LED%d: on=%d  delay_ms=%d\n",p-leds+1,p->on,p->delay_ms);
	}
	p = leds + 2;
	p->on = 1;
	printf("打开3号灯后：LED3 on=%d\n",p->on);
	
	
	
	
	
	
	
	
	
	return 0;
}
