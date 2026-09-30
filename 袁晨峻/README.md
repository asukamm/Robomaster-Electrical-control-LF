# RoboMaster 电控学习记录

跟着中科大 RoboWalker 电控培训（Yssickjgd）从零搭建的 STM32 工程合集。
按培训篇章顺序推进，每个阶段一个独立工程。

## 工程一览

| 工程 | 主控 | 内容 | 对应章节 |
|---|---|---|---|
| [`STM32F103C8T6`](STM32F103C8T6/) | F103C8T6 | 最小可运行工程：CMake 构建 + 启动文件 + `main.c` | 基础篇起步 |
| [`STM32F103C8T6_LED`](STM32F103C8T6_LED/) | F103C8T6 | 从 C 转 C++：呼吸灯逻辑收进 `class Breather`，用 `extern "C"` 桥接 | 阶段 A · 3.1 C++ 桥梁 |
| [`STM32F103C8T6_CAN`](STM32F103C8T6_CAN/) | F103C8T6 | CAN 通信与电机驱动（F103 过渡工程） | 进阶篇 03（3.2 起） |
| [`STM32F407IGH6TR`](STM32F407IGH6TR/) | F407IGH6TR（C 板） | **主工程**：CAN 全链路 + 串级 PID 电机控制 | 3.3 / 3.7 / 3.8 / 3.10 |

## 主工程 F407 的结构

分层组织，各层职责单一：

```
Core/           应用层    main.c：外设初始化 + 中断回调 + 主循环设目标
User/
  alg_pid.*     算法层    Class_PID —— 位置式 PID，含积分限幅 / 输出限幅 / 前馈项
  dvc_motor.*   设备层    Class_Motor —— 电机对象：CAN 收发 + 拆包 + 双环串级
cmake/          构建      工具链配置
Drivers/        ST 官方   HAL 与 CMSIS（未纳入仓库，见下）
```

总线层（`drv_can`）尚未拆分 —— CAN 的收发目前仍在 `Class_Motor` 内部，
这是多电机控制（3.10）正在处理的问题。

## 编译

用 STM32 VS Code 扩展自带的 CMake + Ninja + arm-none-eabi GCC：

```bash
cmake --build build/Debug
```

构建参数在 `CMakePresets.json`，工具链路径由扩展的 `${env:CUBE_BUNDLE_PATH}` 解析，
仓库里不含任何本机绝对路径。

## 关于 Drivers/

`Drivers/`（ST 官方 HAL 与 CMSIS，约 50 MB/工程）未纳入仓库 —— 全是厂商原始文件，
加进来只会让 clone 变慢、diff 变吵。需要编译时用 STM32CubeMX 打开对应 `.ioc` 重新生成。
（`STM32F103C8T6` 是最小的手写工程，本来就没有这一层。）

## 硬件

- **F407 主工程**：大疆 C 板（STM32F407IGH6TR）+ M3508 电机 + C620 电调，CAN 1 Mbps。
  开环转向、闭环速度环已真机实测通过。
