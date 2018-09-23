

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "WMV_TO_H264Dlg.h"
#include "santachiLib.h"

ST_NET_INITCLIENTNETLIB                  					st_net_initClientNetLib						=NULL;	
ST_NET_INITSERVERINFO                    					st_net_initServerInfo						=NULL;	
ST_NET_USERLOGIN                         					st_net_userLogin							=NULL;	
ST_NET_DEINITSERVERINFO                  					st_net_deinitServerInfo						=NULL;	
ST_NET_GETDEVICETYPE                     					st_net_getDeviceType						=NULL;	
ST_NET_GETPARAM                          					st_net_getParam								=NULL;	
ST_NET_GETHEARTBEATSTATUS                					st_net_getHeartbeatStatus					=NULL;	
ST_NET_REGISTERSERVERALARMINFOCALLBACK   					st_net_registerServerAlarmInfoCallback		=NULL;	
ST_NET_OPENCHANNELSTREAM                 					st_net_openChannelStream					=NULL;	
ST_NET_STARTCHANNELSTREAM                					st_net_startChannelStream					=NULL;	
ST_NET_REGISTERCHANNELSTREAMCALLBACK     					st_net_registerChannelStreamCallback		=NULL;	
ST_NET_CLOSECHANNELSTREAM                					st_net_closeChannelStream					=NULL;	
ST_NET_USERLOGOUT                        					st_net_userLogout							=NULL;

ST_PLAYER_INITPLAYLIB                    					st_player_initPlayLib						=NULL;	
ST_PLAYER_INPUTNETFRAME                  					st_player_inputNetFrame						=NULL;	
ST_PLAYER_CREATEPLAYINSTANCE             					st_player_createPlayInstance				=NULL;	
ST_PLAYER_SETPLAYWND                     					st_player_setPlayWnd						=NULL;	
ST_PLAYER_STARTPLAY                      					st_player_startPlay							=NULL;	
ST_PLAYER_SETCUSHIONRANGE                					st_player_setCushionRange					=NULL;	
ST_PLAYER_REGISTERDECCALLBACK            					st_player_registerDecCallback				=NULL;	
ST_PLAYER_STOPPLAY                       					st_player_stopPlay							=NULL;	
ST_PLAYER_DESTROYPLAYINSTANCE            					st_player_destroyPlayInstance				=NULL;	

int loadSantachiLib(const char * dir)
{
	HMODULE hProcessModule=NULL;
	SetDllDirectory(g_CurrentDir+"\\santachiLib\\");
	hProcessModule	=  LoadLibraryA("decNetLib.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload decNetLib.dll failed\n");
		return -1;
	}

	st_net_initClientNetLib=(ST_NET_INITCLIENTNETLIB)GetProcAddress(hProcessModule,"st_net_initClientNetLib");
	if(st_net_initClientNetLib == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_initClientNetLib failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_initServerInfo=(ST_NET_INITSERVERINFO)GetProcAddress(hProcessModule,"st_net_initServerInfo");
	if(st_net_initServerInfo == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_initServerInfo failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_userLogin=(ST_NET_USERLOGIN)GetProcAddress(hProcessModule,"st_net_userLogin");
	if(st_net_userLogin == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_userLogin failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_deinitServerInfo=(ST_NET_DEINITSERVERINFO)GetProcAddress(hProcessModule,"st_net_deinitServerInfo");
	if(st_net_deinitServerInfo == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_deinitServerInfo failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_getDeviceType=(ST_NET_GETDEVICETYPE)GetProcAddress(hProcessModule,"st_net_getDeviceType");
	if(st_net_getDeviceType == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_getDeviceType failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_getParam=(ST_NET_GETPARAM)GetProcAddress(hProcessModule,"st_net_getParam");
	if(st_net_getParam == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_getParam failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_getHeartbeatStatus=(ST_NET_GETHEARTBEATSTATUS)GetProcAddress(hProcessModule,"st_net_getHeartbeatStatus");
	if(st_net_getHeartbeatStatus == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_getHeartbeatStatus failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_registerServerAlarmInfoCallback=(ST_NET_REGISTERSERVERALARMINFOCALLBACK)GetProcAddress(hProcessModule,"st_net_registerServerAlarmInfoCallback");
	if(st_net_registerServerAlarmInfoCallback == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_registerServerAlarmInfoCallback failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_openChannelStream=(ST_NET_OPENCHANNELSTREAM)GetProcAddress(hProcessModule,"st_net_openChannelStream");
	if(st_net_openChannelStream == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_openChannelStream failed[%d]\n",GetLastError());
		return -1;
	}
	st_net_startChannelStream=(ST_NET_STARTCHANNELSTREAM)GetProcAddress(hProcessModule,"st_net_startChannelStream");
	if(st_net_startChannelStream == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_startChannelStream failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_registerChannelStreamCallback=(ST_NET_REGISTERCHANNELSTREAMCALLBACK)GetProcAddress(hProcessModule,"st_net_registerChannelStreamCallback");
	if(st_net_registerChannelStreamCallback == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_registerChannelStreamCallback failed[%d]\n",GetLastError());
		return -1;
	}
	st_net_closeChannelStream=(ST_NET_CLOSECHANNELSTREAM)GetProcAddress(hProcessModule,"st_net_closeChannelStream");
	if(st_net_closeChannelStream == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_closeChannelStream failed[%d]\n",GetLastError());
		return -1;
	}

	st_net_userLogout=(ST_NET_USERLOGOUT)GetProcAddress(hProcessModule,"st_net_userLogout");
	if(st_net_userLogout == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_net_userLogout failed[%d]\n",GetLastError());
		return -1;
	}


	hProcessModule=NULL;
	hProcessModule	=  LoadLibraryA("Play.dll");
	if(hProcessModule == NULL)
	{
		m_Log.Add("ERR£ºload Play.dll failed\n");
		return -1;
	}

	st_player_initPlayLib=(ST_PLAYER_INITPLAYLIB)GetProcAddress(hProcessModule,"st_player_initPlayLib");
	if(st_player_initPlayLib == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_player_initPlayLib failed[%d]\n",GetLastError());
		return -1;
	}
	
	st_player_inputNetFrame=(ST_PLAYER_INPUTNETFRAME)GetProcAddress(hProcessModule,"st_player_inputNetFrame");
	if(st_player_inputNetFrame == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_player_inputNetFrame failed[%d]\n",GetLastError());
		return -1;
	}
	
	st_player_createPlayInstance=(ST_PLAYER_CREATEPLAYINSTANCE)GetProcAddress(hProcessModule,"st_player_createPlayInstance");
	if(st_player_createPlayInstance == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_player_createPlayInstance failed[%d]\n",GetLastError());
		return -1;
	}
	
	st_player_setPlayWnd=(ST_PLAYER_SETPLAYWND)GetProcAddress(hProcessModule,"st_player_setPlayWnd");
	if(st_player_setPlayWnd == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_player_setPlayWnd failed[%d]\n",GetLastError());
		return -1;
	}
	
	st_player_startPlay=(ST_PLAYER_STARTPLAY)GetProcAddress(hProcessModule,"st_player_startPlay");
	if(st_player_startPlay == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_player_startPlay failed[%d]\n",GetLastError());
		return -1;
	}

	st_player_setCushionRange=(ST_PLAYER_SETCUSHIONRANGE)GetProcAddress(hProcessModule,"st_player_setCushionRange");
	if(st_player_setCushionRange == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_player_setCushionRange failed[%d]\n",GetLastError());
		return -1;
	}

	st_player_registerDecCallback=(ST_PLAYER_REGISTERDECCALLBACK)GetProcAddress(hProcessModule,"st_player_registerDecCallback");
	if(st_player_registerDecCallback == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_player_registerDecCallback failed[%d]\n",GetLastError());
		return -1;
	}

	st_player_stopPlay=(ST_PLAYER_STOPPLAY)GetProcAddress(hProcessModule,"st_player_stopPlay");
	if(st_player_stopPlay == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_player_stopPlay failed[%d]\n",GetLastError());
		return -1;
	}

	st_player_destroyPlayInstance=(ST_PLAYER_DESTROYPLAYINSTANCE)GetProcAddress(hProcessModule,"st_player_destroyPlayInstance");
	if(st_player_destroyPlayInstance == NULL)
	{
		m_Log.Add("ERR£ºGetProcAddress st_player_destroyPlayInstance failed[%d]\n",GetLastError());
		return -1;
	}


	return 0;
}

