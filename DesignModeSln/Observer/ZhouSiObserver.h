#pragma once
#include "iobserver.h"
#include <iostream>
using std::string;
class CZhouSiObserver :
	public IObserver
{
public:
	CZhouSiObserver(void);
	~CZhouSiObserver(void);
	void Update(string context);
	string GetName();
private:
	void Cry(string report);
};