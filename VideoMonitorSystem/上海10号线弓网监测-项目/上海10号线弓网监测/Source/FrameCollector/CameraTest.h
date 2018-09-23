#pragma once

//////////////////////////////////////////////////////////////////////////////
#include "CamPicture/CameraObject.h"
typedef int (*CREATESERVICE_CAMPICTURE)(Cam::CCamObjectService** pService);
//////////////////////////////////////////////////////////////////////////////

typedef struct tag_st_arc_camera
{
	UINT32     flag1;  
	UINT32     flag2; 
	UINT16     nWidth;                //Normalģʽ�£�Ϊʵ��ֵ��Snapģʽ�£��̶�Ϊ0
	UINT16     nHeight;                //Normalģʽ�£�Ϊʵ��ֵ��Snapģʽ�£��̶�Ϊ1
	UINT32     nImagePackageLen;    //Snapģʽ�£���ֵ��Ϊ���ݿ��
	UINT32     nCounter;  // �������ݵ�һ��������ı�ţ����ϵ翪ʼһֱ���0-0xffffffff��
	UINT16     nImageInfo;            //normalģʽ����0x3001��Snapģʽ��0x4000
	UINT16     nCalcResultPrecion;
	UINT32     sIndex;                //���������ڵĵ�һ�������ı�ţ�һ��ȼ���У���һ����һ��Ϊ0��
	UINT32     eIndex;                //���������ڵģ����һ��������ı��
	UINT32     aIndex;                 //����ȼ�����������ݰ������ı��
	UINT32     aSize;                //�����ڵ�ȼ����С��������ȼ���ۼ�ֵ*ϵ��
	UINT32     nReserved[6];
}st_arc_camera;

typedef enum{
	E_CamType_TEST,
}E_CAMERATYPE;

//�����������
enum CameraParamType
{
	CPT_SUNNY=0,
	CPT_CLOUDY,
	CPT_RAINY,
	CPT_SNOW,
	CPT_NORMAL,
	CPT_TUNNEL,
	CPT_AUTO,

	CPT_COUNT,
};

//�����Ϣ
//ini�ļ���ȡ�ı��������ڴ�
typedef struct tagCameraInfo
{
	tagCameraInfo()
		:strCameraID("")
		,iChannel(0)
		,iOffsetX(0)
		,iOffsetY(0)
		,iWidth(0)
		,iHeight(0)
		,iZoomWidth(0)
		,iZoomHeight(0)
		,fFrameRate(15.0f)
		,iPointAlgorithm(0)
		,iQuality(85)
		,iTriggerMode(0)
		,iTriggerType(0)
		,bAutoSet(false)
		,iAutoExpMin(0)
		,iAutoExpMax(0)
		,iAutoGainMin(0)
		,iAutoGainMax(0)
		,iQueueCount(0)
		,iNumBufferCount(0)
		,iQueueSizePreQueue(0)
		,iShootingDistance(0)
		,iShootingDistanceMin(0)
		,iShootingDistanceMax(0)
		,iShootingDistanceProtected(0)
		,uIntervalFramesWhenDecodeImage(5)
		,bSaveCopy(false)
		,iBrightness(100)
		,eCamType(E_CamType_TEST)
		,strCameraProvider(_T(""))
		,strDestIP(_T(""))
		,strDestGateWay(_T(""))
		,strDestSubNet(_T(""))
		,uDestUdpPort1(0)
		,uDestUdpPort2(0)
		,uDestTcpPort(0)
		,strLocalIP(_T(""))
		,localBroadcastUdpPort(0)
		,localReceiveUdpPort(0)
		,localReceiveUdpPort2(0)
		,localForwardUdpPort(0)
		,destForwardUdpPort(0)
	{}
	std::string	strCameraID;
	int			iChannel;
	int			iOffsetX;
	int			iOffsetY;
	int			iWidth;
	int			iHeight;
	int			iZoomWidth;
	int			iZoomHeight;
	//֡��
	float		fFrameRate;
	//����㷨
	int			iPointAlgorithm;
	//JPEG����
	int			iQuality;
	int			iTriggerMode;
	//����ɼ���������
	//A80I:���ô������뼫��,0���½���/�͵�ƽ,1��������/�ߵ�ƽ, �ش������ƽ����ģʽ����Ч
	//Basler:1Ϊ������0ΪӲ����
	bool		iTriggerType;
	int			iExposureTime[CPT_COUNT];
	//�Զ������ع�
	bool		bAutoSet;
	int			iAutoExpMin;
	int			iAutoExpMax;
	int			iAutoGainMin;
	int			iAutoGainMax;

	int			iGain[CPT_COUNT];

	///�ɼ�����
	int			iQueueCount;
	int			iNumBufferCount;
	int			iQueueSizePreQueue;

	//������룬��λ�Ƿ���
	int			iShootingDistance;
	int			iShootingDistanceMin;
	int			iShootingDistanceMax;
	int			iShootingDistanceProtected;

	//ÿ������֡��ʼ������ڽ�������ʾͼ��
	unsigned int uIntervalFramesWhenDecodeImage;

	// �Ƿ񿽱�һ��ԭͼ
	bool		bSaveCopy;
	//�������������ֵ
	int			iBrightness;
	//�������
	E_CAMERATYPE eCamType;


	//�������������
	//����ṩ����
	CString		strCameraProvider;
	CString strFriendlyName;
	//Զ��IP��ַ
	CString		strDestIP;
	//Զ������
	CString		strDestGateWay;
	//Զ����������
	CString		strDestSubNet;
	//Զ�̶˿�
	unsigned short uDestUdpPort1, uDestUdpPort2, uDestTcpPort;

	//����IP��ַ
	CString		strLocalIP;
	//����IP��ַ�˿�
	unsigned short		localBroadcastUdpPort;
	unsigned short		localReceiveUdpPort;
	unsigned short		localReceiveUdpPort2;

	//ת����UDP��
	unsigned short		localForwardUdpPort;
	unsigned short		destForwardUdpPort;

}CameraInfo, *PCameraInfo;

typedef struct tagSavePathInfo
{
	tagSavePathInfo()
		:strSavePath(_T(""))
		,strSavePathCopy(_T(""))
		,strDebugPath(_T(""))
		,strMarkErrorPath(_T(""))
		,bSavePathCopy(false)
		,fDiskQuota(4.0)
		,bZoom(true)
	{}
	//ͼƬ�ļ�����·��
	CString strSavePath;
	//ͼƬ�ļ�����·��(����)
	CString strSavePathCopy;
	//����ͼƬ�ļ�����·��
	CString	strDebugPath;
	//������ͼƬ·��(�����)
	CString strMarkErrorPath;

	bool	bSavePathCopy;
	//�������,��λG
	float	fDiskQuota;
	bool	bZoom;
}SavePathInfo, *PSavePathInfo;

typedef struct tagNormalParamInfo
{
	SavePathInfo	sSavePathInfo;
}NormalParamInfo, *PNormalParamInfo;

class CCameraTest
{
public:
	CCameraTest(void);
	~CCameraTest(void);
public:
	bool getConfigureInfo(const CString &strSection);
	bool init();
	bool start();
	bool camera_stop();
	void closeCamera();
	unsigned char* decodeImg(unsigned char* pData, int iDataLen);
public:
	CameraInfo& getCameraInfo(){return m_sCameraInfo;}
	Cam::CamDeviceInfo& getCamDeviceInfo(){return m_sCamDeviceInfo;}
	Cam::CamDeviceFormatInfo& getCamDeviceFormatInfo(){return m_sCamDeviceFormatInfo;}
	bool isValidCamera()const{return (m_pService != NULL && m_pDevice != NULL);}
private:
	void getCameraInfoFromParamInfo(const CameraInfo &sCameraInfo, Cam::CamDeviceInfo &camDeviceInfo, Cam::CamDeviceFormatInfo &camDeviceFormatInfo);
	//�ɹ�����0��ʧ�ܷ���1
	int loadLibrary();
	int initCamera();
	int initSetOptions();
	bool findDevice(const char* pProvider, const char* pDeviceInfos);
public:
	//ͨ�õĲ�������������޹ز���
	CameraInfo					m_sCameraInfo;
	Cam::CamDeviceInfo			m_sCamDeviceInfo;
	Cam::CamDeviceFormatInfo	m_sCamDeviceFormatInfo;

	Cam::CCamObjectService*		m_pService;
	Cam::CCamObjectDevice*		m_pDevice;

	CREATESERVICE_CAMPICTURE m_createService;
};

