#pragma once

namespace NSP_STP_CM
{
	class AbstractTransf
	{
	public:
		AbstractTransf();
		virtual ~AbstractTransf();

		virtual int ConnectService()=0;
		virtual int DisconnectService()=0;
		virtual int Send2Service(const char* p_data, int i_data)=0;
		virtual int RecvFrmService(char* p_data, int i_data)=0;
	};
}

