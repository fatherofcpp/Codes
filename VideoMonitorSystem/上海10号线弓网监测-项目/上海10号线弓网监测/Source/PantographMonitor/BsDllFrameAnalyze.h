#pragma once
#include <string>
#include "BsDefStruct.h"
#include "CmDefStruct.h"

struct tagAnalyzeResArr;

typedef int (*IMGDETECT_INIT)(int width, int height);
typedef int (*IMGDETECT_CLEAR)();
typedef int (*IMGDETECT_DEAL)(
	const unsigned char* frame_data, 
	const int frame_len, 
	const NSP_STP_CM::tag_e_frame_format frame_format, 
	const char* file_path, 
	const int* TrainStation,
	tagAnalyzeResArr* p_AnalyzeResArr 
	);

class CFrameAnalyzeDll
{
public:
	CFrameAnalyzeDll(void);
	~CFrameAnalyzeDll(void);
public:
	int LoadDll();
public:
	IMGDETECT_INIT m_imgdetect_init;
	IMGDETECT_CLEAR m_imgdetect_clear ;
	IMGDETECT_DEAL m_imgdetect_deal;

	unsigned int m_width;	//¿í¶È					
	unsigned int m_height;//¸ß¶È
};
extern CFrameAnalyzeDll* g_pCFrameAnalyzeDll;

