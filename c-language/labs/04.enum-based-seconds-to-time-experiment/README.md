# 04. 基于枚举的秒值转时间值

> 目录：`04.enum-based-seconds-to-time-experiment`

## 实验目的

通过本实验，掌握**枚举类型**和**多分支结构**的使用：

1. 掌握 `enum` 枚举类型的定义与使用；
2. 掌握 `switch-case` 多分支选择结构；
3. 理解枚举常量的默认取值与自增特性（`type++`）。

## 实验内容

在「秒值转时间值」的基础上，改用**枚举 + switch-case** 的方式完成转换：

1. 定义枚举 `EnumTimeVal`，包含 `TIME_VAL_HOUR`、`TIME_VAL_MIN`、`TIME_VAL_SEC`、`TIME_VAL_MAX`；
2. 通过枚举变量配合 `switch-case`，分别计算小时、分钟、秒。

## 涉及知识点

| 知识点 | 说明 |
| --- | --- |
| 枚举 | `enum` 定义、枚举常量取值 |
| 分支结构 | `switch-case` 多分支选择 |
| 自增 | 枚举变量的 `++` 运算 |

## 运行环境

- 操作系统：Windows
- 开发工具：Visual Studio（或任意支持 C 的 IDE / 编译器）

## 编译与运行

1. 用 Visual Studio 打开 `Project` 目录下的工程文件（`.sln`）；
2. 在 `Code/` 的源文件中补全实现；
3. 编译运行后，按提示输入一个 0~86399 之间的整数；
4. 观察换算结果。

## 预期结果

输入 `3661`，输出：

```
Current time: 01-01-01
```

## 思考与拓展

1. 枚举常量 `TIME_VAL_HOUR`、`TIME_VAL_MIN`、`TIME_VAL_SEC` 的值分别是多少？
2. 如果把 `switch` 中的 `break` 去掉，会发生什么现象？
