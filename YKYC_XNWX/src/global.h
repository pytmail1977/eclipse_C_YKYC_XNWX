/*
 * global.h
 *
 *  Created on: 2017年5月11日
 *      Author: pyt64
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "include.h"
#include "includecpp.h"
#include "complier.h"
#include "const.h"
#include "debug.h"
#include "struct.h"

//////////////////////////////////////
//运行控制相关
//////////////////////////////////////
//指示是否按下的CTRL+C/
extern __uint8_t gIntIsRun;

//////////////////////////////////////
//进程和线程相关
//////////////////////////////////////
//进程和线程id
//进程id
extern __int32_t gPid;
//YK线程pthread_t
extern pthread_t       gTidYK;
//YC线程pthread_t
extern pthread_t       gTidYC;
//主线程id
extern __uint64_t gTid;
//第1子线程id
extern __uint64_t gSubTid_1;
//第2子线程id
extern __uint64_t gSubTid_2;


////////////////////////////////////
//状态相关
////////////////////////////////////
//数据库链接状态
extern __uint32_t gIntIsDbConnected;
#ifndef _RUN_ON_XNWX
//服务器链接状态
extern __uint8_t gIntIsSocketConnected;
#else
//中央数据库链接状态
extern __uint8_t gIntIsCenterDBConnected;
#endif
//遥控线程状态
extern __uint8_t gIntIsYKThreadLive;
//遥测线程状态
extern __uint8_t gIntIsYCThreadLive;


//遥控线程状态1，由收发线程更新，例如每次循环增加该变量
extern __uint8_t gIntNewStatYKThread;
//遥控线程状态2，用于和遥控线程状态1进行比对，判断线程是否活跃，比对完成后由主线程更新为和遥控线程状态1相同的值
extern __uint8_t gIntOldStatYKThread;
//遥测线程状态1，由遥测线程更新，例如每次循环增加该变量
extern __uint8_t gIntNewStatYCThread;
//遥测线程状态2，用于和遥测线程状态1进行比对，判断线程是否活跃，比对完成后由主线程更新为和遥测线程状态1相同的值
extern __uint8_t gIntOldStatYCThread;




////////////////////////////////////
//服务句柄
////////////////////////////////////
#ifndef _RUN_ON_XNWX
extern netsocket_t gNetSocket;
#endif

extern mysql_t gMysql[3];


////////////////////////////////////
//计数变量
////////////////////////////////////
//应用心跳信息
extern __uint32_t gYySxxt;
//指令计数
extern __uint8_t gZlCount;
//指令计数
extern __uint8_t gZlCount;
//最近指令类型
extern __uint8_t gZlLx;
//最近指令编号
extern __uint8_t gZlBh;
//最近指令的YY_ID
extern __uint8_t gZlYyId;
//周期遥测计数
extern __uint32_t gZqYcCount;
//按需遥测计数
extern __uint32_t gAxYcCount;

//基础定时器超时次数
extern __uint64_t gBaseTimerCount;

//////////////////////////////////
//缓冲区变量
//////////////////////////////////
//从socket接收到数据后放入该缓冲，指令解析从此缓冲中取出数据进行处理
#ifndef _RUN_ON_XNWX
extern buffer_t gSocketBuffer;//指令缓冲区
#endif

//////////////////////////////////
//遥测相关
//////////////////////////////////
//延时遥测项链表数组，每个采集周期的延时遥测项放在一个链表里，不同的链表组成一个数组
extern ycpzx_t * gYsycPzxLinkArray[YSYC_PZX_CJZQ_MAX_RANGE];
//用于对每个遥测周期是否到时进行计数
extern int gYsycPzxCjzqCount[YSYC_PZX_CJZQ_MAX_RANGE];
//实时遥测包结构
extern zqssyc_struct_t gSsycStruct;
//每一档延时遥测周期的实际周期（以秒计）
extern int gYsycPzxCjzqArray[YSYC_PZX_CJZQ_MAX_RANGE];

///////////////////////////////////
//封包格式相关
//////////////////////////////////
//TCP应用层包头
extern uint8_t gSYNC_HEADER[LEN_OF_TCP_SYNC];


///////////////////////////////////
//调试相关
//////////////////////////////////
extern total_t gTotal;

/////////////////////////////////
//ZL connect timeout count
/////////////////////////////////
extern int gZLConnectionTimeOut;

#ifdef _PRINT_TO_FILE
extern FILE * gOutput;
#endif

/////////////////////////////////////
//Device id
///////////////////////////////////////
extern __int8_t gDeviceId;

///////////////////////////////////////
//mysql_centerDB
///////////////////////////////////////
#ifdef _RUN_ON_XNWX
extern mysql_t gMysql_centerDBforYK;
extern mysql_t gMysql_centerDBforYC;
#endif

#endif /* GLOBAL_H_ */

