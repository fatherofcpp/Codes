#if !defined(AFX_MYCANVAS_H__8FB95786_B924_4A6D_ABC2_DE8BE0727220__INCLUDED_)
#define AFX_MYCANVAS_H__8FB95786_B924_4A6D_ABC2_DE8BE0727220__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCanvas.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyCanvas window


class ImgInfo
{
public:
	ImgInfo()
		:size(0)
		,buf(NULL)
		,m_bAutoDeleteBuf(true)
	{
	}
	ImgInfo(bool bAutoDel)
		:size(0)
		,buf(NULL)
		,m_bAutoDeleteBuf(bAutoDel)
	{
	}
	~ImgInfo(){
		if (buf != NULL && m_bAutoDeleteBuf)
			delete [] buf;
	}
	void setBuf(unsigned char *p)
	{
		if (buf != NULL && m_bAutoDeleteBuf)
			delete [] buf;
		buf = p;
	}
	unsigned char *getBuf()
	{
		return buf;
	}
	
	unsigned int getSize()const
	{
		return size;
	}
	void setSize(unsigned int n)
	{
		size = n;
	}

	unsigned int getWidth()const
	{
		return mwidth;
	}
	void setWidth(unsigned int w)
	{
		mwidth = w;
	}

	unsigned getHeight()const
	{
		return mheight;
	}
	void setHeight(unsigned int h)
	{
		mheight = h;
	}
	
private:
	int				size;
	unsigned char*	buf;
	unsigned int	mwidth;
	unsigned int	mheight;
	bool			m_bAutoDeleteBuf;	

};

enum StdAreaLayerType
{
	SATT_WHOLE_POLE,
	SATT_MIDDLE_POLENUM
};

typedef void (*OnPaintCompletedCallback)(WPARAM wParam,LPARAM lParam);

class CMyCanvas : public CStatic
{
// Construction
public:
	CMyCanvas();

// Attributes
	CRect ImgRect;
	CRect PrevRect;
public:

// Operations
	static void GetStretchRate(const CRect& stdWnd,int stdWidth,int stdHeight,int adpWidth,int adpHeight,int nXOffset,CRect& stdRect,CRect& adpRect);
public:
	OnPaintCompletedCallback pfunPaintCompleted;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCanvas)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
// Implementation
public:
	virtual ~CMyCanvas();
	void Clear()
	{
		m_nWidth = m_nHeight = 0;
		if (m_lpBufferDst != NULL)
			delete[] m_lpBufferDst;
		m_lpBufferDst = NULL;
	}

	void InitBuf(int nw,int nh);
	void LoadImge(unsigned char *Buf,unsigned int w,unsigned int h);
	void LoadImge(ImgInfo *buf);
	void LoadImge(ImgInfo *buf,unsigned int w,unsigned int h);
	void SetStdAreaSign(BOOL bDis)
	{
		m_bDisStdArea = bDis;
	}
	//
	void SetStdAreaOrg(char *pszPts,int nptCount,int width,int height,StdAreaLayerType type);
	void SetStdArea();
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyCanvas)
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
      
// 	ImgInfo *pbuf;
// 	bool bflag;
	BYTE* m_lpBufferDst; //解压后的位图
	int m_nWidth;	//图像宽
	int m_nHeight;	//图像高
	BOOL m_bDisStdArea;
	CDC * m_pdcStdAreaLayer;
	CDC * m_pdcStdAreaOrg;
	CRect m_rcStdArea;//通过对它的比较来决定是否更新 标定图层
	StdAreaLayerType m_eStdAreaType;
	CPoint * m_pPts;

	DECLARE_MESSAGE_MAP()
protected:
	
public:
	void DrawImg(CDC *pdc);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnStnDblclick();
//	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCANVAS_H__8FB95786_B924_4A6D_ABC2_DE8BE0727220__INCLUDED_)
