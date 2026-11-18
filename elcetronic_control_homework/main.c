/*****************************************************************************************
凌风战队电控组 C语言作业（培训第二天）
结构体+指针--模拟 STM32 HAL 库里的 GPIO_InitTypeDef（三成员：Pin / Mode / Speed）

编写人：电控组新人杨嘉皓
*****************************************************************************************/


#include <stdio.h> 


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

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
/*==================================================================================
*第1题
==================================================================================*/
typedef struct
{
   uint8_t id;
   uint8_t on;
   uint32_t delay_ms;

}LED;
/*==================================================================================
*第2题
==================================================================================*/
typedef enum{
   GPIO_MODE_INPUT=0,
   GPIO_MODE_OUTPUT_PP=1,
   GPIO_MODE_OUTPUT_OD=2,

}GPIO_MODE;
typedef enum {
   GPIO_SPEED_LOW=0,
   GPIO_SPEED_MED=1,
   GPIO_SPEED_HIGH=2,
}GPIOSpeed;
typedef struct{
   uint16_t Pin;
   GPIO_MODE Mode;
   GPIOSpeed Speed;

}GPIO_InitTypedef;
void set_speed_by_value(GPIO_InitTypedef g,GPIOSpeed s)
{
 g.Speed=s;
}
void set_speed_by_ptr(GPIO_InitTypedef *p,GPIOSpeed s)
{
p->Speed=s;
}
/*==================================================================================
*第3题
==================================================================================*/
 int has_pin(const GPIO_InitTypedef *p,uint16_t pin)
 {
  return (p->Pin&pin)==pin;
 }
void add_pin(GPIO_InitTypedef *p,uint16_t pin)
{
   p->Pin|=pin;
}
void remove_pin(GPIO_InitTypedef *p,uint16_t pin)
{
   p->Pin&=~pin;
}
/*==================================================================================
*第4题
==================================================================================*/
const char* get_mode_str(GPIO_MODE m) {
    if (m == GPIO_MODE_INPUT) return "输入(INPUT)";
    if (m == GPIO_MODE_OUTPUT_PP) return "推挽输出(OUTPUT_PP)";
    if (m == GPIO_MODE_OUTPUT_OD) return "开漏输出(OUTPUT_OD)";
    return "未知";
}

const char* get_speed_str(GPIOSpeed s) {
    if (s == GPIO_SPEED_LOW) return "低速(LOW)";
    if (s == GPIO_SPEED_MED) return "中速(MED)";
    if (s == GPIO_SPEED_HIGH) return "高速(HIGH)";
    return "未知";
}

void GPIO_Init(GPIO_InitTypedef *init) {
    printf("[GPIO_Init] Pin=0x%04X, Mode=%s, Speed=%s\n",
           (unsigned int)init->Pin,
           get_mode_str(init->Mode),
           get_speed_str(init->Speed));
}
int main(void)
{
   printf("==========第1题==========\n");

   LED led={.id=1,.on=1,.delay_ms=500};
   printf("初始参数：id=%u，on=%u,delay_ms=%u\n",led.id,led.on,led.delay_ms);
   LED *p=&led;
   p->id=2;
   p->on=0;
   p->delay_ms=1000;
   printf("修改后：id=%u，on=%u,delay_ms=%u\n",led.id,led.on,led.delay_ms);

   printf("==========第2题==========\n");

   GPIO_InitTypedef g={.Pin=GPIO_PIN_0,.Mode=GPIO_MODE_OUTPUT_PP,.Speed=GPIO_SPEED_LOW};
   set_speed_by_value(g,GPIO_SPEED_HIGH);
   printf("修改后：speed=%u\n",g.Speed);
   set_speed_by_ptr(&g,GPIO_SPEED_HIGH);
   printf("修改后：speed=%u\n",g.Speed);

   printf("==========第3题==========\n");

   GPIO_InitTypedef gs={.Pin=0,.Mode=GPIO_MODE_OUTPUT_PP,.Speed=GPIO_SPEED_HIGH};
   add_pin(&gs,GPIO_PIN_0);
   add_pin(&gs,GPIO_PIN_2);
   printf("添加后：PIN=0x%04x\n",gs.Pin);

   printf("是否含有pin0：%s\n",has_pin(&gs,GPIO_PIN_0)?"是":"否");
   printf("是否含有pin1：%s\n",has_pin(&gs,GPIO_PIN_1)?"是":"否");

   remove_pin(&gs,GPIO_PIN_0);
   printf("移除PIN0后：PIN=0x%04X\n",gs.Pin);
   printf("是否含PIN0:%s\n\n",has_pin(&gs,GPIO_PIN_0)?"是":"否");

   printf("==========第4题==========\n");
   
   GPIO_InitTypedef GPIO_InitSturcture;
   GPIO_InitSturcture.Mode=GPIO_MODE_OUTPUT_PP;
   GPIO_InitSturcture.Pin=GPIO_PIN_0|GPIO_PIN_1;
   GPIO_InitSturcture.Speed=GPIO_SPEED_HIGH;

   printf("初始配置:\n");
   GPIO_Init(&GPIO_InitSturcture);

   GPIO_InitTypedef *Init=&GPIO_InitSturcture;
   Init->Mode=GPIO_MODE_OUTPUT_OD;
   Init->Pin=GPIO_PIN_3;

   printf("指针配置后:\n");
   GPIO_Init(&GPIO_InitSturcture);

   printf("==========第5题==========\n");

   LED leds[4]={
      {.id=1,.on=0,.delay_ms=100},
      {.id=2,.on=0,.delay_ms=200},
      {.id=3,.on=0,.delay_ms=300},
      {.id=4,.on=0,.delay_ms=400},
   };
for(LED *pl=leds;pl<leds+4;pl++)
{
   printf("id=%u,on=%u,delay_ms=%u\n",pl->id,pl->on,pl->delay_ms);
}
(leds+2)->on=1;
printf("打开三号灯后: LED%d on=%u",(leds+2)->id,(leds+2)->on);


}
