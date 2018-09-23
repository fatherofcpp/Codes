
#ifdef CHIP_DM642
#include <std.h>
#include <sys.h>
#endif

#include "../include/commapp.h"
#include "../include/commbase.h"

#if 0
typedef enum
{
	SOCKET_TCP_MODE = 0,
	SOCKET_UDP_MODE,
	HPIC_MODE,
	MEM_MODE,
	FILE_MODE

}COMM_MODE;

#endif

extern COMM_MODE	g_commType ;//HPIC_MODE
COMM_MODE             g_commType =SOCKET_TCP_MODE  ;

//static	MS_INT32	m_lisSocket = MD_INVALID;

MS_INT32 openNewSession(MS_INT32 *cmid,MS_CHAR *pServerAddr,MU_INT32 m_port)
{
	SOCKET_PARAM params ;
	
	params.m_serverAdd = pServerAddr;
	params.m_port = m_port;
	
	return openNewComm(cmid,&params,g_commType,1000);
	
}


MS_INT32 closeSession(MU_INT32 *ComId)
{

	if (*ComId !=  MD_INVALID)
	{
		switch (g_commType) {

			case  HPIC_MODE:
				*ComId = MD_INVALID;
				break;

			case SOCKET_TCP_MODE:
				closeComm((MS_INT32*)ComId,SOCKET_TCP_MODE);
				break;
			case SOCKET_UDP_MODE:
			case MEM_MODE:
			case FILE_MODE:
				break;
			default:
				break;
		}
	}

	return MD_OK;
		

}

MS_INT32 recvComFrHost(MU_INT32 ComId,MS_CHAR *recvBuf,MU_INT32 bufLen)
{
	MS_INT32 rtn = 0;

	switch (g_commType) {

		case  HPIC_MODE:
			rtn =  readDataFrmCommIm((MS_INT32)ComId,(MU_INT8 *)recvBuf,bufLen,HPIC_MODE);
			break;

		case SOCKET_TCP_MODE:
			rtn =  readDataFrmCommIm((MS_INT32)ComId,(MU_INT8 *)recvBuf,4,SOCKET_TCP_MODE);
			if (rtn>0)
			{
				rtn = readDataFrmCommW((MS_INT32)ComId,(MU_INT8 *)(recvBuf+4),*(MU_INT32 *)recvBuf-4,SOCKET_TCP_MODE,DEF_COMM_READ_WAITTIME_W,1);
				if (rtn > 0)
					rtn += 4;
			}


			break;
		case SOCKET_UDP_MODE:
		case MEM_MODE:
		case FILE_MODE:
			break;
		default:
			break;
	}

	return rtn;

}

MS_INT32 sendComToHostW(MU_INT32 ComId,MS_CHAR *sendBuf1,MU_INT32 bufLen1,MS_CHAR *sendBuf2,MU_INT32 bufLen2)
{
	MS_INT32 rtn = 0;

	//rtn =  sendDataToCommIm((MS_INT32)ComId,(MU_INT8 *)sendBuf1,bufLen1,(MU_INT8 *)sendBuf2,bufLen2,g_commType);
	//if (g_commType == HPIC_MODE)
	//	lock_pend(1,SYS_FOREVER);

	rtn =  sendDataToCommW((MS_INT32)ComId,(MU_INT8 *)sendBuf1,bufLen1,(MU_INT8 *)sendBuf2,bufLen2,g_commType,DEF_COMM_WRITE_WAITTIME_W,1);

	//if (g_commType == HPIC_MODE)
	//	lock_post(1);
	return rtn;


}

MS_INT32 sendComToHostIm(MU_INT32 ComId,MS_CHAR *sendBuf1,MU_INT32 bufLen1,MS_CHAR *sendBuf2,MU_INT32 bufLen2)
{
	MS_INT32 rtn = 0;
	//static MU_INT32 flag=0;
	//if (g_commType == HPIC_MODE)
	//	lock_pend(1,SYS_FOREVER);

	rtn =  sendDataToCommIm((MS_INT32)ComId,(MU_INT8 *)sendBuf1,bufLen1,(MU_INT8 *)sendBuf2,bufLen2,g_commType);

	//if (g_commType == HPIC_MODE)
	//	lock_post(1);
		
		
	//if(flag!=0)
	//	rtn =  sendDataToCommW((MS_INT32)ComId,(MU_INT8 *)sendBuf1,bufLen1,(MU_INT8 *)sendBuf2,bufLen2,g_commType,DEF_COMM_WRITE_WAITTIME_W);
	//else
	//{
	//	rtn  = DAT_copy(sendBuf2,0x80000000,bufLen2);
	//	DAT_wait(rtn);
		//CACHE_clean(CACHE_L2ALL,0,0);
	//	rtn = bufLen2+bufLen1;
	//}
	//flag ^= 1;
		
	return rtn;


}




