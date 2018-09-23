#pragma once
#include <string>
#include <vector>

struct tag_st_pis_pack_inf;
struct _NCIP_HEADER;

struct _Video_ADDTextInfo;
struct _Msg_PresetInfo;
struct _Msg_CameraSwitchInfo;
struct _Msg_PantoLedSwitchInfo;
struct _Msg_PtzControlInfo;
struct _Msg_TimingInfo;
struct _Msg_SearchVideoInfo;
struct _Msg_SearchJpgInfo;
struct _Msg_PresetPos;
namespace MVB
{
	struct tag_st_mvb_alarm_data;
}
class CCriticalSection;

//根据报警类型，等级，数值组装报警消息
int FormatAlarmMessage(const unsigned int alarm_type, const unsigned int alarm_cls, const double alarm_value, std::string& alarm_message);

//根据报警类型，等级，更新MVB报警数据
int UpdataMvbAlarmByFrame(MVB::tag_st_mvb_alarm_data* alarm_data, CCriticalSection* mvb_alarm_lock, const unsigned int alarm_type, const unsigned int alarm_cls);

//查询设备列表
bool findDevice(const char* p_provider, const char* p_deviceInfos);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//pis接收包处理
int HandlePisPacket( const std::vector<tag_st_pis_pack_inf>* p_vec_input, std::vector<tag_st_pis_pack_inf>* p_vec_output );

//组装pis桢
int FormatPisFrame(char* output, _NCIP_HEADER pis_head, const char* data);

//解析pis桢
template <typename T>
int ParsePisFrame(const char* input, const int input_len, T* p_Msg_Pis)
{
	int ret = 0;
	if (input_len <= 0 || !input)
	{
		return -1;
	}

	TCP_HEADER pis_head;
	const int c_head_len = sizeof(pis_head);
	int byte_ind = 0;
	memcpy((char*)&pis_head, input + byte_ind,  c_head_len);
	byte_ind += c_head_len;
	{
		const int c_body_len = sizeof(*p_Msg_Pis);
		memcpy((char*)p_Msg_Pis, input + byte_ind,  c_body_len);
		byte_ind += c_body_len;
	}
	BYTE check = input[byte_ind++];
	return 0;
}
//common
//0PTZ控制报文_Msg_PtzControlInfo* p_Msg_PtzControlInfo
//pis
//1预置位_Msg_PresetPos* p_Msg_PresetPos
//2受电弓闪光灯开关_Msg_PantoLedSwitchInfo* p_Msg_PantoLedSwitchInfo);
//ptu
//3视频叠加字符报文_Video_ADDTextInfo* p_Video_ADDTextInfo);
//4校时报文_Msg_TimingInfo* p_Msg_TimingInfo);
//5远程搜索视频报文_Msg_SearchVideoInfo* p_Msg_SearchVideoInfo);
//6预置位;_Msg_PresetInfo* p_Msg_PresetInfo);
//7受电弓摄像机开关_Msg_CameraSwitchInfo* p_Msg_CameraSwitchInfo);
//8受电弓摄像机补光灯报文;见2受电弓闪光灯开关
//9远程搜索图片_Msg_SearchJpgInfo* p_Msg_SearchJpgInfo);
//10远程搜索视频报文(故障视频);见5远程搜索视频报文

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//cpu掩码
unsigned int CpuMaskExceptBit(unsigned int except_bit);
unsigned int CpuMaskRemain();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
