/*
	class AutocarDis wrapped
	С������ֵ��װ
*/
#pragma once 
#include "stdafx.h"

class AutocarDis
{
public:
	AutocarDis();
	virtual ~AutocarDis();
	float GetDis();
	void  SetDis(float v);
private:
	float m_fDis;
	CRITICAL_SECTION m_cs;
};