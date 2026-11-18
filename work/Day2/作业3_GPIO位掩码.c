#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

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

static int has_pin(const GPIO_InitTypeDef *p, uint16_t pin)
{
    return (p->Pin & pin) == pin;
}

static void add_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    p->Pin |= pin;
}

static void remove_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    p->Pin &= (uint16_t)~pin;
}

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    GPIO_InitTypeDef gpio = {0};

    add_pin(&gpio, GPIO_PIN_0);
    add_pin(&gpio, GPIO_PIN_2);
    printf("添加 PIN0、PIN2 后: Pin=0x%04X\n", (unsigned int)gpio.Pin);

    printf("是否含 PIN0: %s\n", has_pin(&gpio, GPIO_PIN_0) ? "是" : "否");
    printf("是否含 PIN1: %s\n", has_pin(&gpio, GPIO_PIN_1) ? "是" : "否");

    remove_pin(&gpio, GPIO_PIN_0);
    printf("移除 PIN0 后: Pin=0x%04X\n", (unsigned int)gpio.Pin);
    printf("是否含 PIN0: %s\n", has_pin(&gpio, GPIO_PIN_0) ? "是" : "否");

    return 0;
}
