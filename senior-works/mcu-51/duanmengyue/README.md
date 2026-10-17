# duanmengyue 的 51 单片机课后作业

- 作者：段梦越（duanmengyue）
- 内容：51 单片机各章课后作业 + 大作业，均可在 Keil C51 中编译运行。
- 说明：目录已按 `mcu-51/labs/` 的实验编号重命名，各子目录内保留学员原始的源文件名（如 `1.c`、`2.c`），用 `.uvproj` 可直接打开对应工程。

## 目录结构

| 目录 | 对应实验 | 内容 |
| --- | --- | --- |
| `00-base-test/` | 开发平台 | 基准测试工程（main.c + STC51KeilPrj） |
| `01-led-flow/` | LED 流水灯 | 流水灯（流水灯Test1） |
| `02-independent-key/` | 独立按键 | 按 KEY1 控制 LED1、独立按键输入、独立按键输入 2 |
| `03-buzzer/` | 蜂鸣器 | 长按蜂鸣、双闪喇叭、蜂鸣器 1 |
| `04-seg-display/` | 数码管 | 按键计数、数码管显示 1234、数码管显示 3.141 |
| `05-external-interrupt/` | 外部中断 | 书上例子、按键计数、长按蜂鸣、长短按控制流水灯样式 |
| `06-timer-counter/` | 定时器/计数器 | 60s 倒计时、定时器控制 LED、计数器控制 LED、按下按键时间测量 |
| `08-uart/` | 串口通信 | 书上例子、发送数字数码管显示（A/B）、按键发送 Hello World（无/有中断）、串口控制四灯（单字符/字符串/固定长度字符串） |
| `09-watchdog/` | 看门狗 | 外部中断 0 模拟死机触发看门狗复位 |
| `10-eeprom/` | 内部 Flash 读写 | 基于按键计数任务、基于独立按键输入实例 |
| `final-project/` | 大作业 | 串口小助手（C# 上位机）+ 基本任务 |

## 大作业说明

- `final-project/serial-assistant/`：用 C# 编写的串口小助手上位机（Visual Studio 打开 `串口小助手.sln`）。
- `final-project/basic-task/`：51 端基本任务（111.c + EEPROM 读写）。

## 相关目录

- 空工程模板：`mcu-51/labs/`
- 书上例题：`mcu-51/product/`
- 章节任务清单：`mcu-51/assignments/README.md`
