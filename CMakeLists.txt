# =============================================================================
# RM-Electrical-Control —— STM32 固件统一构建入口
#
# 三个固件目标：
#   lab1   30天培训/实验一   STM32F103C8T6   Cortex-M3
#   lab2   30天培训/实验二   STM32F103C8T6   Cortex-M3
#   gm6020 GM6020_demo       STM32F427IIH    Cortex-M4F
#
# 常用命令：
#   cmake --preset debug              # 配置
#   cmake --build --preset debug      # 编译全部
#   cmake --build build --target lab1 # 只编一个
#   cmake --build build --target clean
# =============================================================================
cmake_minimum_required(VERSION 3.20)

project(RM_Electrical_Control
  VERSION 1.0.0
  DESCRIPTION "RoboMaster electrical control firmware"
  LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 给 clangd / cpptools 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Stm32)

message(STATUS "编译产物目录: ${CMAKE_BINARY_DIR}")

add_subdirectory("30天培训/实验一" lab1)
add_subdirectory("30天培训/实验二" lab2)
add_subdirectory(GM6020_demo       gm6020)
