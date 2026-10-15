# 电路设计与制作快速入门

初阶第三站。以「STM32 核心板」为载体，完成原理图、PCB 布局布线、BOM 整理与焊接调试。

## 学员要完成

- 绘制 STM32 核心板（原理图 + PCB）。
- 焊接并调试板子。

## 目录结构

```text
circuit-design/
├── README.md
├── schematic/    # 原理图
├── bom/          # BOM 表
└── layout/       # 布局与布线参考
```

## 资料说明

核心板为 STM32F103RCT6 + CH340C 方案。原理图、BOM、Gerber 见本目录各子文件夹，与 `stm32/hardware/` 中的硬件资料一致（同一块板子，两处仅保留一份即可，本目录为电路课专用入口）。
