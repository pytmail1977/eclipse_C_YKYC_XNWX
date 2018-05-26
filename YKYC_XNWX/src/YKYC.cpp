//============================================================================
// Name        : YKYC.cpp
// Author      : PYT
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include "YKYC.h"


/*
 * 功能：主程序初始化操作，包括变量初始化，链接数据库和链接socket
 * 参数：
 * 无
 * 返回值：
 *@-1 链接数据库失败
 *@-2 链接socket失败
 *@1 成功执行
 */
int init(void){

#ifdef _PRINT_TO_FILE
gOutput = fopen(FILENAME,FILEMODE);
#endif

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: init is called.\n","调用+++YKYC.cpp 的函数: init.\n");

#ifndef _GET_IP_SELF
	  gDeviceId = DEVICE_ID;
#else
    char ip[32] = {0};
    /*
    const char char30 = "192.168.1.30";
    const char char31 = "192.168.1.31";
    const char char32 = "192.168.1.32";
    const char char33 = "192.168.1.33";
*/
    char charTmp[32];

    get_local_ip(IF_NAME, ip);

    if(0 != strcmp(ip, "")){
        //printf("***********************************************%s ip is %s\n",IF_NAME, ip);
        sprintf(charTmp,"%s",ip);
        int i;
        for (i=0;i<strlen(charTmp);i++){
        	//printf(" %d", charTmp[i]);
        }

        /*
    if (charTmp == "192.168.1.30")
    	gDeviceId = 0x30;
    else if (charTmp == "192.168.1.31")
    	gDeviceId = 0x31;
    else if (charTmp == "192.168.1.32")
    	gDeviceId = 0x32;
    else if (charTmp == "192.168.1.33")
    	gDeviceId = 0x33;
    else{
    	printf("ip address is not match.\n");
    	return -3 ;
    }
    */
        if (charTmp[strlen(charTmp)-1] ==48)
        	gDeviceId = 0x1E;
        else if (charTmp[strlen(charTmp)-1] ==49)
        	gDeviceId = 0x1F;
        else if(charTmp[strlen(charTmp)-1] ==50)
        	gDeviceId = 0x20;
        else if(charTmp[strlen(charTmp)-1] ==51)
        	gDeviceId = 0x21;
        else{
        	//printf("ip address is not match.\n");
        	return -3 ;
        }
       // printf("***********************************************Dvice id is %d\n", gDeviceId);



    }else
    {
    	//printf("can not get ip address.\n");
    	return -3 ;
    }
#endif
	///////////////////////////
	//退出控制变量初始化
	///////////////////////////
	gIntIsRun = 1;

	///////////////////////////
	//进程号、线程号初始化
	///////////////////////////
	gPid = 0;
	gTid = 0;
	gSubTid_1 = 0;
	gSubTid_2 = 0;

	///////////////////////////
	//链接状态变量初始化
	///////////////////////////
	gIntIsDbConnected = 0;
#ifndef _RUN_ON_XNWX
	gIntIsSocketConnected = 0;
#else
	gIntIsCenterDBConnected = 0;
#endif

	//////////////////////////
	//线程状态初始化
	//////////////////////////
	gIntIsYKThreadLive = 0;
	gIntIsYCThreadLive = 0;


	//////////////////////
	//TCP应用层封包头gSYNC_HEADER初始化
	//////////////////////
	gSYNC_HEADER[0] = 0xAA;
    gSYNC_HEADER[1] = 0xFE;
    gSYNC_HEADER[2] = 0x55;
    gSYNC_HEADER[3] = 0xFE;
	gSYNC_HEADER[4] = 0x00;
    gSYNC_HEADER[5] = 0xFE;
    gSYNC_HEADER[6] = 0xFF;
    gSYNC_HEADER[7] = 0x53;
    gSYNC_HEADER[8] = 0x44;
    gSYNC_HEADER[9] = 0x53;

	//////////////////////
	//实时遥测数据结构初始化
	//////////////////////
	gSsycStruct.intDataCJUsing = 0;

	bzero(&gSsycStruct.zqssycData[0],sizeof(zqssyc_t));
	bzero(&gSsycStruct.zqssycData[1],sizeof(zqssyc_t));

	gSsycStruct.zqssycData[0].ssyc_head.header[0] = SSYC_SYNC_0;
	gSsycStruct.zqssycData[0].ssyc_head.header[1] = SSYC_SYNC_1;
	gSsycStruct.zqssycData[1].ssyc_head.header[0] = SSYC_SYNC_0;
	gSsycStruct.zqssycData[1].ssyc_head.header[1] = SSYC_SYNC_1;

	gSsycStruct.zqssycData[0].ssyc_head.length = sizeof(zqssyc_t) - sizeof(ssyc_head_t);
	gSsycStruct.zqssycData[1].ssyc_head.length = sizeof(zqssyc_t) - sizeof(ssyc_head_t);

	gSsycStruct.zqssycData[0].ssyc_head.yyId = YYID_CKFW;
	gSsycStruct.zqssycData[1].ssyc_head.yyId = YYID_CKFW;

	//20171218 add
	//20171225 change
#ifdef _USE_SSYC_IN_STRUCT
	gSsycStruct.zqssycData[0].ssyc_head.gs = 1;
	gSsycStruct.zqssycData[1].ssyc_head.gs = 1;
#else
	gSsycStruct.zqssycData[0].ssyc_head.gs = 0;
	gSsycStruct.zqssycData[1].ssyc_head.gs = 0;
#endif

	//////////////////////////
	//应用心跳初始化
	//////////////////////////
	gYySxxt = 0;

	//////////////////////////
	//指令计数初始化
	//////////////////////////
	gZlCount = 0;
	//最近指令类型
	gZlLx = 0;
	//最近指令编号
	gZlBh = 0;
	//最近指令的YY_ID
	gZlYyId = 0;
	//周期遥测计数
	gZqYcCount = 0;
	//按需遥测计数
	gAxYcCount = 0;


	/////////////////////////
	//定时器超时次数初始化
	/////////////////////////
	gBaseTimerCount = 0;

	//////////////////
	//取进程和线程ID
	//////////////////
	//取进程ID
	gPid = getpid();
	tmpPrint(LOGFILE,"TMP---Pid in main thread is %d.\n","临时---主线程Pid：%d.\n",gPid);

	//取主线程ID
	//gTid = pthread_self();
	//tmpPrint(LOGFILE,"TMP---Tid in main thread is %lu.\n",gTid);

	///////////////////////////////
	//网络链接变量初始化
	///////////////////////////////
#ifndef _RUN_ON_XNWX
	initNetsocket(&gNetSocket);
#else
	initMysql(&gMysql_centerDBforYK);
	initMysql(&gMysql_centerDBforYC);
#endif

	//////////////////////////////
	//数据库链接变量初始化
	//////////////////////////////
	//initMysql(&gMysql);
	initMysql(&gMysql[_YKYC]);
	initMysql(&gMysql[_YK]);
	initMysql(&gMysql[_YC]);

	//////////////////////////////
	//socket缓冲区初始化
	//////////////////////////////
#ifndef _RUN_ON_XNWX
	initBuffer(&gSocketBuffer);
#endif


	////////////////////////////////
	//链接数据库，如果失败就返回-1
	///////////////////////////////
	//if (0>=connectDB())
	//	return -1; //-1表示无法链接到数据库
	////////////////////////////////
	//链接数据库，如果失败就返回-1
	///////////////////////////////

	int try_times=0;
	int success=0;
	while(success==0 and try_times<MAX_DB_CONNECT_TIMES){
		if (0>=connectDB(&gMysql[_YKYC])){
			try_times++;
			sleep(SLEEP_FOR_NEXT_DB_CONNECT);
		}else{
			success=1;
		}
	}

	if (success==0){
	//if (0>=connectDB(&gMysql[_YKYC])){
		errorPrint(LOGFILE,"ERR---Can't connect DB:%d.\n","错误---链接数据库失败（错误编号：%d）.\n", errno);
		return -1; //-1表示无法链接到数据库
	}else if (0>=connectDB(&gMysql[_YK])){
		closeDB(&gMysql[_YKYC]);
		errorPrint(LOGFILE,"ERR---Can't connect DB:%d.\n","错误---链接数据库失败（错误编号：%d）.\n", errno);
		return -1;
	}else if (0>=connectDB(&gMysql[_YC])){
		closeDB(&gMysql[_YKYC]);
		closeDB(&gMysql[_YK]);
		errorPrint(LOGFILE,"ERR---Can't connect DB:%d.\n","错误---链接数据库失败（错误编号：%d）.\n", errno);
		return -1;
	}

	/* 20171227 change

	if (0>=connectDB(&gMysql[_YKYC])){
		errorPrint(LOGFILE,"ERR---Can't connect DB:%d.\n","错误---链接数据库失败（错误编号：%d）.\n", errno);
		return -1; //-1表示无法链接到数据库
	}else if (0>=connectDB(&gMysql[_YK])){
		closeDB(&gMysql[_YKYC]);
		errorPrint(LOGFILE,"ERR---Can't connect DB:%d.\n","错误---链接数据库失败（错误编号：%d）.\n", errno);
		return -1;
	}else if (0>=connectDB(&gMysql[_YC])){
		closeDB(&gMysql[_YKYC]);
		closeDB(&gMysql[_YK]);
		errorPrint(LOGFILE,"ERR---Can't connect DB:%d.\n","错误---链接数据库失败（错误编号：%d）.\n", errno);
		return -1;
	}

	*/

	gIntIsDbConnected = 1;

	////////////////////////////////
	//链接服务器，如果失败就返回-2
	///////////////////////////////
#ifndef _RUN_ON_XNWX
#ifdef	_CONNECT_TO_SERVER

/*
	if (0 >= connectSocket()){
		errorPrint(LOGFILE,"ERR---Can't connect ZL server:%d.\n","错误---链接服务器失败（错误编号：%d）.\n", errno);
		return -2;//-2表示无法链接到指令转发服务
	}

*/
	if (0 >= connectSocket()){
		errorPrint(LOGFILE,"ERR---Can't connect ZL server:%d.\n","错误---链接服务器失败（错误编号：%d）.\n", errno);
		//return -2;//-2表示无法链接到指令转发服务
	}
	////////////////////////////////
	//链接服务器MAX_ZL_CONNECT_TIMEOUT_WHILE_INIT，如果失败就返回-2
	///////////////////////////////
/*
	int iCount = 0;
	while (1){
		if (0 < connectSocket()){
			break;
		}else{
			iCount++;
			errorPrint(LOGFILE,"ERR---Can't connect ZL server:%d, %d times.\n","错误---链接服务器失败（错误编号：%d）%d times.\n", errno,iCount);
		}

		if (iCount >= MAX_ZL_CONNECT_TIMEOUT_WHILE_INIT){
			errorPrint(LOGFILE,"ERR---Can't connect ZL server, give up.\n","错误---链接服务器失败. Give up.\n");
			return -2;//-2表示无法链接到指令转发服务
		}else{
			sleep(WAIT_TIME_WHILE_CONNECT_TO_ZLSERVER_FAILED);
		}
	}
*/
#endif
#else
	if (0>=connectCenterDB(&gMysql_centerDBforYK)){
		errorPrint(LOGFILE,"ERR---Can't connect Center DB:%d.\n","错误---链接中央数据库失败（错误编号：%d）.\n", errno);
		return -2; //-1表示无法链接到中央数据库
	}else if (0>=connectCenterDB(&gMysql_centerDBforYC)){
		closeDB(&gMysql_centerDBforYK);
		errorPrint(LOGFILE,"ERR---Can't connect Center DB:%d.\n","错误---链接中央数据库失败（错误编号：%d）.\n", errno);
		return -2;
	}

	gIntIsCenterDBConnected = 1;

#endif
	////////////////////////////////
	//统计信息初始化
	///////////////////////////////
	bzero(&gTotal,sizeof(total_t));

	/////////////////////////////////
	//ZL connect timeout count
	/////////////////////////////////
	 gZLConnectionTimeOut = MAX_ZL_CONNECT_TIMEOUT;

	msgPrint(LOGFILE,"MSG---Init success.\n","消息---初始化成功.\n");
	return 1;
}


/*
 * 功能：退出前的操作，断开与数据库和服务的链接，释放socket缓冲区
 * 参数:
 * @int gYyYxzt指定在退出前，写入应用table_name_JKGL_YYZT的应用状态，包括RET_ERR_CONNECT_SERVER等
 * 返回值：
 * @无，无论更新应用状态或断开链接是否成功都继续关闭动作
 */
void quit(int gYyYxzt){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: exit is called.\n","调用+++YKYC.cpp 的函数: exit.\n");


#ifdef _SETTIMER
	//停止基础定时器
	int ret = stopMainTimer(NULL);
	/*
	if (ret!=1){
		errorPrint(LOGFILE,"ERR---Can't stop base timer: %s.\n","错误---停止主定时器失败: %s.\n", strerror(ret));
	}
	*/

#endif

	//更新应用状态表
	updataYYZT(gYyYxzt);

#ifndef _RUN_ON_XNWX
	//断开链接
	closeSocket();
#endif

#ifndef _RUN_ON_XNWX
	//释放缓冲区
	destroyBuffer(&gSocketBuffer);
#endif
	//释放存储延时遥测配置量的链表数组
	destroyYsycListArray();

	//未避免有其它线程的数据库操作未完成，在关闭数据库之前，暂停一会儿
	sleep(SLEEP_BEFORE_QUIT);

	//关闭中央数据库
#ifdef _RUN_ON_XNWX
	if(1 == gIntIsCenterDBConnected){
		closeDB(&gMysql_centerDBforYK);
		closeDB(&gMysql_centerDBforYC);
		gIntIsCenterDBConnected = 0;
	}

#endif

	//关闭数据库
	//closeDB();
	closeDB(&gMysql[_YKYC]);
	closeDB(&gMysql[_YK]);
	closeDB(&gMysql[_YC]);
	gIntIsDbConnected = 0;

	//释放数据库链接
	//destroyMysql(&gMysql);
	destroyMysql(&gMysql[_YKYC]);
	destroyMysql(&gMysql[_YK]);
	destroyMysql(&gMysql[_YC]);

	msgPrint(LOGFILE,"MSG---Quit success.\n","消息---成功退出.\n");

	#ifdef _PRINT_TO_FILE
	 fclose(gOutput);
	#endif

}



/*
 * 功能：更新自身状态表，包括是否运行，是否联网，遥控线程是否活跃，遥测线程是否活跃
 * 参数：
 * @bool bInsert，为true表示第一次更新自有状态，插入一条新记录，否则false表示更新现有记录
 * 返回值：
 * @-1：数据库未链接；
 * @-2：insert/update失败；
 * @1：成功返回
 */
int updateSelfState(bool bInsert){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: updateSelfStat is called.\n","调用+++YKYC.cpp 的函数: updateSelfStat.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n\n");
		return -1;
	}



	//判断遥控线程状态
	gIntIsYKThreadLive = 0;
	if (gIntNewStatYKThread != gIntOldStatYKThread){
		gIntOldStatYKThread = gIntNewStatYKThread;
		gIntIsYKThreadLive = 1;
	}

	//判断遥测线程状态
	gIntIsYCThreadLive = 0;
	if (gIntOldStatYCThread != gIntNewStatYCThread){
		gIntOldStatYCThread = gIntNewStatYCThread;
		gIntIsYCThreadLive = 1;
	}

	//tmpPrint(LOGFILE,"TMP---gIntIsYKThreadLive: %d.\n",gIntIsYKThreadLive);
	//tmpPrint(LOGFILE,"TMP---gIntIsYCThreadLive: %d.\n",gIntIsYCThreadLive);
	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1){
		errorPrint(LOGFILE,"ERR---Update table %s failed, DB is not connected.\n","错误---更新%s表失败，数据库未链接.\n",table_name_SELF_CKFW_ZT);
		return -1;
	}

	//构造一个插入自身状态的语句，如果是应用首次运行到这里，表中还没有记录，需要执行该语句插入一条记录
	string sqlInsertSelfState = "insert into "+
			string(table_name_SELF_CKFW_ZT)+
			" values(\'" +
			getDateString() +     //更新时间
			"'," +
			int2String(gPid) +	//gPid
			"," +
			int2String(gTid) +	//主线程tid
			"," +
			int2String(gSubTid_1) +	//遥控线程tid
			"," +
			int2String(gSubTid_2) +	//遥测线程tid
			"," +
			int2String(gIntIsDbConnected) +	//是否链接数据库
			"," +
#ifndef _RUN_ON_XNWX
			int2String(gIntIsSocketConnected) + //是否链接socket
#else
			int2String(gIntIsCenterDBConnected) + //是否链接socket
#endif
			"," +
			int2String(gIntIsYKThreadLive) + //遥控线程是否活跃
			"," +
			int2String(gIntIsYCThreadLive) + //遥测线程是否活跃
			"," +
			int2String(gZlCount) + //指令计数
			")";


	//构造一条更新语句，如果不是首次，则已经有记录在表中，只需要更新它
	string sqlUpdateSelfState = "update SELF_CKFW_ZT set CKFW_ZT_GXSJ = \'"+
			//"= \'"+
			getDateString() +
			"'"+
			", CKFW_ZT_TID = "+
			int2String(gTid) +
			", CKFW_ZT_SUBTID1 = "+
			int2String(gSubTid_1) +
			", CKFW_ZT_SUBTID2 = "+
			int2String(gSubTid_2) +
			" , CKFW_ZT_SJK_SFLJ = "+
			int2String(gIntIsDbConnected) +
			" , CKFW_ZT_SOCKET_SFLJ = "+
#ifndef _RUN_ON_XNWX
			int2String(gIntIsSocketConnected)+
#else
			int2String(gIntIsCenterDBConnected)+
#endif
			" , CKFW_ZT_YKXC_SFHY = "+
			int2String(gIntIsYKThreadLive)+
			" , CKFW_ZT_YCXC_SFHY = "+
			int2String(gIntIsYCThreadLive)+
			" , CKFW_ZT_ZL_COUNT = "+
			int2String(gZlCount) +
			" where CKFW_ZT_PID = "+
			int2String(gPid) +
			" ;";

	//调试输出sql语句
	if (bInsert){
		sqlPrint(LOGFILE,"SQL---Insert table %s sql: %s.\n","SQL---插入%s表SQL: %s.\n",table_name_SELF_CKFW_ZT,sqlInsertSelfState.c_str());
	}
	else{
		sqlPrint(LOGFILE,"SQL---Update table %s sql: %s.\n","SQL---更新%s表SQL: %s.\n",table_name_SELF_CKFW_ZT,sqlUpdateSelfState.c_str());
	}

	//执行语句，更新程序自身状态表
	int res;
	if (bInsert)
		res= self_mysql_query(selfMysqlp, sqlInsertSelfState.c_str());
	else
		res= self_mysql_query(selfMysqlp, sqlUpdateSelfState.c_str());

    if (!res) {
         prgPrint(LOGFILE,"PRG---Insert/update self state table %s, affact %d rows.\n","过程---插入/更新自用状态表%s ，影响%d行.\n", table_name_SELF_CKFW_ZT, self_mysql_affected_rows(selfMysqlp));
     } else {
         //原输出（错误转储）
    	 errorPrint(LOGFILE, "ERR---Insert/update self state table %s failed, error %d: %s.\n", "错误---插入/更新自用状态表%s失败（错误编号：%d，%s）.\n",table_name_SELF_CKFW_ZT, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));

    	 //改调用mysql_errno和mysql_error的输出（错误转储）
    	 //errorPrint(LOGFILE, "ERR---Insert/update self state table %s failed, error %d: %s.\n", "错误---插入/更新自用状态表%s失败（错误编号：%d，%s）.\n",table_name_SELF_CKFW_ZT, mysql_errno(&selfMysqlp->mysql), mysql_error(&selfMysqlp->mysql));

    	 //改用printf的输出（正确）
    	 //fprintf(LOGFILE, "错误---插入/更新自用状态表%s失败（错误编号：%d，%s）.\n",table_name_SELF_CKFW_ZT, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));

         //改用printf的输出，且改调用mysql_errno和mysql_error（正确）
       	 //fprintf(LOGFILE, "错误---插入/更新自用状态表%s失败（错误编号：%d，%s）.\n",table_name_SELF_CKFW_ZT, mysql_errno(&selfMysqlp->mysql), mysql_error(&selfMysqlp->mysql));

    	 return -2;
     }

	prgPrint(LOGFILE,"PRG---Update table %s success.\n","过程---更新自用状态表%s成功.\n",table_name_SELF_CKFW_ZT);

    return 1;

}


/*
 * 功能：更新监控管理服务应用状态表
 * 参数：
 * @gYyYxzt：应用状态
 * 返回值：
 * @-1：数据库未链接；
 * @-2：insert/update失败；
 * @1：成功返回
 */
int updataYYZT(int gYyYxzt){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: updataYYZT is called.\n","调用+++YKYC.cpp 的函数: updataYYZT.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}




	/*
	if(gIntIsYKThreadLive && gIntIsYCThreadLive)
		gYySxxt++;
		*/

	if(gIntIsYKThreadLive)
		gYySxxt++;


	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1)
		return -1;

	string tablename = table_name_JKGL_YYZT;

	//构造一个更新sql语句
	string sqlUpdateYYZT = "update "+
			tablename +
			" set YY_PID = "+
			int2String(gPid) +
			" , YY_YXZT = "+
			int2String(gYyYxzt)+
			" , YY_SXXT = "+
			int2String(gYySxxt)+
			" where YY_ID = "+
			int2String(YYID_CKFW)+
			" ;";

	int ret = self_mysql_query(selfMysqlp, sqlUpdateYYZT.c_str());

	sqlPrint(LOGFILE,"SQL---Update table %s SQL: %s.\n","SQL---更新应用状态表%s SQL: %s.\n",table_name_JKGL_YYZT,sqlUpdateYYZT.c_str());

    if (!ret) {
    	prgPrint(LOGFILE,"PRG---Update table %s, affact %d rows.\n", "过程---更新应用状态表%s %llu 行.\n", table_name_JKGL_YYZT, self_mysql_affected_rows(selfMysqlp));

     } else {
         errorPrint(LOGFILE, "ERR---Update table %s error %d: %s.\n","错误---更新应用状态表%s失败（错误编号：%d，%s）.\n", table_name_JKGL_YYZT, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return -2;
     }

	prgPrint(LOGFILE,"PRG---Update table %s success.\n","过程---更新应用状态表%s成功.\n",table_name_JKGL_YYZT);

	return 1;

}

/*
 * 功能：创建应用自用的状态表
 * 参数：
 * 无
 * 返回值：
 * @-1：数据库未链接；
 * @-2：删除旧表失败；
 * @-3：创建新表失败；
 */
int createSelfUsedTables(void){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: createSelfUsedTables is called.\n","调用+++YKYC.cpp 的函数: createSelfUsedTables.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1){
		errorPrint(LOGFILE,"ERR---Create self used tables fail, DB is not connected.","错误---创建自用表失败，数据库未链接.");
		return -1;
	}



	int ret = self_mysql_query(selfMysqlp, dropTable_SELF_CKFW_ZT.c_str());

	sqlPrint(LOGFILE,"SQL---Drop self used tables: %s.\n","SQL---DROP自用表SQL: %s.\n",dropTable_SELF_CKFW_ZT.c_str());

    if (!ret) {
         prgPrint(LOGFILE,"PRG---Drop self used tables, affact %d rows.\n","消息---DROP自用表 ，影响%d行.\n",
                 self_mysql_affected_rows(selfMysqlp));

     } else {
         errorPrint(LOGFILE, "ERR---Drop self used tables error %d: %s.\n", "错误---DROP自用表失败（错误编号：%d: %s）.\n",self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return -2;
     }


	ret = self_mysql_query(selfMysqlp, createTable_SELF_CKFW_ZT.c_str());

	sqlPrint(LOGFILE,"SQL---Create self used tables: %s.\n","SQL---创建自用表SQL: %s.\n", createTable_SELF_CKFW_ZT.c_str());

    if (!ret) {
         prgPrint(LOGFILE,"PRG---create self used tables, affact %d rows.\n","过程---创建自用表 ，影响%d行.\n",
                 self_mysql_affected_rows(selfMysqlp));

     } else {
         errorPrint(LOGFILE, "ERR---Create self used tables error %d: %s.\n","错误---创建自用表失败（错误编号：%d，%s）.\n", self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return -3;
     }

    msgPrint(LOGFILE,"MSG---Create self used tables success.\n","消息---创建自用表成功.\n");

    //updateSelfState(true);

	return 1;
}




/*
 * 功能：响应定时刷新信号，更新本地状态表和监控管理服务YY状态表
 * 参数：
 * @ int
 * 返回值
 * 无
 */
void onMainTimer(int)
{
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: onMainTimer is called.\n", "调用+++YKYC.cpp 的函数: onMainTimer.\n");

	gBaseTimerCount++;

	//根据定时器超时次数是偶数还是奇数分别调用实时遥测数据采集和实时遥测数据发送
	if(gBaseTimerCount%2 == 1){

		//if的这一半都要用“==1”来判是否到了触发时间
//周期性发送的实时遥测数据采集
#ifdef _GETHER_SSYC_DATA
		if(gBaseTimerCount%SSYC_CJ_INTERVAL == 1){
			prgPrint(LOGFILE,"PRG---It is time to gather Ssyc data.\n", "过程---准备收集实时遥测数据.\n");
			gatherZqSsycData();
		}
#endif

//处理指令
#ifdef _DEAL_WITH_ZL

	if(gBaseTimerCount%DEAL_WITH_ZL_INTERVAL == 1){
	    prgPrint(LOGFILE,"PRG---It is time to deal with zl.\n", "过程---准备执行指令");
	    //处理给自己的指令
	    dealWithAnUnreadZl();
	}
#endif

#ifndef _RUN_ON_XNWX
	//reconnect zl server if gZLConnectionTimeOut<=0
	if(gBaseTimerCount%ZL_CONNECTION_INTERVAL == 1){
	    prgPrint(LOGFILE,"PRG---It is time to deal with zl.\n", "过程---准备执行指令");
	    //处理给自己的指令
	    gZLConnectionTimeOut--;
	    if (gZLConnectionTimeOut<=0){
	    	gZLConnectionTimeOut = MAX_ZL_CONNECT_TIMEOUT;
	         errorPrint(LOGFILE,"ERR---Lost heart beats. reconnect to zl server.\n","错误---Lose heart beats. reconnect to zl server.\n");
	    	zlReconnect();
	    }
	}
#endif
	//存储实时遥测历史数据
	#ifdef _STORE_SSYC_LSSJ

		if(gBaseTimerCount%SSYCLS_STORE_INTERVAL == 1){
		    prgPrint(LOGFILE,"PRG---It is time to store ssyc data.\n","过程---准备存储实时遥测数据");
		    //处理给自己的指令
		    //printf("+++++++storeSSycLsSj+++++++\n");
		    storeSSycLsSj();
		}
	#endif

		if(gBaseTimerCount%PRINT_TOTAL_INTERVAL == 1){
			printTJXJ();
		}

	}else{
		//if的这一半都要用“==0”来判是否到了触发时间

//发周期实时遥测数据

#ifdef _SEND_SOCKET_DATA
#ifdef _SEND_ZQ_SSYC

		tmpPrint(LOGFILE,"TMP---gBaseTimerCount = %llu.\n","临时---gBaseTimerCount = %llu.\n",gBaseTimerCount);
		tmpPrint(LOGFILE,"TMP---SOCKET_SEND_INTERVAL = %d.\n","临时---SOCKET_SEND_INTERVAL = %d.\n",SOCKET_SEND_INTERVAL);

		if(gBaseTimerCount%SOCKET_SEND_INTERVAL == 0){
				prgPrint(LOGFILE,"PRG---It is time to send Zq Ssyc data.\n", "过程---准备发送zq实时遥测数据.\n");
#ifndef _RUN_ON_XNWX
				sendZqSsycOnSocket();
#else
				sendZqSsycOnXNWX();
#endif
		}
#endif
#endif

//接收遥控数据
#ifdef _RECV_SOCKET_DATA
		if(gBaseTimerCount%SOCKET_RECV_INTERVAL == 0){
			prgPrint(LOGFILE,"PRG---It is time to recv ZL data.\n", "过程---准备接收指令数据.\n");
			if(1 == recvZl()){
				//DEBUG_STOP
				;
			}
		}
#endif

//采集延时遥测数据
//延时遥测数据采集放在实时遥测数据发送的半秒调度
#ifdef _GETHER_YSYC_DATA


			gatherYsycData();

#endif

//更新应用状态
#ifdef _UPDATE_STATE

		if(gBaseTimerCount%UPDATE_STATE_INTERVAL == 0){
			prgPrint(LOGFILE,"PRG---It is time to update state.\n","过程---准备更新自身状态表和应用状态表.\n");
		    //更新自身状态表
		    updateSelfState(false);
		    //更新监控管理服务应用状态表
		    updataYYZT(RET_RUNNING);
		}
#endif

	}//else




}

/*
 * 功能：启动监听退出信号机制
 * 参数：
 * 无
 * 返回值：
 * @-1：失败；
 * @1：成功；
 */
int startListenerToExitSignal(void){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: startListenerToExitSignal is called.\n","调用+++YKYC.cpp 的函数: startListenerToExitSignal.\n");

    if(signal(SIGINT|SIGQUIT|SIGTERM, onSIGINT) == SIG_ERR)
    {
         errorPrint(LOGFILE,"ERR---register a listener for SIGINT|SIGQUIT|SIGTERM fail.\n","错误---注册SIGINT|SIGQUIT|SIGTERM信号侦听器失败.\n");
         return -1;
    }else{
    	msgPrint(LOGFILE,"MSG---startListenerToExitSignal success.\n","消息---开始侦听SIGINT|SIGQUIT|SIGTERM信号.\n");
    	return 1;
    }

}

/*
 * 功能：根据BASE_TIMER值设置基础定时器，绑定处理函数
 * 参数：
 * @pOldValue：用于返回定时器原始值
 * 返回值：
 * @1:设置成功；
 * @-1：设置失败;
 *
 */
int startMainTimer(struct itimerval * pOldValue){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: startMainTimer is called.\n", "调用+++YKYC.cpp 的函数: startMainTimer.\n");

	//注册一个定时器
    if(signal(SIGALRM , onMainTimer) == SIG_ERR)
    {
    	errorPrint(LOGFILE,"ERR---Register Main Timer callback function fail.\n","错误---注册主定时器回调函数失败.\n");
    	return -1;
    }

    //立即触发定时器
    //alarm(0);

    //设置定时器按BASE_TIMER微秒周期触发定时器
	struct itimerval value;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = BASE_TIMER_INTERVAL;
    value.it_interval.tv_sec = 0;
    value.it_interval.tv_usec = BASE_TIMER_INTERVAL;

    if (setitimer(ITIMER_REAL, &value, pOldValue) != 0){
    	errorPrint(LOGFILE,"ERR---Set main timer fail, errno = %d.\n","错误---设置主定时器失败（错误编号：%d）.\n", errno);
    	return -1;
    }else{
    	msgPrint(LOGFILE,"MSG---startMainTimer success.\n","消息---启动主定时器成功.\n");
    	return 1;
    }
}

/*
 * 功能：：取消定时器设置，恢复原值
 * 参数：
 * @pOldValue：用于传入定时器原始值;
 * 返回值：
 * @1:设置成功；
 * @-1：设置失败;
 *
 */
int stopMainTimer(struct itimerval * pOldValue){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: stopMainTimer is called.\n", "调用+++YKYC.cpp 的函数: stopMainTimer.\n");

    struct itimerval value;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 0;
    value.it_interval = value.it_value;
    int ret = setitimer(ITIMER_REAL, &value, pOldValue);
    if (0 == ret){
    	msgPrint(LOGFILE,"MSG---stopMainTimer success.\n","消息---停止主定时器成功.\n");
    	return 1;
    }
    else{
    	errorPrint(LOGFILE,"ERR---Stop Main Timer fail errno = %d.\n","错误---停止主定时器失败（错误编号：%d）.\n",errno);
    	return -1;
    }


}

/*
 * 功能：当收到退出信号时执行，将运行标记置为0
 * 参数
 * @int；
 * 返回值：
 * 无
 */

void onSIGINT(int){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: onSIGINT is called.\n","调用+++YKYC.cpp 的函数: onSIGINT.\n");

	//////////////////
	//将intRun置为0
	/////////////////
	gIntIsRun = 0; //以前置为-1，程序并不退出，应该置为0，各线程的循环才能终止;
	msgPrint(LOGFILE,"MSG---onSIGINT success.\n","消息---收到退出信号，intRun置为0.\n");

}



/*
 * 主函数
 */

int main(void) {


	/////////////////////
	//初始化随机数种子，用于调试时给每个函数的每次运行一个不重复的随机编号
	/////////////////////
	//为了临近两次调用getRandFuncSeq()的结果也不同，需要协调getRandAddr()函数，将srand((unsigned)time(NULL));语句统一放到函数外部，在main()中调用。
	srand((unsigned)time(NULL));


	/////////////////////
	//输出函数基本信息
	/////////////////////
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: main is called.\n","调用+++YKYC.cpp 的函数: main.\n");

	/////////////////////
	//下面的输出用于验证格式的正确性
	/////////////////////
	//tmpPrint(LOGFILE,SELECTINTO);

	/////////////////////
	//输出关于进程地址空间的调试信息
	/////////////////////
	/*
	tmpPrint(LOGFILE,"TMP---Process Test location:\n");
	tmpPrint(LOGFILE,"TMP---TMP---Address of main(Code Segment)：%p.\n",main);

	tmpPrint(LOGFILE,"TMP---_____________________________________.\n");
	*/


	//////////////////
	//初始化
	//////////////////
	int ret = init();
	/*
	int ret=0;
	while(1!=ret){
		sleep(3);
		ret = init();
	}
	*/


	if (-1 == ret){
		//errorPrint(LOGFILE,"ERR---Can't connect DB:%s.\n", strerror(ret));
		quit(RET_ERR_CONNECT_DB);
		return RET_ERR_CONNECT_SERVER;//表示连库失败
	}
	else if (-2 == ret){
		//errorPrint(LOGFILE, "ERR---Can't connect ZL Server:%s.\n", strerror(ret));
		quit(RET_ERR_CONNECT_SERVER);

		//Do not return while 链接服务失败
		//return RET_ERR_CONNECT_SERVER; //表示链接服务失败
	}else if (-3 == ret){
		//errorPrint(LOGFILE, "ERR---Can't connect ZL Server:%s.\n", strerror(ret));
		quit(RET_ERR_GETIP);
		return RET_ERR_GETIP; //表示get ip失败
	}

	//update sxxt immediately after init()
    //更新监控管理服务应用状态表
    updataYYZT(RET_RUNNING);

	//记录主线程pt
	gTotal.YkycPt = pthread_self();

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}



	//////////////////////////////
	//延时遥测配置项链表数组
	//////////////////////////////
	initYsycPzxListArray();
	//////////////////
	//读取延时遥测配置项，获得需要采集的延时遥测项
	//////////////////
	getYSYCPZX();


	///////////////////////////////////
	//建立自用库表
	///////////////////////////////////
	ret = createSelfUsedTables();
	if (ret!=1){
		//msgPrint(LOGFILE,"MSG---Can't create self used tables :%s.\n", strerror(ret));
		quit(RET_ERR_CREATE_SELF_USED_TABLES);
		return RET_ERR_CREATE_SELF_USED_TABLES; //表示创建服务自用库表失败
	}

	//原来在createSelfUsedTables（）最后调用，现在挪到外面
	updateSelfState(true);

	//////////////////////////////
    //注册一个退出信号监听程序
	//////////////////////////////
	ret = startListenerToExitSignal();
	if (ret!=1){
		//msgPrint(LOGFILE,"MSG---Can't start listener to exit signal :%s.\n", strerror(ret));
		quit(RET_ERR_RIGIST_SIGNAL_LISENER);
		return RET_ERR_RIGIST_SIGNAL_LISENER; //表示注册退出信号监听程序失败
	}



    /////////////////////////////////////////////////////
	//启动基础定时器
    /////////////////////////////////////////////////////
#ifdef _SETTIMER
	//struct itimerval * pOldValue = new (struct itimerval);
	ret = startMainTimer(NULL);
	if (ret!=1){
		//msgPrint(LOGFILE,"MSG---Can't start base timer: %s.\n", strerror(ret));
		quit(RET_ERR_START_BASE_TIMER);
		return RET_ERR_START_BASE_TIMER; //表示设置定时器失败
	}
#endif




	/////////////////////////////////
	//创建遥控线程和遥测线程
	////////////////////////////////

#ifdef _STARTYKTHREAD
    // 创建遥控线程//////////////////
	ret = pthread_create(&gTidYK, NULL, main_loop_of_YK, NULL);

    if ( 0 != ret )
    {
        errorPrint(LOGFILE,"ERR---Can't create YK thread:%s.\n", "错误---无法创建YK线程（错误编号：%s）.\n", strerror(ret));
        quit(RET_ERR_START_YK_THREAD);
        return RET_ERR_START_YK_THREAD;//表示启动遥控线程失败
    }
    msgPrint(LOGFILE,"MSG---Create YK thread success.\n", "消息---创建YK线程成功.\n");
#endif


#ifdef _STARTYCTHREAD

    // 创建遥测线程///////////////////
	ret = pthread_create(&gTidYC, NULL, main_loop_of_YC, NULL);
    if ( 0 != ret )
    {
    	errorPrint(LOGFILE,"ERR---Can't create YC thread:%s.\n", "错误---无法创建YC线程（错误编号：%s）.\n", strerror(ret));
        quit(RET_ERR_START_YC_THREAD);
        return RET_ERR_START_YC_THREAD;//表示启动遥测线程失败
    }
    msgPrint(LOGFILE,"MSG---Create YC thread success.\n", "消息---创建YC线程成功.\n");
#endif


#ifdef _IGNORUNREADZLWHENRESTART

    //////////////////////////
    //开始处理前，清除所有未处理的指令
    /////////////////////////
    ignoreUnreadZLWhenStart();
#endif


#ifdef  _TRUNCATE_SSYCSJ_TABLE_WHENRESTART
    /////////////////////////
    //delete SSYCSJ when started
    /////////////////////////
    deleteSSYCSJWhenStart();
#endif

    //DEBUG_STOP
    //////////////////////////
    //main主循环
    //////////////////////////
    msgPrint(LOGFILE,"MSG---Began main loop of main thread.\n", "消息---进入主线程循环.\n");
    main_loop_of_main_thread();



    //////////////////////////////////////////////////////
    //等待YC线程和YK线程返回，并输出返回值
    //////////////////////////////////////////////////////
    msgPrint(LOGFILE,"MSG---Ended main loop. Waiting for subThreads return.\n","消息---主线程循环结束，等待子线程返回.\n");

#ifdef _STARTYCTHREAD

	int *retThreadYC;
    pthread_join(gTidYC,(void**)(&retThreadYC));
    msgPrint(LOGFILE,"MSG---YCThread return %d.\n","消息---子线程YCThread返回（返回值：%d）.\n", *retThreadYC);
    free(retThreadYC);

#endif

#ifdef _STARTYKTHREAD

	int *retThreadYK;
    pthread_join(gTidYK,(void**)(&retThreadYK));
    msgPrint(LOGFILE,"MSG---YKThread return %d.\n","消息---子线程YKThread返回（返回值：%d）.\n", *retThreadYK);
    free(retThreadYK);

#endif


    /////////////////////////////
    //退出前执行必要的操作
    /////////////////////////////
    quit(RET_CLOSING); //退出前置为正在关闭，表示应用走完了正常关闭流程，最后是否关闭成功由监控管理判断。



    ///////////////////////
    //退出进程
    ///////////////////////
  	msgPrint(LOGFILE,"MSG---YCYK: program is end. Bye.\n","消息---YCYK: 程序结束.\n");
  	//return EXIT_SUCCESS;
  	exit(0);

}


/*
 * 功能：应用启动时，将指令表中所有给自己的未读指令置为抛弃,发给广播地址的指令不会被抛弃
 * 参数：
 * 无
 * 返回值：
 * @-1:失败；
 * @1:成功；
 */
int ignoreUnreadZLWhenStart(void){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: ignoreUnreadZLWhenStart is called.\n","调用+++YKYC.cpp的函数: ignoreUnreadZLWhenStart.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	string strUpdate_ZL_ZXJG_WhenStart = "update " +
			string(table_name_YK_ZL) +
			" set ZL_ZXJG = "+
			int2String(ZXJG_PQ)+
			" where (YY_ID = "+
			int2String(YYID_CKFW)+
			" or YY_ID = "+
			int2String(YYID_GBDZ)+
			") and ZL_ZXJG = "+
			int2String(ZXJG_WD)+
			";";


	sqlPrint(LOGFILE,"SQL---Update unread zl of YKYC to ignored: %s.\n","SQL---更新指令表未读指令为忽略SQL: %s.\n",strUpdate_ZL_ZXJG_WhenStart.c_str());


	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1)
		return -1;

	int ret = self_mysql_query(selfMysqlp, strUpdate_ZL_ZXJG_WhenStart.c_str());

    if (!ret) {
         prgPrint(LOGFILE,"PRG---Update unread zl of YKYC to ignored, affact %d rows.\n","过程---更新指令表未读指令为忽略 ，影响%d行.\n",
                 self_mysql_affected_rows(selfMysqlp));
     } else {
         errorPrint(LOGFILE, "ERR---Update unread zl of YKYC to ignored error %d: %s.\n","错误---更新指令表未读指令为忽略（错误编号：%d，%s）.\n",
        		 self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return -1;
     }

    msgPrint(LOGFILE,"MSG---ignoreUnreadZLWhenStart success.\n","消息---忽略指令表未处理指令成功.\n");
	return 1;
}

/*
 * 功能：应用启动时，将指令表中SSYCSJ抛弃
 * 参数：
 * 无
 * 返回值：
 * @-1:失败；
 * @1:成功；
 */
int deleteSSYCSJWhenStart(void){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: deleteSSYCSJWhenStart is called.\n","调用+++YKYC.cpp的函数: deleteSSYCSJWhenStart.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	string strDelete_GJFW_YC_SSYCSJ_WhenStart = "delete from GJFW_YC_SSYCSJ where 1=1; ";


	sqlPrint(LOGFILE,"SQL---Delete from GJFW_YC_SSYCSJ: %s.\n","SQL---更新指令表未读指令为忽略SQL: %s.\n",strDelete_GJFW_YC_SSYCSJ_WhenStart.c_str());


	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1)
		return -1;

	int ret = self_mysql_query(selfMysqlp, strDelete_GJFW_YC_SSYCSJ_WhenStart.c_str());

    if (!ret) {
         prgPrint(LOGFILE,"PRG--Delete from GJFW_YC_SSYCSJ, affact %d rows.\n","过程---更新指令表未读指令为忽略 ，影响%d行.\n",
                 self_mysql_affected_rows(selfMysqlp));
     } else {
         errorPrint(LOGFILE, "ERR---Delete from GJFW_YC_SSYCSJ error %d: %s.\n","错误---更新指令表未读指令为忽略（错误编号：%d，%s）.\n",
        		 self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return -1;
     }

    msgPrint(LOGFILE,"MSG---Delete from GJFW_YC_SSYCSJ when started success.\n","消息---忽略指令表未处理指令成功.\n");
	return 1;
}


/*
 * 功能：处理一个未读的指令
 * 参数：
 * 无
 * 返回值：
 * @0:查询不到未读指令；
 * @-1:数据库未连接，或者查询失败；
 * @1:查询成功；
 */
int dealWithAnUnreadZl(void){
	GET_FUNCSEQ
	//fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: dealWithAnUnreadZl is called.\n"，"调用+++YKYC.cpp的函数: dealWithAnUnreadZl.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1){
		errorPrint(LOGFILE,"ERR---Can not deal With unread Zl，fail to connect DB.\n","错误---无法处理指令，无法链接到数据库.\n");
		return -1;
	}

	string tablename = table_name_YK_ZL;
	string sqlSelectZL = "select ZL_ID,YY_ID,ZL_LX,ZL_BH,ZL_NR from "+
			tablename +
			" where (YY_ID = "+
			int2String(YYID_CKFW)+
			" or YY_ID = "+
			int2String(YYID_GBDZ)+
			//" or YY_ID = "+
			//int2String(YYID_JKGL)+
			") and ZL_ZXJG = " +
			int2String(ZXJG_WD)+
			" order by ZL_ID"
			" ;";

	sqlPrint(LOGFILE,"SQL---Select from %s: %s .\n","SQL---查询%s表SQL: %s .\n",table_name_YK_ZL,sqlSelectZL.c_str());

	int ret = self_mysql_query(selfMysqlp, sqlSelectZL.c_str());
    if (!ret) {

         //msgPrint(LOGFILE,"MSG---Select from %s, affact %d rows.\n", table_name_YK_ZL,
        //		 self_mysql_affected_rows(selfMysqlp));

         MYSQL_RES *mysql_result = self_mysql_store_result(selfMysqlp);

         //prgPrint(LOGFILE,"PRG---Run to here???????????(add this so it can go on).\n");

         int num_row = 0;
         //如果取到结果集就取行数
         if (NULL != mysql_result){
             num_row = self_mysql_num_rows(selfMysqlp, mysql_result);
             tmpPrint(LOGFILE,"TMP---Select from %s %d rows.\n","临时---Select from %s %d rows.\n", table_name_YK_ZL,num_row);
         }

         //如果行数为0就释放记录并退出
         if(0 == num_row){
             tmpPrint(LOGFILE,"TMP---There is no unread ZL.\n","临时---没有未处理指令.\n");
             self_mysql_free_result(selfMysqlp, mysql_result);
        	 return 0;
         }



         int intZL_ID,intYY_ID,intZL_LX,intZL_BH;
         string strZL_NR = "";
         unsigned char ucharZL_NR[ZL_MAX_LENGTH];
         bzero(ucharZL_NR,ZL_MAX_LENGTH);

         //读取第一条指令
         MYSQL_ROW mysql_row=self_mysql_fetch_row(selfMysqlp, mysql_result);

         prgPrint(LOGFILE,"PRG---Fetch a row of unread ZL.\n","过程---取到一条未处理指令\n.");

         if (mysql_row[0] != NULL)
        	 intZL_ID = atoi(mysql_row[0]);
         if (mysql_row[1] != NULL)
        	 intYY_ID = atoi(mysql_row[1]);
         if (mysql_row[2] != NULL)
        	 intZL_LX = atoi(mysql_row[2]);
         if (mysql_row[3] != NULL)
        	 intZL_BH = atoi(mysql_row[3]);


         if (mysql_row[4] != NULL){
        	 strZL_NR = mysql_row[4];

             int i;
             for (i=0;i<507;i++)
            	 ucharZL_NR[i] = mysql_row[4][i];
             //用strZL_NR.length();取指令内容长度是不正确的
             /*
             //将指令内容由字符串转换为unsigned char数组
             int lengthOfstrZL_NR = strZL_NR.length();
             int i;
             for (i=0;i<lengthOfstrZL_NR;i++){
            	 ucharZL_NR[i] =  strZL_NR.c_str()[i];
             }//for
             */
         }//if
         else{
    		 //反馈执行结果为指令格式错误
             return_ZL_ZXJG(intZL_ID,ZXJG_GSCW);
             msgPrint(LOGFILE,"MSG---error! get a ZL with null zlnr.\n","消息---指令错误! 指令内容为空.\n");

         }

         self_mysql_free_result(selfMysqlp, mysql_result);

         //Debug输出指令参数和内容
         tmpPrint(LOGFILE,"TMP---YYID_CKFW = %d.\n","临时---YYID_CKFW = %d.\n",YYID_CKFW);
         tmpPrint(LOGFILE,"TMP---YYID_GBDZ = %d.\n","临时---YYID_GBDZ = %d.\n",YYID_GBDZ);
         dataPrint(LOGFILE,"DAT---intZL_ID = %d, intYY_ID = %d, intZL_LX = %d, intZL_BH = %d .\n","数据---intZL_ID = %d, intYY_ID = %d, intZL_LX = %d, intZL_BH = %d .\n",intZL_ID,intYY_ID,intZL_LX,intZL_BH);
         dataPrint(LOGFILE,"DAT---ZL_NR in string is %s.\n","数据---ZL_NR（字符串格式）为：%s.\n",strZL_NR.c_str());

         /////////////////////////////
         //对收到的报文进行处理self_mysql_free_result(mysql_result)
         /////////////////////////////


         //如果收到的是到YYID_CKFW的定向指令
         if (intYY_ID ==  YYID_CKFW){
        	 tmpPrint(LOGFILE,"TMP---Get a ZL for YYID_CKFW.\n","临时---取到一条给测控服务的指令.\n");
        	 //判指令类型
        	 switch (intZL_LX){

        	 case ZLLX_JKGL://如果是管理指令
        		 //进一步判指令编号
            	 switch (intZL_BH){

            	 case ZLBH_JKGL_YYSJQL: //应用数据清理
            		 onZL_JKGL_YYSJQL(intZL_ID,ucharZL_NR);
            		 break;
            	 case ZLBH_JKGL_ZRSJSYTZ: //注入数据使用通知
            		 onZL_JKGL_ZRSJSYTZ(intZL_ID,ucharZL_NR);
            		 break;
            	 default:
            		 //反馈执行结果为指令类编号超出正确范围
             		 return_ZL_ZXJG(intZL_ID,ZXJG_ZLCCCLFW);
            		 msgPrint(LOGFILE,"MSG---error! Get an unknown ZLLX_JKGL for YYID_CKFW with error ZL_BH: %d.","消息---指令错误! 管理指令的编号超出正确范围: %d.",intZL_BH);
            		 break;
            	 }

            	 break;

        	 case ZLLX_KZZL://如果是扩展指令
        		 //进一步判指令编号
            	 switch (intZL_BH){

            	 case ZLBH_KZZL_PZ: //配置指令
            		 onZL_KZZL_PZ(intZL_ID,ucharZL_NR);
            		 break;
            	 default:
            		 //反馈执行结果为指令类编号超出正确范围
            		 return_ZL_ZXJG(intZL_ID,ZXJG_ZLCCCLFW);
            		 msgPrint(LOGFILE,"MSG---error! Get an unknown ZLLX_KZZL for YYID_CKFW with error ZL_BH: %d.","消息---指令错误! 扩展指令的编号超出正确范围: %d.",intZL_BH);
             		 break;
            	 }

            	 break;

            	 //add heart beat begin///////////////////////////////////
            	 case ZLLX_XT://如果heart beat指令
            		 //进一步判指令编号
                	 switch (intZL_BH){

                	 case ZLBH_XT_XT: //配置指令
                		 onZL_XT(intZL_ID,ucharZL_NR);
                		 break;
                	 default:
                		 //反馈执行结果为指令类编号超出正确范围
                		 return_ZL_ZXJG(intZL_ID,ZXJG_ZLCCCLFW);
                		 msgPrint(LOGFILE,"MSG---error! Get an unknown ZLLX_XT for YYID_CKFW with error ZL_BH: %d.","消息---指令错误! heart beat指令的编号超出正确范围: %d.",intZL_BH);
                 		 break;
                	 }

                	 break;
                	 //add heart beat end///////////////////////////////////

        	 default:
        		 //反馈执行结果为指令类型超出正确范围
        		 return_ZL_ZXJG(intZL_ID,ZXJG_ZLCCCLFW);
         		 msgPrint(LOGFILE,"MSG---error! Get an unknown ZL for YYID_CKFW with error ZL_LX: %d.","消息---指令错误! 收到给测控服务的指令，类型超出正确范围: %d.",intZL_LX);
              	 break;
        	 }
        	 //反馈执行结果为

         }else  if(intYY_ID == YYID_GBDZ) { //收到的是到广播地址的指令
        	 tmpPrint(LOGFILE,"TMP---Get a ZL for YYID_GBDZ.\n","临时---取到一条广播指令.\n");
        	 if (intZL_LX == ZLLX_JKGL and intZL_BH == ZLBH_JKGL_JDYGJ){
        		 onZL_JKGL_JDYGJ(intZL_ID,ucharZL_NR);
        	 }else{
           		 //反馈执行结果为指令类型超出正确范围
           		 return_ZL_ZXJG(intZL_ID,ZXJG_ZLCCCLFW);
           		 msgPrint(LOGFILE,"MSG---error! Get an unknown ZL for YYID_GBDZ with error ZL_LX: %d and ZL_BH: %d.","消息---指令错误! 收到广播指令，类型超出正确范围: ZL_LX: %d，ZL_BH: %d.",intZL_LX,intZL_BH);
        	 }//else
         }else{
    		 //反馈执行结果为指令地址YYID超出处理范围
             return_ZL_ZXJG(intZL_ID,ZXJG_ZLCCCLFW);
             msgPrint(LOGFILE,"MSG---error! Get an Unknown ZL with YYID: %d.\n","消息---指令错误! 应用ID错误: %d.\n",intYY_ID);
         }



     } else {
         errorPrint(LOGFILE, "ERR---Select from %s error %d: %s.\n", "错误---查询%s表错误（错误编号：%d，%s）.\n",
        		 table_name_YK_ZL,
        		 self_mysql_errno(selfMysqlp),
        		 self_mysql_error(selfMysqlp));
         return -1;
     }

    msgPrint(LOGFILE,"MSG---dealWithAnUnreadZl success.\n","消息---未读指令处理成功.\n");
	return 1;
}

/*
 * 功能：向指令表反馈指令执行结果
 * 参数：
 * @int zl_id：指令ID；
 * @int zl_zxjg：指令执行结果；
 * 返回值：
 * @1:操作成功；
 * @-1:操作失败，或无法取得数据库链接；
 */
int return_ZL_ZXJG(int zl_id,int zl_zxjg){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: return_ZL_ZXJG is called.\n","调用+++YKYC.cpp的函数: return_ZL_ZXJG.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	string strUpdate_ZL_ZXJG = "update " +
			string(table_name_YK_ZL) +
			" set ZL_ZXJG = "+
			int2String(zl_zxjg)+
			" where ZL_ID = "+
			int2String(zl_id)+
			";";


	sqlPrint(LOGFILE,"SQL---Update ZL_ZXJG: %s.\n","SQL---更新指令执行结果SQL: %s.\n",strUpdate_ZL_ZXJG.c_str());


	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1)
		return -1;

	int ret = self_mysql_query(selfMysqlp, strUpdate_ZL_ZXJG.c_str());

    if (!ret) {
         prgPrint(LOGFILE,"PRG---Update ZL_ZXJG, affact %d rows.\n","过程---更新指令执行结果 ，影响%d行.\n",
                 self_mysql_affected_rows(selfMysqlp));
     } else {
         errorPrint(LOGFILE, "ERR---Update ZL_ZXJG error %d: %s.\n", "错误---更新指令执行结果失败（错误编号：%d，%s）.\n", self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return -1;
     }

    msgPrint(LOGFILE,"MSG--return_ZL_ZXJG success.\n","消息---反馈指令执行结果成功.\n");
	return 1;
}

/*
 * 功能：处理节点预关机指令
 * 参数：
 * @int zl_id：指令ID；（未使用）
 * @unsigned char* ucharZL_NR：指令内容；（未使用）
 * 返回值：
 * @1:成功；（没有别的返回值）
 */
int onZL_JKGL_JDYGJ(int zl_id,unsigned char* ucharZL_NR){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: onZL_JKGL_JDYGJ is called.\n","调用+++YKYC.cpp的函数: onZL_JKGL_JDYGJ.\n");



	//广播指令不返回执行结果
	//return_ZL_ZXJG(zl_id,ZXJG_JS);
	gIntIsRun = 0;
	//return_ZL_ZXJG(zl_id,ZXJG_CG);
	msgPrint(LOGFILE,"MSG---onZL_JKGL_JDYGJ success.\n","消息---处理节点预关机指令成功.\n");
	return 1;
}


/*
 * 功能：处理注入数据使用指令 目前什么也不做
 * 参数：
 * @int zl_id：指令ID；（未使用）
 * @unsigned char* ucharZL_NR：指令内容；（未使用）
 * 返回值：
 * @1:成功；（没有别的返回值）
 */
int onZL_JKGL_ZRSJSYTZ(int zl_id,unsigned char* ucharZL_NR){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: onZL_JKGL_ZRSJSYTZ is called.\n","调用+++YKYC.cpp的函数: onZL_JKGL_ZRSJSYTZ.\n");



	//return_ZL_ZXJG(zl_id,ZXJG_JS);
	return_ZL_ZXJG(zl_id,ZXJG_CG);
	msgPrint(LOGFILE,"MSG---onZL_JKGL_ZRSJSYTZ success.\n","消息---处理注入数据使用指令成功.\n");
	return 1;
}


/*
 * 功能：处理应用数据清理指令
 * 参数：
 * @int zl_id：指令ID；（未使用）
 * @unsigned char* ucharZL_NR：指令内容；（未使用）
 * 返回值：
 * @-1：数据库未连接
 * @-2：清理遥控指令表操作失败；
 * @-3：清理延时遥测数据表操作失败；
 * @-4：清理实时遥测历史数据表操作失败；
 * @-5：清理实时遥测数据表操作失败；
 * @-6：清理延时遥测配置项表操作失败；
 * @1:成功；
 */
int onZL_JKGL_YYSJQL(int zl_id,unsigned char* ucharZL_NR){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: onZL_JKGL_YYSJQL is called.\n","调用+++YKYC.cpp的函数: onZL_JKGL_YYSJQL.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}


	return_ZL_ZXJG(zl_id,ZXJG_JS);


	//////////////////////////////////////////////////////////////
	//如果数据库未连接就返回
	//////////////////////////////////////////////////////////////
	if(gIntIsDbConnected!=1){
		errorPrint(LOGFILE,"ERR---Deal with a ZL YYSJQL fail, can't connect to DB.\n","错误---处理应用数据清理指令失败，无法链接到数据库.\n");
        return_ZL_ZXJG(zl_id,ZXJG_SB);
		return -1;
	}

	//////////////////////////////////////////////////////////////
	//取出清理标志
	//////////////////////////////////////////////////////////////
	__uint8_t uintQlBz = ucharZL_NR[0];

	//todo
	//根据清理标志，做不同深度的清理

	//////////////////////////////////////////////////////////////
	//清理应用GJFW_YK_ZL表，如果有多个需要依次执行（注意外键约束顺序）
	//////////////////////////////////////////////////////////////

	string strTruncateZL = "truncate table " +
			string(table_name_YK_ZL) +
			";";

	sqlPrint(LOGFILE,"SQL---Truncate table %s: %s .\n","SQL---清除%s表SQL: %s .\n",table_name_YK_ZL,strTruncateZL.c_str());

	int ret = self_mysql_query(selfMysqlp, strTruncateZL.c_str());

    if (!ret) {
         prgPrint(LOGFILE,"PRG---Truncate table %s, affact %d rows.\n","过程---清除%s表 ，影响%d行.\n",
        		 table_name_YK_ZL,
                 self_mysql_affected_rows(selfMysqlp));
     } else {
         errorPrint(LOGFILE, "ERR---Truncate table %s error %d: %s.\n", "错误---清除%s表失败（错误编号：%d，%s）.\n",
        		 table_name_YK_ZL, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return_ZL_ZXJG(zl_id,ZXJG_SB);
         return -2;
     }

	//////////////////////////////////////////////////////////////
	//清理应用GJFW_YC_YSYCSJ表，注意外键约束顺序，在删除延时遥测配置项之前执行
	//////////////////////////////////////////////////////////////
	string strTruncateYSYCSJ = "truncate table " +
			string(table_name_YC_YSYCSJ) +
			";";

	sqlPrint(LOGFILE,"SQL---Truncate table %s: %s .\n","SQL---清除%s表SQL: %s.\n",table_name_YC_YSYCSJ,strTruncateYSYCSJ.c_str());

	ret = self_mysql_query(selfMysqlp, strTruncateYSYCSJ.c_str());

    if (!ret) {
    	prgPrint(LOGFILE,"PRG---Truncate table %s, affact %d rows.\n","过程---清除%s表 ，影响%d行.\n",
        		 table_name_YC_YSYCSJ,
                 self_mysql_affected_rows(selfMysqlp));
     } else {
         errorPrint(LOGFILE, "ERR---Truncate table %s error %d: %s.\n",  "错误---清除%s表失败（错误编号：%d，%s）.\n",
        		 table_name_YC_YSYCSJ, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return_ZL_ZXJG(zl_id,ZXJG_SB);
         return -3;
     }


	//////////////////////////////////////////////////////////////
	//清理应用GJFW_YC_SSYCLSSJ表
	//////////////////////////////////////////////////////////////
	string strTruncateSSYCLSSJ = "truncate table " +
			string(table_name_YC_SSYCLSSJ) +
			";";

	sqlPrint(LOGFILE,"SQL---Truncate table %s: %s .\n","SQL---清除%s表SQL: %s.\n",table_name_YC_SSYCLSSJ,strTruncateSSYCLSSJ.c_str());

	ret = self_mysql_query(selfMysqlp, strTruncateSSYCLSSJ.c_str());

    if (!ret) {
    	prgPrint(LOGFILE,"PRG---Truncate table %s, affact %d rows.\n","过程---清除%s表 ，影响%d行.\n",
        		 table_name_YC_SSYCLSSJ,
                 self_mysql_affected_rows(selfMysqlp));
     } else {
         errorPrint(LOGFILE, "ERR---Truncate table %s error %d: %s.\n",  "错误---清除%s表失败（错误编号：%d，%s）.\n",
        		 table_name_YC_SSYCLSSJ, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return_ZL_ZXJG(zl_id,ZXJG_SB);
         return -4;
     }

	//////////////////////////////////////////////////////////////
	//清理应用GJFW_YC_SSYCSJ表
	//////////////////////////////////////////////////////////////
	string strTruncateSSYCSJ = "truncate table " +
			string(table_name_YC_SSYCSJ) +
			";";

	sqlPrint(LOGFILE,"SQL---Truncate table %s: %s .\n","SQL---清除%s表SQL: %s.\n",table_name_YC_SSYCSJ,strTruncateSSYCSJ.c_str());

	ret = self_mysql_query(selfMysqlp, strTruncateSSYCSJ.c_str());

    if (!ret) {
    	prgPrint(LOGFILE,"PRG---Truncate table %s, affact %d rows.\n","过程---清除%s表 ，影响%d行.\n",
        		 table_name_YC_SSYCSJ,
                 self_mysql_affected_rows(selfMysqlp));
     } else {
         errorPrint(LOGFILE, "ERR---Truncate table %s error %d: %s.\n",  "错误---清除%s表失败（错误编号：%d，%s）.\n",
        		 table_name_YC_SSYCSJ, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return_ZL_ZXJG(zl_id,ZXJG_SB);
         return -5;
     }

	//////////////////////////////////////////////////////////////
	//清理应用GJFW_YC_YSYCPZX表，删除指令配置的延时遥测配置项
	//////////////////////////////////////////////////////////////
	string strDeleteYSYCPZX = "delete from " +
			string(table_name_YC_YSYCPZX) +
			"where YSYCPZX_LY = 2"
			";";

	sqlPrint(LOGFILE,"SQL---Truncate table %s: %s .\n","SQL---清除%s表SQL: %s.\n",table_name_YC_YSYCPZX,strDeleteYSYCPZX.c_str());

	ret = self_mysql_query(selfMysqlp, strDeleteYSYCPZX.c_str());

    if (!ret) {
    	prgPrint(LOGFILE,"PRG---Truncate table %s, affact %d rows.\n","过程---清除%s表 ，影响%d行.\n",
        		 table_name_YC_YSYCPZX,
                 self_mysql_affected_rows(selfMysqlp));
     } else {
         errorPrint(LOGFILE, "ERR---Truncate table %s error %d: %s.\n",  "错误---清除%s表失败（错误编号：%d，%s）.\n",
        		 table_name_YC_YSYCPZX, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return_ZL_ZXJG(zl_id,ZXJG_SB);
         return -6;
     }


	return_ZL_ZXJG(zl_id,ZXJG_CG);

	msgPrint(LOGFILE,"MSG---onZL_JKGL_YYSJQL success.\n","消息---处理应用数据清理指令成功.\n");
	return 1;
}


/*
 * 功能：处理配置指令
 * 参数：
 * @int zl_id：指令ID；（未使用）
 * @unsigned char* ucharZL_NR：指令内容；
 * 返回值：
 * @-1：数据库未连接，或操作失败；
 * @1:成功；
 */
int onZL_KZZL_PZ(int zl_id,unsigned char* ucharZL_NR){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: onZL_KZZL_PZ is called.\n","调用+++YKYC.cpp的函数: onZL_KZZL_PZ.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}


	return_ZL_ZXJG(zl_id,ZXJG_JS);

	char* charTmp = (char*)ucharZL_NR;

	sqlPrint(LOGFILE,"SQL---Execute expended sql: %s.\n","SQL---扩展指令SQL: %s.\n",charTmp);


	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1)
		return -1;

	//20180110 add
	return_ZL_ZXJG(zl_id,ZXJG_JS);

	int ret = self_mysql_query(selfMysqlp, charTmp);

    if (!ret) {
         prgPrint(LOGFILE,"PRG---Execute expended sql, affact %d rows.\n", "过程---执行扩展指令 ，影响%d行.\n",
                 self_mysql_affected_rows(selfMysqlp));
     } else {
         errorPrint(LOGFILE, "ERR---Execute expended sql error %d: %s.\n", "错误---执行扩展指令失败（错误编号：%d，%s）.\n", self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
         return_ZL_ZXJG(zl_id,ZXJG_SB);
         return -1;
     }
	return_ZL_ZXJG(zl_id,ZXJG_CG);
	msgPrint(LOGFILE,"MSG---onZL_KZZL_PZ success.\n","消息---处理扩展指令成功.\n");

	return 1;
}

#ifndef _RUN_ON_XNWX
/*
 * 功能：读取socket写入指令缓冲
 * 参数：
 * 无
 * 返回值：
 * @-1: 取得数据库链接失败
 * @0：未读到数据；
 * @1:成功；
 */
int readSocket(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: readSocket is called.\n","调用+++YKYC.cpp的函数: readSocket.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	//prgPrint(LOGFILE,"PRG---It is time to recv from zl server.\n");

	if (gIntIsSocketConnected == 1){


		bzero(socketReadBuffer, SOCKET_READ_LENGTH);
		int length;
		length = recv(gNetSocket.client_sockfd,socketReadBuffer,SOCKET_READ_LENGTH,MSG_DONTWAIT);
	    dataPrint(LOGFILE,"DAT---Main loop recv %d byte zl data from socket：.\n", "数据---从服务器接收到%d byte指令数据.\n", length);



	    //输出收到的socket数据

	    int i;
	    for (i=0;i<length;i++){
	    	debugPrint_YKPackage(LOGFILE,"%02X ",socketReadBuffer[i]);
	    }
	    debugPrint_YKPackage(LOGFILE,"\n");

	    if (length > 0){

        	 //dataPrint(LOGFILE,"^^^^^^^^^^^^^^^^^Before push^^^^^^^^^^^^^^^^^^^.\n");
        	 //printBuffer(&gSocketBuffer);
        	 pushBuffer(&gSocketBuffer,socketReadBuffer,length);
        	 //dataPrint(LOGFILE,"#################After push#####################.\n");
	    	 //printBuffer(&gSocketBuffer);

        	 ////////////////////////////////////////////
        	 //字节统计
        	 ////////////////////////////////////////////
        	 gTotal.sizeOfPushBuffer +=  length;
        	 gTotal.sizeRecvFromSocket += length;
        	 //normalPrint(LOGFILE,"**************************************************.\n");
        	 //msgPrint(LOGFILE,"MSG---Main loop recv %d bytes (total %lu bytes) data from socket.\n",length,gTotal.sizeRecvFromSocket);

	    }
	    else
	    	return 0;
	}//if
	//tmpPrint(LOGFILE,"TMP---readSocket success.\n","消息---从服务器读取指令数据成功.\n");


	return 1;
}

#endif

/*
 * 功能：主线程循环，处理发给本应用的指令，在socket进行收发
 * 参数：
 * 无
 * 返回值：
 * 无
 */
void main_loop_of_main_thread(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: main_loop_of_main_thread is called.\n","调用+++YKYC.cpp的函数: main_loop_of_main_thread.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return ;
	}

	 while(gIntIsRun){

		 //printBuffer(&gSocketBuffer);


#ifndef _LOOP
    	//调试，如果没有链接到服务器则暂不循环
    	break;
#else
    	sleep(SLEEP_YCYK_LOOP);

#endif


	 }//while

}
#ifdef _RUN_ON_XNWX
/*
 * 功能：发送需要周期发送的实时遥测数据到中央数据库
 * 参数：
 * 无
 * 返回值
 *
 */
void sendZqSsycOnXNWX(void){
	//todo

}

#else
 /*
  * 功能：发送需要周期发送的实时遥测数据到集成单元
  * 参数：
  * 无
  * 返回值
  *
  */
void sendZqSsycOnSocket(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: sendZqSsyc is called.\n","调用+++YKYC.cpp的函数: sendZqSsyc.\n");

	////////////////////////////
	//计算使用哪个实时遥测数据结构
	////////////////////////////
	int indexToSendOnSocket = (gSsycStruct.intDataCJUsing+1)%2;
	//zqssyc_t *pSsycData = gSsycStruct.ssycData[indexToSendOnSocket];

	//int ycDataLength;
	//char * socketWriteBuffer;

#ifdef _USE_SSYC_IN_STRUCT

		int ycDataLength = sizeof(zqssyc_t);
		char * socketWriteBuffer = (char *)(&(gSsycStruct.zqssycData[indexToSendOnSocket]));

#else
		zqssyc_t *pData = &(gSsycStruct.zqssycData[indexToSendOnSocket]);
		char charTmp[256];
		sprintf(charTmp,"dqSjMs=%lu, zlCount=%d, zlLx=%d, zlBh=%d, zlYyId=%d, zlZxjg=%d, cpu=%f, kyKj=%lu\n",pData->dqSjMs,pData->zlCount,pData->zlLx,pData->zlBh,pData->zlYyId,pData->zlZxjg,pData->cpu,pData->kyKj);
		//socketWriteBuffer = charTmp;
		int intTmp =strlen(charTmp);
		//printf(charTmp);
		//printf("\nlength=%d\n",intTmp);

		int ycDataLength = sizeof(ssyc_head_t) + intTmp;
		zqssyc_string_t zqstr;

		zqstr.ssyc_head.header[0] = SSYC_SYNC_0;
		zqstr.ssyc_head.header[1] = SSYC_SYNC_1;
		zqstr.ssyc_head.yyId = YYID_CKFW;
		zqstr.ssyc_head.length = intTmp;
		zqstr.ssyc_head.gs = 0;
		for(int ii=0;ii<intTmp;ii++){
			zqstr.strZqYc[ii] = charTmp[ii];
		}
		char *socketWriteBuffer  = (char *)(&zqstr);

#endif


	if (gIntIsSocketConnected == 1){
		//prgPrint(LOGFILE,"PRG---Going to send gSsycStruct to zl server.\n");

		int length = send(gNetSocket.client_sockfd,socketWriteBuffer,ycDataLength,MSG_DONTWAIT);



	    dataPrint(LOGFILE,"DAT---Main loop send %d byte gSsycStruct data to socket:\n","数据---向服务器发送%d byte周期实时遥测数据：\n",length);

	    //打印发送的socket数据///////////////////////
	    int i;
	    for (i=0;i<length;i++){
	    	debugPrint_YCPackage(LOGFILE,"%02X ",(unsigned char)socketWriteBuffer[i]);
	    }
	    debugPrint_YCPackage(LOGFILE,"\n");
	    //////////////////////////////////////////

	    //周期遥测计数自增
	    gZqYcCount++;
	}//if

	msgPrint(LOGFILE,"MSG---sendZqSsyc success.\n","消息---发送周期实时遥测数据成功.\n");

}
#endif

void printTJXJ(void){
	normalPrint(LOGFILE,"--------------------------------------------------.\n","--------------------------------------------------.\n");
	normalPrint(LOGFILE,"recv %d bytes, dicard %d bytes for Sync, discard %d bytes for mistake Ins length, resolve %d bytes to %d ZL, discard %d ZL for mistake SBID, Insert to DB %d ZL, Fail to Insert %d ZL, recv heart beat %d, reconnect to zl server %d times; send Zq Yc %d, send Ax Yc %d.\n",
						"收到 %d bytes, 因同步问题丢弃 %d bytes, 因长度错误丢弃 %d bytes, 解析 %d bytes 为 %d 条指令, 因错误设备ID丢弃 %d 条指令, 入库成功 %d 条指令, 失败 %d 条指令, 收到HearBeat %d, reconnect to zl server %d times；发送周期实时遥测%d条，发送按需遥测%d条.\n",
			gTotal.sizeRecvFromSocket,
			gTotal.sizeOfDiscardWhileRelocateSync,
			gTotal.sizeOfDiscardWhileInsLengthError,
			gTotal.sizeOfPackageResolved,
			gTotal.packageResolved,
			gTotal.packageDiscardForSBIDError,
			gTotal.packageInsertedToDB,
			gTotal.packageFailToInsertedToDB,
			gTotal.packageHeartBeat,
			gTotal.countOfReConnZlServer,
			gZqYcCount,
			gAxYcCount
			);

	normalPrint(LOGFILE,"push gSocketBuffer %d bytes, pop gSocketBuffer %d bytes.\n",
				"gSocketBuffer入队 %d bytes, gSocketBuffer出队 %d bytes.\n",
				gTotal.sizeOfPushBuffer,
				gTotal.sizeOfPopBuffer);
#ifndef _RUN_ON_XNWX
	normalPrint(LOGFILE,"gSocketBuffer remain %d bytes.\n",
				"gSocketBuffer剩余 %d bytes.\n",
				lengthOfBuffer(&gSocketBuffer));
#endif
	normalPrint(LOGFILE,"pthreadId: YKYC = %lu, YK = %lu, YC = %lu.\n",
				"线程ID: YKYC = %lu, YK = %lu, YC = %lu.\n",
				gTotal.YkycPt,gTotal.YkPt,gTotal.YcPt);
	normalPrint(LOGFILE,"--------------------------------------------------\n\n\n","--------------------------------------------------\n\n\n");


}


/*
 * 功能：处理heart beat指令
 * 参数：
 * @int zl_id：指令ID；（未使用）
 * @unsigned char* ucharZL_NR：指令内容；
 * 返回值：
 * @-1：数据库未连接，或操作失败；
 * @1:成功；
 */
int onZL_XT(int zl_id,unsigned char* ucharZL_NR){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: onZL_XT is called.\n","调用+++YKYC.cpp的函数: onZL_XT.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	return_ZL_ZXJG(zl_id,ZXJG_JS);
	return_ZL_ZXJG(zl_id,ZXJG_CG);

	return 1;
}

#ifndef _RUN_ON_XNWX
void zlReconnect(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: zlReconnect is called.\n","调用+++YKYC.cpp的函数: zlReconnect.\n");
	////////////////////////////////
	//链接服务器，如果失败就返回-2
	///////////////////////////////
#ifdef	_CONNECT_TO_SERVER
	//断开链接
	closeSocket();

	if (0 >= connectSocket()){
		errorPrint(LOGFILE,"ERR---Can't connect ZL server:%d.\n","错误---链接服务器失败（错误编号：%d）.\n", errno);
		//return -2;//-2表示无法链接到指令转发服务
	}
	gTotal.countOfReConnZlServer++;


#endif

}
#endif

/*
 * 功能：读取指令
 * 参数：
 * 无
 * 返回值：
 * @-1: ，不具备条件失败
 * @0：未读到数据；
 * @1:成功；
 */
int recvZl(void){
#ifndef _RUN_ON_XNWX
	return(readZlfromSocket());
#else
	readZlfromCenterDb();
	updateZlZttoCenterDb();
	return 1;
#endif
}

/*
 * 功能： 从中央数据库读取指令
 * 参数：
 * 无
 * 返回值：
 * @0:未读到数据；
 * @-1:取得本线程对应的数据库链接失败
 * @-2:本地数据库未链接
 * @-3:中央数据库未链接
 * @-4:查询到的中央数据库指令格式错误（指令内容部分为空）
 * @-5:查询刚插入的本地库指令表记录的ZL_ID失败
 * @-6:更新中央数据库zl_zt,zl_id失败
 * @-7:插入本地库指令表失败
 * @-8:查询中央数据库错误
 * @1:成功；
 */
int readZlfromCenterDb(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: readZlfromCenterDb is called.\n","调用+++YKYC.cpp的函数: readZlfromCenterDb.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	//取得YK所用的中央数据库链接
	mysql_t * centerMysqlp = &gMysql_centerDBforYK;


    //__uint8_t yh_bs;
    //__uint8_t wx_bs;

  	//如果本地数据库未连接就返回
  	if(gIntIsDbConnected!=1)
  		return -2;
  	//如果中央数据库未连接就返回
  	if(gIntIsCenterDBConnected!=1)
  		return -3;




    //参考sql
    //select yh_bs,wx_bs,yy_id,zl_lx,zl_bh,zdy_sjy_blob from satellite.all_ins where wx_lb=1 and jd_lb=0x14 and zl_zt=0;
  	string strReadZL = "select yh_bs,wx_bs,yy_id,zl_lx,zl_bh,zdy_sjy_blob,id from satellite.all_ins where wx_lb= "+
  			int2String(WX_LB) +
  			" and zl_zt=0 and jd_lb = " +
  			int2String(DEVICE_ID) +
 			" and wx_bs = " +
  			int2String(WX_ID) +
   			";";


  	sqlPrint(LOGFILE,"SQL---select center db table %s: %s\n","SQL---读取中央数据库%s表SQL: %s\n","satellite.all_ins", strReadZL.c_str());
  	printf("SQL---select center db table %s: %s\n","satellite.all_ins", strReadZL.c_str());

  	///////////////////////////////////////////
  	//读取中央数据库指令表并插入本地指令库表
  	///////////////////////////////////////////


	int ret = self_mysql_query(centerMysqlp, strReadZL.c_str());
    if (!ret) {


         MYSQL_RES *mysql_result = self_mysql_store_result(centerMysqlp);

         int num_row = 0;
         //如果取到结果集就取行数
         if (NULL != mysql_result){
             num_row = self_mysql_num_rows(centerMysqlp, mysql_result);
             tmpPrint(LOGFILE,"TMP---Select from %s %d rows in center db.\n","临时---从中央数据库 Select from %s %d 行.\n", "all_ins",num_row);

         }

         //如果行数为0就释放记录并退出
         if(0 == num_row){
             tmpPrint(LOGFILE,"TMP---There is no unread ZL in center db.\n","临时---中央数据库没有未读指令.\n");
             self_mysql_free_result(centerMysqlp, mysql_result);
        	 return 0;
         }

         int intYY_ID,intZL_LX,intZL_BH;
         string strZL_NR = "";
         unsigned char ucharZL_NR[ZL_MAX_LENGTH];
         bzero(ucharZL_NR,ZL_MAX_LENGTH);
         int intId;//指令在中央数据库中的id
         int intZlNrIsNull;//指示查处的指令内容是否为空

         int intZLShouldBeDiscard; //在关键字段为空，或者因为应用id不对而插入本地库失败等情况下，将此变量置为1，在后续处理中，将中央数据库中的相应指令置为丢弃状态
         //读取第一条指令
         MYSQL_ROW mysql_row;
         while((mysql_row = self_mysql_fetch_row(centerMysqlp,mysql_result))){

        	 intZLShouldBeDiscard = 0;
        	 //2018-05-25
        	 intYY_ID = 0;
        	 intZL_LX = 0;
        	 intZL_BH = 0;
        	 intId = 0;


        	 strZL_NR = "";
        	 bzero(ucharZL_NR,ZL_MAX_LENGTH);

             prgPrint(LOGFILE,"PRG---Fetch a row of unread ZL from Center DB.\n","过程---从中央数据库取到一条未读指令\n.");


             /////////////////////////
             //读取指令
             /////////////////////////

        	 //字段顺序
        	 //select yh_bs,wx_bs,yy_id,zl_lx,zl_bh,zdy_sjy_blob,id from satellite.all_in

             //2018-05-25
             //对每个变量的赋值判断，都增加else intZLShouldBeDiscard = 1的语句，在相应值为空的情况下，作出标记，以便后续在中央数据库中将相应指令置为丢弃

             //更新卫星id和用户id
             if (mysql_row[0] != NULL)
            	 gUserId = atoi(mysql_row[0]);
             //2018-05-25
             else
            	 intZLShouldBeDiscard = 1;

             //if (mysql_row[1] != NULL)
            	// gWxId = atoi(mysql_row[1]);


             if (mysql_row[2] != NULL)
            	 intYY_ID = atoi(mysql_row[2]);
             //2018-05-25
             else
            	 intZLShouldBeDiscard = 1;

             if (mysql_row[3] != NULL)
            	 intZL_LX = atoi(mysql_row[3]);
             //2018-05-25
             else
            	 intZLShouldBeDiscard = 1;

             if (mysql_row[4] != NULL)
            	 intZL_BH = atoi(mysql_row[4]);
             //2018-05-25
             else
            	 intZLShouldBeDiscard = 1;

             if (mysql_row[6] != NULL)
            	 intId = atoi(mysql_row[6]);
             //2018-05-25
             //如果id字段为空（不可能出现）就跳过
             else
            	 continue;

             if (mysql_row[5] != NULL){
            	 strZL_NR = mysql_row[5];
            	 intZlNrIsNull = 0;
                 int i;
                 for (i=0;i<507;i++)
                	 ucharZL_NR[i] = mysql_row[5][i];
             }//if (mysql_row[5] != NULL){
             else{
        		 //指令格式错误，内容为空
            	 //20180424有些指令的内容就是空的。
                 //return -4;
                 //msgPrint(LOGFILE,"MSG---error! get a ZL with null zlnr from center db.\n","消息---指令错误! 从中央数据库读到一条指令，内容为空.\n");
            	 intZlNrIsNull = 1;
             }

             /////////////////////////
             //显示指令
             /////////////////////////

             //Debug输出指令参数和内容
             dataPrint(LOGFILE,"DAT---A ZL from Center db:\n  intYY_ID = %d, intZL_LX = %d, intZL_BH = %d .\n","数据---来自中央数据库的指令：intYY_ID = %d, intZL_LX = %d, intZL_BH = %d .\n",intYY_ID,intZL_LX,intZL_BH);
             dataPrint(LOGFILE,"DAT---ZL_NR in string is %s.\n","数据---ZL_NR（字符串格式）为：%s.\n",strZL_NR.c_str());

             /////////////////////////////
             //对收到的指令进行处理（插入本地数据库）
             /////////////////////////////


             /////////////////////////
             //用构造插入本地数据库的sql
             /////////////////////////

             string strInsertZL;
             if(intZlNrIsNull){
                 //构造插入本地数据库指令表的sql
                  strInsertZL = "insert into " +
              			string(table_name_YK_ZL) +
              			"(YY_ID,ZL_LX,ZL_BH,ZL_JSSJ,ZL_ZXJG,yh_bs) values(" +
              			int2String(intYY_ID) +
              			","+
              			int2String(intZL_LX) +
              			","+
              			int2String(intZL_BH) +
              			","+
              			"'"+getDateString()+"'" +
              			","+
              			int2String(ZXJG_WD)+
              			","+
              			int2String(gUserId)+
              			")";
             }else{
                 char to[1024]; //>501*2+1
                 bzero(to,1024);
                 unsigned long len;

                 //?此处应该用哪个数据库链接进行脱字处理，还是两者都行
                 len = mysql_real_escape_string(&selfMysqlp->mysql, to, (char *)ucharZL_NR, 507);
                 to[len] = '\0';
                 tmpPrint(LOGFILE,"TMP---Escaped string is: \"%s\" (%lu)\n","临时---Escaped string为: \"%s\" (%lu)\n", to, len);
                 tmpPrint(LOGFILE,"TMP---len after escape: %lu\n","临时---调用escape后的长度: %lu.\n",len);
                 tmpPrint(LOGFILE,"TMP---strlen(to): %zu\n","临时---strlen(to): %zu.\n",strlen(to));
                 dataPrint(LOGFILE,"DAT---ZLNR to Insert is:%s\n","数据---待入库的指令内容：%s.\n",ucharZL_NR);


                 string strZLNR((char *)to);

                 //构造插入本地数据库指令表的sql
                 strInsertZL = "insert into " +
             			string(table_name_YK_ZL) +
             			"(YY_ID,ZL_LX,ZL_BH,ZL_JSSJ,ZL_ZXJG,ZL_NR,yh_bs) values(" +
             			int2String(intYY_ID) +
             			","+
             			int2String(intZL_LX) +
             			","+
             			int2String(intZL_BH) +
             			","+
             			"'"+getDateString()+"'" +
             			","+
             			int2String(ZXJG_WD)+
             			","
             			"'"+strZLNR+"'" +
             			","+
             			int2String(gUserId)+
             			")";

             }
          	sqlPrint(LOGFILE,"SQL---Insert table %s: %s\n","SQL---插入%s表SQL: %s\n",table_name_YK_ZL, strInsertZL.c_str());

        	//////////////////////
          	//指令入库
          	//////////////////////
          	int ret;
          	//尝试三次操作数据库，如果都失败，就认了。
          	int count;
          	for(count=0;count<3;count++){
          		ret = self_mysql_query(selfMysqlp, strInsertZL.c_str());
                if (!ret) {
                	break;
                }else{
                	sleep(1);
                }
          	}



             if (!ret) {
                  prgPrint(LOGFILE,"PRG---Insert into %s, affact %d rows.\n","过程---插入%s表 ，影响%d行.\n",table_name_YK_ZL,
                               self_mysql_affected_rows(selfMysqlp));

                  /////////////////////////////////////////
                  //更新统计信息
                  /////////////////////////////////////////
                  //指令计数增1
                  gZlCount++;
                  //最近指令类型
                  gZlLx = intZL_LX;
                  //最近指令编号
                  gZlBh = intZL_BH;
                  //最近指令的YY_ID
                  gZlYyId = intYY_ID;

                  //统计成功入库的指令数
                  gTotal.packageInsertedToDB ++;
                  prgPrint(LOGFILE,"PRG---Insert 1 ZL (total %lu ZL) to DB from Center DB.\n","过程---从中央数据库入库1条指令 (共%lu条指令).\n",gTotal.packageInsertedToDB);


                  //////////////////////////////////////////
                  //取本地库中刚插入的指令的ZL_ID，更新中央数据库中，以便将来本地数据库有指令执行结果时更新中央数据库
                  //////////////////////////////////////////
                  int intLastInsertedZLID = -1;
                  string strLastInsertId = "SELECT LAST_INSERT_ID();";

                  //如果本地数据库连接就查询
                  if(gIntIsDbConnected==1){
                    	int ret = self_mysql_query(selfMysqlp, strLastInsertId.c_str());

                        if (!ret) {
                             prgPrint(LOGFILE,"PRG---Select last Insert ZL_ID from local DB, affact %d rows.\n","过程---查询到刚插入的本地数据库指令表的ZL_ID ，影响%d行.\n",
                                     self_mysql_affected_rows(selfMysqlp));

                             MYSQL_RES *mysql_result_select_lastZLID = self_mysql_store_result(selfMysqlp);

                             int num_row = 0;
                             //如果取到结果集就取行数
                             if (NULL != mysql_result){
                                 num_row = self_mysql_num_rows(selfMysqlp, mysql_result_select_lastZLID);
                                 tmpPrint(LOGFILE,"TMP---Select from %s %d rows.\n","临时---Select from %s %d rows.\n", table_name_YK_ZL,num_row);
                             }

                             //如果行数为0就报错，否则就记录ZL_ID
                             if(0 == num_row){
                                 tmpPrint(LOGFILE,"TMP---There is no last inserted ZL_ID.\n","临时---没有查到最近插入指令的ZL_ID.\n");

                             }else{
                                 //读取第最新插入的指令ID
                                 MYSQL_ROW mysql_row=self_mysql_fetch_row(selfMysqlp, mysql_result_select_lastZLID);
                                 if (mysql_row[0] != NULL)
                                	 intLastInsertedZLID = atoi(mysql_row[0]);
                             }//if(0 == num_row){
                             //把释放操作放在这里
                             //释放mysql_result_select_lastZLID
                             self_mysql_free_result(selfMysqlp, mysql_result_select_lastZLID);
                         } else {
                             errorPrint(LOGFILE, "ERR---Select last Insert ZL_ID from local DB, error %d: %s.\n", "错误---查询刚插入的本地库指令表记录的ZL_ID失败（错误编号：%d，%s）.\n", self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
                             //2018-05-25
                             //改return为continue
                             //return -5;
                             continue;
                         }//if (!ret)

                  }//if(gIntIsCenterDBConnected==1)


                  //////////////////////////////////////////
                  //更新中央数据库相应记录的状态为接受，zl_id为本地库中插入生成的ZL_ID
                  //////////////////////////////////////////

                  //如果查询到在本地库中插入的最后一条指令的ZL_ID，就更新中央数据库
                  if(-1!=intLastInsertedZLID){
                      string strUpdate_ZLZT_ZLID = "update satellite.all_ins set zl_zt = " +
                  			int2String(ZXJG_JS) +
                  			", bd_id = " +
                  			int2String(intLastInsertedZLID) +
                  			" where id = "+
                  			int2String(intId)+
                  			";";


                      sqlPrint(LOGFILE,"SQL---Update satellite.all_ins.zl_zt and zl_id: %s.\n","SQL---更新中央数据库指令zl_zt和zl_id，SQL: %s.\n",strUpdate_ZLZT_ZLID.c_str());


                      //如果中央数据库连接就执行update操作
                      if(gIntIsCenterDBConnected==1){
                        	int ret = self_mysql_query(centerMysqlp, strUpdate_ZLZT_ZLID.c_str());

                            if (!ret) {
                                 prgPrint(LOGFILE,"PRG---Update satellite.all_ins.zl_zt, zl_id, affact %d rows.\n","过程---更新中央数据库指zl_zt,zl_id ，影响%d行.\n",
                                         self_mysql_affected_rows(centerMysqlp));
                             } else {
                                 errorPrint(LOGFILE, "ERR---Update satellite.all_ins.zl_zt, zl_id error %d: %s.\n", "错误---更新中央数据库zl_zt,zl_id失败（错误编号：%d，%s）.\n", self_mysql_errno(centerMysqlp), self_mysql_error(centerMysqlp));
                                 //2018-05-25
                                 //改return为continue
                                 //return -6;
                                 continue;
                             }//if (!ret)

                      }//if(gIntIsCenterDBConnected==1)

                  }//if(-1!=intLastInsertedZLID){

             } else {//插入本地库%s表失败
                 errorPrint(LOGFILE, "ERR---Insert into local DB %s error %d: %s\n", "错误---插入本地库%s表失败（错误编号：%d，%s）\n", table_name_YK_ZL, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
                	/////////////////////////////////////////
                	//更新统计信息
                	/////////////////////////////////////////
                  //统计入库失败的指令数
                  gTotal.packageFailToInsertedToDB ++;
                  prgPrint(LOGFILE,"PRG---YCYK Fail to Insert 1 ZL (total %lu ZL) to local DB.：\n","过程---主线程入库失败1条指令 (共%lu条指令).\n",gTotal.packageFailToInsertedToDB);



                  //如果插入本地库失败，应该将中央数据库中的相应指令标记为丢弃,而不是return或continue直接处理下一条，因为那样会导致反复处理问题指令
                  //2018-05-25
                  //如果向本地库插入一条指令失败，就尝试下一条（continue），而非return
                  //return -7;
                  intZLShouldBeDiscard = 1;
             }

             //////////////////////////
             //更新中央数据库中问题指令的状态为丢弃
             //////////////////////////
             if(intZLShouldBeDiscard == 1){
                 //构造更新中央数据库问题指令的sql语句
                 string strUpdate_ZLZT_2_GSCW = "update satellite.all_ins set zl_zt = " +
                   			int2String(ZXJG_GSCW) +
                   			" where id = "+
                   			int2String(intId)+
                   			";";
                 sqlPrint(LOGFILE,"SQL---Update wrong ZL in satellite.all_ins: %s.\n","SQL---更新中央数据库错误指令，SQL: %s.\n",strUpdate_ZLZT_2_GSCW.c_str());


                 //如果中央数据库连接就执行update操作
                 if(gIntIsCenterDBConnected==1){
                   	int ret = self_mysql_query(centerMysqlp, strUpdate_ZLZT_2_GSCW.c_str());

                       if (!ret) {
                            prgPrint(LOGFILE,"PRG---Update wrong ZL in satellite.all_ins success, affact %d rows.\n","过程---更新中央数据库错误指令的状态成功 ，影响%d行.\n",
                                    self_mysql_affected_rows(centerMysqlp));
                        } else {
                            errorPrint(LOGFILE, "ERR---Update wrong ZL in satellite.all_ins error %d: %s.\n", "错误---更新中央数据库错误指令的状态失败（错误编号：%d，%s）.\n", self_mysql_errno(centerMysqlp), self_mysql_error(centerMysqlp));
                            continue;
                        }//if (!ret)

                 }//if(gIntIsCenterDBConnected==1)




             }


         }//while(mysql_row = self_mysql_fetch_row(result)){

         //释放此次查询记录
         self_mysql_free_result(centerMysqlp, mysql_result);

    }// if (!ret)
    else {
        errorPrint(LOGFILE, "ERR---Select from Center DB error %d: %s.\n", "错误---查询中央数据库错误（错误编号：%d，%s）.\n",
       		 self_mysql_errno(centerMysqlp),
       		 self_mysql_error(centerMysqlp));
        return -8;
    }

    msgPrint(LOGFILE,"MSG---readZlfromCenterDb success.\n","消息---读中央数据库插本地指令表成功.\n");
    return 1;
}





/*
 * 更新中央数据库中的指令状态
 * @1:执行成功
 * @0:本地数据库没有满足条件的记录
 * @-1:取得本线程对应的数据库链接错误
 * @-2:本地数据库未连接
 * @-3:中央数据库未连接
 * @-4:查询本地数据库错误
 * @-5:更新本地数据库指令表FLAG字段失败
 * @-6:更新中央数据库指令表指令状态失败
 */
int updateZlZttoCenterDb(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: updateZlZttoCenterDb is called.\n","调用+++YKYC.cpp的函数: updateZlZttoCenterDb.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	//取得YK所用的中央数据库链接
	mysql_t * centerMysqlp = &gMysql_centerDBforYK;



	//string strGetZL_ZXJG = "select ZL_ID,ZL_ZXJG from node.GJFW_YK_ZL where ZL_ZXJG!=ZXJG_WD and ZL_ZXJG!=ZXJG_JS";
  	string strGetZL_ZXJG = "select ZL_ID,ZL_ZXJG from node.GJFW_YK_ZL where ZL_FLAG=1";

  	sqlPrint(LOGFILE,"SQL---select local db table %s: %s\n","SQL---读取本地数据库%s表SQL: %s\n","GJFW_YK_ZL", strGetZL_ZXJG.c_str());

  	/////////////////////////////////
  	//读取本地数据库指令表，找到执行结果已经返回的记录，并将执行结果更新到中央数据库
  	/////////////////////////////////

  	//如果本地数据库未连接就返回
  	if(gIntIsDbConnected!=1)
  		return -2;

  	//如果中央数据库未链接就返回
  	if(gIntIsCenterDBConnected!=1)
  		return -3;

	int ret = self_mysql_query(selfMysqlp, strGetZL_ZXJG.c_str());

    if (!ret) {




         MYSQL_RES *mysql_result = self_mysql_store_result(selfMysqlp);



         int num_row = 0;
         //如果取到结果集就取行数
         if (NULL != mysql_result){
             num_row = self_mysql_num_rows(selfMysqlp, mysql_result);
             tmpPrint(LOGFILE,"TMP---Select from %s %d rows in local db.\n","临时---从本地数据库 Select from %s %d 行.\n", table_name_YK_ZL,num_row);
             printf("\\\\\\\\\\\\\\\\\\\\\n\\\\\\\\\\\\\\\\\\\n\\\\\\\\\\\\\n");
             printf("num_row = %d",num_row);
         	R2H("a")
         }

         //如果行数为0就释放记录并退出
         if(0 == num_row){
             tmpPrint(LOGFILE,"TMP---There is no dealed ZL in local db.\n","临时---本地数据库没有已完成指令.\n");
             R2H("b")
             self_mysql_free_result(selfMysqlp, mysql_result);
        	 return 0;
         }



         int intZL_ID,intZL_ZXJG;

         //读取第一条指令
         MYSQL_ROW mysql_row;
         R2H("c")
         while((mysql_row = self_mysql_fetch_row(selfMysqlp,mysql_result))){

             prgPrint(LOGFILE,"PRG---Fetch a row of unread ZL from Center DB.\n","过程---从中央数据库取到一条未读指令\n.");
             R2H("d")

             /////////////////////////
             //读取ZL_ID和指令执行结果
             /////////////////////////
             if (mysql_row[0] != NULL) {
            	 intZL_ID = atoi(mysql_row[0]);
             }
             if (mysql_row[1] != NULL){
            	 intZL_ZXJG = atoi(mysql_row[1]);
             }


             /////////////////////////////
             //对查到的指令执行结果进行处理（插入中央数据库）
             /////////////////////////////
             R2H("e")
             //构造sql
             string strUpdateZLZT = "update satellite.all_ins set zl_zt = "  +
         			int2String(intZL_ZXJG) +
         			" where bd_id = " +
         			int2String(intZL_ID) +
         			" and yh_bs = " +
         			int2String(gUserId) +
         			" and wx_bs = " +
         			int2String(WX_ID) +
         			" and wx_lb = " +
         			int2String(WX_LB) +
         			" and jd_lb = " +
         			int2String(DEVICE_ID) +
         			";";

            R2H("f")
          	sqlPrint(LOGFILE,"SQL---update satellite.all_ins set zl_zt: %s\n","SQL---更新中央数据库指令表指令状态SQL: %s\n", strUpdateZLZT.c_str());

          	int ret;
          	//尝试三次操作数据库，如果都失败，就认了。
          	int count;
          	for(count=0;count<3;count++){
          		ret = self_mysql_query(centerMysqlp, strUpdateZLZT.c_str());
                  if (!ret) {
                  	break;
                  }else{
                  	sleep(1);
                  }
          	}


            /////////////////////////////////////////
            //如果成功更新了中央数据库指令表指令状态，就更新本地数据库相应记录的FLAG字段，使之归零
            /////////////////////////////////////////
             if (!ret) {
                  prgPrint(LOGFILE,"PRG---update satellite.all_ins set zl_zt, affact %d rows.\n","过程---更新中央数据库指令表指令状态 ，影响%d行.\n",
                               self_mysql_affected_rows(centerMysqlp));




                  string strUpdateFlagOfYKZL = "update node.GJFW_YK_ZL set ZL_FLAG = 0 where ZL_ID = " +
              			int2String(intZL_ID)+
              			";";

                  sqlPrint(LOGFILE,"SQL---update node.GJFW_YK_ZL set ZL_FLAG = 0: %s.\n","SQL---更新本地数据库指令表FLAG字段SQL: %s.\n",strUpdateFlagOfYKZL.c_str());


                  //如果数据库连接就执行
                  if(gIntIsDbConnected==1){
                    	int ret = self_mysql_query(selfMysqlp, strUpdateFlagOfYKZL.c_str());

                        if (!ret) {
                             prgPrint(LOGFILE,"PRG---update node.GJFW_YK_ZL set ZL_FLAG, affact %d rows.\n","过程---更新本地数据库指令表FLAG字段 ，影响%d行.\n",
                                     self_mysql_affected_rows(selfMysqlp));
                         } else {
                             errorPrint(LOGFILE, "ERR---update node.GJFW_YK_ZL set ZL_FLAG error %d: %s.\n", "错误---更新本地数据库指令表FLAG字段失败（错误编号：%d，%s）.\n", self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
                             return -5;
                         }//if (!ret)

                  }//if(gIntIsDbConnected==1)

             } else {
                  errorPrint(LOGFILE, "ERR---update satellite.all_ins set zl_zt， error %d: %s\n", "错误---更新中央数据库指令表指令状态失败（错误编号：%d，%s）\n", self_mysql_errno(centerMysqlp), self_mysql_error(centerMysqlp));
                  return -6;
             }



         }//while((mysql_row = self_mysql_fetch_row(selfMysqlp,mysql_result))){

         //释放此次查询记录
         self_mysql_free_result(selfMysqlp, mysql_result);

    }// if (!ret)
    else {
        errorPrint(LOGFILE, "ERR---Select from local DB error %d: %s.\n", "错误---查询本地数据库错误（错误编号：%d，%s）.\n",
       		 self_mysql_errno(selfMysqlp),
       		 self_mysql_error(selfMysqlp));
        return -4;
    }

    msgPrint(LOGFILE,"MSG---updateZlZttoCenterDb success.\n","消息---根据本地数据库指令执行结果更新中央数据库指令表指令状态成功.\n");
    return 1;

}
