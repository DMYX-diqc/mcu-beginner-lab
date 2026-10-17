/*********************************************************************************************************
* 模块名称：Calculator.h
* 摘    要：
* 当前版本：1.0.0
* 作    者：DMYX
* 完成日期：2026年07月19日 
* 内    容：
* 注    意：                                                                  
**********************************************************************************************************
* 取代版本：
* 作    者：
* 完成日期：
* 修改内容：
* 修改文件：
*********************************************************************************************************/
#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include "DataType.h"
#include <stdint.h>

#define EXPR_MAX_LEN 32  //存储完整的表达式的数组的最大容量
#define STACK_MAX 16     //栈的最大容量
#define HIST_REC_MAX   2    // 最多5条历史记录
#define KEY_LONG_MS    500  // 长按判定500ms

//全局变量导出
extern char expr_buf[EXPR_MAX_LEN];
extern uint8_t expr_len;
extern float calc_result;
extern uint8_t flag_has_expr;
extern uint8_t flag_calc_done;
extern uint8_t calc_err;
//栈
extern float num_stack[STACK_MAX];
extern char op_stack[STACK_MAX];
extern uint8_t num_top;
extern uint8_t op_top;
//历史记录环形缓存
extern char hist_expr[HIST_REC_MAX][EXPR_MAX_LEN];//二维字符数组
extern float hist_result[HIST_REC_MAX];//算式历史结果
extern uint8_t hist_store_idx;//写入指针，指向下一个要写入的位置
extern uint8_t hist_valid_cnt;//记录当前已保存的有效历史条数
// OLED页面
extern uint8_t oled_page; // 0=首页 1=历史页


//===========================API函数声明============================//
void ParseAndCalcExpr(void);//解析并计算单运算符四则表达式（KEY1功能）
void InitCalculator(void); //计算器归零（KEY3功能）
void SaveCalcHistory(void); // 保存计算记录
#endif


