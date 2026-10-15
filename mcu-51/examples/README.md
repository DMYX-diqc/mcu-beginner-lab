# examples — 基础例程

放置上课演示的基础例程，用于让学员先把板子跑起来。

| 原目录 | 建议新目录 | 说明 |
| --- | --- | --- |
| 51基础例程V1.4 | `51-basic-examples/` | 内含 10 个基础实验 + 核心板综合测试例程 |

原 `51基础例程V1.4` 内结构（去重后保留一份即可）：

```text
51-basic-examples/
├── 01-led-flow/          # LED 流水灯（按位控制 / 总线控制）
├── 02-key/               # 独立按键
├── 03-buzzer/            # 蜂鸣器
├── 04-seg-display/       # 数码管
├── 05-external-interrupt/ # 外部中断
├── 06-timer-counter/     # 定时器 / 计数器
├── 07-pwm-breathing/     # PWM 与呼吸灯
├── 08-uart/              # 串口通信
├── 09-watchdog/          # 看门狗
├── 10-eeprom/            # 模拟 EEPROM
└── core-board-test/      # 核心板综合测试例程
```

> 原文件夹里存在一份嵌套的重复拷贝 `51基础例程V1.4/51基础例程V1.4/` 和一个 `.rar` 压缩包，筛选时删除，只保留一份干净源码。
> Keil 编译产物（`Objects/`、`Listings/`、`*.uvgui.*`、`*.bak` 等）已被 `.gitignore` 忽略，无需手动删除。
