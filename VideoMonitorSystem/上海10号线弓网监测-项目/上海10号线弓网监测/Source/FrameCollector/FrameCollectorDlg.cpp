
// FrameCollectorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FrameCollector.h"
#include "FrameCollectorDlg.h"
#include "afxdialogex.h"
#include "ijl/ijlutil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_DAS_MESSAGE_CAMERA_SHOW   (WM_USER + 50)
typedef struct tagCalcResult
{
	UINT32	head; // 计算的帧头，固定为：0x44504300
	UINT16	pointX;//计算得到的光点x坐标
	UINT16	pointY;// 计算得到的光点y坐标

	UINT8	ip[4]; //相机的ip地址
	UINT32  nCounter; //计算结果编号
}CalcResult, *PCalcResult;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFrameCollectorDlg 对话框




CFrameCollectorDlg::CFrameCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFrameCollectorDlg::IDD, pParent)
	,m_bInit(false)
	,m_bPlaying(false)
	,m_bPointRun(false)
	,m_bForwardUdp(false)
	,ImageBuffer(NULL)
	,m_bImageReady(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFrameCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_Image);
}

BEGIN_MESSAGE_MAP(CFrameCollectorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CFrameCollectorDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CFrameCollectorDlg::OnBnClickedButtonPlay)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_DAS_MESSAGE_CAMERA_SHOW, &CFrameCollectorDlg::OnShowImage)
END_MESSAGE_MAP()


// CFrameCollectorDlg 消息处理程序

BOOL CFrameCollectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "{8,1238},{8,688},{608,688},{608,1238},{8,1238}");
	m_Image.SetStdAreaOrg(buffer, 5, theApp.getCamera().getCameraInfo().iWidth, theApp.getCamera().getCameraInfo().iWidth, SATT_MIDDLE_POLENUM);

	SendMessage(WM_SIZE, 0, 0);

	this->SetTimer(1, 1000, NULL);
	this->SetTimer(2, 2000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFrameCollectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFrameCollectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFrameCollectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

static int saveFile(LPCTSTR lpFileName, const unsigned char *pImgBuf, int iImgSize)
{
	//保存文件
	int ret = -1;
	if (FILE *fp = _wfopen(lpFileName, L"wb+"))
	{
		ret = fwrite(pImgBuf, sizeof(unsigned char), iImgSize, fp);
		fclose(fp);
	}
	return ret;
}


static int SaveBmp(BYTE *buf, int width, int height, int iDataLen, BITMAPINFO * pBitmapInfo, LPCTSTR lpFileName)
{
	BITMAPFILEHEADER fileHeader;               /* bmp file header */
	BITMAPINFOHEADER infoHeader;             /* bmp information header */
	RGBQUAD rgb[256];                                   /* palette */

	int i;

	fileHeader.bfType = 0x4d42;
	fileHeader.bfOffBits = sizeof(fileHeader) + sizeof(infoHeader) + sizeof(rgb);
	fileHeader.bfSize = fileHeader.bfOffBits + iDataLen;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;

	infoHeader.biSize = sizeof(infoHeader);
	infoHeader.biWidth = width;
	infoHeader.biHeight = height;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 8;
	infoHeader.biCompression = BI_RGB;
	infoHeader.biSizeImage = iDataLen;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;

	for (i = 0; i < 256; i++) {
		rgb[i].rgbRed = i;
		rgb[i].rgbGreen = i;
		rgb[i].rgbBlue = i;
		rgb[i].rgbReserved = 0;
	}

	const int new_len = fileHeader.bfSize;
	char* n_bff = new char[new_len];
	int off_set = 0;
	memcpy(n_bff + off_set, &fileHeader, sizeof(fileHeader)); /* 写入位图文件头 */
	off_set += sizeof(fileHeader);
	memcpy(n_bff + off_set, &infoHeader, sizeof(infoHeader));  /* 写入位图信息头 */
	off_set += sizeof(infoHeader);
	memcpy(n_bff + off_set, rgb, sizeof(rgb));/* 写入调色板 */
	off_set += sizeof(rgb);
	memcpy(n_bff + off_set, buf, iDataLen);/* 写入图像数据 */
	off_set += iDataLen;

	saveFile(lpFileName, reinterpret_cast<const unsigned char*>(n_bff), off_set);

	delete[] n_bff;

	return 0;
}

static int bmp2jpg(int width, int height, const char* pData, char*& pJpg, DWORD& destfilelength)
{
	const int jpe_len = width * height * 4;
	pJpg = new char[jpe_len];
	memset(pJpg, 0, jpe_len);
	CIJLUtil ijlutil;
	return ijlutil.Encode(width, height, (LPVOID)pData, width * height, pJpg, jpe_len, &destfilelength);
}


void __stdcall CFrameCollectorDlg::receiveImageData_Tcp(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader)
{
	const std::string provider = theApp.getCamera().m_sCamDeviceInfo.provider;

	static unsigned int uImageIndex = 1;
	static unsigned int uTotalTime = 0;
	static unsigned int uLastFrame = 0;

	CFrameCollectorDlg* pDlg = reinterpret_cast<CFrameCollectorDlg*>(pParam);

	CString strFileName(_T(""));
	int ret = 0;
	double temp_low = 0;
	double temp_high = 0;
	double temp_avarage = 0;
	UINT32 packInd = 0;
	UINT32 arcVal = 0;
	//解码
	//每隔多少帧解码
	if (1)//uImageIndex%theApp.getCamera().getCameraInfo().uIntervalFramesWhenDecodeImage == 0)
	{
		if (pDlg->m_bImageReady)
		{
			unsigned int width = theApp.getCamera().m_sCamDeviceFormatInfo.width;
			unsigned int height = theApp.getCamera().m_sCamDeviceFormatInfo.height;

			if (provider == "CAM_THERMOGROUP_CAMERA")
			{
				int info[5] = {0};
				CThermoGroupDeviceEx* pDevice =  dynamic_cast<CThermoGroupDeviceEx*>(theApp.getCamera().m_pDevice);

				UINT x0 = 0;
				UINT y0 = 0;
				UINT x1 = width - 1;
				UINT y1 = height - 1;
				bool is_get_temp = pDevice->getRectTemperatureInfo(x0, y0, x1, y1, info); 
				if (!is_get_temp)		
				{
					return;
				}
				temp_low = info[0] * 0.001f;
				temp_high = info[1] * 0.001f;
				temp_avarage = info[2] * 0.001f;
				char msg[64];
				sprintf(msg, "temp_low=%.1f, temp_high=%.1f, temp_avarage=%.1f\n", temp_low, temp_high, temp_avarage);
				TRACE(msg);

				DWORD destfilelength = 0;
				char* p_jpeg = NULL;
				ret = bmp2jpg(width, height, pData, p_jpeg, destfilelength);
				if(ret == 0)
				{
					strFileName.Format(_T("E:\\hongwai\\%d.jpg"), uImageIndex);
					ret = saveFile(strFileName, reinterpret_cast<const unsigned char*>(p_jpeg), destfilelength);
				}
				delete[] p_jpeg; 

				pDlg->m_bImageReady = false;
				pDlg->ShowImage((unsigned char*)pData, theApp.getCamera().getCameraInfo().iWidth, theApp.getCamera().getCameraInfo().iHeight);
			}
			else if(provider == "CAM_BASLER_CAMERA")
			{
				DWORD destfilelength = 0;
				char* p_jpeg = NULL;
				ret = bmp2jpg(width, height, pData, p_jpeg, destfilelength);
				if(ret == 0)
				{
					strFileName.Format(_T("E:\\hongwai\\%d.jpg"), uImageIndex);
					ret = saveFile(strFileName, reinterpret_cast<const unsigned char*>(p_jpeg), destfilelength);
				}
				delete[] p_jpeg; 

				pDlg->m_bImageReady = false;
				pDlg->ShowImage((unsigned char*)pData, theApp.getCamera().getCameraInfo().iWidth, theApp.getCamera().getCameraInfo().iHeight);
			}
			else if (provider == "CAM_NETWORK_STAR_PMT_UV_CAMERA")
			{
				st_arc_camera arc_camera;
				if (iDataLen < sizeof(arc_camera))
				{
					return;
				}
				memcpy(&arc_camera, pDataHeader, sizeof(arc_camera));

				{
					packInd = arc_camera.aIndex;
					int point_count = arc_camera.eIndex - arc_camera.sIndex;
					if (point_count == 0)
					{
						point_count = 1;
					}
					arcVal = arc_camera.aSize / point_count;
				}
			}
			else if(provider == "CAM_NETWORK_GX2_3D_CAMERA")
			{
				DWORD destfilelength = 0;
				char* p_jpeg = NULL;
				ret = bmp2jpg(width, height, pData, p_jpeg, destfilelength);
				if(ret == 0)
				{
					strFileName.Format(_T("E:\\hongwai\\%d.jpg"), uImageIndex);
					ret = saveFile(strFileName, reinterpret_cast<const unsigned char*>(p_jpeg), destfilelength);
				}
				delete[] p_jpeg; 

				pDlg->m_bImageReady = false;
				pDlg->ShowImage((unsigned char*)pData, theApp.getCamera().getCameraInfo().iWidth, theApp.getCamera().getCameraInfo().iHeight);
			}
			else if(provider == "CAM_NETWORK_A80I_CAMERA")
			{
				if(ret == 0)
				{
					strFileName.Format(_T("D:\\A80I\\%d.jpg"), uImageIndex);
					ret = saveFile(strFileName, reinterpret_cast<const unsigned char*>(pData), iDataLen);
				}

				//pDlg->m_bImageReady = false;
				//pDlg->ShowImage((unsigned char*)pData, theApp.getCamera().getCameraInfo().iWidth, theApp.getCamera().getCameraInfo().iHeight);
			}
			else
			{
				strFileName.Format(_T("E:\\hongwai\\%d.jpg"), uImageIndex);
				saveFile(strFileName, reinterpret_cast<const unsigned char*>(pData), iDataLen);

				unsigned char* pTempData = theApp.getCamera().decodeImg(reinterpret_cast<unsigned char*>(const_cast<char*>(pData)), iDataLen);
				if (pTempData == NULL)
				{
					TRACE("%s enter, pData=%p,iDataLen=%d,pTempData == NULL\n",__FUNCTION__,pData, iDataLen);
					return;
				}
				pDlg->m_bImageReady = false;
				pDlg->ShowImage(pTempData, theApp.getCamera().getCameraInfo().iWidth, theApp.getCamera().getCameraInfo().iHeight);
				delete [] pTempData;
			}

		}
	}

	//求帧率
	float fps = (uTotalTime > 0?((float)uImageIndex*1000/(float)uTotalTime):0);
	CString strFps(_T(""));
	if (provider == "CAM_THERMOGROUP_CAMERA")
	{
		strFps.Format(_T("%.3f, ImageIndex=%u,temp_low=%.1f, temp_high=%.1f, temp_avarage=%.1f"), fps, uImageIndex,temp_low, temp_high, temp_avarage);
	}
	else if (provider == "CAM_NETWORK_STAR_PMT_UV_CAMERA")
	{
		strFps.Format(_T("%.3f, ImageIndex=%u,packInd=%d, arcVal=%d"), fps, uImageIndex,packInd, arcVal);
	}
	else
	{
		strFps.Format(_T("%.3f, ImageIndex=%u"), fps, uImageIndex);
	}
	pDlg->GetDlgItem(IDC_STATIC_FPS)->SetWindowText(strFps);
	uImageIndex++;
	unsigned int uEnd = ::GetTickCount();
	if (uLastFrame > 0)
		uTotalTime += uEnd - uLastFrame;

	uLastFrame = uEnd;
}

static unsigned int s_uReceiveImageDataFrameCount = 0;
static unsigned int s_uOldReceiveImageDataFrameCount = 0;
static unsigned int s_uReceiveImageData = 0;

void __stdcall CFrameCollectorDlg::receiveImageData_Udp(const char *pData, int iDataLen, PVOID pParam, PVOID pDataHeader)
{
	s_uReceiveImageDataFrameCount++;
	CFrameCollectorDlg* pDlg = reinterpret_cast<CFrameCollectorDlg*>(pParam);
	if (pDlg == NULL)return;
	PCalcResult pResult = reinterpret_cast<PCalcResult>(pDataHeader);
	if (pResult == NULL)return;
	s_uReceiveImageData = ((((unsigned int)pResult->pointY) << 16)&0xFFFF0000) + (((unsigned int)pResult->pointX)&0xFFFF);



	//转发UDP数据包，且只转发坐标点,4个字节
	UINT uData = 0;
	unsigned char* pBuffer = (unsigned char*)&uData;
	//x,y
	*(UINT16*)pBuffer = pResult->pointX;
	*(UINT16*)(pBuffer + sizeof(UINT16)) = pResult->pointY;
	//theApp.getCamera().forwardUdpData((char*)pBuffer, sizeof(UINT));

	//TRACE("%s enter, pData=%p,iDataLen=%d\n",__FUNCTION__,pData, iDataLen);


}

static unsigned int s_uReceiveForwardDataFrameCount = 0;
static unsigned int s_uOldReceiveForwardDataFrameCount = 0;
static unsigned int s_uReceiveForwardData = 0;


//显示 由相机采集完成后 通知 图层刷新显示 ——> 拷贝第二次 
void CFrameCollectorDlg::ShowImage(unsigned char *pbuf, unsigned int w, unsigned int h)
{
	if(ImageBuffer == NULL)
		ImageBuffer = new unsigned char[w*h];
	memcpy(ImageBuffer,pbuf,w*h);
	ImgInfo *ii = new ImgInfo(false);
	ii->setBuf(ImageBuffer);
	ii->setSize(w*h);
	ii->setHeight(h);
	ii->setWidth(w);
	SendMessage(WM_DAS_MESSAGE_CAMERA_SHOW, (WPARAM)ii, (LPARAM)0);
}

unsigned int __stdcall CFrameCollectorDlg::Camera_Close(LPVOID lParam)
{
	CFrameCollectorDlg* pDlg = reinterpret_cast<CFrameCollectorDlg*>(lParam);
	theApp.getCamera().closeCamera();
	pDlg->m_bInit = false;
	pDlg->m_bPointRun = false;
	pDlg->m_bPlaying = false;
	pDlg->GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("打开相机"));
	pDlg->GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("采集图像"));
	pDlg->GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);

	return 0;
}

unsigned int __stdcall CFrameCollectorDlg::Camera_Stop(LPVOID lParam)
{
	CFrameCollectorDlg* pDlg = reinterpret_cast<CFrameCollectorDlg*>(lParam);

	//停止
	bool is_stop = theApp.getCamera().camera_stop();
	pDlg->m_bPlaying = false;
	pDlg->GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("采集图像"));
	pDlg->GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);

	return 0;
}


LRESULT CFrameCollectorDlg::OnShowImage(WPARAM wParam, LPARAM lParam)
{
	if (ImgInfo* imge = reinterpret_cast<ImgInfo*>(wParam))
	{
		m_Image.LoadImge(imge, imge->getWidth(), imge->getHeight());
		m_bImageReady = true;
	}
	return 0L;
}

CFrameCollectorDlg::~CFrameCollectorDlg()
{
	if (ImageBuffer != NULL)
	{
		delete[] ImageBuffer;
		ImageBuffer = NULL;
	}
}

void CFrameCollectorDlg::OnBnClickedButtonConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bInit)
	{
		//关闭
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		_beginthreadex(NULL, 0, &CFrameCollectorDlg::Camera_Close, this, 0, NULL);
		return;
	}

	m_bInit = theApp.getCamera().init();
	if (!m_bInit)
	{
		AfxMessageBox(_T("初始化相机失败！"));
		return;
	}

	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("连接成功！"));
	GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("关闭相机"));
	m_bPointRun = (theApp.getCamera().getCameraInfo().iPointAlgorithm == 1);
	//if (m_bPointRun)
	//GetDlgItem(IDC_BUTTON_POINT)->SetWindowText(_T("关闭光点"));
	//else
	//GetDlgItem(IDC_BUTTON_POINT)->SetWindowText(_T("开启光点"));
}


void CFrameCollectorDlg::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bInit)
	{
		AfxMessageBox(_T("相机未初始化，请先初始化相机！"));
		return;
	}

	//在采集
	if (m_bPlaying)
	{
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
		_beginthreadex(NULL, 0, &CFrameCollectorDlg::Camera_Stop, this, 0, NULL);
	}
	else
	{
		m_Image.Clear();
		//采集
		m_bPlaying = theApp.getCamera().start();
		if (m_bPlaying)
		{
			GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText(_T("停止"));
		}

		//add
		int iRet = 0;
		const std::string provider = theApp.getCamera().m_sCamDeviceInfo.provider;
		if (provider == "CAM_NETWORK_GX2_3D_CAMERA")
		{
			iRet = theApp.getCamera().m_pDevice->setOption(Cam::CAM_COMMAND_TRIGGER_SOFTWARE,
				"\r\n");
		}
		iRet = 1;
		//add
	}
}

void CFrameCollectorDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//先停止相机
	if (m_bPlaying || m_bPointRun || m_bForwardUdp)
	{
		AfxMessageBox(_T("正在采集，请先停止采集，再退出！"));
		return;
	}

	CDialogEx::OnClose();
}

void CFrameCollectorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (GetDlgItem(IDC_STATIC_IMAGE) == NULL)return;
	//计算出新的图片显示位置
	CRect rectImage;
	m_Image.GetWindowRect(&rectImage);
	CMyCanvas::GetStretchRate(rectImage, theApp.getCamera().getCameraInfo().iWidth, theApp.getCamera().getCameraInfo().iHeight,
		theApp.getCamera().getCameraInfo().iWidth, theApp.getCamera().getCameraInfo().iHeight, 0, rectImage, rectImage);
	m_Image.ImgRect = rectImage;

	m_Image.SetStdArea();
}


void CFrameCollectorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 1)//1s
	{
		unsigned int uFrameCount = s_uReceiveForwardDataFrameCount;
		unsigned int pointX = (s_uReceiveForwardData&0xFFFF);
		unsigned int pointY = ((s_uReceiveForwardData >>16)&0xFFFF);

		CString strText(_T(""));
		strText.Format(_T("X=%d, Y=%d, fps=%04d, FrameCount=%u"), pointX, pointY, (uFrameCount - s_uOldReceiveForwardDataFrameCount), uFrameCount);
		//GetDlgItem(IDC_STATIC_FORWARD_RECV_FPS)->SetWindowText(strText);
		s_uOldReceiveForwardDataFrameCount = uFrameCount;
	}
	else if (nIDEvent == 2)//2s
	{
		unsigned int uFrameCount = s_uReceiveImageDataFrameCount;
		unsigned int pointX = (s_uReceiveImageData&0xFFFF);
		unsigned int pointY = ((s_uReceiveImageData >>16)&0xFFFF);
		//光点帧率
		CString strText(_T(""));
		strText.Format(_T("X=%d, Y=%d, fps=%04d, FrameCount=%u"), pointX, pointY, (uFrameCount - s_uOldReceiveImageDataFrameCount)>>1, uFrameCount);
		//GetDlgItem(IDC_STATIC_POINT)->SetWindowText(strText);
		s_uOldReceiveImageDataFrameCount = uFrameCount;
	}

	CDialogEx::OnTimer(nIDEvent);
}
