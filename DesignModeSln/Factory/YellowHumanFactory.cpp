#include "StdAfx.h"
#include "YellowHumanFactory.h"
#include "YellowHuman.h"
CYellowHumanFactory::CYellowHumanFactory(void)
{
}
CYellowHumanFactory::~CYellowHumanFactory(void)
{
}
IHuman * CYellowHumanFactory::CreateHuman( void )
{
	return new CYellowHuman();
}