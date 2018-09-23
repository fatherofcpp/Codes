#include "StdAfx.h"
#include "TrainType.h"
#include "Reflect.h"
#include "FIFO.h"
#include "utility.h"

void TrainBase::LoadCfg(string name)
{
	memset(&trainmsg, 0, sizeof(TRAINMSG));
	char szPath[MAX_PATH];
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strFileName = szPath;
	strFileName.Delete(strFileName.ReverseFind('\\')+1, strFileName.GetLength ()-strFileName.ReverseFind('\\')-1);
	name = strFileName.GetString() + name + ".ini";

	CFIFO::GetInstance()->Write(name+" running\r\n");
	char buff[MAX_PATH] = {0};
	//����
	GetPrivateProfileString(_T("BASE"),_T("traintype"),"CRH380B",buff,MAX_PATH, name.c_str());
	traintype = buff;
	trainmsg.traintype = traintype;
	/*
		ת��ܶ������нṹ
		[BASE]
		traintype = CRH380B

		#һ�ڳ�������ת���ʹ�á�&�������������복��ʹ�á�|���ָ�
		trainzxjsel = DSJ&D|T&T|D&D|T&T0|T0&T|D&D|T&T|D&DSJ
	*/
	GetPrivateProfileString(_T("BASE"),_T("trainzxjsel"),"CRH380B",buff,MAX_PATH, name.c_str());
	zxjsel = buff;
	vector<string> zxjtypesel;
	Split(zxjsel, zxjtypesel, "&|");
	/*
		ͨ�����������е�ת������Ͷ�ȡ��Ӧת�����Ϣ
		[DSJ]
		zxjinfo = 1.05&4&0|2.05&1&3|3.05&2&6

		[D]
		zxjinfo = 1&4&0|2&1&3|3&2&6

		[T]
		zxjinfo = 1&4&0|2&1&3|3&2&6

		[T0]
		zxjinfo = 1&4&0|2&1&3|3&2&6
	*/
	for(auto it = zxjtypesel.begin(); it != zxjtypesel.end(); it++)
	{
		GetPrivateProfileString(it->c_str(),_T("zxjinfo"),"",buff,MAX_PATH, name.c_str());
		string tmp = buff;

		ZXJINFO tmpZxjinfo;
		tmpZxjinfo.zxjname = it->c_str();
		
		vector<string> stpstr;
		Split(tmp, stpstr, "|");
		//ͨ��ת�����Ϣ���ѳ�ÿ��ͣ��λ��Ϣ
		for(auto its = stpstr.begin(); its != stpstr.end(); its++)
		{
			vector<string> stpdel;
			Split(its->c_str(), stpdel, "&");
			STPINFO tmpStpinfo;
			if(stpdel.size()>=3)
			{
				long offset = atol(stpdel.at(0).c_str()); 
				int lrobotnum = atoi(stpdel.at(1).c_str());
				int rrobotnum = atoi(stpdel.at(2).c_str());
				
				tmpStpinfo = STPINFO::make_data(offset, lrobotnum, rrobotnum);
				tmpZxjinfo.stpdata.push_back(tmpStpinfo);
			}
		}
		trainmsg.zxjdata.push_back(tmpZxjinfo);
	}
	//�������ϳ���Ӧ�Ļ����߶�
	dzxjh = GetPrivateProfileInt(_T("SJ"),_T("dzxj"),0,name.c_str());
	tzxjh = GetPrivateProfileInt(_T("SJ"),_T("tzxj"),0,name.c_str());

	trainmsg.dzxjh = dzxjh;
	trainmsg.tzxjh = tzxjh;

	GetPrivateProfileString(_T("BASE"),_T("zjj"),"2.5",buff,MAX_PATH, name.c_str());
	zjj = buff;
	trainmsg.zjj = zjj;

	zxjnum = GetPrivateProfileInt(_T("BASE"),_T("zxjnum"), 0, name.c_str());
	trainmsg.zxjnum = zxjnum;
}

void TrainBase::GetTrainInfo(TRAINMSG& trmsg)
{
	trmsg = this->trainmsg;
}

REGISTER(TestDemo);
REGISTER(CRH380B);
REGISTER(CRH380BL);
REGISTER(CR400BF);
///
REGISTER(CRH1A);
REGISTER(CRH2A);
REGISTER(CRH3A);
REGISTER(CRH380A);
REGISTER(CRH380D);