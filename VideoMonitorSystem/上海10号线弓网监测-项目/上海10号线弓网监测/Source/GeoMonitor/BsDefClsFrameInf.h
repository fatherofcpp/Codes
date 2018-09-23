#pragma once
#include <string>
enum tag_e_frame_warn_type;
enum tag_e_frame_warn_cls;
namespace NSP_STP_CM
{
	enum tag_e_frame_format;
}
class CFrameInf
{
public:
	CFrameInf(const char *p_data, int i_dataLen, PVOID p_dataHeader);
	~CFrameInf();
public:
	unsigned char* m_pData; //数据
	unsigned int m_dataLen;//数据长度
	PVOID m_pDataHeader;

	unsigned int m_width;	//宽度					
	unsigned int m_height;//高度
	NSP_STP_CM::tag_e_frame_format m_frameType;//格式

	//alarm
	tag_e_frame_warn_type m_alarmType;
	tag_e_frame_warn_cls m_alarmCls;
	std::string m_alarmMessage;
	double m_alarmValue;
	//db
	std::string m_filePath;
};

