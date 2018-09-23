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

//���ݱ������ͣ��ȼ�����ֵ��װ������Ϣ
int FormatAlarmMessage(const unsigned int alarm_type, const unsigned int alarm_cls, const double alarm_value, std::string& alarm_message);

//���ݱ������ͣ��ȼ�������MVB��������
int UpdataMvbAlarmByFrame(MVB::tag_st_mvb_alarm_data* alarm_data, CCriticalSection* mvb_alarm_lock, const unsigned int alarm_type, const unsigned int alarm_cls);

//��ѯ�豸�б�
bool findDevice(const char* p_provider, const char* p_deviceInfos);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//pis���հ�����
int HandlePisPacket( const std::vector<tag_st_pis_pack_inf>* p_vec_input, std::vector<tag_st_pis_pack_inf>* p_vec_output );

//��װpis��
int FormatPisFrame(char* output, _NCIP_HEADER pis_head, const char* data);

//����pis��
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
//0PTZ���Ʊ���_Msg_PtzControlInfo* p_Msg_PtzControlInfo
//pis
//1Ԥ��λ_Msg_PresetPos* p_Msg_PresetPos
//2�ܵ繭����ƿ���_Msg_PantoLedSwitchInfo* p_Msg_PantoLedSwitchInfo);
//ptu
//3��Ƶ�����ַ�����_Video_ADDTextInfo* p_Video_ADDTextInfo);
//4Уʱ����_Msg_TimingInfo* p_Msg_TimingInfo);
//5Զ��������Ƶ����_Msg_SearchVideoInfo* p_Msg_SearchVideoInfo);
//6Ԥ��λ;_Msg_PresetInfo* p_Msg_PresetInfo);
//7�ܵ繭���������_Msg_CameraSwitchInfo* p_Msg_CameraSwitchInfo);
//8�ܵ繭���������Ʊ���;��2�ܵ繭����ƿ���
//9Զ������ͼƬ_Msg_SearchJpgInfo* p_Msg_SearchJpgInfo);
//10Զ��������Ƶ����(������Ƶ);��5Զ��������Ƶ����

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//cpu����
unsigned int CpuMaskExceptBit(unsigned int except_bit);
unsigned int CpuMaskRemain();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
