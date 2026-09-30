#ifndef __BREATHER_H
#define __BREATHER_H

#ifdef __cplusplus

#include "main.h"

class Breather {
public:
    Breather(TIM_HandleTypeDef* htim, uint32_t channel);
    void start();
    void update();

private:
    uint16_t pwm_val_;
    int8_t  direction_;
    uint16_t step_;
    uint16_t max_;
    TIM_HandleTypeDef* htim_;
    uint32_t channel_;
};

extern "C" {
#endif

void Breather_Init(void);
void Breather_Update(void);

#ifdef __cplusplus
}
#endif

#endif /* __BREATHER_H */
