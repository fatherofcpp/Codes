#pragma once

#include <string>


class Menuu  
{
public:
	virtual ~Menuu();

	virtual void Add(Menuu*);
	virtual void Remove(Menuu*);
	virtual Menuu* GetChild(int);
	virtual void Display() = 0;
protected:
	Menuu();
	Menuu(std::string);
	std::string m_strName;
};
