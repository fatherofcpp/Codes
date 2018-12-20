
#include "stdafx.h"
#include "BsMyCanvas.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMyCanvas
CMyCanvas::CMyCanvas()
{
	// 	pbuf = NULL;
	// 	bflag = false;
	m_lpBufferDst = NULL ;//��ѹ���λͼ
	m_nWidth = 0;	//ͼ���
	m_nHeight = 0;	//ͼ���
	m_nChannels = 1;
	m_pPts = NULL;
	m_pdcStdAreaOrg = NULL;
	m_pdcStdAreaLayer = NULL;
	m_bDisStdArea = FALSE;
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
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CMyCanvas::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC *pdc = &dc;

	ReleaseDC(pdc);
}


// m_lpBufferDst ѭ������ ��Ϣ��Ӧ�����д��� WM_USER + 001
void CMyCanvas::LoadImge(ImgInfo *pbuf,unsigned int w,unsigned int h, int channels)
{
	const int c_buff_size = pbuf->getSize();
	if(m_lpBufferDst == NULL)
	{
		m_lpBufferDst = new BYTE[c_buff_size];
	}	
	else
	{
		delete[] m_lpBufferDst;
		m_lpBufferDst = new BYTE[c_buff_size];
	}
	m_nWidth = w;
	m_nHeight = h;
	memcpy(m_lpBufferDst, pbuf->getBuf(), c_buff_size);
	m_nChannels = channels;
	delete pbuf;
	this->Invalidate();  // ������ڷ��� ���治��ˢ��
}

void CMyCanvas::InitBuf(int nw,int nh)
{
	if(m_lpBufferDst == NULL)
	{
		m_lpBufferDst = new BYTE[nw*nh];
	}	
}

void CMyCanvas::GetStretchRate(const CRect& stdWnd,int stdWidth,int stdHeight,int adpWidth,int adpHeight,int nXOffset,CRect& stdRect,CRect& adpRect)
{
	FLOAT fXStretch	= 0.0f;
	FLOAT fYStretch	= 0.0f;
	FLOAT strech = 0.0f;
	fXStretch = (FLOAT) (stdWnd.right-stdWnd.left) / stdWidth;
	fYStretch = (FLOAT) (stdWnd.bottom-stdWnd.top) / stdHeight;
	strech = fXStretch > fYStretch?fYStretch:fXStretch;
	/*((rcWnd.right-rcWnd.left) - m_nWidth * strech)/2,	//	Ŀ��λ�ú�����
	((rcWnd.bottom-rcWnd.top) - m_nHeight * strech)/2,//	Ŀ��λ��������
	m_nWidth * strech + 0.5,						//	Ŀ����
	m_nHeight * strech + 0.5,						//	Ŀ��߶�*/
	stdRect.left = (LONG)(((stdWnd.right-stdWnd.left) - stdWidth * strech)/2);	//	Ŀ��λ�ú�����
	stdRect.top = (LONG)(((stdWnd.bottom-stdWnd.top) - stdHeight * strech)/2);	//	Ŀ��λ��������
	stdRect.right = stdRect.left + (int) (stdWidth * strech + 0.5);		//	Ŀ����
	stdRect.bottom = stdRect.top + (int) (stdHeight * strech + 0.5);	//	Ŀ��߶�
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
	//����������	
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
	//���¹���ԭʼ�궨ͼ��
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
		//���߽�
		CPen rpen(PS_SOLID,16,RGB(255,0,0));
		m_pdcStdAreaOrg->SelectObject(&rpen);
		m_pdcStdAreaOrg->Polyline(m_pPts,nptCount);
		CRect rc;
		rc.SetRect(m_pPts[1],m_pPts[3]);
		//������
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
				//���м����
				m_pdcStdAreaOrg->SelectObject(&midPen);
				m_pdcStdAreaOrg->LineTo(x,rc.bottom);
				m_pdcStdAreaOrg->SelectObject(&dashPen);
			}
			else
			{
				//��������
				m_pdcStdAreaOrg->LineTo(x,rc.bottom-yOffset);
			}
		}
	}
	m_pdcStdAreaOrg->SelectObject(pOldPen);
	ReleaseDC(pdc);
}

void CMyCanvas::SetStdArea()
{
	//��������Ƿ�仯
	if(m_ImgRect != m_rcStdArea)
	{
		m_rcStdArea = m_ImgRect;
		if(m_pdcStdAreaOrg == NULL)
		{
			return;			
		}
		CDC *pdc = GetDC();
		//�����µĳߴ�����ͼ��
		if(m_pdcStdAreaLayer == NULL)
		{
			m_pdcStdAreaLayer = new CDC();
			m_pdcStdAreaLayer->CreateCompatibleDC(pdc);
		}
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pdc,m_ImgRect.Width(),m_ImgRect.Height());
		m_pdcStdAreaLayer->SelectObject(&bmp);
		m_pdcStdAreaLayer->StretchBlt(0,0,m_ImgRect.Width(),m_ImgRect.Height(),m_pdcStdAreaOrg,0,0,m_nWidth,m_nHeight,SRCCOPY);
		ReleaseDC(pdc);
	}
}

BOOL CMyCanvas::OnEraseBkgnd(CDC* pDC)
{
	DrawImg(pDC);
	ReleaseDC(pDC);
	return TRUE;//CStatic::OnEraseBkgnd(pDC);
}


void CMyCanvas::DrawImg(CDC *pdc)
{
	CRect rcWnd;
	this->GetClientRect(&rcWnd); 
	//this->GetWindowRect(&rcWnd); 
	//����ͼƬ��ѹ����ʾ
	if (m_nHeight == 0 || m_nWidth == 0 || m_lpBufferDst == NULL) // ���� edit �߿�
	{
		return;

		CBitmap bmp;
		bmp.LoadBitmap(IDB_BITMAP1);
		BITMAP bmpInfo;  
		bmp.GetBitmap(&bmpInfo);  

		//��������dc
		CDC dcMem;  
		dcMem.CreateCompatibleDC(pdc); 

		//������ʾ���ݵ�λͼ
		CBitmap bmpBg;  
		bmpBg.CreateCompatibleBitmap(pdc,rcWnd.Width(),rcWnd.Height());  

		CBitmap* pOldMemBmp = dcMem.SelectObject(&bmpBg);  

		//�ڼ���dc�ϻ�ͼ
		CDC dcDraw;  
		dcDraw.CreateCompatibleDC(pdc);
		CBitmap* pOldDrawBmp = dcDraw.SelectObject(&bmp);  

		dcMem.StretchBlt(0,0,rcWnd.Width(),rcWnd.Height(),&dcDraw,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY); 

		//������dc������������Ļ��
		pdc->BitBlt(0,0,rcWnd.Width(),rcWnd.Height(),&dcMem,0,0,SRCCOPY); 
		// 		CPen pen(PS_SOLID,3,RGB(255,0,0));
		// 		pdc->SelectObject(&pen);
		// 		pdc->MoveTo(0,0);
		// 		pdc->LineTo(rcWnd.Width(),0);

		//����dc��ͼ
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
		//24λλͼ
		/*		BITMAPINFO bitmapInfo = { 0 };
		bitmapInfo.bmiHeader.biBitCount		= 24;	//24λɫ		
		*/
		struct 
		{ 
			BITMAPINFOHEADER   bmiHeader; 
			RGBQUAD   quad[256]; 
		}bitmapInfo; 
		memset(&bitmapInfo, 0, sizeof(bitmapInfo));
		bitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		bitmapInfo.bmiHeader.biWidth		= m_nWidth;
		bitmapInfo.bmiHeader.biHeight		= -m_nHeight;
		bitmapInfo.bmiHeader.biCompression	= BI_RGB;//BI_JPEG;
		bitmapInfo.bmiHeader.biPlanes		= 1;
		bitmapInfo.bmiHeader.biBitCount		= 8 * m_nChannels;
		for   (int i=0;   i <256;   i++) 
		{ 
			bitmapInfo.quad[i].rgbRed   =   BYTE(i); 
			bitmapInfo.quad[i].rgbBlue   =   BYTE(i); 
			bitmapInfo.quad[i].rgbGreen   =   BYTE(i); 
			bitmapInfo.quad[i].rgbReserved   =   BYTE(0); 
		}

		//��������dc
		CDC dcMem;  
		dcMem.CreateCompatibleDC(pdc); 

		CBitmap bmpBg;  
		bmpBg.CreateCompatibleBitmap(pdc,rcWnd.Width(),rcWnd.Height()); 

		CBitmap* pOldMemBmp = dcMem.SelectObject(&bmpBg); 

		CBrush bsh(RGB(255,255,255));
		dcMem.FillRect(rcWnd,&bsh);

		bool is_adjust_dst_rect = true;
		if (is_adjust_dst_rect)
		{
			int dst_width = m_nWidth > m_ImgRect.Width() ? m_ImgRect.Width() : m_nWidth;
			int dst_height = m_nHeight > m_ImgRect.Height() ? m_ImgRect.Height() : m_nHeight;
			if( m_nWidth > m_ImgRect.Width() && m_nHeight > m_ImgRect.Height())
			{
				//��С����С
				SetStretchBltMode(
					dcMem.GetSafeHdc(),           // handle todevice context
					HALFTONE );
			}
			else
			{
				SetStretchBltMode(
					dcMem.GetSafeHdc(),           // handle todevice context
					COLORONCOLOR );
			}
			StretchDIBits(dcMem.GetSafeHdc(), 
				m_ImgRect.left,
				m_ImgRect.top,
				dst_width,
				dst_height,
				0, 0,											//	ԭʼλ��
				m_nWidth, m_nHeight,								//	ԭʼ��С, 
				m_lpBufferDst, 
				(PBITMAPINFO)&bitmapInfo,										//dibλͼ��Ϣ
				DIB_RGB_COLORS, 
				SRCCOPY);
		}
		else
		{
			SetStretchBltMode(dcMem.GetSafeHdc(), COLORONCOLOR);//
			StretchDIBits(dcMem.GetSafeHdc(), 
				m_ImgRect.left,
				m_ImgRect.top,
				m_ImgRect.Width(),
				m_ImgRect.Height(),
				0, 0,											//	ԭʼλ��
				m_nWidth, m_nHeight,								//	ԭʼ��С, 
				m_lpBufferDst, 
				(PBITMAPINFO)&bitmapInfo,										//dibλͼ��Ϣ
				DIB_RGB_COLORS, 
				SRCCOPY);
		}

		/* SetTextColor
		* ���g �� b���ṩ��ֵ��������ܾ���(��ɫ��ɫ��)��
		���û���ṩ�κ�������g �� b��������ܾ���(�ҽ׵�ɫ��). ��ֵΪ 0 �� 255 ֮�䡣
		*/
		// 		dcMem.SetTextColor(RGB(250, 0, 0)); 
		// 		dcMem.SetBkMode(TRANSPARENT);
		//���Ʊ궨�߼�
		if(m_bDisStdArea)
		{
			dcMem.BitBlt(m_ImgRect.left,m_ImgRect.top,m_ImgRect.Width(),m_ImgRect.Height(),m_pdcStdAreaLayer,0,0,SRCPAINT);
		}
		pdc->BitBlt(0,0,rcWnd.Width(),rcWnd.Height(),&dcMem,0,0,SRCCOPY); 
		dcMem.SelectObject(pOldMemBmp);

	}
}

void CMyCanvas::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect r;//,rc,w;
	CDC* pdc = CDC::FromHandle(lpDrawItemStruct->hDC);
	this->GetWindowRect(&r);
	pdc->SetWindowOrg(-50,0);
	pdc->SetViewportOrg(0,0);
	DrawImg(pdc);
	pdc->Detach();
}
