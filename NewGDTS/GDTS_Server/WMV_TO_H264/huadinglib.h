

#ifndef DEF_HUADINGLIB_H_
#define DEF_HUADINGLIB_H_

//#include "common/types.h"
//#include "sdk_Config.h"
#include "VMS\\SDK\\include\\sdk_Header.h"
//#include "dvsstreamplayerctrl1.h"
#if 1
typedef int (__stdcall* VMS_INIT)(void);
typedef int (__stdcall* VMS_DESTORY)(void);
typedef int (__stdcall* VMS_REGCALLBACK)(IN Uint16 cbType, IN ClientCB cb);
typedef int (__stdcall* VMS_LOGIN)(IN Uint32 NvrIp, IN Uint16 NvrPort, IN CHAR *UserName, IN CHAR *Passwd, OUT Int32 *UserPriv);
typedef int (__stdcall* VMS_LOGOUT)(void);
typedef int (__stdcall* VMS_ERRCHSTRING)(IN SdkErrCode err, OUT CHAR *ErrString, INOUT Int32 *ErrStrLen);
typedef int (__stdcall* VMS_RSSTARTREPLY)(void *notiRs, void *notiMN);

typedef int (__stdcall*VMS_STARTMONITOR)(IN Uint8 live_type, IN Int64 ec_channel, IN Int64 dc_channel, IN Uint8 main_sub_flag, IN Int32 ms, IN Uint8 multicast_flag);
typedef int (__stdcall*VMS_STOPMONITOR)(IN Uint8 live_type, IN Int64 ec_channel, IN Int64 dc_channel);
typedef int (__stdcall*VMS_RSSTOPREPLY)(void *rsEndInfo);


extern VMS_INIT VMS_Init;//
extern VMS_DESTORY VMS_Destory;//
extern VMS_REGCALLBACK VMS_RegCallBack;//
extern VMS_LOGIN VMS_Login;//
extern VMS_LOGOUT VMS_Logout;//
extern VMS_ERRCHSTRING VMS_ErrCHString;//
extern VMS_RSSTARTREPLY VMS_RSStartReply;//


extern VMS_STARTMONITOR VMS_StartMonitor;
extern VMS_STOPMONITOR VMS_StopMonitor;
extern VMS_RSSTOPREPLY VMS_RSStopReply;


int loadHuaDingLib();
#endif


#endif
