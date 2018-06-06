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
		msgPrint(LOGFILE,"MSG---Ip address of interface %s is %s.\n","消息---本机网络接口%s的ip地址为%s.\n",IF_NAME, ip);
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
    		errorPrint(LOGFILE,"ERR---Ip address is not match device id.\n","错误---本机ip与设备id不符.\n");
        	return -3 ;
        }
       // printf("***********************************************Dvice id is %d\n", gDeviceId);



    }else
    {
    	//printf("can not get ip address.\n");
    	errorPrint(LOGFILE,"ERR---Can not get ip address.\n","错误---获取ip地址失败.\n");
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
	}else{
		if (0>=connectDB(&gMysql[_YK])){
				closeDB(&gMysql[_YKYC]);
				errorPrint(LOGFILE,"ERR---Can't connect DB:%d.\n","错误---链接数据库失败（错误编号：%d）.\n", errno);
				return -1;
			}else if (0>=connectDB(&gMysql[_YC])){
				closeDB(&gMysql[_YKYC]);
				closeDB(&gMysql[_YK]);
				errorPrint(LOGFILE,"ERR---Can't connect DB:%d.\n","错误---链接数据库失败（错误编号：%d）.\n", errno);
				return -1;
			}
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
		//return -2;//-2表示无法链接到指令转发服务//无法连接也要往下走，通过重连解决问题
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
	selfMysqlp = &gMysql[_YKYC];
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
	selfMysqlp = &gMysql[_YKYC];
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}




	/*
	if(gIntIsYKThreadLive && gIntIsYCThreadLive)
		gYySxxt++;
		*/

#ifdef _SXXT_ANYWAY
	gYySxxt++;
#else
	if(gIntIsYKThreadLive)
		gYySxxt++;
	else{
		prePrint();printf("YK thread is not active, Sxxt will not increased.\n");
	}
#endif

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
    	prePrint();printf("Update YYZT successfully.\n");
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
	selfMysqlp = &gMysql[_YKYC];
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
	    printf("gZLConnectionTimeOut=%d\n\n",gZLConnectionTimeOut);
	    if (gZLConnectionTimeOut<=0){

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
			//printTJXJ();

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

			//发送按需实时遥测数据
			#ifdef _SEND_SOCKET_DATA
			#ifndef _RUN_ON_XNWX
					sendAxSsycOnSocket();
			#else
					sendAxSsycOnXNWX();
			#endif
			#endif
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
	//srand((unsigned)time(NULL));


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


	if (-1 == ret){
		errorPrint(LOGFILE,"ERR---Can't connect DB:%s.\n", strerror(ret));
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
	selfMysqlp = &gMysql[_YKYC];
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
	selfMysqlp = &gMysql[_YKYC];
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
	selfMysqlp = &gMysql[_YKYC];
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
	selfMysqlp = &gMysql[_YKYC];
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
	selfMysqlp = &gMysql[_YKYC];
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
	selfMysqlp = &gMysql[_YKYC];
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
			" where YSYCPZX_LY = 2"
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
	selfMysqlp = &gMysql[_YKYC];
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
int readZlfromSocket(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: readSocket is called.\n","调用+++YKYC.cpp的函数: readSocket.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = &gMysql[_YKYC];
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

	    if(0!=length){
	    	printYK("---------------------Read %d byte from socket! ",length);
	    }



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
	}else{
		errorPrint(LOGFILE,"ERR-C-Net interrupted, can not read zl from socket.\n","错误-C-网络中断，无法从socket上读取指令.\n");
	}//if (gIntIsSocketConnected == 1)
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
	selfMysqlp = &gMysql[_YKYC];
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
	}else{
		errorPrint(LOGFILE,"ERR-C-Net interrupted, can not send Zqssyc to socket.\n","错误-C-网络中断，无法向socket发送周期遥测.\n");
	}


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
	selfMysqlp = &gMysql[_YKYC];
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
	//先断开链接
	closeSocket();

	if (0 >= connectSocket()){
		errorPrint(LOGFILE,"ERR---Can't reconnect ZL server:%d.\n","错误---重新链接服务器失败（错误编号：%d）.\n", errno);
		//return -2;//-2表示无法链接到指令转发服务
	}else{
		//printf("错误顺序！");
		msgPrint(LOGFILE,"MSG---Reconnect ZL server success.\n","消息---重新链接服务器成功.\n");
		gZLConnectionTimeOut = MAX_ZL_CONNECT_TIMEOUT;
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
#ifdef _RUN_ON_XNWX
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
	selfMysqlp = &gMysql[_YKYC];
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
  	//printf("SQL---select center db table %s: %s\n","satellite.all_ins", strReadZL.c_str());

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

#endif //ifdef _RUN_ON_XNWX


#ifdef _RUN_ON_XNWX
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
	selfMysqlp = &gMysql[_YKYC];
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
             //printf("\\\\\\\\\\\\\\\\\\\\\n\\\\\\\\\\\\\\\\\\\n\\\\\\\\\\\\\n");
             //printf("num_row = %d",num_row);
         }

         //如果行数为0就释放记录并退出
         if(0 == num_row){
             tmpPrint(LOGFILE,"TMP---There is no dealed ZL in local db.\n","临时---本地数据库没有已完成指令.\n");
             self_mysql_free_result(selfMysqlp, mysql_result);
        	 return 0;
         }



         int intZL_ID,intZL_ZXJG;

         //读取第一条指令
         MYSQL_ROW mysql_row;
         while((mysql_row = self_mysql_fetch_row(selfMysqlp,mysql_result))){

             prgPrint(LOGFILE,"PRG---Fetch a row of unread ZL from Center DB.\n","过程---从中央数据库取到一条未读指令\n.");

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
#endif //ifdef _RUN_ON_XNWX


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//2018-6-6
//将YC.cpp内容合并至此
//
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/*
 * YC.cpp
 *
 *  Created on: 2017年5月11日
 *      Author: pyt64
 */




#include "YC.h.bk"

//临时定义个变量，用于决定什么时候向遥测数据包结构写入什么测试数据
int intToWrite;
/*
 * 功能：初始化延时遥测配置项链表数组
 */
void initYsycPzxListArray(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.c FUNC: initYsycPzxListArray is called.\n","调用-C-YC.c的函数: initYsycPzxListArray.\n");

	int i;
	//初始化链表数组和采集周期计数器e
	for (i=0;i<YSYC_PZX_CJZQ_MAX_RANGE;i++){
		gYsycPzxLinkArray[i] = NULL;
		gYsycPzxLinkArray[i] = newYCPZX(0,0,0,NULL);
		gYsycPzxCjzqCount[i] = 0;
		gYsycPzxCjzqArray[i] = i;

	}

#ifdef _YSYC_PZX_CJZQ
	gYsycPzxCjzqArray[0] = YSYC_CJZQ_1;
	gYsycPzxCjzqArray[1] = YSYC_CJZQ_2;
	gYsycPzxCjzqArray[2] = YSYC_CJZQ_3;
	gYsycPzxCjzqArray[3] = YSYC_CJZQ_4;
	gYsycPzxCjzqArray[4] = YSYC_CJZQ_5;
	gYsycPzxCjzqArray[5] = YSYC_CJZQ_6;
	gYsycPzxCjzqArray[6] = YSYC_CJZQ_7;
	gYsycPzxCjzqArray[7] = YSYC_CJZQ_8;
	gYsycPzxCjzqArray[8] = YSYC_CJZQ_9;
	gYsycPzxCjzqArray[9] = YSYC_CJZQ_10;
#endif

	//临时全局变量初始化
	intToWrite = 0;

	msgPrint(LOGFILE,"MSG-C-initYsycPzxListArray success.\n","消息-C-初始化延时遥测配置项链表数组成功.\n");

}

/*
 * 功能：销毁延时遥测配置项链表数组
 */
void destroyYsycListArray(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.c FUNC: destroyYsycListArray is called.\n","调用-C-YC.c的函数: destroyYsycListArray.\n");



	//回收链表数组
	int i;
	for (i=0;i<YSYC_PZX_CJZQ_MAX_RANGE;i++)
		destroyYCPZXList(gYsycPzxLinkArray[i]);

	msgPrint(LOGFILE,"MSG-C-destroyYsycListArray success.\n","消息-C-销毁延时遥测配置项链表数组成功.\n");
}

#ifdef _RUN_ON_XNWX
/*
 *  功能：选取和发送实时遥测数据表中的数据到中央数据库
 */
int sendAxSsycOnXNWX(){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC+++YKYC.cpp FUNC: sendAxSsycOnXNWX is called.\n","调用+++YKYC.cpp的函数: sendAxSsycOnXNWX.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = &gMysql[_YKYC];
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR---Can not get mysql connection for this thread.\n","错误---无法取得本线程的mysql句柄.\n");
		return -1;
	}

	//取得YK所用的中央数据库链接
	mysql_t * centerMysqlp = &gMysql_centerDBforYK;


  	//如果本地数据库未连接就返回
  	if(gIntIsDbConnected!=1)
  		return -2;
  	//如果中央数据库未连接就返回
  	if(gIntIsCenterDBConnected!=1)
  		return -3;



	//在GJFW_YC_SSYCSJ中填写优先级
	//从GJFW_JKGL_YYZT读取YY_YXJ,如果为空就以255-YY_ID作为此应用的优先级
	//从GJFW_YC_SSYCSJ中读取应用自设的优先级（0-9），将应用优先级乘以10与这个自设优先级相加作为该条数据的优先级
	////////////////////////////////////////////////////////////////////////////////////////////
	string strGETYxJ="select a.YY_ID,IFNULL(b.YY_YXJ,255-b.YY_ID)*10+IFNULL(a.SSYC_YXJ,0) from GJFW_YC_SSYCSJ a, GJFW_JKGL_YYZT b where a.YY_ID=b.YY_ID;";

	string strSetSsycYxj = "update GJFW_YC_SSYCSJ a,GJFW_JKGL_YYZT b set a.SSYC_YXJ = IFNULL(b.YY_YXJ,255-b.YY_ID)*10+IFNULL(a.SSYC_YXJ,0)  where a.YY_ID=b.YY_ID and (a.SSYC_YXJ is null or a.SSYC_YXJ<10)";

    if(self_mysql_query(selfMysqlp,strSetSsycYxj.c_str())==0)
    {
           prgPrint(LOGFILE,"PRG-C-Set Ssyc YXJ of  %s success.\n","过程-C-设置实时遥测%s表优先级成功.\n",table_name_YC_SSYCSJ);
    }else {
           errorPrint(LOGFILE,"ERR-C-Set Ssyc YXJ of  %s error.\n","错误-C-设置实时遥测%s表优先级失败.\n",table_name_YC_SSYCSJ);
    }

    ////////////////
    //取出一条优先级最大的实时遥测内容
    ////////////////
    string strSSYC_ID = "";
    string strYY_ID = "";
    string strSSYC_CD = "";
    string strYYSC_NR = "";
    //unsigned char ssyc_nr[SSYC_MAX_LENGTH];

    //20171218 add
    string strSSYC_GS = "";

    int intLengthOfSsycNr = 0 ;

    //20171218 change
    //string strSelectSsycNrWithMaxYxj ="select SSYC_ID,YY_ID,SSYC_CD,SSYC_NR from "
    string strSelectSsycNrWithMaxYxj ="select SSYC_ID,YY_ID,SSYC_CD,SSYC_NR,SSYC_GS from "
    		+ string(table_name_YC_SSYCSJ)
    		//+ " order by SSYC_YXJ desc";
    		+ " where SSYC_GS = 0 and SSYC_CD is not NULL order by SSYC_YXJ desc";

    if(self_mysql_query(selfMysqlp,strSelectSsycNrWithMaxYxj.c_str())==0)
    {
          prgPrint(LOGFILE,"PRG-C-Query table %s success.\n","过程-C-查询%s表成功.\n",table_name_YC_SSYCSJ);
          MYSQL_RES *mysql_result = NULL;

          mysql_result = self_mysql_store_result(selfMysqlp);


          if (NULL != mysql_result){
              int num_row=self_mysql_num_rows(selfMysqlp,mysql_result);
              if (0 != num_row){

                  MYSQL_ROW mysql_row=self_mysql_fetch_row(selfMysqlp,mysql_result);

                  if (NULL == mysql_row){

                	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                	  	  errorPrint(LOGFILE,"ERR-C-Can not fetch any ssyc sj，NULL == mysql_row.\n","错误-C-无法取得实时遥测数据，NULL == mysql_row.\n");
                	  	  return -1;
                  }


                   //取出SSYC_ID内容，如果不为NULL，则输出
                   if (mysql_row[0] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column SSYC_ID in %s.\n","错误-C-%s表SSYC_ID列为空.\n", table_name_YC_SSYCSJ);
            	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                 	  return -1;
                   }else{
                	   	   strSSYC_ID = mysql_row[0];

                   }

                   //取出YY_ID内容，如果不为NULL，则输出
                   if (mysql_row[1] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column YY_ID in %s.\n","错误-C-%s表YY_ID列为空.\n", table_name_YC_SSYCSJ);
            	  	  //20171227
                	   //self_mysql_free_result(selfMysqlp,mysql_result);
            	  	  //return -1;
                	   strYY_ID="255";
                   }else{
                	   	   strYY_ID = mysql_row[1];

                   }

                   //取出SSYC_CD内容，如果不为NULL，则输出
                   if (mysql_row[2] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column SSYC_CD in %s.\n","错误-C-%s表SSYC_CD列为空.\n", table_name_YC_SSYCSJ);
            	  	  //self_mysql_free_result(selfMysqlp,mysql_result);
            	  	  //return -1;
                	   strSSYC_CD="0";
                   }else{

                	   	   strSSYC_CD = mysql_row[2];
                	   	   intLengthOfSsycNr = atoi(strSSYC_CD.c_str());
                   	   dataPrint(LOGFILE,"SSYC_CD is: %d .\n","实时遥测长度SSYC_CD: %d .\n",intLengthOfSsycNr);
                   }

                   //取出SSYC_NR内容，如果不为NULL，则输出
                   if (mysql_row[3] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column SSYC_NR in %s.\n","错误-C-%s表SSYC_NR列为空.\n", table_name_YC_SSYCSJ);
            	  	  //self_mysql_free_result(selfMysqlp,mysql_result);
                 	  //return -1;
                   }else{
                	   strYYSC_NR = mysql_row[3];


                   }



            	   //////////////////////////
            	   //定义一个遥测字符串，并填充
            	   //////////////////////////
            	   string strYC = "";
            	   strYC = "YY_ID=" +
            			   strYY_ID +
            			   ", YC_NR=" +
            			   strYYSC_NR;



                   ////////////////////////////
                   //向中央数据库遥测表写入遥测数据
                   ////////////////////////////

                   if (gIntIsCenterDBConnected == 1){
                       //按需遥测次数自增
                       gAxYcCount ++;




                       //向中央数据库插入遥测数据
                       string strInsert_AXYC2CenterDB = "insert satellite.qlzx_jsjd_yc(yh_bs,wx_lb,wx_bs,jd_lb,js_js,yc_xx) values( " +
                    		   int2String(gUserId) +
                    		   "," +
                    		   int2String(WX_LB) +
                    		   "," +
                    		   int2String(WX_ID) +
                    		   "," +
                    		   int2String(JD_LB) +
                    		   ",\'" +
                    		   getDateString() +
                    		   "\',\'" +
                    		   strYC +
                    		   "\');";


                        sqlPrint(LOGFILE,"SQL---Insert satellite.qlzx_test_yc: %s.\n","SQL---插入中央数据遥测，SQL: %s.\n",strInsert_AXYC2CenterDB.c_str());

                        int ret = self_mysql_query(centerMysqlp, strInsert_AXYC2CenterDB.c_str());

                        if (!ret) {
                             prgPrint(LOGFILE,"PRG---Insert satellite.qlzx_test_yc, affact %d rows.\n","过程---插入中央数据遥测成功 ，影响%d行.\n",
                                     self_mysql_affected_rows(centerMysqlp));
                         } else {
                             errorPrint(LOGFILE, "ERR---Insert satellite.qlzx_test_yc error %d: %s.\n", "错误---插入中央数据遥测失败（错误编号：%d，%s）.\n", self_mysql_errno(centerMysqlp), self_mysql_error(centerMysqlp));

                         }//if (!ret)

                  	}//if (gIntIsCenterDBConnected == 1){
               }//if (0 != num_row){
              self_mysql_free_result(selfMysqlp,mysql_result);


          }//(NULL != mysql_result){
    }else {
           errorPrint(LOGFILE,"ERR-C-Query table %s error.\n","错误-C-查询%s表失败.\n",table_name_YC_SSYCSJ);
    }



    ////////////////
    //删除已经发送的实时遥测数据
    ////////////////
#ifndef _DONT_DELETE_AFTER_SEND_AX_SSYC
    if("" != strSSYC_ID){
        string strDeleteSsycSj = "delete from GJFW_YC_SSYCSJ where SSYC_ID = "
        		+strSSYC_ID;

        if(self_mysql_query(selfMysqlp,strDeleteSsycSj.c_str())==0)
        {
               prgPrint(LOGFILE,"PRG-C-Delete Ssycsj of  %s success.\n","过程-C-删除%s表已发送的实时遥测数据成功.\n",table_name_YC_SSYCSJ);
        }else {
               errorPrint(LOGFILE,"ERR-C-Delete Ssycsj of  %s error.\n","错误-C-删除%s表已发送的实时遥测数据失败.\n",table_name_YC_SSYCSJ);
        }
    }
#endif


    ////////////////
    //将所有超过9的优先级都自增1
    ////////////////
    string strUpdateSsycYxj = "update GJFW_YC_SSYCSJ set SSYC_YXJ = SSYC_YXJ+1 where SSYC_YXJ>9";
    if(self_mysql_query(selfMysqlp,strUpdateSsycYxj.c_str())==0)
    {
           prgPrint(LOGFILE,"PRG-C-Update Ssyc YXJ of  %s success.\n","过程-C-更新实时遥测数据表%s的优先级+1成功.\n",table_name_YC_SSYCSJ);
    }else {
           errorPrint(LOGFILE,"ERR-C-Update Ssyc YXJ of  %s error.\n","错误-C-更新实时遥测数据表%s的优先级+1失败.\n",table_name_YC_SSYCSJ);
    }

   prgPrint(LOGFILE,"PRG-C-sendAxSsyc success.\n","过程-C-发送按需实时遥测数据成功");


	return 1;
}

#else
/*
 *  功能：选取和发送实时遥测数据表中的数据到集成单元
 */
int sendAxSsycOnSocket(void){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.c FUNC: sendAxSsyc is called.\n","调用-C-YC.c的函数: sendAxSsyc.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = &gMysql[_YKYC];
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR-C-Can not get mysql connection for this thread.\n","错误-C-发送实时遥测数据失败，无法取得本线程的mysql句柄.\n");
		return -1;
	}

	//在GJFW_YC_SSYCSJ中填写优先级
	//从GJFW_JKGL_YYZT读取YY_YXJ,如果为空就以255-YY_ID作为此应用的优先级
	//从GJFW_YC_SSYCSJ中读取应用自设的优先级（0-9），将应用优先级乘以10与这个自设优先级相加作为该条数据的优先级
	////////////////////////////////////////////////////////////////////////////////////////////
	string strGETYxJ="select a.YY_ID,IFNULL(b.YY_YXJ,255-b.YY_ID)*10+IFNULL(a.SSYC_YXJ,0) from GJFW_YC_SSYCSJ a, GJFW_JKGL_YYZT b where a.YY_ID=b.YY_ID;";

	string strSetSsycYxj = "update GJFW_YC_SSYCSJ a,GJFW_JKGL_YYZT b set a.SSYC_YXJ = IFNULL(b.YY_YXJ,255-b.YY_ID)*10+IFNULL(a.SSYC_YXJ,0)  where a.YY_ID=b.YY_ID and (a.SSYC_YXJ is null or a.SSYC_YXJ<10)";

    if(self_mysql_query(selfMysqlp,strSetSsycYxj.c_str())==0)
    {
           prgPrint(LOGFILE,"PRG-C-Set Ssyc YXJ of  %s success.\n","过程-C-设置实时遥测%s表优先级成功.\n",table_name_YC_SSYCSJ);
    }else {
           errorPrint(LOGFILE,"ERR-C-Set Ssyc YXJ of  %s error.\n","错误-C-设置实时遥测%s表优先级失败.\n",table_name_YC_SSYCSJ);
    }

    ////////////////
    //取出一条优先级最大的实时遥测内容
    ////////////////
    string strSSYC_ID = "";
    string strYY_ID = "";
    string strSSYC_CD = "";
    unsigned char ssyc_nr[SSYC_MAX_LENGTH];

    //20171218 add
    string strSSYC_GS = "";

    int intLengthOfSsycNr = 0 ;

    //20171218 change
    //string strSelectSsycNrWithMaxYxj ="select SSYC_ID,YY_ID,SSYC_CD,SSYC_NR from "
    string strSelectSsycNrWithMaxYxj ="select SSYC_ID,YY_ID,SSYC_CD,SSYC_NR,SSYC_GS from "
    		+ string(table_name_YC_SSYCSJ)
    		//+ " order by SSYC_YXJ desc";
    		+ " where SSYC_CD is not NULL order by SSYC_YXJ desc";

    if(self_mysql_query(selfMysqlp,strSelectSsycNrWithMaxYxj.c_str())==0)
    {
          prgPrint(LOGFILE,"PRG-C-Query table %s success.\n","过程-C-查询%s表成功.\n",table_name_YC_SSYCSJ);
          MYSQL_RES *mysql_result = NULL;

          mysql_result = self_mysql_store_result(selfMysqlp);


          if (NULL != mysql_result){
              int num_row=self_mysql_num_rows(selfMysqlp,mysql_result);
              if (0 != num_row){

                  MYSQL_ROW mysql_row=self_mysql_fetch_row(selfMysqlp,mysql_result);

                  if (NULL == mysql_row){

                	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                	  	  errorPrint(LOGFILE,"ERR-C-Can not fetch any ssyc sj，NULL == mysql_row.\n","错误-C-无法取得实时遥测数据，NULL == mysql_row.\n");
                	  	  return -1;
                  }


                   //取出SSYC_ID内容，如果不为NULL，则输出
                   if (mysql_row[0] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column SSYC_ID in %s.\n","错误-C-%s表SSYC_ID列为空.\n", table_name_YC_SSYCSJ);
            	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                 	  return -1;
                   }else{
                	   	   strSSYC_ID = mysql_row[0];

                   }

                   //取出YY_ID内容，如果不为NULL，则输出
                   if (mysql_row[1] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column YY_ID in %s.\n","错误-C-%s表YY_ID列为空.\n", table_name_YC_SSYCSJ);
            	  	  //20171227
                	   //self_mysql_free_result(selfMysqlp,mysql_result);
            	  	  //return -1;
                	   strYY_ID="255";
                   }else{
                	   	   strYY_ID = mysql_row[1];

                   }

                   //取出SSYC_CD内容，如果不为NULL，则输出
                   if (mysql_row[2] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column SSYC_CD in %s.\n","错误-C-%s表SSYC_CD列为空.\n", table_name_YC_SSYCSJ);
            	  	  //self_mysql_free_result(selfMysqlp,mysql_result);
            	  	  //return -1;
                	   strSSYC_CD="0";
                   }else{

                	   	   strSSYC_CD = mysql_row[2];
                	   	   intLengthOfSsycNr = atoi(strSSYC_CD.c_str());
                   	   dataPrint(LOGFILE,"SSYC_CD is: %d .\n","实时遥测长度SSYC_CD: %d .\n",intLengthOfSsycNr);
                   }

                   //取出SSYC_NR内容，如果不为NULL，则输出
                   if (mysql_row[3] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column SSYC_NR in %s.\n","错误-C-%s表SSYC_NR列为空.\n", table_name_YC_SSYCSJ);
            	  	  //self_mysql_free_result(selfMysqlp,mysql_result);
                 	  //return -1;
                	   ssyc_nr[0]=0;
                   }else{
                   	   int j;

                       for (j=0;j<intLengthOfSsycNr;j++){
                    	   	   ssyc_nr[j] =  mysql_row[3][j];
                    	   	   //printf("%02x ",ssyc_nr[j]);
                       } //for


                   }

                   //20171218 add
                   //取出SSYC_GS内容，如果不为NULL，则输出
                   if (mysql_row[4] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column SSYC_GS in %s.\n","错误-C-%s表SSYC_GS列为空.\n", table_name_YC_SSYCSJ);
                	   strSSYC_GS="0";
                	   //self_mysql_free_result(selfMysqlp,mysql_result);
            	  	  //return -1;
                   }else{

                	   	   strSSYC_GS = mysql_row[4];
                	   	  // intLengthOfSsycNr = atoi(strSSYC_GS.c_str());
                	   	   dataPrint(LOGFILE,"SSYC_GS is: %d .\n","实时遥测GS SSYC_GS: %s .\n",strSSYC_GS.c_str());
                   }

                   //20171218 change
            	   dataPrint(LOGFILE,"DAT-C-Debug read a SSYCSJ with SSYC_ID =%s, YY_ID = %s, SSYC_GS=%s, SSYC_NR = .\n","数据-C-Debug读到一条实时遥测数据 SSYC_ID =%s, YY_ID = %s, SSYC_GS=%s, SSYC_NR = .\n",strSSYC_ID.c_str(),strYY_ID.c_str(),strSSYC_GS.c_str());
               int j;
            	   for (j=0;j<intLengthOfSsycNr;j++){
                	   dataPrint(LOGFILE,"%02X","%02X",ssyc_nr[j]);
                	   dataPrint(LOGFILE,"\n","\n");
               }

            	   /*
            	   //20171218 change
            	   printf("DAT-C-Debug read a SSYCSJ with SSYC_ID =%s, YY_ID = %s, SSYC_GS=%s, SSYC_NR = .\n",strSSYC_ID.c_str(),strYY_ID.c_str(),strSSYC_GS.c_str());
               int jj;
            	   for (jj=0;jj<intLengthOfSsycNr;jj++){
            		   printf("%02X",ssyc_nr[jj]);
            		   printf("\n");
               }
               */
            	   //////////////////////////
            	   //定义一个发送结构，并填充
            	   //////////////////////////
            	   //20171218 change
            	   unsigned char sendData[LEN_OF_SSYC_HEAD+intLengthOfSsycNr];
            	   //unsigned char sendData[LEN_OF_SSYC_HEAD+intLengthOfSsycNr+1];

            	   //写入遥测数据包头
            	   ssyc_head_t * pSsycHeader = (ssyc_head_t *)sendData;
            	   pSsycHeader->header[0] = SSYC_SYNC_0;
            	   pSsycHeader->header[1] = SSYC_SYNC_1;
            	   pSsycHeader->yyId = atoi(strYY_ID.c_str());
            	   pSsycHeader->length = intLengthOfSsycNr;
            	   pSsycHeader->gs = atoi(strSSYC_GS.c_str());

            	   //写入遥测数据内容

                   for (j=0;j<intLengthOfSsycNr;j++){
                	   	   //20171218 change
                	   	   sendData[j+LEN_OF_SSYC_HEAD] =  ssyc_nr[j];
                	   	   //sendData[j+LEN_OF_SSYC_HEAD+1] =  ssyc_nr[j];
                   } //for

                   //20171218 change
                   char to[(SSYC_MAX_LENGTH+LEN_OF_SSYC_HEAD)*2+1];
                   bzero(to,(SSYC_MAX_LENGTH+LEN_OF_SSYC_HEAD)*2+1);
                   //char to[(SSYC_MAX_LENGTH+LEN_OF_SSYC_HEAD+1)*2+1];
                   //bzero(to,(SSYC_MAX_LENGTH+LEN_OF_SSYC_HEAD+1)*2+1)
                   unsigned long len;

                   //20171218 change
                   tmpPrint(LOGFILE,"TMP-C-intLengthOfSsycNr+LEN_OF_SSYC_HEAD: %zu.\n","临时-C-intLengthOfSsycNr+LEN_OF_SSYC_HEAD: %zu.\n",(size_t)(intLengthOfSsycNr+LEN_OF_SSYC_HEAD));
                   //tmpPrint(LOGFILE,"TMP-C-intLengthOfSsycNr+LEN_OF_SSYC_HEAD+1: %zu.\n","临时-C-intLengthOfSsycNr+LEN_OF_SSYC_HEAD+1: %zu.\n",(size_t)(intLengthOfSsycNr+LEN_OF_SSYC_HEAD+1));

                   //用宏ESCAPE_STRTING_BEFORE_SEND_AXSSYC控制是否在发送按需实时遥测前对整个senddata再做一次脱0处理
#ifdef ESCAPE_STRTING_BEFORE_SEND_AXSSYC
                   //20171218 change
                   len = mysql_real_escape_string(selfMysqlp->mysql, to, (char *)sendData, (intLengthOfSsycNr+LEN_OF_SSYC_HEAD));
                   //len = mysql_real_escape_string(selfMysqlp->mysql, to, (char *)sendData, (intLengthOfSsycNr+LEN_OF_SSYC_HEAD+1));
                   to[len] = '\0';
#else
                   //20171218 change
                   copyUCharArray((unsigned char*)sendData,(unsigned char*)to, (intLengthOfSsycNr+LEN_OF_SSYC_HEAD));
                   //copyUCharArray((unsigned char*)sendData,(unsigned char*)to, (intLengthOfSsycNr+LEN_OF_SSYC_HEAD+1));
                   len =  (intLengthOfSsycNr+LEN_OF_SSYC_HEAD);
                 //  len =  (intLengthOfSsycNr+LEN_OF_SSYC_HEAD+1);
#endif
                   tmpPrint(LOGFILE,"TMP-C-Escaped string is: \"%s\" (%lu).\n","临时-C-Escaped string: \"%s\" (%lu).\n", to, len);

                   tmpPrint(LOGFILE,"TMP-C-len after escape: %lu.\n","临时-C-escape操作后的长度: %lu.\n",len);
                   tmpPrint(LOGFILE,"TMP-C-strlen(to): %zu.\n","临时-C-strlen(to): %zu.\n",strlen(to));



                   ////////////////////////////
                   //发送实时遥测数据
                   ////////////////////////////
                   int ssycDataLength = len;

                   char * socketWriteBuffer = (char *)(to);

                   if (gIntIsSocketConnected == 1){
                  		//prgPrint(LOGFILE,"PRG---Going to send gSsycStruct to zl server.\n");

                  		int length = send(gNetSocket.client_sockfd,socketWriteBuffer,ssycDataLength,MSG_DONTWAIT);
                  	    dataPrint(LOGFILE,"DAT---YC Loop send %d byte gSsycStruct data to socket.\n","数据---遥测线程发送%d byte实时遥测数据到服务器.\n",length);

                  	    /*
                  	    printf("数据---遥测线程发送%d byte实时遥测数据到服务器.\n",length);
                  	    //打印发送的socket数据///////////////////////
                  	    int i;
                  	    for (i=0;i<length;i++){
                  	    	debugPrint_YCPackage(LOGFILE,"%02X ","%02X ",(unsigned char)socketWriteBuffer[i]);
                  	    	printf("%02X ",(unsigned char)socketWriteBuffer[i]);
                  	    }
                  	    debugPrint_YCPackage(LOGFILE,"\n","\n");
                  	    printf("\n");
                  	    */
                        //按需遥测次数自增
                        gAxYcCount ++;
                  	}else{
                  		errorPrint(LOGFILE,"ERR-C-Net interrupted, can not send Axssyc.\n","错误-C-网络中断，无法发送按需遥测数据.\n");

                  	}//if (gIntIsSocketConnected == 1)

               }//if (0 != num_row){
              self_mysql_free_result(selfMysqlp,mysql_result);


          }//(NULL != mysql_result){
    }else {
           errorPrint(LOGFILE,"ERR-C-Query table %s error.\n","错误-C-查询%s表失败.\n",table_name_YC_SSYCSJ);
    }



    ////////////////
    //删除已经发送的实时遥测数据
    ////////////////
#ifndef _DONT_DELETE_AFTER_SEND_AX_SSYC
    if("" != strSSYC_ID){
        string strDeleteSsycSj = "delete from GJFW_YC_SSYCSJ where SSYC_ID = "
        		+strSSYC_ID;

        if(self_mysql_query(selfMysqlp,strDeleteSsycSj.c_str())==0)
        {
               prgPrint(LOGFILE,"PRG-C-Delete Ssycsj of  %s success.\n","过程-C-删除%s表已发送的实时遥测数据成功.\n",table_name_YC_SSYCSJ);
        }else {
               errorPrint(LOGFILE,"ERR-C-Delete Ssycsj of  %s error.\n","错误-C-删除%s表已发送的实时遥测数据失败.\n",table_name_YC_SSYCSJ);
        }
    }
#endif


    ////////////////
    //将所有超过9的优先级都自增1
    ////////////////
    string strUpdateSsycYxj = "update GJFW_YC_SSYCSJ set SSYC_YXJ = SSYC_YXJ+1 where SSYC_YXJ>9";
    if(self_mysql_query(selfMysqlp,strUpdateSsycYxj.c_str())==0)
    {
           prgPrint(LOGFILE,"PRG-C-Update Ssyc YXJ of  %s success.\n","过程-C-更新实时遥测数据表%s的优先级+1成功.\n",table_name_YC_SSYCSJ);
    }else {
           errorPrint(LOGFILE,"ERR-C-Update Ssyc YXJ of  %s error.\n","错误-C-更新实时遥测数据表%s的优先级+1失败.\n",table_name_YC_SSYCSJ);
    }

   prgPrint(LOGFILE,"PRG-C-sendAxSsyc success.\n","过程-C-发送按需实时遥测数据成功");


    return 1;
}


#endif

void* main_loop_of_YC(void *arg){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.c FUNC: main_loop_of_YC is called.\n","调用-C-YC.c的函数: main_loop_of_YC.\n");

	threadPrint_YC(LOGFILE,"Thr-C-main_loop_of_YC: thread is started successfully.\n","线程-C-遥测线程启动成功.\n");

	int subpid = getpid();
	threadPrint_YC(LOGFILE,"Thr-C-Pid of YC thread is %d.\n","线程-C-遥额线程Pid为%d.\n",subpid);

	//记录YC线程pt
	gTotal.YcPt = pthread_self();

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = &gMysql[_YKYC];
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR-C-Can not get mysql connection for this thread.\n","错误-C-无法取得本线程的mysql句柄.\n");

		int *pRet =  (int*)malloc(sizeof(int));
		*pRet = 1;

		msgPrint(LOGFILE,"MSG-C-main_loop_of_YC is going to close.\n","消息-C-遥测线程即将关闭.\n");

		pthread_exit((void*)pRet);
	}

	//gSubtid_2 = pthread_self();
	//threadPrint_YK("Thr-C-Tid of YC thread is %lu.\n",gSubtid_2);








	while(gIntIsRun){
		gIntNewStatYCThread++;


		//发送按需实时遥测数据（从按需实时遥测表中取出数据并发送）
//发送按需实时遥测数据
#ifdef _SEND_SOCKET_DATA
#ifndef _RUN_ON_XNWX
		sendAxSsycOnSocket();
#else
		sendAxSsycOnXNWX();
#endif
#endif

#ifndef _LOOP
		break;
#else
		sleep(SLEEP_YC_LOOP);
#endif

	}






	int *pRet =  (int*)malloc(sizeof(int));
	*pRet = 1;

	msgPrint(LOGFILE,"MSG-C-main_loop_of_YC is going to close.\n","消息-C-遥测线程即将关闭.\n");

	pthread_exit((void*)pRet);

	//return ( (void*)1);
}

/*
//启动调用遥测处理函数的定时器
int startYSYCSJCJTimer(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.cpp FUNC: startYSYCSJCJTimer is called.\n");


	if(signal(SIGALRM, gatherYsycData) == SIG_ERR)
    {
         perror("register a timer for startYSYCSJCJTimer（SIGALRM） fail");
         return -1;
    }


    return 1;
}
*/



//进行按需周期发送的实时遥测数据采集，填入实时遥测包结构
int gatherZqSsycData(void){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.cpp FUNC: gatherZqSsycData is called.\n","调用-C-YC.cpp的函数: gatherZqSsycData.\n");


	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = &gMysql[_YKYC];
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR-C-Can not gather ZqSsyc Data for this thread.\n","错误-C-采集实时遥测数据失败，无法取得本线程的mysql句柄.\n");
		return -1;
	}


	//2018-05-24
	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1){
		errorPrint(LOGFILE,"ERR---Can not gather ZqSsyc Data，fail to connect DB.\n","错误---采集实时遥测数据失败，无法链接到数据库.\n");
		return -1;
	}

	//确定采集量现在应该写哪个遥测包结构
	int index = gSsycStruct.intDataCJUsing;
	//实时遥测包结构


	/*
	//以下代码用于测试
	//准备一个数据，分别写入26个字母
	char ziMu[26];
	int i;
	for (i=0;i<26;i++){
		ziMu[i] = 65+i;
	}
	gSsycStruct.zqssycData[index].ifZR = 0x01; //0x01当前节点正在注入
	gSsycStruct.zqssycData[index].ifSC = ziMu[intToWrite%26]; //0x01当前节点正在数传
	*/

	//填写指令信息
	gSsycStruct.zqssycData[index].zlCount = gZlCount;
	gSsycStruct.zqssycData[index].zlLx = gZlLx;
	gSsycStruct.zqssycData[index].zlBh = gZlBh;
	gSsycStruct.zqssycData[index].zlYyId = gZlYyId;


	//get zl_zxjg

	__int8_t zlZxjg; //最近指令的ZL_ZXJG

	//填写当前节点信息
	string strSqlFetchJZLZXJG = "select ZL_ZXJG from GJFW_YK_ZL where YY_ID = "+
			int2String(gZlYyId) +
			" and ZL_LX = "+
			int2String(gZlLx)+
			" and ZL_BH = "+
			int2String(gZlBh)+
			" order by ZL_ID desc;";


 	//get ZL_ZXJG
 	int ret1 = self_mysql_query(selfMysqlp, strSqlFetchJZLZXJG.c_str());
     if (!ret1) {
          prgPrint(LOGFILE,"PRG-C-Select from %s, affact %d rows.\n","过程-C-查询%s表，影响%d行.\n", table_name_YK_ZL,self_mysql_affected_rows(selfMysqlp));
          MYSQL_RES *mysql_result = NULL;

          mysql_result = self_mysql_store_result(selfMysqlp);


          if (NULL != mysql_result){
              int num_row=self_mysql_num_rows(selfMysqlp,mysql_result);
              //printf("get %d row\n",num_row);
              if (0 != num_row){


                  MYSQL_ROW mysql_row=self_mysql_fetch_row(selfMysqlp,mysql_result);

                  if (NULL == mysql_row){

                	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                	  	  errorPrint(LOGFILE,"ERR-C-Can not fetch any ZL information，NULL == mysql_row.\n","错误-C-无法取得ZL信息，NULL == mysql_row.\n");
                	  	  return -1;
                  }


                   //取出ZLZXJG内容，如果不为NULL，则记录
                   if (mysql_row[0] == NULL){

                	   errorPrint(LOGFILE,"ERR-C-Get a null column ZL_ZXJG in %s.\n","错误-C-%s表ZL_ZXJG列为空.\n", table_name_YK_ZL);
            	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                 	  return -1;
                   }else{

                	   gSsycStruct.zqssycData[index].zlZxjg = atoi(mysql_row[0]);

                   }

              }
              self_mysql_free_result(selfMysqlp,mysql_result);
          }
     }//if (!ret)
     else {
     	//如果插入延时遥测数据失败，继续运行
         errorPrint(LOGFILE, "ERR-C-Select from %s error %d: %s.\n", "错误-C-查询%s表失败（错误编号：%d，%s）.\n",table_name_YK_ZL, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
     }//else
     //get zl_zxjg


     //get yyzt begin////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 	//
 	string strSqlFetchJYYZT = "select YY_ID,YY_YXZT,YY_SFBH,YY_ZQDSFSN,YY_AZZT from GJFW_JKGL_YYZT;";


  	//get ZL_ZXJG
  	int ret2 = self_mysql_query(selfMysqlp, strSqlFetchJYYZT.c_str());

      if (!ret2) {
           prgPrint(LOGFILE,"PRG-C-Select from %s, affact %d rows.\n","过程-C-查询%s表，影响%d行.\n", table_name_JKGL_YYZT,self_mysql_affected_rows(selfMysqlp));
           MYSQL_RES *mysql_result = NULL;

           mysql_result = self_mysql_store_result(selfMysqlp);


           if (NULL != mysql_result){

               int num_row=self_mysql_num_rows(selfMysqlp,mysql_result);

               //printf("get %d row\n",num_row);
               if (0 != num_row){

            	   //reset yyzt
            	   int ii;
            	   for (ii=0;ii<32;ii++){
            		   gSsycStruct.zqssycData[index].yyZt[ii]=0;
            		   gSsycStruct.zqssycData[index].sfBh[ii]=0;
            		   gSsycStruct.zqssycData[index].sfZqd[ii]=0;
            		   gSsycStruct.zqssycData[index].sfAz[ii]=0;
            	   }


            	   //  MYSQL_ROW mysql_row=self_mysql_fetch_row(selfMysqlp,mysql_result);
            	   //逐条读取遥测配置项并加入链表数组对应项
            	   MYSQL_ROW mysql_row = NULL;
            	   while((mysql_row = self_mysql_fetch_row(selfMysqlp,mysql_result))){
            		   /*
                       if (NULL == mysql_row){

                     	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                     	  	  errorPrint(LOGFILE,"ERR-C-Can not fetch any  yyzt information，NULL == mysql_row.\n","错误-C-无法取得YYZT信息，NULL == mysql_row.\n");
                     	  	  return -1;
                       }
                       */

                        int yyId = 0, yyZt = 0, sfBh = 0, sfZqd = 0, sfAz = 0;


                        //2018-05-24
                        //修改gatherZqSsycData（）函数，当mysql_row[0..4] == NULL时，不再释放mysql_result，因为紧跟的return语句被注释掉，造成多次释放。
                        //取出YY_ID,YY_YXZT,YY_SFBH,YY_ZQDSFSN内容，如果不为NULL，则记录
                        if (mysql_row[0] == NULL){

                     	   errorPrint(LOGFILE,"ERR-C-Get a null column YY_ID in %s.\n","错误-C-%s表YY_ID列为空.\n", table_name_JKGL_YYZT);
                 	  	  //self_mysql_free_result(selfMysqlp,mysql_result);
                      	  //return -1;
                        }else{

                        	yyId = atoi(mysql_row[0]);
                        }


                        //取出YY_YXZT内容，如果不为NULL，则记录
                        if (mysql_row[1] == NULL){

                     	   errorPrint(LOGFILE,"ERR-C-Get a null column YY_YXZT in %s.\n","错误-C-%s表YY_YXZT列为空.\n", table_name_JKGL_YYZT);
                 	  	  //self_mysql_free_result(selfMysqlp,mysql_result);
                      	  //return -1;
                        }else{

                        	yyZt = atoi(mysql_row[1]);

                        }

                        //取出YY_SFBH内容，如果不为NULL，则记录
                        if (mysql_row[2] == NULL){

                     	   errorPrint(LOGFILE,"ERR-C-Get a null column YY_SFBH in %s.\n","错误-C-%s表YY_SFBH列为空.\n", table_name_JKGL_YYZT);
                 	  	  //self_mysql_free_result(selfMysqlp,mysql_result);
                      	  //return -1;
                        }else{

                        	sfBh = atoi(mysql_row[2]);

                        }

                        //取出YY_ZQDSFSN内容，如果不为NULL，则记录
                        if (mysql_row[3] == NULL){

                     	   errorPrint(LOGFILE,"ERR-C-Get a null column YY_ZQDSFSN in %s.\n","错误-C-%s表YY_ZQDSFSN列为空.\n", table_name_JKGL_YYZT);
                 	  	  //self_mysql_free_result(selfMysqlp,mysql_result);
                      	  //return -1;
                        }else{

                        	sfZqd = atoi(mysql_row[3]);

                        }

                        //取出YY_AZZT内容，如果不为NULL，则记录
                        if (mysql_row[4] == NULL){

                     	   //errorPrint(LOGFILE,"ERR-C-Get a null column YY_AZZT in %s.\n","错误-C-%s表YY_AZZT列为空.\n", table_name_JKGL_YYZT);
                 	  	  //self_mysql_free_result(selfMysqlp,mysql_result);
                      	  //return -1;
                        }else{
                        	sfAz = atoi(mysql_row[4]);
                        }




                        //compute , i,j for sfBh[i]
                        int i,j;
                        i = yyId/8;
                        /*
                        if (yyId<32)
                        	i=0;
                        else if(yyId<64)
                        	i=1;
                        else if (yyId<96)
                        	i=2;
                        else if (yyId<128)
                        	i=3;
                        else if (yyId<160)
                        	i=4;
                        else if (yyId<192)
                        	i=5;
                        else if (yyId <224)
                        	i=6;
                        else if (yyId < 256)
                        	i=7;
*/
                        j = (yyId%8);

                        //record sfBh
                        if (sfBh>0)
                        	gSsycStruct.zqssycData[index].sfBh[i] |= 128u>>j;
                        else
                        	gSsycStruct.zqssycData[index].sfBh[i] &= ~(128u>>j);

                        //record sfZqd
                        if (sfZqd>0)
                        	gSsycStruct.zqssycData[index].sfZqd[i] |= 128u>>j;
                        else
                        	gSsycStruct.zqssycData[index].sfZqd[i] &= ~(128u>>j);

                        //record sfAz
                         if (sfAz>0)
                        	 gSsycStruct.zqssycData[index].sfAz[i] |= 128u>>j;
                         else
                        	 gSsycStruct.zqssycData[index].sfAz[i] &= ~(128u>>j);


                         //record yyZt
                         if (yyZt>0)
                        	 gSsycStruct.zqssycData[index].yyZt[i] |= 128u>>j;
                         else
                        	 gSsycStruct.zqssycData[index].yyZt[i] &= ~(128u>>j);




            	   }//while((mysql_row = self_mysql_fetch_row(selfMysqlp,mysql_result)))
/*
                   int jj;
                   for(jj=0;jj<8;jj++){

                  	 	__uint32_t uInt32Tmp;
                  	 	uInt32Tmp=gSsycStruct.zqssycData[index].yyZt[ii];
                  	 	gSsycStruct.zqssycData[index].yyZt[ii] = htonl(uInt32Tmp);

                  	 	uInt32Tmp=gSsycStruct.zqssycData[index].sfBh[ii];
                  	 	gSsycStruct.zqssycData[index].sfBh[ii] = htonl(uInt32Tmp);

                  	 	uInt32Tmp=gSsycStruct.zqssycData[index].sfZqd[ii];
                  	 	gSsycStruct.zqssycData[index].sfZqd[ii] = htonl(uInt32Tmp);

                  	 	uInt32Tmp=gSsycStruct.zqssycData[index].sfAz[ii];
                  	 	gSsycStruct.zqssycData[index].sfAz[ii] = htonl(uInt32Tmp);
                   }//for(jj=0;jj<8;jj++){
*/


               }// if (0 != num_row){
               self_mysql_free_result(selfMysqlp,mysql_result);
           }// if (NULL != mysql_result){
      }//if (!ret)
      else {
      	//如果插入延时遥测数据失败，继续运行
          errorPrint(LOGFILE, "ERR-C-Select from %s error %d: %s.\n", "错误-C-查询%s表失败（错误编号：%d，%s）.\n",table_name_JKGL_YYZT, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
      }//else
      //get zl_zxjg
     //get yyzt end/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//填写当前节点信息
	string strSqlFetchJKGLXX = SQL_GATHER_ZQSSYC;


 	//执行到延时遥测数据表的插入操作
 	int ret = self_mysql_query(selfMysqlp, strSqlFetchJKGLXX.c_str());
     if (!ret) {
          prgPrint(LOGFILE,"PRG-C-Select from %s, affact %d rows.\n","过程-C-查询%s表，影响%d行.\n", table_name_XTXX,self_mysql_affected_rows(selfMysqlp));
          MYSQL_RES *mysql_result = NULL;

          mysql_result = self_mysql_store_result(selfMysqlp);


          if (NULL != mysql_result){
              int num_row=self_mysql_num_rows(selfMysqlp,mysql_result);
              //printf("get %d row\n",num_row);
              if (0 != num_row){


                  MYSQL_ROW mysql_row=self_mysql_fetch_row(selfMysqlp,mysql_result);

                  if (NULL == mysql_row){

                	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                	  	  errorPrint(LOGFILE,"ERR-C-Can not fetch any JKGL XTXX，NULL == mysql_row.\n","错误-C-无法取得监控管理提供的实时系统信息，NULL == mysql_row.\n");
                	  	  return -1;
                  }


                   //取出XTXXXC_KYKJ内容，如果不为NULL，则记录
                   if (mysql_row[0] == NULL){

                	   errorPrint(LOGFILE,"ERR-C-Get a null column XTXXXC_KYKJ in %s.\n","错误-C-%s表XTXXXC_KYKJ列为空.\n", table_name_XTXX);
            	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                 	  return -1;
                   }else{

                	   //gSsycStruct.zqssycData[index].kyKj = atoi(mysql_row[0]);
                	 	__uint32_t uInt32Tmp;
                	 	uInt32Tmp=atoi(mysql_row[0]);
                	 	gSsycStruct.zqssycData[index].kyKj = htonl(uInt32Tmp);
                   }

                   //取出XTXXXC_YYBFB内容，如果不为NULL，则记录
                   if (mysql_row[1] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column XTXXXC_YYBFB in %s.\n","错误-C-%s表XTXXXC_YYBFB列为空.\n", table_name_XTXX);
            	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                 	  return -1;
                   }else{
                	   /*
                	   int i;
                	   for (i=0;i<5;i++){
                		   gSsycStruct.zqssycData[index].YyBfb[i] =  mysql_row[1][i];
                	   }
                	   */
                	   gSsycStruct.zqssycData[index].YyBfb =  atof(mysql_row[1]);
                   }

                   //取出XTXXXC_CPUIDLE内容，如果不为NULL，则记录
                   if (mysql_row[2] == NULL){
                	   errorPrint(LOGFILE,"ERR-C-Get a null column XTXXXC_CPUIDLE in %s.\n","错误-C-%s表XTXXXC_CPUIDLE列为空.\n", table_name_XTXX);
            	  	  self_mysql_free_result(selfMysqlp,mysql_result);
                 	  return -1;
                   }else{
                	  // gSsycStruct.zqssycData[index].cpu = atof(mysql_row[2]);

                  	 	float fTmp;
                  	 	fTmp= atof(mysql_row[2]);
                  	    //gSsycStruct.zqssycData[index].cpu = htonl(fTmp);
                  	    gSsycStruct.zqssycData[index].cpu =htonl(__int32_t(fTmp));

                   }


              }
              self_mysql_free_result(selfMysqlp,mysql_result);
          }
     }//if (!ret)
     else {
     	//如果插入延时遥测数据失败，继续运行
         errorPrint(LOGFILE, "ERR-C-Select from %s error %d: %s.\n", "错误-C-查询%s表失败（错误编号：%d，%s）.\n",table_name_XTXX, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
     }//else

 	//填写当前秒数
 	struct timeval tv;
 	gettimeofday(&tv, NULL);

 	//gSsycStruct.zqssycData[index].dqSjMs = (long)tv.tv_sec + UTC_NTP;

 	__uint32_t uInt32Tmp;
 	uInt32Tmp=(long)tv.tv_sec + UTC_NTP;
 	gSsycStruct.zqssycData[index].dqSjMs = htonl(uInt32Tmp);



	intToWrite++;


	//改变“”另一个遥测数据包结构，主进程就会使用另一个遥测数据包结构进行socket发送
	gSsycStruct.intDataCJUsing = (index+1)%2;

	prgPrint(LOGFILE,"PRG-C-gatherZqSsycData success.\n","过程-C-采集周期实时遥测数据成功.\n");

	return 1;


}



//响应定时刷新信号，进行延时遥测数据采集
void gatherYsycData(void)
{
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.cpp FUNC: gatherYsycData is called.\n","调用-C-YC.cpp的函数: gatherYsycData is called.\n");


	int iCJZQ;
	//对每种采集周期循环
	for (iCJZQ=0;iCJZQ<YSYC_PZX_CJZQ_MAX_RANGE;iCJZQ++){

		gYsycPzxCjzqCount[iCJZQ]++;
		//如果达到采集周期


		if (gYsycPzxCjzqCount[iCJZQ] >=  gYsycPzxCjzqArray[iCJZQ]+1){
			gYsycPzxCjzqCount[iCJZQ] = 0;

			//针对遥测周期iCJZQ的延时遥测配置项进行处理，采集相关数据
			DealWtihYSYCPZX(iCJZQ);
		}


	} //for(iCJZQ=0;iCJZQ<YSYC_PZX_CJZQ_MAX_RANGE;iCJZQ++){

    //触发下次定时器
    //alarm(YSYCSJCJ_MIN_INTERVAL);
	prgPrint(LOGFILE,"PRG-C-gatherYsycData success.\n","过程-C-采集延时遥测数据成功.\n");

}

//处理指定采集周期的延时遥测配置项
int DealWtihYSYCPZX(int iCJZQ){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.cpp FUNC: DealWtihYSYCPZX is called.\n","调用-C-YC.cpp的函数: DealWtihYSYCPZX.\n");


	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = &gMysql[_YKYC];
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR-C-Can not get mysql connection for this thread.\n","错误-C-处理指定采集周期的延时遥测配置项失败，无法取得本线程的mysql句柄.\n");
		return -1;
	}

	ycpzx_t * head = gYsycPzxLinkArray[iCJZQ];
	ycpzx_t * temp = head;

	//如果数据库未连接就返回
	if(gIntIsDbConnected!=1){
		errorPrint(LOGFILE,"ERR-C-Can't Execute YSYC DB connection is go away.\n","错误-C-处理指定采集周期的延时遥测配置项失败，数据库未连接.\n");
		return -1;
	}


	//判断有没有对应这个采集周期的遥测配置项，如果没有就返回-1
	if (NULL == head->next){
		prgPrint(LOGFILE,"PRG-C-There is no YSYCPZX with given CJZQ.\n","过程-C-没有延时遥测配置项对应给定采集周期.\n");
		return -1;
	}



    int intYY_ID,intYSYCPZX_ID;

    char *charYSYCPZX_CJFF;


    while(NULL != temp->next){
    	//先删除可能存在的延时遥测数据临时文件
        const char * fileName = YCCJ_OUTFILE_NAME;
        int res = remove(fileName);
        if(0==res){
        	prgPrint(LOGFILE,"PRG-C-An existing OUTFILE %s of YCSJ is removed.\n","过程-C-删除已存在的遥测数据输出文件%s.\n",fileName);
        }else{
        	//因为是删除可能存在的临时文件，所以删除不成功并不需要报错
        	//errorPrint(LOGFILE,"ERR-C-An existing OUTFILE of YCSJ can't not be removed.\n","错误-C-无法删除可能存在的延时遥测输出文件.\n");
        }

    	//指向下一个
    	temp = temp->next;
        intYY_ID = temp->yy_id;
        intYSYCPZX_ID = temp->pzx_id;
        charYSYCPZX_CJFF = temp->cjff_sql;

        tmpPrint(LOGFILE,"TMP-C-temp->cjff_sql: %s.\n","临时-C-temp->cjff_sql: %s.\n", temp->cjff_sql);


        tmpPrint(LOGFILE,"TMP---It is time to gather Ysyc data with yy_id = %d, ysycpzx_id = %d, cjzq = %d (s).\n","临时---即将采集延时遥测数据 yy_id = %d, ysycpzx_id = %d, cjzq = %d (秒).\n",intYY_ID,intYSYCPZX_ID,gYsycPzxCjzqArray[iCJZQ]);

        //去掉sql语句最后可能会有的分号
        string strSql = string(temp->cjff_sql);
        if (';' == strSql[strSql.length()-1]){
        	strSql = strSql.substr(0,strSql.length()-1);
        }

        //将写入数据库的附加语句添加在遥测量采集sql后面
        strSql.append(string(SELECTINTO));

        //执行采集指令
    	sqlPrint(LOGFILE,"SQL-C-Execute YSYC sql: %s.\n", "SQL-C-采集延时遥测数据SQLl: %s.\n", strSql.c_str());
    	//tmpPrint(LOGFILE,"SQL-C-Execute YSYC sql: %s.\n", "SQL-C-采集延时遥测数据SQLl: %s.\n", strSql.c_str());
    	//执行遥测量采集
    	int ret = self_mysql_query(selfMysqlp, strSql.c_str());

    	//如果成功，则将导出的文件压缩，并作为一个字段写入延时遥测数据表
        if (!ret) {
             prgPrint(LOGFILE,"PRG-C-Execute YSYC sql, affact %d rows.\n","过程-C-采集延时遥测数据 ，影响%d行.\n",
                     self_mysql_affected_rows(selfMysqlp));

             //todo
             //执行一个压缩脚本


             //读出延时遥测数据文件
             //char * fileBuffer = NULL;//[SQL_OUTFILE_MAXSIZE];
             char  fileBuffer[SQL_OUTFILE_MAXSIZE];
             bzero(fileBuffer,SQL_OUTFILE_MAXSIZE);

             ifstream in(fileName, ios::in|ios::binary|ios::ate);
             long fileSize = in.tellg();
             in.seekg (0, ios::beg);
             //fileBuffer = new char [fileSize];
             in.read (fileBuffer, fileSize);
             in.close();

             tmpPrint(LOGFILE,"TMP-C-fileBuffer: %s.\n","临时-C-fileBuffer: %s.\n",fileBuffer);

             char to[SQL_OUTFILE_MAXSIZE_WITH_ESCAPE];
             bzero(to,SQL_OUTFILE_MAXSIZE_WITH_ESCAPE);
             unsigned long len;

             tmpPrint(LOGFILE,"TMP-C-fileSize: %ld.\n","临时-C-延时遥测数据文件大小: %ld.\n",fileSize);
             tmpPrint(LOGFILE,"TMP-C-strlen(fileBuffer): %zu.\n","临时-C-strlen(fileBuffer): %zu.\n",strlen(fileBuffer));
             len = mysql_real_escape_string(&selfMysqlp->mysql, to, fileBuffer, strlen(fileBuffer));
             to[len] = '\0';
             tmpPrint(LOGFILE,"TMP-C-Escaped string is: \"%s\" (%lu).\n","临时-C-Escaped string is: \"%s\" (%lu).\n", to, len);

             tmpPrint(LOGFILE,"TMP-C-len after escape: %lu.\n","临时-C-escape后的长度: %lu.\n",len);
             tmpPrint(LOGFILE,"TMP-C-strlen(to): %zu.\n","临时-C-strlen(to): %zu.\n",strlen(to));


             //执行一个删除脚本，删除延时遥测数据文件
             int res = remove(fileName);
             if(0==res){
            	 prgPrint(LOGFILE,"PRG-C-OUTFILE of YCSJ is removed.\n","过程-C-删除延时遥测导出文件成功.\n");
             }else{
            	 errorPrint(LOGFILE,"ERR-C-OUTFILE of YCSJ can't not be removed.\n","错误-C-删除延时遥测导出文件失败.\n");
             }

             //向延时遥测数据表插入记录/////////////////////////////////////////
         	//构造insert语句
         	string strInsertSql = "insert into " +
         			string(table_name_YC_YSYCSJ) +
         			"( YY_ID, YSYCPZX_ID,YSYCSJ_CJSJ,YSYCSJ_CJNR) " +
         			" values(" +
         			int2String(intYY_ID) +
         			", " +
         			int2String(intYSYCPZX_ID)+
         			", \'" +
         			getDateString() +
         			"\', \'" +
         			to +
         			"\');";

         	sqlPrint(LOGFILE,"SQL-C-Insert into %s: %s .\n","SQL-C-插入%s表SQL: %s .\n",table_name_YC_YSYCSJ, strInsertSql.c_str());

         	//执行到延时遥测数据表的插入操作
         	int ret = self_mysql_query(selfMysqlp, strInsertSql.c_str());
             if (!ret) {

                  //PYT//prgPrint(LOGFILE,"PRG-C-Insert into %s, affact %d rows.\n","过程-C-插入%s表 ，影响%d行.\n", table_name_YC_YSYCSJ,self_mysql_affected_rows(selfMysqlp));

             }//if (!ret)
             else {
             	//如果插入延时遥测数据失败，继续运行
                 errorPrint(LOGFILE, "ERR-C-Insert into %s error %d: %s.\n", "错误-C-插入%s表失败（错误编号：%d，%s）.\n",table_name_YC_YSYCSJ, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
             }//else

             //////////////////////////////////////////////////////////////

             //释放fileBuffer，
             //if(NULL != fileBuffer)
            //	 free(fileBuffer);
         } else {
        	 //如果执行延时遥测采集失败，则继续循环
             errorPrint(LOGFILE, "ERR-C-Execute YSYC sql error %d: %s.\n", "错误-C-执行延时遥测采集SQL失败（错误编号：%d，%s）.\n",  self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
             //移动到下一项
             //temp = temp->next;
             continue;
         }


    	//处理完一个遥测配置项采集，移动到配置项链表的下一项
    	//temp = temp->next;
    }
    prgPrint(LOGFILE,"PRG-C-DealWtihYSYCPZX with given CJZQ = %d success.\n","过程-C-处理采集周期为%d的延时遥测配置项成功.\n",iCJZQ);
    return 1;
}

//读取数据库，更新每个采集周期的延时遥测配置项列表
int getYSYCPZX(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.cpp FUNC: getYSYCPZX is called.\n","调用-C-YC.cpp的函数: getYSYCPZX.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = &gMysql[_YKYC];
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR-C-Can not get mysql connection for this thread.\n","错误-C-无法取得本线程的mysql句柄.\n");
		return -1;
	}

	int iCJZQ;
	//对每种采集周期循环
	for (iCJZQ=0;iCJZQ<YSYC_PZX_CJZQ_MAX_RANGE;iCJZQ++){
		//清理链表
		cleanYCPZCList(gYsycPzxLinkArray[iCJZQ]);
		//读取延时遥测配置项表中，采集周期为i的所有配置项，加入链表ysycxPtrArray[iCJZQ]

		//查询那些有效的，采集周期为i的配置项
		string strSql = "select YY_ID, YSYCPZX_ID, YSYCPZX_CJFF from " +
				string(table_name_YC_YSYCPZX) +
				" where YSYCPZX_SFYX = 1" +
				" and YSYCPZX_CJZQ =" +
				int2String(iCJZQ) +
				";";

		sqlPrint(LOGFILE,"SQL-C-Select from %s: %s .\n","SQL-C-查询%s表: %s .\n",table_name_YC_YSYCPZX, strSql.c_str());

		int ret = self_mysql_query(selfMysqlp, strSql.c_str());
	    if (!ret) {

	         prgPrint(LOGFILE,"PRG-C-Select from %s, affact %d rows.\n","过程-C-查询%s表 ，影响%d行.\n", table_name_YC_YSYCPZX,
	        		 self_mysql_affected_rows(selfMysqlp));

	         MYSQL_RES *mysql_result = self_mysql_store_result(selfMysqlp);

	         int num_row = 0;
	         //如果取到结果集就取行数
	         if (NULL != mysql_result){
	             num_row = self_mysql_num_rows(selfMysqlp,mysql_result);
	             dataPrint(LOGFILE,"DAT-C-Select %d YSYCPZX with CJZQ = %d.\n","数据-C-查询到%d条采集周期为%d的延时遥测配置项.\n",num_row,iCJZQ);
	         }else{
	        	 tmpPrint(LOGFILE,"TMP-C-Tmysql_result is null while select from YSYCPZX with CJZQ = %d.\n","临时-C-没有查询到采集周期为%d的延时遥测配置项.\n",iCJZQ);
	        	 continue;
	         }

	         //如果行数为0就释放记录并查询采集周期为i+1的遥测配置项
	         if(0 == num_row){
	             tmpPrint(LOGFILE,"TMP-C-There is no YSYCPZX with CJZQ = %d.\n","临时-C-查询到0条采集周期为%d的延时遥测配置项.\n",iCJZQ);
	             self_mysql_free_result(selfMysqlp,mysql_result);
	        	 continue;
	         }


	         int intYY_ID,intYSYCPZX_ID;
	         string strYSYCPZX_CJFF = "";
	         char charYSYCPZX_CJFF[YSYC_CJFF_SQL_MAX_LENGTH];
	         bzero(charYSYCPZX_CJFF,YSYC_CJFF_SQL_MAX_LENGTH);

	         //逐条读取遥测配置项并加入链表数组对应项
	         MYSQL_ROW mysql_row = NULL;
	         while((mysql_row = self_mysql_fetch_row(selfMysqlp,mysql_result))){
	        	 tmpPrint(LOGFILE,"TMP-C-Fetch a row.\n","临时-C-取到一条记录.\n");

		         if (mysql_row[0] != NULL)
		        	 intYY_ID = atoi(mysql_row[0]);
		         if (mysql_row[1] != NULL)
		        	 intYSYCPZX_ID = atoi(mysql_row[1]);


		         if (mysql_row[2] != NULL){

		        	 strYSYCPZX_CJFF = mysql_row[2];

		        	 //tmpPrint(LOGFILE,"TMP-C-intYY_ID: %d, intYSYCPZX_ID: %d, strYSYCPZX_CJFF: %s .\n","临时-C-intYY_ID: %d, intYSYCPZX_ID: %d, strYSYCPZX_CJFF: %s .\n",intYY_ID,intYSYCPZX_ID,strYSYCPZX_CJFF.c_str());

		             //将采集方法由字符串转换为unsigned char数组
		             int lengthOfstrYSYCPZX_CJFF = strYSYCPZX_CJFF.length();
		             int j;
		             for (j=0;j<lengthOfstrYSYCPZX_CJFF;j++){
		            	 charYSYCPZX_CJFF[j] =  strYSYCPZX_CJFF.c_str()[j];
		             }//for

		        	 //tmpPrint(LOGFILE,"TMP-C-intYY_ID: %d, intYSYCPZX_ID: %d, charYSYCPZX_CJFF: %s .\n","临时-C-intYY_ID: %d, intYSYCPZX_ID: %d, charYSYCPZX_CJFF: %s .\n",intYY_ID,intYSYCPZX_ID,charYSYCPZX_CJFF);

		         }//if
		         else{

		    		 //如果采集方法为空就处理下一条记录
		             tmpPrint(LOGFILE,"TMP-C-Get a NULL YSYCPZX_CJFF.\n","临时-C-延时遥测配置项采集方法为空，处理下一条.\n");
		             continue;

		         }//if (mysql_row[2] != NULL)

		         //构造一个延时遥测配置项
		         ycpzx_t * pYCPZX;

		         //newYCPZX(pYCPZX, intYY_ID,intYSYCPZX_ID,iCJZQ,charYSYCPZX_CJFF);
		         pYCPZX = newYCPZX(intYY_ID,intYSYCPZX_ID,iCJZQ,charYSYCPZX_CJFF);

		         tmpPrint(LOGFILE,"TMP-C-pYCPZX->cjff_sql = %s .\n","临时-C-pYCPZX->cjff_sql = %s .\n", pYCPZX->cjff_sql);

		         //插入遥测周期为iCJZQ的遥测项链表
		         insertYCPZX(gYsycPzxLinkArray[iCJZQ],pYCPZX);


	         }//while(mysql_row = self_mysql_fetch_row(result)){

	         //释放此次查询记录
	         self_mysql_free_result(selfMysqlp,mysql_result);

	    }//if (!ret)
	    else {
	    	//如果查询遥测周期为iCJZQ的遥测配置项失败，继续查询下一个周期的遥测配置项
            errorPrint(LOGFILE, "ERR-C-Select from %s error %d: %s.\n", "错误-C-查询%s表失败（错误编号：%d，%s）.\n",table_name_YC_YSYCPZX, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
            continue;
	    }//else

	} //for(iCJZQ=0;iCJZQ<YSYC_PZX_CJZQ_MAX_RANGE;iCJZQ++){
    prgPrint(LOGFILE,"PRG-C-getYSYCPZX success.\n","过程-C-成功读取数据库，更新每个采集周期的延时遥测配置项列表.\n");
	return 1;
}

//将实时遥测历史数据保存到数据库
void storeSSycLsSj(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-C-YC.cpp FUNC: storeSSycLsSj is called.\n","调用-C-YC.cpp的函数: storeSSycLsSj.\n");

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = &gMysql[_YKYC];
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR-C-Can not get mysql connection for this thread.\n","错误-C-保存实时遥测历史数据失败，无法取得本线程的mysql句柄.\n");
		return ;
	}

	//每个实时遥测数据结构的大小的sizeof(ssyc_t)，为了对其中的字符进行转意，并在末尾加入一个\0，所以需要申请一个2倍+1的空间
    char to[sizeof(zqssyc_t)*2+1];
    bzero(to,sizeof(zqssyc_t)*2+1);
    unsigned long len;

    //每次只采第一组实时遥测数据结构gSsycStruct.ssycData[0],只采sizeof(ssyc_head_t)字节之后的内容，ssyc_t的前面sizeof(ssyc_head_t)字节是遥测头
    len = mysql_real_escape_string(&selfMysqlp->mysql, (char*)to, (char*)(&gSsycStruct.zqssycData[0])+sizeof(ssyc_head_t), sizeof(zqssyc_t)-sizeof(ssyc_head_t));
    to[len] = '\0';
    tmpPrint(LOGFILE,"TMP-C-Escaped gSsycStruct.ssycData[0] is: \"%s\" (%lu).\n","临时-C-Escaped gSsycStruct.ssycData[0] is: \"%s\" (%lu).\n", to, len);


    //向GJFW_YC_SSYCLSSJ表插入记录/////////////////////////////////////////
	//构造insert语句
	string strInsertSql = "insert into " +
			string(table_name_YC_SSYCLSSJ) +
			"( SSYCSJ_CJSJ, SSYCSJ_CJNR) " +
			" values(\'" +
			getDateString() +
			"\','" +
			to +
			"\');";

	sqlPrint(LOGFILE,"SQL-C-Insert into %s: %s .\n","SQL-C-插入%s表SQL: %s .\n",table_name_YC_SSYCLSSJ, strInsertSql.c_str());

	//执行到GJFW_YC_SSYCLSSJ表的插入操作
	int ret = self_mysql_query(selfMysqlp, strInsertSql.c_str());
    if (!ret) {

         //PYT//prgPrint(LOGFILE,"PRG-C-Insert into %s, affact %d rows.\n","过程-C-插入%s表 ，影响%d行.\n", table_name_YC_SSYCLSSJ, self_mysql_affected_rows(selfMysqlp));

    }//if (!ret)
    else {
    	//如果插入延时遥测数据失败，继续运行
        errorPrint(LOGFILE, "ERR-C-Insert into %s error %d: %s.\n","错误-C-插入%s表失败（错误编号：%d，%s）.\n",table_name_YC_SSYCLSSJ, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
    }//else

    prgPrint(LOGFILE,"MSG-C-storeSSycLsSj success.\n","过程-C-实时遥测历史数据保存成功.\n");
}
