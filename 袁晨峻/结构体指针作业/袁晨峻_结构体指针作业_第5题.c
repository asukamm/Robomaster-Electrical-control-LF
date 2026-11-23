/*
 * 凌风战队电控组 C 语言作业 · 结构体与指针
 * 把每个 ____ 换成相应的代码或简答，再用 C99 编译并运行。
 * 按题分别编译运行，最终提交 .c 源文件及运行截图。
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/* LED 类型由题目给出。 */
typedef struct {
    uint8_t id;
    uint8_t on;
    uint32_t delay_ms;
} LED;

int main(void)
{
    puts("========== 第 5 题 ==========");
    /* 初始化四个 LED，使第三个 LED 初始为灭。 */
    LED leds[4] = {
        {1, 1, 100},
        {2, 1, 200},
        {3, 0, 300},
        {4, 1, 400},
    };
    /* 只用指针遍历，不能用下标访问。 */
    for (LED *current = leds; current != leds + 4; ++current)
    {
        printf("LED%d: on=%d delay_ms=%" PRIu32 "\n", current->id, current->on, current->delay_ms);
    }

    /* 用指针找到第三个元素，把 on 设为 1 并打印。 */
    LED *third = leds + 2;
    third->on=1;
    printf("打开 3 号灯后: LED%d on=%d\n", third->id, third->on);

    return 0;
}
