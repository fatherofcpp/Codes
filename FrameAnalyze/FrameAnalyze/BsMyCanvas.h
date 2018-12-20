#pragma once

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

class CMyCanvas : public CStatic
{
public:
	CMyCanvas();
	virtual ~CMyCanvas();
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	void Clear()
	{
		m_nWidth = m_nHeight = 0;
		if (m_lpBufferDst != NULL)
			delete[] m_lpBufferDst;
		m_lpBufferDst = NULL;
	}
	void InitBuf(int nw,int nh);
	void LoadImge(ImgInfo *buf,unsigned int w,unsigned int h, int channels);
	void SetStdAreaSign(BOOL bDis)
	{
		m_bDisStdArea = bDis;
	}
	void SetStdAreaOrg(char *pszPts,int nptCount,int width,int height,StdAreaLayerType type);
	void SetStdArea();
	void DrawImg(CDC *pdc);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
public:
	static void GetStretchRate(const CRect& stdWnd,int stdWidth,int stdHeight,int adpWidth,int adpHeight,int nXOffset,CRect& stdRect,CRect& adpRect);
public:
	CRect m_ImgRect;
	CRect m_PrevRect;
private:
	BYTE* m_lpBufferDst; //��ѹ���λͼ
	int m_nWidth;	//ͼ���
	int m_nHeight;	//ͼ���
	int m_nChannels;//ͨ���ţ�1�Ҷȣ�3��ɫ
	BOOL m_bDisStdArea;
	CDC * m_pdcStdAreaLayer;
	CDC * m_pdcStdAreaOrg;
	CRect m_rcStdArea;//ͨ�������ıȽ��������Ƿ���� �궨ͼ��
	StdAreaLayerType m_eStdAreaType;
	CPoint * m_pPts;
};

