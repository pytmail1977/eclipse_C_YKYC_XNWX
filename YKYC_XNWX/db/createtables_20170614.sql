drop table if exists GJFW_JKGL_GLPZ;

drop table if exists GJFW_JKGL_YYZT;

drop table if exists GJFW_LRXJ_XJZT;

drop table if exists GJFW_LRXJ_ZP;

drop table if exists GJFW_PTSJFW_CGSJ;

drop table if exists GJFW_PTSJFW_CGSJ_GPS;

drop table if exists GJFW_PTSJFW_XWSJ;

drop table if exists GJFW_SCJ_SCWJ;

drop table if exists GJFW_SLT;

drop table if exists GJFW_SZ_SZSJ;

drop table if exists GJFW_SZ_SZWJ;

drop table if exists GJFW_YC_SSYCLSSJ;

drop table if exists GJFW_YC_YSYCPZX;

drop table if exists GJFW_YC_YSYCSJ;

drop table if exists GJFW_YK_ZL;

drop table if exists GJFW_YST;

create table GJFW_JKGL_GLPZ
(
   GLX_ID               tinyint unsigned not null comment '由预先配置或管理项配置指令指定',
   GLX_MC               varchar(50) comment '可空',
   GLX_PZFS             tinyint comment '1：地面预置，2：指令配置',
   GLX_PZSFYX           tinyint comment '1：有效，-1无效',
   GLX_CZFF             tinyint comment '1：数据库执行，2：脚本执行',
   GLX_CZNR_sql1        varchar(1024) comment '数据库清理sql语句',
   GLX_CZNR_sql2        varchar(1024) comment '从数据库中查询需清理文件的sql语句',
   GLX_CZNR_JB          varchar(4096) comment '脚本文件路径＋名称',
   primary key (GLX_ID)
);

create table GJFW_JKGL_YYZT
(
   YY_ID                tinyint unsigned not null comment '此编号用于识别应用实例，安装时确定',
   YY_PID               int unsigned comment '应用的进程id',
   YY_LY                tinyint comment '1：地面预装，2：指令安装',
   YY_AZLJ              varchar(4096) comment '对手机应用此字段无效',
   YY_AZBWJBH           tinyint unsigned comment '用于指明此应用对应哪个安装包',
   YY_AZZT              tinyint comment '1：已安装，-1：未安装（应用成功安装前，或卸载后此字段为-1）',
   YY_AZSJ              datetime comment '安装完成的时间',
   YY_XZSJ              datetime comment '卸载完成的时间（应用卸载后，在此表中的记录不删除）',
   YY_ZQDSFSN           tinyint default -1 comment '1：是，-1：否',
   YY_SFBH              tinyint default -1 comment '1：是，-1：否',
   YY_YXZT              tinyint default -1 comment '0未启动，1正在启动，2正常运行，<0启动出错（具体值及涵义有应用自行定义）',
   YY_SXXT              int unsigned comment '由各个应用刷新此字段，通过编程规范规定最长刷新周期',
   YY_YC                tinyblob comment '应用遥测35字节',
   YY_CW                tinyint comment '由监控管理填写此字段',
   primary key (YY_ID)
);

create table GJFW_LRXJ_XJZT
(
   XJZT_ID              int not null auto_increment,
   XJZT_KJZT            tinyint comment '0：关机，1：开机',
   XJZT_GZZT1           tinyint comment '0：休眠，1：唤醒',
   XJZT_GZZT2           tinyint comment '0：正常模式，1：异常模式',
   XJZT_GZZT3           tinyint comment '0：通信正常，1：通信异常',
   XJZT_BGMS            tinyint comment '0：自动，1：手动，',
   XJZT_ZP              tinyint comment '0：5s/帧，1：1s/帧，2：0.2s/帧，3：0.1s/帧',
   XJZT_BGSJ            int comment '单位：微秒；0：无效（当相机曝光模式为1时，此处应置为无效），其它：10us ~9500000us(指令中规定长度为3字节)',
   XJZT_BGZY            tinyint comment '1~20倍（默认1倍）',
   XJZT_GDGD            tinyint comment '0：340Km，1：500Km',
   XJZT_RWMS            tinyint comment '0：对天，1：对地',
   XJZT_TCQWD           tinyint comment '？需确定单位',
   XJZT_CJSJ            datetime comment '以智能节点收到遥测返回的时间为准',
   primary key (XJZT_ID)
);

create table GJFW_LRXJ_ZP
(
   ZPWJ_ID              int not null auto_increment,
   XJZT_ID              int not null,
   ZPWJ_LJ              varchar(4096),
   ZPWJ_PSSJ            datetime comment '以收到照片文件的时间为准，由智能节点填写',
   ZPWJ_GS              tinyint comment '0：未知，1：TIFF，2：PNG，3：JPEG（需要此字段么？）',
   ZPWJ_DX              int comment '大小端跟指令统一',
   ZPWJ_BGSJ            int comment '单位：微秒，字节序：同相机通信协议规定，从接收到的照片文件头中读取，由相机填写',
   ZPWJ_BGZY            tinyint comment '单位：倍数，从接收到的照片文件头中读取，由相机填写',
   ZPWJ_PSZT1           int comment '姿态四元数1，访问平台数据服务获得',
   ZPWJ_PSZT2           int comment '姿态四元数2，访问平台数据服务获得',
   ZPWJ_PSZT3           int comment '姿态四元数3，访问平台数据服务获得',
   ZPWJ_PSZT4           int comment '姿态四元数4，访问平台数据服务获得',
   ZPWJ_SLTCD           int comment '缩略图文件长度',
   primary key (ZPWJ_ID)
);

create table GJFW_PTSJFW_CGSJ
(
   Attribute_120        int not null auto_increment,
   primary key (Attribute_120)
);

create table GJFW_PTSJFW_CGSJ_GPS
(
   Attribute_122        int not null auto_increment,
   primary key (Attribute_122)
);

create table GJFW_PTSJFW_XWSJ
(
   Attribute_121        int not null auto_increment,
   primary key (Attribute_121)
);

create table GJFW_SCJ_SCWJ
(
   SC_WJXH              smallint unsigned not null comment '每次生成数传文件时从1开始往后排序',
   SC_RWID              tinyint unsigned not null comment '如果指令指定的任务ID在表里有，就清除相关记录',
   SC_WJMC              varchar(50),
   SC_WJLJ              varchar(4096),
   primary key (SC_RWID, SC_WJXH)
);

create table GJFW_SLT
(
   SLT_WJID             int not null auto_increment,
   YY_ID                tinyint comment '此编号用于识别应用实例，安装时确定',
   SLT_YWJLJ            varchar(4096),
   SLT_YWJTCD           int,
   SLT_YWJK             int,
   SLT_YWJG             int,
   SLT_YWJS             int,
   SLT_CS               tinyint,
   SLT_WJLJ             varchar(4096),
   SLT_WJK              int,
   SLT_WLG              int,
   SLT_ZXJG             tinyint comment '0：请求，1：成功，<0：失败',
   primary key (SLT_WJID)
);

create table GJFW_SZ_SZSJ
(
   SZSJB_ID             int not null auto_increment,
   SZSJB_YYID           tinyint unsigned comment '从上注数据包中解析得到',
   SZSJB_WJBH           tinyint unsigned comment '从上注数据包中解析得到',
   SZSJB_BH             int unsigned comment '由数据包解析出编号(指令中规定长度为3字节)',
   SZSJB_CD             smallint unsigned comment '单位：字节',
   SZSJB_NR             blob,
   SZSJB_GS             tinyint comment '1：开始包；2：中间包；3：结束包',
   SZSJB_ZT             tinyint comment '0：未使用；1：已使用',
   primary key (SZSJB_ID)
);

create table GJFW_SZ_SZWJ
(
   WJ_YYID              tinyint unsigned not null comment '使用该文件的应用ID',
   WJ_BH                tinyint unsigned not null comment '对单个应用来说，此文件编号应唯一，用于应用识别文件',
   WJ_LY                tinyint comment '0：预存，1：上注',
   WJ_LXBH              tinyint unsigned comment '对单个应用来说，此文件类型编号应唯一，用于应用识别文件用途',
   WJ_MC                varchar(50),
   WJ_LJ                varchar(4096),
   WJ_YT                tinyint unsigned comment '用于指示应用如何使用该文件',
   WJ_CD                int unsigned comment 'WJ_LY==1时有效，地面上传的文件长度(指令中规定长度为3字节)',
   WJ_MD5JY             blob comment 'WJ_LY==1时有效，地面上传的正确校验值',
   WJ_SZSFCG            tinyint comment 'WJ_LY==1时有效，0：未知，1：成功，-1：失败（正确数据包不足），-2：失败（文件校验失败），-3：无法写入文件',
   WJ_SZKSSJ            datetime comment 'WJ_LY==1时有效',
   WJ_SZJSSJ            datetime comment 'WJ_LY==1时有效',
   primary key (WJ_YYID, WJ_BH)
);

create table GJFW_YC_SSYCLSSJ
(
   SSYCSJ_CJSJ          datetime not null,
   SSYCSJ_CJNR          longblob,
   primary key (SSYCSJ_CJSJ)
);

create table GJFW_YC_YSYCPZX
(
   YY_ID                tinyint unsigned not null comment '此编号用于识别应用实例，安装时确定',
   YSYCPZX_ID           tinyint unsigned not null comment ' ',
   YSYCPZX_MC           varchar(50) comment '可空',
   YSYCPZX_PZSJ         datetime comment '可空',
   YSYCPZX_LY           tinyint comment '1：地面预置，2：指令配置',
   YSYCPZX_CJFF         varchar(1024) comment '长度大于512是为了保存预置sql',
   YSYCPZX_CJZQ         tinyint comment '单位：秒，0表示不周期性执行',
   YSYCPZX_SFYX         tinyint comment '1：有效，-1无效',
   primary key (YY_ID, YSYCPZX_ID)
);

create table GJFW_YC_YSYCSJ
(
   YY_ID                tinyint unsigned not null comment '此编号用于识别应用实例，安装时确定',
   YSYCPZX_ID           tinyint unsigned not null comment '由预先配置或遥测量配置指令指定',
   YSYCSJ_CJSJ          datetime not null,
   YSYCSJ_CJNR          longblob,
   primary key (YY_ID, YSYCPZX_ID, YSYCSJ_CJSJ)
);

create table GJFW_YK_ZL
(
   ZL_ID                int not null auto_increment comment '自增，用于区分指令到达智能节点的顺序',
   YY_ID                tinyint unsigned not null comment '此编号用于识别应用实例，安装时确定',
   ZL_LX                tinyint unsigned comment '表示应用应完成哪类操作',
   ZL_BH                tinyint unsigned,
   ZL_JSSJ              datetime comment '填写指令的解析时间，用于按时间清理陈旧指令',
   ZL_NR                blob comment '指令内容的进一步解析由接收该指令的应用根据指令类型和指令编号完成',
   ZL_ZXJG              tinyint default 0 comment '0：未读，1：接受，2：成功，小于0：失败（-1：应用重新启动抛弃未执行指令，-2：条件不具备拒绝执行，<=-3执行失败原因由各应用自行定义）',
   primary key (ZL_ID)
);

create table GJFW_YST
(
   YST_WJID             int not null auto_increment,
   YY_ID                tinyint comment '此编号用于识别应用实例，安装时确定',
   YST_YWJLJ            varchar(4096),
   YST_YWJTCD           int,
   YST_YWJK             int,
   YST_YWJG             int,
   YST_YWJS             int,
   YST_CS               tinyint,
   YST_WJLJ             varchar(4096),
   YST_ZXJG             tinyint comment '0：请求，1：成功，<0：失败',
   primary key (YST_WJID)
);

alter table GJFW_LRXJ_ZP add constraint FK_Relationship_5 foreign key (XJZT_ID)
      references GJFW_LRXJ_XJZT (XJZT_ID) on delete restrict on update restrict;

alter table GJFW_SLT add constraint FK_Relationship_6 foreign key (YY_ID)
      references GJFW_JKGL_YYZT (YY_ID) on delete restrict on update restrict;

alter table GJFW_YC_YSYCPZX add constraint FK_Relationship_3 foreign key (YY_ID)
      references GJFW_JKGL_YYZT (YY_ID) on delete restrict on update restrict;

alter table GJFW_YC_YSYCSJ add constraint FK_Relationship_1 foreign key (YY_ID, YSYCPZX_ID)
      references GJFW_YC_YSYCPZX (YY_ID, YSYCPZX_ID) on delete restrict on update restrict;

alter table GJFW_YK_ZL add constraint FK_Relationship_2 foreign key (YY_ID)
      references GJFW_JKGL_YYZT (YY_ID) on delete restrict on update restrict;

alter table GJFW_YST add constraint FK_Relationship_7 foreign key (YY_ID)
      references GJFW_JKGL_YYZT (YY_ID) on delete restrict on update restrict;

