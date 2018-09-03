#pragma once
#include "icarbuilder.h"
#include "CarModel.h"
#include <iostream>
#include <vector>
using std::string;
using std::vector;
class CBenzBuilder :
	public ICarBuilder
{
public:
	CBenzBuilder(void);
	~CBenzBuilder(void);
	void SetSequence(vector<string> *pSeq);
	CCarModel * GetCarModel();
private:
	CCarModel *m_pBenz;
};