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
		//֡��
		M_FLOAT fps;
		//�Ƿ�ʹ���ع�����
		MS_INT32 isUseExposeCmd;
		//�Ƿ��Զ��ع�
		MS_INT32 isAutoExpose;
		//�ع�ʱ��
		MS_INT32 exposeTimeUs;
		//�Ƿ�ʹ����������
		MS_INT32 isUseGainCmd;
		//�Ƿ��Զ�����
		MS_INT32 isAutoGain;
		//����
		MS_INT32 gainVal;
		//�ü�ԭ��ƫ��
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
		MS_INT32 baud;//������9600
		MS_INT32 stop;//ֹͣλ
		MS_INT32 byte_size;//����λ
		MS_INT32 parity;//У�� 0~4=no, odd, even, mark, space
		MS_INT32 read_timeout_m;//����ʱ����4000
		MS_INT32 write_timeout_m;//д��ʱ����4000
		MS_INT32 buff_size;//�շ�������
	}StSerialOpt;
	///////////////////////////////////////////

}
