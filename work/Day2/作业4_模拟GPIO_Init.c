#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT_PP = 1,
    GPIO_MODE_OUTPUT_OD = 2
} GPIOMode;

typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MED = 1,
    GPIO_SPEED_HIGH = 2
} GPIOSpeed;

typedef struct {
    uint16_t Pin;
    GPIOMode Mode;
    GPIOSpeed Speed;
} GPIO_InitTypeDef;

#define GPIO_PIN_0 ((uint16_t)0x0001)
#define GPIO_PIN_1 ((uint16_t)0x0002)
#define GPIO_PIN_3 ((uint16_t)0x0008)

static const char *mode_name(GPIOMode mode)
{
    switch (mode) {
    case GPIO_MODE_INPUT:
        return "输入(INPUT)";
    case GPIO_MODE_OUTPUT_PP:
        return "推挽输出(OUTPUT_PP)";
    case GPIO_MODE_OUTPUT_OD:
        return "开漏输出(OUTPUT_OD)";
    default:
        return "未知模式";
    }
}

static const char *speed_name(GPIOSpeed speed)
{
    switch (speed) {
    case GPIO_SPEED_LOW:
        return "低速(LOW)";
    case GPIO_SPEED_MED:
        return "中速(MED)";
    case GPIO_SPEED_HIGH:
        return "高速(HIGH)";
    default:
        return "未知速度";
    }
}

static void GPIO_Init(GPIO_InitTypeDef *init)
{
    if (init == NULL) {
        return;
    }

    /* 用打印模拟把 Pin、Mode、Speed 写入 GPIO 配置寄存器。 */
    printf("[GPIO_Init] Pin=0x%04X, Mode=%s, Speed=%s\n",
           (unsigned int)init->Pin,
           mode_name(init->Mode),
           speed_name(init->Speed));
}

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    GPIO_InitTypeDef config = {
        .Pin = GPIO_PIN_0 | GPIO_PIN_1,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Speed = GPIO_SPEED_HIGH
    };

    printf("初始配置:\n");
    GPIO_Init(&config);

    config.Mode = GPIO_MODE_OUTPUT_OD;
    config.Pin = GPIO_PIN_3;

    printf("修改后配置:\n");
    GPIO_Init(&config);

    /*
     * 简答：
     * GPIO_Init 使用指针，可以直接访问并处理调用者传入的真实配置对象，
     * 避免复制整个结构体；如果函数内部需要修改配置，修改也能保留
     */
    return 0;
}
