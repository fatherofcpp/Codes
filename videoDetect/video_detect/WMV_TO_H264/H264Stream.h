#pragma once
#include "inc.h"
#include "CriticalSection.h"

#include "rtmp.h"



class CH264Stream
{
public:
	CH264Stream(void);
	~CH264Stream(void);
	
//	BYTE* encodebuff;//reversed
	CCriticalSection m_streamlock;//mutex lock when write or read data to or from shared dynamic array
	CCriticalSection m_streamcloselock;//mutex lock when close video stream
//	HANDLE h_encoder;
//	HANDLE h_stream;

	//待编码数据缓冲取
	CArray<MyAvFrame*,MyAvFrame*> p_Array;	//video data

	CArray<MyAvFrame*,MyAvFrame*> p_EncodeArray; //send xml data

//上海三利
	//CNetvideoctrl1 *pNetVideo;
	enum CONVERT_DEV_TYPE m_devType;//device type

	int b_stop;//标识该路是否需要停止推送数据，是从占用网络带宽的角度考虑。
				//-1 表示第一次启动，0表示该路已经启用，1表示该路持续一段时间没有观看，需要停止

	HANDLE m_hSendThread;//send thread
	HANDLE m_hXmlThread;//reversed
	HANDLE m_hParseThread;//parse thread
	HANDLE m_hSerStateThread;//service state thread
	HANDLE m_hTaskThread;//reversed

	bool first;//reversed
	bool b_start;//whether to detect ,if false then can detect ,if ture then is being detected

	int thisIndex;//index of this way

	//WMV拉流
	HANDLE h_stream;//handle of open video stream 
	//
	CString SrcUrl;//url of video stream
	CString SrcIp;//ip of video stream
	CString SrcNam;//publish name of video stream
	int SrcPort;//port of video stream

	
	CString	DstIp;//dest service ip to report to
	int		DstPort;//dest service port
	char	StreamOff;//video stream pulled by this way is offline
	char	ServerOff;//video stream pulled by this way is online

	CString StaTim;//video detect start time
	CString StpTim;//video detect stop time
	CString SerStaTim;//service which has video stream detect start time
	CString SerStpTim;//service which has video stream detect stop time
	CString StreamOffStaTim;//stream off detect start time
	CString StreamOffStpTim;//stream off detect stop time
	CString CmdId;//flag of each report of video detect
	CString ServerCmdId;//flag of each report of service which contains video stream detect
	CString StreamOffCmdId;//flag of each report of stream off

	char direct;//report type:0 auto,1 task
	int IsTask;//report of this way is task report
	int TaskEnd;//end thread of all this way
	int RePullByNetTsk;//report of this way is required by netTsk 
	CString NetTskCmId;//flag of report of this way which is required by netTsk 

	_YiChang YCH;//netTsk post information

	int parTskEnd;//parse thread is end
	int senTskEnd;//send thread is end
	int serStaEnd;//service state thread is end

private:

	ENCODERSTREAMPARAMS EncodeStreamParams;//reversed
	STREAMINFO_OUTPUT Params;//reversed

public:

	int StartPush(int Width, int Height);//begin detect
	//int StartGet(void);
};
