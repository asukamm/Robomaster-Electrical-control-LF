# =============================================================================
# CMake 工具链文件：GNU Arm Embedded (arm-none-eabi-gcc)
#
# 用法：预设里已经指定，一般不需要手动传
#   cmake --preset debug
# 手动指定：
#   cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
# =============================================================================
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 交叉编译时禁止 CMake 尝试运行目标程序（否则 try_compile 会失败）
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TOOLCHAIN_PREFIX "arm-none-eabi-" CACHE STRING "交叉工具链前缀")

set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_AR           ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_RANLIB       ${TOOLCHAIN_PREFIX}ranlib)
set(CMAKE_OBJCOPY      ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP      ${TOOLCHAIN_PREFIX}objdump)
set(CMAKE_SIZE         ${TOOLCHAIN_PREFIX}size)

# startup_xxx.s 里带有 #if defined(...) 之类的预处理指令，
# 必须走 assembler-with-cpp，否则汇编器会把它当成语法错误。
set(CMAKE_ASM_FLAGS_INIT "-x assembler-with-cpp")

# 目标平台的库/头文件只在工具链目录里找，不在宿主机找
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# 可执行文件后缀：产物统一是 build/<target>.elf
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
