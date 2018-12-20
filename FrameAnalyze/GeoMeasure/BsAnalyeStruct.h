#pragma once

#include <deque>

struct _IplImage;
struct CvMat;
class CAnalyeStruct
{
public:
	CAnalyeStruct(void);
	~CAnalyeStruct(void);
public:
	std::string analyze_struct(std::string file_path);

private:
	bool m_isFirst ;
	_IplImage* m_pFrame;
	_IplImage* m_pFrameGray ;
	CvMat* m_pFrameGrayMat;
	CvMat* m_pFrameNobkMat;
	CvMat* m_pFrameNobkMatFirst ;
	CvMat* m_pFrameNobkMatSecond ;
	CvMat* m_pFrameNobkMatThird ;
	CvMat* m_pFrameNobkMatBinary ;
	CvMat* m_pBkMat ;
	CvMat* m_pDiffNobkMat ;
	CvMat* m_pDiffNobkMatBinary ;
	int m_frameStep ; 
};

