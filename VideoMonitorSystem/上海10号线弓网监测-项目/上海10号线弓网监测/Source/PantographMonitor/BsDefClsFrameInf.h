#pragma once
#include <string>

enum tag_e_frame_warn_type;
enum tag_e_frame_warn_cls;
struct tagAnalyzeResArr;
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
	tagAnalyzeResArr* m_pAnalyzeResArr;

	//db
	std::string m_filePath;

	bool m_isTempSave;
	bool m_isTempMapSave;
	bool m_isTempAlarm;
	bool m_isColor;

	double m_tempLow;//����¶�
	double m_tempHigh;//����¶�
	double m_tempAvarage;//�����¶�
	int m_tempHignLeftUpX;//����¶����Ͻ�x����
	int m_tempHignLeftUpY;//����¶����Ͻ�y����
};

