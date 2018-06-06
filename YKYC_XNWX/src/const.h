/*
 * const.h
 *
 *  Created on: 2015年1月25日
 *      Author: pyt64
 */

#ifndef CONST_H_
#define CONST_H_

#include "complier.h"

//////////////////////
//有关自身节点的参数
//////////////////////
#ifdef _RUN_ON_XNWX
#define DEVICE_ID  0x1E //如果在虚拟卫星上运行设备地址都为0x1E
#else
#define DEVICE_ID  0x21 //设备地址，30-33为4个云计算节点
#endif

//////////////////////
//集成单元地址、服务端口
//////////////////////
#ifndef _RUN_ON_XNWX
#ifdef _CONNECT_TO_MY_SERVER
#ifdef  _BUILD_ON_MAC
#ifndef _CONNECT_TO_31_SERVER
#define NETSERVER_ADDR "127.0.0.1"
#endif
#endif

#ifdef  _BUILD_ON_MAC
#ifdef _CONNECT_TO_31_SERVER
#define NETSERVER_ADDR "192.168.1.31"
#endif
#endif
#else
//集成单元的地址
#define NETSERVER_ADDR "192.168.1.10"
#endif
#endif //ifndef _RUN_ON_XNWX

#ifndef _RUN_ON_XNWX
//指令转发服务端口
#define ZL_SERVER_PORT 8001
#endif //ifndef _RUN_ON_XNWX

//数传机控制服务端口
#define SCJ_KZ_SERVER_PORT 8005
//数传机数据服务端口
#define SCJ_SJ_SERVER_PORT 8006

//////////////////////
//数据库
//////////////////////
//数据库服务所在主机地址
#ifdef  _BUILD_ON_MAC
#define DB_HOST "localhost"
#else
#ifdef _USEOTHERDB
#define DB_HOST "192.168.1.31"
#else

#define DB_HOST "127.0.0.1"
//#define DB_HOST "192.168.1.32"
#endif
#endif

//yc gs
//#define _YC_STRUCT //if defined use struct else use string

//数据库名称
#define DB_NAME "node"
//数据库用户
#define DB_USER "root"
//数据库口令
#define DB_PASS "pyt999"

//////////////////////
//DB connect retry time
//////////////////////
#define MAX_DB_CONNECT_TIMES 5

//////////////////////
//JCDY connect time out count
//////////////////////
#define MAX_ZL_CONNECT_TIMEOUT 15

//////////////////////
//TCP应用层包头
//////////////////////
#define LEN_OF_TCP_HEAD  20 //TCP上的应用层格式——传输同步头长度
#define LEN_OF_TCP_SYNC 10//同步头长度
#define LEN_OF_TCP_LENGTH  4 //TCP上的应用层格式——传输同步头长度

//////////////////////
//遥测包头（外部不再使用TCP应用层包头）
//////////////////////
#define LEN_OF_SSYC_SYNC 2 //实时遥测包的同步头长度

//20171218 change
//#define LEN_OF_SSYC_HEAD 4 //实时遥测包的包头长度
#define LEN_OF_SSYC_HEAD 5 //实时遥测包的包头长度

#define SSYC_SYNC_0 0xEB
#define SSYC_SYNC_1 0x90
//////////////////////
//关键服务应用ID和广播地址
//////////////////////
#define YYID_CKFW 0x01	//测控服务
#define YYID_JKGL 0x02	//平台监控及管理
#define YYID_LRXJ 0x04	//灵瑞相机服务
#define YYID_SCFW 0x05	//数传机客户端
#define YYID_ZRGL 0x06	//注入管理
#define YYID_PTSJ 0x07	//平台数据处理
#define YYID_SJTB 0x08	//NTP时间同步
#define YYID_GBDZ 0xFF	//广播地址


//////////////////////
//指令类型
//////////////////////
#define ZLLX_CKFW 0x00	//集成单元测控指令
#define ZLLX_JKGL 0x02	//监控及管理指令
#define ZLLX_SJXJ 0x03	//手机相机指令
#define ZLLX_LRXJ 0x04	//灵瑞相机指令
#define ZLLX_SCJ  0x05	//数传指令
#define ZLLX_ZRGL 0x06	//注入指令
#define ZLLX_FLCX 0x09	//分离成像指令
#define ZLLX_KZZL 0x20	//扩展指令类型
#define ZLLX_XT 0x80	//heart beat指令类型
//////////////////////
//指令编号
//////////////////////
//集成单元测控指令的指令编号 略

//监控及管理指令的指令编号
#define ZLBH_JKGL_JDSJQL 0x01 //节点数据清理
#define ZLBH_JKGL_YYZQDSN 0x02 //应用自启动使能
#define ZLBH_JKGL_YYQDTZ 0x03 //应用启动停止
#define ZLBH_JKGL_YYSXKZ 0x04 //应用属性控制（保活、不保活）
#define ZLBH_JKGL_YYAZ 0x05 //应用安装
#define ZLBH_JKGL_YYXZ 0x06 //应用卸载
#define ZLBH_JKGL_YYSJQL 0x07 //应用数据清理 //各服务需要处理
#define ZLBH_JKGL_JDYGJ 0x08 //节点预关机 //各服务需要处理，但是广播地址，不需要写回执行结果
#define ZLBH_JKGL_ZRSJSYTZ 0x09 //注入数据使用通知 //各服务需要处理

//手机相机指令的指令编号 略

//灵瑞相机指令的指令编号 略

//数传指令的指令编号
#define ZLBH_SCJ_SCDWSZ 0x01 //数传档位设置 //数传服务需要处理
#define ZLBH_SCJ_SCSJZZ 0x02//数传数据组织 //数传服务需要处理
#define ZLBH_SCJ_SCKS 0x03 //数传启动	//数传服务需要处理
#define ZLBH_SCJ_SCTZ 0x04 //数传停止	//数传服务需要处理

//注入指令的指令编号 略

//分离成像指令的指令编号 略

//扩展指令的指令编号
#define ZLBH_KZZL_PZ 0x01 //配置指令 //测控服务需要处理


//heart beat指令编号
#define ZLBH_XT_XT 0x00

//////////////////////
//指令执行结果
//////////////////////
#define ZXJG_WD 0//未读
#define ZXJG_JS 1//接受
#define ZXJG_CG 2//执行成功
#define ZXJG_PQ -1//重启抛弃未执行指令
#define ZXJG_TJBJB -2//条件不具备
#define ZXJG_ZLCCCLFW -3//指令超出处理范围内
#define ZXJG_SB -4//执行失败
#define ZXJG_GSCW -5 //指令格式错误，如指令内容为空


//////////////////////
//数据库表名
//////////////////////
//关键服务公用表
#define table_name_JKGL_GLPZ  "GJFW_JKGL_GLPZ"//
#define table_name_JKGL_YYZT  "GJFW_JKGL_YYZT"//
#define table_name_LRXJ_SLT  "GJFW_LRXJ_SLT"//
#define table_name_LRXJ_XJZT  "GJFW_LRXJ_XJZT"//
#define table_name_LRXJ_YST  "GJFW_LRXJ_YST"//
#define table_name_LRXJ_ZP  "GJFW_LRXJ_ZP"//
#define table_name_PTSJFW_CGSJ  "GJFW_PTSJFW_CGSJ"//
#define table_name_CGSJ_GPS  "GJFW_PTSJFW_CGSJ_GPS"//
#define table_name_PTSJFW_XWSJ  "GJFW_PTSJFW_XWSJ"//
#define table_name_SCJ_SCWJ  "GJFW_SCJ_SCWJ"//
#define table_name_SZ_SZSJ  "GJFW_SZ_SZSJ"//
#define table_name_SZ_SZWJ  "GJFW_SZ_SZWJ"//
#define table_name_YC_SSYCLSSJ  "GJFW_YC_SSYCLSSJ"//
#define table_name_YC_YSYCPZX  "GJFW_YC_YSYCPZX"//
#define table_name_YK_ZL  "GJFW_YK_ZL"//
#define table_name_YC_YSYCSJ  "GJFW_YC_YSYCSJ"//
#define table_name_YC_SSYCSJ "GJFW_YC_SSYCSJ" //

//测控服务私有表
#define table_name_SELF_CKFW_ZT "SELF_CKFW_ZT" //测控服务私有表 状态表

//表的列号
//GJFW_YC_SSYCSJ表的列号(未使用)
#define column_num_of_SSYC_ID_in_table_name_YC_SSYCSJ 0 //GJFW_YC_SSYCSJ表的SSYC_ID列号
#define column_num_of_YY_ID_in_table_name_YC_SSYCSJ 1 //GJFW_YC_SSYCSJ表的YY_ID列号
#define column_num_of_SSYC_YXJ_in_table_name_YC_SSYCSJ 2 //GJFW_YC_SSYCSJ表的SSYC_YXJ列号
#define column_num_of_SSYC_NR_in_table_name_YC_SSYCSJ 3 //GJFW_YC_SSYCSJ表的SSYC_NR列号

//GJFW_YC_YSYCPZX表的列号(未使用)
#define column_num_of_YY_ID_in_table_name_YC_YSYCPZX 0 //GJFW_YC_YSYCPZX表的YY_ID列号
#define column_num_of_YSYCPZX_ID_in_table_name_YC_YSYCPZX 1 //GJFW_YC_YSYCPZX表的YSYCPZX_ID列号
#define column_num_of_YSYCPZX_CJFF_in_table_name_YC_YSYCPZX 4 //GJFW_YC_YSYCPZX表的YSYCPZX_CJFF列号

//GJFW_YK_ZL表的列号(未使用)
#define column_num_of_ZL_ID_in_table_name_YK_ZL 0 //GJFW_YK_ZL表的ZL_ID列号
#define column_num_of_YY_ID_in_table_name_YK_ZL 1 //GJFW_YK_ZL表的YY_ID列号
#define column_num_of_ZL_LX_in_table_name_YK_ZL 2 //GJFW_YK_ZL表的ZL_LX列号
#define column_num_of_ZL_BH_in_table_name_YK_ZL 3 //GJFW_YK_ZL表的ZL_BH列号
#define column_num_of_ZL_NR_in_table_name_YK_ZL 5 //GJFW_YK_ZL表的ZL_NR列号
#define column_num_of_ZL_ZXJG_in_table_name_YK_ZL 6 //GJFW_YK_ZL表的ZL_ZXJG列号

//////////////////////
//main()返回值，更新监控管理服务应用状态表时填写的应用状态字段值
//////////////////////
#define RET_NOTRUNNING 0 //0表示未运行
#define RET_RUNNING 2 //2表示正常运行
#define RET_CLOSING 3 //3表示正在正常关闭，之后由监控管理应用检查关闭情况，确认关闭成功后，由其置为RET_NOTRUNNING
#define RET_ERR_CONNECT_DB -1 //-1表示连库失败
#define RET_ERR_CONNECT_SERVER -2 //-2表示链接服务失败
#define RET_ERR_RIGIST_SIGNAL_LISENER -3 //-3表示注册信号监听程序失败
#define RET_ERR_START_YK_THREAD -4 //-4表示启动遥控线程失败
#define RET_ERR_START_YC_THREAD -5 //-5表示启动遥线程失败
#define RET_ERR_CREATE_SELF_USED_TABLES -6 // -6表示创建服务自用库表失败
#define RET_ERR_START_BASE_TIMER -7 //-7表示启动基础定时器失败
#define RET_ERR_GETIP -8 //-2表示get ip



//////////////////////
//长度、大小
//////////////////////


//指令头长度
#define ZL_HEAD_LENGTH 5
//指令帧长度
#define ZL_MAX_LENGTH 512
//socket接收长度
#ifdef _DEBUG
#define SOCKET_READ_LENGTH (ZL_MAX_LENGTH*100)
#else
#define SOCKET_READ_LENGTH ZL_MAX_LENGTH
#endif
//存放已收socket数据的缓冲长度
#define BUFFER_SIZE (SOCKET_READ_LENGTH*2)

//延时遥测采集sql长度
#define YSYC_CJFF_SQL_MAX_LENGTH 1024

//实时遥测最大长度
#define SSYC_MAX_LENGTH 256

//采集周期相同的延时遥测配置项统一处理，这里规定一个周期范围，在这个范围内的配置项归拢到同一个时间点处理，超出这个范围的不做处理
#define YSYC_PZX_CJZQ_MAX_RANGE 10
//每个种遥测采集周期的间隔时间
#define YSYC_CJZQ_1 1 //1s
#define YSYC_CJZQ_2 5 //5s
#define YSYC_CJZQ_3 10 //10s
#define YSYC_CJZQ_4 30 //30s 0.5m
#define YSYC_CJZQ_5 60 //60s 1m
#define YSYC_CJZQ_6 300 //300s 5m
#define YSYC_CJZQ_7 600 //600s 10m
#define YSYC_CJZQ_8 1800 //1800s 0.5h
#define YSYC_CJZQ_9 3600 //3600s 1h
#define YSYC_CJZQ_10 36000 //36000s 10h

//遥测采集时sql产成的最大文件长度
#define SQL_OUTFILE_MAXSIZE 102400 //最大100KB
//将遥测临时文件读出到字符数组并调用mysql_real_escape_string()转义特殊字符时，最多可能出现的特殊字符个数取文件总长的1/10
//#define SQL_OUTFILE_MAXSIZE_WITH_ESCAPE int(SQL_OUTFILE_MAXSIZE*1.1)
//还是取2倍+1比较稳妥
#define SQL_OUTFILE_MAXSIZE_WITH_ESCAPE int(SQL_OUTFILE_MAXSIZE*2+1)

//////////////////////
//时间
//////////////////////
//基础定时器的微秒数
#define BASE_TIMER_INTERVAL 500000

#define NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND 1000000/BASE_TIMER_INTERVAL
//状态更新定时器超时时间
#define UPDATE_STATE_INTERVAL (5*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //5s器更新一次状态表

//指令执行定时器超时时间
#define DEAL_WITH_ZL_INTERVAL (5*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //5s执行一次指令

//socket读取定时器超时时间
#define SOCKET_RECV_INTERVAL (1*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //2s读取一次socket数据

//输出统计信息超时时间
#define PRINT_TOTAL_INTERVAL (10*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //20s输出一次统计信息

//socket发送定时器超时时间
#ifdef _SEND_ZQ_YC_SLOWLY
#define SOCKET_SEND_INTERVAL (30*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //30s发送一次socket数据
//实时遥测数据采集定时器超时时间
#define SSYC_CJ_INTERVAL (30*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //30s采集一次实时遥测数据
//实时遥测历史数据写数据库超时时间
#define SSYCLS_STORE_INTERVAL (60*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //60s写数据库一次实时遥测历史数据

#else
//socket发送定时器超时时间
#define SOCKET_SEND_INTERVAL (1*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //1s发送一次socket数据
//实时遥测数据采集定时器超时时间
#define SSYC_CJ_INTERVAL (1*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //1s采集一次实时遥测数据
//实时遥测历史数据写数据库超时时间
#define SSYCLS_STORE_INTERVAL (10*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //10s写数据库一次实时遥测历史数据
#endif

//发送按需遥测超时时间
#define SEND_AXYC_INTERVAL (1*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //1s发送一次按需遥测

#define ZL_CONNECTION_INTERVAL (1*(NUM_OF_BASE_TIMER_TICK_FOR_A_SECOND)) //1s gZLConnectionTimeOut--
//////////////////////
//休眠时间
//////////////////////
//退出程序前等待的时间，避免有其他线程的数据库操作未完成
#define SLEEP_BEFORE_QUIT 1 //1s

#ifdef _DEBUG
//遥控线程每次循环休眠时间
#define SLEEP_YK_LOOP 0 //0s
//遥测线程每次循环休眠时间
#define SLEEP_YC_LOOP 1
#else
//遥控线程每次循环休眠时间
#define SLEEP_YK_LOOP 1 //1s
//遥测线程每次循环休眠时间
#define SLEEP_YC_LOOP 0
#endif

#ifdef _PRINT_TO_FILE
#define FILENAME "output"
#define FILEMODE "w"
#define LOGFILE gOutput
#else
#define LOGFILE stdout
#endif

//主线程每次循环休眠时间
#define SLEEP_YCYK_LOOP 30 //1s这个休眠时间决定了socket接收（不包括发送）和指令处理的动作频率 该循环目前没有用

//time wait for next db connect
#define SLEEP_FOR_NEXT_DB_CONNECT 10

//////////////////////
//select into的目标
//////////////////////
//附加在延时遥测量采集sql的后面，用于将select的结果写入一个临时文件
//#define SELECTINTO " INTO OUTFILE \"/sqlexport/pytfile\" FIELDS TERMINATED BY \',\' OPTIONALLY ENCLOSED BY \'\"\' LINES TERMINATED BY \'\\n\';"
#define SELECTINTO " INTO OUTFILE \"/home/sqlexport/pytfile\" FIELDS TERMINATED BY \',\' OPTIONALLY ENCLOSED BY \'\"\' LINES TERMINATED BY \'\\n\';"

//select into的临时文件路径
//#define YCCJ_OUTFILE_NAME "/sqlexport/pytfile"
#define YCCJ_OUTFILE_NAME "/home/sqlexport/pytfile"

////////////////////////
//定义报文数据输出格式
////////////////////////

#define HEX_FORM "%02hhx "
#define DEC_FORM "%03d "
#define UL_FORM "%lu"

////////////////////////
//定义周期发送的实时遥测的采集Sql
////////////////////////

#define SQL_GATHER_ZQSSYC "select * from SELF_JKGL_XTXXXC"

//定义三个线程的编号，用于区分数据链接
#define _YKYC 0
#define _YK 1
#define _YC 2

//////////////////////////////
//监控管理提供的系统信息
//////////////////////////////
#define table_name_CC "SELF_JKGL_CCXX" //存储信息
#define table_name_CPU "SELF_JKGL_CPUXX" //CPU信息
#define table_name_JC_CPU "SELF_JKGL_JCXX_CPU" //进程信息-CPU
#define table_name_JCMEM "SELF_JKGL_JCXX_MEM" //进程信息-MEM
#define table_name_WJLJ "SELF_JKGL_WJLJ" //ls导出文件的时间、路径信息
#define table_name_XTXX "SELF_JKGL_XTXXXC" //系统信息下传

//为记录当前秒数（自1970年）
#define UTC_NTP 2208988800U /* 1970 - 1900 */

//当在mac上编译时，IF_NAME定义为网络接口“en0”
#ifdef _BUILD_ON_MAC
#define IF_NAME "en0"
#else
#ifdef _RUN_ON_LENOVO
#define IF_NAME "wlan5"
#else
#define IF_NAME "eth0"
#endif //ifdef _RUN_ON_LENOVO
#endif //ifdef _BUILD_ON_MAC

	////////////////////////
	//JCDY connect time out count while init
	////////////////////////
	#define MAX_ZL_CONNECT_TIMEOUT_WHILE_INIT 20
	#define WAIT_TIME_WHILE_CONNECT_TO_ZLSERVER_FAILED 30 //Minute

//////////////////////////////////////////////////////////////
//如果在虚拟卫星上运行
//////////////////////////////////////////////////////////////
#ifdef _RUN_ON_XNWX

//定义虚拟卫星的卫星类别为1
#define  _WXLB_XNWX 1
//定义桌面卫星的卫星类别为2
#define  _WXLB_ZMWX 2
//定义虚拟卫星的卫星类别为1
//以下替代配置文件
///////////////////////////////

//中心数据库地址
#define CENTER_DB_HOST "140.143.228.174"
//中心数据库名称
#define CENTER_DB_NAME "satellite"
//中心数据库用户
#define CENTER_DB_USER "satellite"
//中心数据库口令
#define CENTER_DB_PASS "satellite"
//虚拟类别
#define WX_LB _WXLB_XNWX
//虚拟卫星ID
#define WX_ID 3
//节点类别
#define JD_LB 30 //节点类别
#endif

#endif /* CONST_H_ */
