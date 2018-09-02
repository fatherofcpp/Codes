#pragma once

#include "CmDefType.h"
#include <string>
#include <sstream>
#include <vector>
#include <map>

namespace NSP_STP_CM
{
	typedef struct tag_st_time
	{
		tag_st_time()
		{
			year = 0;
			mon = 0;
			day = 0;
			hour = 0;
			min = 0;
			sec = 0;
		}
		MS_INT32 year;
		MS_INT32 mon;
		MS_INT32 day;
		MS_INT32 hour;
		MS_INT32 min;
		MS_INT32 sec;
		MS_INT32 ms;
	} st_time;

	typedef enum tag_e_frame_format
	{
		//JPEG
		E_FRAME_FORMAT_JPEG = 0,
		E_FRAME_FORMAT_BMP,
	}e_frame_format;

	typedef struct tag_st_report_service_inf
	{
		tag_st_report_service_inf()
		{
			ip = "";
			port = 0;
		}
		M_STRING ip;
		MS_INT32 port;
	}st_report_service_inf;

	typedef struct tag_st_dbmysql_param
	{
		tag_st_dbmysql_param()
		{
			dbHost = "";
			dbUser = "";
			dbPass = "";
			dbName = "";
			dbPort = 0;
		}

		M_STRING dbHost;
		M_STRING dbUser;
		M_STRING dbPass;
		M_STRING dbName ;
		MS_INT32 dbPort;
	}st_dbmysql_param;

	typedef struct tagStCameraOpt
	{
		tagStCameraOpt()
		{
			fps = 0.0;
			isUseExposeCmd = 0;
			isAutoExpose = 0;
			exposeTimeUs = 0;
			isUseGainCmd = 0;
			isAutoGain = 0;
			gainVal = 0;
			orgOffX = 0;
			orgOffY = 0;
		}
		//帧率
		M_FLOAT fps;
		//是否使用曝光命令
		MS_INT32 isUseExposeCmd;
		//是否自动曝光
		MS_INT32 isAutoExpose;
		//曝光时间
		MS_INT32 exposeTimeUs;
		//是否使用增益命令
		MS_INT32 isUseGainCmd;
		//是否自动增益
		MS_INT32 isAutoGain;
		//增益
		MS_INT32 gainVal;
		//裁剪原点偏移
		MS_INT32 orgOffX;
		MS_INT32 orgOffY;
	}StCameraOpt;

	typedef struct tagStSerialOpt
	{
		tagStSerialOpt()
		{
			com_name = "";
			baud = 0;
			stop = 0;
			byte_size = 0;
			parity = 0;
			read_timeout_m = 0;
			write_timeout_m = 0;
			buff_size = 4096;
		}
		M_STRING com_name;//COM4
		MS_INT32 baud;//波特率9600
		MS_INT32 stop;//停止位
		MS_INT32 byte_size;//数据位
		MS_INT32 parity;//校验 0~4=no, odd, even, mark, space
		MS_INT32 read_timeout_m;//读超时参数4000
		MS_INT32 write_timeout_m;//写超时参数4000
		MS_INT32 buff_size;//收发缓冲区
	}StSerialOpt;
	///////////////////////////////////////////

}
