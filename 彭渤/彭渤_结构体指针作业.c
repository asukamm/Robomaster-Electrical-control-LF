#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t id;        /* LED 编号 */
    uint8_t on;        /* 1 表示亮，0 表示灭 */
    uint32_t delay_ms; /* 闪烁周期（毫秒） */
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

/* 第 n 位对应第 n 个引脚，可以用按位或组合多个引脚。 */
#define GPIO_PIN_0  ((uint16_t)(1u << 0))
#define GPIO_PIN_1  ((uint16_t)(1u << 1))
#define GPIO_PIN_2  ((uint16_t)(1u << 2))
#define GPIO_PIN_3  ((uint16_t)(1u << 3))
#define GPIO_PIN_4  ((uint16_t)(1u << 4))
#define GPIO_PIN_5  ((uint16_t)(1u << 5))
#define GPIO_PIN_6  ((uint16_t)(1u << 6))
#define GPIO_PIN_7  ((uint16_t)(1u << 7))
#define GPIO_PIN_8  ((uint16_t)(1u << 8))
#define GPIO_PIN_9  ((uint16_t)(1u << 9))
#define GPIO_PIN_10 ((uint16_t)(1u << 10))
#define GPIO_PIN_11 ((uint16_t)(1u << 11))
#define GPIO_PIN_12 ((uint16_t)(1u << 12))
#define GPIO_PIN_13 ((uint16_t)(1u << 13))
#define GPIO_PIN_14 ((uint16_t)(1u << 14))
#define GPIO_PIN_15 ((uint16_t)(1u << 15))

/* 第 2 题：g 是实参的副本，所以改动只在函数内有效。 */
void set_speed_by_value(GPIO_InitTypeDef g, GPIOSpeed s)
{
    g.Speed = s;
    (void)g; /* 本题故意只修改副本。 */
}

/* C 仍然按值传递指针，但该指针指向调用者的原结构体。 */
void set_speed_by_ptr(GPIO_InitTypeDef *p, GPIOSpeed s)
{
    p->Speed = s;
}

int has_pin(const GPIO_InitTypeDef *p, uint16_t pin)
{
    return (p->Pin & pin) == pin;
}

void add_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    p->Pin |= pin;
}

void remove_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    p->Pin &= (uint16_t)~pin;
}

static const char *mode_name(GPIOMode mode)
{
    switch (mode) {
    case GPIO_MODE_INPUT:     return "INPUT";
    case GPIO_MODE_OUTPUT_PP: return "OUTPUT_PP";
    case GPIO_MODE_OUTPUT_OD: return "OUTPUT_OD";
    default:                  return "UNKNOWN";
    }
}

static const char *speed_name(GPIOSpeed speed)
{
    switch (speed) {
    case GPIO_SPEED_LOW:  return "LOW";
    case GPIO_SPEED_MED:  return "MED";
    case GPIO_SPEED_HIGH: return "HIGH";
    default:              return "UNKNOWN";
    }
}

/*
 * 第 4 题：传指针可以直接读取调用者准备好的配置，无需复制整个结构体。
 * 这里仅打印配置，模拟初始化硬件，因此函数不修改配置本身。
 */
void GPIO_Init(GPIO_InitTypeDef *init)
{
    printf("  [GPIO_Init] Pin=0x%04X, Mode=%s, Speed=%s\n",
           (unsigned)init->Pin, mode_name(init->Mode), speed_name(init->Speed));
}

int main(void)
{
    LED led = { .id = 1, .on = 1, .delay_ms = 250 };
    LED *p = &led;

    puts("========== 1. LED structure ==========");
    printf("Before: id=%u on=%u delay_ms=%" PRIu32 "\n",
           (unsigned)led.id, (unsigned)led.on, led.delay_ms);

    p->id = 2;
    p->on = 0;
    p->delay_ms = 750;
    printf("After:  id=%u on=%u delay_ms=%" PRIu32 "\n",
           (unsigned)led.id, (unsigned)led.on, led.delay_ms);

    /* led.id 直接访问对象成员。p->id 通过指针访问，等价于 (*p).id。
       这里 p 指向 led，因此三种写法读取的是同一个成员。 */
    printf("led.id=%u, (*p).id=%u, p->id=%u\n",
           (unsigned)led.id, (unsigned)(*p).id, (unsigned)p->id);

    puts("\n========== 2. Value and pointer ==========");
    GPIO_InitTypeDef gpio = {
        .Pin = 0, .Mode = GPIO_MODE_INPUT, .Speed = GPIO_SPEED_LOW
    };

    set_speed_by_value(gpio, GPIO_SPEED_HIGH);
    printf("After passing by value:   Speed=%d (%s)\n",
           (int)gpio.Speed, speed_name(gpio.Speed));
    set_speed_by_ptr(&gpio, GPIO_SPEED_HIGH);
    printf("After passing by pointer: Speed=%d (%s)\n",
           (int)gpio.Speed, speed_name(gpio.Speed));

    puts("\n========== 3. GPIO pin mask ==========");
    add_pin(&gpio, GPIO_PIN_0);
    add_pin(&gpio, GPIO_PIN_2);
    printf("After adding PIN0 and PIN2: Pin=0x%04X\n", (unsigned)gpio.Pin);
    printf("Has PIN0: %s\n", has_pin(&gpio, GPIO_PIN_0) ? "yes" : "no");
    printf("Has PIN1: %s\n", has_pin(&gpio, GPIO_PIN_1) ? "yes" : "no");
    remove_pin(&gpio, GPIO_PIN_0);
    printf("After removing PIN0:        Pin=0x%04X\n", (unsigned)gpio.Pin);
    printf("Has PIN0: %s\n", has_pin(&gpio, GPIO_PIN_0) ? "yes" : "no");

    puts("\n========== 4. Simulated GPIO initialization ==========");
    GPIO_InitTypeDef config = {
        .Pin = GPIO_PIN_0 | GPIO_PIN_1,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_HIGH
    };
    GPIO_InitTypeDef *config_ptr = &config;

    puts("Before:");
    GPIO_Init(config_ptr);
    config_ptr->Mode = GPIO_MODE_OUTPUT_OD;
    config_ptr->Pin = GPIO_PIN_3;
    puts("After:");
    GPIO_Init(config_ptr);

    puts("\n========== 5. LED array traversal ==========");
    LED leds[4] = {
        { .id = 1, .on = 1, .delay_ms = 150 },
        { .id = 2, .on = 0, .delay_ms = 300 },
        { .id = 3, .on = 0, .delay_ms = 450 },
        { .id = 4, .on = 1, .delay_ms = 600 }
    };

    for (p = leds; p != leds + 4; ++p) {
        printf("LED%u: on=%u delay_ms=%" PRIu32 "\n",
               (unsigned)p->id, (unsigned)p->on, p->delay_ms);
    }
    p = leds + 2;
    p->on = 1;
    printf("Third LED switched on: LED%u on=%u\n",
           (unsigned)p->id, (unsigned)p->on);

    return 0;
}
