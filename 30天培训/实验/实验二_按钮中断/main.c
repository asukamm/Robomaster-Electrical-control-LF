/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  Calls target system initialization then loop in main.
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEBOUNCE_DELAY 50  // 防抖延时，单位ms

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t last_trigger_time = 0;  // 记录上次触发时间

/* Private functions prototype -----------------------------------------------*/

/**
 * @brief EXTI触发回调函数
 * @param hexti: EXTI句柄指针
 * @param trigger: 触发类型（上升沿/下降沿）
 */
void HAL_EXTI_TriggerCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger)
{
  uint32_t current_time = HAL_GetTick();  // 获取当前时间（ms）

  // 防抖：如果距离上次触发时间小于DEBOUNCE_DELAY，则忽略
  if (current_time - last_trigger_time > DEBOUNCE_DELAY)
  {
    // 切换LED状态
    HAL_GPIO_TogglePin(HAL_GPIOC, HAL_GPIO_PIN_0);

    // 更新上次触发时间
    last_trigger_time = current_time;
  }
}

/**
  * brief:  The application entry point.
  * retval: none but we specify int to comply with C99 standard
  */
int main(void)
{
  /** System Init: this code placed in targets folder initializes your system.
    * It calls the initialization (and sets the initial configuration) of the peripherals.
    * You can use STM32CubeMX to generate and call this code or not in this project.
    * It also contains the HAL initialization and the initial clock configuration.
    */
  if (mx_system_init() != SYSTEM_OK)
  {
    return (-1);
  }
  else
  {
    /*
      * You can start your application code here
      */
    while (1) {}
  }
} /* end main */

