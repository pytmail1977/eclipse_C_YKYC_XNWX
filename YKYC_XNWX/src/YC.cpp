/*
 * YC.cpp
 *
 *  Created on: 2017年5月11日
 *      Author: pyt64
 */




#include "YC.h"

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
	selfMysqlp = getMysql();
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
	selfMysqlp = getMysql();
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
	selfMysqlp = getMysql();
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
	selfMysqlp = getMysql();
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
	selfMysqlp = getMysql();
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
	selfMysqlp = getMysql();
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
	selfMysqlp = getMysql();
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
