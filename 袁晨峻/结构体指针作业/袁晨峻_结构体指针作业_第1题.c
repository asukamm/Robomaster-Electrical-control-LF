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
    puts("========== 第 1 题 ==========");
    LED led = {1,2,10};
    /* 用 . 打印三个成员。 */
    printf("初始: id=%d on=%d delay_ms=%" PRIu32 "\n", led.id, led.on, led.delay_ms);

    LED *p = &led;
    /* 用 p 和 -> 分别修改三个成员。 */
    p->id=2;
    p->on=0;
    p->delay_ms=100;
    printf("修改后: id=%d on=%d delay_ms=%" PRIu32 "\n", led.id, led.on, led.delay_ms);

    /*
     * 简答 1：led.id 和 p->id 分别是什么访问方式？
     * 在什么条件下它们访问的是同一个成员？
     * led.id 直接访问结构体对象的成员，p->id 通过指针访问成员，
     * 等价于 (*p).id。当 p 指向 led 时，两者访问同一个 id
     */

    return 0;
}
