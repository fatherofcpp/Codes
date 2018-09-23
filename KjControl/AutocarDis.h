/*
	class AutocarDis wrapped
	小车距离值封装
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