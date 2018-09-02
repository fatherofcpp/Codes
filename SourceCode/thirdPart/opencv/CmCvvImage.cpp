//**********************CvvImage.cpp**********************//
#include "StdAfx.h"
#include "CmCvvImage.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CV_INLINE RECT NormalizeRect( RECT r );
CV_INLINE RECT NormalizeRect( RECT r )
{
	int t;
	if( r.left > r.right )
	{
		t = r.left;
		r.left = r.right;
		r.right = t;
	}
	if( r.top > r.bottom )
	{
		t = r.top;
		r.top = r.bottom;
		r.bottom = t;
	}
	return r;
}

CV_INLINE CvRect RectToCvRect( RECT sr);
CV_INLINE CvRect RectToCvRect( RECT sr)
{
	sr = NormalizeRect( sr );
	return cvRect( sr.left, sr.top, sr.right -sr.left, sr.bottom - sr.top );
}

CV_INLINE RECT CvRectToRect( CvRect sr);
CV_INLINE RECT CvRectToRect( CvRect sr)
{
	RECT dr;
	dr.left = sr.x;
	dr.top = sr.y;
	dr.right = sr.x + sr.width;
	dr.bottom = sr.y + sr.height;
	return dr;
}

CV_INLINE IplROI RectToROI( RECT r );
CV_INLINE IplROI RectToROI( RECT r )
{
	IplROI roi;
	r = NormalizeRect( r );
	roi.xOffset = r.left;
	roi.yOffset = r.top;
	roi.width = r.right - r.left;
	roi.height = r.bottom - r.top;
	roi.coi = 0;
	return roi;
}

void FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
	assert( bmi && width >= 0&& height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
	BITMAPINFOHEADER* bmih =&(bmi->bmiHeader);
	memset( bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) :-abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if( bpp == 8 )
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for( i = 0; i < 256; i++ )
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

NSP_STP_CM::CvvImage::CvvImage()
{
	m_img = 0;
}

void NSP_STP_CM::CvvImage::Destroy()
{
	cvReleaseImage( &m_img );
}

NSP_STP_CM::CvvImage::~CvvImage()
{
	Destroy();
}

bool NSP_STP_CM::CvvImage::Create( int w, int h, int bpp, int origin )
{
	const unsigned c_max_img_pixel = 10000;
	if( (bpp != 8 && bpp != 24 &&bpp != 32) ||
		(unsigned)w >=  c_max_img_pixel ||(unsigned)h >= c_max_img_pixel ||
		(origin != IPL_ORIGIN_TL && origin != IPL_ORIGIN_BL))
	{
		return false;
	}

	if( !m_img || Bpp() != bpp || m_img->width!= w || m_img->height != h )
	{
		if( m_img && m_img->nSize == sizeof(IplImage))
			Destroy();

		m_img = cvCreateImage( cvSize( w, h ), IPL_DEPTH_8U, bpp/8 );
	}

	if( m_img )
		m_img->origin = origin == 0 ? IPL_ORIGIN_TL : IPL_ORIGIN_BL;

	return m_img != 0;
}

void NSP_STP_CM::CvvImage::CopyOf( CvvImage& image, int desired_color )
{
	IplImage* img = image.GetImage();
	if( img )
	{
		CopyOf( img, desired_color );
	}
}

#define HG_IS_IMAGE(img)                                                 \
	((img) != 0 && ((const IplImage*)(img))->nSize == sizeof(IplImage) && \
	((IplImage*)img)->imageData != 0)

void NSP_STP_CM::CvvImage::CopyOf( IplImage* img, int desired_color )
{
	if( HG_IS_IMAGE(img) )
	{
		int color = desired_color;
		CvSize size = cvGetSize( img );

		if( color < 0 )
			color = img->nChannels > 1;

		if( Create( size.width, size.height,
			(!color ? 1 : img->nChannels > 1 ? img->nChannels : 3)*8,
			img->origin ))
		{
			cvConvertImage( img, m_img, 0 );
		}
	}
}

bool NSP_STP_CM::CvvImage::Load( const char* filename, int desired_color )
{
	IplImage* img = cvLoadImage( filename,desired_color );
	if( !img )
		return false;
	CopyOf( img, desired_color );
	cvReleaseImage( &img );
	return true;
}

bool NSP_STP_CM::CvvImage::LoadRect( const char* filename,
	int desired_color, CvRect r )
{
	if( r.width < 0 || r.height < 0 ) 
		return false;

	IplImage* img = cvLoadImage( filename,desired_color );
	if( !img )
		return false;

	if( r.width == 0 || r.height == 0 )
	{
		r.width = img->width;
		r.height = img->height;
		r.x = r.y = 0;
	}

	if( r.x > img->width || r.y >img->height ||
		r.x + r.width < 0 || r.y + r.height < 0 )
	{
		cvReleaseImage( &img );
		return false;
	}

	if( r.x < 0 )
	{
		r.width += r.x;
		r.x = 0;
	}

	if( r.y < 0 )
	{
		r.height += r.y;
		r.y = 0;
	}

	if( r.x + r.width > img->width )
		r.width = img->width - r.x;

	if( r.y + r.height > img->height )
		r.height = img->height - r.y;

	cvSetImageROI( img, r );

	CopyOf( img, desired_color );

	cvReleaseImage( &img );

	return true;
}

bool NSP_STP_CM::CvvImage::Save( const char* filename )
{
	if( !m_img )
		return false;

	cvSaveImage( filename, m_img );
	return true;
}

void NSP_STP_CM::CvvImage::Show( const char* window )
{
	if( m_img )
		cvShowImage( window, m_img );
}

void NSP_STP_CM::CvvImage::Show( HDC dc, int x, int y, int w, int h, int from_x, int from_y )
{
	if( m_img && m_img->depth ==IPL_DEPTH_8U )
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = m_img->width, bmp_h = m_img->height;
		FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), m_img->origin );
		from_x = MIN( MAX( from_x, 0 ), bmp_w - 1 );
		from_y = MIN( MAX( from_y, 0 ), bmp_h - 1 );
		int sw = MAX( MIN( bmp_w - from_x, w ), 0 );
		int sh = MAX( MIN( bmp_h - from_y, h ), 0 );
		SetDIBitsToDevice(
			dc, x, y, sw, sh, from_x, from_y, from_y, sh,
			m_img->imageData + from_y*m_img->widthStep,
			bmi, DIB_RGB_COLORS );
	}
}

#if 0

void NSP_STP_CM::CvvImage::DrawToHDC( HDC hDCDst, RECT* pDstRect )
{
	if( pDstRect && m_img &&m_img->depth == IPL_DEPTH_8U && m_img->imageData )
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = m_img->width, bmp_h = m_img->height;
		CvRect roi = cvGetImageROI( m_img );
		CvRect dst = RectToCvRect( *pDstRect );
		if( roi.width == dst.width && roi.height == dst.height )
		{
			Show( hDCDst, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y );
			return;
		}
		if( roi.width > dst.width )
		{
			SetStretchBltMode(
				hDCDst,           // handle todevice context
				HALFTONE );
		}
		else
		{
			SetStretchBltMode(
				hDCDst,           // handle todevice context
				COLORONCOLOR );
		}
		FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), m_img->origin );
		::StretchDIBits(
			hDCDst,
			dst.x, dst.y, dst.width, dst.height,
			roi.x, roi.y, roi.width, roi.height,
			m_img->imageData, bmi, DIB_RGB_COLORS, SRCCOPY );
	}
}

#else

void NSP_STP_CM::CvvImage::DrawToHDC( HDC hDCDst, RECT* pDstRect )
{
	if( pDstRect && m_img &&m_img->depth == IPL_DEPTH_8U && m_img->imageData )
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = m_img->width, bmp_h = m_img->height;
		CvRect roi = cvGetImageROI( m_img );
		CvRect dst = RectToCvRect( *pDstRect );
		int dst_x = dst.x;
		int dst_y = dst.y;
		int dst_width = roi.width > dst.width ? dst.width : roi.width;
		int dst_height = roi.height > dst.height ? dst.height : roi.height;
		if( roi.width == dst.width && roi.height == dst.height )
		{
			Show( hDCDst, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y );
			return;
		}
		if( roi.width > dst.width && roi.height > dst.height)
		{
			//¿íÐ¡£¬¸ßÐ¡
			SetStretchBltMode(
				hDCDst,           // handle todevice context
				HALFTONE );
		}
		else
		{
			SetStretchBltMode(
				hDCDst,           // handle todevice context
				COLORONCOLOR );
		}

		FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), m_img->origin );
		::StretchDIBits(
			hDCDst,
			dst_x, dst_y, dst_width, dst_height,
			roi.x, roi.y, roi.width, roi.height,
			m_img->imageData, bmi, DIB_RGB_COLORS, SRCCOPY );
	}
}

#endif

void NSP_STP_CM::CvvImage::Fill( int color )
{
	cvSet( m_img,cvScalar(color&255,(color>>8)&255,(color>>16)&255,(color>>24)&255));
}
