# 在 Ubuntu (WSL2) + VS Code 里开发 STM32

本仓库原来只有 Keil MDK 工程（`.uvprojx`），在 Ubuntu 下打不开也编不了。
现在加了一套 **CMake + Ninja + arm-none-eabi-gcc + OpenOCD + cortex-debug** 的构建/调试系统，
三套 Keil 工程都能在 Linux 下直接编译、烧录、单步调试，Keil 工程本身没有动。

---

## 0. 这套东西由什么组成

| 角色 | 工具 | 本机版本 |
| --- | --- | --- |
| 交叉编译器 | `arm-none-eabi-gcc` | 14.2.1 (`gcc-arm-none-eabi` 包) |
| 构建系统 | `cmake` + `ninja` | 4.2.3 / 1.13.2 |
| 烧录 / 调试服务 | `openocd` | 0.12.0 |
| 调试器 | `gdb-multiarch` | 17.1 |
| 编辑器 | VS Code + `cortex-debug`、`cpptools`、`cmake-tools` | — |
| 硬件链路 | ST-Link V2 → `usbipd-win` → WSL | — |

> `gdb-multiarch` 代替了 `arm-none-eabi-gdb`：Ubuntu 从 20.04 起就不再单独打包
> `arm-none-eabi-gdb` 了，功能上完全等价，`launch.json` 里已经用 `gdbPath` 指定。

---

## 1. 一次性安装

```bash
# ① 系统依赖 + ST-Link udev 规则（需要密码，跑一次就行）
sudo bash setup/install-toolchain.sh

# ② VS Code 扩展（要在「WSL: Ubuntu」窗口的终端里跑，不是 Windows 终端）
bash setup/install-extensions.sh
```

装完 `sudo` 脚本会顺带把当前用户加进 `plugdev` 组，**要重开一个 WSL 终端（或 `wsl --shutdown`）才生效**。

---

## 2. 编译

```bash
cmake --preset debug          # 配置（生成 build/ 和 compile_commands.json）
cmake --build build           # 编译全部三个固件
cmake --build build --target lab1 -j   # 只编一个
cmake --build build --target clean     # 清理
```

产物统一在 `build/` 下：

| 目标 | 芯片 | 产物 | 当前占用 |
| --- | --- | --- | --- |
| `lab1` | STM32F103C8T6 | `build/lab1.elf / .bin / .hex / .map` | Flash 4948 B (7.6%) · RAM 1584 B (7.7%) |
| `lab2` | STM32F103C8T6 | `build/lab2.elf / .bin / .hex / .map` | Flash 5008 B (7.6%) · RAM 1584 B (7.7%) |
| `gm6020` | STM32F427IIH | `build/gm6020.elf / .bin / .hex / .map` | Flash 14424 B (0.7%) · RAM 2152 B (1.1%) |

想编 Release（体积优化）就把上面命令里的 `debug` 换成 `release`。

---

## 3. 把板子接到 WSL（关键一步）

WSL2 默认**看不到** USB 设备，必须先直通。在 **Windows 上以管理员身份** 打开 PowerShell：

```powershell
winget install --exact --id dorssel.usbipd-win
```

装完重开一个管理员 PowerShell，然后跑仓库里的脚本（它会自动找出 ST-Link 并绑定+接入）：

```powershell
pwsh -File setup/windows-usbipd.ps1
```

或者手动来：

```powershell
usbipd list                      # 找到 0483:3748 STM32 STLink 那一行的 BUSID
usbipd bind   --busid 2-1        # 只需做一次，重启后仍然有效
usbipd attach --wsl --busid 2-1  # 每次插拔/重启 WSL 后都要重新 attach
```

然后在 WSL 里验证：

```bash
lsusb | grep -i 0483             # 应该能看到 STM32 STLink
```

> 板子重新插拔、或 `wsl --shutdown` 之后，`attach` 都会掉，重跑一次脚本即可。

---

## 4. 烧录

```bash
# 实验一（F103C8）
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
        -c "program build/lab1.elf verify reset exit"

# 实验二
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
        -c "program build/lab2.elf verify reset exit"

# GM6020（F427）
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
        -c "program build/gm6020.elf verify reset exit"
```

VS Code 里更省事：`Ctrl+Shift+P` → `Tasks: Run Task` → 选 `Flash: 实验一 (lab1)` 之类，
任务会先自动编译再下载。

---

## 5. 调试

`F5`（或左侧运行面板）里有五个配置：

- 调试 实验一 (lab1 · STM32F103C8)
- 调试 实验二 (lab2 · STM32F103C8)
- 调试 GM6020 (gm6020 · STM32F427IIH)
- 附加到 实验一 / 附加到 GM6020（程序已经在跑，只想挂上去看）

断点、单步、外设寄存器都走 OpenOCD + `gdb-multiarch`。
想按外设名看寄存器，下载对应 `.svd` 文件后在 `launch.json` 的 `svdFile` 里填上路径即可。

---

## 6. 三个工程的对接关系

| Keil 工程 | CMake 目标 | 启动文件 | 链接脚本 | OpenOCD target |
| --- | --- | --- | --- | --- |
| `30天培训/实验一/MDK-ARM/Lightening.uvprojx` | `lab1` | `Core/Startup/startup_stm32f103c8tx.s` | `STM32F103C8TX_FLASH.ld` | `target/stm32f1x.cfg` |
| `30天培训/实验二/MDK-ARM/button.uvprojx` | `lab2` | `Core/Startup/startup_stm32f103xb.s` | `STM32F103C8TX_FLASH.ld` | `target/stm32f1x.cfg` |
| `GM6020_demo/MDK-ARM/GM6020_demo.uvprojx` | `gm6020` | `Core/Startup/startup_stm32f427xx.s` | `STM32F427IIHx_FLASH.ld` | `target/stm32f4x.cfg` |

Keil 用的启动文件是 armasm 语法（`AREA` / `DCD`），GCC 不吃；
所以实验二的 `Core/Startup/`、`GM6020_demo/Core/Startup/` 是新增的 **GCC 语法启动文件**，
和原 `MDK-ARM/startup_*.s` 各用各的，互不影响。

`gm6020` 的 `STM32F427IIHx_FLASH.ld` 是按 Keil 里的 IROM/IRAM 配置换算的：
Flash 2MB @ `0x08000000`，RAM 192KB @ `0x20000000`，CCM 64KB @ `0x10000000`。

---

## 7. 以后新增一个工程

1. 目录里放好 `Core/`（或 `Src/`、`Inc/`）、`Drivers/`、GCC 版 `startup_*.s` 和 `.ld`；
2. 新建 `CMakeLists.txt`，抄一份现有的改 `MCU_FLAGS`、`DEFINES`、`STARTUP`、`LDSCRIPT`、`SOURCES`；
3. 在根 `CMakeLists.txt` 里加一行 `add_subdirectory(新目录 新目标名)`；
4. 重新 `cmake --preset debug && cmake --build build`。

`stm32_add_firmware()` 会自动帮你产出 `.elf/.bin/.hex/.map` 并打印 Flash/RAM 占用。

常见芯片的 `MCU_FLAGS`：

| 芯片 | 编译选项 |
| --- | --- |
| Cortex-M3 (F103) | `-mcpu=cortex-m3 -mthumb -mfloat-abi=soft` |
| Cortex-M4F (F427) | `-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard` |
| Cortex-M7 (H7) | `-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard` |
| Cortex-M33 (C5/G4) | `-mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard` |

---

## 8. 疑难杂症

| 现象 | 原因 / 解决 |
| --- | --- |
| `cmake: command not found` | 还没跑 `sudo bash setup/install-toolchain.sh`；或这是 Windows 终端而不是 WSL 终端 |
| `openocd` 报 `Error: open failed` / `unable to find a matching CMSIS-DAP device` | USB 没直通。重跑 `setup/windows-usbipd.ps1`，再 `lsusb` 确认 |
| `Permission denied` 访问 `/dev/bus/usb/...` | udev 规则没生效：重跑安装脚本，然后 `wsl --shutdown` 再进 |
| 链接时一堆 `_write is not implemented` | 正常现象。`nosys` 桩函数的提示，`--gc-sections` 会把它们丢掉，不影响运行 |
| IntelliSense 满屏红波浪线 | 先跑一次 `cmake --preset debug` 生成 `build/compile_commands.json`，再 `Ctrl+Shift+P` → `C/C++: Reset IntelliSense Database` |
| 用 `\\wsl$\...` 打开工程，编译报路径错误 | 用 VS Code 的 **Remote-WSL** 打开（`code .` 在 WSL 终端里执行），不要用 Windows 直接访问 UNC 路径 |
| 在 WSL 里敲 `wsl --shutdown` 报 `Unknown command: --shutdown` | WSL 内部有个**同名的 Linux 程序** `/usr/bin/wsl`（WS-Management 的 Wsm Shell，`dpkg -S /usr/bin/wsl` 可查），不是 Windows 的 `wsl.exe`。Linux 下要写 `wsl.exe --shutdown`（带 `.exe` 才走 interop），或者回 Windows 的 PowerShell 跑 `wsl --shutdown` |
| 改了 `CMakeLists.txt` 后行为没变 | `cmake --preset debug` 重新配置；实在不行删掉 `build/` |

---

## 9. 关于 STM32C593RET6

这颗是 ST 新的 STM32C5 系列（Cortex-M33 @144MHz，512KB Flash / 256KB RAM，LQFP64），
**和仓库里现有的 F103 / F427 工程不是一回事**。当前这套构建系统可以复用它，但要用起来还差两件事：

1. **CMSIS 设备包 + GCC 启动文件 + 链接脚本**：需要从 `STM32CubeC5` 里取（通过 STM32CubeMX 生成最省事）；
2. **烧录**：`openocd 0.12.0` 里没有 `target/stm32c5x.cfg`（上游 master 也还没有），
   所以 C5 目前得用 **STM32CubeProgrammer** 的 Linux CLI 烧录，或者等 OpenOCD 支持。

也就是说：F103 / F427 这三套现在就是完整可用的；
C593 等有了具体工程（`.ioc` 或代码），按第 7 节加一个目标即可，烧录步骤单独配 `STM32CubeProgrammer`。
