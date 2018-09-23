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
	unsigned char* m_pData; //����
	unsigned int m_dataLen;//���ݳ���
	PVOID m_pDataHeader;

	unsigned int m_width;	//���					
	unsigned int m_height;//�߶�
	NSP_STP_CM::tag_e_frame_format m_frameType;//��ʽ

	//alarm
	tag_e_frame_warn_type m_alarmType;
	tag_e_frame_warn_cls m_alarmCls;
	std::string m_alarmMessage;
	double m_alarmValue;
	//db
	std::string m_filePath;
};

