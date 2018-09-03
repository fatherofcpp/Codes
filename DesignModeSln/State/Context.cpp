#include "StdAfx.h"
#include "Context.h"
COpenningState* CContext::pOpenningState = NULL;
CCloseingState* CContext::pCloseingState = NULL;
CRunningState* CContext::pRunningState = NULL;
CStoppingState* CContext::pStoppingState = NULL;

CContext::CContext(void)
{
	m_pLiftState = NULL;
	pOpenningState = new COpenningState();
	pCloseingState = new CCloseingState();
	pRunningState = new CRunningState();
	pStoppingState = new CStoppingState();
}

CContext::~CContext(void)
{
	delete pOpenningState;
	pOpenningState = NULL;
	delete pCloseingState;
	pCloseingState = NULL;
	delete pRunningState;
	pRunningState = NULL;
	delete pStoppingState;
	pStoppingState = NULL;
}

CLiftState * CContext::GetLiftState()
{
	return m_pLiftState;
}

void CContext::SetLiftState(CLiftState *pLiftState)
{
	this->m_pLiftState = pLiftState;
	this->m_pLiftState->SetContext(this);
}

void CContext::Open()
{
	this->m_pLiftState->Open();
}

void CContext::Close()
{
	this->m_pLiftState->Close();
}

void CContext::Run()
{
	this->m_pLiftState->Run();
}

void CContext::Stop()
{
	this->m_pLiftState->Stop();
}