# =============================================================================
# 实验一（Lightening）—— STM32F103C8T6 / Cortex-M3 / 64KB Flash / 20KB RAM
# 对应 Keil 工程：MDK-ARM/Lightening.uvprojx
# =============================================================================
set(PROJ  ${CMAKE_CURRENT_SOURCE_DIR})
set(HAL   ${PROJ}/Drivers/STM32F1xx_HAL_Driver)
set(CMSIS ${PROJ}/Drivers/CMSIS)

stm32_add_firmware(lab1
  MCU_FLAGS
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
  DEFINES
    USE_HAL_DRIVER
    STM32F103xB
  STARTUP
    ${PROJ}/Core/Startup/startup_stm32f103c8tx.s
  LDSCRIPT
    ${PROJ}/STM32F103C8TX_FLASH.ld
  INCLUDES
    ${PROJ}/Core/Inc
    ${HAL}/Inc
    ${HAL}/Inc/Legacy
    ${CMSIS}/Device/ST/STM32F1xx/Include
    ${CMSIS}/Include
  SOURCES
    ${PROJ}/Core/Src/main.c
    ${PROJ}/Core/Src/gpio.c
    ${PROJ}/Core/Src/stm32f1xx_it.c
    ${PROJ}/Core/Src/stm32f1xx_hal_msp.c
    ${PROJ}/Core/Src/system_stm32f1xx.c
    ${HAL}/Src/stm32f1xx_hal.c
    ${HAL}/Src/stm32f1xx_hal_cortex.c
    ${HAL}/Src/stm32f1xx_hal_rcc.c
    ${HAL}/Src/stm32f1xx_hal_rcc_ex.c
    ${HAL}/Src/stm32f1xx_hal_gpio.c
    ${HAL}/Src/stm32f1xx_hal_gpio_ex.c
    ${HAL}/Src/stm32f1xx_hal_dma.c
    ${HAL}/Src/stm32f1xx_hal_pwr.c
    ${HAL}/Src/stm32f1xx_hal_flash.c
    ${HAL}/Src/stm32f1xx_hal_flash_ex.c
    ${HAL}/Src/stm32f1xx_hal_exti.c
    ${HAL}/Src/stm32f1xx_hal_tim.c
    ${HAL}/Src/stm32f1xx_hal_tim_ex.c)
