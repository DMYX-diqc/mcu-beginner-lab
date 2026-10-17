# duanmengyue 的 STM32 课后作业

- 作者：段梦越（duanmengyue）
- 内容：STM32F103 各章课后作业 + 大作业，均可在 Keil MDK 中编译运行。
- 说明：目录已按 `stm32/labs/` 的实验编号重命名。每个实验下可能有多个子工程，对应「书上例题」和「课后任务」等不同实现。

## 目录结构

| 目录 | 对应实验 | 子工程 |
| --- | --- | --- |
| `01.f103-base-project/` | F103 基准工程 | 单工程 |
| `02.uart-clock/` | 串口电子钟 | 单工程 |
| `03.gpio-led-flow/` | GPIO 流水灯 | `textbook-example`（书上例子）、`task`（任务） |
| `04.gpio-key/` | GPIO 独立按键 | `task`（任务）、`scan-key`（扫描按键） |
| `05.uart/` | 串口通信 | `uart1-only`（只有 UART1 模块）、`uart2-added`（新增 UART2 模块） |
| `06.timer/` | 定时器 | `textbook-example`、`scan-key` |
| `07.systick/` | SysTick | `textbook-example` |
| `08.rcc/` | RCC | 单工程 |
| `09.external-interrupt/` | 外部中断 | `key-toggle-leds`（按键翻转 LED）、`key-switch-led-order`（按键切换亮灯顺序） |
| `10.oled/` | OLED 显示 | `textbook-example`、`dynamic-time-display`（动态时间显示） |
| `11.iwdg/` | 独立看门狗 | `key2-auto-manual-feed`（按键 2 切换自动/手动喂狗）、`key1-manual-feed`（按键 1 手动喂狗） |
| `12.internal-flash/` | 读写内部 Flash | `password-unlock`（密码解锁）、`key-flash-read-write`（按键模拟读写 Flash） |
| `13.pwm-output/` | PWM 输出 | `breathing-light`（呼吸灯）、`observe-waveform`（观察波形） |
| `14.input-capture/` | 输入捕获 | `record-key-press-duration`（记录按键按下时长）、`record-high-duration`（记录高电平持续时间） |
| `15.dac/` | DAC | `textbook-example`、`with-key-and-oled`（加上按键和 OLED） |
| `16.adc/` | ADC | 单工程 |
| `final-project/` | 大作业 | 简单计算器（含按键、OLED、RCC、Timer、UART 等模块） |

## 说明

- 每个子工程都是一个完整的 Keil MDK 工程（含 `Project/*.uvprojx` 工程文件、`App/` 用户代码、`FW/` 标准外设库、`HW/` 硬件驱动、`ARM/` 启动文件等），用 Keil MDK 打开 `.uvprojx` 即可编译。
- 已剔除编译产物（`.o/.axf/.hex/.map/.lst/.crf` 等），只保留源码与工程文件。

## 相关目录

- 空工程模板：`stm32/labs/`
- 书上例题：`stm32/product/`
- 章节任务清单：`stm32/assignments/README.md`
