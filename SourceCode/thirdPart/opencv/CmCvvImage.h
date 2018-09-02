#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>

namespace NSP_STP_CM
{
	class CvvImage
	{
	public:
		CvvImage();
		virtual ~CvvImage();

		//创建结构
		virtual bool Create( int width, int height, int bits_per_pixel, int image_origin = 0);

		//加载图像
		virtual bool Load( const char* filename, int desired_color = 1 );

		//加载图像中指定区域
		virtual bool LoadRect( const char* filename,
			int desired_color, CvRect r );

#if defined WIN32 || defined _WIN32
		virtual bool LoadRect( const char* filename,
			int desired_color, RECT r )
		{
			return LoadRect( filename, desired_color,
				cvRect( r.left, r.top, r.right - r.left, r.bottom - r.top ));
		}
#endif

		//保存图像
		virtual bool Save( const char* filename );

		//图像拷贝
		virtual void CopyOf( CvvImage& image, int desired_color = -1 );
		virtual void CopyOf( IplImage* img, int desired_color = -1 );

		IplImage* GetImage() { return m_img; };

		//释放图像
		virtual void Destroy(void);

		int Width() { return !m_img ? 0 :!m_img->roi ? m_img->width : m_img->roi->width; };
		int Height() { return !m_img ? 0 :!m_img->roi ? m_img->height : m_img->roi->height;};

		int Bpp() { return m_img ? (m_img->depth & 255)*m_img->nChannels : 0; };

		virtual void Fill( int color );

		virtual void Show( const char* window );

#if defined WIN32 || defined _WIN32

		virtual void Show( HDC dc, int x, int y, int width, int height,
			int from_x = 0, int from_y = 0 );

		virtual void DrawToHDC( HDC hDCDst, RECT* pDstRect );
#endif

	protected:
		IplImage* m_img;
	};
}

