#pragma once

//////////////////////////////////////////////////////////////////////////////
#include "CamPicture/CameraObject.h"
typedef int (*CREATESERVICE_CAMPICTURE)(Cam::CCamObjectService** pService);
//////////////////////////////////////////////////////////////////////////////

typedef struct tag_st_arc_camera
{
	UINT32     flag1;  
	UINT32     flag2; 
	UINT16     nWidth;                //Normal模式下，为实际值；Snap模式下，固定为0
	UINT16     nHeight;                //Normal模式下，为实际值；Snap模式下，固定为1
	UINT32     nImagePackageLen;    //Snap模式下，该值即为数据宽度
	UINT32     nCounter;  // 本包数据第一个采样点的编号，从上电开始一直编号0-0xffffffff。
	UINT16     nImageInfo;            //normal模式下是0x3001，Snap模式下0x4000
	UINT16     nCalcResultPrecion;
	UINT32     sIndex;                //本包数据内的第一个采样的编号（一次燃弧中，第一包第一个为0）
	UINT32     eIndex;                //本包数据内的，最后一个采样点的编号
	UINT32     aIndex;                 //本次燃弧，长传数据包所属的编号
	UINT32     aSize;                //本包内的燃弧大小：本包的燃弧累加值*系数
	UINT32     nReserved[6];
}st_arc_camera;

typedef enum{
	E_CamType_TEST,
}E_CAMERATYPE;

//相机参数类型
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

//相机信息
//ini文件提取的变量保存在此
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
	//帧率
	float		fFrameRate;
	//光点算法
	int			iPointAlgorithm;
	//JPEG因子
	int			iQuality;
	int			iTriggerMode;
	//相机采集触发类型
	//A80I:设置触发输入极性,0：下降沿/低电平,1：上升沿/高电平, 沿触发或电平触发模式下有效
	//Basler:1为软触发，0为硬触发
	bool		iTriggerType;
	int			iExposureTime[CPT_COUNT];
	//自动连续曝光
	bool		bAutoSet;
	int			iAutoExpMin;
	int			iAutoExpMax;
	int			iAutoGainMin;
	int			iAutoGainMax;

	int			iGain[CPT_COUNT];

	///采集参数
	int			iQueueCount;
	int			iNumBufferCount;
	int			iQueueSizePreQueue;

	//拍摄距离，单位是分米
	int			iShootingDistance;
	int			iShootingDistanceMin;
	int			iShootingDistanceMax;
	int			iShootingDistanceProtected;

	//每隔多少帧开始解码或在界面上显示图像
	unsigned int uIntervalFramesWhenDecodeImage;

	// 是否拷贝一份原图
	bool		bSaveCopy;
	//隧道补光亮度阈值
	int			iBrightness;
	//相机类型
	E_CAMERATYPE eCamType;


	//相机的网络属性
	//相机提供者名
	CString		strCameraProvider;
	CString strFriendlyName;
	//远程IP地址
	CString		strDestIP;
	//远程网关
	CString		strDestGateWay;
	//远程子网掩码
	CString		strDestSubNet;
	//远程端口
	unsigned short uDestUdpPort1, uDestUdpPort2, uDestTcpPort;

	//本地IP地址
	CString		strLocalIP;
	//本地IP地址端口
	unsigned short		localBroadcastUdpPort;
	unsigned short		localReceiveUdpPort;
	unsigned short		localReceiveUdpPort2;

	//转发给UDP包
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
	//图片文件保存路径
	CString strSavePath;
	//图片文件保存路径(拷贝)
	CString strSavePathCopy;
	//调试图片文件保存路径
	CString	strDebugPath;
	//错误标记图片路径(即打点)
	CString strMarkErrorPath;

	bool	bSavePathCopy;
	//磁盘配额,单位G
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
	//成功返回0，失败返回1
	int loadLibrary();
	int initCamera();
	int initSetOptions();
	bool findDevice(const char* pProvider, const char* pDeviceInfos);
public:
	//通用的参数，即和相机无关参数
	CameraInfo					m_sCameraInfo;
	Cam::CamDeviceInfo			m_sCamDeviceInfo;
	Cam::CamDeviceFormatInfo	m_sCamDeviceFormatInfo;

	Cam::CCamObjectService*		m_pService;
	Cam::CCamObjectDevice*		m_pDevice;

	CREATESERVICE_CAMPICTURE m_createService;
};

