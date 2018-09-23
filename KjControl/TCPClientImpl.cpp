#include "stdafx.h"
#include "TCPClientImpl.h"

TCPClientImpl::TCPClientImpl():/*mdisvec(NULL),*/
	/*mdis2vec(NULL),*/
	m_str(""),
	lvheight(0.0),
	pcilong(0)
{
	//mdisvec = make_shared<vector<PAIR>>();
	//mdisvec->clear();
	//mdisvec.clear();

	mdisvec = make_shared<vector<PAIR>>();
	mdisvec->clear();
	TRACE("TCPClientImpl struct ...\r\n");
}

TCPClientImpl::~TCPClientImpl()
{
	TRACE("TCPClientImpl destruct ...\r\n");
}

int TCPClientImpl::Run()
{
	char buff[1024] = {0};
	//string recvdata = "";
	//string substr;
	//char c1 = '{';
	//char c2 = '}';
	//unsigned int ad1,ad2;
	//ad1 = ad2 = 0;
	//unsigned long pl1,pl2,pl3;
	//pl1 = pl2 = pl3 = 0;
	m_str.clear();
	int len = 1024;
	while(this->m_running)
	{
		memset(buff, 0, 1024);
		int ret = this->ReadData(buff, len, 200);
		if(ret>0)
		{
			m_str.append(buff,ret);
			//recvdata.append(buff, ret);
			/*
			while(recvdata.size()>=8)
			{
				int idx1 = recvdata.find_first_of(c1);
				int idx2 = recvdata.find_first_of(c2);
				if(idx1 >=0 && idx2>=0)
				{
					if(idx2 > idx1 && idx2 - idx1 == 7)
					{
						substr = recvdata.substr(idx1, idx2-idx1+1);
						
						ad1 = unsigned char(substr.at(1))<<8;
						ad2 = unsigned char(substr.at(2));
						int ad = ad1 + ad2;
						lvheight = ad*AD_XISHU;

						pl1 = unsigned char(substr.at(4))<<16;
						pl2 = unsigned char(substr.at(5))<<8;
						pl3 = unsigned char(substr.at(6));
						pcilong = pl1 + pl2 + pl3;
						//mdisvec.push_back(make_pair<float, long>(lvheight, pcilong ));
						mdisvec->push_back(make_pair<float, long>(lvheight, pcilong ));
					}
				}
				// É¾³ýÇ°×÷ÅÐ¶Ï£¬±ÜÃâÒì³£
				if(idx2>= 0)
					recvdata = recvdata.erase(0,idx2+1);
				else
					recvdata.clear();
			}
			*/
		}
	}
	return 0;
}