#pragma once

#ifndef _WIN64
#include "ijl/ijl.h"
#pragma comment(lib,"ijl15l.lib")
#else
#include "ijl/IJL20.h"
#pragma comment(lib,"ijl/lib/ijl20.lib")
#endif

namespace NSP_STP_CM
{
	class CIJLUtil
	{
	public:
		CIJLUtil();
		~CIJLUtil();

		typedef unsigned char uint8_t;

	public:
		int DeCompressImg(BYTE **img, BYTE *shuju, int len, int * width, int * height, int *lpoutlength);

		int Encode(DWORD dwWidth, DWORD dwHeight, LPVOID lpYUYVSrc, DWORD dwSrcLength, 
			LPVOID lpJPEGDst, DWORD dwDstLength, LPDWORD lpJPEGSize,bool bColorCam = false);

	private:
		CCriticalSection m_cvtLock;
		JPEG_CORE_PROPERTIES m_jpeg;
	};

	int bmp2jpg(int width, int height, const char* pData, char* pJpg, int jpgLen, DWORD& destfilelength, bool useColor = false);

}
extern NSP_STP_CM::CIJLUtil gCIJLUtil;
