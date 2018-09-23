// MyCanvas.cpp : implementation file
//
#include "stdafx.h"
#include "MyCanvas.h"
#include "ijl/ijlutil.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CMyCanvas
CMyCanvas::CMyCanvas()
{
	// 	pbuf = NULL;
	// 	bflag = false;
	m_lpBufferDst = NULL ;//解压后的位图
	m_nWidth = 0;	//图像宽
	m_nHeight = 0;	//图像高
	m_pPts = NULL;
	m_pdcStdAreaOrg = NULL;
	m_pdcStdAreaLayer = NULL;
	m_bDisStdArea = FALSE;
	pfunPaintCompleted = NULL;
}
CMyCanvas::~CMyCanvas()
{
	if(m_pPts != NULL)
	{
		delete [] m_pPts;
	}
	if(m_pdcStdAreaOrg != NULL)
	{
		m_pdcStdAreaOrg->DeleteDC();
		delete m_pdcStdAreaOrg;
	}
	if(m_pdcStdAreaLayer != NULL)
	{
		m_pdcStdAreaLayer->DeleteDC();
		delete m_pdcStdAreaLayer;
	}
	
	Clear();
}
BEGIN_MESSAGE_MAP(CMyCanvas, CStatic)
	//{{AFX_MSG_MAP(CMyCanvas)
	//	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
	//	ON_WM_LBUTTONDBLCLK()
	//	ON_CONTROL_REFLECT(STN_DBLCLK, &CMyCanvas::OnStnDblclick)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMyCanvas message handlers
//BOOL CMyCanvas::OnEraseBkgnd(CDC* pDC) 
//{
//	// TODO: Add your message handler code here and/or call default
//	//防止背景闪烁	
//	return true;
//}
void CMyCanvas::OnPaint() 
{
	//gl_log.TraceMessage("MyCanvers-Onpaint");
	CPaintDC dc(this); // device context for painting
	CDC *pdc = &dc;
	// TODO: Add your message handler code here
	// 自绘控件
	//CDC *pdc = GetDC();
	//DrawImg(pdc);
	ReleaseDC(pdc);
	// Do not call CStatic::OnPaint() for painting messages
}
void CMyCanvas::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	//设置控件自绘
	//	DWORD dwStyle = this->GetStyle();
	//	::SetWindowLong(this->GetSafeHwnd(),GWL_STYLE,dwStyle|SS_OWNERDRAW);
	//ModifyStyle(0,SS_OWNERDRAW);	
	CStatic::PreSubclassWindow();
}

// m_lpBufferDst 循环利用 消息响应函数中处理 WM_USER + 001
void CMyCanvas::LoadImge(ImgInfo *pbuf,unsigned int w,unsigned int h)
{
	//unsigned char *tmp = m_lpBufferDst;
	if(m_lpBufferDst == NULL)
	{
		m_lpBufferDst = new BYTE[w*h];
	}	
	m_nWidth = w;
	m_nHeight = h;
	memcpy(m_lpBufferDst,pbuf->getBuf(),w*h);
	//if (tmp != NULL)
	//delete[] tmp;
	delete pbuf;
	this->Invalidate();  // 必须存在否者 界面不会刷新
	//this->UpdateWindow();
}

void CMyCanvas::InitBuf(int nw,int nh)
{
	if(m_lpBufferDst == NULL)
	{
		m_lpBufferDst = new BYTE[nw*nh];
	}	
}

// 在 dlg 的定时器中触发
void CMyCanvas::LoadImge(ImgInfo *buf)
{
	//图像压缩
	if (buf != NULL)
	{
		BYTE *tmp = NULL;
		int outlen = 0;
		//		CLogMessage(LOG_INFO, "正在解压图像\n");
		CString strfmt;
		strfmt.Format(_T("%d_\n"),buf->getSize());
		//		CIJLUtil ijlutil;
		CIJLUtil iJLUtil;
		if (iJLUtil.DeCompressImg(&tmp,buf->getBuf(),buf->getSize(),&m_nWidth,&m_nHeight,&outlen) < 0)
		{
			delete buf;
			return;
		}
		//		CLogMessage(LOG_INFO, "解压图像成功\n");
		//删除上一次
		if (m_lpBufferDst != NULL)
		{
			delete[] m_lpBufferDst;
			m_lpBufferDst = NULL;
		}
		m_lpBufferDst = tmp;

		delete buf;
	}
	//图像显示
	this->RedrawWindow();
}

void CMyCanvas::GetStretchRate(const CRect& stdWnd,int stdWidth,int stdHeight,int adpWidth,int adpHeight,int nXOffset,CRect& stdRect,CRect& adpRect)
{
	FLOAT fXStretch	= 0.0f;
	FLOAT fYStretch	= 0.0f;
	FLOAT strech = 0.0f;
	fXStretch = (FLOAT) (stdWnd.right-stdWnd.left) / stdWidth;
	fYStretch = (FLOAT) (stdWnd.bottom-stdWnd.top) / stdHeight;
	strech = fXStretch > fYStretch?fYStretch:fXStretch;
	/*((rcWnd.right-rcWnd.left) - m_nWidth * strech)/2,	//	目标位置横坐标
	((rcWnd.bottom-rcWnd.top) - m_nHeight * strech)/2,//	目标位置纵坐标
	m_nWidth * strech + 0.5,						//	目标宽度
	m_nHeight * strech + 0.5,						//	目标高度*/
	stdRect.left = (LONG)(((stdWnd.right-stdWnd.left) - stdWidth * strech)/2);	//	目标位置横坐标
	stdRect.top = (LONG)(((stdWnd.bottom-stdWnd.top) - stdHeight * strech)/2);	//	目标位置纵坐标
	stdRect.right = stdRect.left + (int) (stdWidth * strech + 0.5);		//	目标宽度
	stdRect.bottom = stdRect.top + (int) (stdHeight * strech + 0.5);	//	目标高度
	adpRect = stdRect;
	/*adpRect.left = stdRect.left + nXOffset;
	adpRect.top = stdRect.top;
	adpRect.right += stdRect.Width();
	adpRect.bottom = stdRect.bottom;*/
}

void CMyCanvas::SetStdAreaOrg(char *pszPts,int nptCount,int width,int height,StdAreaLayerType type)
{
	m_eStdAreaType = type;
	m_nWidth = width;
	m_nHeight = height;
	//解析点数组	
	if(m_pPts != NULL)
	{
		delete [] m_pPts;
	}
	m_pPts = new CPoint[nptCount];
	int x,y;
	char * szstr = pszPts;
	for(int i = 0;i<nptCount;i++)
	{
		sscanf_s(szstr,"{%d,%d}",&x,&y);
		szstr = strchr(++szstr,'{');
		m_pPts[i].x = x;
		m_pPts[i].y = y;
	}
	CDC *pdc = GetDC();
	//重新构造原始标定图层
	if(m_pdcStdAreaOrg != NULL)
	{
		m_pdcStdAreaOrg->DeleteDC();
	}
	else
	{
		m_pdcStdAreaOrg = new CDC();
	}
	m_pdcStdAreaOrg->CreateCompatibleDC(pdc);
	CBitmap bmpArea;
	bmpArea.CreateCompatibleBitmap(pdc,width,height);
	m_pdcStdAreaOrg->SelectObject(&bmpArea);
	CPen redpen(PS_SOLID,20,RGB(255,0,0));
	CPen * pOldPen = m_pdcStdAreaOrg->SelectObject(&redpen);
	if(type == SATT_WHOLE_POLE)
	{
		//CBrush redbrush(RGB(255,0,0));
		//m_pdcStdAreaOrg->SelectObject(&redbrush);
		//m_pdcStdAreaOrg->BeginPath();
		m_pdcStdAreaOrg->Polyline(m_pPts,nptCount);
		//m_pdcStdAreaOrg->EndPath();
		//m_pdcStdAreaOrg->SelectObject(&redbrush);
		//m_pdcStdAreaOrg->FillPath();
	}
	else if(type == SATT_MIDDLE_POLENUM)
	{
		//画边界
		CPen rpen(PS_SOLID,16,RGB(255,0,0));
		m_pdcStdAreaOrg->SelectObject(&rpen);
		m_pdcStdAreaOrg->Polyline(m_pPts,nptCount);
		CRect rc;
		rc.SetRect(m_pPts[1],m_pPts[3]);
		//画虚线
		LOGBRUSH logbrush;
		logbrush.lbStyle=BS_SOLID;
		logbrush.lbColor=RGB(255,255,0);
		CPen midPen;
		midPen.CreatePen(PS_DOT|PS_GEOMETRIC,16,&logbrush);
		CPen dashPen;
		dashPen.CreatePen(PS_DOT|PS_GEOMETRIC,12,&logbrush);
		m_pdcStdAreaOrg->SelectObject(&dashPen);
		int nGap = rc.Width()/10;
		int x,yOffset=8;
		for(int i = 0; i<9;i++)
		{
			x=(i+1)*nGap;
			m_pdcStdAreaOrg->MoveTo(x,rc.top + yOffset);
			if(i==4)
			{
				//画中间的线
				m_pdcStdAreaOrg->SelectObject(&midPen);
				m_pdcStdAreaOrg->LineTo(x,rc.bottom);
				m_pdcStdAreaOrg->SelectObject(&dashPen);
			}
			else
			{
				//画其他线
				m_pdcStdAreaOrg->LineTo(x,rc.bottom-yOffset);
			}
		}
	}
	m_pdcStdAreaOrg->SelectObject(pOldPen);
	ReleaseDC(pdc);
}

void CMyCanvas::SetStdArea()
{
	//检查区域是否变化
	if(ImgRect != m_rcStdArea)
	{
		m_rcStdArea = ImgRect;
		if(m_pdcStdAreaOrg == NULL)
		{
			return;			
		}
		CDC *pdc = GetDC();
		//按照新的尺寸缩放图层
		if(m_pdcStdAreaLayer == NULL)
		{
			m_pdcStdAreaLayer = new CDC();
			m_pdcStdAreaLayer->CreateCompatibleDC(pdc);
		}
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pdc,ImgRect.Width(),ImgRect.Height());
		m_pdcStdAreaLayer->SelectObject(&bmp);
		m_pdcStdAreaLayer->StretchBlt(0,0,ImgRect.Width(),ImgRect.Height(),m_pdcStdAreaOrg,0,0,m_nWidth,m_nHeight,SRCCOPY);
		ReleaseDC(pdc);
	}
}

BOOL CMyCanvas::OnEraseBkgnd(CDC* pDC)
{
	// 	CRect r,w;
	// 	this->GetClientRect(&r);
	//gl_log.TraceMessage("MyCanvers-OnEraseBkgnd(%d,%d,%d,%d)",r.left,r.top,r.right,r.bottom);
	DrawImg(pDC);
	ReleaseDC(pDC);
	return TRUE;//CStatic::OnEraseBkgnd(pDC);
}

void CMyCanvas::DrawImg(CDC *pdc)
{
	CRect rcWnd;
	this->GetClientRect(&rcWnd); 
	//this->GetWindowRect(&rcWnd); 
	//加载图片并压缩显示
	if (m_nHeight == 0 || m_nWidth == 0 || m_lpBufferDst == NULL) // 画出 edit 边框
	{
		return;

		CBitmap bmp;
		//bmp.LoadBitmap(IDB_BITMAP1);
		BITMAP bmpInfo;  
		bmp.GetBitmap(&bmpInfo);  
		//建立兼容dc
		CDC dcMem;  
		dcMem.CreateCompatibleDC(pdc); 
		//建立显示兼容的位图
		CBitmap bmpBg;  
		bmpBg.CreateCompatibleBitmap(pdc,rcWnd.Width(),rcWnd.Height());  
		CBitmap* pOldMemBmp = dcMem.SelectObject(&bmpBg);  
		//在兼容dc上画图
		CDC dcDraw;  
		dcDraw.CreateCompatibleDC(pdc);
		CBitmap* pOldDrawBmp = dcDraw.SelectObject(&bmp);  
		dcMem.StretchBlt(0,0,rcWnd.Width(),rcWnd.Height(),&dcDraw,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);  
		//将兼容dc的内容贴到屏幕上
		pdc->BitBlt(0,0,rcWnd.Width(),rcWnd.Height(),&dcMem,0,0,SRCCOPY); 
		// 		CPen pen(PS_SOLID,3,RGB(255,0,0));
		// 		pdc->SelectObject(&pen);
		// 		pdc->MoveTo(0,0);
		// 		pdc->LineTo(rcWnd.Width(),0);
		//销毁dc及图
		dcMem.SelectObject(pOldMemBmp);
		dcDraw.SelectObject(pOldDrawBmp);
		//bmp.DeleteObject();
		//bmpBg.DeleteObject();
		//dcDraw.DeleteDC();
		//dcMem.DeleteDC();
		ReleaseDC(&dcDraw);
	}
	else
	{
		//24位位图
		/*		BITMAPINFO bitmapInfo = { 0 };
		bitmapInfo.bmiHeader.biBitCount		= 24;	//24位色		
		*/
		struct 
		{ 
			BITMAPINFOHEADER   bmiHeader; 
			RGBQUAD   quad[256]; 
		}bitmapInfo; 
		bitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biWidth		= m_nWidth;
		bitmapInfo.bmiHeader.biHeight		= -m_nHeight;
		bitmapInfo.bmiHeader.biCompression	= BI_RGB;//BI_JPEG;
		bitmapInfo.bmiHeader.biPlanes		= 1;
		bitmapInfo.bmiHeader.biBitCount		= 8;
		for   (int i=0;   i <256;   i++) 
		{ 
			bitmapInfo.quad[i].rgbRed   =   BYTE(i); 
			bitmapInfo.quad[i].rgbBlue   =   BYTE(i); 
			bitmapInfo.quad[i].rgbGreen   =   BYTE(i); 
			bitmapInfo.quad[i].rgbReserved   =   BYTE(0); 
		}
		/*FLOAT fXStretch	= 0.0f;
		FLOAT fYStretch	= 0.0f;
		FLOAT strech = 0.0f;
		fXStretch = (FLOAT) (rcWnd.right-rcWnd.left) / m_nWidth;
		fYStretch = (FLOAT) (rcWnd.bottom-rcWnd.top) / m_nHeight;
		strech = fXStretch > fYStretch?fYStretch:fXStretch;*/
		//建立兼容dc
		CDC dcMem;  
		dcMem.CreateCompatibleDC(pdc); 
		CBitmap bmpBg;  
		bmpBg.CreateCompatibleBitmap(pdc,rcWnd.Width(),rcWnd.Height());  
		CBitmap* pOldMemBmp = dcMem.SelectObject(&bmpBg); 
		CBrush bsh(RGB(255,255,255));
		dcMem.FillRect(rcWnd,&bsh);
		SetStretchBltMode(dcMem.GetSafeHdc(), COLORONCOLOR);//
		if(0 == StretchDIBits(dcMem.GetSafeHdc(), 
			/*((rcWnd.right-rcWnd.left) - m_nWidth * strech)/2,	//	目标位置横坐标
			((rcWnd.bottom-rcWnd.top) - m_nHeight * strech)/2,//	目标位置纵坐标
			m_nWidth * strech + 0.5,						//	目标宽度
			m_nHeight * strech + 0.5,						//	目标高度*/
			ImgRect.left,
			ImgRect.top,
			ImgRect.Width(),
			ImgRect.Height(),
			0, 0,											//	原始位置
			m_nWidth, m_nHeight,								//	原始大小, 
			m_lpBufferDst, 
			(PBITMAPINFO)&bitmapInfo,										//dib位图信息
			DIB_RGB_COLORS, 
			SRCCOPY))
		{
		}
		/* SetTextColor
		* 如果g 和 b有提供数值，这个功能就是(红色的色彩)。
		如果没有提供任何数据在g 和 b，这个功能就是(灰阶的色彩). 数值为 0 至 255 之间。
		*/
		// 		dcMem.SetTextColor(RGB(250, 0, 0)); 
		// 		dcMem.SetBkMode(TRANSPARENT);
		//绘制标定逻辑
		if(m_bDisStdArea)
		{
			dcMem.BitBlt(ImgRect.left,ImgRect.top,ImgRect.Width(),ImgRect.Height(),m_pdcStdAreaLayer,0,0,SRCPAINT);
		}
		pdc->BitBlt(0,0,rcWnd.Width(),rcWnd.Height(),&dcMem,0,0,SRCCOPY); 
		dcMem.SelectObject(pOldMemBmp);
		// 		bmpBg.DeleteObject();
		// 		dcMem.DeleteDC();
		//回调
		if(pfunPaintCompleted)
		{
			(*pfunPaintCompleted)((WPARAM)0,(LPARAM)0);
		}
	}
}

void CMyCanvas::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect r;//,rc,w;
	// 	this->GetClientRect(&r);
	// 	rc = lpDrawItemStruct->rcItem;
	// 	this->GetWindowRect(&w);
	// 	gl_log.TraceMessage("DrawItem(%d,%d,%d,%d) rcItem(%d,%d,%d,%d) window(%d,%d,%d,%d)",r.left,r.top,r.right,r.bottom,rc.left,rc.top,rc.right,rc.bottom,
	// 		w.left,w.top,w.right,w.bottom);
	CDC* pdc = CDC::FromHandle(lpDrawItemStruct->hDC);
	this->GetWindowRect(&r);
	pdc->SetWindowOrg(-50,0);
	pdc->SetViewportOrg(0,0);
	DrawImg(pdc);
	pdc->Detach();
}

BOOL CMyCanvas::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类	
	//gl_log.TraceMessage("Style:%d",cs.style);
	return CStatic::PreCreateWindow(cs);
}
