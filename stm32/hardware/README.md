# hardware — 硬件资料

原理图、BOM、Gerber、数据手册。原文件散落在 `STM32资料包/03.原理图` 与 `07.数据手册`。

| 原文件 | 建议新文件名 | 说明 |
| --- | --- | --- |
| Schematic_STM32-CH340C CoreBoard_2025-07-08.pdf | `schematic-stm32-ch340c-coreboard.pdf` | 原理图 |
| BOM_STM32-CH340C CoreBoard_2025-07-08.xlsx | `bom-stm32-ch340c-coreboard.xlsx` | BOM 表 |
| Gerber_STM32CoreBoard-V1.0.0-...zip | `gerber-stm32-coreboard-v1.0.0.zip` | Gerber 制板文件 |
| STM32核心板开发套件（LY-STM32）.pdf | `core-board-guide.pdf` | 核心板使用说明 |
| 各器件数据手册（07.数据手册/） | 放入 `datasheets/` 子目录 | 电容电阻、晶振、STM32F103RCT6、SSD1306 等 |

> 07.数据手册 里器件 PDF 较多，建议统一放入 `hardware/datasheets/`，文件名去空格、去百分号（例如 `22uF(226) ±20% 25V.pdf` → `cap-22uf-226-25v.pdf`）。
