/*
 * global.cpp
 *
 *  Created on: 2017年5月11日
 *      Author: pyt64
 */

#include "global.h"


//////////////////////////////////////
//运行控制相关
//////////////////////////////////////
//指示是否按下的CTRL+C/
__uint8_t gIntIsRun;

//////////////////////////////////////
//进程和线程相关
//////////////////////////////////////
//进程和线程id
//进程id
__int32_t gPid;
//YK线程pthread_t
pthread_t       gTidYK;
//YC线程pthread_t
pthread_t       gTidYC;
//主线程id
__uint64_t gTid;
//第1子线程id
__uint64_t gSubTid_1;
//第2子线程id
__uint64_t gSubTid_2;


////////////////////////////////////
//状态相关
////////////////////////////////////
//数据库链接状态
__uint32_t gIntIsDbConnected;
#ifndef _RUN_ON_XNWX
//服务器链接状态
__uint8_t gIntIsSocketConnected;
#else
//中央数据库链接状态
__uint8_t gIntIsCenterDBConnected;
#endif

//遥控线程状态
__uint8_t gIntIsYKThreadLive;
//遥测线程状态
__uint8_t gIntIsYCThreadLive;


//遥控线程状态1，由收发线程更新，例如每次循环增加该变量
__uint8_t gIntNewStatYKThread;
//遥控线程状态2，用于和遥控线程状态1进行比对，判断线程是否活跃，比对完成后由主线程更新为和遥控线程状态1相同的值
__uint8_t gIntOldStatYKThread;
//遥测线程状态1，由遥测线程更新，例如每次循环增加该变量
__uint8_t gIntNewStatYCThread;
//遥测线程状态2，用于和遥测线程状态1进行比对，判断线程是否活跃，比对完成后由主线程更新为和遥测线程状态1相同的值
__uint8_t gIntOldStatYCThread;




////////////////////////////////////
//服务句柄
////////////////////////////////////
#ifndef _RUN_ON_XNWX
netsocket_t gNetSocket;
#endif

mysql_t gMysql[3];//每个线程一个数据库链接


////////////////////////////////////
//计数变量
////////////////////////////////////
//应用心跳信息
__uint32_t gYySxxt;
//指令计数
__uint8_t gZlCount;
//最近指令类型
__uint8_t gZlLx;
//最近指令编号
__uint8_t gZlBh;
//最近指令的YY_ID
__uint8_t gZlYyId;
//周期遥测计数
__uint32_t gZqYcCount;
//按需遥测计数
__uint32_t gAxYcCount;

//基础定时器超时次数
__uint64_t gBaseTimerCount;

//////////////////////////////////
//缓冲区变量
//////////////////////////////////
//从socket接收到数据后放入该缓冲，指令解析从此缓冲中取出数据进行处理
#ifndef _RUN_ON_XNWX
buffer_t gSocketBuffer;//指令缓冲区
#endif
//////////////////////////////////
//遥测相关
//////////////////////////////////
//延时遥测项链表数组，每个采集周期的延时遥测项放在一个链表里，不同的链表组成一个数组
ycpzx_t * gYsycPzxLinkArray[YSYC_PZX_CJZQ_MAX_RANGE];
//实时遥测包结构
zqssyc_struct_t gSsycStruct;


//////////////////////////////////
//定时器相关
//////////////////////////////////
//用于对每个遥测周期是否到时进行计数
int gYsycPzxCjzqCount[YSYC_PZX_CJZQ_MAX_RANGE];
//每一档延时遥测周期的实际周期（以秒计）
int gYsycPzxCjzqArray[YSYC_PZX_CJZQ_MAX_RANGE];


///////////////////////////////////
//封包格式相关
//////////////////////////////////
//TCP应用层包头
uint8_t gSYNC_HEADER[LEN_OF_TCP_SYNC];


///////////////////////////////////
//统计信息
//////////////////////////////////
total_t gTotal;


/////////////////////////////////
//ZL connect timeout count
/////////////////////////////////
int gZLConnectionTimeOut;


#ifdef _PRINT_TO_FILE
FILE * gOutput;
#endif

/////////////////////////////////////
//Device id
///////////////////////////////////////
__int8_t gDeviceId;

///////////////////////////////////////
//mysql_centerDB
///////////////////////////////////////
#ifdef _RUN_ON_XNWX
mysql_t gMysql_centerDBforYK;
mysql_t gMysql_centerDBforYC;
//虚拟卫星的用户ID
int gUserId;

#endif
