#include "StdAfx.h"
#include "WhiteHumanFactory.h"
#include "WhiteHuman.h"
CWhiteHumanFactory::CWhiteHumanFactory(void)
{
}
CWhiteHumanFactory::~CWhiteHumanFactory(void)
{
}
IHuman * CWhiteHumanFactory::CreateHuman( void )
{
	return new CWhiteHuman();
}