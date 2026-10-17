# 10. 模拟 EEPROM 实验

> 目录：`10-eeprom`

## 实验目的

通过本实验掌握用**内部 Flash 模拟 EEPROM** 的读写：

1. 理解 STC 单片机用内部 Flash（IAP）模拟 EEPROM 的原理；
2. 掌握 IAP 相关寄存器（`ISP_DATA`、`ISP_ADDRH/L`、`ISP_CMD`、`ISP_TRIG`、`ISP_CONTR`）；
3. 掌握 **扇区擦除 / 字节写入 / 字节读取** 三个基本操作；
4. 理解「掉电保存数据，上电恢复」的应用场景。

## 实验内容

按键切换 LED 状态，并把当前 LED 状态写入 EEPROM（地址 `0x2000`）；上电时从 EEPROM 读出上次保存的状态并恢复 LED。工程包含：

- `main.c`：按键检测 + LED 控制 + 调用 EEPROM 读写；
- `EEPROM.h` / `EEPROM.c`：IAP 读写模块（扇区擦除、字节写、字节读）。

## 涉及知识点

| 知识点 | 说明 |
| --- | --- |
| IAP/EEPROM | 内部 Flash 模拟 EEPROM |
| 特殊功能寄存器 | `sfr` 定义 IAP 寄存器 |
| 扇区擦除 | `IAPSectorErase` |
| 字节读写 | `IAPByteWrite` / `IAPByteRead` |
| 模块化 | `.c` / `.h` 分离 |

## 硬件资源

- KEY1 ~ KEY3 接 `P3.2` ~ `P3.4`；LED1 ~ LED3 接 `P2.4` ~ `P2.6`。
- EEPROM 存储地址 `0x2000`。

## 编译与烧录

1. 用 Keil C51 打开 `STC51KeilPrj.uvproj`；
2. 编译生成 `.hex` 并烧写到核心板；
3. 按键改变 LED 状态，断电再上电，观察 LED 状态是否恢复。

## 预期现象

按键切换 LED 后断电，重新上电时 LED 恢复到断电前的状态（数据已保存到 EEPROM）。

## 思考与拓展

1. 为什么写 EEPROM 之前要先擦除扇区？
2. Flash 的写入寿命有限，频繁写同一个地址有什么风险？
