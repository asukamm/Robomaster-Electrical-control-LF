# =============================================================================
# stm32_add_firmware() —— 把「一堆 .c + 一个 startup.s + 一个 .ld」变成可烧录固件
#
# 用法示例：
#   stm32_add_firmware(lab1
#     MCU_FLAGS  -mcpu=cortex-m3 -mthumb -mfloat-abi=soft
#     DEFINES    USE_HAL_DRIVER STM32F103xB
#     STARTUP    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Startup/startup_stm32f103c8tx.s
#     LDSCRIPT   ${CMAKE_CURRENT_SOURCE_DIR}/STM32F103C8TX_FLASH.ld
#     INCLUDES   Core/Inc Drivers/...
#     SOURCES    Core/Src/main.c ...)
#
# 产物（都在 build/ 下）：
#   <target>.elf  .bin  .hex  .map
# =============================================================================
function(stm32_add_firmware TARGET)
  cmake_parse_arguments(A
    ""
    "STARTUP;LDSCRIPT;FLOAT_ABI"
    "MCU_FLAGS;DEFINES;INCLUDES;SOURCES;OPTIONS"
    ${ARGN})

  if(NOT A_STARTUP)
    message(FATAL_ERROR "stm32_add_firmware(${TARGET}): 缺少 STARTUP 参数")
  endif()
  if(NOT A_LDSCRIPT)
    message(FATAL_ERROR "stm32_add_firmware(${TARGET}): 缺少 LDSCRIPT 参数")
  endif()
  if(NOT EXISTS "${A_STARTUP}")
    message(FATAL_ERROR "stm32_add_firmware(${TARGET}): 启动文件不存在: ${A_STARTUP}")
  endif()
  if(NOT EXISTS "${A_LDSCRIPT}")
    message(FATAL_ERROR "stm32_add_firmware(${TARGET}): 链接脚本不存在: ${A_LDSCRIPT}")
  endif()

  add_executable(${TARGET}
    ${A_SOURCES}
    ${A_STARTUP})

  target_include_directories(${TARGET} PRIVATE ${A_INCLUDES})
  target_compile_definitions(${TARGET} PRIVATE ${A_DEFINES})

  target_compile_options(${TARGET} PRIVATE
    ${A_MCU_FLAGS}
    ${A_OPTIONS}
    -Wall
    -fdata-sections
    -ffunction-sections
    -MMD -MP)

  target_link_options(${TARGET} PRIVATE
    ${A_MCU_FLAGS}
    -T${A_LDSCRIPT}
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_BINARY_DIR}/${TARGET}.map
    -Wl,--print-memory-usage)

  # 依赖 .ld：改了链接脚本会自动重新链接
  set_target_properties(${TARGET} PROPERTIES
    LINK_DEPENDS    "${A_LDSCRIPT}"
    SUFFIX          ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

  # 顺手产出 .bin / .hex，Keil / STM32CubeProgrammer 也能直接用
  add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}> ${CMAKE_BINARY_DIR}/${TARGET}.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:${TARGET}> ${CMAKE_BINARY_DIR}/${TARGET}.hex
    BYPRODUCTS ${CMAKE_BINARY_DIR}/${TARGET}.bin ${CMAKE_BINARY_DIR}/${TARGET}.hex
    COMMENT "生成 ${TARGET}.bin / ${TARGET}.hex")
endfunction()
