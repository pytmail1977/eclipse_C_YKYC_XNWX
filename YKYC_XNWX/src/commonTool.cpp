/*
 * commonTool.c
 *
 *  Created on: 2015年1月26日
 *      Author: pyt64
 */


#include  "commonTool.h"

/*
 * 功能：用于连接到指定的服务端口
 * 参数：
 * @NETSERVER_ADDR：来自宏；
 * @ZL_SERVER_PORT：来自宏；
 * @ gNetSocket：来自全局变量；
 * @gIntIsSocketConnected：socket是否连接的标志量，来自全局变量；
 * 返回值：
 * @1：成功；
 * @-1：失败；
 */
#ifndef _RUN_ON_XNWX
int connectSocket(void){

	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: connectSocket is called.\n","调用++++++commonTool.cpp的函数: connectSocket.\n");

	if(1 == gIntIsSocketConnected){
		prgPrint(LOGFILE,"PRG-T-connectSocket() will do nothing because gIntIsSocketConnected = 1.\n","过程-T-connectSocket()无事可做，因为网络状态为连接.\n");
		return -1;
	}

    memset(&gNetSocket.remote_addr,0,sizeof(gNetSocket.remote_addr)); //数据初始化--清零
    gNetSocket.client_sockfd = -1; //数据初始化

    gNetSocket.remote_addr.sin_family=AF_INET; //设置为IP通信
    gNetSocket.remote_addr.sin_addr.s_addr=inet_addr(NETSERVER_ADDR);//服务器IP地址
    gNetSocket.remote_addr.sin_port=htons(ZL_SERVER_PORT); //服务器端口号

    //创建客户端套接字--IPv4协议，面向连接通信，TCP协议
    if((gNetSocket.client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        errorPrint(LOGFILE, "ERR-T-when init socket fd.\n", "错误-T-初始化socket fd失败.\n");
        return -1;
    }

    //int flags = fcntl(gNetSocket.client_sockfd,F_GETFL,0);//获取建立的sockfd的当前状态（非阻塞）
    //fcntl(gNetSocket.client_sockfd,F_SETFL,flags|O_NONBLOCK);//将当前sockfd设置为非阻塞

    //将套接字绑定到服务器的网络地址上
    if(connect(gNetSocket.client_sockfd,(struct sockaddr *)&gNetSocket.remote_addr,sizeof(struct sockaddr))<0)
    {
        errorPrint(LOGFILE, "ERR-T-when bind socket fd to sockaddr.\n", "错误-T-绑定socket fd到sockaddr失败.\n");
        return -1;
    }

    gIntIsSocketConnected = 1;
    msgPrint(LOGFILE,"MSG-T-Connected to socket successfully.\n","消息-T-链接socket成功.\n");


	return 1;
}

/*
 * 功能：断开到指定服务器端口的连接
 * 参数：
 * @gNetSocket：来自全局变量；
 * @gIntIsSocketConnected：socket是否连接的标志量，来自全局变量；
 * 返回值：
 * @-1：处在未链接状态；
 * @else：调用close(gNetSocket.client_sockfd)的返回值；
 */
int closeSocket(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: closeSocket is called.\n","调用++++++commonTool.cpp的函数: closeSocket.\n");


	if(0 == gIntIsSocketConnected){
		prgPrint(LOGFILE,"PRG-T-closeSocket() will do nothing because gIntIsSocketConnected = 0.\n","过程-T-closeSocket()无事可做，因为网络状态为关闭.\n");
		return -1;
	}

	int ret = close(gNetSocket.client_sockfd);
	if (!ret){
		msgPrint(LOGFILE,"MSG-T-closeSocket success.\n","消息-T-closeSocket成功.\n");
		gIntIsSocketConnected = 0;
	}else{
		msgPrint(LOGFILE,"MSG-T-closeSocket fail.\n","消息-T-closeSocket失败.\n");
		//gIntIsSocketConnected = 0;
	}
	return ret;

}

#endif

/*
 * 功能：连接到数据库
 * 参数：
 * @Mysql：数据库句柄，来自全局变量；
 *
 * @ DB_HOST：数据库主机地址，来自宏；
 * @ DB_USER：数据库用户名，来自宏；
 * @ DB_PASS：数据库口令，来自宏；
 * @ DB_NAME：数据库名	来自宏；
 * 返回值：
 * @-1：调用mysql_init失败，或调用mysql_real_connect失败；
 * @1：连接成功；
 *
 */
int connectDB(mysql_t * Mysql){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: connectDB is called.\n","调用++++++commonTool.cpp的函数: connectDB.\n");


	//mysql_library_init(0,NULL,NULL);

	if (NULL == mysql_init(&Mysql->mysql)) {    //分配和初始化MYSQL对象
		errorPrint(LOGFILE,"ERR-T-mysql_init(): %s.\n","错误-T-初始化MYSQL对象失败: %s.\n", mysql_error(&Mysql->mysql));
        return -1;
    }


	bool my_true= true;
	mysql_options(&Mysql->mysql,MYSQL_OPT_RECONNECT,&my_true);

    //尝试与运行在主机上的MySQL数据库引擎建立连接
    if (NULL == mysql_real_connect(&Mysql->mysql,
    			DB_HOST,
                DB_USER,
                DB_PASS,
                DB_NAME,
                0,
                NULL,
                0)) {
        errorPrint(LOGFILE,"ERR-T-mysql_real_connect(): %s.\n","错误-T-链接mysql实例失败: %s.\n", mysql_error(&Mysql->mysql));
        return -1;
    }

    //设置为自动commit
    mysql_autocommit(&Mysql->mysql,true);
    //gIntIsDbConnected = 1;
    //msgPrint(LOGFILE,"MSG-T-Connected database successfully.\n");
    return 1;
}

#ifdef _RUN_ON_XNWX
int connectCenterDB(mysql_t * Mysql){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: connectDB is called.\n","调用++++++commonTool.cpp的函数: connectDB.\n");


	//mysql_library_init(0,NULL,NULL);

	if (NULL == mysql_init(&Mysql->mysql)) {    //分配和初始化MYSQL对象
        errorPrint(LOGFILE,"ERR-T-mysql_init(): %s.\n","错误-T-初始化MYSQL对象失败: %s.\n", mysql_error(&Mysql->mysql));
        return -1;
    }


	bool my_true= true;
	mysql_options(&Mysql->mysql,MYSQL_OPT_RECONNECT,&my_true);

    //尝试与运行在主机上的MySQL数据库引擎建立连接
    if (NULL == mysql_real_connect(&Mysql->mysql,
    			CENTER_DB_HOST,
    			CENTER_DB_USER,
    			CENTER_DB_PASS,
    			CENTER_DB_NAME,
                0,
                NULL,
                0)) {
        errorPrint(LOGFILE,"ERR-T-mysql_real_connect(): %s.\n","错误-T-链接mysql实例失败: %s.\n", mysql_error(&Mysql->mysql));
        return -1;
    }

    //设置为自动commit
    mysql_autocommit(&Mysql->mysql,true);
    //gIntIsDbConnected = 1;
    //msgPrint(LOGFILE,"MSG-T-Connected database successfully.\n");
    return 1;
}
#endif //#ifdef _RUN_ON_XNWX
/*
 * 功能：关闭到数据库的连接
 * 参数：
 * @gMysql：数据库句柄，来自全局变量
 *
 * 返回值：
 * 无
 *
 */
void closeDB(mysql_t * Mysql){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: closeDB is called.\n","调用++++++commonTool.cpp的函数: closeDB.\n");

	//closeDB不止本地数据库使用，中央数据库可能也要使用。所以在closeDB函数内部判断1==gIntIsDbConnected是不合适的。
	//if (1==gIntIsDbConnected){
		mysql_close(&(Mysql->mysql));
		//mysql_library_end();
		//gIntIsDbConnected = 0;

	//}
}

/*
 * 功能：初始化自定义socket连接结构，将服务器地址字段置0
 * 参数：
 * @netsocket_t *pNetsocket：指向自定义socket连接句柄结构的指针
 * 返回值：
 * 无
 */
void initNetsocket(netsocket_t *pNetsocket){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: initNetsocket is called.\n","调用++++++commonTool.cpp的函数: initNetsocket.\n");
	pNetsocket->client_sockfd = -1;
	memset(&pNetsocket->remote_addr,0,sizeof(pNetsocket->remote_addr)); //数据初始化--清零

}


/*
 * 功能：初始化mysql_t中的锁
 * 参数：
 * @mysql_t* pMYsql：自定义数据库连接句柄结构
 * 返回值：
 * 无
 */
void initMysql(mysql_t* pMYsql){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: initMysql is called.\n","调用++++++commonTool.cpp的函数: initMysql is called.\n");
	worm_mutex_init(&(pMYsql->mutex));

}

/*
 * 功能：销毁mysql_t中的锁
 * @mysql_t* pMYsql：自定义数据库连接句柄结构
 * 返回值：
 * 无
 */
void destroyMysql(mysql_t* pMYsql){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: destroyMysql is called.\n","调用++++++commonTool.cpp的函数: destroyMysql.\n");

	worm_mutex_destroy(&(pMYsql->mutex));
}

/*
 * 功能：封装mysql_affected_rows
 */
my_ulonglong STDCALL self_mysql_affected_rows(mysql_t* pMYsql){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: self_mysql_affected_rows is called.\n","调用++++++commonTool.cpp的函数: self_mysql_affected_rows.\n");

	my_ulonglong STDCALL ret;

	//worm_mutex_lock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&pMYsql->mutex,funcSeq);
	ret = mysql_affected_rows(&pMYsql->mysql);
	//worm_mutex_unlock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&pMYsql->mutex,funcSeq);

	return ret;
}

/*
 * 功能：封装mysql_query
 */
int		STDCALL self_mysql_query(mysql_t* pMYsql, const char *q){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: self_mysql_query is called.\n","调用++++++commonTool.cpp的函数: self_mysql_query.\n");

	int ret;
	//printYK("self_mysql_query is going to lock mysql=%d, sql:%s.\n",pMYsql->mysql,q);
	//worm_mutex_lock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&pMYsql->mutex,funcSeq);
	ret = mysql_query(&pMYsql->mysql,q);
	//worm_mutex_unlock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&pMYsql->mutex,funcSeq);
	//printYK("self_mysql_query unlocked mysq=%d. sql:%d\n",pMYsql->mysql,q);

	return ret;
}

/*
 * 功能：封装mysql_real_query
 */
int		STDCALL self_mysql_real_query(mysql_t* pMYsql, const char *q,unsigned long length){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: self_mysql_real_query is called.\n","调用++++++commonTool.cpp的函数: self_mysql_real_query.\n");

	int ret;

	//worm_mutex_lock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&pMYsql->mutex,funcSeq);
	ret = mysql_real_query(&pMYsql->mysql,q,length);
	//worm_mutex_unlock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&pMYsql->mutex,funcSeq);

	return ret;

}

/*
 * 功能：封装mysql_errno
 */
unsigned int STDCALL self_mysql_errno(mysql_t* pMYsql){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: self_mysql_errno is called.\n","调用++++++commonTool.cpp的函数: self_mysql_errno.\n");

	unsigned int STDCALL ret;
	//worm_mutex_lock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&pMYsql->mutex,funcSeq);
	ret = mysql_errno(&pMYsql->mysql);
	//worm_mutex_unlock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&pMYsql->mutex,funcSeq);

	return ret;

}

/*
 * 封装mysql_error
 */
const char * STDCALL self_mysql_error(mysql_t* pMYsql){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: self_mysql_error is called.\n","调用++++++commonTool.cpp的函数: self_mysql_error.\n");

	const char * STDCALL ret;


	//worm_mutex_lock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&pMYsql->mutex,funcSeq);

	ret = mysql_error(&pMYsql->mysql);

	//worm_mutex_unlock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&pMYsql->mutex,funcSeq);


	return ret;

}

/*
 * 功能：封装mysql_store_result
 */
MYSQL_RES *     STDCALL self_mysql_store_result(mysql_t* pMYsql){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: self_mysql_store_result is called.\n","调用++++++commonTool.cpp的函数: self_mysql_store_result.\n");

	MYSQL_RES *     STDCALL ret;
	//worm_mutex_lock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&pMYsql->mutex,funcSeq);
	ret = mysql_store_result(&pMYsql->mysql);
	//worm_mutex_unlock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&pMYsql->mutex,funcSeq);

	return ret;

}

/*
 * 功能：封装mysql_num_rows
 */
my_ulonglong STDCALL self_mysql_num_rows(mysql_t* pMYsql,MYSQL_RES *res){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: self_mysql_num_rows is called.\n","调用++++++commonTool.cpp的函数: self_mysql_num_rows.\n");

	my_ulonglong STDCALL ret;

	//worm_mutex_lock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&pMYsql->mutex,funcSeq);
	ret = mysql_num_rows(res);
	//worm_mutex_unlock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&pMYsql->mutex,funcSeq);

	return ret;

}

/*
 * 功能：封装mysql_free_result
 */
void		STDCALL self_mysql_free_result(mysql_t* pMYsql,MYSQL_RES *result){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: self_mysql_free_result is called.\n","调用++++++commonTool.cpp的函数: self_mysql_free_result.\n");

	//void		STDCALL ret;

	//worm_mutex_lock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&pMYsql->mutex,funcSeq);
	mysql_free_result(result);
	//worm_mutex_unlock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&pMYsql->mutex,funcSeq);

	//return ret;
}


/*
 * 功能：封装mysql_fetch_row
 */
MYSQL_ROW	STDCALL self_mysql_fetch_row(mysql_t* pMYsql,MYSQL_RES *result){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: self_mysql_fetch_row is called.\n","调用++++++commonTool.cpp的函数: self_mysql_fetch_row.\n");

	MYSQL_ROW ret;

	//worm_mutex_lock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&pMYsql->mutex,funcSeq);
	ret = mysql_fetch_row(result);
	//worm_mutex_unlock(&pMYsql->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&pMYsql->mutex,funcSeq);

	return ret;
}



/*
 * 功能：初始化缓冲（用于在socket接收和指令解析过程之间缓冲数据）初始化锁，初始化头尾指针
 * 参数：
 * @️buffer_t* buffer：指向自定义缓冲区buffer_t结构变量的指针；
 * 返回值：
 * 无
 *
 */
void initBuffer(buffer_t* buffer){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: initBuffer is called.\n","调用++++++commonTool.cpp的函数: initBuffer.\n");

	if(NULL == buffer)
		return;
	bzero(buffer, sizeof(buffer_t));
	//worm_mutex_init(&(buffer->mutex),NULL);
	worm_mutex_init(&(buffer->mutex));
	buffer->head = -1;
	buffer->tail = -1;
	buffer->bIsBusy = 0;
	buffer->flag = 0;
}

/*
 * 功能：销毁缓冲
 * 参数：
 * @️buffer_t* buffer：指向自定义缓冲区buffer_t结构变量的指针；
 * 返回值：
 * 无
 *
 */
void destroyBuffer(buffer_t* buffer){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: destroyBuffer is called.\n","调用++++++commonTool.cpp的函数: destroyBuffer.\n");

	worm_mutex_destroy(&(buffer->mutex));
}

/*
 * 功能：获取缓冲区长度
 * 参数：
 * @️buffer_t* buffer：指向自定义缓冲区buffer_t结构变量的指针
 * 返回值：
 * 缓冲区有效内容所占的长度
 *
 */
int lengthOfBuffer(buffer_t* buffer){
	GET_FUNCSEQ
	//fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: lengthOfBuffer is called.\n","调用++++++commonTool.cpp的函数: lengthOfBuffer.\n");

	int length = 0;

	worm_mutex_lock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&buffer->mutex,funcSeq);
	if(buffer->head != -1)
		length = (buffer->tail + BUFFER_SIZE -buffer->head +1)%BUFFER_SIZE;
	worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);

	return length;
}

/*
 * 功能：写入缓冲
 * 参数：
 * @️buffer_t* buffer：指向自定义缓冲区buffer_t结构变量的指针；
 * @️__uint8_t data[]：写入数组；
 * @️int dataLength：写入长度；
 * 返回值：
 * @-1；空间不足；
 * @1：成功；
 *
 */
int pushBuffer(buffer_t* buffer, __uint8_t data[], int dataLength){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: pushBuffer is called.\n","调用++++++commonTool.cpp的函数: pushBuffer.\n");

	int i;
/*	int emptyroom = BUFFER_SIZE-lengthOfBuffer(buffer);
	if (dataLength>emptyroom)
		return -1;//空间不足
*/
	//如果队列为空，则从头开始写
	worm_mutex_lock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&buffer->mutex,funcSeq);
	//检查空间是否够

	//不用lengthOfBuffer（）
	int length = 0;
	if(buffer->head != -1)
		length = (buffer->tail + BUFFER_SIZE -buffer->head +1)%BUFFER_SIZE;

	//int emptyroom = BUFFER_SIZE-lengthOfBuffer(buffer);
	int emptyroom = BUFFER_SIZE-length;



	if (dataLength>emptyroom){
		worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);
		return -1;//空间不足
	}

	//写入
	if(buffer->tail == -1 && buffer->head == -1){
		int i;
		for(i=0;i<dataLength;i++)
			buffer->buff[i] = data[i];
		buffer->head = 0;
		buffer->tail = dataLength - 1;

	}
	//否则从队尾开始写
	else{
		for(i=0;i<dataLength;i++){
			int site = (buffer->tail+i+1)%BUFFER_SIZE;
			buffer->buff[site] = data[i];
		}
		buffer->tail = (buffer->tail+dataLength)%BUFFER_SIZE;

	}
	worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);

	return 1;
}

/*
 * 功能：读出缓冲（读出的内容弹出）
 * 参数：
 * @️buffer_t* buffer：指向自定义缓冲区buffer_t结构变量的指针；
 * @️__uint8_t data[]：读出到数组；
 * @️int dataLength：读出长度；
 * 返回值：
 * @-1：数据不够则返回；
 * @1：成功；
 *
 */
int popBuffer(buffer_t* buffer, __uint8_t data[], int dataLength){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: popBuffer is called.\n","调用++++++commonTool.cpp的函数: popBuffer.\n");

	int i;
/*	int bufferLength = lengthOfBuffer(buffer);

	if(bufferLength < dataLength)
		return -1; //数据不够则返回
*/
	worm_mutex_lock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&buffer->mutex,funcSeq);
	//判断是否有足够数据
	//不用lengthOfBuffer（）
	int length = 0;
	if(buffer->head != -1)
		length = (buffer->tail + BUFFER_SIZE -buffer->head +1)%BUFFER_SIZE;

	//int bufferLength = lengthOfBuffer(buffer);
	int bufferLength = length;

	if(bufferLength < dataLength){
		worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);
		return -1; //数据不够则返回
	}

	//读出数据
	for(i = 0;i<dataLength;i++)
		data[i] = buffer->buff[(i+buffer->head)%BUFFER_SIZE];

	//重设头指针
	buffer->head = (buffer->head+dataLength)%BUFFER_SIZE;

	//如果读出了队列的最后一个元素，则出队后头尾指针复位
	if(buffer->head == (buffer->tail+1)%BUFFER_SIZE){
		buffer->head = -1;
		buffer->tail = -1;
	}
	worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);

	return 1;
}

/*
 * 功能：清除缓冲至指定位置（不含）
 * 参数：
 * @️buffer_t* buffer：指向自定义缓冲区buffer_t结构变量的指针；
 * @️int index：清除至某个位置；
 * 返回值：
 * @-1：给出位置如果不再有效数据区就退出；
 * @1：成功；
 *
 */
int cleanBuffer2Index(buffer_t* buffer, int index){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: cleanBuffer2Index is called.\n","调用++++++commonTool.cpp的函数: cleanBuffer2Index.\n");

	int head = buffer->head;
	int tail = buffer->tail;

	//////////////////////////
	//给出位置如果不再有效数据区就退出
	///////////////////////////

	tmpPrint(LOGFILE,"TMP-T-cleanBuffer2Index head = %d, tail = %d, index = %d.\n",
			"临时-T-函数cleanBuffer2Index head = %d, tail = %d, index = %d.\n",
			head,tail,index);

	//如果头在前，尾在后，这样判断index是否在有效数据区
	if(( (tail+1)%BUFFER_SIZE >= head) && ((index < head) || (index > (tail+1)%BUFFER_SIZE)))
		return -1;

	//如果尾在前，头在后，这样判断index是否在有效数据区
	if(( (tail+1)%BUFFER_SIZE < head) && ((index < head) && (index > (tail+1)%BUFFER_SIZE )))
		return -1;

	worm_mutex_lock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&buffer->mutex,funcSeq);
	buffer->head = index;

	//如果队列为空了，则出队后头尾指针复位
	if(buffer->head == (buffer->tail+1)%BUFFER_SIZE){
		buffer->head = -1;
		buffer->tail = -1;
	}
	worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);

	return 1;
}


/*
 * 功能：读出缓冲（读出的内容不弹出）
 * 参数：
 * @️buffer_t* buffer：指向自定义缓冲区buffer_t结构变量的指针；
 * @️__uint8_t data[]：读出到数组；
 * @️int dataLength：读出长度；
 * @int from：从那个位置开始读；
 * 返回值：
 * @-1：数据不够则返回；
 * @1：成功；
 *
 */
int readBuffer(buffer_t* buffer, __uint8_t data[], int dataLength, int from){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: readBuffer is called.\n","调用++++++commonTool.cpp的函数: readBuffer.\n");

	int i;
/*	int bufferLength = lengthOfBuffer(buffer);

	tmpPrint(LOGFILE,"lengthOfBuffer is %d.\n",bufferLength);

	if(bufferLength < dataLength+from)
		return -1; //数据不够则返回
*/
	worm_mutex_lock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&buffer->mutex,funcSeq);
	//不用lengthOfBuffer（）
	int length = 0;
	if(buffer->head != -1)
		length = (buffer->tail + BUFFER_SIZE -buffer->head +1)%BUFFER_SIZE;

	//int bufferLength = lengthOfBuffer(buffer);
	int bufferLength = length;

	tmpPrint(LOGFILE,"TMP-T-lengthOfBuffer is %d.\n","临时-T-lengthOfBuffer为%d.\n",bufferLength);

	if(bufferLength < dataLength+from){
		worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);
		return -1; //数据不够则返回
	}

	for(i = 0;i<dataLength;i++){
		data[i] = buffer->buff[(i+buffer->head+from)%BUFFER_SIZE];
		dataPrint(LOGFILE,"DAT-T-buffer from %d is:.\n","数据-T-buffer自%d起：",from);
		dataPrint(LOGFILE,"[%d]=%02X ","[%d]=%02X ",i+buffer->head+from,data[i]);
	}

	worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);

	return 1;
}

/*
 * 功能：在缓冲区中定位一个子串
 * 参数：
 * @️buffer_t* buffer：指向自定义缓冲区buffer_t结构变量的指针；
 * @️__uint8_t data[]：需定位的子串；
 * @️int dataLength：子串长度；
 * 返回值：
 * @定位的位置，-1表示没有找到
 *
 */
int findBuffer(buffer_t* buffer, __uint8_t data[], int dataLength){
	GET_FUNCSEQ
	//fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: findBuffer is called.\n","调用++++++commonTool.cpp的函数: findBuffer.\n");

	int i;
	int j;
	int index = -1;

/*
	int bufferLength = lengthOfBuffer(buffer);

	if(bufferLength < dataLength)
		return index; //数据不够则肯定找不到，返回-1
*/
	worm_mutex_lock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&buffer->mutex,funcSeq);
	//不用lengthOfBuffer（）
	int length = 0;
	if(buffer->head != -1)
		length = (buffer->tail + BUFFER_SIZE -buffer->head +1)%BUFFER_SIZE;

	//int bufferLength = lengthOfBuffer(buffer);
	int bufferLength = length;

	if(bufferLength < dataLength){
		worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);
		return index; //数据不够则肯定找不到，返回-1
	}



	for(i = 0;i<bufferLength;i++){
		//从队列里读取一个长度为dataLength的字串
		for(j=0;j<dataLength;j++){
			if (data[j] != buffer->buff[(i+buffer->head+j)%BUFFER_SIZE])
				break; //for(j=0;j<dataLength;j++){
		}
		if (j == dataLength){
			index = i;
			break; //for(i = 0;i<bufferLength;i++){
		}
	}
	worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);

	return index;
}

/*
 * 功能：输出buffer的内容
 * 参数：
 * @️buffer_t* buffer：指向自定义缓冲区buffer_t结构变量的指针；
 * 返回值：
 * @打印的内容长度；
 *
 */
int printBuffer(buffer_t* buffer){
	GET_FUNCSEQ
	//fucPrint(LOGFILE,"FUC++++++commonTool.cpp FUNC: printBuffer is called.\n","调用++++++commonTool.cpp的函数: printBuffer.\n");

	int i;


	worm_mutex_lock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_lock(&buffer->mutex,funcSeq);
	//不用lengthOfBuffer（）
	int length = 0;
	if(buffer->head != -1)
		length = (buffer->tail + BUFFER_SIZE -buffer->head +1)%BUFFER_SIZE;

	//int bufferLength = lengthOfBuffer(buffer);
	int bufferLength = length;

	if(bufferLength == 0){
		//dataPrint(LOGFILE,"DAT-T-Buffer is empty.\n");
		worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);
		return bufferLength;
	}

	dataPrint(LOGFILE,"DAT-T-Buffer head = %d; tail = %d; length = %d.\n",
			"数据-T-Buffer head = %d; tail = %d; length = %d.\n",
			buffer->head,buffer->tail,bufferLength);

	for(i = 0;i<bufferLength;i++){
		//从队列里读取一个长度为bufferLength的字串
		normalPrint(LOGFILE,"%02X ","%02X ",buffer->buff[(i+buffer->head)%BUFFER_SIZE]);
	}
	worm_mutex_unlock(&buffer->mutex); //2018-6-3不再使用funcSeq //worm_mutex_unlock(&buffer->mutex); //2018-6-3 不再使用funcSeq //worm_mutex_unlock(&buffer->mutex,funcSeq);
	normalPrint(LOGFILE,"\n","\n");

	return bufferLength;
}

//pyt 20150326 修改了线程ID跟踪方法
/*
 * 功能：打印时间和线程ID
 * 参数：
 * @funcSeq：标示函数某次调用的随机数；
 * 返回值：
 * 无
 */
//2018-6-3 不再使用参数funcSeq_t funcSeq
//void prePrint(funcSeq_t funcSeq){
void prePrint(void){
	time_t nowtime;
	struct tm * tm;
	time(&nowtime);
	tm = localtime(&nowtime);

//2018-6-3 不再使用参数funcSeq_t funcSeq
/*
#ifdef _PRE_PRINT_THREAD_AND_FUNCSEQ
	printf("(%4d-%02d-%02d, %02d:%02d:%02d) [T-%lu][F-%lu] ",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec,(unsigned long)(pthread_self()),(unsigned long)(funcSeq));
#else
	printf("(%4d-%02d-%02d, %02d:%02d:%02d) ",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);
#endif
*/
	printf("(%4d-%02d-%02d, %02d:%02d:%02d) ",tm->tm_year+1900,tm->tm_mon+1,tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec);


}


/*
 * 功能：以16bit为单位计算校验和
 * 参数
 * @__uint16_t * buf：缓冲区指针；
 * @int len：缓冲区长度；
 * 返回值：
 * 校验和；
 */
__uint16_t checkSum16(__uint16_t * buf, int len)
{
	//GET_FUNCSEQ
	//fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: checkSum16 is called.\n","调用++++++commonTool.c的函数: checkSum16.\n");

	__uint64_t  sum = 0;
	while (len > 1)
	{
		sum += *buf++;
		len -= sizeof(__uint16_t);
	}
	if(len)
	{
		sum += *((__uint8_t*)buf);
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return (__uint16_t)(~sum);
}



/*
 * 功能：对两个UChar数组进行拷贝
 * 参数
 * @const __uint8_t *src：源缓冲区；
 * @__uint8_t *dst：目标缓冲区；
 * @int count：拷贝字节数；
 * 返回值：
 * 无
 */
void copyUCharArray(const __uint8_t *src, __uint8_t *dst, int count)
{
	//GET_FUNCSEQ
	//fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: copyUCharArray is called.\n","调用++++++commonTool.c的函数: copyUCharArray.\n");

	int i;
	for (i=0;i<count;i++)
	{
		dst[i]=src[i];
	}
}

/*
 * 功能：去一个随机数唯一标示当前函数的当前运行
 * 参数：
 * 无
 * 返回值：
 * 随机数；
 */
//2018-6-3 取消函数定义funcSeq_t getRandFuncSeq()
/*
funcSeq_t getRandFuncSeq(){

	//此函数不能加函数调用输出，因为那个功能需要调用这个函数
	funcSeq_t funcSeq = rand();
	return(funcSeq);
}
*/



/*
 * 功能：初始化锁
 * 参数：
 * @worm_mutex_t *pWormMutex：锁指针；
 * 返回值：
 * 无
 */
//void wormhole_mutex_init(worm_mutex_t *pWormMutex,void* pointer){
void wormhole_mutex_init(worm_mutex_t *pWormMutex){
	//GET_FUNCSEQ
	//fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: wormhole_mutex_init is called.\n","调用++++++commonTool.c的函数: wormhole_mutex_init.\n");

	if(!pWormMutex)
		return;

	pWormMutex->pt = 0;//-1; //表示没有人锁 //2017-5-19改为0
	pWormMutex->funcSeq = -1; //表示没有人锁
	pthread_mutex_init(&pWormMutex->mutex,NULL);
	//msgPrint(LOGFILE,"MSG-T-A mutex is inint.%d.\n","消息-T-A 锁初始化成功.%d.\n",&pWormMutex->mutex,funcSeq);

}

/*
 * 功能：销毁锁
 * 参数：
 * @worm_mutex_t *pWormMutex：锁指针；
 * 返回值：
 * 无
 */
void wormhole_mutex_destroy(worm_mutex_t *pWormMutex){
	//GET_FUNCSEQ
	//fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: wormhole_mutex_destroy is called.\n","调用++++++commonTool.c的函数: wormhole_mutex_destroy.\n");
	if(!pWormMutex)
		return;

	pthread_mutex_destroy(&pWormMutex->mutex);
	pWormMutex->pt = 0; //表示没有人锁
	pWormMutex->funcSeq = -1; //表示没有人锁
	//msgPrint(LOGFILE,"MSG-T-A mutex is destroy.%d.\n","消息-T-锁成功销毁.%d.\n",&pWormMutex->mutex,funcSeq);

}


//2018-6-3修改以下三个函数定义，取消参数funcSeq_t funcSeq
//void wormhole_mutex_lock(worm_mutex_t *pWormMutex, funcSeq_t funcSeq);
//void wormhole_mutex_unlock(worm_mutex_t *pWormMutex, funcSeq_t funcSeq);
//int  wormhole_mutex_trylock(worm_mutex_t *pWormMutex, funcSeq_t funcSeq);




#ifdef _USE_LOCK_SAMPLE
void wormhole_mutex_lock(worm_mutex_t *pWormMutex){

	if(!pWormMutex)
		return;
	pthread_mutex_lock(&pWormMutex->mutex);
}

void wormhole_mutex_unlock(worm_mutex_t *pWormMutex){
	if(!pWormMutex)
		return;

	pthread_mutex_unlock(&pWormMutex->mutex);

}
int  wormhole_mutex_trylock(worm_mutex_t *pWormMutex){
	if(!pWormMutex)
		return -1;
	int ret;
	ret = pthread_mutex_trylock(&pWormMutex->mutex);
	return ret;
}
#else
/*
 * 功能：请求锁
 * 参数：
 * @worm_mutex_t *pWormMutex：锁指针；
 * 返回值：
 * 无
 */
//void wormhole_mutex_lock(worm_mutex_t *pWormMutex, funcSeq_t funcSeq){
void wormhole_mutex_lock(worm_mutex_t *pWormMutex){
	//GET_FUNCSEQ //这里会报重复定义
	//fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: wormhole_mutex_lock is called.\n","调用++++++commonTool.c的函数: wormhole_mutex_lock.\n");

	//prgPrint(LOGFILE,"PRG-T-wormhole_mutex_lock: begin of lock.\n","过程-T-wormhole_mutex_lock: 开始加锁.\n");

	pthread_t selfPt= pthread_self();
	//pthread_t selfPt=1999;

	if(!pWormMutex)
		return;

	//如果正被本线程锁住
	if(selfPt == pWormMutex->pt)
		return;

	//否则
	pthread_mutex_lock(&pWormMutex->mutex);
	pWormMutex->pt = selfPt;
	//pyt20150520 在锁住之后，除了将pt置为1，还将funcSeq记入pWormMutex->funcSeq
	//2018-6-3 不再记录funcSeq
	//pWormMutex->funcSeq = funcSeq;

	//prgPrint(LOGFILE,"PRG-T-wormhole_mutex_lock: end of lock.\n","过程-T-wormhole_mutex_lock: 结束加锁.\n");
	//msgPrint(LOGFILE,"TMP-T-wormhole_mutex_lock: A mutex %d is lock by thread %d func %d.\n","消息-T-wormhole_mutex_lock: 锁-%d被线程-%d函数-%d加锁.\n",&pWormMutex->mutex, selfPt, funcSeq);
}


/*
 * 功能：释放锁
 * 参数：
 * @worm_mutex_t *pWormMutex：锁指针；
 * 返回值：
 * 无
 */
//void wormhole_mutex_unlock(worm_mutex_t *pWormMutex, funcSeq_t funcSeq){
void wormhole_mutex_unlock(worm_mutex_t *pWormMutex){
	//GET_FUNCSEQ //这里会报重复定义
	//fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: wormhole_mutex_unlock is called.\n","调用++++++commonTool.c调用: wormhole_mutex_unlock.\n");
	//prgPrint(LOGFILE,"PRG-T-wormhole_mutex_unlock: begin of unlock.\n","过程-T-wormhole_mutex_unlock: 开始解锁.\n");

	pthread_t selfPt= pthread_self();
	//pthread_t selfPt=1999;

	if(!pWormMutex)
		return;

	//如果未被锁住
	if(0 == pWormMutex->pt){
		//msgPrint(LOGFILE,"MSG-T-wormhole_mutex_unlock: given mutex %d has not been locked now.\n","消息-T-wormhole_mutex_unlock:锁-%d未被锁住.\n",&pWormMutex->mutex,funcSeq);
		//DEBUG_STOP
		return;
	}


	//pyt20150520 将解锁的条件由“正被本线程锁住”修改为“正被本线程锁住且锁住的是本函数实体”
	/*
	//如果正被本线程锁住
	if(selfPt == pWormMutex->pt){
		pWormMutex->pt = 0;
		pthread_mutex_unlock(&pWormMutex->mutex,funcSeq);
		msgPrint(LOGFILE,"MSG-T-A mutex %d is unlock by thread %d.\n",&pWormMutex->mutex, selfPt);

	}else{
		errorPrint(LOGFILE,"ERR-T-wormhole_mutex_unlock: given mutex %d is locked by thread %d, this thread %d can not unlock it!!!!!!!!!!!!!!.\n",&pWormMutex->mutex,pWormMutex->pt,selfPt);
		DEBUG_STOP
	}
	*/


	//2018-6-3 解锁的时候不再判断是不是当前函数实例锁住了相应对象，只要线程是当前线程就可以解锁
	//改
	/*
	//如果正被本线程锁住且锁住的是本函数实体
	if( (selfPt == pWormMutex->pt) && (funcSeq == pWormMutex->funcSeq) ){
		pWormMutex->pt = 0;
		pWormMutex->funcSeq = -1;
		pthread_mutex_unlock(&pWormMutex->mutex);
		//msgPrint(LOGFILE,"MSG-T-wormhole_mutex_unlock: A mutex %d is unlock by thread %d func %d.\n","消息-T-wormhole_mutex_unlock: 锁-%d被线程-%d函数-%d解锁.\n",&pWormMutex->mutex, selfPt, funcSeq);

	}else{
		msgPrint(LOGFILE,"MSG-T-wormhole_mutex_unlock: given mutex %lu is locked by thread %lu func %d, this thread %lu func %d can not unlock it!!!!!!!!!!!!!!.\n",
				"消息-T-wormhole_mutex_unlock: 锁-%lu被线程-%lu函数-%d锁住,本县程-%lu函数-%d无法解锁它!!!!!!!!!!!!!!.\n",
				long(&pWormMutex->mutex),(unsigned long int)(pWormMutex->pt),pWormMutex->funcSeq,(unsigned long int)(selfPt),funcSeq);
		//DEBUG_STOP
	}
	*/
	//为
	//如果正被本线程锁住且锁住的是本函数实体
	if( (selfPt == pWormMutex->pt) ){
		pWormMutex->pt = 0;
		//pWormMutex->funcSeq = -1;
		pthread_mutex_unlock(&pWormMutex->mutex);
		//msgPrint(LOGFILE,"MSG-T-wormhole_mutex_unlock: A mutex %d is unlock by thread %d func %d.\n","消息-T-wormhole_mutex_unlock: 锁-%d被线程-%d函数-%d解锁.\n",&pWormMutex->mutex, selfPt, funcSeq);

	}else{
		msgPrint(LOGFILE,"MSG-T-wormhole_mutex_unlock: given mutex %lu is locked by thread %lu, this thread %lu can not unlock it!!!!!!!!!!!!!!.\n",
				"消息-T-wormhole_mutex_unlock: 锁-%lu被线程-%lu锁住,本线程-%lu无法解锁它!!!!!!!!!!!!!!.\n",
				long(&pWormMutex->mutex),(unsigned long int)(pWormMutex->pt),(unsigned long int)(selfPt));
		//DEBUG_STOP
	}

	//prgPrint(LOGFILE,"PRG-T-wormhole_mutex_unlock: end of unlock.\n","过程-T-wormhole_mutex_unlock:结束解锁.\n");

}

/*
 * 功能：尝试请求锁
 * 参数：
 * @worm_mutex_t *pWormMutex：锁指针；
 * 返回值：
 * @-1：传入的锁指针为空；
 * @0：此锁被本线程锁住；
 * @其它：调用pthread_mutex_trylock返回值；
 */
//int  wormhole_mutex_trylock(worm_mutex_t *pWormMutex, funcSeq_t funcSeq){
int  wormhole_mutex_trylock(worm_mutex_t *pWormMutex){
	//GET_FUNCSEQ //这里会报重复定义
	//fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: wormhole_mutex_trylock is called.\n","调用++++++commonTool.c的函数: wormhole_mutex_trylock.\n");

	pthread_t selfPt= pthread_self();

	if(!pWormMutex)
		return -1;
	//如果正被本线程锁住
	if(selfPt == pWormMutex->pt)
		return 0;
	//否则
	int ret;
	ret = pthread_mutex_trylock(&pWormMutex->mutex);
	//pyt20150520 在锁住之后，除了将pt置为1，还将funcSeq记入pWormMutex->funcSeq
	if(0 == ret){
		pWormMutex->pt = selfPt;
		//2018-6-3 不再记录funcSeq
		//pWormMutex->funcSeq = funcSeq;
		//msgPrint(LOGFILE,"MSG-T-wormhole_mutex_trylock: A mutex %d is lock by thread %d func %d.\n","消息-T-wormhole_mutex_trylock: 锁-%d被线程-%d函数-%d加锁.\n",&pWormMutex->mutex,selfPt,funcSeq);
	}

	return ret;


}
#endif //ifdef_USE_LOCK_SAMPLE
/*
 * 功能：查询谁锁定了指定的锁
 * 参数：
 * @worm_mutex_t *pWormMutex：锁指针；
 * 返回值：
 * 无
 */
//pyt20150520 将返回锁线程，改为输出当前的锁是被哪个线程的那个函数实例所锁住的
void wormhole_mutex_wholock(worm_mutex_t *pWormMutex){
	GET_FUNCSEQ
	//fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: wormhole_mutex_wholock is called.\n","调用++++++commonTool.c的函数: wormhole_mutex_wholock.\n");

	msgPrint(LOGFILE,"MSG-T-wormhole_mutex_wholock: mutex %lu is locked by thread %lu function %d.\n",
			"消息-T-wormhole_mutex_wholock: 锁-%lu正被线程-%lu函数-%d锁定.\n",
			(long int)(&pWormMutex->mutex),(unsigned long int)(pWormMutex->pt),pWormMutex->funcSeq);

}



//
/*
 * 功能：new一个配置项并初始化
 * 参数：
 * @int yy_id：用于初始化的YY_ID;
 * @int pzx_id:用于初始化的PZX_ID;
 * @int cjzq:用于初始化的CJZQ;
 * @char sql[]:用于初始化的CJFF;
 * 返回值：
 * @ycpzx_t * pYCPZX：传出的遥测配置项结构指针；
 */
ycpzx_t * newYCPZX(int yy_id, int pzx_id, int cjzq, char sql[]){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: newYCPZX is called.\n","调用++++++commonTool.c的函数: newYCPZX.\n");
	ycpzx_t * pTmp = NULL;
	pTmp = new(ycpzx_t);
	if (pTmp == NULL){
		errorPrint(LOGFILE,"ERR-T-Can't new a ycpzx_t*.\n","错误-T-无法NEW一个ycpzx_t*类型的指针.\n");
	}
	pTmp->yy_id = yy_id;
	//tmpPrint(LOGFILE,"pYCPZX->yy_id =%d .\n", pYCPZX->yy_id);
	pTmp->pzx_id = pzx_id;
	pTmp->cjzq = cjzq;
	pTmp->next = NULL;
	pTmp->prev = NULL;
	bzero(pTmp->cjff_sql,YSYC_CJFF_SQL_MAX_LENGTH);

	if (NULL == sql)
		return pTmp;

	int i;

	for (i=0;i<YSYC_CJFF_SQL_MAX_LENGTH;i++){
		pTmp->cjff_sql[i] = sql[i];
	}
	//tmpPrint(LOGFILE,"pTmp->cjff_sql =%s .\n","pTmp->cjff_sql =%s .\n", pTmp->cjff_sql);
	return pTmp;
}

/*
 * 功能：向配置项链表插入一个配置项
 * 参数：
 * @ycpzx_t * head：待插入的链表头；
 * @ycpzx_t * pYCPZX：待插入的配置项指针
 * 返回值：
 * 无
 */
void insertYCPZX(ycpzx_t * head,ycpzx_t * pYCPZX){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: insertYCPZX is called.\n","调用++++++commonTool.c的函数: insertYCPZX.\n");

	//如果要插入的是一个空指针则返回
	if (pYCPZX == NULL) return;
	//插链表头

	//tmpPrint(LOGFILE,"head->next:%d","head->next:%d",head->next);
	pYCPZX->next = head->next;

	pYCPZX->prev = head;

	//修改原来第一个元素的prev指针
    if(NULL != head->next)
    	head->next->prev = pYCPZX;

	//修改链表头指针
	head->next = pYCPZX;

}

/*
 * 功能：根据指针删除一个配置项
 * 参数：
 * @ycpzx_t * head：待删除的链表头；（此参数实际无用）
 * @ycpzx_t * pYCPZX：待删除的配置项指针；
 * 返回值：
 * 无
 */
void deleteYCPZXbyPtr(ycpzx_t * head,ycpzx_t * pYCPZX){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: deleteYCPZXbyPtr is called.\n","调用++++++commonTool.c的函数: deleteYCPZXbyPtr.\n");

	//如果要删除的是一个空指针则返回
	if (pYCPZX == NULL) return;
	//如果要删除的是链表头则返回
	if (pYCPZX->prev == NULL) return;
	//前一项的next指到后一项
	pYCPZX->prev->next = pYCPZX->next;
	//如果要删除的不是最后一项，则后一项的prev指向前一项
	if (pYCPZX->next != NULL)
		pYCPZX->next->prev = pYCPZX->prev;

	//删除当前项
	delete(pYCPZX);
}


/*
 * 功能：根据配置项id定位并删除一个配置项
 * 参数：
 * @ycpzx_t * head：待删除的链表头；（此参数实际无用）
 * @int yy_id：待删除的配置项所属应用ID；
 * @int pzx_id：待删除的配置项ID；
 * 返回值：
 * 无
 */
void deleteYCPZXbyId(ycpzx_t * head,int yy_id,int pzx_id){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: deleteYCPZXbyId is called.\n","调用++++++commonTool.c的函数: deleteYCPZXbyId.\n");

	//如果链表为空则返回
	if (head == NULL) return;
	//定位id正确的链表项
	ycpzx_t* tmp = head->next;
	while((tmp->yy_id != yy_id || tmp->pzx_id != pzx_id) && tmp->next!= NULL)
		tmp = tmp->next;
	//如果定位失败则返回
	if (tmp->pzx_id != pzx_id || tmp->yy_id != yy_id)
		return;
	//否则删除对应链表项
	deleteYCPZXbyPtr(head,tmp);
}

/*
 * 功能：清除一个带头的配置项链表
 * 参数：
 * @ycpzx_t * head：待清除的链表头
 */
void destroyYCPZXList(ycpzx_t * head){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: destroyYCPZXList is called.\n","调用++++++commonTool.c的函数: destroyYCPZXList.\n");

	//如果链表为空则返回
	if (head == NULL) return;

	ycpzx_t* tmp = head->next;
	ycpzx_t* tmpNext = NULL;
	while (tmp != NULL){
		tmpNext = tmp->next;
		deleteYCPZXbyPtr(head,tmp);
		tmp = tmpNext;
	}
	delete(head);
}



/*
 * 功能：清理一个带头的配置项链表，删除除了链表头以外的其他项
 * 参数：
 * @ycpzx_t * head：待清理的链表头
 */
void cleanYCPZCList(ycpzx_t * head){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: cleanYCPZCList is called.\n","调用++++++commonTool.c的函数: cleanYCPZCList.\n");

	if (head == NULL) return;
	ycpzx_t* tmp = head->next;
	while (tmp != NULL){
		deleteYCPZXbyPtr(head,tmp);
		tmp = tmp->next;
	}
}

/*
 * 功能：获取当前时间，并标示为易于写入数据库的字符串格式
 * 参数：
 * 无
 * 返回值：
 * 字符串表示的日期时间“年-月-日 时：分：秒”
 */
string getDateString(void){

//	GET_FUNCSEQ
//	fucPrint(LOGFILE,"FUC++++++YKYC.cpp FUNC: getDateString is called.\n","调用++++++YKYC.cpp的函数: getDateString.\n");

	time_t timep;
	time(&timep); //获取time_t类型的当前时间
	tm *tmp = gmtime(&timep);
	string mytime = int2String(1900 + tmp->tm_year)
			+ "-" + int2String(1+tmp->tm_mon)
			+ "-" + int2String(tmp->tm_mday)
			+ " " + int2String(tmp->tm_hour)
			+ ":" + int2String(tmp->tm_min)
			+ ":" + int2String(tmp->tm_sec);
	//dataPrint(LOGFILE,mytime.c_str(),mytime.c_str());
	return mytime;

}

/*
 * 功能：整形转字符串
 * 参数：
 * @int n：待转的整形
 * 返回值：
 * 转换后的字符串（string类型）
 */
string int2String(int n)
{
//	GET_FUNCSEQ
//	fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: int2String is called.\n","调用++++++commonTool.c的函数: int2String.\n");

	ostringstream stream;
	stream<<n; //n为int类型
	return stream.str();
}

int getLengthOfData(__uint8_t b1, __uint8_t b2, __uint8_t b3, __uint8_t b4) {
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: getLengthOfData is called.\n","调用++++++commonTool.c的函数: getLengthOfData.\n");

	return  ((b1<<24) | (b2<<16) | (b3<<8) | b4);
}

//根据调用此函数所在的线程，返回适当的数据库链接
/*
mysql_t * getMysql(void){
	GET_FUNCSEQ
	fucPrint(LOGFILE,"FUC++++++commonTool.c FUNC: getMysql is called.\n","调用++++++commonTool.c的函数: getMysql.\n");

	pthread_t pt = pthread_self();
	if (pt == gTotal.YkycPt){
		return &gMysql[_YKYC];
	}else if (pt == gTotal.YkPt){
		return &gMysql[_YK];
	}else if (pt == gTotal.YcPt){
		return &gMysql[_YC];
	}else{
		return NULL;
	}
}
*/

int myfprintf (FILE *__restrict __stream,
		    __const char *__restrict __format1,__const char *__restrict __format2, ...){
	va_list args;
	va_start(args,__format2);

#ifdef _PRINT_AS_CHINESE
	//printf("format1 is:%s\n",__format1);
	//printf("format2 is:%s\n",__format2);
	//printf("something is:%s\n","wrong");
	//usleep(100000);
	vfprintf (__stream,__format2, args);
#else
	vfprintf (__stream,__format1, args);
#endif
	va_end(args);
	return 1;
}


int get_local_ip(char * ifname, char * ip)
{
    char *temp = NULL;
    int inet_sock;
    struct ifreq ifr;

    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);

    memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    memcpy(ifr.ifr_name, ifname, strlen(ifname));

    if(0 != ioctl(inet_sock, SIOCGIFADDR, &ifr))
    {
        perror("ioctl error");
        return -1;
    }

    temp = inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);
    memcpy(ip, temp, strlen(temp));

    close(inet_sock);

    return 0;
}
