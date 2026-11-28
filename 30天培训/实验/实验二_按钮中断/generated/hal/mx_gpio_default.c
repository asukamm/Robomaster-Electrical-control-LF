/**
  ******************************************************************************
  * @file           : mx_gpio_default.c
  * @brief          : gpio_default Peripheral initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32c5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32c5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_gpio_default.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/
static hal_exti_handle_t hEXTI4;

/******************************************************************************/
/* Exported functions for GPIO in HAL layer                                   */
/******************************************************************************/
system_status_t mx_gpio_default_init(void)
{
  hal_gpio_config_t  gpio_config;

  HAL_RCC_GPIOA_EnableClock();

  HAL_RCC_GPIOC_EnableClock();

  /* Configure PA15 GPIO pin in input mode */
  gpio_config.mode            = HAL_GPIO_MODE_INPUT;
  gpio_config.pull            = HAL_GPIO_PULL_DOWN;
  if (HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_15, &gpio_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  hal_exti_config_t exti_config;

  /* Initialize the EXTI for line 15 */
  HAL_EXTI_Init(&hEXTI4, HAL_EXTI_LINE_15);

  /* Set the trigger as RISING for the GPIOA */
  exti_config.trigger   = HAL_EXTI_TRIGGER_RISING;
  exti_config.gpio_port = HAL_EXTI_GPIOA;
  HAL_EXTI_SetConfig(&hEXTI4, &exti_config);

  /* Enable the INTERRUPT mode */
  HAL_EXTI_Enable(&hEXTI4, HAL_EXTI_MODE_INTERRUPT);

  /* Set line 15 Interrupt priority */
  HAL_CORTEX_NVIC_SetPriority(EXTI15_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(EXTI15_IRQn);

  /* Configure PC0 GPIO pin in output mode */
  gpio_config.mode            = HAL_GPIO_MODE_OUTPUT;
  gpio_config.speed           = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.pull            = HAL_GPIO_PULL_NO;
  gpio_config.output_type     = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.init_state      = HAL_GPIO_PIN_SET;
  if (HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_0, &gpio_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  return SYSTEM_OK;
}

system_status_t mx_gpio_default_deinit(void)
{
  /* De-initialize the EXTI for GPIOA line15 */
  HAL_EXTI_DeInit(&hEXTI4);

  /* set line 15 Interrupt priority */
  HAL_CORTEX_NVIC_DisableIRQ(EXTI15_IRQn);

  /* De-initialize pins of GPIOA port */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_15);

  /* De-initialize pins of GPIOC port */
  HAL_GPIO_DeInit(HAL_GPIOC, HAL_GPIO_PIN_0);

  return SYSTEM_OK;
}

hal_exti_handle_t *mx_gpio_default_exti15_gethandle(void)
{
  return &hEXTI4;
}

/******************************************************************************/
/*                            EXTI Line15 interrupt                            */
/******************************************************************************/
void EXTI15_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hEXTI4);
}
