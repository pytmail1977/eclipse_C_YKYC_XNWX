/*
 * YKYC.h
 *
 *  Created on: 2017年5月11日
 *      Author: pyt64
 */

#ifndef YKYC_H_
#define YKYC_H_

#include "include.h"
#include "includecpp.h"
#include "complier.h"
#include "const.h"
#include "debug.h"
#include "struct.h"
#include "global.h"

//extern "C"{
#include "commonTool.h"
//#include "YC.h.bk"
//}

#include "YK.h"


/////////////////////////////////////////////
//变量
////////////////////////////////////////////

//自用库表创建sql语句

string dropTable_SELF_CKFW_ZT = "drop table IF EXISTS " + string(table_name_SELF_CKFW_ZT) + ";";
string createTable_SELF_CKFW_ZT = "create table IF NOT EXISTS " + string(table_name_SELF_CKFW_ZT) +
"(	\
   CKFW_ZT_GXSJ         datetime, 	\
   CKFW_ZT_PID          int	not null,\
   CKFW_ZT_TID          int	default 0,\
   CKFW_ZT_SUBTID1      int	default 0,\
   CKFW_ZT_SUBTID2      int	default 0,\
   CKFW_ZT_SJK_SFLJ     int default 0,	\
   CKFW_ZT_SOCKET_SFLJ    int default 0,	\
   CKFW_ZT_YKXC_SFHY    int default 0,	\
   CKFW_ZT_YCXC_SFHY    int default 0,	\
   CKFW_ZT_ZL_COUNT		int default 0,	\
   primary key (CKFW_ZT_PID)	\
);";

//读取socket的缓冲，长度为指令最大长度
__uint8_t socketReadBuffer[SOCKET_READ_LENGTH];


/////////////////////////////////////////////
//函数
////////////////////////////////////////////

////////////////////////////
//主函数及主循环函数
////////////////////////////
int main(void);
//主线程循环
void main_loop_of_main_thread(void);



////////////////////////////
//初始化及回收相关函数
////////////////////////////
//初始化
int init(void);
//退出前处理
void quit(int yy_zt);
//创建自用库表
int createSelfUsedTables(void);


////////////////////////////
//定时器管理相关函数
////////////////////////////
//启动基础定时器
int startMainTimer(struct itimerval * pOldValue);
//停止基础定时器
int stopMainTimer(struct itimerval * pOldValue);
//启动监听退出信号机制
int startListenerToExitSignal(void);


////////////////////////////
//定时器处理相关函数
////////////////////////////
//响应基础定时器超时
void onMainTimer(int);
//更新自身状态表
int updateSelfState(void);
//更新关键服务-监控管理-应用状态表
int updataYYZT(int yy_yxzt);
//当收到退出信号时执行，将运行标记置为-1
void onSIGINT(int);


////////////////////////////
//指令处理相关函数
////////////////////////////
//处理一个未读的指令
int dealWithAnUnreadZl(void);
//处理应用数据清理指令
int onZL_JKGL_YYSJQL(int zl_id,unsigned char* ucharZL_NR);
//处理节点预关机指令
int onZL_JKGL_JDYGJ(int zl_id,unsigned char* ucharZL_NR);
//处理注入数据使用指令 目前什么也不做
int onZL_JKGL_ZRSJSYTZ(int zl_id,unsigned char* ucharZL_NR);
//处理配置指令
int onZL_KZZL_PZ(int zl_id,unsigned char* ucharZL_NR);
//向指令表写入指令的执行结果
int return_ZL_ZXJG(int zl_id,int zl_zxjg);
//应用启动时，将指令表中所有给自己的未读指令置为抛弃
int ignoreUnreadZLWhenStart(void);
//delete SSYCSJ when Start
int deleteSSYCSJWhenStart(void);

////////////////////////////
//对外IO相关函数
////////////////////////////


#ifdef _RUN_ON_XNWX
//发送周期实时遥测数据
void sendZqSsycOnXNWX(void);
#else
//发送周期实时遥测数据
void sendZqSsycOnSocket(void);
#endif

//输出统计信息
void printTJXJ(void);

//处理 heart beat 指令
int onZL_XT(int zl_id,unsigned char* ucharZL_NR);

//reconnect zl server
void zlReconnect(void);

//取得指令
int recvZl(void);

#ifndef _RUN_ON_XNWX
//读取socket写入指令缓冲
int readZlfromSocket(void);
#else
//从中央数据库获取指令
int readZlfromCenterDb(void);
//更新中央数据库中的指令状态
int updateZlZttoCenterDb(void);
#endif



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//2018-6-6
//将YC.h的内容移至此处
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

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

#endif /* YKYC_H_ */
