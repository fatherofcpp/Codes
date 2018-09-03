#pragma once
#include "LiftState.h"
#include "OpenningState.h"
#include "CloseingState.h"
#include "RunningState.h"
#include "StoppingState.h"
class CContext
{
public:
	CContext(void);
	~CContext(void);
	static COpenningState *pOpenningState;
	static CCloseingState *pCloseingState;
	static CRunningState *pRunningState;
	static CStoppingState *pStoppingState;
	CLiftState * GetLiftState();
	void SetLiftState(CLiftState *pLiftState);
	void Open();
	void Close();
	void Run();
	void Stop();
private:
	CLiftState *m_pLiftState;
};