/*
	TCPClientImpl.h File
*/
#pragma once

#include <memory>
#include <string>
#include "Thread.h"
#include "TCPClient.h"
#include "Algo.h"
#include "WSocket.h"

//class TCPClientImpl:public TCPClient, public CThread
class TCPClientImpl:public CWSocket, public CThread
{
public :
	TCPClientImpl();
	virtual ~TCPClientImpl();
	virtual int Run();
	shared_ptr<vector<PAIR>>& GetMdisVec(){ return mdisvec;};
	//vector<PAIR> GetMdisVec(){ return mdisvec;};
	//void MdisVecClear() {mdisvec.clear();};
	void MdisVecClear() {mdisvec->clear();};
	//vector<PAIR>& GetMdisVec(){ return mdisvec;};
	string& GetRecvString(){return m_str;};
	void    ClearRecvString() { m_str.clear();};
private:
	//vector<PAIR> mdisvec;
	shared_ptr<vector<PAIR>> mdisvec;
	//vector<PAIR> mdisvec;
	string m_str;
	float lvheight;
	long  pcilong;
};

