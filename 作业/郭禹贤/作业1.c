#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t id;
    uint8_t on;
    uint32_t delay_ms;
} LED;

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT_PP = 1,
    GPIO_MODE_OUTPUT_OD = 2
} GPIOMode;

typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MED = 1,
    GPIO_SPEED_HIGH = 2
} GPIOSpeed;

typedef struct {
    uint16_t Pin;
    GPIOMode Mode;
    GPIOSpeed Speed;
} GPIO_InitTypeDef;

   
void set_speed_by_value(GPIO_InitTypeDef g, GPIOSpeed s);
void set_speed_by_ptr(GPIO_InitTypeDef *p, GPIOSpeed s);

int has_pin(const GPIO_InitTypeDef *p, uint16_t pin);
void add_pin(GPIO_InitTypeDef *p, uint16_t pin);
void remove_pin(GPIO_InitTypeDef *p, uint16_t pin);

void GPIO_Init(GPIO_InitTypeDef *init);

int main(void)
{
    printf("1.\n");

    LED led = {1, 1, 500};
    LED *p = &led;

    printf("初始: id=%d on=%d delay_ms=%d\n",led.id, led.on,led.delay_ms);

    p->id = 2;
    p->on = 0;
    p->delay_ms = 1000;

    printf("修改: id=%d on=%d delay_ms=%d\n",led.id, led.on,led.delay_ms);

    printf("验证: led.id=%d, (*p).id=%d, p->id=%d\n\n",led.id,(*p).id,p->id);

//led.id 使用结构体变量的成员访问运算符 .   p->id 使用结构体指针的成员访问运算符->  两者等价

    printf("2.\n");

    GPIO_InitTypeDef gpio2 = {
        .Pin = 0,
        .Mode = GPIO_MODE_INPUT,
        .Speed = GPIO_SPEED_LOW
    };

    set_speed_by_value(gpio2, GPIO_SPEED_HIGH);
    printf("传值调用后: Speed=%d\n", gpio2.Speed);

    set_speed_by_ptr(&gpio2, GPIO_SPEED_HIGH);
    printf("传址调用后: Speed=%d\n", gpio2.Speed);
//传值调用时，函数得到的是实参的副本，修改副本不会影响原变量；传址调用时，函数通过指针直接访问原变量，因此能够修改原变量的值。
    printf("3.\n");
  	
	#define GPIO_PIN_0  ((uint16_t)0x0001)
	#define GPIO_PIN_1  ((uint16_t)0x0002)
	#define GPIO_PIN_2  ((uint16_t)0x0004)
	#define GPIO_PIN_3  ((uint16_t)0x0008)
	#define GPIO_PIN_4  ((uint16_t)0x0010)
	#define GPIO_PIN_5  ((uint16_t)0x0020)
	#define GPIO_PIN_6  ((uint16_t)0x0040)
	#define GPIO_PIN_7  ((uint16_t)0x0080)
	#define GPIO_PIN_8  ((uint16_t)0x0100)
	#define GPIO_PIN_9  ((uint16_t)0x0200)
	#define GPIO_PIN_10 ((uint16_t)0x0400)
	#define GPIO_PIN_11 ((uint16_t)0x0800)
	#define GPIO_PIN_12 ((uint16_t)0x1000)
	#define GPIO_PIN_13 ((uint16_t)0x2000)
	#define GPIO_PIN_14 ((uint16_t)0x4000)
	#define GPIO_PIN_15 ((uint16_t)0x8000)
    GPIO_InitTypeDef gpio3 = {0};

    add_pin(&gpio3, GPIO_PIN_0);
    add_pin(&gpio3, GPIO_PIN_2);

    printf("添加 PIN0、PIN2 后: Pin=0x%04X\n",gpio3.Pin);

    printf("是否含 PIN0: %s\n", has_pin(&gpio3, GPIO_PIN_0) ? "是" : "否");

    printf("是否含 PIN1: %s\n",has_pin(&gpio3, GPIO_PIN_1) ? "是" : "否");

    remove_pin(&gpio3, GPIO_PIN_0);

    printf("移除 PIN0 后: Pin=0x%04X\n",gpio3.Pin);

    printf("是否含 PIN0: %s\n", has_pin(&gpio3, GPIO_PIN_0) ? "是" : "否");

    printf("4.\n");

    GPIO_InitTypeDef gpio4 = {
        .Pin = GPIO_PIN_0 | GPIO_PIN_1,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_HIGH
    };

    printf("初始配置:\n");
    GPIO_Init(&gpio4);

    gpio4.Pin = GPIO_PIN_3;
    gpio4.Mode = GPIO_MODE_OUTPUT_OD;

    printf("修改后配置:\n");
    GPIO_Init(&gpio4);
//使用指针可以避免复制整个结构体，提高效率，并且能够直接访问和修改原结构体成员

    printf("5.\n");

    LED leds[4] = {
        {1, 1, 100},
        {2, 0, 200},
        {3, 0, 300},
        {4, 0, 400}
    };

    LED *pl = leds;

    for (; pl < leds + 4; pl++)
    {
        printf("LED%d: on=%d delay_ms=%lu\n",
               pl->id, pl->on, (unsigned long)pl->delay_ms);
    }

    pl = leds + 2;
    pl->on = 1;

    printf("打开 3 号灯后: LED%d on=%d\n", pl->id, pl->on);

    return 0;
}

void set_speed_by_value(GPIO_InitTypeDef g, GPIOSpeed s)
{
    g.Speed = s;
}

void set_speed_by_ptr(GPIO_InitTypeDef *p, GPIOSpeed s)
{
    if (p != NULL)
        p->Speed = s;
}

int has_pin(const GPIO_InitTypeDef *p, uint16_t pin)
{
    if (p == NULL || pin == 0)
        return 0;

    return ((p->Pin & pin) == pin);
}

void add_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    if (p != NULL)
        p->Pin |= pin;
}

void remove_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    if (p != NULL)
        p->Pin &= (uint16_t)(~pin);
}

void GPIO_Init(GPIO_InitTypeDef *init)
{
    if (init == NULL) {
        printf("  [GPIO_Init] NULL pointer!\n");
        return;
    }

    const char *modeStr;
    const char *speedStr;

    switch (init->Mode)
    {
        case GPIO_MODE_INPUT:
            modeStr = "INPUT";
            break;
        case GPIO_MODE_OUTPUT_PP:
            modeStr = "OUTPUT_PP";
            break;
        case GPIO_MODE_OUTPUT_OD:
            modeStr = "OUTPUT_OD";
            break;
        default:
            modeStr = "UNKNOWN";
    }

    switch (init->Speed)
    {
        case GPIO_SPEED_LOW:
            speedStr = "LOW";
            break;
        case GPIO_SPEED_MED:
            speedStr = "MED";
            break;
        case GPIO_SPEED_HIGH:
            speedStr = "HIGH";
            break;
        default:
            speedStr = "UNKNOWN";
    }

    printf("  [GPIO_Init] Pin=0x%04X, Mode=%s, Speed=%s\n",init->Pin, modeStr, speedStr);
}