// pos.cpp : Defines the entry point for the console application.
//

#pragma once
#include "posDefine.h"
/////////////////////////////////////////////////////////////////////////////////////////
class CPosition
{
	// ππ‘Ï
public:
	CPosition();	
	~CPosition();

public:
	void parser_db();

	CString GetNameById(int id);

private:
	st_nation* pNation;
public:
	friend class CColtor;
};