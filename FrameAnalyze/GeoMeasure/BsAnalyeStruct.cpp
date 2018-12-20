#include "StdAfx.h"
#include "BsAnalyeStruct.h"
#include "CmOpenCvDll.h"
#include "Log/CmLog.h"

#include <opencv/cv.hpp>  

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp> 

using namespace cv;  

typedef enum 
{
	E_FRAME_FIRST = 0,
	E_FRAME_SECOND,
	E_FRAME_THIRD,
};

CAnalyeStruct::CAnalyeStruct(void)
{
	m_isFirst = true;

	m_frameStep = E_FRAME_FIRST; 

	m_pFrame = NULL;

	m_pFrameGray = NULL;

	m_pFrameGrayMat = NULL;

	m_pFrameNobkMat = NULL;

	m_pFrameNobkMatFirst = NULL;

	m_pFrameNobkMatSecond = NULL;

	m_pFrameNobkMatThird = NULL;

	m_pFrameNobkMatBinary = NULL;

	m_pBkMat = NULL;

	m_pDiffNobkMat = NULL;

	m_pDiffNobkMatBinary = NULL;
}


CAnalyeStruct::~CAnalyeStruct(void)
{
	cvReleaseImage(&m_pFrame);

	cvReleaseImage(&m_pFrameGray);

	cvReleaseMat(&m_pFrameGrayMat);

	cvReleaseMat(&m_pFrameNobkMat);

	cvReleaseMat(&m_pFrameNobkMatFirst);

	cvReleaseMat(&m_pFrameNobkMatSecond);

	cvReleaseMat(&m_pFrameNobkMatThird);

	cvReleaseMat(&m_pFrameNobkMatBinary);

	cvReleaseMat(&m_pDiffNobkMat);

	cvReleaseMat(&m_pDiffNobkMatBinary);

	cvReleaseMat(&m_pBkMat);
}

std::string CAnalyeStruct::analyze_struct( std::string file_path )
{
	const int c_frame_nobk_mat_binary_normal_max_count = 10000;//异物像素数
	const int c_diff_nobk_mat_binary_continue_max_count = 3000;//桢连续的最大差异像素数
	const double c_thresh_frame_nobk_mat_binary = 150;//150;//当前前景图矩阵阀值

	int warning_num = 0;      

	IplImage* src = cvLoadImage( file_path.c_str(), CV_LOAD_IMAGE_COLOR);
	if (!src)
	{
		return "file_not_exist";
	}
	if (m_isFirst)
	{
		m_isFirst = false;

		m_pFrame = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, src->nChannels);   
		cvSmooth(src, m_pFrame, CV_GAUSSIAN, 7, src->nChannels);
		m_pFrameGray = cvCreateImage(cvSize(m_pFrame->width, m_pFrame->height), IPL_DEPTH_8U,1);
		m_pBkMat = cvCreateMat(m_pFrame->height, m_pFrame->width, CV_32FC1);
		m_pDiffNobkMat = cvCreateMat(m_pFrame->height, m_pFrame->width, CV_32FC1);
		m_pFrameNobkMat = cvCreateMat(m_pFrame->height, m_pFrame->width, CV_32FC1);
		m_pFrameNobkMatBinary = cvCreateMat(m_pFrame->height, m_pFrame->width, CV_8UC1);
		m_pDiffNobkMatBinary = cvCreateMat(m_pFrame->height, m_pFrame->width, CV_8UC1);
		m_pFrameGrayMat = cvCreateMat(m_pFrame->height, m_pFrame->width, CV_32FC1);
		cvZero(m_pDiffNobkMat);
		cvConvert(m_pFrameGray, m_pBkMat);
	}
	else
	{
		cvSmooth(src, m_pFrame, CV_GAUSSIAN, 7, src->nChannels);
		cvCvtColor(m_pFrame, m_pFrameGray, CV_BGR2GRAY);
		cvConvert(m_pFrameGray, m_pFrameGrayMat);
		cvAbsDiff(m_pFrameGrayMat, m_pBkMat, m_pFrameNobkMat);
		cvThreshold(m_pFrameNobkMat, m_pFrameNobkMatBinary, c_thresh_frame_nobk_mat_binary, 1, CV_THRESH_BINARY);

		const int c_frame_nobk_mat_binary = cvCountNonZero(m_pFrameNobkMatBinary);
		gLog.Add("%s c_fr_matb=%d,c_thresh_pFrMat=%.3f,file_path=%s\n", __FUNCTION__, c_frame_nobk_mat_binary, c_thresh_frame_nobk_mat_binary, file_path.c_str());

		if (c_frame_nobk_mat_binary > c_frame_nobk_mat_binary_normal_max_count && m_frameStep == E_FRAME_FIRST) 
		{
			cvReleaseMat(&m_pFrameNobkMatFirst);
			m_pFrameNobkMatFirst = cvCloneMat(m_pFrameNobkMat);
			m_frameStep = E_FRAME_SECOND;    
			gLog.Add("%s E_FRAME_FIRTST to E_FRAME_SECOND\n", __FUNCTION__);
		}
		else if (c_frame_nobk_mat_binary < c_frame_nobk_mat_binary_normal_max_count && m_frameStep == E_FRAME_SECOND)
		{
			gLog.Add("%s s_status=%d, NO.%d warning!!!!\n", __FUNCTION__, m_frameStep, warning_num++);
			m_frameStep = E_FRAME_FIRST;
		}
		else if (c_frame_nobk_mat_binary > c_frame_nobk_mat_binary_normal_max_count && m_frameStep == E_FRAME_SECOND)
		{
			cvReleaseMat(&m_pFrameNobkMatSecond);
			m_pFrameNobkMatSecond = cvCloneMat(m_pFrameNobkMat);
			cvAbsDiff(m_pFrameNobkMatFirst, m_pFrameNobkMatSecond, m_pDiffNobkMat);
			const double c_thresh_pZeroMat = 20;
			cvThreshold(m_pDiffNobkMat, m_pDiffNobkMatBinary, c_thresh_pZeroMat, 1, CV_THRESH_BINARY);
			const int c_diff_nobk_mat_binary = cvCountNonZero(m_pDiffNobkMatBinary);
			if (c_diff_nobk_mat_binary > c_diff_nobk_mat_binary_continue_max_count ) 
			{
				gLog.Add("%s s_status=%d, NO.%d warning!!!!\n", __FUNCTION__, m_frameStep, warning_num++);
				m_frameStep = E_FRAME_FIRST;
			}
			else
			{
				m_frameStep = E_FRAME_THIRD;                 
			}
		}
		else if (c_frame_nobk_mat_binary < c_frame_nobk_mat_binary_normal_max_count && m_frameStep == E_FRAME_THIRD)
		{
			gLog.Add("%s s_status=%d, NO.%d warning!!!!\n", __FUNCTION__, m_frameStep, warning_num++);
			m_frameStep = E_FRAME_FIRST;
		}
		else if (c_frame_nobk_mat_binary > c_frame_nobk_mat_binary_normal_max_count && m_frameStep == E_FRAME_THIRD)
		{
			cvReleaseMat(&m_pFrameNobkMatThird);
			m_pFrameNobkMatThird = cvCloneMat(m_pFrameNobkMat);
			cvAbsDiff(m_pFrameNobkMatSecond, m_pFrameNobkMatThird, m_pDiffNobkMat);
			const double c_thresh_pZeroMat = 20;
			cvThreshold(m_pDiffNobkMat,m_pDiffNobkMatBinary, c_thresh_pZeroMat, 1, CV_THRESH_BINARY);
			const int c_diff_nobk_mat_binary = cvCountNonZero(m_pDiffNobkMatBinary);
			if (c_diff_nobk_mat_binary > c_diff_nobk_mat_binary_continue_max_count ) 
			{
				gLog.Add("%s s_status=%d, NO.%d warning!!!!\n", __FUNCTION__, m_frameStep, warning_num++);
			}
			else 
			{
				cvReleaseMat(&m_pBkMat);
				m_pBkMat = cvCloneMat(m_pFrameGrayMat); 
				//cvConvert(m_pFrameGray, m_pBkMat);
				gLog.Add("%s s_status=%d, update p_bk_mat!!!!\n", __FUNCTION__, m_frameStep);
			}
			m_frameStep = E_FRAME_FIRST;                
		}
	}

	cvReleaseImage(&src);

	std::string analyze_res;
	if (warning_num > 0)
	{
		analyze_res = "foreign body";
	}
	else
	{
		analyze_res = "normal";
	}
	return analyze_res;
}
