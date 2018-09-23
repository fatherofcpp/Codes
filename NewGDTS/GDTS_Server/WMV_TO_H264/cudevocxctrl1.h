#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CCudevocxctrl1 包装类

class CCudevocxctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CCudevocxctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x731B048F, 0x7419, 0x41FB, { 0x88, 0xC7, 0xF7, 0x4A, 0x85, 0x2C, 0xF0, 0x9A } };
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

// _DCudevocx

// Functions
//

	BOOL ConnectToServer(LPCTSTR strIpAddr, long dwPort, LPCTSTR strUserName, LPCTSTR strUserPwd)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strIpAddr, dwPort, strUserName, strUserPwd);
		return result;
	}
	void SetProxyNatInfo(BOOL bNat, BOOL bProxy, LPCTSTR strProxyIpAddr, long dwProxyPort, LPCTSTR strUserName, LPCTSTR strPwd)
	{
		static BYTE parms[] = VTS_BOOL VTS_BOOL VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bNat, bProxy, strProxyIpAddr, dwProxyPort, strUserName, strPwd);
	}
	void GetVideoSourceInfo(LPCTSTR strGepID, short wChn)
	{
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strGepID, wChn);
	}
	BOOL StartPlay(LPCTSTR strGepID, short wChn)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strGepID, wChn);
		return result;
	}
	BOOL StopPlay()
	{
		BOOL result;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetMoveSpeed(short wSpeed)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, wSpeed);
	}
	void SendPtzCmd(short wCommandType)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, wCommandType);
	}
	void DisConnectToServer()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL StartPlayByWndIndex(short wWndIndex, LPCTSTR strGepID, short wChn)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_BSTR VTS_I2 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wWndIndex, strGepID, wChn);
		return result;
	}
	BOOL StopPlayByWndIndex(short nWndIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nWndIndex);
		return result;
	}
	BOOL SetPlayWindowStyle(BOOL bAverageShow, short wShowNumbers)
	{
		BOOL result;
		static BYTE parms[] = VTS_BOOL VTS_I2 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bAverageShow, wShowNumbers);
		return result;
	}
	BOOL GetVideoSourceByWndIndex(short wWndIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wWndIndex);
		return result;
	}
	BOOL LoadPreset(short wPreset)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wPreset);
		return result;
	}
	BOOL SavePreset(short wPreset)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wPreset);
		return result;
	}
	BOOL SwitchVideoSource(short wVideoSourceId)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wVideoSourceId);
		return result;
	}
	BOOL CapturePic(LPCTSTR strPicFileFullName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strPicFileFullName);
		return result;
	}
	BOOL CapturePicWithType(LPCTSTR strPicSavePath, short wPicType)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strPicSavePath, wPicType);
		return result;
	}
	BOOL InitLocalRec(short wMaxRecNum)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wMaxRecNum);
		return result;
	}
	BOOL SetLocalRecSavePath(LPCTSTR strPath)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strPath);
		return result;
	}
	BOOL StartLocalRec(LPCTSTR strSaveFileName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strSaveFileName);
		return result;
	}
	BOOL StopLocalRec()
	{
		BOOL result;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL StartLocalRecByWndIndex(short wWndIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wWndIndex);
		return result;
	}
	BOOL StopLocalRecByWndIndex(short wWndIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wWndIndex);
		return result;
	}
	BOOL IsLocalRecoding()
	{
		BOOL result;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL IsLocalRecodingByWndIndex(short wWndIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wWndIndex);
		return result;
	}
	BOOL IsPlaying()
	{
		BOOL result;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void RecordPUQuerySetChannel(LPCTSTR strGepID, short wChn)
	{
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strGepID, wChn);
	}
	BOOL RecordPUQuerySetTime(long dwStartTime, long dwEndTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwStartTime, dwEndTime);
		return result;
	}
	BOOL RecordPUQueryItem(short nQueryPage)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nQueryPage);
		return result;
	}
	void RecordPUQueryClear()
	{
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void RecordNRUQuerySetChannel(LPCTSTR strGepID, short wChn)
	{
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strGepID, wChn);
	}
	BOOL RecordNRUQueryItem(short nQueryPage)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nQueryPage);
		return result;
	}
	void RecordNRUQueryClear()
	{
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL RecordPlaySetWnd(long hWnd)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, hWnd);
		return result;
	}
	BOOL RecordPlayPause()
	{
		BOOL result;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL RecordPlayStop()
	{
		BOOL result;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL RecordPlayFast()
	{
		BOOL result;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL RecordPlaySlow()
	{
		BOOL result;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL RecordPlaySeekTo(short nProgressPercent)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nProgressPercent);
		return result;
	}
	BOOL RecordPlayNormal()
	{
		BOOL result;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL RecordPlayRestart()
	{
		BOOL result;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void RecordPlaySetAudioVolume(BOOL bHasSound, short nVolume)
	{
		static BYTE parms[] = VTS_BOOL VTS_I2 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bHasSound, nVolume);
	}
	BOOL RecordStartPURec(LPCTSTR strGepID, short wChn)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strGepID, wChn);
		return result;
	}
	BOOL RecordStopPURec(LPCTSTR strGepID, short wChn)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strGepID, wChn);
		return result;
	}
	BOOL RecordStartNRURec(LPCTSTR strGepID, short wChn)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strGepID, wChn);
		return result;
	}
	BOOL RecordStopNRURec(LPCTSTR strGepID, short wChn)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strGepID, wChn);
		return result;
	}
	BOOL RecordPlayStart(long dwFileID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwFileID);
		return result;
	}
	BOOL RecordFileStopDownload()
	{
		BOOL result;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL RecordFileStartDownload(long dwFileID, LPCTSTR szPathName, LPCTSTR szFileName)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwFileID, szPathName, szFileName);
		return result;
	}
	BOOL RecordNRUQuerySetTime(long dwStartTime, long dwEndTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwStartTime, dwEndTime);
		return result;
	}
	void SetUseDefaultPlayWnd(BOOL bUseDefault)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bUseDefault);
	}
	void SetVideoPlayWnd(short nIndex, long hWnd)
	{
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nIndex, hWnd);
	}
	BOOL SetCurPlayWndIndex(short nPlayWndIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPlayWndIndex);
		return result;
	}
	void SetNatInterval(long nNatInterval)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nNatInterval);
	}
	long GetCurUserPrivilege()
	{
		long result;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPUPrivilege(LPCTSTR strGepID)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID);
		return result;
	}
	long GetAudioCallInfo()
	{
		long result;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL StartAudioCall()
	{
		BOOL result;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL StopAudioCall()
	{
		BOOL result;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL RecordFileDelete(long dwFileID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwFileID);
		return result;
	}
	void SetNetResendRegroup(BOOL bNetRSRG)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bNetRSRG);
	}
	BOOL LogOfUserQuerySetInfo(long dwUseID, short nQueryType, long dwStartTime, long dwEndTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwUseID, nQueryType, dwStartTime, dwEndTime);
		return result;
	}
	BOOL LogOfDeviceQuerySetInfo(LPCTSTR strGepID, short nQueryType, long dwStartTime, long dwEndTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strGepID, nQueryType, dwStartTime, dwEndTime);
		return result;
	}
	BOOL LogOfAlarmQuerySetInfo(LPCTSTR strGepID, short nQueryType, long dwStartTime, long dwEndTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strGepID, nQueryType, dwStartTime, dwEndTime);
		return result;
	}
	BOOL LogOfUserQueryTerm(long nPage)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x41, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPage);
		return result;
	}
	BOOL LogOfDeviceQueryTerm(long nPage)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPage);
		return result;
	}
	BOOL LogOfAlarmQueryTerm(long nPage)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nPage);
		return result;
	}
	BOOL LogOfUserDeleteByID(long dwLogID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x44, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwLogID);
		return result;
	}
	BOOL LogOfDeviceDeleteByID(long dwLogID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x45, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwLogID);
		return result;
	}
	BOOL LogOfAlarmDeleteByID(long dwLogID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x46, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwLogID);
		return result;
	}
	BOOL SetCameraType(short nCameraType)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x47, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nCameraType);
		return result;
	}
	BOOL MonitorPresetGetAll()
	{
		BOOL result;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL MonitorPresetLoadByID(long dwMonitorPresetID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x49, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwMonitorPresetID);
		return result;
	}
	BOOL MonitorPresetSaveWithName(LPCTSTR szMonitorPresetName, short wWndStyle)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szMonitorPresetName, wWndStyle);
		return result;
	}
	BOOL MonitorPresetDeleteByID(long dwMonitorPresetID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwMonitorPresetID);
		return result;
	}
	BOOL DeviceGroupCreate(LPCTSTR szDeviceGroupName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szDeviceGroupName);
		return result;
	}
	BOOL DeviceGroupDeleteByID(long dwDeviceGroupID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x4d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwDeviceGroupID);
		return result;
	}
	BOOL DeviceGroupAddDevice(long dwDeviceGroupID, LPCTSTR strDeviceGepID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x4e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwDeviceGroupID, strDeviceGepID);
		return result;
	}
	BOOL DeviceGroupDeleteDevice(long dwDeviceGroupID, LPCTSTR strDeviceGepID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x4f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwDeviceGroupID, strDeviceGepID);
		return result;
	}
	BOOL UserGroupCreate(LPCTSTR szGroupName, long dwGroupPrivilege, BOOL bUniformPrivilege)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BOOL ;
		InvokeHelper(0x50, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szGroupName, dwGroupPrivilege, bUniformPrivilege);
		return result;
	}
	BOOL UserGroupDeleteByID(long dwUserGroupID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x51, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwUserGroupID);
		return result;
	}
	BOOL UserCreateSetInfoStep1(LPCTSTR szUserName, BOOL bUserSex, LPCTSTR szUserLoginPWD)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL VTS_BSTR ;
		InvokeHelper(0x52, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szUserName, bUserSex, szUserLoginPWD);
		return result;
	}
	BOOL UserCreateSetInfoStep2(short nUserType, long dwUserPrivilege)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0x53, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nUserType, dwUserPrivilege);
		return result;
	}
	BOOL UserCreate()
	{
		BOOL result;
		InvokeHelper(0x54, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL UserDeletByID(long dwUserID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x55, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwUserID);
		return result;
	}
	BOOL UserGroupAddUser(long dwUserGroupID, long dwUserID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x56, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwUserGroupID, dwUserID);
		return result;
	}
	BOOL UserGroupDeleteUser(long dwUserGroupID, long dwUserID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x57, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwUserGroupID, dwUserID);
		return result;
	}
	BOOL UserBeAssignedDevice(long dwUserID, LPCTSTR strDeviceGepID, long dwPrivilege)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I4 ;
		InvokeHelper(0x58, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwUserID, strDeviceGepID, dwPrivilege);
		return result;
	}
	BOOL UserBeReclaimedDevice(long dwUserID, LPCTSTR strDeviceGepID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x59, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwUserID, strDeviceGepID);
		return result;
	}
	BOOL DeviceGroupGetAll()
	{
		BOOL result;
		InvokeHelper(0x5a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL DeviceGetAll()
	{
		BOOL result;
		InvokeHelper(0x5b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL UserGroupGetAll()
	{
		BOOL result;
		InvokeHelper(0x5c, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL UserGetAll()
	{
		BOOL result;
		InvokeHelper(0x5d, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL UserModifyLoginPWD(LPCTSTR szUserLoginPWD)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szUserLoginPWD);
		return result;
	}
	BOOL UserModifyPrivilege(short dwUserID, short dwUserPrivilege)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x5f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwUserID, dwUserPrivilege);
		return result;
	}
	short GetCurUserType()
	{
		short result;
		InvokeHelper(0x60, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	long GetDeviceStatus(LPCTSTR strGepID, short wChn)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x61, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChn);
		return result;
	}
	void SetAudioVolume(short nAudioVolumn)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x62, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nAudioVolumn);
	}
	BOOL RecordPlayCapturePic(LPCTSTR strPicFileFullName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x63, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strPicFileFullName);
		return result;
	}
	BOOL RecordPlaySingleFrame()
	{
		BOOL result;
		InvokeHelper(0x64, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL RecordPlayLocalFile(LPCTSTR szLocalRecFileName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x65, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szLocalRecFileName);
		return result;
	}
	BOOL TVWallDecoderGetAll()
	{
		BOOL result;
		InvokeHelper(0x66, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL TVWallGetAll()
	{
		BOOL result;
		InvokeHelper(0x67, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL TVWallBegainCreate(LPCTSTR szTVWallName, short wTVWallStyle)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x68, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szTVWallName, wTVWallStyle);
		return result;
	}
	long TVWallBindDecoder(short wTVWallWndIndex, LPCTSTR strDecoderGepID, short wChnIndex)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_BSTR VTS_I2 ;
		InvokeHelper(0x69, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wTVWallWndIndex, strDecoderGepID, wChnIndex);
		return result;
	}
	BOOL TVWallCancelCrete()
	{
		BOOL result;
		InvokeHelper(0x6a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL TVWallFinishCreate()
	{
		BOOL result;
		InvokeHelper(0x6b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL TVWallDelete(long dwTVWallID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwTVWallID);
		return result;
	}
	BOOL TVWallPresetGetAll()
	{
		BOOL result;
		InvokeHelper(0x6d, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL TVWallPresetSave(LPCTSTR szTVWallPreserName, short wTVWallPresetType)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x6e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, szTVWallPreserName, wTVWallPresetType);
		return result;
	}
	long TVWallPresetBindEncoder(short wTVWndIndex, short wDivisionIndex, LPCTSTR strEncoderGepID, short wEncoderChn)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_BSTR VTS_I2 ;
		InvokeHelper(0x6f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wTVWndIndex, wDivisionIndex, strEncoderGepID, wEncoderChn);
		return result;
	}
	long TVWallPresetBindEncByDec(LPCTSTR strDecoderGepID, short wDecoderChn, LPCTSTR strEncoderGepID, short wEncoderChn)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_BSTR VTS_I2 ;
		InvokeHelper(0x70, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strDecoderGepID, wDecoderChn, strEncoderGepID, wEncoderChn);
		return result;
	}
	BOOL TVWallPresetLoadTVWall(long dwTVWallID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x71, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwTVWallID);
		return result;
	}
	BOOL TVWallPresetDelete(long dwTVWallPresetID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x72, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwTVWallPresetID);
		return result;
	}
	BOOL TVWallPresetUndoEncoder(long dwTVWallPresetID, short wTVWndIndex, short wDivisionIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I2 ;
		InvokeHelper(0x73, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwTVWallPresetID, wTVWndIndex, wDivisionIndex);
		return result;
	}
	BOOL TVWallPresetReleaseEncoder(LPCTSTR strDecoderGepID, short wDecoderChn)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x74, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strDecoderGepID, wDecoderChn);
		return result;
	}
	BOOL TVWallPresetModify(long dwTVWallPresetID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x75, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwTVWallPresetID);
		return result;
	}
	BOOL TVWallPresetLoadByID(long dwTVWallPresetID)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x76, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwTVWallPresetID);
		return result;
	}
	long ConfigSetUseCustom(long dwCustomUI)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x77, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwCustomUI);
		return result;
	}
	long PUGetParallelPortNum(LPCTSTR strGepID, BOOL bInputOrOutput)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL ;
		InvokeHelper(0x78, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, bInputOrOutput);
		return result;
	}
	long PUConfigBegin(LPCTSTR strGepID)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x79, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID);
		return result;
	}
	long PUConfigGetItemInfo(short wChnIndex, short wCfgItem)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x7a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wChnIndex, wCfgItem);
		return result;
	}
	long PUConfigBannerSetBaseInfo(BOOL bBannerEnable, short wBannerBindParallelPort)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I2 ;
		InvokeHelper(0x7b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bBannerEnable, wBannerBindParallelPort);
		return result;
	}
	long PUConfigBannerSetTitle(LPCTSTR strBannerTitle)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x7c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strBannerTitle);
		return result;
	}
	long PUConfigBannerSetPosInfo(long lBannerXPos, long lBannerYPos, long lBannerHeight)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lBannerXPos, lBannerYPos, lBannerHeight);
		return result;
	}
	long PUConfigBannerSetScrollInfo(long wlcrollMode, long lScrollSpeed, long lScrollTimes, long lScrollPauseTime)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wlcrollMode, lScrollSpeed, lScrollTimes, lScrollPauseTime);
		return result;
	}
	long PUConfigBannerSetBKGInfo(long lBKGTransparency, long lFontTransparency, long lBKGColorIndex, long lFontColorIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x7f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lBKGTransparency, lFontTransparency, lBKGColorIndex, lFontColorIndex);
		return result;
	}
	long PUConfigEnd(long lSaveType)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x80, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lSaveType);
		return result;
	}
	long PUGetAvailableFontColorNum()
	{
		long result;
		InvokeHelper(0x81, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long PUGetFontColorByIndex(long lFontColorIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x82, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lFontColorIndex);
		return result;
	}
	long PUConfigSetCurChn(short wChnIndex)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x83, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wChnIndex);
		return result;
	}
	long PUConfigBaseSetInfo(BOOL bDirectEnable, short wDirectNum)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I2 ;
		InvokeHelper(0x84, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bDirectEnable, wDirectNum);
		return result;
	}
	long PUConfigCOMSetInfo(long lComID, short wComType, long dwBaundRate)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 ;
		InvokeHelper(0x85, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lComID, wComType, dwBaundRate);
		return result;
	}
	long PUConfigMatrixSetBaseInfo(BOOL bHasMatrix, short wMatrixProtocol)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I2 ;
		InvokeHelper(0x86, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bHasMatrix, wMatrixProtocol);
		return result;
	}
	long PUConfigMatrixSetIOPortInfo(short wInputPortNum, short wInputStartPort, short wOutputPortNum, short wOutputStartPort)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 VTS_I2 ;
		InvokeHelper(0x87, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wInputPortNum, wInputStartPort, wOutputPortNum, wOutputStartPort);
		return result;
	}
	long PUConfigMatrixGetVideoSrcInfo(short wVideoSrcID)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x88, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wVideoSrcID);
		return result;
	}
	long PUConfigMatrixSetVideoSrcInfo(short wVideoSrcID, short wMatrixOutputPort)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x89, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wVideoSrcID, wMatrixOutputPort);
		return result;
	}
	long PUConfigAlarmOutSetInfo(long lOutputChn, LPCTSTR strOutputAlias)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x8a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lOutputChn, strOutputAlias);
		return result;
	}
	long PUConfigRecSaveSetBaseInfo(long dwRestDiskSpace, short wOverStrategy, long dwSmallFileRecTime)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_I4 ;
		InvokeHelper(0x8b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwRestDiskSpace, wOverStrategy, dwSmallFileRecTime);
		return result;
	}
	long PUConfigRecSaveAddPath(long dwPathID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwPathID);
		return result;
	}
	long PUConfigRecSaveDelPath(long dwPathID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwPathID);
		return result;
	}
	long PUConfigEncodeGetVideoEncType(short wChnIndex)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x8e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wChnIndex);
		return result;
	}
	long PUConfigEncodeGetVideoRS(short wChnIndex, long lVideoType)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0x8f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wChnIndex, lVideoType);
		return result;
	}
	long PUConfigEncodeSetVideoInfo(long lVidType, long lVidResolution, short wVidQuality)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 ;
		InvokeHelper(0x90, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lVidType, lVidResolution, wVidQuality);
		return result;
	}
	long PUConfigEncodeSetFrameInfo(long lFrameRate, long lBitRate, long lKeyFrame)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x91, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lFrameRate, lBitRate, lKeyFrame);
		return result;
	}
	long PUConfigEncodeSetAdvance(short wBright, short wContrast, short wSaturation)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 ;
		InvokeHelper(0x92, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wBright, wContrast, wSaturation);
		return result;
	}
	long PUConfigVideoGetInfo(short wVideoSrcID)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x93, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wVideoSrcID);
		return result;
	}
	long PUConfigVideoSetBaseInfo(short wVideoSrcID, BOOL bEnableVideoSrc, BOOL bPMParserProtocol, LPCTSTR szVideoSrcAlias)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_BOOL VTS_BOOL VTS_BSTR ;
		InvokeHelper(0x94, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wVideoSrcID, bEnableVideoSrc, bPMParserProtocol, szVideoSrcAlias);
		return result;
	}
	long PUConfigVideoSetCameralInfo(short wVideoSrcID, short wCameralAddressCode, short wCameralType, short wCameralDirection)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 VTS_I2 ;
		InvokeHelper(0x95, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wVideoSrcID, wCameralAddressCode, wCameralType, wCameralDirection);
		return result;
	}
	long PUConfigOSDSetBaseInfo(BOOL bOSDEnable, long lLeftMargin, long lTopMargin)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x96, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bOSDEnable, lLeftMargin, lTopMargin);
		return result;
	}
	long PUConfigOSDSetFont(LPCTSTR strFontName, short wCharset, long lFontSize, long lFontBold)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x97, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strFontName, wCharset, lFontSize, lFontBold);
		return result;
	}
	long PUConfigOSDSetTitle(LPCTSTR strOSD, short lFontColorIndex)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0x98, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strOSD, lFontColorIndex);
		return result;
	}
	long PUConfigNRUSetBaseInfo(BOOL bFrameRec, BOOL bRecAudio)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_BOOL ;
		InvokeHelper(0x99, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bFrameRec, bRecAudio);
		return result;
	}
	long PUConfigNRUSetPreRecInfo(BOOL bPreRec, long lPreRecTime, long lResumeRecTime)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x9a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bPreRec, lPreRecTime, lResumeRecTime);
		return result;
	}
	long PUConfigNRUSetTimeRecInfo(BOOL bTimeRec, long lTimeRecStartTime, long lTimeRecEndTime)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x9b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bTimeRec, lTimeRecStartTime, lTimeRecEndTime);
		return result;
	}
	long PUConfigNRUSetCycRecBaseInfo(BOOL bCycRec, short wCycRecMode)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I2 ;
		InvokeHelper(0x9c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bCycRec, wCycRecMode);
		return result;
	}
	long PUConfigNRUSetCycRecPeriod(short wWeeklyDay, short wCycPeriodIndex, long lCycRecStartTime, long lCycRecEndTime)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x9d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wWeeklyDay, wCycPeriodIndex, lCycRecStartTime, lCycRecEndTime);
		return result;
	}
	long PUConfigPRSetBaseInfo(BOOL bFrameRec, BOOL bRecAudio, BOOL bAlarmRec)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_BOOL VTS_BOOL ;
		InvokeHelper(0x9e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bFrameRec, bRecAudio, bAlarmRec);
		return result;
	}
	long PUConfigPRSetPreRecInfo(BOOL bPreRec, long lPreRecTime, long lResumeRecTime)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x9f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bPreRec, lPreRecTime, lResumeRecTime);
		return result;
	}
	long PUConfigPRSetTimeRecInfo(BOOL bTimeRec, long lTimeRecStartTime, long lTimeRecEndTime)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0xa0, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bTimeRec, lTimeRecStartTime, lTimeRecEndTime);
		return result;
	}
	long PUConfigPRSetCycRecBaseInfo(BOOL bCycRec, short wCycRecMode)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I2 ;
		InvokeHelper(0xa1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bCycRec, wCycRecMode);
		return result;
	}
	long PUConfigPRSetCycRecPeriod1(short wWeeklyDay, short wCycPeriodIndex, long lCycRecStartTime, long lCycRecEndTime)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0xa2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wWeeklyDay, wCycPeriodIndex, lCycRecStartTime, lCycRecEndTime);
		return result;
	}
	long PUConfigSnapPicSetInfo(BOOL bParallePortSnap, BOOL bDetectSanp, long dwSnapPicNum, long dwSnapInterval)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0xa3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bParallePortSnap, bDetectSanp, dwSnapPicNum, dwSnapInterval);
		return result;
	}
	long PUReboot(LPCTSTR strGepID)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID);
		return result;
	}
	long PUGetVideoSourceInfo(LPCTSTR strGepID, short wChnIndex)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0xa5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChnIndex);
		return result;
	}
	long PUGetVideoSourceDetail(LPCTSTR strGepID, short wChnIndex, short wVideoSrcIndex)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_I2 ;
		InvokeHelper(0xa6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChnIndex, wVideoSrcIndex);
		return result;
	}
	long PUGetCapacityByType(LPCTSTR strGepID, short wCapaciType)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0xa7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wCapaciType);
		return result;
	}
	long PUCOMGetSupportType(LPCTSTR strGepID, long lComID)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0xa8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, lComID);
		return result;
	}
	long PUAlarmCfgBegin(LPCTSTR strGepID)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xa9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID);
		return result;
	}
	long PUAlarmCfgGetItemInfo(short wLinkageType, short wLinkageIndex)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0xaa, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wLinkageType, wLinkageIndex);
		return result;
	}
	long PUAlarmCfgSetCurModifyItem(short wLinkageType, short wLinkageIndex)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0xab, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wLinkageType, wLinkageIndex);
		return result;
	}
	long PUAlarmCfgSetInputAssociate(LPCTSTR strAlarmAlias, short wChnIndex, short wVedioSrcID, BOOL bStartSafty)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_I2 VTS_BOOL ;
		InvokeHelper(0xac, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strAlarmAlias, wChnIndex, wVedioSrcID, bStartSafty);
		return result;
	}
	long PUAlarmCfgSetDetectAssociate(LPCTSTR strAlarmAlia, BOOL bStartSafty)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL ;
		InvokeHelper(0xad, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strAlarmAlia, bStartSafty);
		return result;
	}
	long PUAlarmCfgSetLinkageTVWall(BOOL bTurnTVWall, long dwTVWallID, long dwTVWallTVIndex)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0xae, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bTurnTVWall, dwTVWallID, dwTVWallTVIndex);
		return result;
	}
	long PUAlarmCfgSetLinkageBase(BOOL bCountTime, BOOL bRecord, BOOL bPTZTrace, short wPTZPreset)
	{
		long result;
		static BYTE parms[] = VTS_BOOL VTS_BOOL VTS_BOOL VTS_I2 ;
		InvokeHelper(0xaf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bCountTime, bRecord, bPTZTrace, wPTZPreset);
		return result;
	}
	long PUAlarmCfgEnd(long lSaveType)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb0, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lSaveType);
		return result;
	}
	long TVWallPollCfgBegin(long dwTVWallID, long dwTVWndIndex, short wDivisionIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 ;
		InvokeHelper(0xb1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwTVWallID, dwTVWndIndex, wDivisionIndex);
		return result;
	}
	long TVWallPollCfgClearAll()
	{
		long result;
		InvokeHelper(0xb2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long TVWallPollCfgAddPUChn(LPCTSTR strGepID, short wChnIndex, long dwPollInterval)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_I4 ;
		InvokeHelper(0xb3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChnIndex, dwPollInterval);
		return result;
	}
	long TVWallPollCfgDelPUChn(LPCTSTR strGepID, short wChnIndex)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0xb4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChnIndex);
		return result;
	}
	long TVWallPollCfgEnd(long lSaveType)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lSaveType);
		return result;
	}
	long TVWallPollOperate(long dwTVWallID, long dwTVWndIndex, long dwDivisionIndex, short wOperateType)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I2 ;
		InvokeHelper(0xb6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwTVWallID, dwTVWndIndex, dwDivisionIndex, wOperateType);
		return result;
	}
	short VideoBrowseChangePlayWnd(short wPlayWndIndex, long hPlayWnd)
	{
		short result;
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0xb7, DISPATCH_METHOD, VT_I2, (void*)&result, parms, wPlayWndIndex, hPlayWnd);
		return result;
	}
	long DeskPollConfigBegin(short wPlayWndIndex)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xb8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wPlayWndIndex);
		return result;
	}
	long DeskPollConfigClearAll()
	{
		long result;
		InvokeHelper(0xb9, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long DeskPollConfigAddPUChn(LPCTSTR strGepID, short wChnIndex)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0xba, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChnIndex);
		return result;
	}
	long DeskPollConfigDelPUChn(LPCTSTR strGepID, short wChnIndex)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0xbb, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChnIndex);
		return result;
	}
	long DeskPollConfigInterval(long dwPollInterval)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xbc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwPollInterval);
		return result;
	}
	long DeskPollConfigEnd(long lSaveType)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xbd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lSaveType);
		return result;
	}
	long DeskPollOperate(short wPlayWndIndex, short wOperateType)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0xbe, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wPlayWndIndex, wOperateType);
		return result;
	}
	long PersonalConfig(short wCfgType, long lCfgPara)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0xbf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wCfgType, lCfgPara);
		return result;
	}
	long SystemConfigSetInfo(short wDiskFullStategy, short wLogSaveDay, short wAlarmSaveDay)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I2 ;
		InvokeHelper(0xc0, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wDiskFullStategy, wLogSaveDay, wAlarmSaveDay);
		return result;
	}
	long PUCollectDataStartStop(LPCTSTR strGepID, short wChn, BOOL bStart)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_BOOL ;
		InvokeHelper(0xc1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChn, bStart);
		return result;
	}
	long MonitorDetectBegin(short wPlayWndIndex, long hPlayWnd)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0xc2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wPlayWndIndex, hPlayWnd);
		return result;
	}
	long MonitorDetectAddArea(long dwXPos, long dwYPos, long dwWidth, long dwHeight)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xc3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwXPos, dwYPos, dwWidth, dwHeight);
		return result;
	}
	long MonitorDetectDelArea(long dwXPos, long dwYPos, long dwWidth, long dwHeight)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xc4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwXPos, dwYPos, dwWidth, dwHeight);
		return result;
	}
	long MonitorDetectAddWhole()
	{
		long result;
		InvokeHelper(0xc5, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long MonitorDetectDelWhole()
	{
		long result;
		InvokeHelper(0xc6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long MonitorDetectSetAlarmRate(short wAlarmRate, short wAlarmResumRate)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0xc7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wAlarmRate, wAlarmResumRate);
		return result;
	}
	long MonitorDetectEnd(long lSaveType)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xc8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lSaveType);
		return result;
	}
	long TVWallModifyDecoder(short wOperType, long dwTVWallID, short byTVWallWndIndex, LPCTSTR strDecGepID, short wChnIndex)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I4 VTS_I2 VTS_BSTR VTS_I2 ;
		InvokeHelper(0xc9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wOperType, dwTVWallID, byTVWallWndIndex, strDecGepID, wChnIndex);
		return result;
	}
	long SystemConfigGetInfo()
	{
		long result;
		InvokeHelper(0xca, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	BOOL RecordFileBeforeDownloadSetInfo(long dwFileID, long dwStartTime, long dwEndTime, long dwExtension)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xcb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwFileID, dwStartTime, dwEndTime, dwExtension);
		return result;
	}
	BOOL RecordPUQuerySetType(short wRecordType, long dwExtension)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0xcc, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wRecordType, dwExtension);
		return result;
	}
	BOOL RecordNRUQuerySetType(short wRecordType, BOOL bTimeAscending, long dwExtension)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 VTS_BOOL VTS_I4 ;
		InvokeHelper(0xcd, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, wRecordType, bTimeAscending, dwExtension);
		return result;
	}
	long PUSnapPicQuerySetChannel(LPCTSTR strGepID, short wChn)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0xce, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChn);
		return result;
	}
	long PUSnapPicQuerySetDetailInfo(short wSnapPicType, long dwStartTime, long dwEndTime, long dwExtension)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xcf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wSnapPicType, dwStartTime, dwEndTime, dwExtension);
		return result;
	}
	long PUSnapPicQueryItem(short nQueryPage)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xd0, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nQueryPage);
		return result;
	}
	long PUSnapPicDownLoad(long dwSnapPicID, LPCTSTR szSavePath, LPCTSTR szSaveFileName)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR ;
		InvokeHelper(0xd1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwSnapPicID, szSavePath, szSaveFileName);
		return result;
	}
	long PUSnapPicDelete(long dwSnapPicID)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwSnapPicID);
		return result;
	}
	long VideoPlayGetEncInfoByWndIndex(long lWndIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd3, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lWndIndex);
		return result;
	}
	long PUPeripherySendComData(LPCTSTR strGepID, short wChnId, short wVidSrcId, LPCTSTR strSendData)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_I2 VTS_BSTR ;
		InvokeHelper(0xd4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChnId, wVidSrcId, strSendData);
		return result;
	}
	long GetDeviceManufacture(LPCTSTR strGepID)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID);
		return result;
	}
	long ConnectThirdManufactureDevice(LPCTSTR strGepID, BOOL bConnect)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL ;
		InvokeHelper(0xd6, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, bConnect);
		return result;
	}
	long OCXOpenFile(LPCTSTR szURL, short wFileType)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 ;
		InvokeHelper(0xd7, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szURL, wFileType);
		return result;
	}
	long PersonalConfigEX(short wCfgType, long lCfgPara, LPCTSTR szConfigInfo)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I4 VTS_BSTR ;
		InvokeHelper(0xd8, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wCfgType, lCfgPara, szConfigInfo);
		return result;
	}
	long PUAlarmCfgSetLinkageEX(long dwLinkageType, long dwOperation, long dwLinkageValue, LPCTSTR szLinkageList)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0xd9, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwLinkageType, dwOperation, dwLinkageValue, szLinkageList);
		return result;
	}
	long OperateOnAudioCallVolume(short nAudioVolumn, short wOperateType)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0xda, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nAudioVolumn, wOperateType);
		return result;
	}
	BOOL UserGroupModify(long dwUGrpID, LPCTSTR szUGrpname, long dwUGrpPrivilege, BOOL bUniformPrivilege)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL ;
		InvokeHelper(0xdb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dwUGrpID, szUGrpname, dwUGrpPrivilege, bUniformPrivilege);
		return result;
	}
	long MonitorDetectBlindSetWholeArea(LPCTSTR strGepID, short wChn, BOOL bEnable, long dwOccurRate, long dwRecoverRate)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I2 VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0xdc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strGepID, wChn, bEnable, dwOccurRate, dwRecoverRate);
		return result;
	}
	long OperateCommonEntry(LPCTSTR strParserBody)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xdd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, strParserBody);
		return result;
	}
	long PUGetProtocolInfo(short wProtolType)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xde, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wProtolType);
		return result;
	}
	long StartStreamDispatchByWndInx(short wWndIndex, LPCTSTR strGepID, short wChn)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_BSTR VTS_I2 ;
		InvokeHelper(0xdf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wWndIndex, strGepID, wChn);
		return result;
	}
	long StopStreamDispatchByWndInx(short nWndIndex)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xe0, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nWndIndex);
		return result;
	}
	long StartStreamDispatchAudioCall(short wWndInx, LPCTSTR strPuId, short byChnId)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_BSTR VTS_I2 ;
		InvokeHelper(0xe1, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wWndInx, strPuId, byChnId);
		return result;
	}
	long StopStreamDispatchAudioCall(short wWndInx)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0xe2, DISPATCH_METHOD, VT_I4, (void*)&result, parms, wWndInx);
		return result;
	}
	CString GetSelfData(long nDataType)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xe3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nDataType);
		return result;
	}
	BOOL SetSelfData(long nDataType, LPCTSTR strXml)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0xe4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, nDataType, strXml);
		return result;
	}
	long EnablePlugin(long dwPluginType, BOOL bEnable)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0xe5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, dwPluginType, bEnable);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//



};
