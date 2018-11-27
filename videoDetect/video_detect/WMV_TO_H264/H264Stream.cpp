/*
2013-12-6-JiCheng
video quality detect
*/
#include "StdAfx.h"
#include "H264Stream.h"
#include "WMV_TO_H264Dlg.h"
//#define MAXBUFFSIZE 120000
//DWORD portNum =1;
#include "SimpleResize.h"
#include "Log.h"
#include "netapi.h"
//
//
//#define STREAMPATH "microvideo"
#define STREAMPATH "live"

STREAMPROCESSOPEN ProcessOpen;
STREAMPROCESSSTART ProcessStart;
STREAMPROCESSCLOSE ProcessClose;

//video detect 
VideoDetect AVidDet[MAX_DETECT_NUM];
IntSiVa_General_API* APP[MAX_DETECT_NUM];
//end video detect
CLog m_Log;
CLog SenLog;
CCriticalSection m_ffmpeglock;
CCriticalSection m_Loglock;//mutex lock when close video stream
#include <algorithm>
using std::fill_n;

extern CWMV_TO_H264Dlg* Main;

#define DEF_PUSHSTREAM_SLEEP_TIME 10 //25  //40 //

#pragma pack(2)
const int Table_fv1[256]={ -180, -179, -177, -176, -174, -173, -172, -170, -169, -167, -166, -165, -163, -162, -160, -159, -158, -156, -155, -153, -152, -151, -149, -148, -146, -145, -144, -142, -141, -139, -138, -137, -135, -134, -132, -131, -130, -128, -127, -125, -124, -123, -121, -120, -118, -117, -115, -114, -113, -111, -110, -108, -107, -106, -104, -103, -101, -100, -99, -97, -96, -94, -93, -92, -90, -89, -87, -86, -85, -83, -82, -80, -79, -78, -76, -75, -73, -72, -71, -69, -68, -66, -65, -64, -62, -61, -59, -58, -57, -55, -54, -52, -51, -50, -48, -47, -45, -44, -43, -41, -40, -38, -37, -36, -34, -33, -31, -30, -29, -27, -26, -24, -23, -22, -20, -19, -17, -16, -15, -13, -12, -10, -9, -8, -6, -5, -3, -2, 0, 1, 2, 4, 5, 7, 8, 9, 11, 12, 14, 15, 16, 18, 19, 21, 22, 23, 25, 26, 28, 29, 30, 32, 33, 35, 36, 37, 39, 40, 42, 43, 44, 46, 47, 49, 50, 51, 53, 54, 56, 57, 58, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 79, 81, 82, 84, 85, 86, 88, 89, 91, 92, 93, 95, 96, 98, 99, 100, 102, 103, 105, 106, 107, 109, 110, 112, 113, 114, 116, 117, 119, 120, 122, 123, 124, 126, 127, 129, 130, 131, 133, 134, 136, 137, 138, 140, 141, 143, 144, 145, 147, 148, 150, 151, 152, 154, 155, 157, 158, 159, 161, 162, 164, 165, 166, 168, 169, 171, 172, 173, 175, 176, 178 };
const int Table_fv2[256]={ -92, -91, -91, -90, -89, -88, -88, -87, -86, -86, -85, -84, -83, -83, -82, -81, -81, -80, -79, -78, -78, -77, -76, -76, -75, -74, -73, -73, -72, -71, -71, -70, -69, -68, -68, -67, -66, -66, -65, -64, -63, -63, -62, -61, -61, -60, -59, -58, -58, -57, -56, -56, -55, -54, -53, -53, -52, -51, -51, -50, -49, -48, -48, -47, -46, -46, -45, -44, -43, -43, -42, -41, -41, -40, -39, -38, -38, -37, -36, -36, -35, -34, -33, -33, -32, -31, -31, -30, -29, -28, -28, -27, -26, -26, -25, -24, -23, -23, -22, -21, -21, -20, -19, -18, -18, -17, -16, -16, -15, -14, -13, -13, -12, -11, -11, -10, -9, -8, -8, -7, -6, -6, -5, -4, -3, -3, -2, -1, 0, 0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 17, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 37, 38, 39, 40, 40, 41, 42, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50, 50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 62, 63, 64, 65, 65, 66, 67, 67, 68, 69, 70, 70, 71, 72, 72, 73, 74, 75, 75, 76, 77, 77, 78, 79, 80, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 87, 88, 89, 90, 90 };
const int Table_fu1[256]={ -44, -44, -44, -43, -43, -43, -42, -42, -42, -41, -41, -41, -40, -40, -40, -39, -39, -39, -38, -38, -38, -37, -37, -37, -36, -36, -36, -35, -35, -35, -34, -34, -33, -33, -33, -32, -32, -32, -31, -31, -31, -30, -30, -30, -29, -29, -29, -28, -28, -28, -27, -27, -27, -26, -26, -26, -25, -25, -25, -24, -24, -24, -23, -23, -22, -22, -22, -21, -21, -21, -20, -20, -20, -19, -19, -19, -18, -18, -18, -17, -17, -17, -16, -16, -16, -15, -15, -15, -14, -14, -14, -13, -13, -13, -12, -12, -11, -11, -11, -10, -10, -10, -9, -9, -9, -8, -8, -8, -7, -7, -7, -6, -6, -6, -5, -5, -5, -4, -4, -4, -3, -3, -3, -2, -2, -2, -1, -1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43 };
const int Table_fu2[256]={ -227, -226, -224, -222, -220, -219, -217, -215, -213, -212, -210, -208, -206, -204, -203, -201, -199, -197, -196, -194, -192, -190, -188, -187, -185, -183, -181, -180, -178, -176, -174, -173, -171, -169, -167, -165, -164, -162, -160, -158, -157, -155, -153, -151, -149, -148, -146, -144, -142, -141, -139, -137, -135, -134, -132, -130, -128, -126, -125, -123, -121, -119, -118, -116, -114, -112, -110, -109, -107, -105, -103, -102, -100, -98, -96, -94, -93, -91, -89, -87, -86, -84, -82, -80, -79, -77, -75, -73, -71, -70, -68, -66, -64, -63, -61, -59, -57, -55, -54, -52, -50, -48, -47, -45, -43, -41, -40, -38, -36, -34, -32, -31, -29, -27, -25, -24, -22, -20, -18, -16, -15, -13, -11, -9, -8, -6, -4, -2, 0, 1, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 21, 23, 24, 26, 28, 30, 31, 33, 35, 37, 39, 40, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 60, 62, 63, 65, 67, 69, 70, 72, 74, 76, 78, 79, 81, 83, 85, 86, 88, 90, 92, 93, 95, 97, 99, 101, 102, 104, 106, 108, 109, 111, 113, 115, 117, 118, 120, 122, 124, 125, 127, 129, 131, 133, 134, 136, 138, 140, 141, 143, 145, 147, 148, 150, 152, 154, 156, 157, 159, 161, 163, 164, 166, 168, 170, 172, 173, 175, 177, 179, 180, 182, 184, 186, 187, 189, 191, 193, 195, 196, 198, 200, 202, 203, 205, 207, 209, 211, 212, 214, 216, 218, 219, 221, 223, 225 };
//下面两个结构是位图的结构
typedef struct gd_BITMAPFILEHEADER
{ 
	unsigned short bfType; 
	unsigned int bfSize; 
	unsigned short bfReserved1; 
	unsigned short bfReserved2; 
	unsigned int bfOffBits; 
}gd_BITMAPFILEHEADER; 

typedef struct gd_BITMAPINFOHEADER
{ 
	unsigned int biSize; 
	unsigned int biWidth; 
	unsigned int biHeight; 
	unsigned short biPlanes; 
	unsigned short biBitCount; 
	unsigned int biCompression; 
	unsigned int biSizeImage; 
	unsigned int biXPelsPerMeter; 
	unsigned int biYPelsPerMeter; 
	unsigned int biClrUsed; 
	unsigned int biClrImportant; 
}gd_BITMAPINFODEADER;

BOOL FolderExist(CString strPath);
BOOL CreateFolder(CString strPath);
static int recvHttpHead(int s,char *pBuf,int maxLen,int *pLen,CH264Stream* p_H264Stream);
int recReplyBody(int s,char*pBuf,int maxLen,CH264Stream* p_H264Stream);

/*************************************************************************************
函数名称：StrToInt
函数描述: transfer string to int
函数调用：CompareTime
全局变量：
参数1:address of the string 
参数2:lenth of the string
返 回 值: the int data transfered to
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

int StrToInt(char* p, int len)
{
	int i = 0, ret = 0;
	while(i < len)
	{
		if ((p[i] >= '0') && (p[i] <= '9'))
		{
			int value = p[i]-48;
			ret = ret*10 +value;
			i++;
		}
		else
			break;
	}
	return ret;
}
/*************************************************************************************
函数名称：CurTim
函数描述: get current time as YYYY-MM-DD hh:mm:ss
函数调用：SendStreamOff，parseTask，serverState，CycleDetect
全局变量：
参数:buffer to save this time
返 回 值： 
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6     V1.0        jicheng         创建
**************************************************************************************/
static void CurTim(char*dst)
{
	struct tm *t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);
	sprintf(dst,"%4d-%02d-%02d %02d:%02d:%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
}
/*************************************************************************************
函数名称：GetPathName
函数描述: create log path as c:/log/YYMMDD.log,and save this path
函数调用：SenStreamOffToServer,SenSerState,SenToSer,SendStreamOff,sendTask,serverState
全局变量：
参数1:buffer to save total path of this log
返 回 值：
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18    V1.0        jicheng         创建
**************************************************************************************/

void GetPathName(char*PathName)
{
	int ret=0;
	char tmp[64];
	memset(tmp,0,sizeof(tmp));
	struct tm *t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);
	sprintf(tmp,"%4d%02d%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
	sprintf(PathName,"%s\\%s.log",LogPath,tmp);
	ret=FolderExist(LogPath);
	if(ret==0){//not exist
		ret=CreateFolder(LogPath);
	}
}
/*2013-12-6-jc
Function:send service state
Param1:point to H264Stream
Param2:send content
*/
int SenStreamOffToServer(CH264Stream* H264Stream,char*SenCon)
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	char PathName[64];
	memset(PathName,0,sizeof(PathName));
	int	fdClient=-1;
	int senLen=0,port=0;
	CString	ip(p_H264Stream->DstIp);
	port=p_H264Stream->DstPort;
	int ret=0,i=0,len=0;
	senLen=strlen(SenCon);

	fdClient = ConnectTCP(ip,port);
	if (fdClient < 0){
		TRACE("[SenStreamOffToServer]:connect server failure\n");
		//	NetClose(fdClient);
		return -1;
	}
	ret = NetWrite(fdClient, (const uint8_t *)SenCon, senLen);
	if (ret < 0)
	{
		TRACE("[SenStreamOffToServer]:Send data failure\n");
		NetClose(fdClient);
		fdClient=-1;
		return -2;			
	}
	if(*SenCon){
		TRACE("[SenStreamOffToServer]:%s\n",SenCon);
	}
	if(IsSaveLog)
	{
		if(LogSaveType==0){//save as date
			GetPathName(PathName);
			m_Loglock.Lock();
			SenLog.InitLog(PathName);
			SenLog.Add(SenCon);
			m_Loglock.Unlock();
		}else if(LogSaveType==1){//save less than 8M
			m_Log.Add(SenCon);
		}

	}
	TRACE("[SenStreamOffToServer]:%s send server succ\n",p_H264Stream->SrcUrl);
	NetClose(fdClient);
	fdClient=-1;
	return 0;
}
/*2013-12-6-jc
Function:send service state
Param1:point to H264Stream
Param2:send content
*/
int SenSerState(CH264Stream* H264Stream,char*SenCon)
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;

	CString	ip(p_H264Stream->DstIp);
	int port=p_H264Stream->DstPort;


	int	fdClient=-1;
	int senLen=0;
	int ret=0,i=0,len=0;

	char PathName[64];

	memset(PathName,0,sizeof(PathName));

	senLen=strlen(SenCon);

	fdClient = ConnectTCP(ip,port);
	if (fdClient < 0){
		TRACE("[SenSerState]:connect server failure\n");
		//		NetClose(fdClient);
		return -1;
	}
	ret = NetWrite(fdClient, (const uint8_t *)SenCon, senLen);
	if (ret < 0){
		TRACE("[SenSerState]:Send data failure\n");
		NetClose(fdClient);
		fdClient=-1;
		return -2;			
	}
	if(*SenCon){
		TRACE("[SenSerSta]:%s\n",SenCon);
	}
	if(IsSaveLog)
	{
		if(LogSaveType==0){//save as date
			GetPathName(PathName);
			m_Loglock.Lock();
			SenLog.InitLog(PathName);
			SenLog.Add(SenCon);
			m_Loglock.Unlock();
		}else if(LogSaveType==1){//save less than 8M
			m_Log.Add(SenCon);
		}

	}
	TRACE("[SenSerState]:%s send server succ\n",p_H264Stream->SrcUrl);
	NetClose(fdClient);
	fdClient=-1;
	return 0;
}
/*************************************************************************************
函数名称：FormTim
函数描述: get current time HHMMSS
函数调用：CompareTime
全局变量：
参数:char* to save current time
返 回 值： 
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18      V1.0        jicheng         创建
**************************************************************************************/
static void FormTim(char*dst)
{
	struct tm *t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);
	sprintf(dst,"%02d%02d%02d",t->tm_hour,t->tm_min,t->tm_sec);
}
/*************************************************************************************
函数名称：CompareTime
函数描述: compare current time with beginTime and endTime which set in config
函数调用：CycleDetect
全局变量：
参数:
返 回 值： when current time is bettwen beginTime and endTime return 0;else return -1
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18      V1.0        jicheng         创建
**************************************************************************************/

static int CompareTime(void)
{
	time_t t;
	int nNowTim=0;
	int nBeginTim=0;
	int nEndTim=0;
	char tmp[16];
	memset(tmp,0,sizeof(tmp));

	FormTim(tmp);
	nNowTim=StrToInt(tmp,6);

	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"%s",WorkBeginTime);
	nBeginTim=StrToInt(tmp,6);

	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"%s",WorkEndTime);
	nEndTim=StrToInt(tmp,6);

	if(nNowTim>=nBeginTim && nNowTim<=nEndTim){
		return 0;
	}else{
		return -1;
	}
}
/*************************************************************************************
函数名称：ParStaTim
函数描述: transfer YYYY-MM-DD hh-mm-ss to YYYYMMDDhhmmss
函数调用：GetPictPath
全局变量：
参数1:point to H264Stream class
返 回 值：
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

int ParStaTim(char*dst,CH264Stream* H264Stream)
{
	//2013-12-11 14:17:12
	char*p1=NULL;
	char*p2=NULL;
	char*p3=NULL;
	char*p4=NULL;
	char*p5=NULL;
	char y[8];
	char m[4];
	char d[4];
	char h[4];
	char min[4];
	char s[4];
	memset(y,0,sizeof(y));
	memset(m,0,sizeof(m));
	memset(d,0,sizeof(d));
	memset(h,0,sizeof(h));
	memset(min,0,sizeof(min));
	memset(s,0,sizeof(s));

	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	char starTim[32];
	memset(starTim,0,sizeof(starTim));
	//memcpy(starTim,p_H264Stream->StaTim,32);
	memcpy(starTim,p_H264Stream->StaTim,p_H264Stream->StaTim.GetLength());
	p1=strstr(starTim,"-");
	if(!p1){
		return -1;
	}
	p2=strstr(p1+1,"-");
	if(!p2){
		return -1;
	}
	p3=strstr(p2+1," ");
	if(!p3){
		return -1;
	}
	p4=strstr(p3+1,":");
	if(!p4){
		return -1;
	}
	p5=strstr(p4+1,":");
	if(!p5){
		return -1;
	}
	memcpy(y,starTim,p1-starTim);
	memcpy(m,p1+1,p2-p1-1);
	memcpy(d,p2+1,p3-p2-1);
	memcpy(h,p3+1,p4-p3-1);
	memcpy(min,p4+1,p5-p4-1);
	memcpy(s,p5+1,2);
	sprintf(dst,"%s%s%s%s%s%s",y,m,d,h,min,s);
	return 0;
}
/*************************************************************************************
函数名称：CreateGuid
函数描述: create a guid
函数调用：FormSenDatStreamOff,parseTask,ForSerSen
全局变量：
参数:buffer to save this guid
返 回 值：
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18      V1.0        jicheng         创建
**************************************************************************************/

void CreateGuid(char*dst)
{
	GUID   guid; 
	while(1)
	{
		if   (S_OK   ==   ::CoCreateGuid(&guid)) 
		{ 
			sprintf(dst,"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X" 
				,guid.Data1 
				,guid.Data2 
				,guid.Data3 
				,guid.Data4[0],guid.Data4[1] 
			,guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5] 
			,guid.Data4[6],guid.Data4[7]); 
			return;
		} 
	}
}
/*************************************************************************************
函数名称：FolderExist
函数描述: test whether a Folder is exist
函数调用：GetPathName,GetPictPath
全局变量：
参数1:Folder path
返 回 值：1 as exist;0 as not exist
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18    V1.0        jicheng         创建
**************************************************************************************/

BOOL FolderExist(CString strPath)
{
	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind!=INVALID_HANDLE_VALUE) && 
		(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}
/*************************************************************************************
函数名称：FormSenDatStreamOff
函数描述: format vedio detect result send data
函数调用：SendStreamOff
全局变量：
参数1:point to class which has buff to save send data
参数2:point to class which has srcIp,SrcNam..
返 回 值：0
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

int	FormSenDatStreamOff(CH264Stream* p_H264Stream,char*dst)
{
	int YiChNum=8;

	int len=0,i=0;

	char s_body[1024];
	char s_content[1024];
	char guid[64];

	memset(s_body,0,sizeof(s_body));
	memset(s_content,0,sizeof(s_content));
	memset(guid,0,sizeof(guid));

	if(p_H264Stream->IsTask==0){
		CreateGuid(guid);
		p_H264Stream->StreamOffCmdId=guid;
	}else{

	}

	sprintf(s_body,
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
		"<root>\r\n"
		"   <DetectResult>\r\n"		
		"		<CmdID>%s</CmdID>\r\n"
		"		<Direct>%d</Direct>\r\n"
		"		<StartTime>%s</StartTime>\r\n"
		"		<StopTime>%s</StopTime>\r\n"
		"  		<StreamSrvIP>%s</StreamSrvIP>\r\n"
		"  		<StreamName>%s</StreamName>\r\n"
		"  		<Analyze>%s</Analyze>\r\n"
		"	</DetectResult>\r\n"
		"</root>\r\n"
		,p_H264Stream->StreamOffCmdId
		,p_H264Stream->direct
		,p_H264Stream->StreamOffStaTim
		,p_H264Stream->StreamOffStpTim
		,p_H264Stream->SrcIp
		,p_H264Stream->SrcNam
		,"000000001"
		);

	sprintf(s_content,
		"POST http://%s:%d%s HTTP/1.1\r\n"
		"Host:%s\r\n"
		"Content-Type: application/xml;charset=utf-8\r\n"
		"Content-Length:%d\r\n"
		"Connection: closed\r\n\r\n"
		"%s"
		,p_H264Stream->DstIp
		,p_H264Stream->DstPort
		,VedioDetectPath
		,p_H264Stream->DstIp
		,strlen(s_body)
		,s_body);
	memcpy(dst,s_content,strlen(s_content));
	return 0;
}

/*************************************************************************************
函数名称：CreateFolder
函数描述: create a Folder under an existed Folder 
函数调用：GetPathName,GetPictPath
全局变量：
参数:Folder path
返 回 值： 1 as suc;0 as fail
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18      V1.0        jicheng         创建
**************************************************************************************/

BOOL CreateFolder(CString strPath)
{
	SECURITY_ATTRIBUTES attrib;
	return ::CreateDirectory(strPath, NULL);
}
/*************************************************************************************
函数名称：GetPictPath
函数描述: get a picture's path 
函数调用：parseTask
全局变量：
参数1:buffer to save total path of this picture
参数2:pointer to CH264Stream
返 回 值：
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18    V1.0        jicheng         创建
**************************************************************************************/

int GetPictPath(char*PathName,CH264Stream* H264Stream,int picInd)
{
	//2013-12-11 14:17:12
	int ret=0;
	CString dstFile;
	char starTim[32];
	memset(starTim,0,sizeof(starTim));

	char tmp[256];
	memset(tmp,0,sizeof(tmp));
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	ret=ParStaTim(starTim,p_H264Stream);
	if(ret==-1){
		memset(starTim,0,sizeof(starTim));
		sprintf(starTim,"%s","dataerror");
		return -1;
	}

	struct tm *t;
	time_t tt;
	time(&tt);
	t=localtime(&tt);
	//	sprintf(tmp,"%4d%02d%02d\\%s_%s_%s_%d.bmp",
	sprintf(tmp,"%4d%02d%02d\\%s_%s_%s_%d.jpg",
		t->tm_year+1900,t->tm_mon+1,t->tm_mday,
		p_H264Stream->SrcNam,
		p_H264Stream->CmdId,
		starTim,
		picInd);
	sprintf(PathName,"%s\\%s",PicturePath,tmp);
	dstFile.Format("%s\\%4d%02d%02d",PicturePath,t->tm_year+1900,t->tm_mon+1,t->tm_mday);

	//检测配置文件里的图片路径 c:/pic/test/..
	//	ret=CreatFloder(dstFile);
	ret=FolderExist(PicturePath);
	if(ret==0){//not exist create
		ret=CreateFolder(PicturePath);
		if(ret==0){
			return -1;
		}
	}
	ret=FolderExist(dstFile);
	if(ret==0){//not exist
		ret=CreateFolder(dstFile);
		if(ret==0){
			return -1;
		}
	}
	return 0;
}

/*************************************************************************************
函数名称：SenToSer
函数描述: send vedio detect result to dstService
函数调用：sendTask
全局变量：
参数1:point to MyAvFrame
参数2:point to CH264Stream
返 回 值：0
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

int SenToSer(MyAvFrame *m_Frame,CH264Stream* H264Stream)
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;

	CString	ip(p_H264Stream->DstIp);
	int port=p_H264Stream->DstPort;

	int	fdClient=-1;
	int senLen=0;
	int ret=0,i=0,len=0;

	char SenCon[1024]="";
	char RecCon[1024]="";

	char PathName[64];

	memset(SenCon,0,sizeof(SenCon));
	memset(RecCon,0,sizeof(RecCon));

	memset(PathName,0,sizeof(PathName));

	memcpy(SenCon,(char*)m_Frame->pSenDat,strlen((char*)m_Frame->pSenDat));
	senLen=strlen(SenCon);

	fdClient = ConnectTCP(ip,port);
	if (fdClient < 0){
		TRACE("[SenToSer]:connect server failure\n");
		//		NetClose(fdClient);
		return -1;
	}
	ret = NetWrite(fdClient, (const uint8_t *)SenCon, senLen);
	if (ret < 0){
		TRACE("[SenToSer]:Send data failure\n");
		NetClose(fdClient);
		fdClient=-1;
		return -2;			
	}
	if(*SenCon){
		TRACE("SenTo:%s\n",SenCon);
	}
	//	logWri
	if(IsSaveLog)
	{
		if(LogSaveType==0){//save as date
			GetPathName(PathName);
			m_Loglock.Lock();
			SenLog.InitLog(PathName);
			SenLog.Add(SenCon);
			m_Loglock.Unlock();
		}else if(LogSaveType==1){//save less than 8M
			m_Log.Add(SenCon);
		}
	}
	//1223
	//	memset(RecCon,0,sizeof(RecCon));
	//	ret=NetRead(fdClient, (uint8_t *)RecCon, 1024, 1 );
	//	TRACE("[SenToSer]:%s\n",RecCon);
	//1223
	//add
	//int dataLen=0;
	//ret = recvHttpHead(fdClient,RecCon,sizeof(RecCon),&dataLen,p_H264Stream);
	//if(ret == 0)
	//{
	//	TRACE("[SenToSer]:%d###\n",dataLen);
	//}
	//memset(RecCon,0,sizeof(RecCon));
	//ret=recReplyBody(fdClient,RecCon,dataLen,p_H264Stream);
	//if(ret == 0)
	//{
	//	TRACE("[SenToSer]:%s###\n",RecCon);
	//}
	//end add
	TRACE("[SenToSer]:%s send server succ\n",p_H264Stream->SrcUrl);
	NetClose(fdClient);
	fdClient=-1;
	return 0;
}
/*************************************************************************************
函数名称：SendStreamOff
函数描述: send vedio detect result to dstService when video stream is offline
函数调用：parseTask
全局变量：TestCycle
参数1:point to CH264Stream
返 回 值：0
创建时间：2014-4-1
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-4-1    V1.0        jicheng         创建
**************************************************************************************/

int SendStreamOff(CH264Stream* p_H264Stream)
{
	//constant
	int totalTim=TestCycle;//detect cycle
	p_H264Stream->StreamOff=1;
	//variable
	int rtn=0,i=0;

	int reSendTim=0;//send fail numbers

	char PathName[64];
	char tmpTim[64];
	char sendDat[1024];

	memset(PathName,0,sizeof(PathName));
	memset(sendDat,0,sizeof(sendDat));
	memset(tmpTim,0,sizeof(tmpTim));

	CurTim(tmpTim);
	p_H264Stream->StreamOffStaTim.Format("%s",tmpTim);
	p_H264Stream->StreamOffStpTim.Format("%s",tmpTim);

	if(PRINT)
		TRACE("[SendStreamOff]:%s:before FormSenDatStreamOff\n",p_H264Stream->SrcUrl);
	FormSenDatStreamOff(p_H264Stream,sendDat);//rtn==0
	if(PRINT)
		TRACE("[SendStreamOff]:%s:after FormSenDatStreamOff\n",p_H264Stream->SrcUrl);

	rtn=-1;
	while(rtn<0)
	{
		if(reSendTim>=3){
			break;
		}
		rtn=SenStreamOffToServer(p_H264Stream,(char*)sendDat);
		if(rtn<0)
		{
			reSendTim++;
			Sleep(1000);
			if(IsSaveLog==1)
			{
				if(LogSaveType==0){//save as date
					memset(PathName,0,sizeof(PathName));
					GetPathName(PathName);
					m_Loglock.Lock();
					SenLog.InitLog(PathName);
					SenLog.Add("[sendTask]:sendfail:%s\n%s\nlen=%d\n",
						p_H264Stream->SrcNam,(char*)sendDat,strlen((char*)sendDat));
					m_Loglock.Unlock();
				}else if(LogSaveType==1){//save less than 8M
					m_Log.Add("[sendTask]:sendfail:%s\n%s\nlen=%d\n",
						p_H264Stream->SrcNam,(char*)sendDat,strlen((char*)sendDat));
				}
			}//end savelog
		}//end if rtn<0
	}//end while(rtn<0)

	reSendTim=0;

	if(PRINT)
	{
		//	m_Log.Add("[SendStreamOff]:%s over %d time cannot get sendData\n",p_H264Stream->SrcUrl,totalTim);
		TRACE("[SendStreamOff]:%s:over %d time cannot get sendData\n",p_H264Stream->SrcUrl,totalTim);
	}
	return 0;
}
//
/*2013-12-6-jc
Function:send vedio detect result thread
Param1:point to H264Stream class
*/

UINT _stdcall sendTask(void* H264Stream)
{
	//pointer
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	MyAvFrame *m_Frame=NULL;
	//constant
	int intervalTime=TestCycle;//test cycle
	int requireList=0;//require list from service
	int YiChNum=8;//8 numbers of YiChang
	int NoSendTsk=noSend;//0,normal;1,test
	//variable
	int rtn=0,i=0;
	int TIM=0;

	int reSendTim=0;//counts of send fail
	int IsGetDat=0;//have get data from array
	time_t NodatTim0=0;//old time,continue no data from arry one cycle
	time_t NodatTim1=0;//new time
	int SlepTim=0;//sleeping time

	char tmpTim[64];
	char sendDat[1024];//save send data
	char PathName[64];//log path

	memset(PathName,0,sizeof(PathName));
	memset(sendDat,0,sizeof(sendDat));
	memset(tmpTim,0,sizeof(tmpTim));

	time(&NodatTim0);
	while (true)
	{
		if(NoSendTsk==1)
		{
			p_H264Stream->senTskEnd=1;
			Sleep(1000);
			continue;
		}
		if(PRINT)
		{
			if(TIM%(5*5)==0){
				TRACE("[sendTask%s][thisIndex:%d]:%s is running,parTskEnd:%d,serStaEnd:%d,senTskEnd:%d\n",
					p_H264Stream->SrcNam,p_H264Stream->thisIndex,p_H264Stream->SrcUrl,p_H264Stream->parTskEnd,p_H264Stream->serStaEnd,p_H264Stream->senTskEnd);
			}
		}
		TIM++;

		Sleep(1000);
		if(Main->g_Stop == 1){
			TRACE("[sendTask]:Main->g_Stop == 1\n");
			break;
		}

		if(p_H264Stream->senTskEnd==1)
		{
			IsGetDat=0;
			time(&NodatTim0);
			time(&NodatTim1);
			if(SlepTim%5==0){
				TRACE("[sendTask%s]:%s is sleeping,parTskEnd:%d,serStaEnd:%d,senTskEnd:%d\n",
					p_H264Stream->SrcNam,p_H264Stream->SrcUrl,p_H264Stream->parTskEnd,p_H264Stream->serStaEnd,p_H264Stream->							senTskEnd);
				SlepTim=0;
			}
			continue;
		}

		m_Frame=NULL;
		p_H264Stream->m_streamlock.Lock();
		if (p_H264Stream->p_EncodeArray.GetSize()>0)
		{
			m_Frame=p_H264Stream->p_EncodeArray.ElementAt(0);
			p_H264Stream->p_EncodeArray.RemoveAt(0);
		}
		p_H264Stream->m_streamlock.Unlock();

		if (m_Frame!=NULL)
		{		
			if (NULL ==m_Frame->pSenDat)
			{
				if(PRINT){
					TRACE("[sendTask]:NULL ==m_Frame->pSenDat,%s\n",p_H264Stream->SrcUrl);
				}
				if (m_Frame!=NULL)
				{
					delete m_Frame;
					m_Frame=NULL;
				}
				continue;
			}

			time(&NodatTim0);//updata nodata old time

			if(1)			//sendToServer
			{
				rtn=-1;
				while(rtn<0)
				{
					if(reSendTim>=3){
						break;
					}
					rtn=SenToSer(m_Frame,p_H264Stream);
					if(rtn<0)
					{
						reSendTim++;
						Sleep(1000);
						m_Log.Add("sendfail:%s\n%s\nlen=%d\n",
							p_H264Stream->SrcNam,(char*)m_Frame->pSenDat,strlen((char*)m_Frame->pSenDat));
						if(IsSaveLog)
						{
							if(LogSaveType==0){//save as date
								memset(PathName,0,sizeof(PathName));
								GetPathName(PathName);
								m_Loglock.Lock();
								SenLog.InitLog(PathName);
								SenLog.Add("sendfail:%s\n%s\nlen=%d\n",
									p_H264Stream->SrcNam,(char*)m_Frame->pSenDat,strlen((char*)m_Frame->pSenDat));
								m_Loglock.Unlock();
							}else if(LogSaveType==1){//save less than 8M
								m_Log.Add("sendfail:%s\n%s\nlen=%d\n",
									p_H264Stream->SrcNam,(char*)m_Frame->pSenDat,strlen((char*)m_Frame->pSenDat));
							}//end else
						}//end if(IsSaveLog)
					}//end if(rtn<0)
				}//end while
			}//end sendToServer
			reSendTim=0;
			IsGetDat=1;

			if(p_H264Stream->parTskEnd==1)
				p_H264Stream->senTskEnd=1;


			if(PRINT){
				TRACE("[sendTask]:after send%s\n",p_H264Stream->SrcUrl);
			}

			if (m_Frame!=NULL)
			{
				delete m_Frame;
				m_Frame=NULL;
			}
		}//end if(m_Frame!=NULL)
		else{//m_Frame==NULL
			if(IsGetDat==1)//only waitting for parseTsk
			{
				if(p_H264Stream->parTskEnd==1)
					p_H264Stream->senTskEnd=1;
			}
			time(&NodatTim1);
			if(NodatTim1-NodatTim0>intervalTime)
			{
				if(p_H264Stream->parTskEnd==1)
					p_H264Stream->senTskEnd=1;
			}
		}//end m_Frame==NULL
	}//end while(1)

leave:

	if (m_Frame!=NULL)
	{
		delete m_Frame;
		m_Frame=NULL;
	}

#if 1
	if(PRINT){
		TRACE("[sendTask]:%s goto leave before while1\n",p_H264Stream->SrcUrl);
	}
	while(1){

		m_Frame=NULL;//宽，高，I帧等

		p_H264Stream->m_streamlock.Lock();
		if (p_H264Stream->p_EncodeArray.GetSize()>0)//模板CArry有数据
		{
			m_Frame=p_H264Stream->p_EncodeArray.ElementAt(0);//从模板CArray<MyAvFrame*,MyAvFrame*>获取数据
			p_H264Stream->p_EncodeArray.RemoveAt(0);//获取完删除
		}
		p_H264Stream->m_streamlock.Unlock();

		if (m_Frame!=NULL)
		{
			delete m_Frame;
			m_Frame=NULL;
		}else
			break;
	}

#endif

	if(PRINT){
		TRACE("[sendTask]:%s _endthreadex\n",p_H264Stream->SrcUrl);
	}
	HRESULT  hr=S_OK;
	_endthreadex(hr);

	return hr;
}
/*************************************************************************************
函数名称：str_to_float
函数描述: transfer string to float
函数调用：avdInit
全局变量：
参数1:address of the string 
参数2:lenth of the string
返 回 值: the float data transfered to
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

float str_to_float(char* p, int len)
{
	int i = 0;
	int XiaoShu=0;
	int XiaoShuDian=0;
	float ret = 0.0;
	while(i < len)
	{
		if ((p[i] >= '0') && (p[i] <= '9'))
		{
			int value = p[i]-48;
			ret = ret*10 +value;
			i++;
			if(XiaoShuDian==1)
			{
				XiaoShu++;
			}
		}
		else if(p[i]=='.')
		{
			XiaoShuDian=1;
			i++;			
		}
		else
		{
			for(i=0;i<XiaoShu;i++)
			{
				ret=ret/10;
			}
			break;
		}
	}
	return ret;
}
/*************************************************************************************
函数名称：avdInit
函数描述: init IntSiVa
函数调用：TestFrame
全局变量：AVidDet,APP
参数1:struct VideoDetect pointer
参数2:class CH264Stream pointer
返 回 值: 
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

void avdInit(CH264Stream* p_H264Stream,int ind)
{

	int i,j;
	int ObjIndex=4,Lock=1;
	char   path[256];	
	char path2[256];

	AVidDet[ind].Param._AlgType=16;
	AVidDet[ind].Param._ImgHeight=576;
	AVidDet[ind].Param._ImgWidth=704;
	AVidDet[ind].Param._CamYiChangParam._Black_Senstivity=str_to_float(p_H264Stream->YCH.DiuShi,10);
	AVidDet[ind].Param._CamYiChangParam._Cover_Senstivity=str_to_float(p_H264Stream->YCH.ZheDang,10);
	AVidDet[ind].Param._CamYiChangParam._Dim_Senstivity=str_to_float(p_H264Stream->YCH.DuiJiao,10);
	AVidDet[ind].Param._CamYiChangParam._Flicker_Senstivity=str_to_float(p_H264Stream->YCH.ShanShuo,10);
	AVidDet[ind].Param._CamYiChangParam._Forzon_Senstivity=str_to_float(p_H264Stream->YCH.DongJie,10);
	AVidDet[ind].Param._CamYiChangParam._IntensityAbnormal_Senstivity=str_to_float(p_H264Stream->YCH.LiangDu,10);
	AVidDet[ind].Param._CamYiChangParam._SaltNoise_Senstivity=str_to_float(p_H264Stream->YCH.XueHua,10);

	APP[ind]->IntSiVa_Parameter_Set(&(AVidDet[ind].Param));

}
/*************************************************************************************
函数名称：TestFrame
函数描述: use sdk to test every Frame and save 8 exceptions of each Frame if exits
函数调用：parseTask
全局变量：AVidDet,APP
参数1:point to MyAvFrame
参数2:point to int array to save times of 8 exceptions 
参数3:point to CH264Stream
返 回 值: 0
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6      V1.0        jicheng         创建
**************************************************************************************/
int SavePicture(MyAvFrame *m_Frame,char*picPath);

int TestFrame(MyAvFrame *m_Frame,int*pParRes,CH264Stream* H264Stream)
{
	/*
	IntSiVa_General_API* APP[5];
	IntSiVa_InPut Param;
	IntSiVa_OutPut Out;
	IntSiVa_InPutFrame Input;
	*/

	int YiChNum=8;
	int i,j;
	int ObjIndex=4,Lock=1,nFraNum=0;

	int rtn=0;
	char PathName[255];
	static int YiChCount=0;
	static int talNum=0;

	memset(PathName,0,sizeof(PathName));

	CH264Stream* p_H264Stream=H264Stream;

	BYTE*pDat=m_Frame->p_data;
	BYTE*data=m_Frame->p_Ydata;
	int width=m_Frame->i_width;
	int	height=m_Frame->i_height;

	talNum++;
//	TRACE("[TestFrame]:**[%s]**YiChCount[%d],talNum[%d]\n",p_H264Stream->SrcNam,YiChCount,talNum);

	if(p_H264Stream->IsTask==0)
	{
		int AlarmType =AVidDet[p_H264Stream->thisIndex].Param._AlgType;
		if(AVidDet[p_H264Stream->thisIndex].Param._ImgHeight != height || AVidDet[p_H264Stream->thisIndex].Param._ImgWidth != width)
		{
			//		AVidDet[p_H264Stream->thisIndex].APP[ObjIndex]->IntSiVa_Release();
			AVidDet[p_H264Stream->thisIndex].Param._ImgHeight=height;
			AVidDet[p_H264Stream->thisIndex].Param._ImgWidth=width;
			APP[p_H264Stream->thisIndex]->IntSiVa_Parameter_Set((&AVidDet[p_H264Stream->thisIndex].Param));
		}

		AVidDet[p_H264Stream->thisIndex].Input._CamYCFrame._GrayData = (char*)data;
		AlarmType =AVidDet[p_H264Stream->thisIndex].Param._AlgType;

		APP[p_H264Stream->thisIndex]->IntSiVa_Process(&AVidDet[p_H264Stream->thisIndex].Input,&Lock);
		APP[p_H264Stream->thisIndex]->IntSiVa_GetOutput(&AVidDet[p_H264Stream->thisIndex].Out);

		if(AlarmType&Event_CamYiChang)
		{
			for (i=0;i<YiChNum;i++)
			{
				if (AVidDet[p_H264Stream->thisIndex].Out._CamYiChangOutPut._alarmoutput[i])
				{
					pParRes[i]++;//黑屏,遮挡,模糊,亮度异常,冻结,噪声,闪屏,滚纹
					//test
#if 0
					if(i == 7)
					{
						YiChCount++;
					//	sprintf(PathName,"c:\\pic\\20140829\\GunWenYiChang_%d.jpg",YiChCount);
						sprintf(PathName,"c:\\pic\\20140829\\GunWenYiChang_%d.yuv",YiChCount);


						//save jpg
						//rtn=SavePicture(m_Frame,PathName);
						//if(rtn==-1)
						//{
						//	TRACE("[TestFrame]:%s after SavePicture,rtn:%d\n",p_H264Stream->SrcUrl,rtn);
						//}
						//end save jpg

						//save Y
						//FILE*fp = fopen((const char *)PathName, "wb");
						//if(fp == NULL){
						//	return -1;
						//}
						//rtn=fwrite(m_Frame->p_Ydata,1,m_Frame->i_width*m_Frame->i_height,fp);
						//fclose(fp);
						//end save Y
					}
#endif
					//end test
				}
			}
		}
	}//end IsTask==0
	if(p_H264Stream->IsTask==1)
	{		
		int ind=p_H264Stream->thisIndex;//StartNewDetect have decide 
		//ind=FindApp();
		if(ind<16 || ind>31){//
			//			return -1;
		}
		avdInit(p_H264Stream,ind);
		int AlarmType =AVidDet[ind].Param._AlgType;
		if(AVidDet[ind].Param._ImgHeight != height || AVidDet[ind].Param._ImgWidth != width)
		{

			AVidDet[ind].Param._ImgHeight=height;
			AVidDet[ind].Param._ImgWidth=width;
			APP[ind]->IntSiVa_Parameter_Set(&(AVidDet[ind].Param));
		}

		AVidDet[ind].Input._CamYCFrame._GrayData = (char*)data;
		AlarmType =AVidDet[ind].Param._AlgType;

		APP[ind]->IntSiVa_Process(&(AVidDet[ind].Input),&Lock);
		APP[ind]->IntSiVa_GetOutput(&(AVidDet[ind].Out));

		if(AlarmType&Event_CamYiChang)
		{
			for (i=0;i<YiChNum;i++){
				if (AVidDet[ind].Out._CamYiChangOutPut._alarmoutput[i]){
					pParRes[i]++;
				}
			}
		}
		//		APP[ind]->IntSiVa_Release();
	}//end IsTask==1
	return 0;
}
/*************************************************************************************
函数名称：FormSenDat
函数描述: format vedio detect result send data
函数调用：parseTask
全局变量：
参数1:point to class which has buff to save send data
参数2:point to class which has srcIp,SrcNam..
返 回 值：0
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

int	FormSenDat(MyAvFrame *m_tmp,CH264Stream* p_H264Stream)
{
	int len=0;
	char s_body[1024];
	char s_content[1024];
	memset(s_body,0,sizeof(s_body));
	memset(s_content,0,sizeof(s_content));

	sprintf(s_body,
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
		"<root>\r\n"
		"   <DetectResult>\r\n"		
		"		<CmdID>%s</CmdID>\r\n"
		"		<Direct>%d</Direct>\r\n"
		"		<StartTime>%s</StartTime>\r\n"
		"		<StopTime>%s</StopTime>\r\n"
		"  		<StreamSrvIP>%s</StreamSrvIP>\r\n"
		"  		<StreamName>%s</StreamName>\r\n"
		"  		<Analyze>%s</Analyze>\r\n"
		"	</DetectResult>\r\n"
		"</root>\r\n"
		,p_H264Stream->CmdId
		,p_H264Stream->direct
		,p_H264Stream->StaTim
		,p_H264Stream->StpTim
		,p_H264Stream->SrcIp
		,p_H264Stream->SrcNam
		,m_tmp->p_vidDet
		);

	sprintf(s_content,
		"POST http://%s:%d%s HTTP/1.1\r\n"
		"Host:%s\r\n"
		"Content-Type: application/xml;charset=utf-8\r\n"
		"Content-Length:%d\r\n"
		"Connection: closed\r\n\r\n"
		"%s"
		,p_H264Stream->DstIp
		,p_H264Stream->DstPort
		,VedioDetectPath
		,p_H264Stream->DstIp
		,strlen(s_body)
		,s_body);

	memcpy(m_tmp->pSenDat,s_content,strlen(s_content));
	return 0;
}

/*************************************************************************************
函数名称：ParFrame
函数描述: fp[i]+=np[i]*coe
函数调用：parseTask
全局变量：
参数1:point to int array which contains times of each 8 exception of vedio in 1/3 cycle
参数2:point to float array which save each 8 ex of vedio in total cycle
参数3:multiple
返 回 值：
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

void ParFrame(int*np,float*fp,float coe,CH264Stream* p_H264Stream)
{
	int YiChNum=8;
	int i;

//	m_Log.Add("@@ParFrame");
	for(i=0;i<YiChNum;i++){
		fp[i]+=np[i]*coe;
	//	m_Log.Add("ParFrame[%d][%d] ",i,np[i]);
	}
//	m_Log.Add("##ParFrame");
		
	m_Log.Add("ParFrame:[srcUrl:%s][thisIndex:%d]\n[Black_Senstivity信号丢失_黑屏0[%d],Cover_Senstivity遮挡1[%d],Dim_Senstivity对焦_模糊2[%d],IntensityAbnormal_Senstivity亮度3[%d],Forzon_Senstivity画面冻结4[%d],SaltNoise_Senstivity雪花噪声5[%d],Flicker_Senstivity图像闪烁6[%d],RollStripe_Senstivity滚动条纹[%d]]",p_H264Stream->SrcUrl,p_H264Stream->thisIndex,np[0],np[1],np[2],np[3],np[4],np[5],np[6],np[7]);

}

/*************************************************************************************
函数名称：YV12_To_RGB24
函数描述: transfer YUV420 data to RGB data
函数调用：SavePicture
全局变量：Table_fv1,Table_fu1,Table_fv2,Table_fu2,pRGB24
参数1:YUV data
参数2:RGB data
参数3:width of YUV 
参数4:height of YUV
返 回 值:1 as suc,0 as fail
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6     V1.0        jicheng         创建
**************************************************************************************/

bool YV12_To_RGB24(const unsigned char* pYV12, unsigned char* pRGB24, const int& width, const int& height)
{
	if(NULL == pYV12)
		return false;

	//	long buffsize = width * 24 * height;

	//pRGB24 = new unsigned char[buffsize];

	if(!pYV12 || !pRGB24){
		return false;
	}

	const long nYLen = long(width*height);
	const int nHfWidth = (width>>1);

	if(nYLen<1 || nHfWidth<1){
		return false;
	}

	// Y data
	const unsigned char* yData = pYV12;
	// v data
	const unsigned char* vData = &yData[nYLen];
	// u data
	const unsigned char* uData = &vData[nYLen>>2];

	if(!uData || !vData)
		return false;

	int rgb[3];
	int i, j, m, n, x, y, py, rdif, invgdif, bdif;
	m = -width;
	n = -nHfWidth;

	bool addhalf = true;
	for(y=0; y<height;y++) {
		m += width;
		if( addhalf ){
			n+=nHfWidth;
			addhalf = false;
		} else {
			addhalf = true;
		}
		for(x=0; x<width;x++)  {
			i = m + x;
			j = n + (x>>1);

			py = yData[i];

			// search tables to get rdif invgdif and bidif
			rdif = Table_fv1[vData[j]];    // fv1
			invgdif = Table_fu1[uData[j]] + Table_fv2[vData[j]]; // fu1+fv2
			bdif = Table_fu2[uData[j]]; // fu2

			rgb[2] = py+rdif;    // R
			rgb[1] = py-invgdif; // G
			rgb[0] = py+bdif;    // B

			//j = nYLen - width - m + x;//?a???áí???μ?μ1
			j = m + x;
			i = (j<<1) + j;

			// copy this pixel to rgb data
			for(j=0; j<3; j++){
				if(rgb[j]>=0 && rgb[j]<=255){
					pRGB24[i + j] = rgb[j];
				}else{
					pRGB24[i + j] = (rgb[j] < 0)? 0 : 255;
				}
			}
		}
	}
	return true;
}
/*************************************************************************************
函数名称：saveFrame
函数描述: transfer  RGB data to .jpg picture
函数调用：SavePicture
全局变量：
参数1:picture path
参数2:RGB data
参数3:width of YUV
参数4:height of YUV
返 回 值：0 
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

int saveFrame(unsigned char * pFileName, unsigned char *pRGBBuffer, int width, int height)
{
	gd_BITMAPFILEHEADER bmpheader;
	gd_BITMAPINFOHEADER bmpinfo;
	FILE *fp;

	unsigned int uiTmp, uiTmp2;
	unsigned char *ucTmp = NULL;
	unsigned char ucRGB;
	int i;

	uiTmp = (width*3+3)/4*4*height;
	uiTmp2 = width*height*3;

	/*
	fp=fopen((const char*)pFileName, "wb");
	if(fp==NULL)
	return;

	//文件标识"BM"（即0x4D42）表示位图
	bmpheader.bfType = 0x4D42;
	//保留。设置为0
	bmpheader.bfReserved1 = 0;
	//保留。设置为0
	bmpheader.bfReserved2 = 0;
	//从文件开始到位图数据的偏移量（单位：字节）
	bmpheader.bfOffBits = sizeof(bmpheader) + sizeof(bmpinfo);
	//整个文件的大小（单位：字节）
	bmpheader.bfSize = bmpheader.bfOffBits + uiTmp;

	//信息头长度（单位：字节）。典型值为28
	bmpinfo.biSize = 0x28;
	//位图宽度（单位：像素）
	bmpinfo.biWidth = width;
	//位图高度（单位：像素）。若其为正，表示倒向的位图。若为负，表示正向的位图
	bmpinfo.biHeight = height;
	//位图的面数（为1）
	bmpinfo.biPlanes = 1;
	//每个像素的位数
	bmpinfo.biBitCount = 24;
	//压缩说明。0(BI_RGB)表示不压缩
	bmpinfo.biCompression = 0;
	//用字节数表示的位图数据的大小（为4的位数）
	bmpinfo.biSizeImage = uiTmp;
	//水平分辨率（单位：像素/米）
	bmpinfo.biXPelsPerMeter = 0;
	//垂直分辨率（单位：像素/米）
	bmpinfo.biYPelsPerMeter = 0;
	//位图使用的颜色数
	bmpinfo.biClrUsed = 0;
	//重要的颜色数
	bmpinfo.biClrImportant = 0;

	fwrite(&bmpheader,sizeof(bmpheader),1,fp);
	fwrite(&bmpinfo,sizeof(bmpinfo),1,fp);

	//把图像数据倒置
	unsigned char tmp[1080*3];//临时数据
	for(i = 0; i < height/2; i++)
	{
	memcpy(tmp, &(pRGBBuffer[width*i*3]), width*3);
	memcpy(&(pRGBBuffer[width*i*3]), &(pRGBBuffer[width*(height-1-i)*3]), width*3);
	memcpy(&(pRGBBuffer[width*(height-1-i)*3]), tmp, width*3);
	}

	fwrite(pRGBBuffer,width*height*3,1,fp);
	fclose(fp);
	*/
	struct jpeg_compress_struct jcs;

	// 声明错误处理器，并赋值给jcs.err域
	struct jpeg_error_mgr jem;
	jcs.err = jpeg_std_error(&jem);

	jpeg_create_compress(&jcs);

	fp = fopen((const char *)pFileName, "wb");
	if(fp == NULL){
		return -1;
	}

	jpeg_stdio_dest(&jcs, fp);

	jcs.image_width = width;    // 为图的宽和高，单位为像素 
	jcs.image_height = height;
	jcs.input_components = 3;   // 在此为1,表示灰度图， 如果是彩色位图，则为3 
	jcs.in_color_space = JCS_RGB; //JCS_GRAYSCALE表示灰度图，JCS_RGB表示彩色图像

	jpeg_set_defaults(&jcs); 
	jpeg_set_quality (&jcs, 80, true);

	jpeg_start_compress(&jcs, TRUE);

	JSAMPROW row_pointer[1];   // 一行位图
	int row_stride = jcs.image_width * 3;//每一行的字节数,如果不是索引图,此处需要乘以3

	// 对每一行进行压缩
	while (jcs.next_scanline < jcs.image_height) {
		row_pointer[0] = &(pRGBBuffer[jcs.next_scanline * row_stride]);
		jpeg_write_scanlines(&jcs, row_pointer, 1);
	}

	jpeg_finish_compress(&jcs);
	jpeg_destroy_compress(&jcs);

	fclose(fp);
	return 0;
}
/*************************************************************************************
函数名称：SavePicture
函数描述: transfer a Frame to a picture .jpg
函数调用：parseTask
全局变量：
参数1:MyAvFrame pointer
参数2:picture path
返 回 值：0 as suc;-1 as fail
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

int SavePicture(MyAvFrame *m_Frame,char*picPath)
{
	int ret=0;
	int len=0;
	int picWidth = m_Frame->i_width;
	int picHeight = m_Frame->i_height;
	FILE* fp;
	char* picFile = picPath;

	unsigned char *rgb_buf = new unsigned char[picWidth*picHeight*3];
	if(rgb_buf==NULL){
		m_Log.Add("[savePicture]:new rgb_buf return -1\n");
		return -1;
	}
	memset(rgb_buf,0,picWidth*picHeight*3);
	//	ret=YV12_To_RGB24((const unsigned char*)pYUV, (unsigned char*) rgb_buf, picWidth, picHeight);
	ret=YV12_To_RGB24((const unsigned char*)(m_Frame->p_data), (unsigned char*) rgb_buf, picWidth, picHeight);
	if(ret!=1)
	{
		if(rgb_buf){
			free(rgb_buf);
		}
		m_Log.Add("%s:[savePicture]:YV12_To_RGB24 return -1",picPath);
		return -1;
	}
	ret=saveFrame((unsigned char*)picFile,(unsigned char *)rgb_buf, picWidth, picHeight);
	if(ret==-1){
		if(rgb_buf){
			free(rgb_buf);
		}
		m_Log.Add("%s:[savePicture]:saveFrame return -1",picPath);
		return -1;
	}
	if(rgb_buf){
		free(rgb_buf);
	}
	return 0;
}
/*************************************************************************************
函数名称：DeletePic
函数描述: if a  picture  is normal then delete it
函数调用：parseTask
全局变量：
参数1:path of picture
参数2:detect result of vedio
返 回 值：0 as suc;-1 as fail
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6     V1.0        jicheng         创建
**************************************************************************************/

int	DeletePic(char*PathName,char*chDetRes,CH264Stream* p_H264Stream)
{
	//  c:\pic\20131217\ipcamera_AF00B46F-9F06-45d5-A273-68DF5B449596_20131217104653_3.jpg
	//	TRACE("%s\n",PathName);


	int i=0,ret=0;
	int count=0;
	char*pJpg=NULL;
	char tmp[256];
	char delFile[3][256];
	memset(tmp,0,sizeof(tmp));
	memset(delFile,0,sizeof(delFile));

	pJpg=strstr(PathName,".jpg");
	if(!pJpg){
		m_Log.Add("[DeletePic]:strstr---.jpg return -1,SrcUrl:%s,PathName:%s\n",p_H264Stream->SrcUrl,PathName);
		return -1;
	}
	memcpy(tmp,PathName,pJpg-PathName-1);

	for(i=0;i<7;i++)
	{
		//TRACE("%c ",chDetRes[i]);
		if(chDetRes[i]=='1'){
			count=1;
		}
	}

	if(count==0)
	{
		for(i=0;i<3;i++){
			sprintf(delFile[i],"%s%d.jpg",tmp,i+1);
			ret=remove(delFile[i]);
			if(ret!=0){
				m_Log.Add("[DeletePic]:remove return -1,SrcUrl:%s,delFile:%s\n",p_H264Stream->SrcUrl,delFile);
				return -1;
			}
		}	
	}
	return 0;
}

/*************************************************************************************
函数名称：UnitSampling
函数描述: UnitSampling
函数调用：parseTask
全局变量：
参数1:
返 回 值：
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/
int UnitSampling(int*src,int*dst,CString srcUrl)
{
	int YiChNum=8;//8 numbers of YiChang
	int GunWenInd=7;
	int nUnitFrameNum=40;//least frame number for GunWen YC 
	int nYCPercent=YiChPercent;//percent of Yichang

	for(int i=0;i<YiChNum;i++)
	{
		if(i == GunWenInd)
		{
			if(src[i]>0)
			{
				dst[i] += 1;
			}
		}else
		{
			if(100*src[i]>nUnitFrameNum*nYCPercent)
			{
				dst[i] += 1;
			}
		}
	}

	//m_Log.Add("UnitSampling:[srcUrl:%s]src:Black_Senstivity信号丢失_黑屏0[%d],Cover_Senstivity遮挡1[%d],Dim_Senstivity对焦_模糊2[%d],IntensityAbnormal_Senstivity亮度3[%d],Forzon_Senstivity画面冻结4[%d],SaltNoise_Senstivity雪花噪声5[%d],Flicker_Senstivity图像闪烁6[%d],RollStripe_Senstivity滚动条纹[%d]",srcUrl,src[0],src[1],src[2],src[3],src[4],src[5],src[6],src[7]);
	//m_Log.Add("UnitSampling:[srcUrl:%s]dst:Black_Senstivity信号丢失_黑屏0[%d],Cover_Senstivity遮挡1[%d],Dim_Senstivity对焦_模糊2[%d],IntensityAbnormal_Senstivity亮度3[%d],Forzon_Senstivity画面冻结4[%d],SaltNoise_Senstivity雪花噪声5[%d],Flicker_Senstivity图像闪烁6[%d],RollStripe_Senstivity滚动条纹[%d]",srcUrl,dst[0],dst[1],dst[2],dst[3],dst[4],dst[5],dst[6],dst[7]);
	return 0;
}

/*************************************************************************************
函数名称：parseTask
函数描述: detect vedio quality thread
函数调用：CH264Stream::StartPush(int Width, int Height)
全局变量：YiChPercent,TestCycle,parseTest,Main
参数1:point to H264Stream class
返 回 值：
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

UINT _stdcall parseTask(void* H264Stream)//0320
{
	//pointer
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	MyAvFrame *m_Frame=NULL;
	//constant
	float coe[3]={0.1,0.2,0.7};//YiCha XiShu of each 1/3 testCycle
	int YiChNum=8;//8 numbers of YiChang
	int YCPercent=YiChPercent;//percent of Yichang
	int intervalTime=TestCycle;//test cycle
	//int YChCount=(25*intervalTime/3)*(YCPercent*0.01);//counts of video detect in one cycle
//	int YChCount=0;//counts of video detect in one cycle
	float YChCount=0.0;//counts of video detect in one cycle
	
	int TotalFrame=0;//total frame detected in one cycle
	int requireList=0;//require list from service
	int ParseDebug=parseTest;//0,normal;1,test
	//variable
	int rtn=0,i=0;
	int TIM=0;//counts to TRACE
	int SlepTim=0;//counts to Trace sleep

	int Updata=1;//updata start time
	int resetAll=0;//if err continue

	int resOldTim=1;//updata oldTim
	int resTestOldTim=1;//updata TestOldTim
	int resNodatTim=1;//updata noDataTim
	
	int coeInd=0;//index of  YiCha XiShu
	int partTim=0;// n*(1/3)*cycleTim
	int partPicSav=0;//one picture saved

	time_t oldTim=0;//detect time flag
	time_t newTim=0;//
	time_t TestOldTim=0;//test one time one second
	time_t TestNewTim=0;
	time_t NodatTim0=0;//old time,continue no data from arry one cycle
	time_t NodatTim1=0;//new time

	float fDetectRes[8];//save YiCha counts with XiShu from begining of detect to now,fDetectRes=naParRes*coe
	int naParRes[8];//array to save counts of every YiCha  per frame,total 8 YiCha;黑屏,遮挡,模糊,亮度异常,冻结,噪声,闪屏,滚纹

	//USEUNIT
	int resUnitTim=1;//updata unitTime
	time_t UnitTim0=0;//Unit control time
	time_t UnitTim1=0;
	int nUnitTim=5;//time of each unit
	int nUnitNum=TestCycle/nUnitTim;//number of unit 
	int nUnitFrameNum=40;//least frame number for GunWen YC 
	int nUnitYC[8];//array to save counts of every YiCha  per unit


	//USEUNIT

	char chDetRes[9];//detect result 8(YiChang)+1(streamOn)
	char tmpTim[128];
	char PathName[128];//picture pataName
	char guid[64];

	memset(chDetRes,0,sizeof(chDetRes));
	memset(naParRes,0,sizeof(naParRes));
	memset(fDetectRes,0,sizeof(fDetectRes));
	memset(nUnitYC,0,sizeof(nUnitYC));



	memset(tmpTim,0,sizeof(tmpTim));
	memset(PathName,0,sizeof(PathName));
	memset(guid,0,sizeof(guid));

	time(&NodatTim0);

	while (true)
	{
		Sleep(100);

		if(p_H264Stream->serStaEnd==1)
		{
			if(noServerstate==0){
			//	p_H264Stream->parTskEnd=1;
			}
		}
		if(p_H264Stream->parTskEnd==1)
		{
			//add NetTsk
			if(p_H264Stream->IsTask == 1)
			{
				if(NULL!=p_H264Stream->h_stream)
				{
					ProcessClose(p_H264Stream->h_stream);
					p_H264Stream->h_stream=NULL;
				}
			}
			//end add NetTsk
			resetAll=1;
			if(SlepTim%5==0){
				TRACE("[parseTask%s][thisIndex:%d]:%s is sleeping,parTskEnd:%d,serStaEnd:%d,senTskEnd:%d\n",
					p_H264Stream->SrcNam,p_H264Stream->thisIndex,p_H264Stream->SrcUrl,p_H264Stream->parTskEnd,p_H264Stream->serStaEnd,p_H264Stream->senTskEnd);
				SlepTim=0;
			}
			SlepTim++;
			Sleep(1000);
			continue;
		}

		if(1==resetAll)//start to detect from a new start
		{
			if (m_Frame!=NULL){
				delete m_Frame;
				m_Frame=NULL;
			}
			Updata=1;
			resOldTim=1;
			resTestOldTim=1;
			resNodatTim=1;
			resUnitTim=1;
			coeInd=0;
			partPicSav=0;
			TotalFrame=0;
			memset(naParRes,0,sizeof(naParRes));
			memset(fDetectRes,0,sizeof(fDetectRes));
			memset(chDetRes,0,sizeof(chDetRes));
			memset(nUnitYC,0,sizeof(nUnitYC));

			resetAll=0;
		}

		if(resOldTim==1){//updata oldTim
			time(&oldTim);
			resOldTim=0;
		}

		if(resTestOldTim==1){//updat TestOldTim
			time(&TestOldTim);
			resTestOldTim=0;
		}

		if(resNodatTim==1){//update resNodatTim
			time(&NodatTim0);
			resNodatTim=0;
		}

		if(resUnitTim==1){//update resUnitTim
			time(&UnitTim0);
			resUnitTim=0;
		}

		if(Main->g_Stop == 1){//process exit
			if(PRINT){
				m_Log.Add("[parseTask]:Main->g_Stop == 1\n");
			}
			break;
		}

		if(PRINT)
		{
			if(TIM%(25*5)==0)
			{
				TRACE("[parseTask%s][thisIndex:%d]:%s is running resOldTim:%d,resTestOldTim:%d,Updata:%d,coeInd:%d\n",
					p_H264Stream->SrcNam,p_H264Stream->thisIndex,p_H264Stream->SrcUrl,resOldTim,resTestOldTim,Updata,coeInd);
				TIM=0;
			}
		}
		TIM++;

		//get video data from p_Array
		m_Frame=NULL;
		if(ParseDebug==0)
		{
			p_H264Stream->m_streamlock.Lock();
			if (p_H264Stream->p_Array.GetSize()>0)
			{
				m_Frame=p_H264Stream->p_Array.ElementAt(0);
				p_H264Stream->p_Array.RemoveAt(0);
			}
			p_H264Stream->m_streamlock.Unlock();
		}
		//end get data from p_Array
		if (m_Frame!=NULL)
		{	
			if (NULL ==m_Frame->p_data || 0 == m_Frame->i_height || 0 == m_Frame->i_width)//err data
			{
				m_Log.Add("%s:[parTask]:NULL ==m_Frame->p_data || 0 == m_Frame->i_height",p_H264Stream->SrcUrl);
				resetAll=1;
				continue;
			}	
			//end err data
			time(&NodatTim0);//updata nodata old time
			//updata a new send
			if(Updata==1)
			{
				//get startTim
				memset(tmpTim,0,sizeof(tmpTim));
				CurTim(tmpTim);
				p_H264Stream->StaTim.Format("%s",tmpTim);
				//end get startTim

				//get direct , guid
				if(p_H264Stream->IsTask==0){//local report
					memset(guid,0,sizeof(guid));
					CreateGuid(guid);
					p_H264Stream->CmdId.Format("%s",guid);
				}else{//netTsk

				}
				if(p_H264Stream->RePullByNetTsk==1){//拉取相同流
					p_H264Stream->CmdId=p_H264Stream->NetTskCmId;
					//	p_H264Stream->direct=1;
				}
				//end get direct,guid
				//repull
				Updata=0;
			}//end updata a new send

			//test a frame
			time(&TestNewTim);
			if(ParseDebug==0)
			{
			//	if(TestNewTim-TestOldTim>=1)//test per one seconds
				{
#if USEUNIT
					rtn=TestFrame(m_Frame,nUnitYC,p_H264Stream);
					//if(UnitTim1-UnitTim0>=nUnitTim)//test per one unit
					//{
					//	UnitSampling(nUnitYC,naParRes);
					//	memset(nUnitYC,0,sizeof(nUnitYC));
					//	resUnitTim=1;
					//}


					if(TotalFrame%nUnitFrameNum==0 && TotalFrame>0)//test per one unit
					{
						UnitSampling(nUnitYC,naParRes,p_H264Stream->SrcUrl);
						memset(nUnitYC,0,sizeof(nUnitYC));
						resUnitTim=1;
					}
#else
					rtn=TestFrame(m_Frame,naParRes,p_H264Stream);
#endif

					if(rtn==-1){
						resetAll=1;
						m_Log.Add("%s:[parTask]:TestFrame return -1",p_H264Stream->SrcUrl);
						continue;
					}
					resTestOldTim=1;
					TotalFrame++;
				}
			}
			//end test a frame

			//Analysis ParRes
			time(&newTim);
			partTim=(1+coeInd%3)*intervalTime/3;//3 part count and save pic
			if(newTim-oldTim>=partTim)//3 part count and save pic
			{
				if(ParseDebug==0)
				{
					if(IsSavePicture)
					{
						memset(PathName,0,sizeof(PathName));
						if(PRINT){
						//	TRACE("[parTask]:%s before GetPictPath\n",p_H264Stream->SrcUrl);
						}
						rtn=GetPictPath(PathName,p_H264Stream,(1+coeInd%3));
						if(PRINT){
						//	TRACE("[parTask]:%s after GetPictPath,rtn:%d\n",p_H264Stream->SrcUrl,rtn);
						}
						if(rtn==-1){
							resetAll=1;
							m_Log.Add("%s:[parTask]:GetPictPath return -1",p_H264Stream->SrcUrl);
							continue;
						}
						if(PRINT){
						//	TRACE("[parTask]:%s before SavePicture\n",p_H264Stream->SrcUrl);
						}
						rtn=SavePicture(m_Frame,PathName);
						if(PRINT){
						//	TRACE("[parTask]:%s after SavePicture,rtn:%d\n",p_H264Stream->SrcUrl,rtn);
						}
						if(rtn==-1){
							resetAll=1;
							m_Log.Add("%s:[parTask]:SavePicture return -1",p_H264Stream->SrcUrl);
							continue;
						}
					}//end if(IsSavePicture)
				}
				if(PRINT){
				//	TRACE("[parseTask]:%s before ParFrame\n",p_H264Stream->SrcUrl);
				}
#if USEUNIT
				//count YiChang with XiShu fDetectRes+=naParRes
				ParFrame(naParRes,fDetectRes,1,p_H264Stream);
#else
				//count YiChang with XiShu fDetectRes+=naParRes*coe[]
				ParFrame(naParRes,fDetectRes,coe[coeInd%3],p_H264Stream->SrcUrl);
#endif			
				//end count YiChang with XiShu fDetectRes+=naParRes*coe[]
				if(PRINT){
				//	TRACE("[parseTask]:%s after ParFrame\n",p_H264Stream->SrcUrl);
				}

				memset(naParRes,0,sizeof(naParRes));
				coeInd++;

				partPicSav=1;
			}//end if(newTim-oldTim>=(1+coeInd%3)*intervalTime/3)
			else{//
				partPicSav=0;
			}

			//one cycle detect over
			time(&newTim);

			if(newTim-oldTim>=intervalTime && partPicSav==1)
			{
				if(PRINT){
					TRACE("[parseTask]:%s newTim-oldTim>=intervalTime\n",p_H264Stream->SrcUrl);
				}
				//stopTime
				memset(tmpTim,0,sizeof(tmpTim));
				CurTim(tmpTim);
				p_H264Stream->StpTim.Format("%s",tmpTim);
				//end stopTime
				if(PRINT){
					TRACE("[parseTask]:%s after StpTim.Format\n",p_H264Stream->SrcUrl);
				}
				//save detect result to chDetRes[] such as "000100010",from fDetectRes[] to chDetRes[]

#if USEUNIT
			//	YChCount=(nUnitNum)*(YCPercent*0.01);

				YChCount=(TotalFrame/nUnitFrameNum)*0.4;
#else
				YChCount=(TotalFrame/3)*(YCPercent*0.01);
#endif
				for(i=0;i<YiChNum;i++)
				{
					if(fDetectRes[i]>YChCount){//YChCount=(25*intervalTime/3)*(YCPercent*0.01);
						//if total 300,then max =10+20+70=100
						chDetRes[YiChNum-1-i]='1';
					}else{
						chDetRes[YiChNum-1-i]='0';
					}
				}
				m_Log.Add("[parTask][%s][thisIndex:%d]:\nchDetRes[GW%f,SS%f,ZS%f,DJ%f,LD%f,MH%f,ZD%f,HP%f],\nTotalFrame[%d],YChCount[%f]",p_H264Stream->SrcUrl,p_H264Stream->thisIndex,fDetectRes[7],fDetectRes[6],fDetectRes[5],fDetectRes[4],fDetectRes[3],fDetectRes[2],fDetectRes[1],fDetectRes[0],TotalFrame,YChCount);
				TRACE("[parTask][%s][thisIndex:%d]:\nchDetRes[GW%f,SS%f,ZS%f,DJ%f,LD%f,MH%f,ZD%f,HP%f],\nTotalFrame[%d],YChCount[%f]",p_H264Stream->SrcUrl,p_H264Stream->thisIndex,fDetectRes[7],fDetectRes[6],fDetectRes[5],fDetectRes[4],fDetectRes[3],fDetectRes[2],fDetectRes[1],fDetectRes[0],TotalFrame,YChCount);
				//modify 0616 
				//if(p_H264Stream->StreamOff==1){//'0' means online
				//	chDetRes[YiChNum]='1';
				//}else{
				//	chDetRes[YiChNum]='0';
				//}
				chDetRes[YiChNum]='0';

				if(PRINT){
					TRACE("[parseTask]:%s after fDetectRes->chDetRes\n",p_H264Stream->SrcUrl);
				}
				//end save detect result
				memset(fDetectRes,0,sizeof(fDetectRes));


				if(ParseDebug==0)
				{
					//delete picture
					if(IsSavePicture)
					{
						if(PRINT){
						//	TRACE("[parseTask]:%s before DeletePic\n",p_H264Stream->SrcUrl);
						}
						rtn=DeletePic(PathName,chDetRes,p_H264Stream);
						if(PRINT){
						//	TRACE("[parseTask]:%s after DeletePic,rtn:%d\n",p_H264Stream->SrcUrl,rtn);
						}
						if(rtn==-1){
							m_Log.Add("%s:[parTask]:DeletePic return -1",p_H264Stream->SrcUrl);
						}
					}
					//end delete
				}
				if(p_H264Stream->p_EncodeArray.GetSize()>=25*2)
				{
					resetAll=1;
					m_Log.Add("%s:[parseTask]:p_H264Stream->p_EncodeArray.GetSize()>=25*60",p_H264Stream->SrcNam);
					continue;
				}
				if(PRINT){
				//	TRACE("[parseTask]:%s before new MyAvFrame\n",p_H264Stream->SrcUrl);				
				}
				//from chDetRes[] to m_tmp->p_vidDet
				MyAvFrame *m_tmp=new MyAvFrame(7,1);
				if (NULL == m_tmp || NULL == m_tmp->p_data || NULL==m_tmp->p_Ydata || NULL==m_tmp->pSenDat)
				{
					m_Log.Add("%s:[parTask]:NULL == m_tmp || NULL == m_tmp->p_data",p_H264Stream->SrcUrl);
					resetAll=1;
					continue;
				}
				if(PRINT){
				//	TRACE("[parTask]:%s after new MyAvFrame\n",p_H264Stream->SrcUrl);
				}
				memcpy(m_tmp->p_vidDet,chDetRes,sizeof(chDetRes));
				//end from chDetRes[] to m_tmp->p_vidDet

				memset(chDetRes,0,sizeof(chDetRes));

				//form m_tmp->pSenDat
				if(PRINT){
				//	TRACE("[parseTask]:%s before FormSenDat,rtn:%d\n",p_H264Stream->SrcUrl,rtn);
				}
				rtn=FormSenDat(m_tmp,p_H264Stream);//rtn==0
				if(PRINT){
				//	TRACE("[parseTask]:%s after FormSenDat,rtn:%d\n",p_H264Stream->SrcUrl,rtn);
				}
				if(rtn==-1){
					m_Log.Add("[parseTask]:FormSenDat ret -1\n");
					resetAll=1;
					continue;
				}
				//end form m_tmp->pSenDat

				if(noSend==0)
				{
					if(p_H264Stream->p_EncodeArray.GetSize()<3)
					{
						p_H264Stream->m_streamlock.Lock();
						p_H264Stream->p_EncodeArray.Add(m_tmp);
						p_H264Stream->m_streamlock.Unlock();
					}else{
						TRACE("[parseTask]:%s:p_H264Stream->p_EncodeArray.GetSize()>=3\n",p_H264Stream->SrcUrl);
					}
				}
				if(PRINT){
					TRACE("[parseTask]:%s:resetAll=1\n",p_H264Stream->SrcUrl);
				}
				resetAll=1;

				p_H264Stream->parTskEnd=1;

			}//end if(newTim-oldTim>=intervalTime)

			if (m_Frame!=NULL)
			{
				delete m_Frame;
				m_Frame=NULL;
			}
		}//end if (m_Frame!=NULL)
		else
		{//m_Frame==NULL
			time(&NodatTim1);//updata nodata new time
			if(NodatTim1-NodatTim0>=intervalTime)//server on but url not online
			{

				if(requireList==0)
				{
					if(PRINT){
						TRACE("[parseTask]:%s before SendStreamOff\n",p_H264Stream->SrcUrl);
					}
					if(ParseDebug==0)
					{
						rtn=SendStreamOff(p_H264Stream);
					}
					if(PRINT){
						TRACE("[parseTask]:%s after SendStreamOff\n",p_H264Stream->SrcUrl);
					}
				}			
				resetAll=1;
				p_H264Stream->parTskEnd=1;
				if(PRINT){
					TRACE("[parseTask]:set p_H264Stream->TaskEnd==1 and break,%s\n",p_H264Stream->SrcUrl);
				}
				m_Log.Add("[parseTask]:%s over %d time cannot get sendData\n",p_H264Stream->SrcUrl,NodatTim1-NodatTim0);
			}//end nowTim-oldTim>=totalTim
		}//end m_Frame==NULL
	}//end while(1)

leave:
	if (m_Frame!=NULL)
	{
		delete m_Frame;
		m_Frame=NULL;
	}
#if 1
	while(1)
	{
		m_Frame=NULL;//宽，高，I帧等
		p_H264Stream->m_streamlock.Lock();
		if (p_H264Stream->p_Array.GetSize()>0)//模板CArry有数据
		{
			m_Frame=p_H264Stream->p_Array.ElementAt(0);//从模板CArray<MyAvFrame*,MyAvFrame*>获取数据
			p_H264Stream->p_Array.RemoveAt(0);//获取完删除
		}
		p_H264Stream->m_streamlock.Unlock();

		if (m_Frame!=NULL)
		{
			delete m_Frame;
			m_Frame=NULL;
		}else
			break;
	}

#endif

	HRESULT  hr=S_OK;
	_endthreadex(hr);

	return hr;
}
/*************************************************************************************
函数名称：parseHttpHead
函数描述: read httpHead Content-Length
函数调用：recvHttpHead
全局变量：
参数1:httpHead content
参数2:save Content-Length
返 回 值：0 as suc;-1 as fail
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18    V1.0        jicheng         创建
**************************************************************************************/

int parseHttpHead(char *const buf,int *pLen,CH264Stream* p_H264Stream)
{
	int rtn = 0;
	char *pkey;
	pkey = strstr(buf,"Content-Length:");
	if(PRINT){
	//	TRACE("[parseHttpHead]:%s pkey:%s\n",p_H264Stream->SrcUrl,pkey);
	}
	if(pkey != NULL)
	{
		*pLen = atoi(pkey+strlen("Content-Length:"));
		if(PRINT){
		//	TRACE("[parseHttpHead]:%s *pLen:%d\n",p_H264Stream->SrcUrl,*pLen);
		}
	}
	else
	{
		*pLen = 0;
		return -1;
	}
	return rtn;
}
/*************************************************************************************
函数名称：recvHttpHead
函数描述: read httpHead
函数调用：xmlrequire
全局变量：
参数1:socket
参数2:save Content-Length
返 回 值：0 as suc;-1 as fail
创建时间：2013-12-18
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-18    V1.0        jicheng         创建
**************************************************************************************/

static int recvHttpHead(int s,char *pBuf,int maxLen,int *pLen,CH264Stream* p_H264Stream)
{
	int tmpLen = 0;
	int rtn = 0;
	char *pKeyBuf;

	while(1)
	{
		//	rtn = recv(s,(char*)(pBuf+tmpLen),1,0);
		rtn=NetRead(s, (uint8_t *)(pBuf+tmpLen), 1, 1 );
		if(rtn <= 0)
		{
			//	TRACE("ERROR: recvHttpHead->readDataFrmCommW error recv[%d]\n",tmpLen);
			return -1;
		}
		tmpLen += rtn;
		if(tmpLen >= (maxLen))
		{
			pBuf[maxLen-1] = 0; 
			//	TRACE("ERROR: the http head is lenger than %d [%s]\n",maxLen,pBuf);
			return -1;
		}
		pKeyBuf = strstr(pBuf, "\r\n\r\n");//从字符串str1中查找是否有字符串str2
		if(pKeyBuf != NULL)
		{
			parseHttpHead(pBuf,pLen,p_H264Stream);//pLen=Content-Length:data
			return 0;
		}
	}
	return rtn;
}
/*************************************************************************************
函数名称：CheckUrlOnline
函数描述: check if url is online
函数调用：recHttpBody
全局变量：
参数1:list of online video
参数2:pointer of CH264Stream
返 回 值: 
创建时间：2014-2-25
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-2-25      V1.0        jicheng         创建
**************************************************************************************/

int CheckUrlOnline(char*pBuf,CH264Stream*p_H264Stream)
{	
	char*ret=NULL;
	char serch[128];
	if(PRINT){
	//	TRACE("[CheckUrlOnline]:%s----1\n",p_H264Stream->SrcUrl);
	}
	sprintf(serch,"<name>%s</name>",p_H264Stream->SrcNam);
	//<name>nlljk_142_38</name>
	if(PRINT){
		//	fflush(stdout);
	//	TRACE("[CheckUrlOnline]:%s-before strstr,serch:%s\npBuf:%s\n",p_H264Stream->SrcUrl,serch,pBuf);
	}
	ret=strstr(pBuf,serch);
	if(PRINT){
	//	TRACE("[CheckUrlOnline]:%s-after strstr,ret:%p\n",p_H264Stream->SrcUrl,ret);
	}
	if(ret==NULL)//not online
	{
		return -1;
	}else{//online
		return 1;
	}
}
/*************************************************************************************
函数名称：recHttpBody
函数描述: read httpBody
函数调用：xmlrequire
全局变量：
参数1:socket
参数2:save httpBody
参数3:lenth of httpBody
参数4:pointer of CH264Stream
返 回 值：1 as streamOnline;-1 as neterr;-2 as streamOff;
创建时间：2014-2-25
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-2-25    V1.0        jicheng         创建
**************************************************************************************/

int recHttpBody(int s,char*pBuf,int maxLen,CH264Stream* p_H264Stream)
{
	int tmpLen = 0;
	int rtn = 0;
	char *pKeyBuf;
	if(PRINT){
	//	TRACE("[recHttpBody]:%s before while-tmpLen<maxLen \n",p_H264Stream->SrcUrl);
	}
	while(tmpLen<maxLen)
	{
		//	rtn = recv(s,(char*)(pBuf+tmpLen),maxLen-tmpLen,0);
		rtn=NetRead(s, (uint8_t *)(pBuf+tmpLen), maxLen-tmpLen, 1 );
		if(rtn <= 0)
		{
			return -1;
		}
#if 0
		if(PRINT)
		{
			TRACE("INFO:[%d]:\n",rtn);
			for(int i = 0;i<rtn;i++)
			{
				//	TRACE("[%d]%02X ",tmpLen+i,*(unsigned char*)(pBuf+tmpLen+i));
				TRACE("[%d]%c ",tmpLen+i,(pBuf+tmpLen+i));
			}
			TRACE("[%d]\n",rtn);
		}
#endif
		tmpLen += rtn;
	}

	if(PRINT)
	{
		//m_Log.Add("$$$$pBuf:%s\n",pBuf);
		//TRACE("INFO:[%d]:[%s]\n",maxLen,pBuf);
	//	TRACE("[recHttpBody]:%s before CheckUrlOnline \n",p_H264Stream->SrcUrl);
	}
	rtn=CheckUrlOnline(pBuf,p_H264Stream);
	if(PRINT){
	//	TRACE("[recHttpBody]:%s after CheckUrlOnline \n",p_H264Stream->SrcUrl);
	}
	if(rtn==1)//url online
	{
		return 1;
	}else{
		return -2;
	}
}

int recReplyBody(int s,char*pBuf,int maxLen,CH264Stream* p_H264Stream)
{
	int tmpLen = 0;
	int rtn = 0;
	char *pKeyBuf;

	while(tmpLen<maxLen)
	{
		//	rtn = recv(s,(char*)(pBuf+tmpLen),maxLen-tmpLen,0);
		rtn=NetRead(s, (uint8_t *)(pBuf+tmpLen), maxLen-tmpLen, 1 );
		if(rtn <= 0)
		{
			return -1;
		}
		tmpLen += rtn;
	}
	return 0;
}
int StreamType(void* H264Stream)
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	char XieYi[32];
	int TypeLen=4;//strlen("http");
	memset(XieYi,0,sizeof(XieYi));
	memcpy(XieYi,p_H264Stream->SrcUrl.GetString(),TypeLen);
	if(0 == strcmp(XieYi,"http"))
	{
		return 1;
	}
	if(0 == strcmp(XieYi,"rtmp"))
	{
		return 0;
	}
	return 0;//default not require video list


}

/*************************************************************************************
函数名称：xmlrequire
函数描述: test state of service
函数调用：serverState
全局变量：
参数1:pointer to CH264Stream
返 回 值:0 as service ok and stream on;-1 as normal err;-2 as server off;-3 as server on but stream off
创建时间：2014-1-14
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2014-1-14      V1.0        jicheng         创建
**************************************************************************************/

int xmlrequire(void* H264Stream)
{
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	char*HttpData=NULL;

	int port1=1234;
	int port2=p_H264Stream->SrcPort;
	int requireList=0;

	int rtn = -1,tmpLen=0,xmlLen=0;
	int fdClient1=-1;
	int fdClient2=-1;
	int dataLen=0;

	char tmpBuf[512*4];

	memset(tmpBuf,0,sizeof(tmpBuf));

	requireList=StreamType(p_H264Stream);//1 as http require, 0 as rtmp not require
	if(requireList==1)//http req
	{
		if(PRINT){
		//	TRACE("[xmlrequire]:%s before ConnectTCP%s:%d\n",p_H264Stream->SrcUrl,p_H264Stream->SrcIp,port1);
		}
		fdClient1=ConnectTCP(p_H264Stream->SrcIp,port1);//ip:1234,fdClient1
		if(PRINT){
		//	TRACE("[xmlrequire]:%s after ConnectTCP%s:%d\n",p_H264Stream->SrcUrl,p_H264Stream->SrcIp,port1);
		}
		if(fdClient1<0){//server off -2
			if(PRINT){
				TRACE("[xmlrequire]:%s ConnectTCP(%s,%d) fail\n",p_H264Stream->SrcUrl,p_H264Stream->SrcIp,port1);
			}
			p_H264Stream->ServerOff=1;
			p_H264Stream->StreamOff=1;
			//	NetClose(fdClient1);
			return -2;
		}
		if(PRINT){
		//	TRACE("[xmlrequire]:%s before ConnectTCP%s:%d\n",p_H264Stream->SrcUrl,p_H264Stream->SrcIp,port2);
		}
		fdClient2 = ConnectTCP(p_H264Stream->SrcIp,port2);//ip:port,fdClient2
		if(PRINT){
		//	TRACE("[xmlrequire]:%s after ConnectTCP%s:%d\n",p_H264Stream->SrcUrl,p_H264Stream->SrcIp,port2);
		}
		if (fdClient2 < 0){//server on but stream off -3
			if(PRINT){
				TRACE("[xmlrequire]:%s ConnectTCP(%s,%d) fail\n",p_H264Stream->SrcUrl,p_H264Stream->SrcIp,port2);
			}
			p_H264Stream->ServerOff=0;
			p_H264Stream->StreamOff=1;
			//	NetClose(fdClient2);
			return -3;
		}
		NetClose(fdClient2);
		fdClient2=-1;

		tmpLen += sprintf(tmpBuf + tmpLen, "GET /mctl?dev=live&act=get&type=all HTTP/1.0\r\n");
		tmpLen += sprintf(tmpBuf + tmpLen, "Host: %s:%d\r\n", p_H264Stream->SrcIp,port1);
		tmpLen += sprintf(tmpBuf + tmpLen, "content-type:application/xml;charset=utf-8\r\n");
		tmpLen += sprintf(tmpBuf + tmpLen, "\r\n");
		if(PRINT){
		//	TRACE("[xmlrequire]:%s before NetWrite tmpLen:%d\n",p_H264Stream->SrcUrl,tmpLen);
		}
		rtn = NetWrite(fdClient1, (const uint8_t *)tmpBuf, tmpLen);
		if(PRINT){
		//	TRACE("[xmlrequire]:%s after NetWrite rtn:%d\n",p_H264Stream->SrcUrl,rtn);
		}
		if (rtn < 0){//server off -2
			if(PRINT){
				TRACE("[xmlrequire]:%s NetWrite fail\n",p_H264Stream->SrcUrl);
			}
			p_H264Stream->ServerOff=1;
			p_H264Stream->StreamOff=1;
			NetClose(fdClient1);
			fdClient1=-1;
			return -2;			
		}

		memset(tmpBuf,0,sizeof(tmpBuf));
		if(PRINT){
		//	TRACE("[xmlrequire]:%s before recvHttpHead\n",p_H264Stream->SrcUrl);
		}
		rtn = recvHttpHead(fdClient1,tmpBuf,sizeof(tmpBuf),&dataLen,p_H264Stream);//not close  fdClient1
		if(PRINT){// xmlrequire
		//	TRACE("[xmlrequire]:%s after recvHttpHead,dataLen:%d\n",p_H264Stream->SrcUrl,dataLen);
		}
		if(rtn == -1)//server off -2
		{
			if(PRINT){
				TRACE("[xmlrequire]:%s recvHttpHead fail\n",p_H264Stream->SrcUrl);
			}
			p_H264Stream->ServerOff=1;
			p_H264Stream->StreamOff=1;
			return -2;
		}
		HttpData=(char*)malloc(dataLen+1);
		memset(HttpData,0,dataLen+1);
		if(HttpData==NULL){
			m_Log.Add("[xmlrequire]:%s malloc fail\n",p_H264Stream->SrcUrl);
			TRACE("[xmlrequire]:%s malloc fail\n",p_H264Stream->SrcUrl);
			NetClose(fdClient1);
			fdClient1=-1;
			return -1;
		}
		if(PRINT){
		//	TRACE("[xmlrequire]:%s before recHttpBody\n",p_H264Stream->SrcUrl);
		}
		rtn=recHttpBody(fdClient1,HttpData,dataLen,p_H264Stream);
		if(PRINT){
		//	TRACE("[xmlrequire]:%s after recHttpBody\n",p_H264Stream->SrcUrl);
		}
		NetClose(fdClient1);
		fdClient1=-1;
		if(HttpData){
			free(HttpData);
			HttpData=NULL;
		}
		if(rtn!=1){//server on but url not online
			if(rtn==-1){
				if(PRINT){
					TRACE("[xmlrequire]:%s recHttpBody fail\n",p_H264Stream->SrcUrl);
				}
			}else{
				if(PRINT){
					TRACE("[xmlrequire]:%s stream off\n",p_H264Stream->SrcUrl);
				}
			}
			p_H264Stream->ServerOff=0;
			p_H264Stream->StreamOff=1;
			return -3;
		}
		//server on and url  online
		p_H264Stream->ServerOff=0;
		p_H264Stream->StreamOff=0;
		return 0;
	}else
	{
		//-2 as server off;0 as server and stream online
		if(PRINT){
		//	TRACE("[xmlrequire]:%s before ConnectTCP%s:%d\n",p_H264Stream->SrcUrl,p_H264Stream->SrcIp,port1);
		}
		fdClient1=ConnectTCP(p_H264Stream->SrcIp,port1);//ip:1234,fdClient1
		if(PRINT){
		//	TRACE("[xmlrequire]:%s after ConnectTCP%s:%d,fdClient1:%d\n",p_H264Stream->SrcUrl,p_H264Stream->SrcIp,port1,fdClient1);
		}
		if(fdClient1<0){//server off -2
			p_H264Stream->ServerOff=1;
			p_H264Stream->StreamOff=1;
			//	NetClose(fdClient1);
			return -2;
		}
		NetClose(fdClient1);
		fdClient1=-1;
		if(PRINT){
		//	TRACE("[xmlrequire]:%s before ConnectTCP%s:%d\n",p_H264Stream->SrcUrl,p_H264Stream->SrcIp,port2);
		}
		p_H264Stream->ServerOff=0;
		p_H264Stream->StreamOff=0;
		return 0;
	}
} 

/*************************************************************************************
函数名称：ForSerSen
函数描述: format service online state send data
函数调用：serverState
全局变量：
参数1:point to class which has SrcIp,SrcNam,service state..
参数2:buff to save send data
返 回 值：0
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

int ForSerSen(CH264Stream* p_H264Stream,char*dst)
{
	int len=0;
	char s_body[1024];
	char s_content[1024];
	char guid[64];
	memset(s_body,0,sizeof(s_body));
	memset(s_content,0,sizeof(s_content));
	memset(guid,0,sizeof(guid));

	if(p_H264Stream->IsTask==0){
		CreateGuid(guid);
		p_H264Stream->ServerCmdId=guid;
	}else{

	}

	sprintf(s_body,
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
		"<root>\r\n"
		"   <DetectResult>\r\n"		
		"		<CmdID>%s</CmdID>\r\n"
		"		<Direct>%d</Direct>\r\n"
		"		<StartTime>%s</StartTime>\r\n"
		"		<StopTime>%s</StopTime>\r\n"
		"  		<StreamSrvIP>%s</StreamSrvIP>\r\n"
		"  		<StreamName>%s</StreamName>\r\n"
		"  		<Analyze>%d</Analyze>\r\n"
		"	</DetectResult>\r\n"
		"</root>\r\n"
		,p_H264Stream->ServerCmdId
		,p_H264Stream->direct
		,p_H264Stream->SerStaTim
		,p_H264Stream->SerStpTim
		,p_H264Stream->SrcIp
		,p_H264Stream->SrcNam
		,p_H264Stream->ServerOff
		);

	sprintf(s_content,
		"POST http://%s:%d%s HTTP/1.1\r\n"
		"Host:%s\r\n"
		"Content-Type: application/xml;charset=utf-8\r\n"
		"Content-Length:%d\r\n"
		"Connection: closed\r\n\r\n"
		"%s"
		,p_H264Stream->DstIp
		,p_H264Stream->DstPort
		,ServiceStatePath
		,p_H264Stream->DstIp
		,strlen(s_body)
		,s_body);
	memcpy(dst,s_content,strlen(s_content));
	return 0;
}
/*************************************************************************************
函数名称：serverState
函数描述: send service state thread
函数调用：CH264Stream::StartPush(int Width, int Height)
全局变量：
参数1:point to CH264Stream
返 回 值：
创建时间：2013-12-6
修改日期        版本号     修改人           修改内容
-------------------------------------------------
2013-12-6    V1.0        jicheng         创建
**************************************************************************************/

UINT _stdcall serverState(void* H264Stream)
{
	//pointer
	CH264Stream* p_H264Stream=(CH264Stream*)H264Stream;
	//constant
	int intervalTime=TestCycle;//detect cycle
	int SleepTimUs=1000*5*4;//sleep time
	int YiChNum=8;//8 numbers of YiChang
	int NoServerTsk=noServerstate;//0,normal;1,test
	//variable
	int rtn=0;
	int Tim=0;

	int Updata=1;//updata server start time
	int reSenTim=0;//counts of send fail
	int SlepTim=0;//sleeping time

	time_t oldTim=0;//reversed
	time_t newTim=0;//reversed

	char SenDat[1024];//send data
	char tmpTim[64];
	char PathName[64];//log path

	memset(PathName,0,sizeof(PathName));
	memset(SenDat,0,sizeof(SenDat));
	memset(tmpTim,0,sizeof(tmpTim));

	time(&oldTim);

	while(1)
	{
		if(NoServerTsk==1)
		{
			p_H264Stream->serStaEnd=1;
			Sleep(1000);
			continue;
		}
		Sleep(SleepTimUs);
		if(Tim%(2)==0)
		{
			if(PRINT){
				TRACE("[serverState%s][thisIndex:%d]:%s is running, parTskEnd:%d,serStaEnd:%d,senTskEnd:%d\n",
					p_H264Stream->SrcNam,p_H264Stream->thisIndex,p_H264Stream->SrcUrl,p_H264Stream->parTskEnd,p_H264Stream->serStaEnd,p_H264Stream->senTskEnd);
			}
		}
		Tim++;

		if(p_H264Stream->parTskEnd==1)
		{
			p_H264Stream->serStaEnd=1;
		}
		if(p_H264Stream->serStaEnd==1)
		{
			Updata=1;
			if(SlepTim%5==0){
				TRACE("[serverState%s]:%s is sleeping,parTskEnd:%d,serStaEnd:%d,senTskEnd:%d\n",
					p_H264Stream->SrcNam,p_H264Stream->SrcUrl,p_H264Stream->parTskEnd,p_H264Stream->serStaEnd,p_H264Stream->							senTskEnd);
				SlepTim=0;
			}
			continue;
		}
		time(&newTim);

		if(Main->g_Stop == 1){
			TRACE("[serverState]:Main->g_Stop == 1\n");
			break;
		}

		//startTime	
		if(Updata==1){
			memset(tmpTim,0,sizeof(tmpTim));
			CurTim(tmpTim);
			p_H264Stream->SerStaTim.Format("%s",tmpTim);
			Updata=0;
		}
		if(PRINT){// xmlrequire
		//	TRACE("[serverState]:%s before xmlrequire\n",p_H264Stream->SrcUrl);
		}
		rtn=xmlrequire(p_H264Stream);
		if(PRINT){
		//	TRACE("[serverState]:%s after xmlrequire,rtn=%d\n",p_H264Stream->SrcUrl,rtn);
		}
		if(rtn==0){//server on and stream on
			Updata=1;
			continue;
		}
		if(rtn==-1){//public exception
			Updata=1;
			continue;
		}
		if(rtn==-2)	//service off 
		{	
			//stopTime
			memset(tmpTim,0,sizeof(tmpTim));
			CurTim(tmpTim);
			p_H264Stream->SerStpTim.Format("%s",tmpTim);

			ForSerSen(p_H264Stream,SenDat);
			rtn=-1;
			while(rtn<0)
			{
				if(reSenTim>=3){
					break;
				}
				rtn=SenSerState(p_H264Stream,SenDat);
				if(rtn<0){
					Sleep(1000);
					reSenTim++;
					if(IsSaveLog==1)
					{
						if(LogSaveType==0){//save as date
							memset(PathName,0,sizeof(PathName));
							GetPathName(PathName);
							m_Loglock.Lock();
							SenLog.InitLog(PathName);
							SenLog.Add("[SenSerState]:sendfail:%s\n%s\nlen=%d\n",
								p_H264Stream->SrcNam,SenDat,strlen(SenDat));
							m_Loglock.Unlock();
						}else if(LogSaveType==1){//save less than 8M
							m_Log.Add("[SenSerState]:sendfail:%s\n%s\nlen=%d\n",
								p_H264Stream->SrcNam,SenDat,strlen(SenDat));
						}
					}//end savelog
				}//end if rtn<0
			}//end while(rtn<0)

			reSenTim=0;
			memset(SenDat,0,sizeof(SenDat));
		//	Updata=1;

			p_H264Stream->serStaEnd=1;
			//add
			p_H264Stream->parTskEnd=1;
			SendStreamOff(p_H264Stream);
			//end add
			if(PRINT){
				TRACE("[SenSerState]:set p_H264Stream->TaskEnd==1,%s\n",p_H264Stream->SrcUrl);
			}
		}//end if(rtn==-2)
		if(rtn==-3)	//service on but url off
		{
			//add
			m_Log.Add("[serverState]:%s not on video list\n",p_H264Stream->SrcUrl);
			p_H264Stream->serStaEnd=1;
			p_H264Stream->parTskEnd=1;
			SendStreamOff(p_H264Stream);
			//end add
		}
	}//end while(1)

leave:
	HRESULT  hr=S_OK;
	_endthreadex(hr);

	return hr;

}

CH264Stream::CH264Stream(void)
{
	//	h_encoder=NULL;

	// 	CString str="";
	// 	str.Format("%d",this);
	m_hSendThread = NULL;
	m_hXmlThread = NULL;
	m_hParseThread = NULL;
	m_hSerStateThread=NULL;
	m_hTaskThread=NULL;


	memset(&EncodeStreamParams, 0, sizeof(EncodeStreamParams));
	memset(&Params, 0, sizeof(Params));

	//memset(&StreamParams, 0, sizeof(StreamParams));
	h_stream=NULL;
	b_start=false;
	SrcUrl="";
	SrcIp="";
	SrcPort=0;
	SrcNam="";

	DstIp="";
	DstPort=0;

	b_stop = -1;
	StreamOff=0;
	ServerOff=0;
	StaTim="";
	StpTim="";
	SerStaTim="";
	SerStpTim="";
	StreamOffStaTim="";
	StreamOffStpTim="";
	CmdId="";
	ServerCmdId="";
	StreamOffCmdId="";

	IsTask=0;
	TaskEnd=0;
	RePullByNetTsk=0;
	NetTskCmId="";
	direct=0;


	memset(&YCH,0,sizeof(YCH));

	parTskEnd=0;
	senTskEnd=0;
	serStaEnd=0;
}

CH264Stream::~CH264Stream(void)
{

	if(NULL != m_hParseThread)
	{
		WaitForSingleObject(m_hParseThread,INFINITE);
		CloseHandle( m_hParseThread );
	}

	if(NULL != m_hSendThread)
	{
		WaitForSingleObject(m_hSendThread,INFINITE);
		CloseHandle( m_hSendThread );
	}
	if(NULL != m_hXmlThread)
	{
		WaitForSingleObject(m_hXmlThread,INFINITE);
		CloseHandle( m_hXmlThread );
	}
	if(NULL != m_hSerStateThread)
	{
		WaitForSingleObject(m_hSerStateThread,INFINITE);
		CloseHandle( m_hSerStateThread );
	}

}


int CH264Stream::StartPush(int Width, int Height)
{
	unsigned int threadid;

	m_hParseThread=(HANDLE)_beginthreadex(NULL,0,parseTask,(void *)this,CREATE_SUSPENDED,&threadid);
	SetThreadPriority(m_hParseThread, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(m_hParseThread);

	m_hSendThread=(HANDLE)_beginthreadex(NULL,0,sendTask,(void *)this,CREATE_SUSPENDED,&threadid);
	SetThreadPriority(m_hSendThread, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(m_hSendThread);

	m_hSerStateThread=(HANDLE)_beginthreadex(NULL,0,serverState,(void *)this,CREATE_SUSPENDED,&threadid);
	SetThreadPriority(m_hSerStateThread, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(m_hSerStateThread);

	return 0;
}


