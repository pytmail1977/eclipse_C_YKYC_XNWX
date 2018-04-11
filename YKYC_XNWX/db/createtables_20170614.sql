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
   GLX_ID               tinyint unsigned not null comment '��Ԥ�����û����������ָ��ָ��',
   GLX_MC               varchar(50) comment '�ɿ�',
   GLX_PZFS             tinyint comment '1������Ԥ�ã�2��ָ������',
   GLX_PZSFYX           tinyint comment '1����Ч��-1��Ч',
   GLX_CZFF             tinyint comment '1�����ݿ�ִ�У�2���ű�ִ��',
   GLX_CZNR_sql1        varchar(1024) comment '���ݿ�����sql���',
   GLX_CZNR_sql2        varchar(1024) comment '�����ݿ��в�ѯ�������ļ���sql���',
   GLX_CZNR_JB          varchar(4096) comment '�ű��ļ�·��������',
   primary key (GLX_ID)
);

create table GJFW_JKGL_YYZT
(
   YY_ID                tinyint unsigned not null comment '�˱������ʶ��Ӧ��ʵ������װʱȷ��',
   YY_PID               int unsigned comment 'Ӧ�õĽ���id',
   YY_LY                tinyint comment '1������Ԥװ��2��ָ�װ',
   YY_AZLJ              varchar(4096) comment '���ֻ�Ӧ�ô��ֶ���Ч',
   YY_AZBWJBH           tinyint unsigned comment '����ָ����Ӧ�ö�Ӧ�ĸ���װ��',
   YY_AZZT              tinyint comment '1���Ѱ�װ��-1��δ��װ��Ӧ�óɹ���װǰ����ж�غ���ֶ�Ϊ-1��',
   YY_AZSJ              datetime comment '��װ��ɵ�ʱ��',
   YY_XZSJ              datetime comment 'ж����ɵ�ʱ�䣨Ӧ��ж�غ��ڴ˱��еļ�¼��ɾ����',
   YY_ZQDSFSN           tinyint default -1 comment '1���ǣ�-1����',
   YY_SFBH              tinyint default -1 comment '1���ǣ�-1����',
   YY_YXZT              tinyint default -1 comment '0δ������1����������2�������У�<0������������ֵ��������Ӧ�����ж��壩',
   YY_SXXT              int unsigned comment '�ɸ���Ӧ��ˢ�´��ֶΣ�ͨ����̹淶�涨�ˢ������',
   YY_YC                tinyblob comment 'Ӧ��ң��35�ֽ�',
   YY_CW                tinyint comment '�ɼ�ع�����д���ֶ�',
   primary key (YY_ID)
);

create table GJFW_LRXJ_XJZT
(
   XJZT_ID              int not null auto_increment,
   XJZT_KJZT            tinyint comment '0���ػ���1������',
   XJZT_GZZT1           tinyint comment '0�����ߣ�1������',
   XJZT_GZZT2           tinyint comment '0������ģʽ��1���쳣ģʽ',
   XJZT_GZZT3           tinyint comment '0��ͨ��������1��ͨ���쳣',
   XJZT_BGMS            tinyint comment '0���Զ���1���ֶ���',
   XJZT_ZP              tinyint comment '0��5s/֡��1��1s/֡��2��0.2s/֡��3��0.1s/֡',
   XJZT_BGSJ            int comment '��λ��΢�룻0����Ч��������ع�ģʽΪ1ʱ���˴�Ӧ��Ϊ��Ч����������10us ~9500000us(ָ���й涨����Ϊ3�ֽ�)',
   XJZT_BGZY            tinyint comment '1~20����Ĭ��1����',
   XJZT_GDGD            tinyint comment '0��340Km��1��500Km',
   XJZT_RWMS            tinyint comment '0�����죬1���Ե�',
   XJZT_TCQWD           tinyint comment '����ȷ����λ',
   XJZT_CJSJ            datetime comment '�����ܽڵ��յ�ң�ⷵ�ص�ʱ��Ϊ׼',
   primary key (XJZT_ID)
);

create table GJFW_LRXJ_ZP
(
   ZPWJ_ID              int not null auto_increment,
   XJZT_ID              int not null,
   ZPWJ_LJ              varchar(4096),
   ZPWJ_PSSJ            datetime comment '���յ���Ƭ�ļ���ʱ��Ϊ׼�������ܽڵ���д',
   ZPWJ_GS              tinyint comment '0��δ֪��1��TIFF��2��PNG��3��JPEG����Ҫ���ֶ�ô����',
   ZPWJ_DX              int comment '��С�˸�ָ��ͳһ',
   ZPWJ_BGSJ            int comment '��λ��΢�룬�ֽ���ͬ���ͨ��Э��涨���ӽ��յ�����Ƭ�ļ�ͷ�ж�ȡ���������д',
   ZPWJ_BGZY            tinyint comment '��λ���������ӽ��յ�����Ƭ�ļ�ͷ�ж�ȡ���������д',
   ZPWJ_PSZT1           int comment '��̬��Ԫ��1������ƽ̨���ݷ�����',
   ZPWJ_PSZT2           int comment '��̬��Ԫ��2������ƽ̨���ݷ�����',
   ZPWJ_PSZT3           int comment '��̬��Ԫ��3������ƽ̨���ݷ�����',
   ZPWJ_PSZT4           int comment '��̬��Ԫ��4������ƽ̨���ݷ�����',
   ZPWJ_SLTCD           int comment '����ͼ�ļ�����',
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
   SC_WJXH              smallint unsigned not null comment 'ÿ�����������ļ�ʱ��1��ʼ��������',
   SC_RWID              tinyint unsigned not null comment '���ָ��ָ��������ID�ڱ����У��������ؼ�¼',
   SC_WJMC              varchar(50),
   SC_WJLJ              varchar(4096),
   primary key (SC_RWID, SC_WJXH)
);

create table GJFW_SLT
(
   SLT_WJID             int not null auto_increment,
   YY_ID                tinyint comment '�˱������ʶ��Ӧ��ʵ������װʱȷ��',
   SLT_YWJLJ            varchar(4096),
   SLT_YWJTCD           int,
   SLT_YWJK             int,
   SLT_YWJG             int,
   SLT_YWJS             int,
   SLT_CS               tinyint,
   SLT_WJLJ             varchar(4096),
   SLT_WJK              int,
   SLT_WLG              int,
   SLT_ZXJG             tinyint comment '0������1���ɹ���<0��ʧ��',
   primary key (SLT_WJID)
);

create table GJFW_SZ_SZSJ
(
   SZSJB_ID             int not null auto_increment,
   SZSJB_YYID           tinyint unsigned comment '����ע���ݰ��н����õ�',
   SZSJB_WJBH           tinyint unsigned comment '����ע���ݰ��н����õ�',
   SZSJB_BH             int unsigned comment '�����ݰ����������(ָ���й涨����Ϊ3�ֽ�)',
   SZSJB_CD             smallint unsigned comment '��λ���ֽ�',
   SZSJB_NR             blob,
   SZSJB_GS             tinyint comment '1����ʼ����2���м����3��������',
   SZSJB_ZT             tinyint comment '0��δʹ�ã�1����ʹ��',
   primary key (SZSJB_ID)
);

create table GJFW_SZ_SZWJ
(
   WJ_YYID              tinyint unsigned not null comment 'ʹ�ø��ļ���Ӧ��ID',
   WJ_BH                tinyint unsigned not null comment '�Ե���Ӧ����˵�����ļ����ӦΨһ������Ӧ��ʶ���ļ�',
   WJ_LY                tinyint comment '0��Ԥ�棬1����ע',
   WJ_LXBH              tinyint unsigned comment '�Ե���Ӧ����˵�����ļ����ͱ��ӦΨһ������Ӧ��ʶ���ļ���;',
   WJ_MC                varchar(50),
   WJ_LJ                varchar(4096),
   WJ_YT                tinyint unsigned comment '����ָʾӦ�����ʹ�ø��ļ�',
   WJ_CD                int unsigned comment 'WJ_LY==1ʱ��Ч�������ϴ����ļ�����(ָ���й涨����Ϊ3�ֽ�)',
   WJ_MD5JY             blob comment 'WJ_LY==1ʱ��Ч�������ϴ�����ȷУ��ֵ',
   WJ_SZSFCG            tinyint comment 'WJ_LY==1ʱ��Ч��0��δ֪��1���ɹ���-1��ʧ�ܣ���ȷ���ݰ����㣩��-2��ʧ�ܣ��ļ�У��ʧ�ܣ���-3���޷�д���ļ�',
   WJ_SZKSSJ            datetime comment 'WJ_LY==1ʱ��Ч',
   WJ_SZJSSJ            datetime comment 'WJ_LY==1ʱ��Ч',
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
   YY_ID                tinyint unsigned not null comment '�˱������ʶ��Ӧ��ʵ������װʱȷ��',
   YSYCPZX_ID           tinyint unsigned not null comment ' ',
   YSYCPZX_MC           varchar(50) comment '�ɿ�',
   YSYCPZX_PZSJ         datetime comment '�ɿ�',
   YSYCPZX_LY           tinyint comment '1������Ԥ�ã�2��ָ������',
   YSYCPZX_CJFF         varchar(1024) comment '���ȴ���512��Ϊ�˱���Ԥ��sql',
   YSYCPZX_CJZQ         tinyint comment '��λ���룬0��ʾ��������ִ��',
   YSYCPZX_SFYX         tinyint comment '1����Ч��-1��Ч',
   primary key (YY_ID, YSYCPZX_ID)
);

create table GJFW_YC_YSYCSJ
(
   YY_ID                tinyint unsigned not null comment '�˱������ʶ��Ӧ��ʵ������װʱȷ��',
   YSYCPZX_ID           tinyint unsigned not null comment '��Ԥ�����û�ң��������ָ��ָ��',
   YSYCSJ_CJSJ          datetime not null,
   YSYCSJ_CJNR          longblob,
   primary key (YY_ID, YSYCPZX_ID, YSYCSJ_CJSJ)
);

create table GJFW_YK_ZL
(
   ZL_ID                int not null auto_increment comment '��������������ָ������ܽڵ��˳��',
   YY_ID                tinyint unsigned not null comment '�˱������ʶ��Ӧ��ʵ������װʱȷ��',
   ZL_LX                tinyint unsigned comment '��ʾӦ��Ӧ����������',
   ZL_BH                tinyint unsigned,
   ZL_JSSJ              datetime comment '��дָ��Ľ���ʱ�䣬���ڰ�ʱ������¾�ָ��',
   ZL_NR                blob comment 'ָ�����ݵĽ�һ�������ɽ��ո�ָ���Ӧ�ø���ָ�����ͺ�ָ�������',
   ZL_ZXJG              tinyint default 0 comment '0��δ����1�����ܣ�2���ɹ���С��0��ʧ�ܣ�-1��Ӧ��������������δִ��ָ�-2���������߱��ܾ�ִ�У�<=-3ִ��ʧ��ԭ���ɸ�Ӧ�����ж��壩',
   primary key (ZL_ID)
);

create table GJFW_YST
(
   YST_WJID             int not null auto_increment,
   YY_ID                tinyint comment '�˱������ʶ��Ӧ��ʵ������װʱȷ��',
   YST_YWJLJ            varchar(4096),
   YST_YWJTCD           int,
   YST_YWJK             int,
   YST_YWJG             int,
   YST_YWJS             int,
   YST_CS               tinyint,
   YST_WJLJ             varchar(4096),
   YST_ZXJG             tinyint comment '0������1���ɹ���<0��ʧ��',
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

