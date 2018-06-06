/*
 * YK.cpp
 *
 *  Created on: 2017年5月11日
 *      Author: pyt64
 */



#include "YK.h"

#ifndef _RUN_ON_XNWX
//void* main_loop_of_YK(void *arg){
int resolveZl(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-K-+YK.c FUNC: main_loop_of_YK is called.\n","调用-K-YK.c的函数: main_loop_of_YK.\n");

	threadPrint_YK(LOGFILE,"Thr-K-main_loop_of_YK: thread is started successfully.\n","线程-K-遥控线程启动成功.\n");
	int subpid = getpid();
	threadPrint_YK(LOGFILE,"Thr-K-Pid of YK thread is %d.\n","线程-K-遥控线程Pid：%d.\n",subpid);


	//记录YK线程pt
	gTotal.YkPt = pthread_self();

	//gSubtid_1 = pthread_self();
	//threadPrint_YK("Thr-K-Tid of YK thread is %lu.\n",gSubtid_1);

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = &gMysql[_YK];
	/*
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR-K-Can not get mysql connection for this thread.\n","错误-K-无法取得本线程的mysql句柄.\n");

		int *pRet =  (int*)malloc(sizeof(int));
		*pRet = -1;

		msgPrint(LOGFILE,"MSG-K-main_loop_of_YK is going to close.\n","消息-K-遥控线程即将关闭.\n");

		pthread_exit((void*)pRet);
	}
	*/

	int lenOfIns;
    __uint8_t  zlData[ZL_MAX_LENGTH];
    __uint8_t  tmpData[BUFFER_SIZE]; //在清理buffer无用数据时，用于读取popBuffer返回的值



//	while(gIntIsRun){

		//printYK("YK thread begain while.\n");

    	//遥控线程保活计数器+1
    	gIntNewStatYKThread++;


		lenOfIns = 0;
	    bzero(zlData,ZL_MAX_LENGTH);

#ifdef _CONNECT_TO_SERVER

		//////////////////
		//取指令
		//////////////////
		//printBuffer(&gSocketBuffer);


		//定位同步头
		int indexOfSyncHeader = findBuffer(&gSocketBuffer,gSYNC_HEADER,LEN_OF_TCP_SYNC);
        if (indexOfSyncHeader<0){
        	//printYK("YK thread not find Sync Header.\n");
        	//continue;
        	return 0;
        }

	    tmpPrint(LOGFILE,"TMP-K-Find Sync Head from socketBuffer.\n","临时-K-在socketBuffer中发现同步头.\n");
	    //DEBUG_STOP

        //丢掉同步头之前的内容
        //popBuffer(&gSocketBuffer,tmpData,indexOfSyncHeader +4 -1);
        //取指令长度
        __uint8_t b[LEN_OF_TCP_LENGTH];
        //popBuffer(&gSocketBuffer,b,4);
        int retOfRead;
        retOfRead = readBuffer(&gSocketBuffer,b,LEN_OF_TCP_LENGTH,indexOfSyncHeader+LEN_OF_TCP_SYNC);

        //如果头中长度部分还没来全就循环等待
        if(-1 == retOfRead){
        	prgPrint(LOGFILE,"PRG-K-Buffer is not ready for read out all TCP Head long = %d.\n","过程-K-数据未到全，暂无法按TCP应用头指定的长度%d读取.\n",LEN_OF_TCP_LENGTH);
        	printYK("YK thread wait for the length part of  ZL.\n");
        	//continue;
        	return 0;
        }

        //dataPrint(LOGFILE,"DAT-K-Get a SyncHeader at %d\n",indexOfSyncHeader);

        lenOfIns = getLengthOfData(b[0],b[1],b[2],b[3]);

        dataPrint(LOGFILE,"DAT-K-b[0],b[1],b[2],b[3] is %02X,%02X,%02X,%02X, lenOfIns is %d\n","数据-K-b[0],b[1],b[2],b[3]为 %02X,%02X,%02X,%02X, 长度为 %d\n",b[0],b[1],b[2],b[3],lenOfIns);

        //如果lenOfIns正常，则取指令，否则指令错误，清除缓冲所有内容
        if (lenOfIns > 0) {

        	 retOfRead = readBuffer(&gSocketBuffer,zlData,lenOfIns,(indexOfSyncHeader+LEN_OF_TCP_HEAD)%BUFFER_SIZE);

        	 //如果没有读出lenOfIns这么多（后续的内容还没有来全）
        	 if(-1 == retOfRead){
        		 prgPrint(LOGFILE,"PRG-K-Buffer is not ready for read out all Ins long = %d.\n","过程-K-数据未到全，暂无法按指定指令长度%d读出.\n",lenOfIns);
        		 printYK("YK thread wait for the compliment ZL.\n");
        		 //continue;
        		 return 0;

        	 }

        	 //为避免等待后续socket数据的时候反复输出，这两行挪到这里
             dataPrint(LOGFILE,"DAT-K-Get a SyncHeader at %d\n","数据-K-定位到同步头，位置：%d.\n",indexOfSyncHeader);
             dataPrint(LOGFILE,"DAT-K-b[0],b[1],b[2],b[3] is %02X,%02X,%02X,%02X, lenOfIns is %d\n","数据-K-b[0],b[1],b[2],b[3]为 %02X,%02X,%02X,%02X, 长度为 %d\n",b[0],b[1],b[2],b[3],lenOfIns);

        	 /*
        	 //打印输出的部分
         	dataPrint(LOGFILE,"***********zlData BEGIN**********\n");
         	int k;
         	for (k=0;k<lenOfIns;k++){
         		normalPrint(LOGFILE,"%02X ",zlData[k]);
         	}
         	dataPrint(LOGFILE,"\n");
         	*/
        	 //dataPrint(LOGFILE,"++++++++++++++++Before clean+++++++++++++++++++++\n");
        	 //printBuffer(&gSocketBuffer);

        	 /*
             //去掉已经处理的部分
         	int rett = cleanBuffer2Index(&gSocketBuffer,(indexOfSyncHeader+LEN_OF_TCP_HEAD+lenOfIns)%BUFFER_SIZE);
            if(rett == -1){
           		errorPrint(LOGFILE,"ERR-K-cleanBuffer2Index return -1.\n");
           		DEBUG_STOP
            }
            */


             //取pop之前的长度
             printYK("------------------------YK thread got a  ZL. \n");
             printYK("Begin call lengthOfBuffer.\n");
             int length1 = lengthOfBuffer(&gSocketBuffer);
             printYK("YK thread return from the calling of  lengthOfBuffer.\n");

             //令人疑惑的注解/////////////////////////////////////////////////
             //——//以下为错误代码，错误原因是如果值indexOfSyncHeader+LEN_OF_TCP_HEAD+lenOfIns超过BUFFER_SIZE，它就并不是要清除的缓冲的长度，
             //分析可能当时indexOfSyncHeader取的是绝对位置，而非buffer中相对0的位置，所以indexOfSyncHeader+LEN_OF_TCP_HEAD+lenOfIns可能会超过BUFFER_SIZE
             //现在indexOfSyncHeader是相对位置，在前面readBuffer执行成功的基础上，不会出现indexOfSyncHeader+LEN_OF_TCP_HEAD+lenOfIns超过BUFFER_SIZE的现象。
             /////////////////////////////////////////////////////////////////


             int rett = popBuffer(&gSocketBuffer,tmpData,(indexOfSyncHeader+LEN_OF_TCP_HEAD+lenOfIns));

         	//如果弹出gSocketBuffer出现错误则退出YK循环
             if(rett == -1){
         		errorPrint(LOGFILE,"ERR-K-Pop gSocketBuffer error, There is no enough data.\n","错误-K-从gSocketBuffer弹出数据错误，长度不足.\n");
         		//break;
         		return -1;

             }

             //////////////////////////////////////////
             //字节统计
             //////////////////////////////////////////

             gTotal.sizeOfPopBuffer +=  (indexOfSyncHeader+LEN_OF_TCP_HEAD+lenOfIns);

             if(indexOfSyncHeader>0){
                 //统计重新同步而丢弃的字节数
                 gTotal.sizeOfDiscardWhileRelocateSync  += indexOfSyncHeader;
                 prgPrint(LOGFILE,"PRG-K-YK discard %d bytes (total %lu bytes) data from socket buffer, because relocate sycn in received socket flow.\n",
                		 "过程-K-因定位同步头位置，遥控线程丢弃%d bytes(共丢弃%lu bytes)数据.\n",
                		 indexOfSyncHeader,gTotal.sizeOfDiscardWhileRelocateSync);
             }
             //统计解析为指令的字节数，包括TCP同步头20字节
             gTotal.sizeOfPackageResolved += (LEN_OF_TCP_HEAD+lenOfIns);
             prgPrint(LOGFILE,"PRG-K-YK resolve %d ZL bytes (total %lu ZL bytes) data from socket buffer.\n",
            		 "过程-K-遥控线程解析%d bytes指令数据 (共解析%lu bytes).\n",
            		 (LEN_OF_TCP_HEAD+lenOfIns),gTotal.sizeOfPackageResolved);
             //统计解析处的指令数
             gTotal.packageResolved ++;
             prgPrint(LOGFILE,"PRG-K-YK resolve 1 ZL (total %lu ZL) data from socket buffer.\n",
            		 "过程-K-遥控线程解析出1条指令 (共解析出%lu条指令).\n",
            		 gTotal.packageResolved);

             printYK("YK thread finish counting.\n");

             //已经前移，此处不要了
             /*
             if(rett == -1){
            	 errorPrint(LOGFILE,"ERR-K-popBuffer return -1.\n");
            	 //DEBUG_STOP
             }
             */

             printYK("YK thread another call of lengthOfBuffer.\n");
             int length2 = lengthOfBuffer(&gSocketBuffer);
             dataPrint(LOGFILE,"DAT-K-Pop %d bytes from gSocketBuffer.\n","数据-K-遥控线程弹出%d bytes数据.\n",length1-length2);



            //printBuffer(&gSocketBuffer);




        }else{
        	printYK("YK thread ERR ZL length lenOfIns = %d. \n",lenOfIns);
        	int length = lengthOfBuffer(&gSocketBuffer);
        	int ret =popBuffer(&gSocketBuffer,tmpData,length);

        	//如果清除gSocketBuffer出现错误则退出YK循环
        	if(ret == -1){
        		errorPrint(LOGFILE,"ERR-K-Empty gSocketBuffer error, There is no enough data.\n","错误-K-清空gSocketBuffer缓冲错误, 没有足够数据.\n");
        		//break;
        		return -1;
        	}


        	//////////////////////////////////////////
        	//字节统计
        	//////////////////////////////////////////
        	gTotal.sizeOfPopBuffer +=  length;
            if(indexOfSyncHeader>0){
                //统计重新同步而丢弃的字节数
                gTotal.sizeOfDiscardWhileRelocateSync  += indexOfSyncHeader;
                prgPrint(LOGFILE,"PRG-K-YK discard %d bytes (total %lu bytes) data from socket buffer, because relocate sycn in received socket flow.\n",
                		"过程-K-因定位同步头位置，遥控线程丢弃%d bytes(共丢弃%lu bytes)数据.\n",
                		indexOfSyncHeader,gTotal.sizeOfDiscardWhileRelocateSync);
            }
        	//统计因指令长度错误，清楚的接收缓冲大小
        	int size = length - indexOfSyncHeader;//indexOfSyncHeader之前的数据是因为重新同步而丢弃的
        	gTotal.sizeOfDiscardWhileInsLengthError += size;
        	prgPrint(LOGFILE,"PRG-K-YK discard %d bytes (total %lu bytes) data from socket buffer, becase length of Ins error.\n",
        			"过程-K-因指令长度错误，遥控线程丢弃%d bytes (共丢弃%lu bytes)数据.\n",
        			size,gTotal.sizeOfDiscardWhileInsLengthError);

        	//continue;
        	return 0;
        }
#else
#ifdef _INSERTZLMANUALLY
        //否则如果没有链接到集成单元，则使用示例数据zlData[]
        lenOfIns = 8;

        //设备ID
        zlData[0] = DEVICE_ID;

        zlData[1] = YYID_CKFW;//测控服务应用ID
        //zlData[1] = YYID_GBDZ; //广播应用ID

        //指令类型
        zlData[2] = ZLLX_JKGL;//监控及管理指令
        //zlData[2] = ZLLX_KZZL;//扩展指令类型

        //指令编号
        //zlData[3] = ZLBH_JKGL_YYSJQL; //应用数据清理
        //zlData[3] = ZLBH_JKGL_JDYGJ; //预关机
        zlData[3] = ZLBH_JKGL_ZRSJSYTZ; //注入数据使用通知
        //zlData[3] = ZLBH_KZZL_PZ; //配置指令

        //执行结果
        zlData[4] = ZXJG_WD;

        //指令内容
        zlData[5] = 0x00;
        zlData[6] = 0x00;
        zlData[7] = 0x00;
#endif //#ifdef _INSERTZLMANUALLY
#endif //#ifdef _CONNECT_TO_SERVER

        /////////////////
        //指令入库
        /////////////////

        printYK("YK thread begin to insert ZL to db.\n");

        //解析指令////////////////////////////////
        struct_ZLFrameHeader_t *pzlFrameHeader;
    	pzlFrameHeader = (struct_ZLFrameHeader_t*)zlData;

        //如果设备ID不对则忽略该指令
        if (gDeviceId != pzlFrameHeader->sbid){
        	prgPrint(LOGFILE,"PRG-K-Get a Zl with error SBID %d\n","过程-K-遥控线程取得的指令设备ID错误：%d\n",pzlFrameHeader->sbid);
        	//DEBUG_STOP

        	printYK("YK thread ZL SBID wrong\n");
        	/////////////////////////////////////////
            //统计解析处的指令数
            gTotal.packageDiscardForSBIDError ++;
            prgPrint(LOGFILE,"PRG-K-YK discard 1 ZL (total %lu ZL) because of error SBID.\n","过程-K-因设备ID错误，遥控线程丢弃1条指令 (共丢弃%lu条指令).\n",gTotal.packageDiscardForSBIDError);

        	//continue;
            return 0;
        }

        //printf("YYID=%d, ZZLX=%d,ZLBH=%d\n",pzlFrameHeader->yyid,pzlFrameHeader->zllx,pzlFrameHeader->zlbh);

        //reset gZLConnectionTimeOut whatever recv
        gZLConnectionTimeOut = MAX_ZL_CONNECT_TIMEOUT;

        //如果取得heart beat指令
        if (ZLLX_XT == pzlFrameHeader->zllx){
        	prgPrint(LOGFILE,"PRG-K-Get a heart beat Zl\n","过程-K-遥控线程取得heart beat指令\n");
        	//DEBUG_STOP
        	printYK("YK thread got a heart beat ZL\n");
        	gZLConnectionTimeOut = MAX_ZL_CONNECT_TIMEOUT;

        	/////////////////////////////////////////
        	//字数统计
        	/////////////////////////////////////////
            //统计解析处的指令数
            gTotal.packageHeartBeat ++;
            prgPrint(LOGFILE,"PRG-K-YK recv 1 heart beat ZL (total %lu ZL) .\n","过程-K-遥控线程取得1条heart beat指令 (共取得%lu条heart beat指令).\n",gTotal.packageHeartBeat);

        	//continue;
            return 0;
        }




        //取指令内容并做脱字处理/////////////////////////////////
        unsigned char zlnr[507];
        bzero(zlnr,507);
        int zlnrLength = lenOfIns - ZL_HEAD_LENGTH;
        int i;
        for (i=0;i<zlnrLength;i++)
        	zlnr[i] = zlData[i+ZL_HEAD_LENGTH];

        char to[1024]; //>501*2+1
        bzero(to,1024);
        unsigned long len;

        len = mysql_real_escape_string(&selfMysqlp->mysql, to, (char *)zlnr, 507);
        to[len] = '\0';
        tmpPrint(LOGFILE,"TMP-K-Escaped string is: \"%s\" (%lu)\n","临时-K-Escaped string为: \"%s\" (%lu)\n", to, len);

        tmpPrint(LOGFILE,"TMP-K-len after escape: %lu\n","临时-K-调用escape后的长度: %lu.\n",len);
        tmpPrint(LOGFILE,"TMP-K-strlen(to): %zu\n","临时-K-strlen(to): %zu.\n",strlen(to));

        dataPrint(LOGFILE,"DAT-K-ZLNR to Insert is:%s\n","数据-K-待入库的指令内容：%s.\n",zlnr);

        printYK("YK thread Escaped string of  ZL_NR\n");

        //构造sql
    	string strZLNR((char *)to);
    	//tmpPrint ("yyid is %d \n",pzlFrameHeader->yyid);
    	//tmpPrint ("yyid is %d \n",0xFF);
    	string strInsertZL = "insert into " +
    			string(table_name_YK_ZL) +
    			"(YY_ID,ZL_LX,ZL_BH,ZL_JSSJ,ZL_ZXJG,ZL_NR) values(" +
    			int2String(pzlFrameHeader->yyid) +
    			","+
    			int2String(pzlFrameHeader->zllx) +
    			","+
    			int2String(pzlFrameHeader->zlbh) +
    			","+
    			"'"+getDateString()+"'" +
    			","+
    			int2String(ZXJG_WD)+
    			","
    			"'"+strZLNR+"'" +
    			")";


    	sqlPrint(LOGFILE,"SQL-K-Insert table %s: %s\n","SQL-K-插入%s表SQL: %s\n",table_name_YK_ZL, strInsertZL.c_str());
    	printYK("YK thread ZL SQL:%s\n", strInsertZL.c_str());

    	//指令入库
    	//如果数据库未连接就返回
    	if(gIntIsDbConnected != 1 || NULL == selfMysqlp ){
    		printYK("YK thread db  is not connected\n");
    		//continue;
    		return 0;
    	}


    	int ret;
    	//尝试三次操作数据库，如果都失败，就认了。
    	int count;
    	for(count=0;count<3;count++){
    		printYK("YK thread insert db begin.\n");
    		ret = self_mysql_query(selfMysqlp, strInsertZL.c_str());
    		//ret = self_mysql_real_query(selfMysqlp, strInsertZL.c_str(),strInsertZL.length());
            if (!ret) {
            	printYK("YK thread insert db ok.\n");
            	break;
            }else{
            	printYK("YK thread insert db try again.\n");
            	sleep(1);
            }
    	}



        if (!ret) {

             prgPrint(LOGFILE,"PRG-K-Insert into %s, affact %d rows.\n","过程-K-插入%s表 ，影响%d行.\n",table_name_YK_ZL,
                     self_mysql_affected_rows(selfMysqlp));
             //z指令计数增1
             gZlCount++;
             //最近指令类型
             gZlLx = pzlFrameHeader->zllx;
             //最近指令编号
             gZlBh = pzlFrameHeader->zlbh;
             //最近指令的YY_ID
             gZlYyId = pzlFrameHeader->yyid;

         	/////////////////////////////////////////
         	//字数统计
         	/////////////////////////////////////////
             //统计成功入库的指令数
             gTotal.packageInsertedToDB ++;
             prgPrint(LOGFILE,"PRG-K-YK Insert 1 ZL (total %lu ZL) to DB.\n","过程-K-遥控线程入库1条指令 (共%lu条指令).\n",gTotal.packageInsertedToDB);

         } else {
             errorPrint(LOGFILE, "ERR-K-Insert into %s error %d: %s\n", "错误-K-插入%s表失败（错误编号：%d，%s）\n", table_name_YK_ZL, self_mysql_errno(selfMysqlp), self_mysql_error(selfMysqlp));
          	/////////////////////////////////////////
          	//字数统计
          	/////////////////////////////////////////
            //统计入库失败的指令数
            gTotal.packageFailToInsertedToDB ++;
            prgPrint(LOGFILE,"PRG-K-YK Fail to Insert 1 ZL (total %lu ZL) to DB.：\n","过程-K-遥控线程入库失败1条指令 (共%lu条指令).\n",gTotal.packageFailToInsertedToDB);

            printYK("YK thread insert db fail, dell with other ZL.\n");
             //continue;
            return 0;
         }

        printYK("+++++++++++++++++YK thread finish inserting ZL to db.\n");
        /////////////////
        //Debug指令读取显示插入的指令
        /////////////////
#ifdef _READZLAFTERINSERT
    	if(gIntIsDbConnected == 1){

            string strSelectZL = "select YY_ID,ZL_LX,ZL_BH,ZL_NR from " + string(table_name_YK_ZL);
            if(self_mysql_query(selfMysqlp,strSelectZL.c_str())==0)
            {
                  prgPrint(LOGFILE,"PRG-K-Debug query table %s success.\n","过程-K-Debug query table %s success.\n",table_name_YK_ZL);
                  MYSQL_RES *mysql_result = NULL;

                  mysql_result = self_mysql_store_result(selfMysqlp);


                  if (NULL != mysql_result){
                      int num_row=self_mysql_num_rows(mysql_result);
                      if (0 != num_row){

                          MYSQL_ROW mysql_row=self_mysql_fetch_row(mysql_result);

                           //取出内容，如果不为NULL，则输出
                           if (mysql_row[0] == NULL){
                         	  tmpPrint(LOGFILE,"TMP-K-Get a null col.\n","临时-K-Get a null col.\n");
                           }else{

                        	   string strYY_ID = mysql_row[0];
                        	   string strZL_LX = mysql_row[1];
                        	   string strZL_BH = mysql_row[2];

                        	   dataPrint(LOGFILE,"DAT-K-Debug read a ZL with YY_ID = %s, ZL_LX = %s, ZL_BH = %s.\n",
                        			   "数据-K-Debug read a ZL with YY_ID = %s, ZL_LX = %s, ZL_BH = %s.\n",
                        			   strYY_ID.c_str(),strZL_LX.c_str(), strZL_BH.c_str());



                        	   //如果mysql_row[3] == NULL，就不能用它初始化string类型，所以改为先判后读到string
                         	   if (mysql_row[3] != NULL){
                         		   	   string strNR = mysql_row[3];

                            		   dataPrint(LOGFILE,"DAT-K-ZL_NR in string is %s\n","数据-K-ZL_NR in string is %s\n",strNR.c_str());

                                   	   int intLengthOfstrTmp = strNR.length();

                                	   int j;
                                       unsigned char charTmp[ZL_MAX_LENGTH];
                                       for (j=0;j<intLengthOfstrTmp;j++){
                                     	  charTmp[j] =  strNR.c_str()[j];
                                     	  dataPrint(LOGFILE,"DAT-K-NR in ZL_DATA[%d] in number is %02X\n","数据-K-NR in ZL_DATA[%d] in number is %02X\n",j+ZL_HEAD_LENGTH,zlData[j+ZL_HEAD_LENGTH]);
                                     	  /*
                                     	  //逐字节显示写入的指令内容和读出的指令内容，以及读出的指令内容是否等于zlData中的原始数据
                                     	  dataPrint(LOGFILE,"DAT-K-ZL_NR[%d] inserted in number is %02X\n",j,zlnr[j]);
                                     	  dataPrint(LOGFILE,"DAT-K-ZL_NR[%d] selected in number is %02X\n",j,charTmp[j]);
                                           if (charTmp[j] == zlData[j+ZL_HEAD_LENGTH]){
                                         	  dataPrint(LOGFILE,"DAT-K-ZL_NR[%d] = zlData[%d+ZL_HEAD_LENGTH], which is %d\n",j,j,zlData[j+ZL_HEAD_LENGTH]);
                                           }//if
                                           */
                                       } //for

                            	   }//if (!strTmp){

                            } //else
                       }//if (0 != num_row){
                      self_mysql_free_result(mysql_result);
                  }//(NULL != mysql_result){
            }else {
                   errorPrint(LOGFILE,"ERR-K-Debug query zl table error.\n","错误-K-Debug 查询指令表失败.\n");
            }
     	}//if connected to DB


#endif //#ifdef _READZLAFTERINSERT

/*

#ifndef _LOOP
    	//调试，如果没有链接到服务器则暂不循环
    	break;
#else
    	//sleep(SLEEP_YK_LOOP);
#endif
*/


//	}//while



/*
	int *pRet =  (int*)malloc(sizeof(int));
	*pRet = 2;

	msgPrint(LOGFILE,"MSG-K-main_loop_of_YK is going to close.\n","消息-K-遥控线程将关闭.\n");

	pthread_exit((void*)pRet);
	//pthread_exit((void*)(&retValOfThreadYK));
	//return ( (void*)1);
*/

    	return 1;

}


#else
void* main_loop_of_YK(void *arg){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC-K-+YK.c FUNC: main_loop_of_YK is called.\n","调用-K-YK.c的函数: main_loop_of_YK.\n");

	threadPrint_YK(LOGFILE,"Thr-K-main_loop_of_YK: thread is started successfully.\n","线程-K-遥控线程启动成功.\n");
	int subpid = getpid();
	threadPrint_YK(LOGFILE,"Thr-K-Pid of YK thread is %d.\n","线程-K-遥控线程Pid：%d.\n",subpid);


	//记录YK线程pt
	gTotal.YkPt = pthread_self();

	//gSubtid_1 = pthread_self();
	//threadPrint_YK("Thr-K-Tid of YK thread is %lu.\n",gSubtid_1);

	//取得本线程对应的数据库链接
	mysql_t * selfMysqlp = NULL;
	selfMysqlp = getMysql();
	if (NULL == selfMysqlp){
		errorPrint(LOGFILE,"ERR-K-Can not get mysql connection for this thread.\n","错误-K-无法取得本线程的mysql句柄.\n");

		int *pRet =  (int*)malloc(sizeof(int));
		*pRet = -1;

		msgPrint(LOGFILE,"MSG-K-main_loop_of_YK is going to close.\n","消息-K-遥控线程即将关闭.\n");

		pthread_exit((void*)pRet);
	}

	int lenOfIns;
    __uint8_t  zlData[ZL_MAX_LENGTH];
    __uint8_t  tmpData[BUFFER_SIZE]; //在清理buffer无用数据时，用于读取popBuffer返回的值



	while(gIntIsRun){
		//2018-05-24
    	//遥控线程保活计数器+1
    	gIntNewStatYKThread++;
#ifndef _LOOP
    	//调试，如果没有链接到服务器则暂不循环
    	break;
#else
    	sleep(SLEEP_YK_LOOP);
#endif



	}//while




	int *pRet =  (int*)malloc(sizeof(int));
	*pRet = 2;

	msgPrint(LOGFILE,"MSG-K-main_loop_of_YK is going to close.\n","消息-K-遥控线程将关闭.\n");

	pthread_exit((void*)pRet);
	//pthread_exit((void*)(&retValOfThreadYK));
	//return ( (void*)1);


}
#endif
