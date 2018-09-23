#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。

/////////////////////////////////////////////////////////////////////////////
// CDvsstreamplayerctrl1 包装类

class CDvsstreamplayerctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDvsstreamplayerctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x19c65fda, 0xd8ba, 0x4db8, { 0xa3, 0x96, 0xd5, 0x8d, 0x29, 0x40, 0x58, 0x42 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:


// 操作
public:

// _DDvsStreamPlayer

// Functions
//

	long Cleanup()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetErrorInfo()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPicture(LPCTSTR PicturePath)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, PicturePath);
		return result;
	}
	long VoiceComOff()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long VoiceComOn(long ServerIP, long ServerPort)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ServerIP, ServerPort);
		return result;
	}
	long Init()
	{
		long result;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SPlayerCtrl(long index, long cmd, long cmdPara1, double cmdPara2 = 0)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_R8;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, cmd, cmdPara1, cmdPara2);
		return result;
	}
	long SPlayerSetPMode(long mode)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, mode);
		return result;
	}
	long SPlayerSetchannelNum(long channelNum, long arg)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, channelNum, arg);
		return result;
	}
	long Stop(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index);
		return result;
	}
	long RequestNPB(long index, unsigned char * url, ULONGLONG start_time, ULONGLONG end_time)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_UI8 VTS_UI8;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, url, start_time, end_time);
		return result;
	}
	long Play(long index, long Web_EcIp, long Web_DataPort, long Mode, long Web_StreamType, long Web_StreamForamtResoultion, LPCTSTR LpbFile, long Web_McFlag, long Web_McIp, long Web_McPort, long Web_LocalIp)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, Web_EcIp, Web_DataPort, Mode, Web_StreamType, Web_StreamForamtResoultion, LpbFile, Web_McFlag, Web_McIp, Web_McPort, Web_LocalIp);
		return result;
	}
	long GetLPBFileHighPos()
	{
		long result;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetLPBFileLowPos()
	{
		long result;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetMaskRect(long RectIndex, long PointType)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, RectIndex, PointType);
		return result;
	}
	long GetMotionRect(long RectIndex, long PointType)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, RectIndex, PointType);
		return result;
	}
	long GetMotionRectValid(long RectIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, RectIndex);
		return result;
	}
	long GetSensitivityArea(long LineNum)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms, LineNum);
		return result;
	}
	long GetVideoImageHeight(long PlayerChannel)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, PlayerChannel);
		return result;
	}
	long GetVideoImageWidth(long PlayerChannel)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms, PlayerChannel);
		return result;
	}
	long SetMaskRect(long RectIndex, long left, long top, long right, long bottom)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms, RectIndex, left, top, right, bottom);
		return result;
	}
	long SetMotionRect(long RectIndex, long left, long top, long right, long bottom)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms, RectIndex, left, top, right, bottom);
		return result;
	}
	long SetMotionRectValid(long RectIndex, long valid)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, RectIndex, valid);
		return result;
	}
	long SetRTDisplayType(long type)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms, type);
		return result;
	}
	long SetSensitivityArea(LPCTSTR pSelectedArea)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pSelectedArea);
		return result;
	}
	long SetHighlightChannel(long x, long y, long * channel)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, x, y, channel);
		return result;
	}
	long SetOSDType(long type)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, type);
		return result;
	}
	long SetOSDArea(long x, long y, long height, long width, long index)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, x, y, height, width, index);
		return result;
	}
	long GetOSDArea(long * x, long * y, long index)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_PI4 VTS_I4;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, x, y, index);
		return result;
	}
	long SetMNInfo(long p1, long p2)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, p1, p2);
		return result;
	}
	long SetSensitivityAreaNew(unsigned char * SelectedArea, long Num)
	{
		long result;
		static BYTE parms[] = VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SelectedArea, Num);
		return result;
	}
	long GetSensitivityAreaNew(unsigned char * SelectedArea, long * Num)
	{
		long result;
		static BYTE parms[] = VTS_PUI1 VTS_PI4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, SelectedArea, Num);
		return result;
	}
	long SetPlayBackMode(long mode)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms, mode);
		return result;
	}
	long RegGetStreamDataCB(long pCB, long pUserData)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pCB, pUserData);
		return result;
	}
	long StartGetStreamData(long index, long mask)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, mask);
		return result;
	}
	long StopGetStreamData(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index);
		return result;
	}
	long StartDownLoad(long index, BYTE *url, ULONGLONG start_time, ULONGLONG end_time, long speed, LPCTSTR file_path, long file_type)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_UI8 VTS_UI8 VTS_I4 VTS_BSTR VTS_I4;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index, url, start_time, end_time, speed, file_path, file_type);
		return result;
	}
	long StopDownLoad(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index);
		return result;
	}
	long long QureyDownLoadProgress(long index)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, parms, index);
		return result;
	}
	long QureyFullScreen()
	{
		long result;
		InvokeHelper(0x28,DISPATCH_METHOD,VT_I4,(void*)&result,NULL);
		return result;
	}
	long SetDejitterTime(long index, unsigned short time)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_UI2;
		InvokeHelper(0x29,DISPATCH_METHOD,VT_I4,(void*)&result, parms, index, time);
		return result;
	}
	long SetMultiIndexFullScreen(long ChannelNum)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x2a,DISPATCH_METHOD,VT_I4,(void*)&result, parms, ChannelNum);
		return result;
	}
	long SetFocusChannelUnselect(long bUnselect)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x2b,DISPATCH_METHOD,VT_I4,(void*)&result, parms, bUnselect);
		return result;
	}
	long SetDownloadMaxFileSize(ULONGLONG MaxSize)
	{
		long result;
		static BYTE parms[] = VTS_UI8;
		InvokeHelper(0x2c,DISPATCH_METHOD,VT_I4,(void*)&result, parms, MaxSize);
		return result;
	}
	long StartDownLoadTask(BYTE *url, ULONGLONG start_time, ULONGLONG end_time, long speed, LPCTSTR file_path, long file_type, long handle_ret)
	{
		long result;
		static BYTE parms[] = VTS_PUI1 VTS_UI8 VTS_UI8 VTS_I4 VTS_BSTR VTS_I4 VTS_I4;
		InvokeHelper(0x2d,DISPATCH_METHOD,VT_I4,(void*)&result, parms, url, start_time, end_time, speed, file_path, file_type, handle_ret);
		return result;
	}
	long StopDownLoadTask(long hDwonLoad)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x2e,DISPATCH_METHOD,VT_I4,(void*)&result, parms, hDwonLoad);
		return result;
	}
	long QueryDownLoadTaskProgress(long hDwonLoad)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x2f,DISPATCH_METHOD,VT_I4,(void*)&result, parms, hDwonLoad);
		return result;
	}
	long RegDownLoadTaskFinishFuc(long hDwonLoad, long InfoCB, long pUserData)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4;
		InvokeHelper(0x30,DISPATCH_METHOD,VT_I4,(void*)&result, parms, hDwonLoad, InfoCB, pUserData);
		return result;
	}
	long SetDownLoadTaskMaxFileSize(ULONGLONG MaxSize)
	{
		long result;
		static BYTE parms[] = VTS_UI8;
		InvokeHelper(0x31,DISPATCH_METHOD,VT_I4,(void*)&result, parms, MaxSize);
		return result;
	}
	long RegDownLoadTaskDataOutCB(long hDwonLoad, long DataOutCB, long mask, long pUserData)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4;
		InvokeHelper(0x32,DISPATCH_METHOD,VT_I4,(void*)&result, parms, hDwonLoad, DataOutCB, mask, pUserData);
		return result;
	}
	long QueryDownLoadTaskFps(long hDwonLoad)
	{
		long result;
		static BYTE parms[] = VTS_I4;
		InvokeHelper(0x33,DISPATCH_METHOD,VT_I4,(void*)&result, parms, hDwonLoad);
		return result;
	}
// Properties
//
};
