/*
 * debug.h
 *
 *  Created on: 2017年5月16日
 *      Author: pyt64
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "complier.h"
#include "commonTool.h"




////////////////////////////////////////////////////
//断点工具
////////////////////////////////////////////////////
//中止等待用户输入一个字符，用于停止程序，调试用相当于断点Break point
#define DEBUG_STOP printf("input any char to continue:___________________\n"); getchar();
//显示运行到此处，Run to here
#define R2H(arg) 	printf("RUN to Here %s！\n",arg);

//////////////////////
//Debug输出函数使用的辅助信息打印函数
//////////////////////
//定义取函数实例序号的函数调用为宏
//2018-06-03 修改宏定义GET_FUNCSEQ，不再在函数内引入变量funcSeq_t funcSeq
#define GET_FUNCSEQ funcSeq_t funcSeq = getRandFuncSeq();
#define GET_FUNCSEQ //funcSeq_t funcSeq = getRandFuncSeq();
//定义时间、线程信息输出
//2018-06-03 修改宏定义infoPrint使用不带参数funcSeq的prePrint
//#define infoPrint prePrint(funcSeq);
#define infoPrint prePrint();

///////////////////////
//定义用于Debug的输出函数
///////////////////////

//调试输出
#ifdef _DEBUGPRINT
#define prgPrint infoPrint myfprintf //主要用于调试流程 类似 IT TIME TO DO ……
#define dataPrint infoPrint myfprintf  //主要用于调试数据
#define sqlPrint infoPrint myfprintf  //主要用于调试SQL
#define tmpPrint infoPrint myfprintf //主要用于调试 	临时的提示信息 如进程id，等
#define debugPrint infoPrint myfprintf //主要用于临时调试

//定义用于帧、包输出的函数
#define debugPrint_YKPackage //fprintf //主要用于调试接收TCP传输帧中封装的指令帧 未使用
#define debugPrint_YCPackage //fprintf //主要用于调试发送的遥测包

//定义用于显示调用关系的输出函数
#define fucPrint infoPrint myfprintf
//定义用于消息显示的输出函数
#define msgPrint infoPrint myfprintf
//定义用于错误提示的输出函数
#define errorPrint infoPrint myfprintf

//定义用于子线程的输出函数
//YK线程
#define threadPrint_YK infoPrint myfprintf
//YC线程
#define threadPrint_YC infoPrint myfprintf

//定义用于普通输出格式的输出函数
#define normalPrint myfprintf

#else
#define prgPrint //infoPrint myfprintf //主要用于调试流程 类似 IT TIME TO DO ……
#define dataPrint //infoPrint myfprintf  //主要用于调试数据
#define sqlPrint //infoPrint myfprintf  //主要用于调试SQL
#define tmpPrint //infoPrint myfprintf //主要用于临时调试 	临时的提示信息 如进程id，等

//定义用于帧、包输出的函数
#define debugPrint_YKPackage //myfprintf //主要用于调试接收TCP传输帧中封装的指令帧
#define debugPrint_YCPackage //myfprintf //主要用于调试发送的遥测包

//定义用于显示调用关系的输出函数
#define fucPrint //infoPrint myfprintf
//定义用于消息显示的输出函数
#define msgPrint infoPrint myfprintf
//定义用于错误提示的输出函数
#define errorPrint infoPrint myfprintf

//定义用于子线程的输出函数
//YK线程
#define threadPrint_YK //infoPrint fprintf
//YC线程
#define threadPrint_YC //infoPrint fprintf

//定义用于普通输出格式的输出函数
#define normalPrint myfprintf
#endif


//////////////////////
//加锁控制
//////////////////////

#ifdef _LOCK
#define worm_mutex_init wormhole_mutex_init
#define worm_mutex_destroy wormhole_mutex_destroy
#define worm_mutex_lock wormhole_mutex_lock
#define worm_mutex_unlock wormhole_mutex_unlock
#define worm_mutex_trylock wormhole_mutex_trylock
#else
#define worm_mutex_init //wormhole_mutex_init
#define worm_mutex_destroy //wormhole_mutex_destroy
#define worm_mutex_lock //wormhole_mutex_lock
#define worm_mutex_unlock //wormhole_mutex_unlock
#define worm_mutex_trylock //wormhole_mutex_trylock
#endif

#endif /* DEBUG_H_ */
