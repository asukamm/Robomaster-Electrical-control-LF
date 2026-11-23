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
#define GPIO_PIN_3 ((uint16_t)0x0008)

void GPIO_Init(GPIO_InitTypeDef *init)
{
    /* 从参数读取 Pin、Mode、Speed，用 printf 模拟写寄存器。 */
    printf("Pin=0x%04X Mode=%d Speed=%d\n",
           (unsigned)init->Pin, (int)init->Mode, (int)init->Speed);
}

int main(void)
{
    puts("========== 第 4 题 ==========");
    /* 用 C99 指定成员初始化。选 PIN0 和 PIN1、推挽输出、高速。 */
    GPIO_InitTypeDef config = {
        .Pin = GPIO_PIN_0 | GPIO_PIN_1,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_HIGH,
    };
    GPIO_InitTypeDef *config_ptr = &config;
    puts("初始配置:");
    GPIO_Init(config_ptr);

    /* 通过 config_ptr 把 Mode 改为开漏输出，把 Pin 改为 PIN3。 */
    config_ptr->Mode = GPIO_MODE_OUTPUT_OD;
    config_ptr->Pin = GPIO_PIN_3;
    puts("修改后配置:");
    GPIO_Init(config_ptr);

    /* 简答 3：模拟的 GPIO_Init 为什么用指针接收配置？
     * 传入结构体地址，函数能直接读取原配置的成员，
     * 无需复制整个结构体。模拟写寄存器时，配置本身保持原值。
     */

    return 0;
}
