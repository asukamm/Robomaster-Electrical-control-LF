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

static void print_led(const LED *led)
{
    printf("LED%u: on=%u delay_ms=%u\n",
           (unsigned int)led->id,
           (unsigned int)led->on,
           (unsigned int)led->delay_ms);
}

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    LED leds[4] = {
        {1, 1, 100},
        {2, 0, 200},
        {3, 0, 300},
        {4, 0, 400}
    };
    LED *p = NULL;

    for (p = leds; p < leds + 4; p++) {
        print_led(p);
    }

    p = leds + 2;
    p->on = 1;
    printf("打开 3 号灯后: LED%u on=%u\n",
           (unsigned int)p->id,
           (unsigned int)p->on);

    return 0;
}
