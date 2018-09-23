//#include "stdafx.h"
#include "C2Htype.h"

#include "../include/mysocket.h"
#include "../include/commbase.h"
#include "../include/thread.h"
#include <WinSock.h>
#define USES_SOCKET_LIB


MS_INT32 closeComm(MS_INT32 *cmid,COMM_MODE mode)
{
	MS_INT32 rtn = MD_OK;

	switch (mode) {
		case SOCKET_TCP_MODE:
		case SOCKET_UDP_MODE:
		
			#ifdef USES_SOCKET_LIB
			CloseSocket((SOCKET *)cmid);
			#endif

			break;

		case HPIC_MODE:
			break;

		default:
			rtn = MD_FAILED;

			break;
	}

	return rtn;

}



MS_INT32 openNewComm(MS_INT32 *cmid,M_HANDLE params,COMM_MODE mode,MU_INT32 timeout)
{
	MS_INT32 rtn = MD_OK;
	SOCKET_PARAM *s_params;

	switch (mode) {
		case SOCKET_TCP_MODE:
		
		#ifdef USES_SOCKET_LIB
			if (*cmid != (MS_INT32)INVALID_SOCKET)
			{
				closeComm(cmid,mode);
			}

			//CreateSocket((SOCKET *)cmid,AF_INET, SOCK_STREAM, IPPROTO_TCP);

			s_params = (SOCKET_PARAM *)params;
		
			rtn = ConnectSocket((SOCKET *)cmid,s_params->m_serverAdd,s_params->m_port,timeout);

			if (*cmid == (MS_INT32)INVALID_SOCKET)
			{
				//myLOGprint("Create a new socket failed.\n",DEF_MSG_PRINT,0);

				rtn = MD_FAILED;
			}
			//else
			//{
			//	SOCKET_PARAM *s_params = (SOCKET_PARAM *)params;
			
			//	rtn = ConnectSocket(*cmid,s_params->m_serverAdd,s_params->m_port);

			//}
		#endif
			break;
		case HPIC_MODE:
		
			*cmid = 0;
			
			break;
		default:
			break;
	}

	return rtn;



}





MS_INT32 CreateListenComm(MS_INT32 *scomId, MU_INT32 port,COMM_MODE mode)
{

	MS_INT32 rtn = MD_FAILED;

	switch (mode) {
		case SOCKET_TCP_MODE:
		#ifdef USES_SOCKET_LIB
		//case SOCKET_UDP_MODE:
			rtn = CreateListenSock((SOCKET *)scomId, AF_INET,SOCK_STREAM, IPPROTO_TCP, port);

			break;
		#endif

		case HPIC_MODE:
			break;

		default:
			rtn = MD_FAILED;

			break;
	}

	return rtn;

}



MS_INT32 listenComm(MS_INT32 scomId,MS_INT32 *cmid,COMM_MODE mode)
{
	MS_INT32 rtn = MD_OK;

	switch (mode) {
		case SOCKET_TCP_MODE:
		#ifdef USES_SOCKET_LIB
		//case SOCKET_UDP_MODE:
			rtn = ListenSock((SOCKET)scomId,(SOCKET *)cmid);

			break;
		#endif

		case HPIC_MODE:
			break;

		default:
			rtn = MD_FAILED;

			break;
	}

	return rtn;

}

		

MS_INT32 readDataFrmCommW(MS_INT32 cmid,MU_INT8 *recvBuf,MU_INT32 recvLen,COMM_MODE mode,MS_INT32 timeOut,MS_INT32 rtyTimes)
{

	MS_INT32 rtn = 0;

	switch (mode) {
		case SOCKET_TCP_MODE:
		case SOCKET_UDP_MODE:
		#ifdef USES_SOCKET_LIB
			//rtn = ListenSock((SOCKET)scomId,(SOCKET *)cmid);
			rtn = RecvSocketData((SOCKET)cmid,(MS_CHAR *)recvBuf,recvLen,timeOut,rtyTimes);
			break;
		#endif
		case HPIC_MODE:
			break;

		default:
			rtn = MD_FAILED;

			break;
	}

	return rtn;



}

MS_INT32 readDataFrmCommIm(MS_INT32 cmid,MU_INT8 *recvBuf,MU_INT32 recvLen,COMM_MODE mode)
{

	MS_INT32 	rtn = 0;
	MS_INT32	res;

	switch (mode) {
		case SOCKET_TCP_MODE:
		#ifdef USES_SOCKET_LIB
		//case SOCKET_UDP_MODE:
			//rtn = ListenSock((SOCKET)scomId,(SOCKET *)cmid);
			res	= RecvSocketReady((SOCKET)cmid);

			if (MD_OK == res)
				rtn = RecvSocketData((SOCKET)cmid,(MS_CHAR *)recvBuf,recvLen,3000,1);
			else if ((MS_INT32)MD_FAILED == res)
				rtn = MD_FAILED;


			break;
		#endif

		case HPIC_MODE:
		
			//rtn = RecvHPIData(cmid,(MS_CHAR *)recvBuf,recvLen);
			
			/*
			res = RecvHPIReady(cmid);

			if (MD_OK == res)
				rtn = RecvHPIData(cmid,(MS_CHAR *)recvBuf,recvLen);
			else if (MD_FAILED == res)
				rtn = MD_FAILED;
			*/

			break;

		default:
			rtn = MD_FAILED;

			break;
	}

	return rtn;



}

MS_INT32 sendDataToCommIm(MS_INT32 cmid,MU_INT8 *sendBuf1,MU_INT32 sendLen1,MU_INT8 *sendBuf2,MU_INT32 sendLen2,COMM_MODE mode)
{

	MS_INT32 rtn = 0;
	MS_INT32 res;

	switch (mode) {
		case SOCKET_TCP_MODE:
		case SOCKET_UDP_MODE:
		#ifdef USES_SOCKET_LIB
			//rtn = ListenSock((SOCKET)scomId,(SOCKET *)cmid);
			res = SendSocketReady((SOCKET)cmid);

			if (MD_OK == res)
			{
				rtn = SendSocketData((SOCKET)cmid,(MS_CHAR *)sendBuf1,sendLen1,(MS_CHAR *)sendBuf2,sendLen2,2000,1);//DEF_COMM_WRITE_WAITTIME_IM);
				//printf("send Data size =%d.\n",rtn);
			}
			else if (res ==  (MS_INT32)MD_FAILED)
				rtn = MD_FAILED;
				
			break;
		#endif

		case HPIC_MODE:
		
			//rtn = SendHPIData(cmid,(MS_CHAR *)sendBuf1,sendLen1,(MS_CHAR *)sendBuf2,sendLen2);
			
			/*
			res	= SendHPIReady(cmid);

			if (MD_OK == res)
				rtn = SendHPIData(cmid,(MS_CHAR *)sendBuf1,sendLen1,(MS_CHAR *)sendBuf2,sendLen2);
			else if (MD_FAILED == res)
				rtn = MD_FAILED;
			*/
			break;

		default:
			rtn = MD_FAILED;

			break;
	}

	return rtn;



}


MS_INT32 sendDataToCommW(MS_INT32 cmid,MU_INT8 *sendBuf1,MU_INT32 sendLen1,MU_INT8 *sendBuf2,MU_INT32 sendLen2,COMM_MODE mode,MS_INT32 timeOut,MS_INT32 rtyTimes)
{

	MS_INT32 rtn = 0;

	switch (mode) {
		case SOCKET_TCP_MODE:
		case SOCKET_UDP_MODE:
		#ifdef USES_SOCKET_LIB
			rtn = SendSocketData((SOCKET)cmid,(MS_CHAR *)sendBuf1,sendLen1,(MS_CHAR *)sendBuf2,sendLen2,timeOut,rtyTimes);//DEF_COMM_WRITE_WAITTIME_W);
			break;
		#endif
		case HPIC_MODE:
		
		
			/*		
			while (MD_OK != SendHPIReady(cmid))
				ms_sleep(10);

			rtn = SendHPIData(cmid, (MS_CHAR *)sendBuf1, sendLen1,(MS_CHAR *)sendBuf2,sendLen2);
			*/
			break;

		default:
			rtn = MD_FAILED;

			break;
	}

	return rtn;



}




