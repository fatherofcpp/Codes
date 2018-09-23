#include "StdAfx.h"
#include "BsGeoColtor.h"
#include "CamPicture/CameraObject.h"
#include "BsDefStruct.h"
#include "Log/CmLog.h"

CGeoColtor::CGeoColtor(void)
{
}


CGeoColtor::~CGeoColtor(void)
{
}

void CGeoColtor::SetCameraOpt()
{
	int iRet = 0;

	const float fps = m_fps;
	const unsigned int uMicoSeconds = (unsigned int)((float)1.0/fps * 1000*1000);

	while(1)
	{
		iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_FRAME_CYCLE, 
			"%u\r\n", uMicoSeconds);
		if (iRet == 0)
		{
			break;
		}
		else
		{
			Sleep(1000);
			gLog.Add("%s CAM_COMMAND_FRAME_CYCLE, m_cameraInd=%d\n", __FUNCTION__, m_cameraInd);
		}
	}

	while(1)
	{
		//实际图
		iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_SWM, 
			"%d\r\n", 1);
		if (iRet == 0)
		{
			break;
		}
		else
		{
			Sleep(1000);
			gLog.Add("%s 实际图, m_cameraInd=%d\n", __FUNCTION__, m_cameraInd);
		}
	}

	while(1)
	{
		//激光器延后触发
		iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_STDT, 
			"%d\r\n", m_jgqPreReadyTms * 1000);
		if (iRet == 0)
		{
			break;
		}
		else
		{
			Sleep(1000);
			gLog.Add("%s 延后触发, m_cameraInd=%d\n", __FUNCTION__, m_cameraInd);
		}
	}

	if (1 == ((Cam::tagDeviceFormatInfo*)(m_pAbstractFormatInfo))->triggerMode)
	{
		if(1 == ((Cam::tagDeviceFormatInfo*)(m_pAbstractFormatInfo))->triggerType)
		{
			while(1)
			{
				iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_TRIGGER_MODE, 
					"%d\r\n", 3);
				if (iRet == 0)
				{
					break;
				}
				else
				{
					Sleep(1000);
					gLog.Add("%s 软触发, m_cameraInd=%d\n", __FUNCTION__, m_cameraInd);
				}
			}
		}
	}
	else
	{
		while(1)
		{
			iRet = m_pCObjectDevice->setOption(Cam::CAM_COMMAND_TRIGGER_MODE, 
				"%d\r\n", 0);
			if (iRet == 0)
			{
				break;
			}
			else
			{
				Sleep(1000);
				gLog.Add("%s 自由触发, m_cameraInd=%d\n", __FUNCTION__, m_cameraInd);
			}
		}

	}

	iRet = 0;
}
