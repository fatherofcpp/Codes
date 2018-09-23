

#ifndef DEF_H3CIPCLIB_H_
#define DEF_H3CIPCLIB_H_


#include "ipc/include/imos_sdk_def.h"
#include "ipc/include/imos_sdk_pub.h"
//#include "ipc/include/imos_sdk_func.h"


typedef ULONG (__stdcall* IMOS_MW_LOGIN)(IN CHAR *pcUserName,
                                        IN CHAR *pcPassword,
                                        IN CHAR *pcServerIP,
                                        IN USHORT usServerPort,
                                        OUT CHAR *pcUserID);

typedef ULONG (__stdcall*  IMOS_MW_STARTMEDIASTREAM)(IN CHAR *pcUserID,
                                                   IN ULONG ulChannel,
                                                   IN ULONG ulStreamID,
                                                   IN ULONG ulTransMode,
                                                   IN CHAR *pcIpAddress,
                                                   IN USHORT usPort,
                                                   IN BOOL_T bKeepAlive,
                                                   OUT ULONG *pulStreamHandle);

typedef ULONG (__stdcall*  IMOS_MW_SETDISPLAYPARAM)(IN IMOS_MW_DISPLAY_PARAM_S *pstDisplayParam);

typedef ULONG (__stdcall* IMOS_MW_STARTPLAY)(IN ULONG ulWndHandle,
                                            IN ULONG ulStreamHandle);

typedef VOID (STDCALL* IMOS_MW_DECODE_VIDEO_DATA_CALLBACK_PF)(IN ULONG ulStreamHandle,
                                                              IN const IMOS_MW_PICTURE_DATA_S *pstPictureData,
                                                              IN LONG lUserParam,
                                                              IN LONG lReserved);
typedef ULONG (__stdcall* IMOS_MW_SETDECODEDVIDEODATACALLBACK)(IN ULONG ulStreamHandle,
                                                              IN IMOS_MW_DECODE_VIDEO_DATA_CALLBACK_PF pfnDecodeVideoDataCBFun, 
                                                              IN BOOL_T bContinue,
                                                              IN LONG lUserParam);
typedef ULONG  (__stdcall* IMOS_MW_INITIATE)(VOID);

typedef ULONG (__stdcall* IMOS_MW_STOPPLAY)(IN ULONG ulWndHandle);
typedef ULONG (__stdcall* IMOS_MW_STOPMEDIASTREAM)(IN CHAR *pcUserID,
                                                  IN ULONG ulStreamHandle);
typedef ULONG (__stdcall* IMOS_MW_LOGOUT)(IN CHAR *pcUserID);
typedef ULONG (__stdcall* IMOS_MW_CLEANUP)(VOID);


extern IMOS_MW_LOGIN			IMOS_MW_Login; 
extern IMOS_MW_STARTMEDIASTREAM			IMOS_MW_StartMediaStream;
extern IMOS_MW_SETDISPLAYPARAM			IMOS_MW_SetDisplayParam;
extern IMOS_MW_STARTPLAY			IMOS_MW_StartPlay; 
extern IMOS_MW_SETDECODEDVIDEODATACALLBACK			IMOS_MW_SetDecodedVideoDataCallback;
extern IMOS_MW_INITIATE			IMOS_MW_Initiate; 

extern IMOS_MW_STOPPLAY			IMOS_MW_StopPlay; 
extern IMOS_MW_STOPMEDIASTREAM			IMOS_MW_StopMediaStream; 
extern IMOS_MW_LOGOUT			IMOS_MW_Logout; 
extern IMOS_MW_CLEANUP			IMOS_MW_Cleanup; 


int loadH3cIPCLib();


#endif
