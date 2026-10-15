# labs — 实验任务框架

每章任务的起始源码框架。已配好标准外设库环境，核心代码留空，需要你在这些空框架上完成实验。

> 本目录**不含 Keil 工程文件（`.uvproj`）**，需自行新建工程（实验 01「F103 基准工程」教的就是建工程）。

## 用法

1. 新建 Keil MDK 工程，把对应实验的源码加入工程。
2. 补全 `App/` 中的空实现。
3. 编译、下载（ST-LINK 或串口 ISP）验证。

## 实验列表（共 16 个）

| 目录 | 实验 |
| --- | --- |
| `01.f103-base-project` | F103 基准工程 |
| `02.uart-clock` | 串口电子钟 |
| `03.gpio-led-flow` | GPIO 流水灯 |
| `04.gpio-key` | GPIO 独立按键 |
| `05.uart` | 串口通信 |
| `06.timer` | 定时器 |
| `07.systick` | SysTick |
| `08.rcc` | RCC |
| `09.external-interrupt` | 外部中断 |
| `10.oled` | OLED 显示 |
| `11.iwdg` | 独立看门狗 |
| `12.internal-flash` | 读写内部 Flash |
| `13.pwm-output` | PWM 输出 |
| `14.input-capture` | 输入捕获 |
| `15.dac` | DAC |
| `16.adc` | ADC |
