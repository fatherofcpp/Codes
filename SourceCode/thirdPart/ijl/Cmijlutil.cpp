#include "stdafx.h"
#include "Cmijlutil.h"



NSP_STP_CM::CIJLUtil gCIJLUtil;

NSP_STP_CM::CIJLUtil::CIJLUtil()
{
	memset(&m_jpeg,   0,   sizeof(JPEG_CORE_PROPERTIES));
	ijlInit(&m_jpeg);
}

NSP_STP_CM::CIJLUtil::~CIJLUtil()
{
	ijlFree(&m_jpeg);
}

int NSP_STP_CM::CIJLUtil::DeCompressImg(BYTE **img, BYTE *shuju, int len, int * width, int * height, int *lpoutlength)
{
	m_cvtLock.Lock();

	IJLERR jerr;
	JPEG_CORE_PROPERTIES jpeg;
	memset(&jpeg, 0, sizeof(jpeg));
	jerr = ijlInit(&jpeg);
	if (jerr != IJL_OK)
	{
		m_cvtLock.Unlock();

		return jerr;
	}

	jpeg.JPGBytes =shuju;
	jpeg.JPGSizeBytes = len;
	jerr = ijlRead(&jpeg, IJL_JBUFF_READPARAMS);
	if (jerr != IJL_OK)
	{
		ijlFree(&jpeg);
		m_cvtLock.Unlock();
		return jerr;
	}

	switch(jpeg.JPGChannels)
	{
	case 1:
		{
			jpeg.JPGColor		= IJL_G;
			jpeg.DIBChannels	= 1;
			jpeg.DIBColor		= IJL_G;
			break;
		}
	case 4:
		{
			jpeg.JPGColor		= IJL_YCBCRA_FPX;
			jpeg.DIBChannels	= 4;
			jpeg.DIBColor		= IJL_RGBA_FPX;
			break;
		}
		/*
		default:
		jpeg.DIBColor		= (IJL_COLOR) IJL_OTHER;
		jpeg.JPGColor		= (IJL_COLOR) IJL_OTHER;
		jpeg.DIBChannels	= jpeg.JPGChannels;
		break;
		*/
	default:
		//case 3:
		{
			jpeg.JPGColor		= IJL_YCBCR;
			jpeg.DIBChannels	= 1;
			jpeg.DIBColor		= IJL_G;
			break;
		}
	}

	*width = jpeg.JPGWidth;
	*height = jpeg.JPGHeight;

	*lpoutlength =  jpeg.JPGWidth * jpeg.JPGHeight * jpeg.DIBChannels;
	BYTE *bms = new BYTE[(long)*lpoutlength];
	jpeg.DIBWidth = jpeg.JPGWidth;
	jpeg.DIBHeight = jpeg.JPGHeight; // Implies a bottom-up DIB.
	//	jcprops.DIBChannels = 3;
	//	jcprops.DIBColor = IJL_BGR;
	jpeg.DIBPadBytes = 0;
	jpeg.DIBBytes = bms;
	*img = bms;

	jerr = ijlRead(&jpeg,  IJL_JBUFF_READWHOLEIMAGE);
	ijlFree(&jpeg);

	m_cvtLock.Unlock();

	return jerr;
}

int NSP_STP_CM::CIJLUtil::Encode( DWORD dwWidth, DWORD dwHeight, LPVOID lpYUYVSrc, DWORD dwSrcLength, LPVOID lpJPEGDst, DWORD dwDstLength, LPDWORD lpJPEGSize,bool bColorCam)
{
	m_cvtLock.Lock();

	if (lpYUYVSrc == NULL || lpJPEGDst == NULL /*|| dwSrcLength == 0 || dwDstLength == 0*/) {
		m_cvtLock.Unlock();

		return IJL_MEMORY_ERROR;
	}
	if (dwDstLength < (dwWidth * dwHeight >> 1)) {
		m_cvtLock.Unlock();

		return IJL_BUFFER_TOO_SMALL;
	}

	IJLERR ijlError = IJL_RESERVED;
	m_jpeg.DIBWidth		= dwWidth;
	m_jpeg.DIBHeight	= dwHeight;
	m_jpeg.DIBBytes		= (LPBYTE) lpYUYVSrc;
	m_jpeg.DIBChannels	= 1;
	m_jpeg.DIBColor		= IJL_G/*IJL_RGB*/;
	if(bColorCam)
	{
		m_jpeg.DIBChannels	= 3;
		m_jpeg.DIBColor		= IJL_RGB;
	}
	m_jpeg.JPGWidth		= m_jpeg.DIBWidth;
	m_jpeg.JPGHeight	= m_jpeg.DIBHeight;
	m_jpeg.JPGBytes		= (LPBYTE) lpJPEGDst;
	m_jpeg.JPGChannels	= m_jpeg.DIBChannels;
	m_jpeg.JPGColor		= IJL_G/*IJL_RGB*/;
	m_jpeg.JPGSubsampling= IJL_NONE;
	if(bColorCam)
	{
		m_jpeg.JPGColor		= IJL_YCBCR;
		m_jpeg.JPGSubsampling= IJL_411/*IJL_NONE*/;
	}
	m_jpeg.JPGSizeBytes	= dwDstLength;
	m_jpeg.jquality		= 75;

	ijlError = ijlWrite(&m_jpeg, IJL_JBUFF_WRITEWHOLEIMAGE);
	if (ijlError == IJL_OK) {
		*lpJPEGSize = m_jpeg.JPGSizeBytes;
	}
	m_cvtLock.Unlock();

	return ijlError;
}

int bmp2jpg(int width, int height, const char* pData, char* pJpg, int jpgLen, DWORD& destfilelength, bool useColor = false)
{
	int ret = 0;
	ret = gCIJLUtil.Encode(width, height, (LPVOID)pData, width * height, pJpg, jpgLen, &destfilelength, useColor);
	return ret;
}
