#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef enum {
    GPIO_MODE_INPUT = 0,     /* 输入 */
    GPIO_MODE_OUTPUT_PP = 1, /* 推挽输出 */
    GPIO_MODE_OUTPUT_OD = 2  /* 开漏输出 */
} GPIOMode;

typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MED = 1,
    GPIO_SPEED_HIGH = 2
} GPIOSpeed;

typedef struct {
    uint16_t Pin;   /* 引脚位掩码 */
    GPIOMode Mode;
    GPIOSpeed Speed;
} GPIO_InitTypeDef;

static void set_speed_by_value(GPIO_InitTypeDef g, GPIOSpeed s)
{
    g.Speed = s;
}

static void set_speed_by_ptr(GPIO_InitTypeDef *p, GPIOSpeed s)
{
    p->Speed = s;
}

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    GPIO_InitTypeDef gpio = {
        .Pin = 0U,
        .Mode = GPIO_MODE_INPUT,
        .Speed = GPIO_SPEED_LOW
    };

    set_speed_by_value(gpio, GPIO_SPEED_HIGH);
    printf("传值调用后: Speed=%d\n",
           (int)gpio.Speed);

    set_speed_by_ptr(&gpio, GPIO_SPEED_HIGH);
    printf("传址调用后: Speed=%d\n",
           (int)gpio.Speed);

    /*
     * 简答：
     * 传值时，函数得到的是实参结构体的副本，修改副本中的 Speed
     * 不会影响 main 里的 gpio。传址时，函数拿到 gpio 的地址，通过
     * 指针直接修改原对象，因此 gpio.Speed 会被真正改成 HIGH。
     */
    return 0;
}
