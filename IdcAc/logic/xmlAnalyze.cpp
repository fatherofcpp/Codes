#include "xmlAnalyze.h"
#include "dataConvert.h"
#include "CommonFunction.h"
#include "FileReadWrite.h"
#include "Log.h"
CXmlAnalyze gXmlAnalyze;

CXmlAnalyze::CXmlAnalyze()
{
    buffMallocLen = MAX_XML_LEN;
    pBuff = (char*)malloc(buffMallocLen);
    memset(pBuff, 0, buffMallocLen);

    pOuterDev = new str_outer_dev;
    memset(pOuterDev, 0, sizeof(*pOuterDev));
    pColtorInf = new st_coltor_inf;
    memset(pColtorInf, 0, sizeof(*pColtorInf));

    deqStUserInf.clear();

    InitCmdIdName();
}

CXmlAnalyze::~CXmlAnalyze()
{
    free(pBuff);
    delete pOuterDev;
    delete pColtorInf;
}

void CXmlAnalyze::ClearBuff()
{
    memset(pBuff, 0, buffMallocLen);
}

int CXmlAnalyze::InitCmdIdName()
{
    mapCmdIdName[E_DTU_CMD] = "dtu_new";
    mapCmdIdName[E_READ_WARN_HISTORY_CMD] = "read_warn_data";
    mapCmdIdName[E_WRITE_WARN_HISTORY_CMD] = "write_warn_data";
    mapCmdIdName[E_READ_THRESHOLD_CMD] = "read_warn_threshold";
    mapCmdIdName[E_WRITE_THRESHOLD_CMD] = "write_warn_threshold";
    mapCmdIdName[E_READ_COLTOR_CMD] = "read_col_conf";
    mapCmdIdName[E_WRITE_COLTOR_CMD] = "write_col_conf";
    mapCmdIdName[E_WRITE_COLTOR_TIME_CMD] = "write_time";
    mapCmdIdName[E_WRITE_UPGRADE_CMD] = "update_sw_by_udisk";
    return 0;
}

int CXmlAnalyze::GetSubNodeNum(XMLNODE *base_root, const char *name)
{
    XMLNODE *node = NULL;
    int num = 0;

    node = f_xmlnode_sch_elem(base_root, name);
    base_root = node;

    while(node != NULL)
    {
        num ++;

        node = f_xmlnode_nxt_node(base_root, name);
        base_root = node;
    }
    return num;
}

void CXmlAnalyze::FormMessage(str_communicate_data& com_data)
{
    MU_CHAR val = 0;
    int tmp_ind = 0;
    //head
    tmp_ind = 0;
    val = 0x68;
    com_data.head[tmp_ind++] = val;
    val = 0x68;
    com_data.head[tmp_ind++] = val;
    val = 0x16;
    com_data.head[tmp_ind++] = val;
    val = 0x16;
    com_data.head[tmp_ind++] = val;

    //指令序号
    tmp_ind = 0;
    val = 0x00;
    com_data.sequence[tmp_ind++] = val;
    com_data.sequence[tmp_ind++] = val;
    com_data.sequence[tmp_ind++] = val;
    com_data.sequence[tmp_ind++] = val;

    //AES加密
    int remain = (com_data.xml_len) % AES_BLOCK_SIZE ;
    if (remain != 0)
    {
        int add_len = AES_BLOCK_SIZE - remain;
        for (int i = 0; i < add_len; i++)
        {
            com_data.xml_data[com_data.xml_len + i] = 0x20;
        }
        com_data.xml_len += add_len;
    }
    if(MAX_XML_LEN < com_data.xml_len)
    {
        gLog.Add("%s MAX_XML_LEN < com_data.xml_len", __FUNCTION__);
        return;
    }
    encry_aes((MU_CHAR*)com_data.xml_data, com_data.xml_len);

    //数据长度
    tmp_ind = 0;
    int data_len = com_data.xml_len + 4;
    //低位在前
    val = data_len & 0xFF;
    com_data.data_len[tmp_ind++] = val;

    val = (data_len >> 8) & 0xFF;
    com_data.data_len[tmp_ind++] = val;

    val = (data_len >> 16) & 0xFF;
    com_data.data_len[tmp_ind++] = val;

    val = (data_len >> 24) & 0xFF;
    com_data.data_len[tmp_ind++] = val;

    //tail
    tmp_ind = 0;
    val = 0x55;
    com_data.tail[tmp_ind++] = val;

    val = 0xAA;
    com_data.tail[tmp_ind++] = val;

    val = 0x55;
    com_data.tail[tmp_ind++] = val;

    val = 0xAA;
    com_data.tail[tmp_ind++] = val;

    //format
    int tmp_len = 0;
    //head
    for (int i = 0; i < 4; i++)
    {
        pBuff[tmp_len++] = com_data.head[i];
    }
    //数据长度
    for (int i = 0; i < 4; i++)
    {
        pBuff[tmp_len++] = com_data.data_len[i];
    }
    //指令序号
    for (int i = 0; i < 4; i++)
    {
        pBuff[tmp_len++] = com_data.sequence[i];
    }

    //tmp_len += sprintf(pBuff + tmp_len, "%s",com_data.xml_data);
    ///AES加密后不能当字符串使用
    for (MU_INT32 i = 0; i < com_data.xml_len; i++)
    {
        pBuff[tmp_len++] = com_data.xml_data[i];
    }

    //CRC低位在前
    com_data.parity_crc = parity_crc16(0, (MU_CHAR*)pBuff, tmp_len);
    val = (com_data.parity_crc) & 0xFF;
    pBuff[tmp_len++] = val;

    val = (com_data.parity_crc>>8) & 0xFF;
    pBuff[tmp_len++] = val;

    //tail
    for (int i = 0; i < 4; i++)
    {
        pBuff[tmp_len++] = com_data.tail[i];
    }

}

int CXmlAnalyze::FormCmdByType( string type_name )
{
    ClearBuff();
    str_communicate_data com_data;
    memset(&com_data, 0, sizeof(com_data));

    com_data.xml_len += sprintf(com_data.xml_data + com_data.xml_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    com_data.xml_len += sprintf(com_data.xml_data + com_data.xml_len, "<root>\r\n");
    com_data.xml_len += sprintf(com_data.xml_data + com_data.xml_len, "	<common>\r\n");
    com_data.xml_len += sprintf(com_data.xml_data + com_data.xml_len, "		<type>%s</type>\r\n",type_name.c_str());
    com_data.xml_len += sprintf(com_data.xml_data + com_data.xml_len, "	</common>\r\n");
    com_data.xml_len += sprintf(com_data.xml_data + com_data.xml_len, "</root>\r\n");

    //test

    //end test
    if(com_data.xml_len >= MAX_XML_LEN)
    {
        gLog.Add("%s com_data.xml_len >= MAX_XML_LEN", __FUNCTION__);
        return -1;
    }

    FormMessage(com_data);

    int tatal_len = HEAD_TAIL_LEN + com_data.xml_len;

    return tatal_len;
}


int CXmlAnalyze::FormCmdByXmlData( string xml_data )
{
    ClearBuff();
    str_communicate_data com_data;
    memset(&com_data, 0, sizeof(com_data));

    com_data.xml_len += sprintf(com_data.xml_data + com_data.xml_len, "%s", xml_data.c_str());

    if(com_data.xml_len >= MAX_XML_LEN)
    {
        gLog.Add("%s com_data.xml_len >= MAX_XML_LEN", __FUNCTION__);
        return -1;
    }

    FormMessage(com_data);

    int tatal_len = HEAD_TAIL_LEN + com_data.xml_len;

    return tatal_len;
}


XMLNODE * CXmlAnalyze::ParseRspParam(XML& xml)
{
    XMLNODE *param_node=NULL;
    XMLNODE *node = NULL;

    if(xml.root ==NULL)
    {
        param_node = NULL;
        goto leave;
    }

    if(strcmp(xml.root->name,"root")!=0)
    {
        param_node = NULL;
        goto leave;
    }

    param_node = xml.root;

    node = f_xmlnode_sch_elem(param_node, "param");
    if (node != NULL)
    {
        param_node = node;
    }
    else
    {
        param_node = NULL;
        goto leave;
    }

leave:
    return param_node;
}

int CXmlAnalyze::FormGetAgeCmd()
{
    int tatal_len = FormCmdByType("read_age");
    return tatal_len;
}

int CXmlAnalyze::ParseGetAgeRsp(int& age)
{
    MS_INT32 rtn = 0;
    XMLNODE *base_root=NULL;
    XMLNODE *node = NULL;
    XML xml;

    memset(&xml,0,sizeof(xml));

    rtn = f_xmlparse_init(&xml, pBuff);
    if (rtn < 0)
    {
        rtn = -1;
        goto leave;
    }

    node = ParseRspParam(xml);
    if (node != NULL)
    {
        base_root = node;
    }
    else
    {
        rtn = -2;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "age");
    if (node != NULL)
    {
        printf("%s node->data=%s\n", __FUNCTION__,node->data);
        str_to_int32(node->data, age);
    }
    else
    {
        rtn = -3;
        goto leave;
    }

leave:
    f_xmlnode_del(&xml);
    return rtn;
}

int CXmlAnalyze::FormSetAgeCmd()
{
    MS_CHAR xml_data[MAX_XML_LEN];
    memset(xml_data, 0, sizeof(xml_data));
    int tmp_len = 0;

    tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n", "write_age");
    tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
    int new_age = 123;
    tmp_len += sprintf(xml_data + tmp_len, "		<age>%d</age>\r\n", new_age);
    tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

    int tatal_len = FormCmdByXmlData(xml_data);

    return tatal_len;
}

int CXmlAnalyze::ParseSetAgeRsp()
{
    MS_INT32 rt = 0;
    MS_INT32 rtn = 0;
    XMLNODE *base_root=NULL;
    XMLNODE *node = NULL;
    XML xml;

    memset(&xml,0,sizeof(xml));

    rt = f_xmlparse_init(&xml, pBuff);
    if (rt < 0)
    {
        rtn = -1;
        goto leave;
    }

    if(xml.root ==NULL)
    {
        rtn = -1;
        goto leave;
    }

    if(strcmp(xml.root->name,"root")!=0)
    {
        rtn = -1;
        goto leave;
    }

    base_root = xml.root;

    node = f_xmlnode_sch_elem(base_root, "param");
    if (node != NULL)
    {
        base_root = node;
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "result");
    if (node != NULL)
    {
        if(1 != atoi(node->data))
        {
            rtn = -1;
            goto leave;
        }
    }
    else
    {
        rtn = -1;
        goto leave;
    }

leave:
    f_xmlnode_del(&xml);
    return rtn;
}

int CXmlAnalyze::FormDtuCmd(string dtu_data)
{
    Sleep(FRAME_DTU_DELAY_M);
    MS_CHAR xml_data[MAX_XML_LEN];
    memset(xml_data, 0, sizeof(xml_data));
    int tmp_len = 0;
    tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",
                       mapCmdIdName[E_DTU_CMD].c_str());
    tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "        <dtu_data>%s</dtu_data>\r\n",
                       dtu_data.c_str());
    tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

    int tatal_len = FormCmdByXmlData(xml_data);

    return tatal_len;
}

int CXmlAnalyze::ParseDtuRsp(string& dtu_data)
{
    MS_INT32 rtn = 0;
    XMLNODE *base_root=NULL;
    XMLNODE *node = NULL;
    XML xml;

    memset(&xml,0,sizeof(xml));

    rtn = f_xmlparse_init(&xml, pBuff);
    if (rtn < 0)
    {
        rtn = -1;
        goto leave;
    }

    node = ParseRspParam(xml);
    if (node != NULL)
    {
        base_root = node;
    }
    else
    {
        rtn = -2;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "dtu_data");
    if (node != NULL)
    {
        dtu_data = node->data;
    }
    else
    {
        rtn = -3;
        goto leave;
    }

leave:
    f_xmlnode_del(&xml);
    return rtn;
}

int CXmlAnalyze::FormSetWarnHistoryCmd(const st_warn_history_inf* warn_history_inf)
{
    MS_CHAR xml_data[MAX_XML_LEN];
    memset(xml_data, 0, sizeof(xml_data));
    int tmp_len = 0;
    tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[E_WRITE_WARN_HISTORY_CMD].c_str());
    tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "        <start>%s</start>\r\n",
                       warn_history_inf->start_time);
    tmp_len += sprintf(xml_data + tmp_len, "        <end>%s</end>\r\n",
                       warn_history_inf->stop_time);
    tmp_len += sprintf(xml_data + tmp_len, "        <dev_type>%d</dev_type>\r\n",
                       warn_history_inf->dev_type);
    tmp_len += sprintf(xml_data + tmp_len, "        <way_code>%d</way_code>\r\n",
                       warn_history_inf->way_code);
    tmp_len += sprintf(xml_data + tmp_len, "        <object>%s</object>\r\n",
                       warn_history_inf->obj_name);
    tmp_len += sprintf(xml_data + tmp_len, "        <warn_code>%s</warn_code>\r\n",
                       warn_history_inf->warn_type);
    tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

    int tatal_len = FormCmdByXmlData(xml_data);

    return tatal_len;
}

int CXmlAnalyze::ParseSetWarnHistoryRsp()
{
    int ret = ParseSetAgeRsp();
    return ret;
}

int CXmlAnalyze::FormGetThresholdCmd()
{
    MS_CHAR xml_data[MAX_XML_LEN];
    memset(xml_data, 0, sizeof(xml_data));
    int tmp_len = 0;
    tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[E_READ_THRESHOLD_CMD].c_str());
    tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

    int tatal_len = FormCmdByXmlData(xml_data);

    return tatal_len;
}

int CXmlAnalyze::ParseGetThresholdRsp()
{
    MS_INT32 rtn = 0;
    XMLNODE *base_root=NULL;
    XMLNODE *node = NULL;
    XML xml;

    memset(&xml,0,sizeof(xml));

    rtn = f_xmlparse_init(&xml, pBuff);
    if (rtn < 0)
    {
        gLog.Add("%s f_xmlparse_init", __FUNCTION__);
        rtn = -1;
        goto leave;
    }

    node = ParseRspParam(xml);
    if (node != NULL)
    {
        base_root = node;
    }
    else
    {
        gLog.Add("%s ParseRspParam", __FUNCTION__);
        rtn = -2;
        goto leave;
    }

    rtn = ParseThreSholdNode(node);

leave:
    f_xmlnode_del(&xml);
    return rtn;
}

int CXmlAnalyze::FormSetThresholdCmd(const VEC_THRESHOLD_ITEM& vec_threshold_set)
{
    MS_CHAR xml_data[MAX_XML_LEN];
    memset(xml_data, 0, sizeof(xml_data));
    int tmp_len = 0;
    tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",
                       mapCmdIdName[E_WRITE_THRESHOLD_CMD].c_str());
    tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
    MU_INT32 cont_size = vec_threshold_set.size();
    for(MU_INT32 i = 0; i < cont_size; i++)
    {
        const st_threshold_item* p_st_thread_param_threshold = &vec_threshold_set[i];
        tmp_len += sprintf(xml_data + tmp_len, "        <i>%d,%d,%d,%d,%d</i>\r\n",
                           p_st_thread_param_threshold->dev_type,
                           p_st_thread_param_threshold->way_code,
                           p_st_thread_param_threshold->warn_type,
                           p_st_thread_param_threshold->is_enable,
                           p_st_thread_param_threshold->thre_val);
        //dev_type, way_code, warn_threshold_code, enable, value
    }
    tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

    int tatal_len = FormCmdByXmlData(xml_data);

    return tatal_len;
}

int CXmlAnalyze::ParseSetThresholdRsp()
{
    int ret = ParseSetAgeRsp();
    return ret;
}

int CXmlAnalyze::FormGetCollectorInfCmd()
{
    int tatal_len = FormCmdByType(mapCmdIdName[E_READ_COLTOR_CMD]);
    return tatal_len;
}

int CXmlAnalyze::ParseGetCollectorInfRsp()
{
    MS_INT32 rtn = 0;
    XMLNODE *base_root=NULL;
    XMLNODE *node = NULL;
    XML xml;

    memset(&xml,0,sizeof(xml));

    rtn = f_xmlparse_init(&xml, pBuff);
    if (rtn < 0)
    {
        rtn = -1;
        goto leave;
    }

    node = ParseRspParam(xml);
    if (node != NULL)
    {
        base_root = node;
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    rtn = ParseCollectorNode(base_root);

leave:
    f_xmlnode_del(&xml);
    return rtn;
}

int CXmlAnalyze::FormSetCollectorInfCmd()
{
    MS_CHAR xml_data[MAX_XML_LEN];
    memset(xml_data, 0, sizeof(xml_data));
    int tmp_len = 0;

    tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[E_WRITE_COLTOR_CMD].c_str());
    tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<addr>%d</addr>\r\n",pColtorInf->col_conf.id);
    tmp_len += sprintf(xml_data + tmp_len, "		<type>%d</type>\r\n",pColtorInf->col_conf.type);
    if(0 == strlen(pColtorInf->col_conf.name))
    {
        strcpy(pColtorInf->col_conf.name, "default_name");
    }
    tmp_len += sprintf(xml_data + tmp_len, "		<name>%s</name>\r\n",pColtorInf->col_conf.name);
    if(0 == strlen(pColtorInf->col_conf.pos_name))
    {
        strcpy(pColtorInf->col_conf.pos_name, "default_pos_name");
    }
    tmp_len += sprintf(xml_data + tmp_len, "		<pos>%s</pos>\r\n",pColtorInf->col_conf.pos_name);
    tmp_len += sprintf(xml_data + tmp_len, "		<sample_m>%d</sample_m>\r\n",pColtorInf->col_conf.sample_m);
    tmp_len += sprintf(xml_data + tmp_len, "		<report_m>%d</report_m>\r\n",pColtorInf->col_conf.report_m);
    tmp_len += sprintf(xml_data + tmp_len, "		<heart_s>%d</heart_s>\r\n",pColtorInf->col_conf.heart_s);
    tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

    int tatal_len = FormCmdByXmlData(xml_data);

    return tatal_len;
}

int CXmlAnalyze::ParseSetCollectorInfRsp()
{
    int ret = ParseSetAgeRsp();
    return ret;
}

int CXmlAnalyze::FormSetTimeInfCmd()
{
    MS_CHAR xml_data[MAX_XML_LEN];
    memset(xml_data, 0, sizeof(xml_data));
    int tmp_len = 0;

    tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n", mapCmdIdName[E_WRITE_COLTOR_TIME_CMD].c_str());
    tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<time>%04d-%02d-%02d,%02d:%02d:%02d</time>\r\n",
                       pColtorInf->dev_time.dev_time.YY,
                       pColtorInf->dev_time.dev_time.MM,
                       pColtorInf->dev_time.dev_time.DD,
                       pColtorInf->dev_time.dev_time.HH,
                       pColtorInf->dev_time.dev_time.MI,
                       pColtorInf->dev_time.dev_time.SS);
    tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

    int tatal_len = FormCmdByXmlData(xml_data);

    return tatal_len;
}

int CXmlAnalyze::ParseSetTimeInfRsp()
{
    int ret = ParseSetAgeRsp();
    return ret;
}

int CXmlAnalyze::FormSetUpgradeCmd()
{
    int tatal_len = FormCmdByType(mapCmdIdName[E_WRITE_UPGRADE_CMD]);
    return tatal_len;
}

int CXmlAnalyze::ParseSetUpgradRsp()
{
    int ret = ParseSetAgeRsp();
    return ret;
}

int CXmlAnalyze::FormGetWarnHistoryCmd(int off_set)
{
    MS_CHAR xml_data[MAX_XML_LEN];
    memset(xml_data, 0, sizeof(xml_data));
    int tmp_len = 0;
    tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[E_READ_WARN_HISTORY_CMD].c_str());
    tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "        <offset>%d</offset>\r\n", off_set);
    tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
    tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

    int tatal_len = FormCmdByXmlData(xml_data);

    return tatal_len;
}

int CXmlAnalyze::ParseGetWarnHistoryRsp(DEQ_ST_WARN_HISTORY_INF& vec_st_warn_history)
{
    MS_INT32 rtn = 0;
    XMLNODE *base_root=NULL;
    XMLNODE *node = NULL;
    XML xml;

    memset(&xml,0,sizeof(xml));

    rtn = f_xmlparse_init(&xml, pBuff);
    if (rtn < 0)
    {
        rtn = -1;
        goto leave;
    }

    node = ParseRspParam(xml);
    if (node != NULL)
    {
        base_root = node;
    }
    else
    {
        rtn = -2;
        goto leave;
    }

    rtn = ParseWarnEventDataNode(base_root, vec_st_warn_history);
    if (rtn != 0)
    {
        rtn = -3;
        goto leave;
    }

leave:
    f_xmlnode_del(&xml);
    return rtn;
}

int CXmlAnalyze::LoadLocalXml(const char *pfile)
{
    int rtn = 0;

    rtn = LoadFile(pfile);
    if (rtn != 0)
    {
        return -1;
    }

    rtn = ParseLocalXml();
    if (rtn != 0)
    {
        return -2;
    }

    return rtn;
}

int CXmlAnalyze::LoadUserGroup(const char *pfile)
{
    int rtn = 0;

    deqStUserInf.clear();

    rtn = LoadFile(pfile);
    if (rtn != 0)
    {
        return -1;
    }

    rtn = ParseUserGroup();
    if (rtn != 0)
    {
        return -2;
    }

    return rtn;
}

int CXmlAnalyze::SaveUserGroup(const char *pfile)
{
    FILE *pf=NULL;
    int rtn = 0;

    ClearBuff();
    int tmp_len = 0;
    tmp_len += sprintf(pBuff + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
    tmp_len += sprintf(pBuff + tmp_len, "<root>\r\n");
    MU_INT32 cont_num = deqStUserInf.size();
    for(MU_INT32 i = 0; i < cont_num; i++)
    {
        st_user_inf* p_st_user_inf = &deqStUserInf[i];
        tmp_len += sprintf(pBuff + tmp_len, "<user>\r\n");
        tmp_len += sprintf(pBuff + tmp_len, "   <user_name>%s</user_name>\r\n",
                           p_st_user_inf->user_name);
        tmp_len += sprintf(pBuff + tmp_len, "   <user_descript>%s</user_descript>\r\n",
                           p_st_user_inf->user_descript);
        tmp_len += sprintf(pBuff + tmp_len, "   <user_pwd>%s</user_pwd>\r\n",
                           p_st_user_inf->user_pwd);
        tmp_len += sprintf(pBuff + tmp_len, "</user>\r\n");
    }
    tmp_len += sprintf(pBuff + tmp_len, "</root>\r\n");

    pf = fopen(pfile,"wb");
    if(pf)
    {
        fwrite(pBuff, sizeof(char), tmp_len, pf);
        fclose(pf);
    }
    else
    {
        return -1;
    }

    return rtn;
}

int CXmlAnalyze::LoadFile(const char *pfile)
{
    int len = 0;
    int rtn = 0;

    ClearBuff();
    len = GetFileLen(pfile);
    if (len == -1)
    {
        return -1;
    }

    rtn = GetFileData(pfile,len,pBuff);
    if (rtn == 0)
    {
        return 0;
    }
    else
    {
        return -2;
    }
}

int CXmlAnalyze::ParseLocalXml()
{
    MS_INT32 rtn = 0;
    XMLNODE *base_root = NULL;
    XMLNODE *node = NULL;
    XML xml;
    memset(&xml,0,sizeof(xml));

    rtn = f_xmlparse_init(&xml, pBuff);
    if (rtn < 0)
    {
        rtn = -1;
        goto leave;
    }

    if(xml.root ==NULL)
    {
        rtn = -1;
        goto leave;
    }

    if(strcmp(xml.root->name,"save")!=0)
    {
        rtn = -1;
        goto leave;
    }

    base_root = xml.root;

    //dev
    {
        XMLNODE *port_root = NULL;
        int port_num = GetSubNodeNum(base_root, "port");
        if(port_num > MAX_PORT_NUM)
        {
            rtn = -1;
            goto leave;
        }
        pOuterDev->port_num = port_num;
        for (int i = 0; i < port_num; i++)
        {
            if (i == 0)
            {
                node = f_xmlnode_sch_elem(base_root, "port");
                if(!node)
                {
                    gLog.Add("%s f_xmlnode_sch_elem port", __FUNCTION__);
                    rtn = -1;
                    goto leave;
                }
                port_root = node;
            }
            else
            {
                node = f_xmlnode_nxt_node(port_root, "port");
                if(!node)
                {
                    gLog.Add("%s f_xmlnode_nxt_node port", __FUNCTION__);
                    rtn = -1;
                    goto leave;
                }
                port_root = node;
            }

            XMLNODE *dev_root = NULL;
            int dev_num = GetSubNodeNum(port_root, "dev");
            pOuterDev->port_dev[i].dev_num = dev_num;
            for (int j = 0; j < dev_num; j++)
            {
                if (j == 0)
                {
                    node = f_xmlnode_sch_elem(port_root, "dev");
                    if(!node)
                    {
                        gLog.Add("%s f_xmlnode_sch_elem dev", __FUNCTION__);
                        rtn = -1;
                        goto leave;
                    }
                    dev_root = node;
                }
                else
                {
                    node = f_xmlnode_nxt_node(dev_root, "dev");
                    if(!node)
                    {
                        gLog.Add("%s f_xmlnode_nxt_node dev", __FUNCTION__);
                        rtn = -1;
                        goto leave;
                    }
                    dev_root = node;
                }
                rtn = ParsePortDevNode(dev_root, i, j);
                if (rtn < 0)
                {
                    goto leave;
                }
            }
        }
    }

leave:
    f_xmlnode_del(&xml);
    return rtn;
}

int CXmlAnalyze::ParsePortDevNode(XMLNODE *base_root, int port_ind, int dev_ind)
{
    int rtn = 0;
    XMLNODE* node = NULL;

    int ret = ParseDevInf(base_root, port_ind, dev_ind);
    if (ret != 0)
    {
        rtn = -1;
        goto leave;
    }
leave:
    return rtn;
}

int CXmlAnalyze::ParseDevInf(XMLNODE *base_root, int port_ind, int dev_ind)
{
    XMLNODE* tmp_root = NULL;
    XMLNODE* node = NULL;
    int rtn = 0;
    VEC_INT vec_way_list;
    int way_num = 0;

    node = f_xmlnode_sch_elem(base_root, "addr");
    if (node != NULL)
    {
        str_to_int8(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].dev_addr.id);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "type");
    if (node != NULL)
    {
        str_to_int32(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].dev_type);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "sub_type");
    if (node != NULL)
    {
        str_to_int32(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].sub_type);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "name");
    if (node != NULL)
    {
        strcpy(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].name, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "pos");
    if (node != NULL)
    {
        strcpy(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].pos_name, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "opt");
    if (node != NULL)
    {
        string str = node->data;
        vector<string> vecElem;
        int ret = SplitToString(str, vecElem, ',');
        if (ret == 4)
        {
            int databit = 0;
            str_to_int32(vecElem[0].c_str(),
                         databit);
            pOuterDev->port_dev[port_ind].dev_inf[dev_ind].rs485_conf.databits = databit;

            str_to_int32(vecElem[1].c_str(),
                         pOuterDev->port_dev[port_ind].dev_inf[dev_ind].rs485_conf.pari);

            int stop = 0;
            str_to_int32(vecElem[2].c_str(),
                         stop);
            pOuterDev->port_dev[port_ind].dev_inf[dev_ind].rs485_conf.stop = stop;

            int baud = 0;
            str_to_int32(vecElem[3].c_str(),
                         baud);
            pOuterDev->port_dev[port_ind].dev_inf[dev_ind].rs485_conf.baud = baud;
        }
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "factory");
    if (node != NULL)
    {
        tmp_root = node;
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(tmp_root, "hw_ver");
    if (node != NULL)
    {
        strcpy(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].hw_ver, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(tmp_root, "sw_ver");
    if (node != NULL)
    {
        strcpy(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].sw_ver, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "way_mask");
    if (node != NULL)
    {
        str_to_int64(node->data,
                     pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_mask, 16);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    {
        //0x03 0,1根据掩码获取使用的回路下标数组
        int list_num =
                GetWayListByMask(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_mask, vec_way_list, MAX_WAY_NUM);
    }
    way_num = GetSubNodeNum(base_root, "way");
    for (int i = 0; i < way_num; i++)
    {
        if (i == 0)
        {
            node = f_xmlnode_sch_elem(base_root, "way");
            if(!node)
            {
                gLog.Add("%s f_xmlnode_sch_elem way", __FUNCTION__);
                rtn = -1;
                goto leave;
            }
            base_root = node;
        }
        else
        {
            node = f_xmlnode_nxt_node(base_root, "way");
            if(!node)
            {
                gLog.Add("%s f_xmlnode_nxt_node way", __FUNCTION__);
                rtn = -1;
                goto leave;
            }
            base_root = node;
        }

        int ret = ParseWayInf(base_root, port_ind, dev_ind, vec_way_list[i]);
        if (ret == 0)
        {
            continue;
        }
        else
        {
            rtn = -1;
            goto leave;
        }
    }

leave:
    return rtn;
}

int CXmlAnalyze::ParseUserGroup()
{
    /*
    <root>
        <user>
            <user_name>name</user_name>
            <user_descript>des</user_descript>
            <user_pwd>pwd</user_pwd>
        </user>
        ...
        <user>
            <user_name>name</user_name>
            <user_descript>des</user_descript>
            <user_pwd>pwd</user_pwd>
        </user>
    </root>
    */
    MS_INT32 rtn = 0;
    XMLNODE *base_root = NULL;
    XMLNODE *node = NULL;
    XML xml;
    memset(&xml,0,sizeof(xml));

    rtn = f_xmlparse_init(&xml, pBuff);
    if (rtn < 0)
    {
        rtn = -1;
        goto leave;
    }

    if(xml.root ==NULL)
    {
        rtn = -1;
        goto leave;
    }

    if(strcmp(xml.root->name,"root")!=0)
    {
        rtn = -1;
        goto leave;
    }

    base_root = xml.root;
    {
        st_user_inf user_inf;
        memset(&user_inf, 0, sizeof(user_inf));

        XMLNODE *user_root = NULL;
        int user_num = GetSubNodeNum(base_root, "user");
        for (int user_ind = 0; user_ind < user_num; user_ind++)
        {
            if (user_ind == 0)
            {
                node = f_xmlnode_sch_elem(base_root, "user");
                if(!node)
                {
                    gLog.Add("%s f_xmlnode_sch_elem user", __FUNCTION__);
                    rtn = -1;
                    goto leave;
                }
                user_root = node;
            }
            else
            {
                node = f_xmlnode_nxt_node(user_root, "user");
                if(!node)
                {
                    gLog.Add("%s f_xmlnode_nxt_node user", __FUNCTION__);
                    rtn = -1;
                    goto leave;
                }
                user_root = node;
            }
            rtn = ParseUserNode(user_root, &user_inf);
            if (rtn < 0)
            {
                goto leave;
            }
            else
            {
                deqStUserInf.push_back(user_inf);
            }
        }
    }

leave:
    return rtn;
}

int CXmlAnalyze::ParseUserNode(XMLNODE *base_root, st_user_inf* p_user_inf)
{
    /*
        <user>
            <user_name>name</user_name>
            <user_descript>des</user_descript>
            <user_pwd>pwd</user_pwd>
        </user>
    */
    XMLNODE* node = NULL;
    int rtn = 0;

    node = f_xmlnode_sch_elem(base_root, "user_name");
    if (node != NULL)
    {
        strcpy(p_user_inf->user_name, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "user_descript");
    if (node != NULL)
    {
        strcpy(p_user_inf->user_descript, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "user_pwd");
    if (node != NULL)
    {
        strcpy(p_user_inf->user_pwd, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

leave:
    return rtn;
}

int CXmlAnalyze::ParseWarnEventDataNode(XMLNODE *base_root, DEQ_ST_WARN_HISTORY_INF& vec_st_warn_history)
{
    int rtn = 0;
    XMLNODE* node = NULL;

    char ele_name[32] = "warn";
    int node_num = GetSubNodeNum(base_root, ele_name);
    for (int i = 0; i < node_num; i++)
    {
        if (i == 0)
        {
            node = f_xmlnode_sch_elem(base_root, ele_name);
            if(!node)
            {
                rtn = -1;
                goto LEAVE;
            }
            base_root = node;
        }
        else
        {
            node = f_xmlnode_nxt_node(base_root, ele_name);
            if(!node)
            {
                rtn = -2;
                goto LEAVE;
            }
            base_root = node;
        }

        st_warn_history_inf* p_warn_history = new st_warn_history_inf;
        memset(p_warn_history, 0, sizeof(*p_warn_history));

        int ret = ParseWarnEventDataWNode(base_root, i, *p_warn_history);
        if (ret == 0)
        {
            vec_st_warn_history.push_back(p_warn_history);
            continue;
        }
        else
        {
            rtn = -3;
            goto LEAVE;
        }
    }
LEAVE:
    return rtn;
}

int CXmlAnalyze::ParseWarnEventDataWNode(XMLNODE *base_root, int param_ind,
                                         st_warn_history_inf& warn_history)
{
    int rtn = 0;
    XMLNODE* node = NULL;

    node = f_xmlnode_sch_elem(base_root, "start");
    if (node != NULL)
    {
        strcpy(warn_history.start_time, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "end");
    if (node != NULL)
    {
        strcpy(warn_history.stop_time , node->data);
    }
    else
    {
        rtn = -2;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "dev_type");
    if (node != NULL)
    {
        str_to_int32(node->data, warn_history.dev_type);
    }
    else
    {
        rtn = -3;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "way_code");
    if (node != NULL)
    {
        str_to_int32(node->data, warn_history.way_code);
    }
    else
    {
        rtn = -4;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "object");
    if (node != NULL)
    {
        strcpy(warn_history.obj_name , node->data);
    }
    else
    {
        rtn = -5;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "warn_code");
    if (node != NULL)
    {
        strcpy(warn_history.warn_type , node->data);
    }
    else
    {
        rtn = -6;
        goto leave;
    }
leave:
    return rtn;
}

int CXmlAnalyze::ParseWayInf(XMLNODE *base_root, int port_ind, int dev_ind, int way_ind)
{
    XMLNODE* tmp_root = NULL;
    XMLNODE* node = NULL;
    int rtn = 0;

    node = f_xmlnode_sch_elem(base_root, "id");
    if (node != NULL)
    {
        str_to_int32(node->data,
                     pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].way_id);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "class_id");
    if (node != NULL)
    {
        str_to_int32(node->data,
                     pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].class_id);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "item_id");
    if (node != NULL)
    {
        str_to_int32(node->data,
                     pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].item_id);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "addr");
    if (node != NULL)
    {
        int addr = 0;
        str_to_int32(node->data, addr);
        pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].way_addr = addr;
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "type");
    if (node != NULL)
    {
        str_to_int32(node->data,
                     pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].way_type);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "name");
    if (node != NULL)
    {
        strcpy(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].way_name, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "pos");
    if (node != NULL)
    {
        strcpy(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].pos_name, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

    node = f_xmlnode_sch_elem(base_root, "detail");
    if (node != NULL)
    {
        strcpy(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].detail, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }

leave:
    return rtn;
}

int CXmlAnalyze::ParseThreSholdNode(XMLNODE *base_root)
{
    int rtn = 0;
    XMLNODE* node = NULL;

    char ele_name[32] = "i";
    int node_num = GetSubNodeNum(base_root, ele_name);
    for (int i = 0; i < node_num; i++)
    {
        if (i == 0)
        {
            node = f_xmlnode_sch_elem(base_root, ele_name);
            if(!node)
            {
                gLog.Add("%s f_xmlnode_sch_elem", __FUNCTION__);
                rtn = -1;
                goto leave;
            }
            base_root = node;
        }
        else
        {
            node = f_xmlnode_nxt_node(base_root, ele_name);
            if(!node)
            {
                gLog.Add("%s f_xmlnode_nxt_node", __FUNCTION__);
                rtn = -1;
                goto leave;
            }
            base_root = node;
        }

        rtn = ParseThreSholdINode(node);
        if (rtn != 0)
        {
            gLog.Add("%s ParseThreSholdINode", __FUNCTION__);
            goto leave;
        }
    }
leave:
    return rtn;
}

int CXmlAnalyze::ParseThreSholdINode(XMLNODE *base_root)
{
    int rtn = 0;
    XMLNODE* node = NULL;
    node = base_root;
    if (node != NULL)
    {
        string str = node->data;
        vector<string> vecElem;
        int ret = SplitToString(str, vecElem, ',');
        if (ret == 5)
        {
            //dev_type, way_code, warn_threshold_code, enable, value
            st_threshold_item param_threshold;
            memset(&param_threshold, 0, sizeof(param_threshold));
            str_to_int32(vecElem[0].c_str(), param_threshold.dev_type);
            str_to_int32(vecElem[1].c_str(), param_threshold.way_code);
            str_to_int32(vecElem[2].c_str(), param_threshold.warn_type);
            int is_enable = 0;
            str_to_int32(vecElem[3].c_str(), is_enable);
            param_threshold.is_enable = (is_enable != 0);
            str_to_int32(vecElem[4].c_str(), param_threshold.thre_val);
            st_way_data* p_st_way_data =
                    GetWayInfByCode(GetOuterDev(),
                                    param_threshold.way_code,
                                    param_threshold.dev_type);
            if(p_st_way_data)
            {
                MU_INT32* p_thre = GetThreByWarnType(p_st_way_data, param_threshold.warn_type);
                if(p_thre)
                {
                    p_thre[0] = param_threshold.thre_val;
                    p_thre[1] = is_enable;
                }
                else
                {
                    gLog.Add("%s GetThreByWarnType", __FUNCTION__);
                    rtn = -1;
                    goto leave;
                }
            }
            else
            {
                gLog.Add("%s GetWayInfByCode", __FUNCTION__);
                rtn = -2;
                goto leave;
            }
        }
        else
        {
            gLog.Add("%s SplitToString", __FUNCTION__);
            rtn = -2;
            goto leave;
        }
    }
    else
    {
        gLog.Add("%s node", __FUNCTION__);
        rtn = -3;
        goto leave;
    }
leave:
    return rtn;
}

int CXmlAnalyze::ParseCollectorNode(XMLNODE *base_root)
{
    int rtn = 0;
    XMLNODE* node = NULL;
    node = f_xmlnode_sch_elem(base_root, "addr");
    if (node != NULL)
    {
        str_to_int32(node->data,pColtorInf->col_conf.id);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
    node = f_xmlnode_sch_elem(base_root, "type");
    if (node != NULL)
    {
        str_to_int32(node->data,pColtorInf->col_conf.type);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
    node = f_xmlnode_sch_elem(base_root, "name");
    if (node != NULL)
    {
        strcpy(pColtorInf->col_conf.name, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
    node = f_xmlnode_sch_elem(base_root, "pos");
    if (node != NULL)
    {
        strcpy(pColtorInf->col_conf.pos_name, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
    node = f_xmlnode_sch_elem(base_root, "sample_m");
    if (node != NULL)
    {
        str_to_int32(node->data,pColtorInf->col_conf.sample_m);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
    node = f_xmlnode_sch_elem(base_root, "heart_s");
    if (node != NULL)
    {
        str_to_int32(node->data,pColtorInf->col_conf.heart_s);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
    node = f_xmlnode_sch_elem(base_root, "report_m");
    if (node != NULL)
    {
        str_to_int32(node->data,pColtorInf->col_conf.report_m);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
    node = f_xmlnode_sch_elem(base_root, "if_num");
    if (node != NULL)
    {
        MU_INT32 port_num = 0;
        str_to_int32(node->data, port_num);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
    node = f_xmlnode_sch_elem(base_root, "hw_ver");
    if (node != NULL)
    {
        strcpy(pColtorInf->col_conf.hw_ver, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
    node = f_xmlnode_sch_elem(base_root, "sw_ver");
    if (node != NULL)
    {
        strcpy(pColtorInf->col_conf.sw_ver, node->data);
    }
    else
    {
        rtn = -1;
        goto leave;
    }
leave:
    return rtn;
}
