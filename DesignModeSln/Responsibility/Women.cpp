#include "StdAfx.h"
#include "Women.h"
CWomen::CWomen( int _type, string _request )
{
	this->m_type = _type;
	switch (this->m_type)
	{
	case 1:
		this->m_request.append("Ů���������ǣ�");
		this->m_request.append(_request);
		break;
	case 2:
		this->m_request.append("���ӵ������ǣ�");
		this->m_request.append(_request);
		break;
	case 3:
		this->m_request.append("ĸ�׵������ǣ�");
		this->m_request.append(_request);
		break;
	}
}
CWomen::~CWomen(void)
{
}
int CWomen::GetType()
{
	return m_type;
}
string CWomen::GetRequest()
{
	return m_request;
}