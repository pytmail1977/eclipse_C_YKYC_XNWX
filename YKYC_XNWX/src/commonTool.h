/*
 * commonTool.h
 *
 *  Created on: 2017年5月19日
 *      Author: pyt64
 */

#ifndef COMMONTOOL_H_
#define COMMONTOOL_H_



#include "include.h"
#include "includecpp.h"
#include "const.h"
#include "struct.h"
#include "debug.h"
#include "global.h"



////////////////////
//服务和数据库链接和断开操作
////////////////////
//初始化netsocket_t结构
void initNetsocket(netsocket_t *pNetsocket);
//链接指令转发服务器
int connectSocket(void);
//关闭到指令转发服务的链接
int closeSocket(void);
//链接数据库
int connectDB(mysql_t *Mysql);
//关闭到数据库的链接
void closeDB(mysql_t *Mysql);

////////////////////
//缓冲区操作（队列）
///////////////////
//初始化缓冲区
void initBuffer(buffer_t* buffer);
//销毁缓冲区
void destroyBuffer(buffer_t* buffer);
//向缓冲区写入数据
int pushBuffer(buffer_t* buffer, __uint8_t data[], int dataLength);
//从缓冲区弹出数据
int popBuffer(buffer_t* buffer, __uint8_t data[], int dataLength);
//获得缓冲区中数据长度
int lengthOfBuffer(buffer_t* buffer);
//在缓冲区中定位🈯️内容的位置
int findBuffer(buffer_t* buffer, __uint8_t data[], int dataLength);
//从缓冲区读出数据
int readBuffer(buffer_t* buffer, __uint8_t data[], int dataLength, int from);
//输出缓冲区的内容
int printBuffer(buffer_t* buffer);
//清除缓冲区内容至某个位置
int cleanBuffer2Index(buffer_t* buffer, int index);
//////////////////////
//暂无用
//////////////////////
void copyUCharArray(const __uint8_t *src,  __uint8_t *dst, int count);
__uint16_t checkSum16(__uint16_t * buf, int len);

//////////////////////
//用户屏幕输出
//////////////////////
//获得当前时间
char* getTime();
//打印当前线程
void printThread(int thread);
//打印当前时间
void printTime();
//在正式输出前，打印时间和线程信息
void prePrint(funcSeq_t funcSeq);

///////////////////////
//mutex相关函数
///////////////////////
//获取一个随机数作为当次函数调用的标示
funcSeq_t getRandFuncSeq();
//void wormhole_mutex_init(worm_mutex_t *pWormMutex,void* pointer); //在mac下编译有问题，因此去掉最后一个参数，在初始化时使用NULL代替
void wormhole_mutex_init(worm_mutex_t *pWormMutex);
void wormhole_mutex_destroy(worm_mutex_t *pWormMutex);
void wormhole_mutex_lock(worm_mutex_t *pWormMutex, funcSeq_t funcSeq);
void wormhole_mutex_unlock(worm_mutex_t *pWormMutex, funcSeq_t funcSeq);
int  wormhole_mutex_trylock(worm_mutex_t *pWormMutex, funcSeq_t funcSeq);
//打印当前是那个线程锁住了指定的锁
void wormhole_mutex_wholock(worm_mutex_t *pWormMutex);

//////////////////////
//遥测配置项链表操作相关函数
//////////////////////
//new一个配置项并初始化
ycpzx_t * newYCPZX(int yy_id, int pzx_id, int cjzq, char sql[]);
//向配置项链表插入一个配置项
void insertYCPZX(ycpzx_t * head,ycpzx_t * pYCPZX);
//根据指针删除一个配置项
void deleteYCPZXbyPtr(ycpzx_t * head,ycpzx_t * pYCPZX);
//清除一个带头的配置项链表
void destroyYCPZXList(ycpzx_t * head);
//清理一个带头的配置项链表，删除除了链表头意外的其他项
void cleanYCPZCList(ycpzx_t * head);
//根据YY_ID和配置项ID删除一个延时遥测配置项
void deleteYCPZXbyId(ycpzx_t * head,int yy_id,int pzx_id);

///////////////////////
//帧内多byte长度字段转数字
///////////////////////
int getLengthOfData(__uint8_t b1, __uint8_t b2, __uint8_t b3, __uint8_t b4);

///////////////////////
//数据库读写相关辅助函数
///////////////////////
//取得用string表示的时间（便于插入数据库）
string getDateString(void);
//整形转字符串（便于插入数据库）
string int2String(int n);

///////////////////////
//多线程操作数据库时为对数据库句柄加锁，而封装的mysql相关函数
///////////////////////
//初始化一个mysql_t类型的指针，包括初始化该类型下的锁字段
void initMysql(mysql_t* pMYsql);
//销毁mysql_t类型的指针
void destroyMysql(mysql_t* pMYsql);
//封装mysql_affected_rows
my_ulonglong STDCALL self_mysql_affected_rows(mysql_t *pMYsql);
//封装mysql_query
int		STDCALL self_mysql_query(mysql_t *pMYsql, const char *q);
//封装mysql_real_query
int		STDCALL self_mysql_real_query(mysql_t* pMYsql, const char *q,unsigned long length);
//封装mysql_errno
unsigned int STDCALL self_mysql_errno(mysql_t *pMYsql);
//封装mysql_error
const char * STDCALL self_mysql_error(mysql_t *pMYsql);
//封装mysql_store_result
MYSQL_RES *     STDCALL self_mysql_store_result(mysql_t *pMYsql);
//封装mysql_num_rows
my_ulonglong STDCALL self_mysql_num_rows(mysql_t *pMYsql,MYSQL_RES *res);
//封装mysql_free_result
void		STDCALL self_mysql_free_result(mysql_t *pMYsql,MYSQL_RES *result);
//封装mysql_fetch_row
MYSQL_ROW	STDCALL self_mysql_fetch_row(mysql_t *pMYsql,MYSQL_RES *result);




//根据调用此函数所在的线程，返回适当的数据库链接
mysql_t * getMysql(void);

//调整中英文输出使用
int myfprintf (FILE *__restrict __stream,
		    __const char *__restrict __format1,__const char *__restrict __format2, ...);

int get_local_ip(char * ifname, char * ip);

#endif /* COMMONTOOL_H_ */

