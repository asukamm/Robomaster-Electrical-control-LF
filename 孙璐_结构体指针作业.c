/

#include <stdio.h>
#include <stdint.h>

/
typedef struct {
    uint8_t  id;       /* LED 编号 */
    uint8_t  on;       /* 1 = 亮, 0 = 灭 */
    uint32_t delay_ms; /* 闪烁周期（毫秒） */
} LED;

/
typedef enum {
    GPIO_MODE_INPUT     = 0,  /* 输入 */
    GPIO_MODE_OUTPUT_PP = 1,  /* 推挽输出 */
    GPIO_MODE_OUTPUT_OD = 2   /* 开漏输出 */
} GPIOMode;

typedef enum {
    GPIO_SPEED_LOW  = 0,
    GPIO_SPEED_MED  = 1,
    GPIO_SPEED_HIGH = 2
} GPIOSpeed;

typedef struct {
    uint16_t  Pin;    /* 引脚位掩码 */
    GPIOMode  Mode;
    GPIOSpeed Speed;
} GPIO_InitTypeDef;

/
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

/
void set_speed_by_value(GPIO_InitTypeDef g, GPIOSpeed s)
{
    g.Speed = s;          
}

void set_speed_by_ptr(GPIO_InitTypeDef *p, GPIOSpeed s)
{
    p->Speed = s;        
}


int has_pin(const GPIO_InitTypeDef *p, uint16_t pin)
{
    return (p->Pin & pin) == pin;   
}

void add_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    p->Pin |= pin;                  
}

void remove_pin(GPIO_InitTypeDef *p, uint16_t pin)
{
    p->Pin &= (uint16_t)(~pin);      

///*  */
static const char *mode_str(GPIOMode m)
{
    switch (m) {
    case GPIO_MODE_INPUT:     return "输入(INPUT)";
    case GPIO_MODE_OUTPUT_PP: return "推挽输出(OUTPUT_PP)";
    case GPIO_MODE_OUTPUT_OD: return "开漏输出(OUTPUT_OD)";
    default:                  return "未知";
    }
}

static const char *speed_str(GPIOSpeed s)
{
    switch (s) {
    case GPIO_SPEED_LOW:  return "低速(LOW)";
    case GPIO_SPEED_MED:  return "中速(MED)";
    case GPIO_SPEED_HIGH: return "高速(HIGH)";
    default:              return "未知";
    }
}

/* 真实库里这一步是把配置写进 GPIOx->CRL/CRH 寄存器；
 * 这里用 printf 代替，"打印出来"就等价于"寄存器被写进去了" */
void GPIO_Init(GPIO_InitTypeDef *init)
{
    printf("[GPIO_Init] Pin=0x%04X, Mode=%s, Speed=%s\n",
           (unsigned)init->Pin, mode_str(init->Mode), speed_str(init->Speed));
}

/* ==========================================================================
 * main：按题号顺序跑一遍
 * ==========================================================================*/
int main(void)
{
    /* ---------------- 第 1 题 ---------------- */
    printf("========== 第 1 题 ==========\n");

    LED led = {1, 1, 500};           /* 编号 1、亮、周期 500ms */
    printf("初始:   id=%d on=%d delay_ms=%lu\n",
           led.id, led.on, (unsigned long)led.delay_ms);

    LED *p = &led;                   /* 指针指向 led */
    p->id       = 2;                 /* 用 -> 改三个成员 */
    p->on       = 0;
    p->delay_ms = 1000;
    printf("修改后: id=%d on=%d delay_ms=%lu\n",
           led.id, led.on, (unsigned long)led.delay_ms);

    printf("等价验证: led.id=%d, (*p).id=%d, p->id=%d  （三者相同）\n",
           led.id, (*p).id, p->id);


    /* ---------------- 第 2 题 ---------------- */
    printf("========== 第 2 题 ==========\n");

    GPIO_InitTypeDef g = {GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_LOW};

    set_speed_by_value(g, GPIO_SPEED_HIGH);           /* 改的是副本 */
    printf("传值调用后: Speed=%d （期望 LOW=0，未被修改）\n", g.Speed);

    set_speed_by_ptr(&g, GPIO_SPEED_HIGH);            /* 传地址，改本体 */
    printf("传址调用后: Speed=%d （期望 HIGH=2，已修改）\n", g.Speed);

    /

    /* ---------------- 第 3 题 ---------------- */
    printf("========== 第 3 题 ==========\n");

    GPIO_InitTypeDef pins;

    pins.Pin  = 0;                                  /* 先清空，模拟"复位默认值" */
    pins.Mode = GPIO_MODE_OUTPUT_PP;
    pins.Speed = GPIO_SPEED_LOW;

    add_pin(&pins, GPIO_PIN_0);
    add_pin(&pins, GPIO_PIN_2);
    printf("添加 PIN0、PIN2 后: Pin=0x%04X\n", (unsigned)pins.Pin);

    printf("是否含 PIN0: %s\n", has_pin(&pins, GPIO_PIN_0) ? "是" : "否");
    printf("是否含 PIN1: %s\n", has_pin(&pins, GPIO_PIN_1) ? "是" : "否");

    remove_pin(&pins, GPIO_PIN_0);
    printf("移除 PIN0 后:      Pin=0x%04X\n", (unsigned)pins.Pin);
    printf("是否含 PIN0: %s\n", has_pin(&pins, GPIO_PIN_0) ? "是" : "否");

    /* ---------------- 第 4 题 ---------------- */
    printf("========== 第 4 题 ==========\n");

    /* C99 指定成员初始化：顺序无关、不漏成员，可读性比按位置初始化强很多，
     * 这也是 HAL 库里 GPIO_InitTypeDef 的标准写法 */
    GPIO_InitTypeDef cfg = {
        .Pin   = GPIO_PIN_0 | GPIO_PIN_1,
        .Mode  = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_HIGH
    };

    printf("初始配置:\n");
    GPIO_Init(&cfg);

    {   /* 题目要求"通过指针"改配置，这里就照 HAL 的用法用指针改 */
        GPIO_InitTypeDef *pc = &cfg;
        pc->Mode = GPIO_MODE_OUTPUT_OD;
        pc->Pin  = GPIO_PIN_3;
    }
    printf("修改后配置:\n");
    GPIO_Init(&cfg);

    
    printf("========== 第 5 题 ==========\n");

    LED leds[4] = {
        {1, 1, 100},
        {2, 0, 200},
        {3, 0, 300},
        {4, 0, 400}
    };

    
    for (const LED *q = leds; q < leds + sizeof(leds) / sizeof(leds[0]); q++) {
        printf("LED%d: on=%d delay_ms=%lu\n",
               q->id, q->on, (unsigned long)q->delay_ms);
    }

    
    LED *third = leds + 2;
    third->on = 1;
    printf("打开 3 号灯后: LED%d on=%d\n", third->id, third->on);


    return 0;
}
