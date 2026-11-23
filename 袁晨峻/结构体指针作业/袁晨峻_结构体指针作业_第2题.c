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

void set_speed_by_value(GPIO_InitTypeDef g, GPIOSpeed s)
{
    /* 将收到的结构体的 Speed 设为 s。 */
    g.Speed = s;
    (void)g;
}

void set_speed_by_ptr(GPIO_InitTypeDef *p, GPIOSpeed s)
{
    /* 将指针所指结构体的 Speed 设为 s。 */
    p->Speed = s;
}

int main(void)
{
    puts("========== 第 2 题 ==========");
    /* 初始化一个 Speed 为 LOW 的配置。两次调用都尝试设为 HIGH。 */
    GPIO_InitTypeDef speed_config = {0x0001, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_LOW};
    set_speed_by_value(speed_config, GPIO_SPEED_HIGH);
    printf("传值调用后: Speed=%d\n", (int)speed_config.Speed);
    set_speed_by_ptr(&speed_config,GPIO_SPEED_HIGH);
    printf("传址调用后: Speed=%d\n", (int)speed_config.Speed);

    /* 简答 2：传值调用修改的是结构体副本 g 的 Speed。
     * 传址调用收到 speed_config 的地址，修改的是 speed_config.Speed。
     */
    return 0;
}
