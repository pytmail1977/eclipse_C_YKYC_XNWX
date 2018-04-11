
#ifndef STRUCT_H_
#define STRUCT_H_


#include "include.h"
#include "complier.h"
#include "const.h"
#include "debug.h"


#pragma pack(1)


//遥控指令头部各字段类型
typedef __uint8_t YKZL_SBID_t;
typedef __uint8_t YKZL_YYID_t;
typedef __uint8_t YKZL_ZLLX_t;
typedef __uint8_t YKZL_ZLBH_t;
typedef __uint8_t YKZL_YL_t;



//遥控指令
typedef struct struct_ZLFrameHeader_s{
	YKZL_SBID_t sbid;//设备ID
	YKZL_YYID_t yyid;//应用ID
	YKZL_ZLLX_t zllx;//指令类型
	YKZL_ZLBH_t zlbh;//指令编号
	YKZL_YL_t yl;//预留
	//__int8_t zlnr[507];
}struct_ZLFrameHeader_t;


//函数随机序号（用于识别函数实例）
typedef __int32_t funcSeq_t;

//锁
typedef struct worm_mutex_s{
	pthread_mutex_t mutex; //锁
	pthread_t pt; //当前由哪个线程锁定
	funcSeq_t funcSeq; //当前由哪个线程的哪个函数实例锁定
}worm_mutex_t;


//缓冲
typedef struct buffer_s{
	__uint8_t buff[BUFFER_SIZE];
	int head; //头指针
	int tail; //尾指针
	int bIsBusy;  //是否忙（未使用）
	int flag; //锁标记（未使用）
	worm_mutex_t mutex; //锁
} buffer_t;

//socket
typedef struct netsocket_s{
	__int8_t client_sockfd; //socketfd
	struct sockaddr_in remote_addr; //服务器端网络地址结构体
} netsocket_t;

//mysql
typedef struct mysql_s{
	MYSQL   mysql;
	worm_mutex_t mutex; //锁
}mysql_t;


//提前声明
struct ycpzx_s;

//延时遥测配置量链表结构
typedef struct ycpzx_s{

	__uint8_t yy_id;//应用id
	__uint8_t pzx_id;//配置项id
	__int8_t cjzq;//采集周期
	char cjff_sql[YSYC_CJFF_SQL_MAX_LENGTH];//采集方法
	struct ycpzx_s* next;
	struct ycpzx_s* prev;

}ycpzx_t;


//todo
//根据实时遥测的最终确定的方法进行遥测
//定义实时遥测数据头
typedef struct ssyc_head_s{
	__uint8_t header[LEN_OF_SSYC_SYNC];
	__uint8_t yyId;
	__uint8_t length;
	//20171218 add
	__uint8_t gs;
}ssyc_head_t;

/*
//定时发送的实时遥测包的结构
typedef struct zqssyc_s{
	ssyc_head_t ssyc_head;
	//已定义
	__uint8_t zlCount; //指令计数
	__uint8_t zlLx; //最近指令类型
	__uint8_t zlBh; //最近指令编号
	__uint8_t zlYyId; //最近指令的YY_ID
	__uint32_t dqSjMs; //当前时间秒数
	//待定义
	__uint8_t ifZR; //0x01当前节点正在注入
	__uint8_t ifSC; //0x01当前节点正在数传
	__uint8_t ZRstate[6]; //见指令定义，智能节点遥测状态表。包括当前注入节点号1B，当前注入文件应用ID1B，当前注入文件序号1B，当前注入帧号3B
	__uint8_t SCstate[4]; //见指令定义，智能节点遥测状态表。包括数传节点及任务ID1B，数传状态1B，数传文件号2B
	__uint8_t ssyc35[35];
}zqssyc_t;
*/
//定时发送的实时遥测包的结构

typedef struct zqssyc_s{
	ssyc_head_t ssyc_head;
	//指令相关信息
	__uint8_t zlCount; //指令计数
	__uint8_t zlLx; //最近指令类型
	__uint8_t zlBh; //最近指令编号
	__uint8_t zlYyId; //最近指令的YY_ID
	__int8_t zlZxjg; //最近指令的ZL_ZXJG

	//时间相关信息
	__uint32_t dqSjMs; //当前时间秒数

	//节点相关信息
	__uint32_t kyKj;//指定磁盘空余空间
	__uint8_t YyBfb;//指定磁盘已用百分比
	__uint32_t cpu;//cpu占用率

	//20180108
	//ying yong zhuang tai
	__uint8_t yyZt[32];
	//shi fou bao huo
	__uint8_t sfBh[32];
	//shi fou zi qi dong
	__uint8_t sfZqd[32];
	//shi fou an zhuang
	__uint8_t sfAz[32];
	__uint8_t nouse;
}zqssyc_t;

typedef struct zqssyc_string{
	ssyc_head_t ssyc_head;
	char strZqYc[256];
}zqssyc_string_t;

//实时遥测包操作结构，采集和发送线程交替使用2个包结构，两个动作触发时间相差0.5秒，调度周期均为1秒
typedef struct qzssyc_struct_s{
	zqssyc_t zqssycData[2]; //实时遥测数据采集轮流使用这两个结构写入当前的遥测量值，
	int intDataCJUsing; //表示遥测数据要写入那个数据包结构，预期socket发送线程会用另一个数据包结构进行socket发送
}zqssyc_struct_t;

#ifdef _RUN_ON_711
//统计信息
typedef struct total_s{
	//从socket上接收到的总字节数
	__uint32_t sizeRecvFromSocket;
	//从收到字节流中解析出的总指令数
	__uint32_t packageResolved;
	//因设备ID不对被丢弃的指令数
	__uint32_t packageDiscardForSBIDError;
	//成功入库的指令数
	__uint32_t packageInsertedToDB;
	//入库失败的指令数
	__uint32_t packageFailToInsertedToDB;
	//解析出的所有指令的总字节数
	__uint32_t sizeOfPackageResolved;
	//重定位同步头时，丢掉的字节数
	__uint32_t sizeOfDiscardWhileRelocateSync;
	//如果指令长度错误就清楚所有指令接收缓冲
	__uint32_t sizeOfDiscardWhileInsLengthError;
	//压入buffer的字节数
	__uint32_t sizeOfPushBuffer;
	//弹出buffer的字节数
	__uint32_t sizeOfPopBuffer;
	//YKYC（主线程）pt
	pthread_t YkycPt;
	//YK线程pt
	pthread_t YkPt;
	//YC线程pt
	pthread_t YcPt;
	//heart beat指令数
	__uint32_t packageHeartBeat;
	__uint32_t countOfReConnZlServer;

}total_t;
#else

//统计信息
typedef struct total_s{
	//从socket上接收到的总字节数
	__uint64_t sizeRecvFromSocket;
	//从收到字节流中解析出的总指令数
	__uint32_t packageResolved;
	//因设备ID不对被丢弃的指令数
	__uint32_t packageDiscardForSBIDError;
	//成功入库的指令数
	__uint32_t packageInsertedToDB;
	//入库失败的指令数
	__uint32_t packageFailToInsertedToDB;
	//解析出的所有指令的总字节数
	__uint64_t sizeOfPackageResolved;
	//重定位同步头时，丢掉的字节数
	__uint64_t sizeOfDiscardWhileRelocateSync;
	//如果指令长度错误就清楚所有指令接收缓冲
	__uint64_t sizeOfDiscardWhileInsLengthError;
	//压入buffer的字节数
	__uint64_t sizeOfPushBuffer;
	//弹出buffer的字节数
	__uint64_t sizeOfPopBuffer;
	//YKYC（主线程）pt
	pthread_t YkycPt;
	//YK线程pt
	pthread_t YkPt;
	//YC线程pt
	pthread_t YcPt;
}total_t;
#endif


#pragma pack()

#endif /* STRUCT_H_ */
