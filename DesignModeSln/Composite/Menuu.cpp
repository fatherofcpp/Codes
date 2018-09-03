#include "stdafx.h"
#include "Menuu.h"

Menuu::Menuu()
{

}

Menuu::Menuu(std::string strName) : m_strName(strName)
{

}

Menuu::~Menuu()
{

}

void Menuu::Add(Menuu* pMenu)
{}

void Menuu::Remove(Menuu* pMenu)
{}

Menuu* Menuu::GetChild(int index)
{
	return NULL;
}