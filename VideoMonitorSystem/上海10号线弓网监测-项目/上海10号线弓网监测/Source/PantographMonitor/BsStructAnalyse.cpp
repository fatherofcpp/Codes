#include "StdAfx.h"
#include "BsStructAnalyse.h"
#include "CmDefStruct.h"
#include "CamPicture/CameraObject.h"
#include "Log/CmLog.h"

CStructAnalyse::CStructAnalyse(void)
{
	m_framePath = "";
}


CStructAnalyse::~CStructAnalyse(void)
{
}

void CStructAnalyse::SetCameraOpt()
{
	SetCameraCmOpt();
}

void CStructAnalyse::UpdataFramePath( std::string frame_path )
{
	m_framePath = frame_path;
}




