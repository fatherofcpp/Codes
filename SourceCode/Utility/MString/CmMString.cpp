#include "StdAfx.h"
#include "CmMString.h"

const char c_string_end = '\0';

NSP_STP_CM::CMString::CMString(void)
{
	const int c_str_len = 0;
	m_pStr = new char[c_str_len + 1];
	m_pStr[c_str_len] = c_string_end;
}

NSP_STP_CM::CMString::CMString( const CMString& other_obj )
{
	const int c_str_len = strlen(other_obj.m_pStr);
	this->m_pStr = new char[c_str_len + 1];
	if (c_str_len > 0)
	{
		strcpy(this->m_pStr, other_obj.m_pStr);
	}
	m_pStr[c_str_len] = c_string_end;
}

NSP_STP_CM::CMString::CMString( const char* other_str )
{
	if (!other_str)
	{
		const int c_str_len = 0;
		m_pStr = new char[c_str_len + 1];
		m_pStr[c_str_len] = c_string_end;
	}
	else
	{
		const int c_str_len = strlen(other_str);
		this->m_pStr = new char[c_str_len + 1];
		if (c_str_len > 0)
		{
			strcpy(this->m_pStr, other_str);
		}
		m_pStr[c_str_len] = c_string_end;
	}
}

NSP_STP_CM::CMString::~CMString(void)
{
	if (m_pStr)
	{
		delete[] m_pStr;
		m_pStr = NULL;
	}
}

NSP_STP_CM::CMString NSP_STP_CM::CMString::operator=( const CMString& other_obj)
{
	*this = other_obj.m_pStr;
	return *this;
}

NSP_STP_CM::CMString NSP_STP_CM::CMString::operator=( const char* other_str )
{
	if (!other_str)
	{
		const int c_str_len = 0;
		m_pStr = new char[c_str_len + 1];
		m_pStr[c_str_len] = c_string_end;
	}
	else
	{
		if (0 == strcmp(this->m_pStr, other_str))
		{

		}
		else
		{
			delete[] m_pStr;

			const int c_str_len = strlen(other_str);
			this->m_pStr = new char[c_str_len + 1];
			if (c_str_len > 0)
			{
				strcpy(this->m_pStr, other_str);
			}
			m_pStr[c_str_len] = c_string_end;
		}
	}
	return *this;
}

NSP_STP_CM::CMString NSP_STP_CM::CMString::operator+( const CMString& other_obj)
{
	*this = *this + other_obj.m_pStr;
	return *this;
}

NSP_STP_CM::CMString NSP_STP_CM::CMString::operator+( const char* other_str )
{
	if (!other_str)
	{

	}
	else
	{
		const int c_other_str_len = strlen(other_str);
		if (c_other_str_len > 0)
		{
			CMString this_copy = *this;
			delete[] this->m_pStr;

			const int c_this_str_len = strlen(this_copy.m_pStr);
			this->m_pStr = new char[c_this_str_len + c_other_str_len + 1];
			strcpy(this->m_pStr, this_copy.m_pStr);
			strcat(this->m_pStr, other_str);
			m_pStr[c_other_str_len] = c_string_end;
		}
		else
		{

		}

	}
	return *this;
}

void NSP_STP_CM::CMString::test()
{
}

bool NSP_STP_CM::CMString::operator==( const CMString& other_obj )
{
	int ret = strcmp(this->m_pStr, other_obj.m_pStr);
	return (ret == 0);
}

bool NSP_STP_CM::CMString::empty()
{
	int ret = strlen(this->m_pStr);
	return (ret == 0);
}

const char* NSP_STP_CM::CMString::c_str()
{
	return m_pStr;
}

void NSP_STP_CM::CMString::swap( CMString& other_obj )
{
	CMString copy_this = *this;
	*this = other_obj;
	other_obj = copy_this;
}
