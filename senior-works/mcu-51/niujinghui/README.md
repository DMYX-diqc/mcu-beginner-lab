# niujinghui 的 51 单片机大作业

- 作者：牛京辉（niujinghui）
- 内容：51 单片机大作业——串口电子钟，可在 Keil C51 中编译运行。

## 功能简介

一个基于 51 核心板的串口电子钟，主要功能：

- 数码管显示时间（年月日、时分秒、星期）；
- 串口通信（上位机串口助手控制）；
- 闹钟 + 蜂鸣器鸣叫；
- 4 路 LED PWM 呼吸灯；
- 看门狗复位保护；
- 掉电保存（EEPROM 存储）。

## 目录结构

```text
niujinghui/
└── final-project/          # 大作业工程
    ├── main.c              # 主程序
    ├── public.c / public.h # 公共定义与函数
    ├── smg.c / smg.h       # 数码管显示
    ├── EEPROM.c / EEPROM.h # 内部 Flash / EEPROM 读写
    ├── watchdog.c / watchdog.h  # 看门狗
    └── main.uvproj         # Keil 工程文件
```

## 相关目录

- 空工程模板：`mcu-51/labs/`
- 书上例题：`mcu-51/product/`
- 章节任务清单：`mcu-51/assignments/README.md`
- 大作业要求：`mcu-51/assignments/final-project.md`
