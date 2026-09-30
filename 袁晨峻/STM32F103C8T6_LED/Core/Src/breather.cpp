#include "breather.h"

Breather::Breather(TIM_HandleTypeDef* htim, uint32_t channel) {
    pwm_val_  = 0;
    direction_ = 1;
    step_     = 10;
    max_      = 999;
    htim_     = htim;
    channel_  = channel;
}

void Breather::start() {
    HAL_TIM_PWM_Start(htim_, channel_);
}

void Breather::update() {
    pwm_val_ += direction_ * step_;
    if (pwm_val_ >= max_) direction_ = -1;
    if (pwm_val_ <= 0)     direction_ = 1;
    __HAL_TIM_SET_COMPARE(htim_, channel_, pwm_val_);
}

// ===== 创建全局对象 + C 接口桥接 =====
static Breather breather(&htim3, TIM_CHANNEL_1);

extern "C" void Breather_Init(void) {
    breather.start();
}

extern "C" void Breather_Update(void) {
    breather.update();
}
