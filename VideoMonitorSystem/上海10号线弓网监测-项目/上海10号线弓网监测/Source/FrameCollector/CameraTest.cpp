#include "stdafx.h"
#include "CameraTest.h"
#include "FrameCollector.h"
#include "FrameCollectorDlg.h"
#include "ijl/ijlutil.h"

#define ALGORITHM 0

CCameraTest::CCameraTest(void)
	:m_pService(NULL)
	,m_pDevice(NULL)
{
	m_createService = NULL;

	int ret = loadLibrary();
	if (ret != 0)
	{
		exit(0);
	}

}


CCameraTest::~CCameraTest(void)
{
	//�ر����
	closeCamera();
}

bool CCameraTest::getConfigureInfo(const CString &strSection)
{
	USES_CONVERSION;
	const int SIZE = 1024;
	TCHAR Buffer[SIZE];
	memset(Buffer, 0, sizeof(Buffer));

	CString strIniFile(theApp.getCurrentPath() + _T("\\configFrame.ini"));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////�������
	GetPrivateProfileString(strSection, _T("CameraID"), _T("S2"), Buffer,SIZE, strIniFile);
	m_sCameraInfo.strCameraID = T2A(Buffer);
	m_sCameraInfo.iChannel = GetPrivateProfileInt(strSection, _T("Channel"), 0,strIniFile);
	m_sCameraInfo.iWidth = GetPrivateProfileInt(strSection, _T("Width"), 0,strIniFile);
	m_sCameraInfo.iHeight = GetPrivateProfileInt(strSection, _T("Height"), 0,strIniFile);
	m_sCameraInfo.iOffsetX = GetPrivateProfileInt(strSection, _T("OffsetX"), 0,strIniFile);
	m_sCameraInfo.iOffsetY = GetPrivateProfileInt(strSection, _T("OffsetY"), 0,strIniFile);
	m_sCameraInfo.iZoomWidth = m_sCameraInfo.iWidth * GetPrivateProfileInt(strSection, _T("ZoomWRate"), 500,strIniFile) / 1000;
	m_sCameraInfo.iZoomHeight = m_sCameraInfo.iHeight * GetPrivateProfileInt(strSection, _T("ZoomHRate"), 500,strIniFile) / 1000;
	GetPrivateProfileString(strSection, _T("FrameRate"), _T("25.0"), Buffer,SIZE, strIniFile);
	m_sCameraInfo.fFrameRate = (float)_ttof(Buffer);

	m_sCameraInfo.iQuality = GetPrivateProfileInt(strSection, _T("Quality"), 85, strIniFile);
	m_sCameraInfo.iPointAlgorithm = GetPrivateProfileInt(strSection, _T("PointAlgorithm"), 0, strIniFile);

	m_sCameraInfo.iTriggerMode = GetPrivateProfileInt(strSection, _T("TriggerMode"), 0,strIniFile);
	m_sCameraInfo.iTriggerType = (bool)(0 != GetPrivateProfileInt(strSection, _T("TriggerType"), 0,strIniFile));
	m_sCameraInfo.iExposureTime[CPT_NORMAL] = GetPrivateProfileInt(strSection, _T("ExposureTime"), 0,strIniFile);
	m_sCameraInfo.iExposureTime[CPT_SUNNY] = GetPrivateProfileInt(strSection, _T("ExposureTime-Sunny"), 0,strIniFile);
	m_sCameraInfo.iExposureTime[CPT_CLOUDY] = GetPrivateProfileInt(strSection, _T("ExposureTime-Cloudy"), 0,strIniFile);
	m_sCameraInfo.iExposureTime[CPT_RAINY] = GetPrivateProfileInt(strSection, _T("ExposureTime-Rainy"), 0,strIniFile);
	m_sCameraInfo.iExposureTime[CPT_SNOW] = GetPrivateProfileInt(strSection, _T("ExposureTime-Snow"), 0,strIniFile);
	m_sCameraInfo.iExposureTime[CPT_TUNNEL] = GetPrivateProfileInt(strSection, _T("ExposureTime-Tunnel"), 0,strIniFile);
	m_sCameraInfo.iExposureTime[CPT_AUTO] = -1;
	m_sCameraInfo.iAutoExpMin = GetPrivateProfileInt(strSection, _T("AutoExp_Min"),0,strIniFile);
	m_sCameraInfo.iAutoExpMax = GetPrivateProfileInt(strSection, _T("AutoExp_Max"),0,strIniFile);
	m_sCameraInfo.iAutoGainMin = GetPrivateProfileInt(strSection, _T("AutoGain_Min"),0,strIniFile);
	m_sCameraInfo.iAutoGainMax = GetPrivateProfileInt(strSection, _T("AutoGain_Max"),0,strIniFile);

	m_sCameraInfo.iGain[CPT_NORMAL] = GetPrivateProfileInt(strSection, _T("Gain"), 320,strIniFile);
	m_sCameraInfo.iGain[CPT_SUNNY] = GetPrivateProfileInt(strSection, _T("Gain-Sunny"), 320,strIniFile);
	m_sCameraInfo.iGain[CPT_CLOUDY] = GetPrivateProfileInt(strSection, _T("Gain-Cloudy"), 320,strIniFile);
	m_sCameraInfo.iGain[CPT_RAINY] = GetPrivateProfileInt(strSection, _T("Gain-Rainy"), 320,strIniFile);
	m_sCameraInfo.iGain[CPT_SNOW] = GetPrivateProfileInt(strSection, _T("Gain-Snow"), 320,strIniFile);
	m_sCameraInfo.iGain[CPT_TUNNEL] = GetPrivateProfileInt(strSection, _T("Gain-Tunnel"), 320,strIniFile);
	m_sCameraInfo.iGain[CPT_AUTO] = m_sCameraInfo.iGain[CPT_NORMAL];

	m_sCameraInfo.iQueueCount = GetPrivateProfileInt(strSection, _T("QueueCount"), 1,strIniFile);
	m_sCameraInfo.iNumBufferCount = GetPrivateProfileInt(strSection, _T("NumBufferCount"), 16,strIniFile);
	m_sCameraInfo.iQueueSizePreQueue = GetPrivateProfileInt(strSection, _T("QueueSize"), 120,strIniFile);
	m_sCameraInfo.iShootingDistance = GetPrivateProfileInt(strSection, _T("JuLi"), 0,strIniFile);
	m_sCameraInfo.uIntervalFramesWhenDecodeImage = GetPrivateProfileInt(strSection, _T("IntervalFramesWhenDecodeImage"), 5, strIniFile);
	if (m_sCameraInfo.uIntervalFramesWhenDecodeImage == 0)m_sCameraInfo.uIntervalFramesWhenDecodeImage = 5;

	//����������
	GetPrivateProfileString(strSection, _T("Provider"), _T(""), Buffer, SIZE, strIniFile);
	m_sCameraInfo.strCameraProvider = Buffer;
	GetPrivateProfileString(strSection, _T("FriendlyName"), _T(""), Buffer, SIZE, strIniFile);
	m_sCameraInfo.strFriendlyName = Buffer;
	GetPrivateProfileString(strSection, _T("DestIP"), _T(""), Buffer, SIZE, strIniFile);
	m_sCameraInfo.strDestIP = Buffer;
	GetPrivateProfileString(strSection, _T("DestGateway"), _T(""), Buffer, SIZE, strIniFile);
	m_sCameraInfo.strDestGateWay = Buffer;
	GetPrivateProfileString(strSection, _T("DestSubnet"), _T(""), Buffer, SIZE, strIniFile);
	m_sCameraInfo.strDestSubNet = Buffer;
	m_sCameraInfo.uDestUdpPort1 = GetPrivateProfileInt(strSection, _T("DestUdpPort1"), 60000, strIniFile);
	m_sCameraInfo.uDestUdpPort2 = GetPrivateProfileInt(strSection, _T("DestUdpPort2"), 60002, strIniFile);
	m_sCameraInfo.uDestTcpPort = GetPrivateProfileInt(strSection, _T("DestTcpPort"), 60003, strIniFile);
	GetPrivateProfileString(strSection, _T("LocalIP"), _T(""), Buffer, SIZE, strIniFile);
	m_sCameraInfo.strLocalIP = Buffer;
	m_sCameraInfo.localBroadcastUdpPort = GetPrivateProfileInt(strSection, _T("LocalBroadcastUdpPort"), 60010, strIniFile);
	m_sCameraInfo.localReceiveUdpPort = GetPrivateProfileInt(strSection, _T("LocalReceiveUdpPort"), 60012, strIniFile);
	m_sCameraInfo.localReceiveUdpPort2 = GetPrivateProfileInt(strSection, _T("LocalReceiveUdpPort2"), 50062, strIniFile);

	m_sCameraInfo.localForwardUdpPort = GetPrivateProfileInt(strSection, _T("localForwardUdpPort"), 41010, strIniFile);
	m_sCameraInfo.destForwardUdpPort = GetPrivateProfileInt(strSection, _T("destForwardUdpPort"), 41020, strIniFile);

	m_sCameraInfo.iBrightness = GetPrivateProfileInt(strSection, _T("Brightness"), 20, strIniFile);
	m_sCameraInfo.bSaveCopy = false;
	m_sCameraInfo.eCamType = E_CamType_TEST;

	getCameraInfoFromParamInfo(m_sCameraInfo, m_sCamDeviceInfo, m_sCamDeviceFormatInfo);

	return TRUE;
}

bool CCameraTest::init()
{
	bool is_suc = false;
	int init_ret = initCamera();
	int set_ret = 0;//initSetOptions();
	is_suc = (init_ret == CAM_OK) && (set_ret == CAM_OK);
	return is_suc;
}

bool CCameraTest::start()
{
	int suc = m_pDevice->play(0xF);
	return (suc == CAM_OK);
}

bool CCameraTest::camera_stop()
{
	return (m_pDevice->stop() == CAM_OK);
}

void CCameraTest::closeCamera()
{
	if (!isValidCamera())return;

	int ret = 0;
	//ֹͣ�豸
	ret =m_pDevice->stop();
	//ɾ���豸
	ret =m_pService->removeDevice(m_pDevice);
	m_pDevice = NULL;
	//ɾ���豸����
	m_pService->releaseService();
	m_pService = NULL;
}


unsigned char* CCameraTest::decodeImg(unsigned char* pData, int iDataLen)
{
	unsigned char* pTempData = NULL;
	int iOutputLen = 0;
	int iWidth = 0;
	int iHeight = 0;
	CIJLUtil iJLUtil;
	int iRet = iJLUtil.DeCompressImg(&pTempData, pData, iDataLen, &iWidth, &iHeight, &iOutputLen);
	if (iRet < 0 || pTempData == NULL)
	{
		//�ͷ�
		if (pTempData != NULL)
		{
			delete[] pTempData;
			pTempData = NULL;
		}
		return NULL;
	}

	if (m_sCameraInfo.iWidth != iWidth	||
		m_sCameraInfo.iHeight != iHeight ||
		iOutputLen != m_sCameraInfo.iWidth*m_sCameraInfo.iHeight)
	{
		//�ͷ�
		delete[] pTempData;
		pTempData = NULL;
	}

	return pTempData;
}


///˽��////////////////////////////////////////////////////////////////////////////////////////
void CCameraTest::getCameraInfoFromParamInfo(const CameraInfo &sCameraInfo, Cam::CamDeviceInfo &camDeviceInfo, Cam::CamDeviceFormatInfo &camDeviceFormatInfo)
{
	USES_CONVERSION;
	camDeviceInfo.provider = T2A(sCameraInfo.strCameraProvider);
	camDeviceInfo.friendlyName = T2A(sCameraInfo.strFriendlyName);
	camDeviceInfo.localIP = T2A(sCameraInfo.strLocalIP);
	//camDeviceInfo.localBroadcastUdpPort  = sCameraInfo.localBroadcastUdpPort;
	//camDeviceInfo.localReceiveUdpPort  = sCameraInfo.localReceiveUdpPort;
	//camDeviceInfo.localReceiveUdpPort2  = sCameraInfo.localReceiveUdpPort2;
	camDeviceInfo.destIP = T2A(sCameraInfo.strDestIP);
	//camDeviceInfo.destGateWay = T2A(sCameraInfo.strDestGateWay);
	//camDeviceInfo.destSubNet = T2A(sCameraInfo.strDestSubNet);
	//camDeviceInfo.destUdpPort1  = sCameraInfo.uDestUdpPort1;
	//camDeviceInfo.destUdpPort2  = sCameraInfo.uDestUdpPort2;
	//camDeviceInfo.destTcpPort  = sCameraInfo.uDestTcpPort;

	//camDeviceFormatInfo.x = sCameraInfo.iOffsetX;
	//camDeviceFormatInfo.y = sCameraInfo.iOffsetY;
	camDeviceFormatInfo.width = sCameraInfo.iWidth;
	camDeviceFormatInfo.height = sCameraInfo.iHeight;
	camDeviceFormatInfo.triggerMode = 1;
	camDeviceFormatInfo.triggerType = 0;
	camDeviceFormatInfo.iStreamType = 0;
	camDeviceFormatInfo.fps = 25;
	camDeviceFormatInfo.pReceiveData = &CFrameCollectorDlg::receiveImageData_Tcp;
	camDeviceFormatInfo.pParam = dynamic_cast<CFrameCollectorDlg*>(theApp.m_pMainWnd);

	camDeviceFormatInfo.pReceiveData2 = &CFrameCollectorDlg::receiveImageData_Udp;
	camDeviceFormatInfo.pParam2 = dynamic_cast<CFrameCollectorDlg*>(theApp.m_pMainWnd);
}

int CCameraTest::initCamera()
{
	USES_CONVERSION;

	Cam::CCamObjectService* pService = NULL;
	m_createService(&pService);

	//ʧ��
	if (pService == NULL)return CAM_FAIL_INVALID_CAMOBJECT;

	//�õ��������
	char info[1024];
	memset(info, 0, sizeof(info));
	int iRet = ((CObjectService*)pService)->getAvailableDevices(info);

	//ʧ��
	if (iRet != CAM_OK ||
		//������Ӧ���
		!findDevice(T2A(m_sCameraInfo.strCameraProvider), info))
	{
		pService->releaseService();

		if (iRet != CAM_OK)return iRet;
		return CAM_FAIL_INVALID_CAMERA;
	}

	Cam::CCamObjectDevice* pDevice = dynamic_cast<Cam::CCamObjectDevice*>(pService->createDevice(m_sCamDeviceInfo, m_sCamDeviceFormatInfo));
	if (pDevice == NULL || !m_sCamDeviceInfo.initSuccess)
	{
		//pService->releaseService();
		//return CAM_FAIL_INVALID_CAMOBJECT;
	}

	Cam::CamDeviceInfo sCamDeviceInfo;
	sCamDeviceInfo = pDevice->getDeviceInfo();

	//����ZSOCKET
	m_sCamDeviceInfo.uUdpZSocketID = sCamDeviceInfo.uUdpZSocketID;
	m_sCamDeviceInfo.uUdpClientID = sCamDeviceInfo.uUdpClientID;

	m_pService = dynamic_cast<Cam::CCamObjectService*>(pService);
	m_pDevice = pDevice;

	//set option
	const std::string provider = m_sCamDeviceInfo.provider;
	if (provider == "CAM_THERMOGROUP_CAMERA")
	{

	}
	else if(provider == "CAM_BASLER_CAMERA")
	{
		unsigned int fps = 32;
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_FRAME_CYCLE, 
			"%u\r\n", fps);
	}
	else if (provider == "CAM_NETWORK_STAR_PMT_UV_CAMERA")
	{
		int snap = 1;
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_STM, "%d\r\n", snap);//1:Snap
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_SAST, "200\r\n");//����ȼ����ʼ��ֵ
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_SAET, "100\r\n");//����ȼ��������ֵ
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_SSPT, "1000\r\n");//����ȼ�����ʱ��
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_SDUT, "500\r\n");//����ȼ������ʱ��
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_SMUT, "1000\r\n");//ȼ���ְ��ϴ�ʱ���λʱ��
	}
	else if (provider == "CAM_NETWORK_GX2_3D_CAMERA")
	{
		const float fps = 10.0;
		const unsigned int uMicoSeconds = (unsigned int)((float)1.0/fps * 1000*1000);
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_FRAME_CYCLE, 
			"%u\r\n", uMicoSeconds);

		
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_SWM, 
			"%d\r\n", 1);

		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_TRIGGER_MODE, 
			"%d\r\n", 3);

	}
	return CAM_OK;
}

int CCameraTest::initSetOptions()
{
	int iRet = CAM_OK;

	do
	{
		/*
		//���ô��ڴ�С�����Ҫ����������Ҫ�ڳ����п���
		iRet = Das_setOption(m_uCameraObjectID, m_uCameraID, Cam::CAM_COMMAND_IMAGE_X_Y_WITH_HEIGHT, 
			"%d %d %d %d\r\n", m_sCamDeviceFormatInfo.x, m_sCamDeviceFormatInfo.y, m_sCamDeviceFormatInfo.width, m_sCamDeviceFormatInfo.height); 
		if (iRet != CAM_OK)break;
		*/

		//����֡���ڣ�us��
		unsigned int uMicoSeconds = (unsigned int)((float)1/m_sCameraInfo.fFrameRate*1000*1000);
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_FRAME_CYCLE, 
			"%u\r\n", uMicoSeconds);
		if (iRet != CAM_OK)break;


		//���ô���ģʽ
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_TRIGGER_MODE, 
			"%d\r\n", m_sCameraInfo.iTriggerMode);
		if (iRet != CAM_OK)break;

		//�ⴥ��������ͼ��Ĵ���ģʽ,0�����ɴ���,1���ش���,2����ƽ������3������
		if (m_sCameraInfo.iTriggerMode == 1 || m_sCameraInfo.iTriggerMode == 2)
		{
			//���ô���ģʽ��0���½���/�͵�ƽ,1��������/�ߵ�ƽ��
			iRet = m_pDevice->setOption(Cam::CAM_COMMAND_TRIGGER_TYPE, 
				"%d\r\n", m_sCameraInfo.iTriggerType);
		}

		if (m_sCameraInfo.iExposureTime[CPT_NORMAL] == -1 || m_sCameraInfo.bAutoSet)
		{
			//�����Զ��ع�
			iRet = m_pDevice->setOption(Cam::CAM_COMMAND_AUTO_EXPOSURE, 
			"%d\r\n", 1);
			if (iRet != CAM_OK)break;
		}
		else
		{
			//�ر��Զ��ع�
			iRet = m_pDevice->setOption(Cam::CAM_COMMAND_AUTO_EXPOSURE, 
			"%d\r\n", 0);
			if (iRet != CAM_OK)break;

			/*
			//�ع�ģʽΪʱ�䣨���basler�����
			//Basler��Ч:0Ϊ�ر��ع�ģʽ��1Ϊ�ع�ʱ�䣬2Ϊ������ȣ�3Ϊ��������			
			iRet = Das_setOption(m_uCameraObjectID, m_uCameraID, Cam::CAM_COMMAND_EXPOSURE_MODE, 
			"%d\r\n", 1);
			if (iRet != CAM_OK)break;
			*/

			//�����ع�ʱ��
			iRet = m_pDevice->setOption(Cam::CAM_COMMAND_EXPOSURE_TIME, 
			"%d\r\n", m_sCameraInfo.iExposureTime[CPT_NORMAL]);
			if (iRet != CAM_OK)break;
		}

		//�ر��Զ�����
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_AUTO_GAIN, "%d\r\n", 0);
		if (iRet != CAM_OK)break;

		//�����Զ����淶Χ
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_AUTO_GAIN_RANGE, "0 10\r\n");
		if (iRet != CAM_OK)break;
		//������ֵ
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_GAIN_VALUE, "%d\r\n", m_sCameraInfo.iGain[CPT_NORMAL]);
		if (iRet != CAM_OK)break;

		//����JPEG����
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_JPEG_ZIP_VALUE, "%d\r\n", m_sCameraInfo.iQuality);
		if (iRet != CAM_OK)break;

#if ALGORITHM
		//��������㷨
		iRet = m_pDevice->setOption(Cam::CAM_COMMAND_ALGORITHM_RUN, "%d\r\n", m_sCameraInfo.iPointAlgorithm);
		if (iRet != CAM_OK)break;
#endif

	}while(false);

	return iRet;
}

bool CCameraTest::findDevice(const char* pProvider, const char* pDeviceInfos)
{
	if (pProvider == NULL || pDeviceInfos == NULL)return false;

	//�Ƚ���Ӧ������������ṩ��"CAM_NETWORK_A80I_CAMERA"
	//const char* pProvider = "CAM_NETWORK_A80I_CAMERA";
	/*
	*�õ����������豸
	*��ʽ���豸��\t�ṩ��\n
		   �豸��2\t�ṩ��\n
	*/
	char *pTempStr = NULL;
	//ǰһ���ַ���
	char *pPrev = const_cast<char*>(pDeviceInfos);
	while(*pPrev != '\0' && (pTempStr = strstr(pPrev, "\n")) != NULL)
	{
		*pTempStr = '\0';
		if (char *pTempStr2 = strstr(pPrev, "\t"))
		{
			if (strncmp(pProvider, pTempStr2 + 1, strlen(pProvider)) == 0)return true;
		}
		*pTempStr = '\n';

		pPrev = pTempStr + 1;
	}//end of while

	return false;
}

static CString getCurrentPath()
{
	CString strCurrentPath(_T(""));
	if (strCurrentPath.IsEmpty())
	{
		WCHAR buffer[MAX_PATH];
		memset(buffer, 0, sizeof(buffer));
		::GetModuleFileName(NULL, buffer, MAX_PATH);
		CString strPath(buffer);
		strPath = strPath.Left(strPath.ReverseFind(_T('\\')));
		strCurrentPath = strPath;
	}
	return strCurrentPath;
}

int CCameraTest::loadLibrary()
{
	USES_CONVERSION;
	CString current_path = getCurrentPath();
	CString current_dll_path = current_path + "\\CamPicture\\"; 
	SetDllDirectory(current_dll_path);
	HMODULE h_Module = LoadLibrary(A2W("Cameras.dll"));
	if (!h_Module)
	{
		return -1;
	}

	m_createService = (CREATESERVICE_CAMPICTURE)GetProcAddress(h_Module, "createService");
	if (!m_createService)
	{
		return -1;
	}
	return 0;
}
