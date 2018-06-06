/*
 * complier.h
 *
 *  Created on: 2017年5月16日
 *      Author: pyt64
 */

#ifndef COMPLIER_H_
#define COMPLIER_H_

//////////////////////
//编译控制
//////////////////////

//如果定义，则表示在mac系统上编译，否则在ubuntu系统上编译
#define _BUILD_ON_MAC

//如果定义，则在lenovo上运行，设置网络接口为eth4或者wlan0
//#define _RUN_ON_LENOVO

//当_SXXT_ANYWAY打开时，积极刷新心跳，不考虑遥控线程是否活跃
#define _SXXT_ANYWAY

//当_PRINT_DETAIL打开时，输出YK线程的细节。
#define _PRINT_DETAIL

#define _USE_LOCK_SAMPLE

//如果定义，则根据需要输出各种消息、错误和debug信息，否则一概不输出
#define _DEBUGPRINT

//如果定义，则连接.31作为网络服务器
//#define _CONNECT_TO_31_SERVER

//如果定义则链接其他节点上的数据库服务
//#define _USEOTHERDB

//定义DEBUG，可作为测试的总开关，目前无用
#define _DEBUG

//如果不调试网络收发，则不需要真正链接服务器
#define _CONNECT_TO_SERVER

//如果定义_LOCK则锁函数都打开，否则都关闭
#define _LOCK

//如果链接自己的测试服务器，则采用的参数不同，如服务器地址等，
//#define _CONNECT_TO_MY_SERVER

//遥控线程在插入一条指令后立即读出显示
//#define _READZLAFTERINSERT



//如果定义，在程序启动时会将所有未读指令置为忽略
#define _IGNORUNREADZLWHENRESTART

//如果定义，遥控线程每次运行时，手工插入一条指令
//#define _INSERTZLMANUALLY

//如果定义，则三个线程都进行循环
#define _LOOP

//如果定义，则启动遥测线程
//#define _STARTYCTHREAD

//如果定义，则启动遥控线程
//#define _STARTYKTHREAD


//如果定义，则启动定时器
#define _SETTIMER

//更新应用状态，否则不更新，主要为调试时减少输出信息而设定
#define _UPDATE_STATE

//如果定义，则将延时遥测配置项的采集周期定义为1s～10h的范围，否则用于测试阶段可定义为1s～10s
//#define _YSYC_PZX_CJZQ

//如果定义，则采集实时遥测数据
#define _GETHER_SSYC_DATA

//如果定义，则发送socket数据(实时遥测)
#define _SEND_SOCKET_DATA

//如果定义，则处理发给自己的指令
#define _DEAL_WITH_ZL

//如果定义，则接收socket数据(指令数据)
#define _RECV_SOCKET_DATA

//如果定义，则采集延时遥测数据
#define _GETHER_YSYC_DATA

//如果定义，则按指定间隔存储实时遥测历史数据
#define _STORE_SSYC_LSSJ

//如果定义，则prePrint 输出线程号和函数序号
//#define _PRE_PRINT_THREAD_AND_FUNCSEQ

//如果定义，则发送周期遥测
#define _SEND_ZQ_SSYC

//如果定义，则在发送一条按需实时遥测数据后保留而不删除
//#define _DONT_DELETE_AFTER_SEND_AX_SSYC

//如果定义，则在发送按需实时遥测之前进行mysql_real_escape_string操作
//#define ESCAPE_STRTING_BEFORE_SEND_AXSSYC

//如果定义则，输出中文
//#define _PRINT_AS_CHINESE

//如果定义，truncate SSYCSJ table
#define _TRUNCATE_SSYCSJ_TABLE_WHENRESTART

//如果定义，则不定义64位的变量
#define _RUN_ON_711

//如果定义，则发送slow周期遥测
#define _SEND_ZQ_YC_SLOWLY

//如果定义，则发送struct周期遥测
#define _USE_SSYC_IN_STRUCT

//如果定义，则print to file
//#define _PRINT_TO_FILE



//如果定义则是运行在虚拟卫星上
//#define _RUN_ON_XNWX
#ifndef _RUN_ON_XNWX
//如果定义，则for all node
#define _GET_IP_SELF
#endif

#endif /* COMPLIER_H_ */
