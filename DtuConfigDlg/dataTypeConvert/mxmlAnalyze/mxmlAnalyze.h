#pragma once
#include "DataTypeDef.h"

class CMXmlAnalyze
{
	// ππ‘Ï
public:
	CMXmlAnalyze();	
	~CMXmlAnalyze();

public:
	friend class CDtuConfigDlgDlg;
};
bool net_pro_parser_resp_4_read_col_conf(char* buf, st_col_conf* resp);
