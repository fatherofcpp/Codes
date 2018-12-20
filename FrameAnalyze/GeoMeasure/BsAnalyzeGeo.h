#pragma once

#include <deque>

typedef struct tag_pic_point
{
	tag_pic_point()
	{
		row = 0;
		col = 0;
		gray = 0;	
	}
	int row;
	int col;
	int gray;
}st_pic_coord;

typedef struct tag_geo
{
	tag_geo()
	{
		dg = 0;
		lc = 0;
	}
	double dg;
	double lc;
}st_geo;

typedef struct tag_area
{
	tag_area()
	{
		row_begin = 0;
		row_end = 0;
		col_begin = 0;
		col_end = 0;
	}

	int row_begin;
	int row_end;
	int col_begin;
	int col_end;
}st_area;

typedef enum tag_e_camera
{
	E_CAMERA_UP = 0,
	E_CAMERA_LEFT,
	E_CAMERA_RIGHT,
	E_CAMERA_COUNT,
}e_camera;

class CAnalyzeGeo
{
public:
	CAnalyzeGeo(void);
	~CAnalyzeGeo(void);

public:
	void analyze_geo(std::string file_path);
private:
	void analyze_find_points(std::string file_path);
	void analyze_find_area();
	void analyze_filter_area();
	int point_2_area(tag_pic_point pic_point);
private:
	std::deque<tag_pic_point> m_matchPoints;
	std::deque<tag_area> m_matchAreas;
public:
	//outer get
	int m_picWith;
	int m_picHeight;

	//outer get
	tag_geo m_geo;

	//outer set
	int m_areaWith;
	int m_areaHeight;

	int m_top;
	int m_bottom;
	int m_left;
	int m_rigth;

	int m_threPoint;//0-255

	int m_cameraInd;
};

