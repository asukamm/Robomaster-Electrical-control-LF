# =============================================================================
# GM6020_demo —— STM32F427IIH / Cortex-M4F / 2MB Flash / 192KB RAM + 64KB CCM
# 对应 Keil 工程：MDK-ARM/GM6020_demo.uvprojx
# 板载 25MHz 晶振，CAN 控制 GM6020 电机
# =============================================================================
set(PROJ  ${CMAKE_CURRENT_SOURCE_DIR})
set(HAL   ${PROJ}/Drivers/STM32F4xx_HAL_Driver)
set(CMSIS ${PROJ}/Drivers/CMSIS)

stm32_add_firmware(gm6020
  MCU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
  DEFINES
    USE_HAL_DRIVER
    STM32F427xx
  STARTUP
    ${PROJ}/Core/Startup/startup_stm32f427xx.s
  LDSCRIPT
    ${PROJ}/STM32F427IIHx_FLASH.ld
  INCLUDES
    ${PROJ}/Inc
    ${PROJ}/Bsp
    ${HAL}/Inc
    ${HAL}/Inc/Legacy
    ${CMSIS}/Device/ST/STM32F4xx/Include
    ${CMSIS}/Include
  SOURCES
    ${PROJ}/Src/main.c
    ${PROJ}/Src/gpio.c
    ${PROJ}/Src/can.c
    ${PROJ}/Src/tim.c
    ${PROJ}/Src/stm32f4xx_it.c
    ${PROJ}/Src/stm32f4xx_hal_msp.c
    ${PROJ}/Src/system_stm32f4xx.c
    ${PROJ}/Bsp/pid.c
    ${PROJ}/Bsp/bsp_key.c
    ${PROJ}/Bsp/bsp_can.c
    ${PROJ}/Bsp/bsp_pwm.c
    ${HAL}/Src/stm32f4xx_hal.c
    ${HAL}/Src/stm32f4xx_hal_cortex.c
    ${HAL}/Src/stm32f4xx_hal_rcc.c
    ${HAL}/Src/stm32f4xx_hal_rcc_ex.c
    ${HAL}/Src/stm32f4xx_hal_gpio.c
    ${HAL}/Src/stm32f4xx_hal_dma.c
    ${HAL}/Src/stm32f4xx_hal_dma_ex.c
    ${HAL}/Src/stm32f4xx_hal_pwr.c
    ${HAL}/Src/stm32f4xx_hal_pwr_ex.c
    ${HAL}/Src/stm32f4xx_hal_flash.c
    ${HAL}/Src/stm32f4xx_hal_flash_ex.c
    ${HAL}/Src/stm32f4xx_hal_flash_ramfunc.c
    ${HAL}/Src/stm32f4xx_hal_can.c
    ${HAL}/Src/stm32f4xx_hal_tim.c
    ${HAL}/Src/stm32f4xx_hal_tim_ex.c)
