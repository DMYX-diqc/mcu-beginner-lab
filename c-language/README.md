# C 语言程序设计

初阶第一站。通过「秒值—时间值转换」「电子钟设计」「通信协议」等系列实验，掌握 C 语言从变量、数组、函数、指针、结构体到多文件工程与 API 设计的完整语法与工程能力。

## 学员要完成

- 完成每章后面的编程任务（共 14 个实验，任务清单见 `assignments/chapter-tasks.md`）。
- 每章的起始框架在 `labs/`（空工程，打开后自己写代码）。
- 先独立完成，再对照 `solutions` 分支的参考答案（Product）。

## 目录结构

```text
c-language/
├── README.md
├── courseware/     # 推荐书籍
├── labs/           # 任务起始框架（空工程，14 个实验）
└── assignments/    # 章节任务清单
```

## 开发环境

- Visual Studio（工程为 `.sln` / `.vcxproj`）
- 打开各实验 `Project/` 下的 `.sln` 即可开始写代码。
- 源码为 `.c` / `.h`，提交时无需包含 `Debug`、`Release` 等编译产物。

## 任务清单

见 `assignments/chapter-tasks.md`，共 14 个实验，知识脉络：

变量与输入输出 → 数组 → 函数 → 枚举 → 指针 → 结构体 → 结构体指针 → 多文件工程 → 定时器 → API 设计 → 循环队列 → 协议处理 → 主机/从机通信。
