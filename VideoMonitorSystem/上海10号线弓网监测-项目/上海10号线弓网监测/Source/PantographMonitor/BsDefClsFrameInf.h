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
	unsigned char* m_pData; //数据
	unsigned int m_dataLen;//数据长度
	PVOID m_pDataHeader;

	unsigned int m_width;	//宽度					
	unsigned int m_height;//高度
	NSP_STP_CM::tag_e_frame_format m_frameType;//格式

	//alarm
	tagAnalyzeResArr* m_pAnalyzeResArr;

	//db
	std::string m_filePath;

	bool m_isTempSave;
	bool m_isTempMapSave;
	bool m_isTempAlarm;
	bool m_isColor;

	double m_tempLow;//最低温度
	double m_tempHigh;//最高温度
	double m_tempAvarage;//评价温度
	int m_tempHignLeftUpX;//最高温度左上角x坐标
	int m_tempHignLeftUpY;//最高温度左上角y坐标
};

