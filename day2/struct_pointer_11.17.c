/*
 * 凌风战队电控组 - Day2作业
 * 作业名称: C语言结构体与指针
 * 姓名: ___________
 * 日期: _2026.11.17__________
 */

#include <stdio.h>
#include <stdint.h>

/* ========================================
 * 第 1 题 · LED 结构体（基础，15 分）
 * ======================================== */

// TODO: 定义LED结构体
typedef struct {
    uint8_t  id;       /* LED 编号 */
    uint8_t  on;       /* 1 = 亮, 0 = 灭 */
    uint32_t delay_ms; /* 闪烁周期（毫秒） */
} LED;

void test_question1(void) {
    printf("========== 第 1 题 ==========\n");

    // TODO: 1. 创建LED变量并初始化
    LED led1 = {1, 1, 500};

    // TODO: 2. 用 . 访问打印三个成员
    printf("初始:   id=%d on=%d delay_ms=%u\n", led1.id, led1.on, led1.delay_ms);

    // TODO: 3. 定义指针p，用 -> 修改成员
    LED *p = &led1;
    p->id = 2;
    p->on = 0;
    p->delay_ms = 1000;

    // TODO: 4. 再次打印验证
    printf("修改后: id=%d on=%d delay_ms=%u\n", p->id, p->on, p->delay_ms);
    printf("等价验证: led1.id=%d, (*p).id=%d, p->id=%d\n", led1.id, (*p).id, p->id);

    /* 回答：
     * Q: led.id 和 p->id 分别是什么访问方式？两者等价吗？
     * A:前者通过变量访问，后者通过指针访问。二者等价。
     */

    printf("\n");
}

/* ========================================
 * 第 2 题 · 传值 vs 传址（20 分）
 * ======================================== */

// 定义GPIO枚举和结构体
typedef enum {
    GPIO_MODE_INPUT     = 0,  /* 输入 */
    GPIO_MODE_OUTPUT_PP = 1,  /* 推挽输出 */
    GPIO_MODE_OUTPUT_OD = 2,  /* 开漏输出 */
} GPIOMode;

typedef enum {
    GPIO_SPEED_LOW  = 0,
    GPIO_SPEED_MED  = 1,
    GPIO_SPEED_HIGH = 2,
} GPIOSpeed;

typedef struct {
    uint16_t  Pin;    /* 引脚位掩码 */
    GPIOMode  Mode;
    GPIOSpeed Speed;
} GPIO_InitTypeDef;

// TODO: 实现传值函数
void set_speed_by_value(GPIO_InitTypeDef g, GPIOSpeed s) {
    // TODO: 把Speed设为s
    g.Speed = s;
}

// TODO: 实现传址函数
void set_speed_by_ptr(GPIO_InitTypeDef *p, GPIOSpeed s) {
    // TODO: 把Speed设为s
    p->Speed = s;
}

void test_question2(void) {
    printf("========== 第 2 题 ==========\n");

    // TODO: 1. 创建一个Speed=LOW的变量
    GPIO_InitTypeDef gpio = {0x0001, GPIO_MODE_INPUT, GPIO_SPEED_LOW};
    // TODO: 2. 调用传值版，打印Speed
    set_speed_by_value(gpio, GPIO_SPEED_HIGH);
    printf("传值调用后: Speed=%d （期望 LOW=0，未被修改）\n", gpio.Speed);
    // TODO: 3. 调用传址版，打印Speed
    set_speed_by_ptr(&gpio, GPIO_SPEED_HIGH);
    printf("传址调用后: Speed=%d （期望 HIGH=2，已修改）\n", gpio.Speed);

    /* 回答：
     * Q: 为什么两次结果不一样？
     * A:传值调用时，函数收到的是变量的副本，副本改变，而原变量不改变；传址调用时，函数收到的是变量的地址，通过地址可以修改原变量。
     */

    printf("\n");
}

/* ========================================
 * 第 3 题 · GPIO 引脚位掩码（20 分）
 * ======================================== */

// 定义引脚宏
#define GPIO_PIN_0  ((uint16_t)0x0001)  // 二进制: 0000 0000 0000 0001
#define GPIO_PIN_1  ((uint16_t)0x0002)  // 二进制: 0000 0000 0000 0010
#define GPIO_PIN_2  ((uint16_t)0x0004)  // 二进制: 0000 0000 0000 0100
#define GPIO_PIN_3  ((uint16_t)0x0008)  // 二进制: 0000 0000 0000 1000
#define GPIO_PIN_4  ((uint16_t)0x0010)
#define GPIO_PIN_5  ((uint16_t)0x0020)
#define GPIO_PIN_6  ((uint16_t)0x0040)
#define GPIO_PIN_7  ((uint16_t)0x0080)
#define GPIO_PIN_8  ((uint16_t)0x0100)
#define GPIO_PIN_9  ((uint16_t)0x0200)
#define GPIO_PIN_10 ((uint16_t)0x0400)
#define GPIO_PIN_11 ((uint16_t)0x0800)
#define GPIO_PIN_12 ((uint16_t)0x1000)
#define GPIO_PIN_13 ((uint16_t)0x2000)
#define GPIO_PIN_14 ((uint16_t)0x4000)
#define GPIO_PIN_15 ((uint16_t)0x8000)

// TODO: 判断是否包含某引脚
int has_pin(const GPIO_InitTypeDef *p, uint16_t pin) {
    // 提示: 使用 & 运算判断
    return (p->Pin & pin) == pin;  // 修改这里
}

// TODO: 添加引脚
void add_pin(GPIO_InitTypeDef *p, uint16_t pin) {
    // 提示: 使用 |= 运算
    p->Pin |= pin;
}

// TODO: 移除引脚
void remove_pin(GPIO_InitTypeDef *p, uint16_t pin) {
    // 提示: 使用 &= ~ 运算
    p->Pin &= ~pin;
}

void test_question3(void) {
    printf("========== 第 3 题 ==========\n");

    // TODO: 创建GPIO_InitTypeDef变量
    GPIO_InitTypeDef gpio = {0, GPIO_MODE_OUTPUT_PP, GPIO_SPEED_LOW};
    // TODO: 添加PIN0、PIN2
    add_pin(&gpio, GPIO_PIN_0);
    add_pin(&gpio, GPIO_PIN_2);
    // TODO: 打印Pin值（应为0x0005）
    printf("添加 PIN0、PIN2 后: Pin=0x%04x\n", gpio.Pin);

    // TODO: 判断是否含PIN0、PIN1
    printf("是否含 PIN0: %s\n", has_pin(&gpio, GPIO_PIN_0)?"是":"否");
    printf("是否含 PIN1: %s\n", has_pin(&gpio, GPIO_PIN_1)?"是":"否");

    // TODO: 移除PIN0
    remove_pin(&gpio, GPIO_PIN_0);

    // TODO: 打印Pin值（应为0x0004）
    printf("移除 PIN0 后:      Pin=0x%04x\n", gpio.Pin);
    printf("是否含 PIN0: %s\n", has_pin(&gpio, GPIO_PIN_0)?"是":"否");

    printf("\n");
}

/* ========================================
 * 第 4 题 · 模拟 GPIO_Init（综合，25 分）
 * ======================================== */

// TODO: 实现GPIO_Init函数
void GPIO_Init(GPIO_InitTypeDef *init) {
    // 提示: 用printf打印配置，模拟写入寄存器
    printf("  [GPIO_Init] Pin=0x%04x, Mode=", init->Pin);

    if (init->Mode == GPIO_MODE_INPUT){
        printf("输入(INPUT)");
    }
    else if (init->Mode == GPIO_MODE_OUTPUT_PP){
        printf("推挽输出(OUTPUT_PP)");
    }
    else if (init->Mode == GPIO_MODE_OUTPUT_OD){
        printf("开漏输出(OUTPUT_OD)");
    }

    printf(", Speed=");
    if (init->Speed == GPIO_SPEED_LOW){
        printf("低速(LOW)");
    }
    else if (init->Speed == GPIO_SPEED_MED){
        printf("中速(MED)");
    }
    else if (init->Speed == GPIO_SPEED_HIGH){
        printf("高速(HIGH)");
    }

    printf("\n");

}

void test_question4(void) {
    printf("========== 第 4 题 ==========\n");

    // TODO: 1. 用C99指定成员初始化创建配置
    GPIO_InitTypeDef config = {
        .Pin = GPIO_PIN_0 | GPIO_PIN_1,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_HIGH
    };

    // TODO: 2. 调用GPIO_Init打印
    printf("初始配置:\n");
    GPIO_Init(&config);

    // TODO: 3. 通过指针修改Mode和Pin
    GPIO_InitTypeDef *p = &config;
    p->Mode = GPIO_MODE_OUTPUT_OD;
    p->Pin = GPIO_PIN_3;

    // TODO: 4. 再次调用GPIO_Init打印
    printf("修改后配置：\n");
    GPIO_Init(&config);

    /* 回答：
     * Q: 为什么GPIO_Init的参数要用指针而不是传值？
     * A:传值会复制整个结构体，浪费内存和时间。而传址只传地址，更高效。
     */

    printf("\n");
}

/* ========================================
 * 第 5 题 · 结构体数组 + 指针遍历（10 分）
 * ======================================== */

void test_question5(void) {
    printf("========== 第 5 题 ==========\n");

    // TODO: 1. 定义并初始化LED数组leds[4]
    LED leds[4] = {
        {1, 1, 100},
        {2, 0, 200},
        {3, 0, 300},
        {4, 0, 400}
    };

    // TODO: 2. 用指针遍历打印（只能用指针，不能用下标）
    LED *p;
    for (p = leds; p < leds + 4; p++){
        printf("LED%d: on=%d delay_ms=%u\n", p->id, p->on, p->delay_ms);
    }

    // TODO: 3. 用指针把第3个元素(下标2)的on置为1并打印
    p = leds + 2;
    p->on = 1;
    printf("打开3号灯后：LED%d on=%d\n", p->id, p->on);

    printf("\n");
}

/* ========================================
 * 主函数
 * ======================================== */

int main(void) {
    printf("凌风战队电控组 - Day2作业：C语言结构体与指针\n");
    printf("=========================================\n\n");

    test_question1();
    test_question2();
    test_question3();
    test_question4();
    test_question5();

    printf("=========================================\n");
    printf("作业完成\n");

    return 0;
}
