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
#include "stm32c5xx_hal_uart.h"
#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
uint8_t rx_data;

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
     hal_uart_handle_t *huart = mx_usart1_uart_gethandle();
     HAL_UART_Receive_IT(huart, &rx_data, 1);

    while (1) {}
  }
} /* end main */

void HAL_UART_RxCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte, hal_uart_rx_event_types_t rx_event)
{
  HAL_UART_Transmit(huart, &rx_data, 1, 100);

  if (rx_data == '1')
    HAL_GPIO_WritePin(HAL_GPIOC, HAL_GPIO_PIN_0, HAL_GPIO_PIN_SET);

  if (rx_data == '0')
    HAL_GPIO_WritePin(HAL_GPIOC, HAL_GPIO_PIN_0, HAL_GPIO_PIN_RESET);

  HAL_UART_Receive_IT(huart, &rx_data, 1);


}

