#include "StdAfx.h"
#include "BsAnalyzeGeo.h"
#include "CmOpenCvDll.h"

#include <opencv/cv.hpp>  

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp> 

using namespace cv;  

CAnalyzeGeo::CAnalyzeGeo(void)
{
	m_picWith = 0;
	m_picHeight = 0;

	m_areaWith = 35;
	m_areaHeight = 15;

	m_top = 4100;
	m_bottom = 4040;
	m_left = -200;
	m_rigth = 200;

	m_threPoint = 0;
	
	m_cameraInd = 0;
}


CAnalyzeGeo::~CAnalyzeGeo(void)
{
}

void CAnalyzeGeo::analyze_geo(std::string file_path)
{
	analyze_find_points(file_path);
	analyze_find_area();
	analyze_filter_area();
	return;  
}

void CAnalyzeGeo::analyze_find_points(std::string file_path)
{
	m_matchPoints.clear();

	const int is_color = CV_LOAD_IMAGE_GRAYSCALE;
	IplImage* src = cvLoadImage( file_path.c_str(), is_color);

	IplImage *img_filtered = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, src->nChannels);   
	cvSmooth(src, img_filtered, CV_GAUSSIAN, 7, src->nChannels);

	int width = img_filtered->width;
	int height = img_filtered->height;
	m_picWith = width;
	m_picHeight = height;


	for (int row = 0; row < height; row ++)  
	{  
		uchar* ptr = (uchar*)(img_filtered->imageData + row * img_filtered->width);//获得灰度值数据指针  
		for (int cols = 0; cols < width; cols++)  
		{  
			int intensity = ptr[cols];  
			if (intensity >= m_threPoint)
			{
				tag_pic_point pic_point;
				pic_point.row = row;
				pic_point.col = cols;
				pic_point.gray = intensity;
				m_matchPoints.push_back(pic_point);

				//TRACE("%s row=%d, cols=%d, intensity=%d\n", __FUNCTION__, row, cols, intensity);
			}
			if (intensity < 1)
			{
				//TRACE("%s row=%d, cols=%d, intensity=%d\n", __FUNCTION__, row, cols, intensity);
			}
		}  
	}  

	cvReleaseImage(&src);
	cvReleaseImage(&img_filtered);
}


void CAnalyzeGeo::analyze_find_area()
{
	m_matchAreas.clear();

	unsigned int vec_size = m_matchPoints.size();
	if (vec_size <= 0)
	{
		return;
	}

	for (unsigned int i = 0; i < vec_size; i ++)
	{
		tag_pic_point pic_point = m_matchPoints[i];
		point_2_area( pic_point);
	}

}

void CAnalyzeGeo::analyze_filter_area()
{
	st_pic_coord pic_point;

	unsigned int vec_size = m_matchAreas.size();

	for (unsigned int i = 0; i < vec_size; i ++)
	{
		tag_area area = m_matchAreas[i];
		if(abs(area.col_end - area.col_begin) <= m_areaWith
			&& abs(area.row_end - area.row_begin) <= m_areaHeight)
		{
			const int col = (area.col_end + area.col_begin) / 2 ;
			const int row = (area.row_end + area.row_begin) / 2 ;
			if (pic_point.row < row)
			{
				pic_point.col = col;
				pic_point.row = row;

				double col_percent = (double)((double)pic_point.col) / ((double)m_picWith) ;
				m_geo.lc = m_rigth - (m_rigth - m_left) * col_percent;

				double row_percent = (double)((double)pic_point.row) / ((double)m_picHeight) ;
				m_geo.dg = m_top - (m_top - m_bottom) * row_percent;
			}
		}
	}

	//gLog.Add("%s m_filePath=%s, col=%d, row=%d\n", __FUNCTION__, m_filePath.c_str(), m_point.col, m_point.row);
}

int CAnalyzeGeo::point_2_area( tag_pic_point pic_point )
{
	unsigned int vec_size_match_area = m_matchAreas.size();
	if (0 == vec_size_match_area)
	{
		tag_area pic_area;
		pic_area.col_begin = pic_point.col;
		pic_area.col_end = pic_point.col;
		pic_area.row_begin = pic_point.row;
		pic_area.row_end = pic_point.row;
		m_matchAreas.push_back(pic_area);
		return 0;
	}


	bool is_same_area = false;

	tag_area* p_area;
	for(unsigned int i = 0; i < m_matchAreas.size(); i ++)
	{
		p_area = &m_matchAreas[i];
		int a = abs(p_area->col_end - pic_point.col);
		int b = abs(p_area->col_begin - pic_point.col);
		int c = abs(p_area->row_end - pic_point.row);
		int d = abs(p_area->row_begin - pic_point.row);
		if(
			(a <= m_areaWith || b <= m_areaWith)  
			||
			(c <= m_areaHeight || d <= m_areaHeight)  
			)
		{
			is_same_area = true;
			break;
		}
		else
		{
			is_same_area = false;
		}
	}

	if(is_same_area)
	{
		if (p_area->col_begin > pic_point.col)
		{
			p_area->col_begin = pic_point.col;
		}

		if (p_area->col_end < pic_point.col)
		{
			p_area->col_end = pic_point.col;
		}

		if (p_area->row_begin > pic_point.row)
		{
			p_area->row_begin = pic_point.row;
		}

		if (p_area->row_end < pic_point.row)
		{
			p_area->row_end = pic_point.row;
		}
	}
	else
	{
		tag_area pic_area;
		pic_area.col_begin = pic_point.col;
		pic_area.col_end = pic_point.col;
		pic_area.row_begin = pic_point.row;
		pic_area.row_end = pic_point.row;
		m_matchAreas.push_back(pic_area);
	}

	return 0;
}






