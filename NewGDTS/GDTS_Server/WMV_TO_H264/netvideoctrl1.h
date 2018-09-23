#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CNetvideoctrl1 包装类

class CNetvideoctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CNetvideoctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xE24D8362, 0x622, 0x4D15, { 0x94, 0xAA, 0x2E, 0x83, 0xA6, 0x61, 0x6E, 0xAC } };
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

// _DNetVideo

// Functions
//

	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void CloseVideo()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long SetDeviceInfo(long nType, LPCTSTR szName, LPCTSTR szIP, long nPort, LPCTSTR szProxyIP, long nProxyPort, LPCTSTR szUserName, LPCTSTR szPassword)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nType, szName, szIP, nPort, szProxyIP, nProxyPort, szUserName, szPassword);
		return result;
	}
	long OpenVideo(long nChannel)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nChannel);
		return result;
	}
	long Save(long bSave)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bSave);
		return result;
	}
	long IOControl(long nNumber, long nParam1, long nParam2)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nNumber, nParam1, nParam2);
		return result;
	}
	long SearchClose()
	{
		long result;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long ShowMenu(LPCTSTR szMenu)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szMenu);
		return result;
	}
	long DownloadFile(LPCTSTR szSourceFile, LPCTSTR szSaveFile, long nStart, long nEnd)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szSourceFile, szSaveFile, nStart, nEnd);
		return result;
	}
	long GetDownloadLength()
	{
		long result;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long OpenDevice()
	{
		long result;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long CloseDevice()
	{
		long result;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetDeviceStatus()
	{
		long result;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long AddDevice(long nType, LPCTSTR szName, LPCTSTR szIP, long nPort, LPCTSTR szProxyIP, long nProxyPort, LPCTSTR szUserName, LPCTSTR szPassword)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nType, szName, szIP, nPort, szProxyIP, nProxyPort, szUserName, szPassword);
		return result;
	}
	long DeleteDevice(LPCTSTR szIP)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szIP);
		return result;
	}
	void TalkStop()
	{
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long CapturePic(LPCTSTR szFileName, long nParam)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szFileName, nParam);
		return result;
	}
	long SetEnableSound(long bEnable)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bEnable);
		return result;
	}
	long TalkStart(long bBroadcast)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bBroadcast);
		return result;
	}
	long YuTaiControl(long nCommand, long nParam)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nCommand, nParam);
		return result;
	}
	long SetRegIP(LPCTSTR RegIP)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms, RegIP);
		return result;
	}
	long GetConfig(LPCTSTR szFileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szFileName);
		return result;
	}
	long SetConfig(LPCTSTR szFileName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szFileName);
		return result;
	}
	long PlayOpen(LPCTSTR szFileName, long nType, long dwParam)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szFileName, nType, dwParam);
		return result;
	}
	long PlayStop()
	{
		long result;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long PlaySetSpeed(long nSpeed)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nSpeed);
		return result;
	}
	long SearchMonth(long nType, long nYear, long nMonth)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nType, nYear, nMonth);
		return result;
	}
	long SearchHaveRecord(long nDay)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nDay);
		return result;
	}
	CString SearchGetDeviceList()
	{
		CString result;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long SearchDay(long nType, LPCTSTR szDeviceName, long nYear, long nMonth, long nDay, long nChannel)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nType, szDeviceName, nYear, nMonth, nDay, nChannel);
		return result;
	}
	CString SearchGetFileInfo()
	{
		CString result;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long SetVideoBuffer(long nBuffer)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nBuffer);
		return result;
	}
	long GetDownloadTotalLength()
	{
		long result;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SearchLogMonth(long nYear, long nMonth)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nYear, nMonth);
		return result;
	}
	long SearchLogDay(long nYear, long nMonth, long nDay)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nYear, nMonth, nDay);
		return result;
	}
	CString SearchGetLogInfo()
	{
		CString result;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long SetSubCode()
	{
		long result;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long ServerConfig(LPCTSTR szIP)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szIP);
		return result;
	}
	long PlayGetCurTime()
	{
		long result;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SetVolume(long nVal)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nVal);
		return result;
	}
	CString GetConfig2(long dwCommand, long nChannel)
	{
		CString result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, dwCommand, nChannel);
		return result;
	}
	long SetConfig2(long dwCommand, long nChannel, LPCTSTR szConfig)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwCommand, nChannel, szConfig);
		return result;
	}
	long SelectFilePlay()
	{
		long result;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long PlayGetTotalTime()
	{
		long result;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long PlaySeek(long nTime)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nTime);
		return result;
	}
	void SetReconnect(long bReconnect)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bReconnect);
	}
	long FullScreen()
	{
		long result;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long UpGrade()
	{
		long result;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SetDeviceInfo2(long DeviceType, LPCTSTR DeviceID, LPCTSTR TransmitIP)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DeviceType, DeviceID, TransmitIP);
		return result;
	}
	long GetUpGradeState()
	{
		long result;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SetProxyIP(LPCTSTR ProxyIP)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_I4, (void*)&result, parms, ProxyIP);
		return result;
	}
	long SetDecodeMode(long mode)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_I4, (void*)&result, parms, mode);
		return result;
	}
	long SetDataRate(long mode)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, mode);
		return result;
	}
	long Record(LPCTSTR path)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, path);
		return result;
	}
	void StopRecord()
	{
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString GetVersion()
	{
		CString result;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long RecordDisk(LPCTSTR DiskName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, DiskName);
		return result;
	}
	CString GetRecordDisk()
	{
		CString result;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetReconnect2(long bReconnect, long nRecCount)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bReconnect, nRecCount);
	}
	long OpenSound(long nTalkType, long nChannel)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x41, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nTalkType, nChannel);
		return result;
	}
	long DownloadFileStop()
	{
		long result;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SearchTime(LPCTSTR szDeviceName, long nYear, long nMonth, long nDay, long nStartHour, long nStartMinute, long nStartSecond, long nEndHour, long nEndMinute, long nEndSecond, long nType, long nChannel)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szDeviceName, nYear, nMonth, nDay, nStartHour, nStartMinute, nStartSecond, nEndHour, nEndMinute, nEndSecond, nType, nChannel);
		return result;
	}
	long GetGps(long * bEnalbe, long * timer)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x44, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bEnalbe, timer);
		return result;
	}
	long SetGps(long bEnalbe, long nTime)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x45, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bEnalbe, nTime);
		return result;
	}
	long SendMessageInfo(long lType, long lHandAck, long lShowPoint, long lShowTime, LPCTSTR szContent)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x47, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lType, lHandAck, lShowPoint, lShowTime, szContent);
		return result;
	}
	CString GetAllHardDiskInfo()
	{
		CString result;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long PlayPause(long bPause)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x49, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bPause);
		return result;
	}
	long SetAllHardDiskInfo(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lpAppName, lpKeyName, lpString);
		return result;
	}
	long SetDeviceInfo3(LPCTSTR szVehicleNo, long nVehicleColor, LPCTSTR szIP, long nPort, LPCTSTR szProxyIP, long nProxyPort, LPCTSTR szUserName, LPCTSTR szPassword, LPCTSTR szLoginName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szVehicleNo, nVehicleColor, szIP, nPort, szProxyIP, nProxyPort, szUserName, szPassword, szLoginName);
		return result;
	}
	long SetDeviceTime()
	{
		long result;
		InvokeHelper(0x4c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SetStreamInfo()
	{
		long result;
		InvokeHelper(0x4d, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long SaveElecFence(LPCTSTR szPath, LPCTSTR szData)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x4e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szPath, szData);
		return result;
	}
	long DownloadFileConvert(LPCTSTR szSourceFile, LPCTSTR szSaveFile, long nStart, long nEnd, long bRemote, long nCovavi)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x4f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szSourceFile, szSaveFile, nStart, nEnd, bRemote, nCovavi);
		return result;
	}
	void SetUser(LPUNKNOWN pUser, long bOpenDecoder)
	{
		static BYTE parms[] = VTS_UNKNOWN VTS_I4 ;
		InvokeHelper(0x50, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pUser, bOpenDecoder);
	}
	long SetRegIPAndPort(LPCTSTR szRetIP, long nRegPort)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x51, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szRetIP, nRegPort);
		return result;
	}
	CString GetRegIpAndPort()
	{
		CString result;
		InvokeHelper(0x52, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void Init()
	{
		InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//



};
