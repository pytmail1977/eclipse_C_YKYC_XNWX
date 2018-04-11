/*
 * YC.h
 *
 *  Created on: 2017年5月11日
 *      Author: pyt64
 */

#ifndef YC_H_
#define YC_H_

#include "include.h"
#include "includecpp.h"
#include "complier.h"
#include "const.h"
#include "debug.h"
#include "struct.h"
#include "global.h"

//extern "C"{
#include "commonTool.h"
//}



//遥测线程主循环
void* main_loop_of_YC(void *arg);
//读取数据库，更新每个采集周期的延时遥测配置项列表
int getYSYCPZX(void);
//响应定时刷新信号，更新本地状态表和监控管理服务YY状态表
void gatherYsycData(void);
//启动监听退出信号机制
int startYSYCSJCJTimer(void);
//处理指定采集周期的延时遥测配置项
int DealWtihYSYCPZX(int iCJZQ);
//进行需周期发送的实时遥测数据采集
int gatherZqSsycData(void);
//初始化掩饰遥测配置项链表数组
void initYsycPzxListArray(void);
//销毁延时遥测配置项链表数组
void destroyYsycListArray(void);
//将实时遥测历史数据保存到数据库
void storeSSycLsSj(void);
//发送按需实时遥测数据

#ifdef _RUN_ON_XNWX
int sendAxSsycOnXNWX();
#else
int sendAxSsycOnSocket(void);
#endif
#endif /* YC_H_ */
