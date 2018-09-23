#include "StdAfx.h"
#include "BsArcCollector.h"
#include "CamPicture/CameraObject.h"

CArcCollector::CArcCollector(void)
{
	m_framePath = "";
}

CArcCollector::~CArcCollector(void)
{
}

void CArcCollector::UpdataFramePath( std::string frame_path )
{
	m_framePath = frame_path;
}

void CArcCollector::SetCameraOpt()
{
	SetCameraCmOpt();
}

