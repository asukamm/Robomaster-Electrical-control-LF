#include <stdio.h>
#include <stdint.h>

// 第1题 LED 结构体
typedef struct {
    uint8_t  id;        // LED 编号
    uint8_t  on;        // 1 亮 0 灭
    uint32_t delay_ms;  // 闪烁周期 毫秒
} LED;

// 第2到4题共用的模拟 GPIO 结构体
typedef enum {
    GPIO_MODE_INPUT     = 0,  // 输入
    GPIO_MODE_OUTPUT_PP = 1,  // 推挽输出
    GPIO_MODE_OUTPUT_OD = 2,  // 开漏输出
} GPIOMode;

typedef enum {
    GPIO_SPEED_LOW  = 0,
    GPIO_SPEED_MED  = 1,
    GPIO_SPEED_HIGH = 2,
} GPIOSpeed;

typedef struct {
    uint16_t  Pin;    // 引脚位掩码
    GPIOMode  Mode;
    GPIOSpeed Speed;
} GPIO_InitTypeDef;

// 第3题 每个引脚占一位 第 n 位是 1 就代表选了 PINn
#define GPIO_PIN_0   ((uint16_t)0x0001)
#define GPIO_PIN_1   ((uint16_t)0x0002)
#define GPIO_PIN_2   ((uint16_t)0x0004)
#define GPIO_PIN_3   ((uint16_t)0x0008)
#define GPIO_PIN_4   ((uint16_t)0x0010)
#define GPIO_PIN_5   ((uint16_t)0x0020)
#define GPIO_PIN_6   ((uint16_t)0x0040)
#define GPIO_PIN_7   ((uint16_t)0x0080)
#define GPIO_PIN_8   ((uint16_t)0x0100)
#define GPIO_PIN_9   ((uint16_t)0x0200)
#define GPIO_PIN_10  ((uint16_t)0x0400)
#define GPIO_PIN_11  ((uint16_t)0x0800)
#define GPIO_PIN_12  ((uint16_t)0x1000)
#define GPIO_PIN_13  ((uint16_t)0x2000)
#define GPIO_PIN_14  ((uint16_t)0x4000)
#define GPIO_PIN_15  ((uint16_t)0x8000)

// 第2题 传值
void set_speed_by_value(GPIO_InitTypeDef g, GPIOSpeed s)
{
    g.Speed = s;   // 改的是形参 g 副本 外面的变量不受影响
}

// 第2题 传址
void set_speed_by_ptr(GPIO_InitTypeDef *p, GPIOSpeed s)
{
    p->Speed = s;  // p里存的是实参的地址 这样改到的就是实参
}

// 第3题 判断掩码里是不是包含了 pin 里的所有位
int has_pin(const GPIO_InitTypeDef *p, uint16_t pin)
{
    return (p->Pin & pin) == pin;
}

// 添加引脚 把对应位置 1
void add_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    p->Pin |= pin;
}

// 移除引脚 把对应位清 0 其他位保持不变
void remove_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
   p->Pin = p->Pin & (~pin);
}

// 第4题
void GPIO_Init(GPIO_InitTypeDef *init)
{
    printf("  GPIO_Init Pin=0x%04X Mode=%d Speed=%d\n",
           init->Pin, init->Mode, init->Speed);
}

int main(void)
{
    // 第1题
    LED led = {1, 1, 500};   // id=1 亮 周期 500ms
    printf("初始 id=%u on=%u delay_ms=%u\n",
           led.id, led.on, led.delay_ms);

    LED *p_led = &led;       // p_led 指向 led
    p_led->id       = 2;     // 用指针改三个成员
    p_led->on       = 0;
    p_led->delay_ms = 1000;

    printf("修改后 id=%u on=%u delay_ms=%u\n",
           led.id, led.on, led.delay_ms);

    printf("等价验证 led.id=%u (*p).id=%u p->id=%u 三者相同\n",
           led.id, (*p_led).id, p_led->id);

    // 第1题简答 led.id 是结构体变量用 . 访问 p->id 是指针用 -> 访问 等价于
    // (*p).id 只要 p 指向 led 两种写法取到的就是同一个成员 完全等价
    // 只是变量用 . 指针用 ->

    // 第2题
    printf("\n第2题\n");

    GPIO_InitTypeDef g2 = {GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_LOW};
    printf("初始 Speed=%d\n", g2.Speed);

    set_speed_by_value(g2, GPIO_SPEED_HIGH);
    printf("传值调用后 Speed=%d 期望 LOW=0 未被修改\n", g2.Speed);

    set_speed_by_ptr(&g2, GPIO_SPEED_HIGH);
    printf("传址调用后 Speed=%d 期望 HIGH=2 已修改\n", g2.Speed);

    // 第2题简答 C 的参数是值传递 传值版把整个结构体复制一份给形参 g 函数里改的
    // 是副本 函数返回后副本就没了 外面的 g2 一点没动 传址版传的是 &g2
    // p 拿到地址以后 p->Speed 写的就是 g2 本身 所以能改成功 另外结构体
    // 越大复制越费时间 这也是 HAL 里都用指针的原因之一

    // 第3题
    printf("\n第3题\n");

    GPIO_InitTypeDef g3 = {0, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_LOW};

    add_pin(&g3, GPIO_PIN_0);
    add_pin(&g3, GPIO_PIN_2);
    printf("添加 PIN0 PIN2 后 Pin=0x%04X\n", g3.Pin);

    printf("是否含 PIN0 %s\n", has_pin(&g3, GPIO_PIN_0) ? "是" : "否");
    printf("是否含 PIN1 %s\n", has_pin(&g3, GPIO_PIN_1) ? "是" : "否");

    remove_pin(&g3, GPIO_PIN_0);
    printf("移除 PIN0 后 Pin=0x%04X\n", g3.Pin);
    printf("是否含 PIN0 %s\n", has_pin(&g3, GPIO_PIN_0) ? "是" : "否");

    // 第4题
    printf("\n第4题\n");

    // C99 指定成员初始化 不用按顺序写
    GPIO_InitTypeDef cfg = {
        .Pin   = GPIO_PIN_0 | GPIO_PIN_1,
        .Mode  = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_HIGH,
    };

    printf("初始配置\n");
    GPIO_Init(&cfg);

    // 用指针改配置 再初始化一次
    GPIO_InitTypeDef *pc = &cfg;
    pc->Mode = GPIO_MODE_OUTPUT_OD;
    pc->Pin  = GPIO_PIN_3;

    printf("修改后配置\n");
    GPIO_Init(&cfg);

    // 第4题简答 一是结构体传值要整个复制 白费栈空间和时间 二是函数里要能直接
    // 改调用者的配置 传值只能改副本 三是和 HAL 写法一致 真实的签名是
    // HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
    // 两个参数都是指针

    // 第5题
    printf("\n第5题\n");

    LED leds[4] = {
        {1, 1, 100},
        {2, 0, 200},
        {3, 0, 300},
        {4, 0, 400},
    };

    // 只用指针遍历 从 leds 走到 leds + 4
    const LED *p5;
    for (p5 = leds; p5 < leds + 4; p5++) {
        printf("  LED%u on=%u delay_ms=%u\n",
               p5->id, p5->on, p5->delay_ms);
    }

    // 把第3个元素 下标 2 的 on 置 1 不用 leds[2]
    LED *p6 = leds + 2;
    p6->on = 1;
    printf("  打开 3 号灯后 LED%u on=%u\n",
           p6->id, p6->on);

    return 0;
}
