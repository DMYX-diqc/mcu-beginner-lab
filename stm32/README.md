# STM32

初阶第四站。基于 STM32F103RCT6 核心板，学习 GPIO、串口、定时器、中断、PWM、ADC/DAC、Flash、看门狗等外设，完成大作业并做 PPT 汇报。

## 学员要完成

- 完成每章后面的任务（对应 16 个实验）。
- 完成大作业 + PPT 汇报。

## 目录结构

```text
stm32/
├── README.md
├── courseware/   # 17 章 PPT 讲义
├── assignments/  # 大作业要求
├── examples/     # 核心板测试例程
├── hardware/     # 原理图、BOM、Gerber、数据手册
├── reference/    # 参考手册、权威指南
└── tools/        # 开发工具说明与下载
```

## 实验概览（共 16 个）

| # | 实验 | # | 实验 |
| --- | --- | --- | --- |
| 01 | F103 基准工程 | 09 | 外部中断 |
| 02 | 串口电子钟 | 10 | OLED 显示 |
| 03 | GPIO 流水灯 | 11 | 独立看门狗 |
| 04 | GPIO 独立按键 | 12 | 读写内部 Flash |
| 05 | 串口通信 | 13 | PWM 输出 |
| 06 | 定时器 | 14 | 输入捕获 |
| 07 | SysTick | 15 | DAC |
| 08 | RCC | 16 | ADC |

## 开发环境

- Keil MDK + STM32F1 标准外设库，见 `tools/README.md`。
- 下载调试：ST-LINK 或串口 ISP。
