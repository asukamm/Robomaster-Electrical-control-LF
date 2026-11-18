#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    uint8_t id;       
    uint8_t on;       
    uint32_t delay_ms;
} LED;

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    LED led = {1, 1, 500};
    LED *p = &led;

    printf("初始:   id=%u on=%u delay_ms=%u\n",
           (unsigned int)led.id,
           (unsigned int)led.on,
           (unsigned int)led.delay_ms);

    p->id = 2;
    p->on = 0;
    p->delay_ms = 1000;

    printf("修改后: id=%u on=%u delay_ms=%u\n",
           (unsigned int)p->id,
           (unsigned int)p->on,
           (unsigned int)p->delay_ms);

    /*
     * 简答：
     * led.id 使用“结构体对象 . 成员”的方式直接访问成员；
     * p->id 使用“结构体指针 -> 成员”的方式访问成员。
     * 二者等价
     */
    return 0;
}
