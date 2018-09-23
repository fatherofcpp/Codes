/********************************************************************
	created:	2014/03/27
	filename: 	avLib.cpp
	file base:	FFMPEG
	file ext:	cpp
	author:		zhh
	
	purpose:	实现动态加载
*********************************************************************/

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "avLib.h"


 GD_avcodec_encode_video GDavcodec_encode_video = NULL;
 GD_avcodec_open GDavcodec_open = NULL;
 GD_avcodec_find_encoder GDavcodec_find_encoder = NULL;
 GD_av_new_stream GDav_new_stream = NULL;
 GD_av_alloc_format_context GDav_alloc_format_context = NULL;
 GD_avpicture_fill GDavpicture_fill = NULL;
 GD_av_free GDav_free = NULL;
 GD_av_malloc GDav_malloc = NULL;
 GD_avpicture_get_size GDavpicture_get_size = NULL;
 GD_avcodec_alloc_frame GDavcodec_alloc_frame = NULL;
 GD_avcodec_init GDavcodec_init = NULL;
 GD_av_register_all GDav_register_all = NULL;

GD_avcodec_find_decoder GDavcodec_find_decoder = NULL;
GD_avcodec_decode_video GDavcodec_decode_video = NULL;
GD_avcodec_close GDavcodec_close = NULL;

int loadAVLib()
{
	//avcodec.dll
	HMODULE hProcessModule1=NULL;
	//SetDllDirectory(NULL);
	SetDllDirectory(g_CurrentDir+"\\avdll\\");
	CString path=g_CurrentDir+"\\avdll\\avcodec.dll";
	//m_Log.Add(g_CurrentDir);
	hProcessModule1	=LoadLibraryA("gdcodec.dll");//LoadLibraryEx(path,NULL, LOAD_WITH_ALTERED_SEARCH_PATH);// 
	//TRACE("ERR is %d\n",GetLastError());
	if(NULL == hProcessModule1)
		return -1;
	GDavcodec_encode_video=(GD_avcodec_encode_video)GetProcAddress(hProcessModule1,"avcodec_encode_video");
	if(NULL == GDavcodec_encode_video)
		return -2;

	GDavcodec_find_decoder=(GD_avcodec_find_decoder)GetProcAddress(hProcessModule1,"avcodec_find_decoder");
	if(NULL == GDavcodec_find_decoder)
		return -102;
	GDavcodec_decode_video=(GD_avcodec_decode_video)GetProcAddress(hProcessModule1,"avcodec_decode_video");
	if(NULL == GDavcodec_decode_video)
		return -202;
	GDavcodec_close=(GD_avcodec_close)GetProcAddress(hProcessModule1,"avcodec_close");
	if(NULL == GDavcodec_close)
		return -302;

	GDavcodec_open=(GD_avcodec_open)GetProcAddress(hProcessModule1,"avcodec_open");
	if(NULL == GDavcodec_open)
		return -3;
	GDavcodec_find_encoder=(GD_avcodec_find_encoder)GetProcAddress(hProcessModule1,"avcodec_find_encoder");
	if(NULL == GDavcodec_find_encoder)
		return -4;
	GDavpicture_fill=(GD_avpicture_fill)GetProcAddress(hProcessModule1,"avpicture_fill");
	if(NULL == GDavpicture_fill)
		return -5;
	GDavpicture_get_size=(GD_avpicture_get_size)GetProcAddress(hProcessModule1,"avpicture_get_size");
	if(NULL == GDavpicture_get_size)
		return -6;
	GDavcodec_alloc_frame=(GD_avcodec_alloc_frame)GetProcAddress(hProcessModule1,"avcodec_alloc_frame");
	if(NULL == GDavcodec_alloc_frame)
		return -7;
	GDavcodec_init=(GD_avcodec_init)GetProcAddress(hProcessModule1,"avcodec_init");
	if(NULL == GDavcodec_init)
		return -8;

	//FreeLibraryA(hProcessModule1);
//	TRACE("%s_______1.1_______n","cqdq");
//avformat.dll
	HMODULE hProcessModule2=NULL;
	//SetDllDirectory(g_CurrentDir);
	hProcessModule2	=  LoadLibraryA("gdformat.dll");
	GDav_new_stream=(GD_av_new_stream)GetProcAddress(hProcessModule2,"av_new_stream");
	if(NULL == GDav_new_stream)
		return -11;
	GDav_alloc_format_context=(GD_av_alloc_format_context)GetProcAddress(hProcessModule2,"av_alloc_format_context");
	if(NULL == GDav_alloc_format_context)
		return -12;
	GDav_register_all=(GD_av_register_all)GetProcAddress(hProcessModule2,"av_register_all");
	if(NULL == GDav_register_all)
		return -13;
	TRACE("%s_______1.2_______n","cqdq");
//avutil.dll
	HMODULE hProcessModule3=NULL;
	//SetDllDirectory(g_CurrentDir);
	hProcessModule3	=  LoadLibraryA("gdutil.dll");
	if(NULL == hProcessModule3)
		return -21;
	GDav_free=(GD_av_free)GetProcAddress(hProcessModule3,"av_free");//
	if(NULL == GDav_free)
		return -22;
	GDav_malloc=(GD_av_malloc)GetProcAddress(hProcessModule3,"av_malloc");//
	if(NULL == GDav_malloc)
		return -23;

	GDavcodec_init();
	GDav_register_all();

	SetDllDirectory(g_CurrentDir+"\\");




	return 0;
}