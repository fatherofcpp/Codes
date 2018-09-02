#pragma once

#include <string>

struct _IplImage;
class CStatic;
class CDC;

namespace NSP_STP_CM
{
	class COpenCvDll
	{
	public:
		COpenCvDll(void);
		~COpenCvDll(void);

	public:
		void ContinueImg(std::string file_path, int y_off, double fScale, int delay_tms, char* msg);
	private:
		bool m_first_enter;
	};

	bool LoadImgFrame(std::string file_path, int iscolor, unsigned char*& pData, int& width, int& height, int& frame_size);  

	void DisImg(std::string file_path, double fScale, int delay_tms);

	void ShowImgStatic(std::string file_path, CStatic* pStc, CDC* pDC);

	void EvaluationImage(char* ImagePath, float& brightness, float& contrast, float& zh, char* resolution, int n_type);

	int ImgRectAdd(std::string file_path, int left_up_x, int left_up_y, int width, int height);

	int ImgTextAdd(std::string file_path, int left_up_x, int left_up_y, int width, int height, const char* text_add);
}
