# 第 4 天 STM32 HAL 点灯

按 2026 年 11 月 18 日 22:11 的更正，只控制连接到 `PC0` 的一颗 LED，不再要求 `PD0/PD1` 交替闪烁。`PC0` 每约 500 ms 翻转一次，亮约 0.5 秒、灭约 0.5 秒，完整周期约 1 秒。LED 高电平点亮还是低电平点亮由板上接线决定，不影响闪烁频率。

## 打开和验证

1. 用 Keil MDK5 打开 `MDK-ARM/Day04_PC0_Blink.uvprojx`。通过 Pack Installer 安装工程用到的 `Keil.STM32F1xx_DFP 2.4.1` 和 `ARM.CMSIS 4.5.0`，编译器选择 Arm Compiler 6。若 Keil 提示找不到指定的 6.24 版本，可选择已安装的兼容 Arm Compiler 6。若中文路径导致旧版工具报错，可将整个 `Day04_PC0_Blink` 目录复制到纯英文路径再打开。
2. 工程目标是 `STM32F103VET6`，外部 8 MHz 晶振经 PLL ×9 得到 72 MHz，APB1 为 36 MHz。使用前核对板型、晶振和 PC0 的 LED 接线；没有 8 MHz 外部晶振的板子需先通过 `Day04_PC0_Blink.ioc` 改为适合实板的时钟配置。
3. Build 后，按实际调试器选 ST-LINK 或 CMSIS-DAP，接口选 SWD，确认 Flash 下载算法匹配 STM32F103 高密度片上 Flash，再 Download 并复位。只接入已核对的电源与调试接口，不接电机或其他负载。
4. 观察 PC0 对应 LED 的亮灭间隔；若有示波器，可在 PC0 测约 1 Hz 方波，边沿间隔约 500 ms。未做实板烧录和实测，以上是验收步骤和预期现象，不是实验记录。

离线验证：使用 Arm Compiler 6.24 命令行编译、汇编并链接成功，生成了 HEX；只有旧版汇编器弃用提示。Keil IDE 若未识别器件包，需先完成步骤 1 的配置。

## 代码怎么工作

`HAL_Init()` 建立毫秒时间基准，`SystemClock_Config()` 配置系统时钟，`MX_GPIO_Init()` 先开启 GPIOC 时钟和设置输出初值，再把 PC0 配为低速、无拉电阻的推挽输出。主循环先用 `HAL_Delay(500U)` 等待，再用 `HAL_GPIO_TogglePin()` 翻转。应用逻辑在 `USER CODE BEGIN 3` 和 `USER CODE END 3` 之间，修改 `.ioc` 后重新生成应保留它。

`HAL_GPIO_WritePin()` 用于明确置高或置低；`HAL_GPIO_TogglePin()` 用于翻转；`HAL_GPIO_ReadPin()` 读取引脚采样电平，本实验不需要读回。这里的延时会阻塞主循环，之后学到定时器时可以改成不阻塞的周期翻转。
