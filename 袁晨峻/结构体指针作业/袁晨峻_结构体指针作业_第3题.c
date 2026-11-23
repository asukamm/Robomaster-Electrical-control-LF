/*
 * 凌风战队电控组 C 语言作业 · 结构体与指针
 * 把每个 ____ 换成相应的代码或简答，再用 C99 编译并运行。
 * 按题分别编译运行，最终提交 .c 源文件及运行截图。
 */

#include <stdint.h>
#include <stdio.h>

/* GPIO 类型和枚举值由题目给出。 */
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT_PP = 1,
    GPIO_MODE_OUTPUT_OD = 2,
} GPIOMode;

typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MED = 1,
    GPIO_SPEED_HIGH = 2,
} GPIOSpeed;

typedef struct {
    uint16_t Pin;
    GPIOMode Mode;
    GPIOSpeed Speed;
} GPIO_InitTypeDef;

#define GPIO_PIN_0 ((uint16_t)0x0001)
#define GPIO_PIN_1 ((uint16_t)0x0002)
#define GPIO_PIN_2 ((uint16_t)0x0004)
#define GPIO_PIN_3 ((uint16_t)0x0008)
/* 补齐 GPIO_PIN_4 到 GPIO_PIN_15 的宏定义。 */
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

int has_pin(const GPIO_InitTypeDef *p, uint16_t pin)
{
    /* 返回当前配置是否包含 pin 指定的位。 */
    return (p->Pin & pin) == pin;
}

void add_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    /* 添加 pin 指定的位，保留原来的其他位。 */
    p->Pin |= pin;
}

void remove_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    /* 清掉 pin 指定的位，保留其他位。 */
    p->Pin &= (uint16_t)~pin;
}

int main(void)
{
    puts("========== 第 3 题 ==========");
    GPIO_InitTypeDef pins = {0};
    /* 依次添加 PIN0 和 PIN2。 */
    add_pin(&pins,GPIO_PIN_0);
    add_pin(&pins,GPIO_PIN_2);
    printf("添加后: Pin=0x%04X\n", (unsigned)pins.Pin);
    printf("是否含 PIN0: %s\n", has_pin(&pins,GPIO_PIN_0) ? "是" : "否");
    printf("是否含 PIN1: %s\n", has_pin(&pins,GPIO_PIN_1) ? "是" : "否");
    remove_pin(&pins,GPIO_PIN_0);
    printf("移除 PIN0 后: Pin=0x%04X\n", (unsigned)pins.Pin);
    printf("是否含 PIN0: %s\n", has_pin(&pins,GPIO_PIN_0) ? "是" : "否");

    return 0;
}
