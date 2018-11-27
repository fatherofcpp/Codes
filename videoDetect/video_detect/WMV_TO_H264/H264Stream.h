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

	//���������ݻ���ȡ
	CArray<MyAvFrame*,MyAvFrame*> p_Array;	//video data

	CArray<MyAvFrame*,MyAvFrame*> p_EncodeArray; //send xml data

//�Ϻ�����
	//CNetvideoctrl1 *pNetVideo;
	enum CONVERT_DEV_TYPE m_devType;//device type

	int b_stop;//��ʶ��·�Ƿ���Ҫֹͣ�������ݣ��Ǵ�ռ���������ĽǶȿ��ǡ�
				//-1 ��ʾ��һ��������0��ʾ��·�Ѿ����ã�1��ʾ��·����һ��ʱ��û�йۿ�����Ҫֹͣ

	HANDLE m_hSendThread;//send thread
	HANDLE m_hXmlThread;//reversed
	HANDLE m_hParseThread;//parse thread
	HANDLE m_hSerStateThread;//service state thread
	HANDLE m_hTaskThread;//reversed

	bool first;//reversed
	bool b_start;//whether to detect ,if false then can detect ,if ture then is being detected

	int thisIndex;//index of this way

	//WMV����
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
