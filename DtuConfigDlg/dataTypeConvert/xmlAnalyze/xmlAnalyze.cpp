#include "stdafx.h"
#include "xmlAnalyze.h"

#include "dataConvert.h"
#include "CommonFunction.h"
#include "FileReadWrite.h"
#include "ParamIdMap.h"

CXmlAnalyze gXmlAnalyze;
using namespace std;

static int gHexTypeXml = 10;

CXmlAnalyze::CXmlAnalyze()
{
	pColtorInf = new st_coltor_inf;
	pOuterDev = new str_outer_dev;
	pServiceInf = new st_net_conf;
	pPreOuterDev = new str_outer_dev;
	pReportFormatStoreInf = new st_report_store_inf;
	pCProtModbusSet = new CProtModbusSet;

	buffMallocLen = 1024 * 256;
	pBuff = (char*)malloc(buffMallocLen);
	memset(pBuff, 0, buffMallocLen);

	memset(pColtorInf, 0, sizeof(*pColtorInf));
	memset(pOuterDev, 0, sizeof(*pOuterDev));
	memset(pServiceInf, 0, sizeof(*pServiceInf));
	memset(pPreOuterDev, 0, sizeof(*pPreOuterDev));
	memset(pReportFormatStoreInf, 0, sizeof(*pReportFormatStoreInf));

	//出始化命令类型与命令名称的对应关系
	InitCmdIdName();
}

CXmlAnalyze::~CXmlAnalyze()
{
	free(pBuff);
	delete pColtorInf;
	delete pOuterDev;
	delete pServiceInf;
	delete pPreOuterDev;
	delete pReportFormatStoreInf;
	delete pCProtModbusSet;
}

void CXmlAnalyze::ClearBuff()
{
	memset(pBuff, 0, buffMallocLen);
}

void CXmlAnalyze::SetColtorInf(st_coltor_inf* coltorInf)
{
	memcpy(pColtorInf, coltorInf, sizeof(*pColtorInf));
}

void CXmlAnalyze::SetOuterDev(str_outer_dev* outerDev)
{
	memcpy(pOuterDev, outerDev, sizeof(*pOuterDev));
}

void CXmlAnalyze::SetServiceInf(st_net_conf* serviceInf)
{
	memcpy(pServiceInf, serviceInf, sizeof(*pServiceInf));
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
	assert(MAX_XML_LEN >= com_data.xml_len);
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


int CXmlAnalyze::FormGetCollectorInfCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[READ_COLLECTOR_CMD]);
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
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_COLLECTOR_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<addr>%d</addr>\r\n",pColtorInf->col_conf.id);
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%d</type>\r\n",pColtorInf->col_conf.type);
	tmp_len += sprintf(xml_data + tmp_len, "		<name>%s</name>\r\n",pColtorInf->col_conf.name);
	tmp_len += sprintf(xml_data + tmp_len, "		<pos>%s</pos>\r\n",pColtorInf->col_conf.pos_name);
	tmp_len += sprintf(xml_data + tmp_len, "		<sample_m>%d</sample_m>\r\n",pColtorInf->col_conf.sample_m);
	tmp_len += sprintf(xml_data + tmp_len, "		<report_m>%d</report_m>\r\n",pColtorInf->col_conf.report_m);
	tmp_len += sprintf(xml_data + tmp_len, "		<heart_s>%d</heart_s>\r\n",pColtorInf->col_conf.heart_s);
	//tmp_len += sprintf(xml_data + tmp_len, "		<if_num>%d</if_num>\r\n",pOuterDev->port_num);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetCollectorInfRsp()
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

int CXmlAnalyze::FormGetTimeInfCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[READ_TIME_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseGetTimeInfRsp()
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

	rtn = ParseTimeNode(base_root);

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormSetTimeInfCmd()
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_TIME_CMD].c_str());
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
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormAddDevInfCmd(st_dev_change_inf & dev_inf)
{
	ClearBuff();
	int tmpLen = MESSAGE_BODY_IND;//消息体起始位置
	int bodyLen = 0;
	tmpLen += sprintf(pBuff + tmpLen, "<req>\r\n");
	tmpLen += sprintf(pBuff + tmpLen, "<addr>%d,%d,%d</addr>\r\n",dev_inf.dev_coord.port_ind,0,0);

	tmpLen += sprintf(pBuff + tmpLen, "<dev>");
	tmpLen += sprintf(pBuff + tmpLen, "<d_addr>%lld</d_addr>",dev_inf.dev_inf.dev_addr.id);
	tmpLen += sprintf(pBuff + tmpLen, "<d_name>%s</d_name>",dev_inf.dev_inf.name);
	tmpLen += sprintf(pBuff + tmpLen, "<d_sub_type>%d</d_sub_type>",dev_inf.dev_inf.sub_type);
	tmpLen += sprintf(pBuff + tmpLen, "<d_opt>%d,%d</d_opt>",
		dev_inf.dev_inf.rs485_conf.baud,
		dev_inf.dev_inf.rs485_conf.pari);
	tmpLen += sprintf(pBuff + tmpLen, "<w_mask>%d</w_mask>",dev_inf.dev_inf.way_inf.way_mask);

	int way_num = GetWayNumBySubTypeValue(dev_inf.dev_inf.sub_type);
	for(int j = 0; j < way_num ; j++)
	{
		if (0 != strlen(dev_inf.dev_inf.way_inf.way_data[j].way_name))
		{
			tmpLen += sprintf(pBuff + tmpLen, "<w_name>%d,%s</w_name>",j,dev_inf.dev_inf.way_inf.way_data[j].way_name);
		}
	}
	tmpLen += sprintf(pBuff + tmpLen, "</dev>");

	tmpLen += sprintf(pBuff + tmpLen, "</req>\r\n");

	bodyLen = tmpLen - MESSAGE_BODY_IND;
	//FormMessage(bodyLen, SET_DEV);	
	return tmpLen;
}

int CXmlAnalyze::ParseAddDevInfRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();	
	return rtn;
}

int CXmlAnalyze::FormGetServInfCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[READ_SERVICE_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseGetServInfRsp()
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

	rtn = ParseServInfNode(base_root);

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormSetServInfCmd()
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_SERVICE_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");

	tmp_len += sprintf(xml_data + tmp_len, "	<secret>%s</secret>\r\n",
		pServiceInf->service_net.secret);
	for(int i = 0; i < MAX_SERVICE_NUM; i++)
	{
		if (true)//(pServiceInf->service_net.ip_port_domain[i].valid)
		{
			tmp_len += sprintf(xml_data + tmp_len, "		<server>\r\n");
			tmp_len += sprintf(xml_data + tmp_len, "			<used>%d</used>\r\n",pServiceInf->service_net.ip_port_domain[i].valid);
			tmp_len += sprintf(xml_data + tmp_len, "			<domain>%s</domain>\r\n",pServiceInf->service_net.ip_port_domain[i].domain);
			tmp_len += sprintf(xml_data + tmp_len, "			<port>%d</port>\r\n",pServiceInf->service_net.ip_port_domain[i].port);
			tmp_len += sprintf(xml_data + tmp_len, "		</server>\r\n");
		}
	}
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetServInfRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetReStartCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_REBOOT_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseSetReStartRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
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
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_AGE_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	int new_age = ++ pColtorInf->col_conf.age;
	tmp_len += sprintf(xml_data + tmp_len, "		<age>%d</age>\r\n", new_age);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetAgeRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormGetAgeCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[READ_AGE_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseGetAgeRsp()
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

	node = f_xmlnode_sch_elem(base_root, "age");
	if (node != NULL)
	{
		str_to_int(node->data,pColtorInf->col_conf.age,gHexTypeXml);
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

void CXmlAnalyze::SavePreOuterDev()
{
	memcpy(pPreOuterDev,pOuterDev,sizeof(*pPreOuterDev));
}

int CXmlAnalyze::FormDelDevInfCmd(st_dev_change_inf & dev_inf)
{
	ClearBuff();
	int bodyLen = 0;//数据包消息体长度0-n
	int tmpLen = MESSAGE_BODY_IND;
	int port_ind = dev_inf.dev_coord.port_ind;
	int pre_sub_type = dev_inf.old_dev_inf.sub_type;
	MU_INT64 id = dev_inf.old_dev_inf.dev_addr.id;

	tmpLen += sprintf(pBuff + tmpLen, "<req>\r\n");
	tmpLen += sprintf(pBuff + tmpLen, "<addr>%d,%lld,%d</addr>\r\n",port_ind,id,pre_sub_type);
	tmpLen += sprintf(pBuff + tmpLen, "</req>\r\n");

	bodyLen = tmpLen - MESSAGE_BODY_IND;
	//FormMessage(bodyLen, DEL_DEV);
	return tmpLen;
}

int CXmlAnalyze::ParseDelDevInfRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::ParseModDevInfRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormModDevInfCmd( st_dev_change_inf & dev_inf)
{
	ClearBuff();
	int tmpLen = MESSAGE_BODY_IND;//消息体起始位置
	int bodyLen = 0;
	int port_ind = dev_inf.dev_coord.port_ind;

	MU_INT64 pre_id = dev_inf.old_dev_inf.dev_addr.id;
	e_dev_sub_type pre_sub_type = dev_inf.old_dev_inf.sub_type;

	tmpLen += sprintf(pBuff + tmpLen, "<req>\r\n");
	tmpLen += sprintf(pBuff + tmpLen, "<addr>%d,%lld,%d</addr>\r\n",port_ind,pre_id,pre_sub_type);

	tmpLen += sprintf(pBuff + tmpLen, "<dev>");
	tmpLen += sprintf(pBuff + tmpLen, "<d_addr>%lld</d_addr>",dev_inf.dev_inf.dev_addr.id);
	tmpLen += sprintf(pBuff + tmpLen, "<d_name>%s</d_name>",dev_inf.dev_inf.name);
	tmpLen += sprintf(pBuff + tmpLen, "<d_sub_type>%d</d_sub_type>",dev_inf.dev_inf.sub_type);
	tmpLen += sprintf(pBuff + tmpLen, "<d_opt>%d,%d</d_opt>",
		dev_inf.dev_inf.rs485_conf.baud,
		dev_inf.dev_inf.rs485_conf.pari);
	tmpLen += sprintf(pBuff + tmpLen, "<w_mask>%d</w_mask>",dev_inf.dev_inf.way_inf.way_mask);

	int way_num = GetWayNumBySubTypeValue(dev_inf.dev_inf.sub_type);
	for(int j = 0; j < way_num ; j++)
	{
		if (0 != strlen(dev_inf.dev_inf.way_inf.way_data[j].way_name))
		{
			tmpLen += sprintf(pBuff + tmpLen, "<w_name>%d,%s</w_name>",j,dev_inf.dev_inf.way_inf.way_data[j].way_name);
		}
	}
	tmpLen += sprintf(pBuff + tmpLen, "</dev>");

	tmpLen += sprintf(pBuff + tmpLen, "</req>\r\n");

	bodyLen = tmpLen - MESSAGE_BODY_IND;
	//FormMessage(bodyLen, SET_DEV);	
	return tmpLen;
}

void CXmlAnalyze::GetDevChange()
{
	int port_num = pOuterDev->port_num;
	int pre_dev_num = 0;
	int cur_dev_num = 0;
	bool bret = 0;
	st_dev_change_inf dev_change;
	ZeroMemory(&dev_change,sizeof(dev_change));
	//devInf[],只是CList中的顺序，要用devCoordinate中的
	vecDevComp.clear();
	//add
	for (int i = 0; i < port_num; i++)
	{
		cur_dev_num = pOuterDev->port_dev[i].dev_num;
		for (int j = 0; j < cur_dev_num; j++)
		{
			bret = IsAdd(pOuterDev->port_dev[i].dev_inf[j],i);
			if (bret)
			{
				//新增
				ZeroMemory(&dev_change,sizeof(dev_change));
				dev_change.dev_coord.port_ind = i;
				dev_change.state = ADD_DEV_COMP;
				memcpy(&dev_change.dev_inf,&pOuterDev->port_dev[i].dev_inf[j],sizeof(dev_change.dev_inf));
				vecDevComp.push_back(dev_change);
			}
			else
			{
				//已存在
				//判断是否修改
				bret = IsModify(pOuterDev->port_dev[i].dev_inf[j],i);
				if (bret)
				{
					//修改
					ZeroMemory(&dev_change,sizeof(dev_change));
					dev_change.dev_coord.port_ind = i;
					dev_change.dev_coord.dev_ind = 
						GetPreDevIndByIdSubType(pOuterDev->port_dev[i].dev_inf[j].dev_addr.id,
						pOuterDev->port_dev[i].dev_inf[j].sub_type,i);
					dev_change.state = MODIFY_DEV_COMP;

					memcpy(&dev_change.dev_inf,&pOuterDev->port_dev[i].dev_inf[j],sizeof(dev_change.dev_inf));
					vecDevComp.push_back(dev_change);
				}
				else
				{
					//未修改
				}
			}

		}
	}

	//delete
	for (int i = 0; i < port_num; i++)
	{
		pre_dev_num = pPreOuterDev->port_dev[i].dev_num;
		for (int j = 0; j < pre_dev_num; j++)
		{
			bret = IsDelete(pPreOuterDev->port_dev[i].dev_inf[j],i);
			if (bret)
			{
				//被删除
				ZeroMemory(&dev_change,sizeof(dev_change));
				dev_change.dev_coord.port_ind = i;
				dev_change.dev_coord.dev_ind = j;
				dev_change.state = DELETE_DEV_COMP;

				memcpy(&dev_change.dev_inf,&pPreOuterDev->port_dev[i].dev_inf[j],sizeof(dev_change.dev_inf));
				vecDevComp.push_back(dev_change);
			}
			else
			{
				//未被删除
			}
		}
	}

	int nMap = 0;
	nMap = vecDevComp.size();
	return;
}

bool CXmlAnalyze::IsAdd(st_dev_inf& dev,int port_ind)
{
	MU_INT64 id = dev.dev_addr.id;
	e_dev_sub_type sub_type = dev.sub_type;
	int pre_dev_num = pPreOuterDev->port_dev[port_ind].dev_num;

	for (int i = 0; i < pre_dev_num; i++)
	{
		if(id == pPreOuterDev->port_dev[port_ind].dev_inf[i].dev_addr.id &&
			sub_type == pPreOuterDev->port_dev[port_ind].dev_inf[i].sub_type)
		{
			return false;
		}
	}
	return true;
}

bool CXmlAnalyze::IsDelete(st_dev_inf& dev,int port_ind)
{
	MU_INT64 id = dev.dev_addr.id;
	e_dev_sub_type sub_type = dev.sub_type;

	int cur_dev_num = pOuterDev->port_dev[port_ind].dev_num;

	for (int i = 0; i < cur_dev_num; i++)
	{
		if(id == pOuterDev->port_dev[port_ind].dev_inf[i].dev_addr.id &&
			sub_type == pOuterDev->port_dev[port_ind].dev_inf[i].sub_type)
		{
			return false;
		}
	}
	return true;
}

bool CXmlAnalyze::IsModify(st_dev_inf& dev,int port_ind)
{
	MU_INT64 id = dev.dev_addr.id;
	e_dev_sub_type sub_type = dev.sub_type;

	int pre_dev_num = pPreOuterDev->port_dev[port_ind].dev_num;

	for (int i = 0; i < pre_dev_num; i++)
	{
		if(id == pPreOuterDev->port_dev[port_ind].dev_inf[i].dev_addr.id &&
			sub_type == pPreOuterDev->port_dev[port_ind].dev_inf[i].sub_type)
		{
			if (0 != memcmp(&dev,&pPreOuterDev->port_dev[port_ind].dev_inf[i],sizeof(dev)))
			{
				return true;
			}
		}
	}
	return false;
}

int CXmlAnalyze::GetPreDevIndByIdSubType( MU_INT64 id,e_dev_sub_type sub_type,int port_ind)
{
	int pre_dev_num = pPreOuterDev->port_dev[port_ind].dev_num;

	for (int i = 0; i < pre_dev_num; i++)
	{
		if(id == pPreOuterDev->port_dev[port_ind].dev_inf[i].dev_addr.id &&
			sub_type == pPreOuterDev->port_dev[port_ind].dev_inf[i].sub_type)
		{
			return i;
		}
	}
	return -1;
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

int CXmlAnalyze::SaveLocalXml( const char *pfile )
{
	FILE *pf=NULL;
	int len = 0;
	int rtn = 0;

	len = FormLocalConfigXml();

	MU_INT8*data = (MU_INT8*)gXmlAnalyze.pBuff;
	encry_local_data(data, len);

	pf = fopen(pfile,"wb");
	if(pf)
	{
		fwrite(pBuff,sizeof(char),len,pf);
		fclose(pf);		
	}
	else
	{
		return -1;
	}	

	return rtn;
}

int CXmlAnalyze::FormLocalConfigXml()
{
	ClearBuff();
	int tmp_len = 0;
	tmp_len += sprintf(pBuff + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "<save>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "	<time>%d-%d-%d,%d:%d:%d</time>\r\n",
		pColtorInf->dev_time.dev_time.YY,pColtorInf->dev_time.dev_time.MM,pColtorInf->dev_time.dev_time.DD,
		pColtorInf->dev_time.dev_time.HH,pColtorInf->dev_time.dev_time.MI,pColtorInf->dev_time.dev_time.SS);
	tmp_len += sprintf(pBuff + tmp_len, "	<collector>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "		<addr>%d</addr>\r\n",pColtorInf->col_conf.id);
	tmp_len += sprintf(pBuff + tmp_len, "		<type>%d</type>\r\n",pColtorInf->col_conf.type);
	tmp_len += sprintf(pBuff + tmp_len, "		<name>%s</name>\r\n",pColtorInf->col_conf.name);
	tmp_len += sprintf(pBuff + tmp_len, "		<pos>%s</pos>\r\n",pColtorInf->col_conf.pos_name);
	tmp_len += sprintf(pBuff + tmp_len, "		<sample_m>%d</sample_m>\r\n",pColtorInf->col_conf.sample_m);
	tmp_len += sprintf(pBuff + tmp_len, "		<report_m>%d</report_m>\r\n",pColtorInf->col_conf.report_m);
	tmp_len += sprintf(pBuff + tmp_len, "		<heart_s>%d</heart_s>\r\n",pColtorInf->col_conf.heart_s);
	tmp_len += sprintf(pBuff + tmp_len, "		<if_num>%d</if_num>\r\n",pOuterDev->port_num);
	tmp_len += sprintf(pBuff + tmp_len, "		<hw_ver>%s</hw_ver>\r\n",pColtorInf->col_conf.hw_ver);
	tmp_len += sprintf(pBuff + tmp_len, "		<sw_ver>%s</sw_ver>\r\n",pColtorInf->col_conf.sw_ver); 
	tmp_len += sprintf(pBuff + tmp_len, "	</collector>\r\n");

	tmp_len += sprintf(pBuff + tmp_len, "	<build>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "		<area_id>%d</area_id>\r\n",
		pColtorInf->build_conf.build_pos.county);
	tmp_len += sprintf(pBuff + tmp_len, "		<type>%d</type>\r\n",pColtorInf->build_conf.build_type);
	tmp_len += sprintf(pBuff + tmp_len, "		<id>%d</id>\r\n", pColtorInf->build_conf.build_code);
	tmp_len += sprintf(pBuff + tmp_len, "		<name>%s</name>\r\n", pColtorInf->build_conf.build_name);
	tmp_len += sprintf(pBuff + tmp_len, "		<pos>%s</pos>\r\n", pColtorInf->build_conf.build_pos.pos_name);
	tmp_len += sprintf(pBuff + tmp_len, "	</build>\r\n");

	//增加自定义子类型
	/*
	<def_sub_type>
	<def_sub_type_num>2</def_sub_type_num>

	<user_sub_type>
	<type>1</type>
	<sub_type>111</sub_type>
	<sub_type_name>xxxxxx</sub_type_name>
	<w_num>4</w_num>
	<reg_info>
	<reg>1,2,1</reg>   //addr, len, unit
	<reg>4,2,1</reg>
	</reg_info>
	</user_sub_type>	
	...
	</def_sub_type>
	*/

	tmp_len += sprintf(pBuff + tmp_len, "	<user_def_type>\r\n");
	int def_sub_type_num = 0;
	int def_modbus_num = pCProtModbusSet->protModbusNum;

	for (int def_sub_type_ind = 0; 
		def_sub_type_ind < def_modbus_num; def_sub_type_ind++)
	{
		if (pCProtModbusSet->pCProtModbusArr[def_sub_type_ind].pStDefSubTypeInfo->dev_type == 0)
		{
			continue;
		}
		def_sub_type_num ++;
	}

	tmp_len += sprintf(pBuff + tmp_len, "		<def_type_num>%d</def_type_num>\r\n", def_sub_type_num);

	for (int def_sub_type_ind = 0; 
		def_sub_type_ind < def_modbus_num; def_sub_type_ind++)
	{
		CProtModbus* p_CProtModbus = 
			&pCProtModbusSet->pCProtModbusArr[def_sub_type_ind];

		if (p_CProtModbus->pStDefSubTypeInfo->dev_type == 0)
		{
			continue;
		}

		int dev_ind = GetDevTypeIndByValue(p_CProtModbus->pStDefSubTypeInfo->dev_type);

		CParamModbus* p_CParamModbus = p_CProtModbus->pCParamModbus;
		tmp_len += sprintf(pBuff + tmp_len, "		<user_sub_type>\r\n");
		tmp_len += sprintf(pBuff + tmp_len, "			<type>%d</type>\r\n", 
			p_CProtModbus->pStDefSubTypeInfo->dev_type);
		tmp_len += sprintf(pBuff + tmp_len, "			<sub_type>%d</sub_type>\r\n",
			p_CProtModbus->pStDefSubTypeInfo->sub_type);
		tmp_len += sprintf(pBuff + tmp_len, "			<sub_type_name>%s</sub_type_name>\r\n",
			p_CProtModbus->pStDefSubTypeInfo->sub_type_name);
		int sub_type = GetSubTypeIndByValue(p_CProtModbus->pStDefSubTypeInfo->sub_type);
		assert(sub_type >= 0);
		tmp_len += sprintf(pBuff + tmp_len, "			<w_num>%d</w_num>\r\n",
			gWayCountValue[sub_type]);
		bool reg_is_same = p_CProtModbus->pStDefSubTypeInfo->is_reg_same;
		tmp_len += sprintf(pBuff + tmp_len, "			<reg_is_same>%d</reg_is_same>\r\n",
			reg_is_same);

		int max_way_num = p_CParamModbus->paramDevTypeNum;
		for (int way_ind = 0; way_ind < max_way_num; way_ind++)
		{
			if (reg_is_same && way_ind > 0)
			{
				break;
			}

			CParamDevType* p_CParamDevSingle = &p_CParamModbus->pCParamDevTypeArr[way_ind];

			tmp_len += sprintf(pBuff + tmp_len, "			<reg_info>\r\n");
			for (int i = 0; i < p_CParamDevSingle->paramInfNum; i++)
			{
				tmp_len += sprintf(pBuff + tmp_len, "				<reg>%d,%d,%d</reg>\r\n",
					p_CParamDevSingle->pStParamInfoArr[i].param_register_addr,
					p_CParamDevSingle->pStParamInfoArr[i].param_register_len,
					p_CParamDevSingle->pStParamInfoArr[i].param_unit
					);
			}
			tmp_len += sprintf(pBuff + tmp_len, "			</reg_info>\r\n");
		}

		tmp_len += sprintf(pBuff + tmp_len, "		</user_sub_type>\r\n");	
	}
	tmp_len += sprintf(pBuff + tmp_len, "	</user_def_type>\r\n");

	for(int i = 0; i < pOuterDev->port_num; i++)
	{
		tmp_len += sprintf(pBuff + tmp_len, "	<port>\r\n");
		for(int j = 0; j < pOuterDev->port_dev[i].dev_num; j++)
		{
			tmp_len += sprintf(pBuff + tmp_len, "		<dev>\r\n");
			tmp_len += sprintf(pBuff + tmp_len, "			<addr>%lld</addr>\r\n",
				pOuterDev->port_dev[i].dev_inf[j].dev_addr.id);          
			tmp_len += sprintf(pBuff + tmp_len, "			<type>%d</type>\r\n",
				pOuterDev->port_dev[i].dev_inf[j].dev_type);
			tmp_len += sprintf(pBuff + tmp_len, "			<sub_type>%d</sub_type>\r\n",
				pOuterDev->port_dev[i].dev_inf[j].sub_type);
			tmp_len += sprintf(pBuff + tmp_len, "			<name>%s</name>\r\n",
				pOuterDev->port_dev[i].dev_inf[j].name);
			tmp_len += sprintf(pBuff + tmp_len, "			<pos>%s</pos>\r\n",
				pOuterDev->port_dev[i].dev_inf[j].pos_name);
			tmp_len += sprintf(pBuff + tmp_len, "			<opt>%d,%d,%d,%d</opt>\r\n",
				pOuterDev->port_dev[i].dev_inf[j].rs485_conf.databits,
				pOuterDev->port_dev[i].dev_inf[j].rs485_conf.pari,
				pOuterDev->port_dev[i].dev_inf[j].rs485_conf.stop,
				pOuterDev->port_dev[i].dev_inf[j].rs485_conf.baud);
			tmp_len += sprintf(pBuff + tmp_len, "			<factory>\r\n");
			tmp_len += sprintf(pBuff + tmp_len, "				<hw_ver>%s</hw_ver>\r\n",
				pOuterDev->port_dev[i].dev_inf[j].hw_ver);
			tmp_len += sprintf(pBuff + tmp_len, "				<sw_ver>%s</sw_ver>\r\n",
				pOuterDev->port_dev[i].dev_inf[j].sw_ver);
			tmp_len += sprintf(pBuff + tmp_len, "			</factory>\r\n");
			tmp_len += sprintf(pBuff + tmp_len, "			<way_mask>%02x</way_mask>\r\n",
				pOuterDev->port_dev[i].dev_inf[j].way_inf.way_mask);

			int way_num = GetWayNumBySubTypeValue(pOuterDev->port_dev[i].dev_inf[j].sub_type);
			for(int k = 0; k < way_num; k++)
			{
				bool way_used = IsWayUsedByMask(k, pOuterDev->port_dev[i].dev_inf[j].way_inf.way_mask);
				if (way_used)
				{
					st_way_data way_data;
					memset(&way_data, 0, sizeof(way_data));
					memcpy(&way_data, &pOuterDev->port_dev[i].dev_inf[j].way_inf.way_data[k], sizeof(way_data));
					tmp_len += sprintf(pBuff + tmp_len, "			<way>\r\n");
					tmp_len += sprintf(pBuff + tmp_len, "				<id>%d</id>\r\n",way_data.way_id);
					tmp_len += sprintf(pBuff + tmp_len, "				<class_id>%d</class_id>\r\n",way_data.class_id);
					tmp_len += sprintf(pBuff + tmp_len, "				<item_id>%d</item_id>\r\n",way_data.item_id);
					tmp_len += sprintf(pBuff + tmp_len, "				<addr>%d</addr>\r\n",way_data.way_addr);
					tmp_len += sprintf(pBuff + tmp_len, "				<type>%d</type>\r\n",way_data.way_type);
					tmp_len += sprintf(pBuff + tmp_len, "				<name>%s</name>\r\n",way_data.way_name);
					tmp_len += sprintf(pBuff + tmp_len, "				<pos>%s</pos>\r\n",way_data.pos_name);
					tmp_len += sprintf(pBuff + tmp_len, "				<detail>%s</detail>\r\n",way_data.detail);
					tmp_len += sprintf(pBuff + tmp_len, "			</way>\r\n");
				}
			}
			tmp_len += sprintf(pBuff + tmp_len, "		</dev>\r\n");
		}
		tmp_len += sprintf(pBuff + tmp_len, "	</port>\r\n");
	}

	tmp_len += sprintf(pBuff + tmp_len, "	<secret>%s</secret>\r\n",
		pServiceInf->service_net.secret);
	for(int i = 0; i < MAX_SERVICE_NUM; i++)
	{
		if (true)//(pServiceInf->service_net.ip_port_domain[i].valid)
		{
			tmp_len += sprintf(pBuff + tmp_len, "	<server>\r\n");
			tmp_len += sprintf(pBuff + tmp_len, "		<used>%d</used>\r\n",pServiceInf->service_net.ip_port_domain[i].valid);
			tmp_len += sprintf(pBuff + tmp_len, "		<domain>%s</domain>\r\n",pServiceInf->service_net.ip_port_domain[i].domain);
			tmp_len += sprintf(pBuff + tmp_len, "		<port>%d</port>\r\n",pServiceInf->service_net.ip_port_domain[i].port);
			tmp_len += sprintf(pBuff + tmp_len, "	</server>\r\n");
		}
	}

	tmp_len += sprintf(pBuff + tmp_len, "	<eth>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "		<used>%d</used>\r\n",pServiceInf->coltor_net.eth.used);
	tmp_len += sprintf(pBuff + tmp_len, "		<dhcp>%d</dhcp>\r\n",pServiceInf->coltor_net.eth.netType);
	tmp_len += sprintf(pBuff + tmp_len, "		<ip>%s</ip>\r\n",pServiceInf->coltor_net.eth.ip);
	tmp_len += sprintf(pBuff + tmp_len, "		<mask>%s</mask>\r\n",pServiceInf->coltor_net.eth.mask);
	tmp_len += sprintf(pBuff + tmp_len, "		<gw>%s</gw>\r\n",pServiceInf->coltor_net.eth.gateWay);
	tmp_len += sprintf(pBuff + tmp_len, "		<dns>%s</dns>\r\n",pServiceInf->coltor_net.eth.dns);
	tmp_len += sprintf(pBuff + tmp_len, "		<mac>%s</mac>\r\n",pServiceInf->coltor_net.eth.mac);
	tmp_len += sprintf(pBuff + tmp_len, "	</eth>\r\n");

	tmp_len += sprintf(pBuff + tmp_len, "	<hotspot>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "		<ssid>%s</ssid>\r\n",pServiceInf->coltor_net.hotspot.ssid);
	tmp_len += sprintf(pBuff + tmp_len, "		<passphrase>%s</passphrase>\r\n",pServiceInf->coltor_net.hotspot.passphrase);
	tmp_len += sprintf(pBuff + tmp_len, "	</hotspot>\r\n");

	tmp_len += sprintf(pBuff + tmp_len, "	<report_format>\r\n");
	for (int i = 0; i < pReportFormatStoreInf->report_format.dev_type_num; i++)
	{
		tmp_len += sprintf(pBuff + tmp_len, "		<dev>\r\n");
		tmp_len += sprintf(pBuff + tmp_len, "			<type>%d</type>\r\n",pReportFormatStoreInf->report_format.dev_type_report[i].dev_type);
		string report_id_data;
		for (int j = 0; j < pReportFormatStoreInf->report_format.dev_type_report[i].param_num; j++)
		{
			CString tmp_id;
			tmp_id.Format("%d",pReportFormatStoreInf->report_format.dev_type_report[i].report_param_id[j]);
			report_id_data += tmp_id;
			if (j < pReportFormatStoreInf->report_format.dev_type_report[i].param_num - 1)
			{
				report_id_data += ",";
			}
		}
		tmp_len += sprintf(pBuff + tmp_len, "			<id>%s</id>\r\n", report_id_data.c_str());
		tmp_len += sprintf(pBuff + tmp_len, "		</dev>\r\n");
	}
	tmp_len += sprintf(pBuff + tmp_len, "	</report_format>\r\n");

	tmp_len += sprintf(pBuff + tmp_len, "</save>\r\n");

	return tmp_len;
}

int CXmlAnalyze::ParseLocalXml()
{
	MS_INT32 rtn = 0;
	XMLNODE *base_root = NULL;
	XMLNODE *node = NULL;
	XML xml;
	CTime tm; 
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

	XMLNODE *time_root = NULL;
	time_root = base_root;
	rtn = ParseTimeNode(time_root);
	if (rtn < 0)
	{
		goto leave;
	}
	//加载本地配置，采集器时间必须保证最新系统时间
	tm = CTime::GetCurrentTime();//获得当前时间 
	pColtorInf->dev_time.dev_time.YY = tm.GetYear();
	pColtorInf->dev_time.dev_time.MM= tm.GetMonth();
	pColtorInf->dev_time.dev_time.DD= tm.GetDay(); 
	pColtorInf->dev_time.dev_time.HH= tm.GetHour();
	pColtorInf->dev_time.dev_time.MI= tm.GetMinute();
	pColtorInf->dev_time.dev_time.SS= tm.GetSecond();
	pColtorInf->dev_time.local_time = GetTickCount();

	XMLNODE *collector_root = NULL;
	node = f_xmlnode_sch_elem(base_root, "collector");
	if (node != NULL)
	{
		collector_root= node;
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	rtn = ParseCollectorNode(collector_root);
	if (rtn < 0)
	{
		goto leave;
	}

	XMLNODE *build_root = NULL;
	node = f_xmlnode_sch_elem(base_root, "build");
	if (node != NULL)
	{
		build_root = node;
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	rtn = ParseBuildNode(build_root);
	if (rtn < 0)
	{
		goto leave;
	}

	XMLNODE *eth_root = NULL;
	node = f_xmlnode_sch_elem(base_root, "eth");
	if (node != NULL)
	{
		eth_root = node;
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	rtn = ParseEthNode(eth_root);
	if (rtn < 0)
	{
		goto leave;
	}
	//加载本地配置时的mac地址确保最新
	memset(pServiceInf->coltor_net.eth.mac, 0, sizeof(pServiceInf->coltor_net.eth.mac));

	XMLNODE *hotspot_root = NULL;
	node = f_xmlnode_sch_elem(base_root, "hotspot");
	if (node != NULL)
	{
		hotspot_root = node;
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	rtn = ParseHotSpotNode(hotspot_root);
	if (rtn < 0)
	{
		goto leave;
	}

	//def_sub_type
	int def_sub_type_num = 0;
	XMLNODE* def_sub_type_root = NULL;//总
	XMLNODE* child_def_sub_type_root = NULL;//子
	XMLNODE* child_reg_inf_root = NULL;//子
	node = f_xmlnode_sch_elem(base_root, "user_def_type");
	if (node != NULL)
	{
		def_sub_type_root = node;
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(def_sub_type_root, "def_type_num");
	if (node != NULL)
	{
		str_to_int(node->data, def_sub_type_num);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	for (int i = 0; i < def_sub_type_num; i++)
	{
		if (i == 0)
		{
			node = f_xmlnode_sch_elem(def_sub_type_root, "user_sub_type");
			assert(node);
			child_def_sub_type_root = node;
		}
		else
		{
			node = f_xmlnode_nxt_node(child_def_sub_type_root, "user_sub_type");
			assert(node);
			child_def_sub_type_root = node;
		}
		rtn = ParseDefSubTypeConfNode(child_def_sub_type_root, i);
		if (rtn < 0)
		{
			rtn = -1;
			goto leave;
		}

		bool is_reg_same 
			= gXmlAnalyze.pCProtModbusSet->pCProtModbusArr[i]
		.pStDefSubTypeInfo->is_reg_same;
		int way_num 
			= gXmlAnalyze.pCProtModbusSet->pCProtModbusArr[i]
		.pStDefSubTypeInfo->way_num;
		if (is_reg_same)
		{
			way_num = (way_num > 0) ? 1 : way_num;
		}
		else
		{
			way_num = way_num;
		}

		for (int way_ind = 0; way_ind < way_num; way_ind++)
		{
			if (way_ind == 0)
			{
				node = f_xmlnode_sch_elem(child_def_sub_type_root, "reg_info");
				assert(node);
				child_reg_inf_root = node;
			}
			else
			{
				node = f_xmlnode_nxt_node(child_reg_inf_root, "reg_info");
				assert(node);
				child_reg_inf_root = node;
			}

			rtn = ParseRegisterInfNode(child_reg_inf_root, i, way_ind);
			if (rtn != 0)
			{
				rtn = -1;
				goto leave;
			}
		}
	}

	//dev
	XMLNODE *port_root = NULL;
	int port_num = GetSubNodeNum(base_root, "port");
	pOuterDev->port_num = port_num;
	for (int i = 0; i < port_num; i++)
	{
		if (i == 0)
		{
			node = f_xmlnode_sch_elem(base_root, "port");
			assert(node);
			port_root = node;
		}
		else
		{
			node = f_xmlnode_nxt_node(port_root, "port");
			assert(node);
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
				assert(node);
				dev_root = node;
			}
			else
			{
				node = f_xmlnode_nxt_node(dev_root, "dev");
				assert(node);
				dev_root = node;
			}
			rtn = ParsePortDevNode(dev_root, i, j);
			if (rtn < 0)
			{
				goto leave;
			}
		}
	}

	rtn = ParseServInfNode(base_root);
	if (rtn < 0)
	{
		goto leave;
	}

	XMLNODE *report_format_root = NULL;
	node = f_xmlnode_sch_elem(base_root, "report_format");
	if (node != NULL)
	{
		report_format_root = node;
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	rtn = ParseReportFormatNode(report_format_root);
	if (rtn < 0)
	{
		goto leave;
	}

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::GetSubNodeNum( XMLNODE *base_root, const char* name )
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

int CXmlAnalyze::FormGetRealTimeDataCmd(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type, int way_ind)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	int dev_ind = GetDevIndByIdSubType(pOuterDev, dev_id, sub_type, port_ind);
	assert(dev_ind >= 0);

	int meter_id = pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].way_id;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<building_id>%d</building_id>\r\n", 0);
	tmp_len += sprintf(xml_data + tmp_len, "		<gateway_id>%d</gateway_id>\r\n", 0);
	tmp_len += sprintf(xml_data + tmp_len, "		<meter_id>%d</meter_id>\r\n", meter_id);
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[READ_REALTIME_DATA_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<net_query operation=\"%s\">\r\n",mapCmdIdName[READ_REALTIME_DATA_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "		<query>%s</query>\r\n",mapCmdIdName[READ_REALTIME_DATA_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</net_query>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseGetRealTimeDataRsp(u_w_measure& w_measure,e_dev_type dev_type)
{
	MS_INT32 rtn = 0;
	XMLNODE* base_root = NULL;
	XMLNODE* node = NULL;
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

	node = f_xmlnode_sch_elem(base_root, "data");
	if (node != NULL)
	{
		base_root = node;
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "meter");
	if (node != NULL)
	{
		base_root = node;
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	rtn = ParseRealTimeDataNode(base_root, w_measure, dev_type);
	if (rtn < 0)
	{
		rtn = -1;
		goto leave;
	}

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormSetStartUpgradCmd(int pack_num, int pack_len_max)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_START_UPGRADE_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<pack_num>%d</pack_num>\r\n", pack_num);
	tmp_len += sprintf(xml_data + tmp_len, "		<pack_len>%d</pack_len>\r\n", pack_len_max*2);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetStartUpgradRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetUpgradDataCmd(int pack_ind,char* data,int len)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_UPGRADE_DATA_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<pack_order>%d</pack_order>\r\n", pack_ind);
	tmp_len += sprintf(xml_data + tmp_len, "		<data>");
	for (int i = 0; i < len; i++)
	{
		tmp_len += sprintf(xml_data + tmp_len, "%02x",(MU_INT8)data[i]);
	}
	tmp_len += sprintf(xml_data + tmp_len, "</data>");
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetUpgradDataRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetEndUpgradCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_END_UPGRADE_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseSetEndUpgradRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::LoadFile( const char *pfile )
{
	FILE *pf=NULL;
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
		MU_INT8*data = (MU_INT8*)gXmlAnalyze.pBuff;
		//本地配置暂不加密
		decry_local_data(data, len);
		return 0;
	}
	else
	{
		return -2;
	}


}

int CXmlAnalyze::FormGetHistoryDataCmd(int pre_result)
{
	ClearBuff();
	int bodyLen = 0;//数据包消息体长度0-n
	int tmpLen = MESSAGE_BODY_IND;

	tmpLen += sprintf(pBuff + tmpLen, "<req>\r\n");
	tmpLen += sprintf(pBuff + tmpLen, "<pre_result>%d</pre_result>\r\n",pre_result);
	tmpLen += sprintf(pBuff + tmpLen, "</req>\r\n");

	bodyLen = tmpLen - MESSAGE_BODY_IND;
	//FormMessage(bodyLen, READ_HISTORYDATA);
	return tmpLen;
}

int CXmlAnalyze::ParseGetHistoryDataRsp(VEC_HISTORY_XML& vec_history_xml)
{
	bool ret = false;
	int cnt = 0;
	int ind = 0;
	char data[1024];
	MS_INT32 rt = 0;
	MS_INT32 rtn = 0;
	XMLNODE *base_root=NULL;
	XMLNODE *node = NULL;
	XML xml;

	memset(&xml,0,sizeof(xml));
	ZeroMemory(data,sizeof(data));

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

	if(strcmp(xml.root->name,"resp")!=0)
	{
		rtn = -1;
		goto leave;
	}	

	base_root = xml.root;

	node = f_xmlnode_sch_elem(base_root, "item");//可能多个外设
	if (node == NULL)//无item项
	{
		rtn = -1;
		goto leave;
	}
	base_root = node;
	while(node != NULL)
	{
		st_history_xml history_inf;
		ZeroMemory(&history_inf,sizeof(history_inf));
		node = f_xmlnode_sch_elem(base_root,"d_type");
		if (node != NULL)
		{
			int type = 0;
			str_to_int(node->data,type,gHexTypeXml);
			history_inf.dev_type = (e_dev_type)type;
		}
		else
		{
			rtn = -1;
			goto leave;
		}

		node = f_xmlnode_sch_elem(base_root,"addr");
		if (node != NULL)
		{
			string str = node->data;
			vector<string> vecElem;
			int ret = SplitToString(str, vecElem, ',');
			if (ret == 3)
			{
				str_to_int(vecElem[0].c_str(),history_inf.port_ind,gHexTypeXml);
				str_to_int(vecElem[1].c_str(),history_inf.dev_ind,gHexTypeXml);
				str_to_int(vecElem[2].c_str(),history_inf.way_ind,gHexTypeXml);
			}	
		}
		else
		{
			rtn = -1;
			goto leave;
		}

		node = f_xmlnode_sch_elem(base_root,"data");
		if (node != NULL)
		{
			strcpy(data,node->data);
			strcpy(history_inf.data,node->data);
		}
		else
		{
			rtn = -1;
			goto leave;
		}
		rtn = ExtractWayData(history_inf.w_measure,history_inf.dev_type,data);
		int len_len = strlen(data);
		vec_history_xml.push_back(history_inf);
		node = f_xmlnode_nxt_node(base_root,"item");
		ind ++;
	}

leave:
	f_xmlnode_del(&xml);
	if (rtn == -1)
	{
		return rtn;
	}
	return rtn;
}

int CXmlAnalyze::FormSaveHistoryDataCmd()
{
	ClearBuff();
	int bodyLen = 0;//数据包消息体长度0-n
	int tmpLen = MESSAGE_BODY_IND;

	bodyLen = tmpLen - MESSAGE_BODY_IND;
	//FormMessage(bodyLen, SAVE_HISTORYDATA);
	return tmpLen;
}

//csv
int CXmlAnalyze::ParseSaveHistoryDataRsp(CString path, bool use_head)
{
	CString file_name;
	CString file_path;
	static char has_write[32] = "";
	if (use_head)
	{
		ZeroMemory(has_write,sizeof(has_write));
	}
	VEC_HISTORY_XML vec_history_xml;
	int ret = ParseGetHistoryDataRsp(vec_history_xml);
	if (ret == 0)
	{
		for (MU_INT32 i = 0; i < vec_history_xml.size(); i ++)
		{
			ClearBuff();
			int tmpLen = 0;
			tmpLen += sprintf(pBuff + tmpLen, "%s,",GetDevTypeNameByValue(vec_history_xml[i].dev_type));
			tmpLen += sprintf(pBuff + tmpLen, "%d,%lld,%d,",
				vec_history_xml[i].port_ind,vec_history_xml[i].dev_ind,vec_history_xml[i].way_ind);

			ConvertDataToCvs( vec_history_xml[i].w_measure,vec_history_xml[i].dev_type,pBuff + tmpLen);

			//写csv
			file_name =  GetSaveFileNameByDevType( vec_history_xml[i].dev_type );
			//path = g_CurrentDir;
			file_path = path + "\\" + file_name;

			if (has_write[vec_history_xml[i].dev_type] == 0)
			{
				//写头
				CString str_head = "设备类型,接口,电表地址,回路地址,";
				str_head +=	GetSaveFileHeadByDevType(vec_history_xml[i].dev_type);
				ret = WriteCVS(str_head.GetString(),file_path);
				if (ret != 0)
				{
					return -1;
				}
				has_write[vec_history_xml[i].dev_type] = 1;
			}

			ret = WriteCVS(pBuff,file_path);
			if (ret != 0)
			{
				return -2;
			}
		}

		return 0;
	}
	else
	{
		return -3;
	}
}

int CXmlAnalyze::FormSetStartSaveHistoryDataCmd()
{
	ClearBuff();
	int bodyLen = 0;//数据包消息体长度0-n
	int tmpLen = MESSAGE_BODY_IND;

	bodyLen = tmpLen - MESSAGE_BODY_IND;
	//FormMessage(bodyLen, SET_STARTSAVEHISTORYDATA);
	return tmpLen;
}

int CXmlAnalyze::ParseSetStartSaveHistoryRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetEndSaveHistoryDataCmd()
{
	ClearBuff();
	int bodyLen = 0;//数据包消息体长度0-n
	int tmpLen = MESSAGE_BODY_IND;

	bodyLen = tmpLen - MESSAGE_BODY_IND;
	//FormMessage(bodyLen, SET_ENDSAVEHISTORYDATA);
	return tmpLen;
}

int CXmlAnalyze::ParseSetEndSaveHistoryRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormClearDevInfCmd()
{
	ClearBuff();
	int bodyLen = 0;//数据包消息体长度0-n
	int tmpLen = MESSAGE_BODY_IND;
	bodyLen = tmpLen - MESSAGE_BODY_IND;
	//FormMessage(bodyLen, CLEAR_DEV);
	return tmpLen;
}

int CXmlAnalyze::ParseClearDevInfRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetCleanHisDataCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_CLEAN_HISTORYDATA_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseSetCleanHisDataRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetStartReportCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_START_REPORT_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseSetStartReportRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetEndReportCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_END_REPORT_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseSetEndReportRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::InitCmdIdName()
{
	mapCmdIdName[READ_TIME_CMD] = "read_time";
	mapCmdIdName[SET_TIME_CMD] = "write_time";
	mapCmdIdName[READ_AGE_CMD] = "read_age"; 
	mapCmdIdName[SET_AGE_CMD] = "write_age";
	mapCmdIdName[READ_BUILD_CMD] = "read_build_conf";
	mapCmdIdName[SET_BUILD_CMD] = "write_build_conf";
	mapCmdIdName[READ_COLLECTOR_CMD] = "read_col_conf"; 
	mapCmdIdName[SET_COLLECTOR_CMD] = "write_col_conf";
	mapCmdIdName[READ_ETH_CMD] = "read_eth_conf";
	mapCmdIdName[SET_ETH_CMD] = "write_eth_conf";
	mapCmdIdName[READ_WLAN_CMD] = "read_wlan_conf";
	mapCmdIdName[SET_WLAN_CMD] = "write_wlan_conf";
	mapCmdIdName[READ_GNET_CMD] = "read_gnet_conf";
	mapCmdIdName[SET_GNET_CMD] = "write_gnet_conf";
	mapCmdIdName[READ_HOTSPOT_CMD] = "read_hostapd_conf";
	mapCmdIdName[SET_HOTSPOT_CMD] = "write_hostapd_conf";
	mapCmdIdName[READ_SERVICE_CMD] = "read_server_conf"; 
	mapCmdIdName[SET_SERVICE_CMD] = "write_server_conf";
	mapCmdIdName[READ_PORT_DEV_CMD] = "read_ifdev_conf"; 
	mapCmdIdName[READ_PORT_DEV_NUM_CMD] = "read_ifdev_num";
	mapCmdIdName[SET_START_SET_PORT_DEV_CMD] = "start_write_ifdev_conf";
	mapCmdIdName[SET_END_SET_PORT_DEV_CMD] = "end_write_ifdev_conf";
	mapCmdIdName[SET_PORT_DEV_CMD] = "write_ifdev_conf";
	mapCmdIdName[READ_REPORT_FORMAT_CMD] = "read_report_format"; 
	mapCmdIdName[SET_REPORT_FORMAT_CMD] = "write_report_format";
	mapCmdIdName[READ_STORE_INF_CMD] = "read_store_conf";
	mapCmdIdName[SET_STORE_INF_CMD] = "write_store_conf";
	mapCmdIdName[SET_EFFECT_NOW_CMD] = "at_once_effect";
	mapCmdIdName[SET_REBOOT_CMD] = "reset_sys"; 
	mapCmdIdName[READ_DEV_FACTORY_CMD] = "read_dev_factory_conf"; 
	mapCmdIdName[READ_REALTIME_DATA_CMD] = "query"; 

	mapCmdIdName[READ_DEF_SUBTYPE_NUM_CMD] = "read_user_sub_type_num"; 
	mapCmdIdName[READ_DEF_SUBTYPE_CONF_CMD] = "read_user_sub_type_conf"; 
	mapCmdIdName[READ_DEF_SUBTYPE_WAY_CMD] = "read_user_sub_type_way_conf"; 
	mapCmdIdName[SET_START_SET_DEF_SUBTYPE_CMD] = "start_write_user_sub_type_conf"; 
	mapCmdIdName[SET_DEF_SUBTYPE_CONF_CMD] = "write_user_sub_type_conf"; 
	mapCmdIdName[SET_DEF_SUBTYPE_WAY_CMD] = "write_user_sub_type_way_conf"; 
	mapCmdIdName[SET_END_SET_DEF_SUBTYPE_CMD] = "end_write_user_sub_type_conf"; 
	mapCmdIdName[SET_CLEAN_HISTORYDATA_CMD] = "clear_his_data"; 
	mapCmdIdName[SET_COPY_HISDATA_UDISK_CMD] = "cp_day_his_data_to_udisk"; 
	mapCmdIdName[GET_MONTH_HISDATA_INF_CMD] = "get_month_his_data_info"; 
	
	mapCmdIdName[SET_COPY_LOG_UDISK_CMD] = "cp_log_to_udisk"; 
	mapCmdIdName[SET_BACK_FACTORY_CMD] = "reset_factory_set"; 
	mapCmdIdName[SET_START_UPGRADE_CMD] = "start_sw_update"; 
	mapCmdIdName[SET_UPGRADE_DATA_CMD] = "update_sw_data"; 
	mapCmdIdName[SET_END_UPGRADE_CMD] = "end_sw_update"; 
	mapCmdIdName[SET_ADAPT_DEV_CMD] = "meter_comm_auto_fit"; 
	mapCmdIdName[SET_HEART_BEAT_CMD] = "handset_heart_beat"; 
	mapCmdIdName[SET_RESET_SDCARD_CMD] = "format_sdcard_ext2"; 
	
	return 0;
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
	ASSERT(com_data.xml_len < MAX_XML_LEN);

	FormMessage(com_data);

	int tatal_len = HEAD_TAIL_LEN + com_data.xml_len;

	return tatal_len;
}

XMLNODE * CXmlAnalyze::ParseRspParam(XML& xml)
{
	MS_INT32 rtn = 0;
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

int CXmlAnalyze::FormGetBuildCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[READ_BUILD_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseGetBuildRsp()
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

	rtn = ParseBuildNode(base_root);

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormSetBuildCmd()
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_BUILD_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<area_id>%d</area_id>\r\n",pColtorInf->build_conf.build_pos.county);
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%d</type>\r\n",pColtorInf->build_conf.build_type);
	tmp_len += sprintf(xml_data + tmp_len, "		<id>%d</id>\r\n",pColtorInf->build_conf.build_code);
	tmp_len += sprintf(xml_data + tmp_len, "		<name>%s</name>\r\n",pColtorInf->build_conf.build_name);
	tmp_len += sprintf(xml_data + tmp_len, "		<pos>%s</pos>\r\n",pColtorInf->build_conf.build_pos.pos_name);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetBuildRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormGetEthCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[READ_ETH_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseGetEthRsp()
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

	rtn = ParseEthNode(base_root);	

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormSetEthCmd()
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_ETH_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<used>%d</used>\r\n",pServiceInf->coltor_net.eth.used);
	tmp_len += sprintf(xml_data + tmp_len, "		<dhcp>%d</dhcp>\r\n",pServiceInf->coltor_net.eth.netType);
	tmp_len += sprintf(xml_data + tmp_len, "		<ip>%s</ip>\r\n",pServiceInf->coltor_net.eth.ip);
	tmp_len += sprintf(xml_data + tmp_len, "		<mask>%s</mask>\r\n",pServiceInf->coltor_net.eth.mask);
	tmp_len += sprintf(xml_data + tmp_len, "		<gw>%s</gw>\r\n",pServiceInf->coltor_net.eth.gateWay);
	tmp_len += sprintf(xml_data + tmp_len, "		<dns>%s</dns>\r\n",pServiceInf->coltor_net.eth.dns);
	tmp_len += sprintf(xml_data + tmp_len, "		<mac>%s</mac>\r\n",pServiceInf->coltor_net.eth.mac);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetEthRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormGetHotSpotCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[READ_HOTSPOT_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseGetHotSpotRsp()
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

	rtn = ParseHotSpotNode(base_root);	

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormSetHotSpotCmd()
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_HOTSPOT_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<ssid>%s</ssid>\r\n",pServiceInf->coltor_net.hotspot.ssid);
	tmp_len += sprintf(xml_data + tmp_len, "		<passphrase>%s</passphrase>\r\n",pServiceInf->coltor_net.hotspot.passphrase);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetHotSpotRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormGetPortDevCmd(int port_ind, int dev_ind)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[READ_PORT_DEV_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<i_addr>%d</i_addr>\r\n",port_ind);
	tmp_len += sprintf(xml_data + tmp_len, "		<dev_order>%d</dev_order>\r\n",dev_ind);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseGetPortDevRsp(int port_ind, int dev_ind)
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

	node = f_xmlnode_sch_elem(base_root, "dev");
	assert(node);
	base_root = node;

	rtn = ParsePortDevNode(base_root,port_ind,dev_ind);

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormSetPortDevCmd(int port_ind, int dev_ind)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "			<type>%s</type>\r\n",mapCmdIdName[SET_PORT_DEV_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "		</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "			<i_addr>%d</i_addr>\r\n",port_ind);
	tmp_len += sprintf(xml_data + tmp_len, "			<dev_order>%d</dev_order>\r\n",dev_ind);

	st_dev_inf* p_dev_inf = new st_dev_inf;
	memset(p_dev_inf, 0, sizeof(*p_dev_inf));
	memcpy(p_dev_inf, &pOuterDev->port_dev[port_ind].dev_inf[dev_ind], sizeof(*p_dev_inf));
	tmp_len += sprintf(xml_data + tmp_len, "			<dev>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "				<addr>%lld</addr>\r\n",p_dev_inf->dev_addr.id);
	tmp_len += sprintf(xml_data + tmp_len, "				<type>%d</type>\r\n",p_dev_inf->dev_type);
	tmp_len += sprintf(xml_data + tmp_len, "				<sub_type>%d</sub_type>\r\n",p_dev_inf->sub_type);
	tmp_len += sprintf(xml_data + tmp_len, "				<name>%s</name>\r\n",p_dev_inf->name);
	tmp_len += sprintf(xml_data + tmp_len, "				<pos>%s</pos>\r\n",p_dev_inf->pos_name);
	tmp_len += sprintf(xml_data + tmp_len, "				<opt>%d,%d,%d,%d</opt>\r\n",
		p_dev_inf->rs485_conf.databits,
		p_dev_inf->rs485_conf.pari,
		p_dev_inf->rs485_conf.stop,
		p_dev_inf->rs485_conf.baud);
	tmp_len += sprintf(xml_data + tmp_len, "				<factory>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "					<hw_ver>%s</hw_ver>\r\n",p_dev_inf->hw_ver);
	tmp_len += sprintf(xml_data + tmp_len, "					<sw_ver>%s</sw_ver>\r\n",p_dev_inf->sw_ver);
	tmp_len += sprintf(xml_data + tmp_len, "				</factory>\r\n");

	tmp_len += sprintf(xml_data + tmp_len, "				<way_mask>0x%x</way_mask>\r\n",p_dev_inf->way_inf.way_mask);

	int way_num = GetWayNumBySubTypeValue(p_dev_inf->sub_type);
	for(int j = 0; j < way_num; j++)
	{
		bool way_used = IsWayUsedByMask(j, p_dev_inf->way_inf.way_mask);
		if (way_used)
		{
			st_way_data way_data;
			memset(&way_data, 0, sizeof(way_data));
			memcpy(&way_data, &p_dev_inf->way_inf.way_data[j], sizeof(way_data));
			tmp_len += sprintf(xml_data + tmp_len, "				<way>\r\n");
			tmp_len += sprintf(xml_data + tmp_len, "					<id>%d</id>\r\n",way_data.way_id);
			tmp_len += sprintf(xml_data + tmp_len, "					<class_id>%d</class_id>\r\n",way_data.class_id);
			tmp_len += sprintf(xml_data + tmp_len, "					<item_id>%d</item_id>\r\n",way_data.item_id);
			tmp_len += sprintf(xml_data + tmp_len, "					<addr>%d</addr>\r\n",way_data.way_addr);
			tmp_len += sprintf(xml_data + tmp_len, "					<type>%d</type>\r\n",way_data.way_type);
			tmp_len += sprintf(xml_data + tmp_len, "					<name>%s</name>\r\n",way_data.way_name);
			tmp_len += sprintf(xml_data + tmp_len, "					<pos>%s</pos>\r\n",way_data.pos_name);
			tmp_len += sprintf(xml_data + tmp_len, "					<detail>%s</detail>\r\n",way_data.detail);
			tmp_len += sprintf(xml_data + tmp_len, "				</way>\r\n");
		}
	}
	tmp_len += sprintf(xml_data + tmp_len, "			</dev>\r\n");

	tmp_len += sprintf(xml_data + tmp_len, "		</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	</root>\r\n");
	delete(p_dev_inf);

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetPortDevRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormGetReportFormatCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[READ_REPORT_FORMAT_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseGetReportFormatRsp()
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

	rtn = ParseReportFormatNode(base_root);

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormSetReportFormatCmd()
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_REPORT_FORMAT_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");

	for (int i = 0; i < pReportFormatStoreInf->report_format.dev_type_num; i++)
	{
		tmp_len += sprintf(xml_data + tmp_len, "		<dev>\r\n");
		tmp_len += sprintf(xml_data + tmp_len, "			<type>%d</type>\r\n",
			pReportFormatStoreInf->report_format.dev_type_report[i].dev_type);
		string report_id_data = "";
		for (int j = 0; j < pReportFormatStoreInf->report_format.dev_type_report[i].param_num; j++)
		{
			CString tmp_id;
			int param_id = pReportFormatStoreInf->report_format.dev_type_report[i].report_param_id[j];
			if (param_id == E_REPORT_ID_METER_TIME ||
				param_id == E_REPORT_ID_METER_STATE )
			{
				continue;
			}

			tmp_id.Format("%d", param_id);
			report_id_data += tmp_id;
			if (j < pReportFormatStoreInf->report_format.dev_type_report[i].param_num - 1)
			{
				report_id_data += ",";
			}
		}
		tmp_len += sprintf(xml_data + tmp_len, "			<id>%s</id>\r\n", report_id_data.c_str());
		tmp_len += sprintf(xml_data + tmp_len, "		</dev>\r\n");
	}

	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetReportFormatRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetEffectNowCmd(VEC_STRING& vec_name_string)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_EFFECT_NOW_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");

	for (MU_INT32 i = 0; i < vec_name_string.size(); i++)
	{
		tmp_len += sprintf(xml_data + tmp_len, "		<conf>%s</conf>\r\n",vec_name_string[i].c_str());
	}
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetEffectNowRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormCmdByXmlData( string xml_data )
{
	ClearBuff();
	str_communicate_data com_data;
	memset(&com_data, 0, sizeof(com_data));

	com_data.xml_len += sprintf(com_data.xml_data + com_data.xml_len, "%s", xml_data.c_str());

	ASSERT(com_data.xml_len < MAX_XML_LEN);

	FormMessage(com_data);

	int tatal_len = HEAD_TAIL_LEN + com_data.xml_len;

	return tatal_len;
}

int CXmlAnalyze::ParseDevInf( XMLNODE *base_root, int port_ind, int dev_ind)
{
	XMLNODE* tmp_root = NULL;
	XMLNODE* node = NULL;
	int rtn = 0;
	VEC_INT vec_way_list;

	node = f_xmlnode_sch_elem(base_root, "addr");
	if (node != NULL)
	{
		str_to_int(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].dev_addr.id, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "type");
	if (node != NULL)
	{
		str_to_int(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].dev_type, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "sub_type");
	if (node != NULL)
	{
		str_to_int(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].sub_type, gHexTypeXml);
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
			str_to_int(vecElem[0].c_str(), 
				databit, gHexTypeXml);
			pOuterDev->port_dev[port_ind].dev_inf[dev_ind].rs485_conf.databits = databit;

			str_to_int(vecElem[1].c_str(), 
				pOuterDev->port_dev[port_ind].dev_inf[dev_ind].rs485_conf.pari, gHexTypeXml);

			int stop = 0;
			str_to_int(vecElem[2].c_str(), 
				stop, gHexTypeXml);
			pOuterDev->port_dev[port_ind].dev_inf[dev_ind].rs485_conf.stop = stop;

			int baud = 0;
			str_to_int(vecElem[3].c_str(), 
				baud, gHexTypeXml);
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
		str_to_int(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_mask, 16);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	//0x03 0,1根据掩码获取使用的回路下标数组
	int list_num = 
		GetWayListByMask(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_mask, vec_way_list, MAX_WAY_NUM);

	int way_num = GetSubNodeNum(base_root, "way");
	for (int i = 0; i < way_num; i++)
	{
		if (i == 0)
		{
			node = f_xmlnode_sch_elem(base_root, "way");
			assert(node);
			base_root = node;
		}
		else
		{
			node = f_xmlnode_nxt_node(base_root, "way");
			assert(node);
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

int CXmlAnalyze::ParseWayInf( XMLNODE *base_root, int port_ind, int dev_ind, int way_ind )
{
	XMLNODE* tmp_root = NULL;
	XMLNODE* node = NULL;
	int rtn = 0;

	node = f_xmlnode_sch_elem(base_root, "id");
	if (node != NULL)
	{
		str_to_int(node->data, 
			pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].way_id, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "class_id");
	if (node != NULL)
	{
		str_to_int(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].class_id, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "item_id");
	if (node != NULL)
	{
		str_to_int(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].item_id, gHexTypeXml);
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
		str_to_int(node->data, addr, gHexTypeXml);
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
		str_to_int(node->data, pOuterDev->port_dev[port_ind].dev_inf[dev_ind].way_inf.way_data[way_ind].way_type, gHexTypeXml);
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

int CXmlAnalyze::ParseServiceInf( XMLNODE *base_root, int service_ind )
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = f_xmlnode_sch_elem(base_root, "used");
	if (node != NULL)
	{
		int valid = 0;
		str_to_int(node->data, valid, gHexTypeXml);
		pServiceInf->service_net.ip_port_domain[service_ind].valid = (valid != 0);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "domain");
	if (node != NULL)
	{
		strcpy(pServiceInf->service_net.ip_port_domain[service_ind].domain, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "port");
	if (node != NULL)
	{
		str_to_int(node->data, pServiceInf->service_net.ip_port_domain[service_ind].port, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

leave:
	return rtn;
}

int CXmlAnalyze::FormGetDevFactoryCmd(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;
	e_dev_type dev_type = (e_dev_type)(sub_type / 100);

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[READ_DEV_FACTORY_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<if_no>%d</if_no>\r\n",port_ind);
	tmp_len += sprintf(xml_data + tmp_len, "		<addr>%lld</addr>\r\n",dev_id);
	tmp_len += sprintf(xml_data + tmp_len, "		<dev_type>%d</dev_type>\r\n",dev_type);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseGetDevFactoryRsp(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type)
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

	int dev_ind = GetDevIndByIdSubType(pOuterDev, dev_id, sub_type, port_ind);
	assert(dev_ind >= 0);

	node = f_xmlnode_sch_elem(base_root, "hw_ver");
	if (node != NULL)
	{
		strcpy(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].hw_ver,node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "sw_ver");
	if (node != NULL)
	{
		strcpy(pOuterDev->port_dev[port_ind].dev_inf[dev_ind].sw_ver,node->data);
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

int CXmlAnalyze::ParseReportFormatInf( XMLNODE *base_root, int dev_type_ind )
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = f_xmlnode_sch_elem(base_root, "type");
	if (node != NULL)
	{
		str_to_int(node->data, pReportFormatStoreInf->report_format.dev_type_report[dev_type_ind].dev_type, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "id");
	if (node != NULL)
	{
		string str = node->data;
		int report_ind = 0;
		VEC_STRING vec_report_id;
		MU_INT32 vec_size = SplitToString(str, vec_report_id, ',');
		for (MU_INT32 i = 0; i < vec_size; i++)
		{
			str_to_int(vec_report_id[i].c_str(), report_ind, gHexTypeXml);
			pReportFormatStoreInf->report_format.dev_type_report[dev_type_ind].report_param_id[i] = report_ind;
			pReportFormatStoreInf->report_format.dev_type_report[dev_type_ind].param_num ++;
		}	
	}
	else
	{
		rtn = -1;
		goto leave;
	}

leave:
	return rtn;
}

int CXmlAnalyze::FormGetPortDevNumCmd( int port_ind )
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[READ_PORT_DEV_NUM_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<i_addr>%d</i_addr>\r\n",port_ind);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseGetPortDevNumRsp( int port_ind )
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

	node = f_xmlnode_sch_elem(base_root, "dev_num");
	if (node != NULL)
	{
		str_to_int(node->data, pOuterDev->port_dev[port_ind].dev_num, gHexTypeXml);
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

int CXmlAnalyze::FormStartSetPortCmd( int port_ind )
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_START_SET_PORT_DEV_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<i_addr>%d</i_addr>\r\n",port_ind);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseStartSetPortCmd()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormEndSetPortCmd( int port_ind )
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_END_SET_PORT_DEV_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<i_addr>%d</i_addr>\r\n",port_ind);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseEndSetPortRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

void CXmlAnalyze::SetReportFormatInf( st_report_store_inf* report_format )
{
	memcpy(pReportFormatStoreInf, report_format, sizeof(*pReportFormatStoreInf));
}

int CXmlAnalyze::ParseTimeNode(XMLNODE* base_root)
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = f_xmlnode_sch_elem(base_root, "time");
	if (node != NULL)
	{
		//2014-02-12 15:30:32
		int yy = 0;
		int mm = 0;
		int dd = 0;
		int hh = 0;
		int mi = 0;
		int ss = 0;

		sscanf(node->data,"%d-%d-%d,%d:%d:%d",&yy,&mm,&dd,&hh,&mi,&ss);
		pColtorInf->dev_time.dev_time.YY = yy;
		pColtorInf->dev_time.dev_time.MM = mm;
		pColtorInf->dev_time.dev_time.DD = dd;
		pColtorInf->dev_time.dev_time.HH = hh;
		pColtorInf->dev_time.dev_time.MI = mi;
		pColtorInf->dev_time.dev_time.SS = ss;
		pColtorInf->dev_time.local_time = GetTickCount();
	}
	else
	{
		rtn = -1;
		goto leave;
	}
leave:
	return rtn;
}

int CXmlAnalyze::ParseBuildNode( XMLNODE* base_root )
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = f_xmlnode_sch_elem(base_root, "area_id");
	if (node != NULL)
	{
		int position = 0;
		str_to_int(node->data,position,gHexTypeXml);
		pColtorInf->build_conf.build_pos.province = position - position % 10000;
		pColtorInf->build_conf.build_pos.city = position - position % 100;
		pColtorInf->build_conf.build_pos.county = position;
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "type");
	if (node != NULL)
	{
		str_to_int(node->data,pColtorInf->build_conf.build_type,gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "id");
	if (node != NULL)
	{
		str_to_int(node->data, pColtorInf->build_conf.build_code, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "name");
	if (node != NULL)
	{
		strcpy(pColtorInf->build_conf.build_name, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "pos");
	if (node != NULL)
	{
		strcpy(pColtorInf->build_conf.build_pos.pos_name, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
leave:
	return rtn;
}

int CXmlAnalyze::ParseCollectorNode( XMLNODE* base_root )
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = f_xmlnode_sch_elem(base_root, "addr");
	if (node != NULL)
	{
		str_to_int(node->data,pColtorInf->col_conf.id,gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "type");
	if (node != NULL)
	{
		str_to_int(node->data,pColtorInf->col_conf.type,gHexTypeXml);
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
		str_to_int(node->data,pColtorInf->col_conf.sample_m,gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "heart_s");
	if (node != NULL)
	{
		str_to_int(node->data,pColtorInf->col_conf.heart_s,gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "report_m");
	if (node != NULL)
	{
		str_to_int(node->data,pColtorInf->col_conf.report_m,gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "if_num");
	if (node != NULL)
	{
		str_to_int(node->data,pOuterDev->port_num,gHexTypeXml);
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

int CXmlAnalyze::ParseEthNode( XMLNODE* base_root )
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = f_xmlnode_sch_elem(base_root, "used");
	if (node != NULL)
	{
		int used = 0;
		str_to_int(node->data, used, gHexTypeXml);
		pServiceInf->coltor_net.eth.used = (used != 0);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "dhcp");
	if (node != NULL)
	{
		int dhcp = 0;
		str_to_int(node->data, dhcp, gHexTypeXml);
		pServiceInf->coltor_net.eth.netType = (e_net_type)dhcp;
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "ip");
	if (node != NULL)
	{
		strcpy(pServiceInf->coltor_net.eth.ip, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "mask");
	if (node != NULL)
	{
		strcpy(pServiceInf->coltor_net.eth.mask, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "gw");
	if (node != NULL)
	{
		strcpy(pServiceInf->coltor_net.eth.gateWay, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "dns");
	if (node != NULL)
	{
		strcpy(pServiceInf->coltor_net.eth.dns, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
	node = f_xmlnode_sch_elem(base_root, "mac");
	if (node != NULL)
	{
		strcpy(pServiceInf->coltor_net.eth.mac, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
leave:
	return rtn;
}

int CXmlAnalyze::ParseHotSpotNode( XMLNODE* base_root )
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = f_xmlnode_sch_elem(base_root, "ssid");
	if (node != NULL)
	{
		strcpy(pServiceInf->coltor_net.hotspot.ssid, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "passphrase");
	if (node != NULL)
	{
		strcpy(pServiceInf->coltor_net.hotspot.passphrase, node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}
leave:
	return rtn;
}

int CXmlAnalyze::ParseServInfNode( XMLNODE* base_root )
{
	int rtn = 0;
	XMLNODE* node = NULL;

	node = f_xmlnode_sch_elem(base_root, "secret");
	if (node != NULL)
	{
		strcpy(pServiceInf->service_net.secret,node->data);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	int service_num = GetSubNodeNum(base_root, "server");
	for (int i = 0; i < service_num; i++)
	{
		if (i == 0)
		{
			node = f_xmlnode_sch_elem(base_root, "server");
			assert(node);
			base_root = node;
		}
		else
		{
			node = f_xmlnode_nxt_node(base_root, "server");
			assert(node);
			base_root = node;
		}

		int ret = ParseServiceInf(base_root, i);
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

int CXmlAnalyze::ParsePortDevNode( XMLNODE* base_root ,int port_ind, int dev_ind)
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

int CXmlAnalyze::ParseReportFormatNode( XMLNODE* base_root )
{
	int rtn = 0;
	XMLNODE* node = NULL;
	int dev_type_num = GetSubNodeNum(base_root, "dev");
	pReportFormatStoreInf->report_format.dev_type_num = dev_type_num;
	for (int i = 0; i < dev_type_num; i++)
	{
		if (i == 0)
		{
			node = f_xmlnode_sch_elem(base_root, "dev");
			assert(node);
			base_root = node;
		}
		else
		{
			node = f_xmlnode_nxt_node(base_root, "dev");
			assert(node);
			base_root = node;
		}

		int ret = ParseReportFormatInf(base_root, i);
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

int CXmlAnalyze::ParseRealTimeDataNode( XMLNODE* base_root, u_w_measure& w_measure, e_dev_type dev_type)
{
	int rtn = 0;
	XMLNODE* node = NULL;
	int meter_param_num = GetSubNodeNum(base_root, "f");
	for (int i = 0; i < meter_param_num; i++)
	{
		if (i == 0)
		{
			node = f_xmlnode_sch_elem(base_root, "f");
			assert(node);
			base_root = node;
		}
		else
		{
			node = f_xmlnode_nxt_node(base_root, "f");
			assert(node);
			base_root = node;
		}

		int ret = ParseRealTimeDataInf(base_root, i, w_measure, dev_type);
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

int CXmlAnalyze::ParseRealTimeDataInf( XMLNODE *base_root, int param_ind, u_w_measure& w_measure, e_dev_type dev_type)
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = base_root;

	int param_id;
	str_to_int(node->attr->data, param_id, gHexType);

	//char function_code[32];
	//memset(function_code, 0, sizeof(function_code));
	//strcpy(function_code, node->attr->next->data);

	//char function_err[32];
	//memset(function_err, 0, sizeof(function_err));
	//strcpy(function_err, node->attr->next->data);

	char c_param_value[64];
	memset(c_param_value, 0, sizeof(c_param_value));
	strcpy(c_param_value, node->data);

	//通过param_id找到对应类型的结构体成员
	//对某种类型设备，通过param_id找到成员的指针偏移值
	CParamIdMap* param_id_map = new CParamIdMap;
	param_id_map->SetValueByParamId(&w_measure, dev_type, c_param_value, param_id);
	delete param_id_map;

	return rtn;
}

void CXmlAnalyze::SetCParamDevDefModbusSet( CProtModbusSet* param_dev_def_modbus_set )
{
	*pCProtModbusSet = param_dev_def_modbus_set;
}

int CXmlAnalyze::FormGetDefSubTypeNumCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[READ_DEF_SUBTYPE_NUM_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseGetDefSubTypeNumRsp(int& num)
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

	node = f_xmlnode_sch_elem(base_root, "sub_type_num");
	if (node != NULL)
	{
		str_to_int(node->data, num, gHexTypeXml);
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

int CXmlAnalyze::FormGetDefSubTypeConfCmd( int ind )
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[READ_DEF_SUBTYPE_CONF_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<order>%d</order>\r\n",ind);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseGetDefSubTypeConfRsp( int ind )
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

	node = f_xmlnode_sch_elem(base_root, "user_sub_type");
	assert(node);
	base_root = node;

	rtn = ParseDefSubTypeConfNode(base_root, ind);

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::ParseDefSubTypeConfNode( XMLNODE* base_root, int def_ind)
{

	int rtn = 0;
	XMLNODE* node = NULL;
	node = f_xmlnode_sch_elem(base_root, "type");
	if (node != NULL)
	{
		str_to_int(node->data, 
			pCProtModbusSet->pCProtModbusArr[def_ind].pStDefSubTypeInfo->dev_type, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "sub_type");
	if (node != NULL)
	{
		str_to_int(node->data, 
			pCProtModbusSet->pCProtModbusArr[def_ind].pStDefSubTypeInfo->sub_type, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "sub_type_name");
	if (node != NULL)
	{
		strcpy(pCProtModbusSet->pCProtModbusArr[def_ind].pStDefSubTypeInfo->sub_type_name, node->data);
		char name[64];
		ZeroMemory(name,sizeof(name));
		utf8_to_gbk(name, node->data, strlen(node->data));
		int sub_type_ind = 
			GetSubTypeIndByValue(pCProtModbusSet->pCProtModbusArr[def_ind].pStDefSubTypeInfo->sub_type);
		assert(sub_type_ind >= 0);
		SetSubTypeNameByInd(sub_type_ind, name);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "w_num");
	if (node != NULL)
	{
		str_to_int(node->data, 
			pCProtModbusSet->pCProtModbusArr[def_ind].pStDefSubTypeInfo->way_num, gHexTypeXml);
		int sub_type_ind = 
			GetSubTypeIndByValue(pCProtModbusSet->pCProtModbusArr[def_ind].pStDefSubTypeInfo->sub_type);
		assert(sub_type_ind >= 0);
		SetWayCountBySubTypeInd(sub_type_ind,
			pCProtModbusSet->pCProtModbusArr[def_ind].pStDefSubTypeInfo->way_num);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "reg_is_same");
	if (node != NULL)
	{
		int val = 0;
		str_to_int(node->data, val, gHexTypeXml);
		pCProtModbusSet->pCProtModbusArr[def_ind].pStDefSubTypeInfo->is_reg_same = (val != 0);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

leave:
	return rtn;
}

int CXmlAnalyze::ParseRegisterInfNode( XMLNODE* base_root, int def_ind, int way_ind)
{
	int rtn = 0;
	XMLNODE* node = NULL;

	int node_num = GetSubNodeNum(base_root, "reg");
	for (int i = 0; i < node_num; i++)
	{
		if (i == 0)
		{
			node = f_xmlnode_sch_elem(base_root, "reg");
			assert(node);
			base_root = node;
		}
		else
		{
			node = f_xmlnode_nxt_node(base_root, "reg");
			assert(node);
			base_root = node;
		}

		int ret = ParseRegNode(base_root, def_ind, way_ind, i);
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

int CXmlAnalyze::ParseRegNode( XMLNODE* base_root, int def_ind , int way_ind, int param_ind)
{
	int rtn = 0;
	XMLNODE* node = NULL;
	node = base_root;
	if (node != NULL)
	{
		string str = node->data;
		vector<string> vecElem;
		int ret = SplitToString(str, vecElem, ',');
		if (ret == 3)
		{
			CProtModbus* p_CProtModbus 
				= &pCProtModbusSet->pCProtModbusArr[def_ind];
			CParamModbus* p_CParamModbus 
				= p_CProtModbus->pCParamModbus;
			CParamDevType* p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
			//addr,len,unit
			int addr = 0;
			str_to_int(vecElem[0].c_str(), 
				addr, gHexTypeXml);
			p_CParamDevType->pStParamInfoArr[param_ind].param_register_addr = addr;

			int len = 0;
			str_to_int(vecElem[1].c_str(), 
				len, gHexTypeXml);
			p_CParamDevType->pStParamInfoArr[param_ind].param_register_len = len;

			int unit = 0;
			str_to_int(vecElem[2].c_str(), 
				unit, gHexTypeXml);

			p_CParamDevType->pStParamInfoArr[param_ind].param_unit = unit;
		}	
	}
	else
	{
		rtn = -1;
		goto leave;
	}
leave:
	return rtn;
}

int CXmlAnalyze::FormDefSubTypeLocal(CProtModbus* p_CProtModbus)
{
	int tmp_len = 0;
	//自定义子类型
	tmp_len += sprintf(pBuff + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "<root>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "		<user_sub_type>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "			<type>%d</type>\r\n",
		p_CProtModbus->pStDefSubTypeInfo->dev_type);
	tmp_len += sprintf(pBuff + tmp_len, "			<sub_type>%d</sub_type>\r\n",
		p_CProtModbus->pStDefSubTypeInfo->sub_type);
	tmp_len += sprintf(pBuff + tmp_len, "			<sub_type_name>%s</sub_type_name>\r\n",
		p_CProtModbus->pStDefSubTypeInfo->sub_type_name);
	tmp_len += sprintf(pBuff + tmp_len, "			<w_num>%d</w_num>\r\n", 
		p_CProtModbus->pStDefSubTypeInfo->way_num);
	bool is_reg_same = p_CProtModbus->pStDefSubTypeInfo->is_reg_same;
	tmp_len += sprintf(pBuff + tmp_len, "			<reg_is_same>%d</reg_is_same>\r\n", 
		is_reg_same);

	int dev_ind = GetDevTypeIndByValue(p_CProtModbus->pStDefSubTypeInfo->dev_type);
	CParamModbus* p_CParamModbus = p_CProtModbus->pCParamModbus;
	int max_way_num = p_CParamModbus->paramDevTypeNum;
	for (int way_ind = 0; way_ind < max_way_num; way_ind++)
	{
		if (is_reg_same && way_ind > 0)
		{
			break;
		}
		CParamDevType* p_CParamDevSingle = &p_CParamModbus->pCParamDevTypeArr[way_ind];
		tmp_len += sprintf(pBuff + tmp_len, "			<reg_info>\r\n");
		for (int i = 0; i < p_CParamDevSingle->paramInfNum; i++)
		{
			tmp_len += sprintf(pBuff + tmp_len, "				<reg>%d,%d,%d</reg>\r\n",
				p_CParamDevSingle->pStParamInfoArr[i].param_register_addr,
				p_CParamDevSingle->pStParamInfoArr[i].param_register_len,
				p_CParamDevSingle->pStParamInfoArr[i].param_unit
				);
		}
		tmp_len += sprintf(pBuff + tmp_len, "			</reg_info>\r\n");
	}

	tmp_len += sprintf(pBuff + tmp_len, "		</user_sub_type>\r\n");
	tmp_len += sprintf(pBuff + tmp_len, "	</param>\r\n");

	tmp_len += sprintf(pBuff + tmp_len, "</root>\r\n");

	//自定义子类型
	return tmp_len;
}

int CXmlAnalyze::ParseDefSubTypeLocal(CProtModbus* p_CProtModbus)
{
	//多个协议or单个协议
	/*
	<?xml version="1.0" encoding="utf-8"?>
	<root>
		<param>
			<user_sub_type>
			<type>1</type>
			<sub_type>111</sub_type>
			<sub_type_name>xxxxxx</sub_type_name>
			<w_num>4</w_num>
			<reg_info>
			<reg>1,2,1</reg>   //addr, len, unit
			<reg>4,2,1</reg>
			</reg_info>
			</user_sub_type>	
		</param>
	</root>	
	*/
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

	node = f_xmlnode_sch_elem(base_root, "user_sub_type");
	assert(node);
	base_root = node;

	node = f_xmlnode_sch_elem(base_root, "type");
	if (node != NULL)
	{
		str_to_int(node->data, 
			p_CProtModbus->pStDefSubTypeInfo->dev_type, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "sub_type");
	if (node != NULL)
	{
		str_to_int(node->data, 
			p_CProtModbus->pStDefSubTypeInfo->sub_type, gHexTypeXml);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "sub_type_name");
	if (node != NULL)
	{
		strcpy(p_CProtModbus->pStDefSubTypeInfo->sub_type_name, node->data);
		char name[64];
		ZeroMemory(name,sizeof(name));
		utf8_to_gbk(name, node->data, strlen(node->data));
		int sub_type_ind = 
			GetSubTypeIndByValue(p_CProtModbus->pStDefSubTypeInfo->sub_type);
		assert(sub_type_ind >= 0);
		SetSubTypeNameByInd(sub_type_ind, name);

	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "w_num");
	if (node != NULL)
	{
		str_to_int(node->data, 
			p_CProtModbus->pStDefSubTypeInfo->way_num, gHexTypeXml);
		int sub_type_ind = 
			GetSubTypeIndByValue(p_CProtModbus->pStDefSubTypeInfo->sub_type);
		assert(sub_type_ind >= 0);
		SetWayCountBySubTypeInd(sub_type_ind,
			p_CProtModbus->pStDefSubTypeInfo->way_num);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	node = f_xmlnode_sch_elem(base_root, "reg_is_same");
	if (node != NULL)
	{
		int val = 0;
		str_to_int(node->data, val, gHexTypeXml);
		p_CProtModbus->pStDefSubTypeInfo->is_reg_same = (val != 0);
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	int node_num = GetSubNodeNum(base_root, "reg_info");
	for (int i = 0; i < node_num; i++)
	{
		if (i == 0)
		{
			node = f_xmlnode_sch_elem(base_root, "reg_info");
			assert(node);
			base_root = node;
		}
		else
		{
			node = f_xmlnode_nxt_node(base_root, "reg_info");
			assert(node);
			base_root = node;
		}

		XMLNODE* reg_base_root = base_root;
		int way_ind = i;
		int node_num = GetSubNodeNum(reg_base_root, "reg");
		for (int i = 0; i < node_num; i++)
		{
			if (i == 0)
			{
				node = f_xmlnode_sch_elem(reg_base_root, "reg");
				assert(node);
				reg_base_root = node;
			}
			else
			{
				node = f_xmlnode_nxt_node(reg_base_root, "reg");
				assert(node);
				reg_base_root = node;
			}

			node = reg_base_root;
			if (node != NULL)
			{
				string str = node->data;
				vector<string> vecElem;
				int ret = SplitToString(str, vecElem, ',');
				if (ret == 3)
				{
					CParamDevType* p_CParamDevType 
						= &p_CProtModbus->pCParamModbus->pCParamDevTypeArr[way_ind];

					//addr,len,unit
					int addr = 0;
					str_to_int(vecElem[0].c_str(), 
						addr, gHexTypeXml);

					p_CParamDevType->pStParamInfoArr[i].param_register_addr = addr;

					int len = 0;
					str_to_int(vecElem[1].c_str(), 
						len, gHexTypeXml);
					p_CParamDevType->pStParamInfoArr[i].param_register_len = len;

					int unit = 0;
					str_to_int(vecElem[2].c_str(), 
						unit, gHexTypeXml);

					p_CParamDevType->pStParamInfoArr[i].param_unit = unit;
				}	
			}
			else
			{
				rtn = -1;
				goto leave;
			}
		}
	}

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::DefSubTypeLocalLoad(const char *pfile, CProtModbus* def_sub_type_modbus)
{
	int rtn = 0;

	rtn = LoadFile(pfile);
	if (rtn != 0)
	{
		return -1;
	}

	rtn = ParseDefSubTypeLocal(def_sub_type_modbus);
	if (rtn != 0)
	{
		return -2;
	}

	return rtn;
}

int CXmlAnalyze::DefSubTypeLocalSave(const char *pfile, CProtModbus* def_sub_type_modbus)
{
	FILE *pf=NULL;
	int len = 0;
	int rtn = 0;

	len = FormDefSubTypeLocal(def_sub_type_modbus);

	MU_INT8*data = (MU_INT8*)gXmlAnalyze.pBuff;
	encry_local_data(data, len);

	pf = fopen(pfile,"wb");
	if(pf)
	{
		fwrite(pBuff,sizeof(char),len,pf);
		fclose(pf);		
	}
	else
	{
		return -1;
	}	

	return rtn;
}

int CXmlAnalyze::FormStartSetDefSubTypeCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_START_SET_DEF_SUBTYPE_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseStartSetDefSubTypeCmd()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetDefSubTypeConfCmd( int def_sub_type_ind, int order_ind)
{
	CProtModbus* p_CParamDevDefModbus = 
		&pCProtModbusSet->pCProtModbusArr[def_sub_type_ind];

	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_DEF_SUBTYPE_CONF_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");

	tmp_len += sprintf(xml_data + tmp_len, "		<order>%d</order>\r\n", order_ind);

	tmp_len += sprintf(xml_data + tmp_len, "		<user_sub_type>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "			<type>%d</type>\r\n", 
		p_CParamDevDefModbus->pStDefSubTypeInfo->dev_type);
	tmp_len += sprintf(xml_data + tmp_len, "			<sub_type>%d</sub_type>\r\n",
		p_CParamDevDefModbus->pStDefSubTypeInfo->sub_type);
	tmp_len += sprintf(xml_data + tmp_len, "			<sub_type_name>%s</sub_type_name>\r\n",
		p_CParamDevDefModbus->pStDefSubTypeInfo->sub_type_name);
	int sub_type = GetSubTypeIndByValue(p_CParamDevDefModbus->pStDefSubTypeInfo->sub_type);
	assert(sub_type >= 0);
	tmp_len += sprintf(xml_data + tmp_len, "			<w_num>%d</w_num>\r\n",
		gWayCountValue[sub_type]);
	bool reg_is_same = p_CParamDevDefModbus->pStDefSubTypeInfo->is_reg_same; 
	tmp_len += sprintf(xml_data + tmp_len, "			<reg_is_same>%d</reg_is_same>\r\n",
		reg_is_same);

	tmp_len += sprintf(xml_data + tmp_len, "		</user_sub_type>\r\n");		

	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetDefSubTypeConfRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormEndSetDefSubTypeCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_END_SET_DEF_SUBTYPE_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseEndSetDefSubTypeCmd()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetCopyHisDataUDiskCmd(st_time& date_time)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_COPY_HISDATA_UDISK_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<day>%04d%02d%02d</day>\r\n", date_time.YY, date_time.MM, date_time.DD);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetCopyHisDataUDiskRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetCopyLogUDiskCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_COPY_LOG_UDISK_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseSetCopyLogUDiskRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormSetBackFactoryCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_BACK_FACTORY_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseSetBackFactoryRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::ParseAdaptDevRsp()
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

	node = f_xmlnode_sch_elem(base_root, "resp");
	if (node != NULL)
	{
		int len = strlen(node->data); 
		if(strlen(node->data) < 7*2)
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

int CXmlAnalyze::FormAdaptDevCmd( st_rs485& rs485, CDevAdaptSendFrame* send_frame , int port_ind)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_ADAPT_DEV_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<i_addr>%d</i_addr>\r\n", port_ind);
	tmp_len += sprintf(xml_data + tmp_len, "		<opt>%d,%d,%d,%d</opt>\r\n", 
		rs485.databits,
		rs485.pari,
		rs485.stop,
		rs485.baud);
	tmp_len += sprintf(xml_data + tmp_len, "		<req>");

	for (int i = 0; i < send_frame->sendFrameLen; i++)
	{
		tmp_len += sprintf(xml_data + tmp_len, "%02x",(MU_INT8)send_frame->sendFrame[i]);
	}
	tmp_len += sprintf(xml_data + tmp_len, "</req>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::FormSetHeartBeatCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_HEART_BEAT_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseSetHeartBeatRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormGetMonthHisDataInfCmd( st_time& date_time )
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[GET_MONTH_HISDATA_INF_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<month>%04d%02d</month>\r\n", date_time.YY, date_time.MM);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseGetMonthHisDataInfRsp( VEC_TIME& data_vec )
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

	int node_num = GetSubNodeNum(base_root, "day");
	for (int i = 0; i < node_num; i++)
	{
		if (i == 0)
		{
			node = f_xmlnode_sch_elem(base_root, "day");
			assert(node);
			base_root = node;
		}
		else
		{
			node = f_xmlnode_nxt_node(base_root, "day");
			assert(node);
			base_root = node;
		}

		st_time tmp_time;
		memset(&tmp_time, 0, sizeof(tmp_time));
		//20150101
		int val = 0;
		str_to_int(node->data, val, gHexTypeXml);
		tmp_time.YY = val /100 / 100;
		tmp_time.MM = val / 100 % 100;
		tmp_time.DD = val % 100;
		data_vec.push_back(tmp_time);
	}

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormResetSDCardCmd()
{
	int tatal_len = FormCmdByType(mapCmdIdName[SET_RESET_SDCARD_CMD]);
	return tatal_len;
}

int CXmlAnalyze::ParseResetSDCardRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}

int CXmlAnalyze::FormGetDefSubTypeWayCmd( int def_sub_type_ind, int way_ind)
{
	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[READ_DEF_SUBTYPE_WAY_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<order>%d</order>\r\n", def_sub_type_ind);
	tmp_len += sprintf(xml_data + tmp_len, "		<w_addr>%d</w_addr>\r\n", way_ind);
	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseGetDefSubTypeWayRsp( int def_sub_type_ind, int way_ind )
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

	node = f_xmlnode_sch_elem(base_root, "reg_info");
	if (node != NULL)
	{
		base_root = node;		
	}
	else
	{
		rtn = -1;
		goto leave;
	}

	rtn = ParseRegisterInfNode(base_root, def_sub_type_ind, way_ind);
	if (rtn != 0)
	{
		rtn = -1;
		goto leave;
	}

leave:
	f_xmlnode_del(&xml);
	return rtn;
}

int CXmlAnalyze::FormSetDefSubTypeWayCmd( int order_ind, int def_sub_type_ind, int way_ind )
{
	CProtModbus* p_CProtModbus = 
		&pCProtModbusSet->pCProtModbusArr[def_sub_type_ind];

	MS_CHAR xml_data[MAX_XML_LEN];
	memset(xml_data, 0, sizeof(xml_data));
	int tmp_len = 0;

	tmp_len += sprintf(xml_data + tmp_len, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "<root>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "		<type>%s</type>\r\n",mapCmdIdName[SET_DEF_SUBTYPE_WAY_CMD].c_str());
	tmp_len += sprintf(xml_data + tmp_len, "	</common>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "	<param>\r\n");

	tmp_len += sprintf(xml_data + tmp_len, "		<order>%d</order>\r\n", order_ind);
	tmp_len += sprintf(xml_data + tmp_len, "		<w_addr>%d</w_addr>\r\n", way_ind);


	CParamModbus* p_CParamModbus = p_CProtModbus->pCParamModbus;

	CParamDevType* p_CParamDevType = &p_CParamModbus->pCParamDevTypeArr[way_ind];
	tmp_len += sprintf(xml_data + tmp_len, "		<reg_info>\r\n");
	for (int i = 0; i < p_CParamDevType->paramInfNum; i++)
	{
		tmp_len += sprintf(xml_data + tmp_len, "			<reg>%d,%d,%d</reg>\r\n",
			p_CParamDevType->pStParamInfoArr[i].param_register_addr,
			p_CParamDevType->pStParamInfoArr[i].param_register_len,
			p_CParamDevType->pStParamInfoArr[i].param_unit
			);
	}
	tmp_len += sprintf(xml_data + tmp_len, "		</reg_info>\r\n");


	tmp_len += sprintf(xml_data + tmp_len, "	</param>\r\n");
	tmp_len += sprintf(xml_data + tmp_len, "</root>\r\n");

	int tatal_len = FormCmdByXmlData(xml_data);

	return tatal_len;
}

int CXmlAnalyze::ParseSetDefSubTypeWayRsp()
{
	int rtn = 0;
	rtn = ParseSetCollectorInfRsp();
	return rtn;
}
