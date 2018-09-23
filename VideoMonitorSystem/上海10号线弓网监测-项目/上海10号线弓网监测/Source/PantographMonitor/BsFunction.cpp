#include "StdAfx.h"
#include "BsFunction.h"
#include "Mvb/ServerTrans.h"
#include "CmFunction.h"
#include "BsDefClsFrameInf.h"
#include "BsDefStruct.h"
#include "CamPicture/CameraObject.h"
#include "CamVideo/VideoObjectDevSer.h"
#include "CamVideo/NVCTRDefines.h"
#include "Thread/CmThread.h"
#include "PantographMonitor.h"
#include "PantographMonitorDlg.h"

int FormatAlarmMessage(const unsigned int alarm_type, const unsigned int alarm_cls, const double alarm_value, std::string& alarm_message)
{
	int ret = 0;
	const int msg_len = 1024;
	char* p_msg = new char[msg_len];
	memset(p_msg, 0, msg_len);

	switch(alarm_type)
	{
		//"�����¶�
	case E_FRAME_WARN_TYPE_PANTEM:
		sprintf(p_msg, "�����¶ȳ���:�ȼ�%d,�����¶�%.02f", alarm_cls, alarm_value);
		break;
		//"ȼ����
	case E_FRAME_WARN_TYPE_ARCRATE:
		sprintf(p_msg, "ȼ���ʳ���:�ȼ�%d,ȼ����%.02f", alarm_cls, alarm_value);
		break;
		//"����¶�
	case E_FRAME_WARN_TYPE_SLOPE:
		sprintf(p_msg, "����¶ȳ���:�ȼ�%d,����¶�%.02f", alarm_cls, alarm_value);
		break;
		//"����ֵ����
	case E_FRAME_WARN_TYPE_GUIDE_HEIGHT:
		sprintf(p_msg, "����ֵ����:�ȼ�%d,����ֵ%.02f", alarm_cls, alarm_value);
		break;
		//"����ֵ����
	case E_FRAME_WARN_TYPE_PULLOUT:
		sprintf(p_msg, "����ֵ����:�ȼ�%d,����ֵ%.02f", alarm_cls, alarm_value);
		break;
		//"��������
	case E_FRAME_WARN_TYPE_FOREIGN_MATTER:
		sprintf(p_msg, "��������");
		break;
		//"�ܵ繭�ṹ�쳣
	case E_FRAME_WARN_TYPE_STRUCTURE_ABNORMAL:
		sprintf(p_msg, "�ܵ繭�ṹ�쳣");
		break;
		//"�Ӵ��߸߶Ȳ�
	case E_FRAME_WARN_TYPE_CONTACT_LINE_HEIGHT_DIFFERENCE:
		sprintf(p_msg, "�Ӵ��߸߶Ȳ��:�ȼ�%d,�Ӵ��߸߶Ȳ�%.02f", alarm_cls, alarm_value);
		break;
		//"������б
	case E_FRAME_WARN_TYPE_SLIDE_TILT:
		sprintf(p_msg, "������б");
		break;
		//"���嶪ʧ
	case E_FRAME_WARN_TYPE_SKATEBOARD_MISSING:
		sprintf(p_msg, "���嶪ʧ");
		break;
		//"����쳣
	case E_FRAME_WARN_TYPE_HORN_ANOMALY:
		sprintf(p_msg, "����쳣");
		break;
	default:
		ret = -1;
		break;
	}

	if (ret == 0)
	{
		alarm_message = p_msg;
	}

	delete[] p_msg;

	CPantographMonitorDlg* p_CPantographMonitorDlg = dynamic_cast<CPantographMonitorDlg*>(theApp.m_pMainWnd);
	if(p_CPantographMonitorDlg)
	{
		p_CPantographMonitorDlg->AddWarn(alarm_message.c_str());
	}

	return ret;
}


bool findDevice( const char* p_provider, const char* p_deviceInfos )
{
	if (p_provider == NULL || p_deviceInfos == NULL)
		return false;

	//�Ƚ���Ӧ������������ṩ��"CAM_NETWORK_A80I_CAMERA"
	//const char* pProvider = "CAM_NETWORK_A80I_CAMERA";
	/*
	*�õ����������豸
	*��ʽ���豸��\t�ṩ��\n
	�豸��2\t�ṩ��\n
	*/
	char *p_tempStr = NULL;
	//ǰһ���ַ���
	char *p_prev = const_cast<char*>(p_deviceInfos);
	while(*p_prev != '\0' && (p_tempStr = strstr(p_prev, "\n")) != NULL)
	{
		*p_tempStr = '\0';
		if (char *pTempStr2 = strstr(p_prev, "\t"))
		{
			if (strncmp(p_provider, pTempStr2 + 1, strlen(p_provider)) == 0)return true;
		}
		*p_tempStr = '\n';

		p_prev = p_tempStr + 1;
	}//end of while

	return false;
}


int HandlePisPacket( const std::vector<st_pis_pack_inf>* p_vec_input, std::vector<st_pis_pack_inf>* p_vec_output )
{
	int ret = 0;

	char* pack_data_total = NULL;

	int pack_len_total = 0;
	for(unsigned int i = 0; i < (*p_vec_input).size(); i ++)
	{
		st_pis_pack_inf pis_pack = (*p_vec_input)[i];
		pack_len_total += pis_pack.iDataLength;
	}

	if(pack_len_total > 0)
	{
		pack_data_total = new char[pack_len_total];
	}
	else
	{
		goto LEAVE;
	}

	int pack_len = 0;
	for(unsigned int i = 0; i < (*p_vec_input).size(); i ++)
	{
		st_pis_pack_inf pis_pack = (*p_vec_input)[i];
		memcpy(pack_data_total + pack_len, pis_pack.pData, pis_pack.iDataLength);
		pack_len += pis_pack.iDataLength;
	}

	const int c_pack_length = pack_len_total;
	int data_ind = 0;
	while(1)
	{
		TCP_HEADER pis_head;
		const int c_head_len = sizeof(pis_head);
		const int c_pack_remain = c_pack_length - (data_ind);
		const char* p_pack_data = pack_data_total + data_ind;
		const unsigned char c_dst_head = 0xff;

		char* p_head = NSP_STP_CM::mem_find(p_pack_data, c_pack_remain,  c_dst_head);
		if (p_head >= p_pack_data)
		{
			//ƫ�Ƶ�0xff
			data_ind += (p_head - p_pack_data);

			const int remain_len = c_pack_length - (data_ind);
			if (remain_len > c_head_len)
			{
				memcpy((char*)&pis_head, p_head, c_head_len);
				const int data_size = (pis_head.DataLenth[0] << 8) + pis_head.DataLenth[1];
				const int frame_len = data_size + 5;
				if(remain_len >= frame_len)
				{
					//ƫ�Ƶ���β
					data_ind += frame_len;

					if(pis_head.HEARD == c_dst_head)
					{
						BYTE check_cal = NSP_STP_CM::CalcCheckSum((BYTE *)(p_head), frame_len - 1);
						BYTE check_in = (BYTE)p_head[frame_len - 1];
						if(check_cal == check_in)
						{
							//����һ֡
							st_pis_pack_inf pis_pack;
							pis_pack.iDataLength = frame_len;
							pis_pack.pData = new char[pis_pack.iDataLength];
							memcpy(pis_pack.pData, p_head, frame_len);
							(*p_vec_output).push_back(pis_pack);
						}
						else
						{
							//��У��ʧ��
							continue;
						}
					}
					else
					{
						//��ͷ�Ƿ�
						continue;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

LEAVE:
	if(pack_data_total)
	{
		delete[] pack_data_total;
	}
	return ret;
}

int FormatPisFrame( char* output, TCP_HEADER pis_head, const char* data )
{
	const int data_len = NSP_STP_CM::Hex2Dec(pis_head.DataLenth, 2);
	if (!output)
	{
		return -1;
	}
	if (data_len > 0 && !data)
	{
		return -1;
	}

	const int head_len = sizeof(pis_head);

	int byte_ind = 0;
	memcpy(output + byte_ind, (char*)&pis_head, head_len);
	byte_ind += head_len;
	memcpy(output + byte_ind, data, data_len);
	byte_ind += data_len;

	output[byte_ind] = NSP_STP_CM::CalcCheckSum((BYTE *)output, byte_ind);
	byte_ind++;
	return byte_ind;
}

int UpdataMvbAlarmByFrame(MVB::tag_st_mvb_alarm_data* alarm_data , CCriticalSection* mvb_alarm_lock, const unsigned int alarm_type, const unsigned int alarm_cls, const bool is_self_check, const bool alarm_exist)
{
	int ret = 0;

	mvb_alarm_lock->Lock();

	if (is_self_check)
	{
		switch(alarm_type)
		{
			//"������ģ��
		case E_SELFCHECK_FAULT_TYPE_INFRARED_MONITORING_MODULE:
			alarm_data->infrared_monitoring_module = alarm_exist;
			break;
			//"ȼ�����ģ��
		case E_SELFCHECK_FAULT_TYPE_ARC_MONITORING_MODULE:
			alarm_data->arc_monitoring_module = alarm_exist;
			break;
			//"��̨����
		case E_SELFCHECK_FAULT_TYPE_HOLDER_FAULT:
			alarm_data->holder_fault = alarm_exist;
			break;
			//"����������
		case E_SELFCHECK_FAULT_TYPE_MONITOR_CAMERA_STATUS:
			alarm_data->monitor_camera_status = alarm_exist;
			break;
			//"��������豸
		case E_SELFCHECK_FAULT_TYPE_ROOF_MONITORING_EQUIPMENT:
			alarm_data->roof_monitoring_equipment = alarm_exist;
			break;
			//"���ײ���װ��
		case E_SELFCHECK_FAULT_TYPE_CAR_BOTTOM_COMPENSATOR:
			alarm_data->car_bottom_compensator = alarm_exist;
			break;
			//"����������
		case E_SELFCHECK_FAULT_TYPE_CURRENT_SENSOR:
			alarm_data->current_sensor = alarm_exist;
			break;
			//"���ػ�����
		case E_SELFCHECK_FAULT_TYPE_IPC_FAULT:
			alarm_data->ipc_fault = alarm_exist;
			break;
			//"Ӳ�̹���
		case E_SELFCHECK_FAULT_TYPE_RIGID_DISK_FAULT:
			alarm_data->rigid_disk_fault = alarm_exist;
			break;
			//"���ݷ�������������
		case E_SELFCHECK_FAULT_TYPE_DATA_ANALYSIS_SERVER_STATUS:
			alarm_data->data_analysis_server_status = alarm_exist;
			break;
			//"���β������
		case E_SELFCHECK_FAULT_TYPE_GEOMETRIC_PARAMETER_MONITORING:
			alarm_data->geometric_parameter_monitoring = alarm_exist;
			break;
		default:
			break;
		}

		if (
			alarm_data->infrared_monitoring_module ||
			alarm_data->arc_monitoring_module ||
			alarm_data->holder_fault ||
			alarm_data->monitor_camera_status ||
			alarm_data->roof_monitoring_equipment ||
			alarm_data->car_bottom_compensator ||
			alarm_data->current_sensor ||
			alarm_data->ipc_fault ||
			alarm_data->rigid_disk_fault ||
			alarm_data->data_analysis_server_status ||
			alarm_data->geometric_parameter_monitoring 
			)
		{
			alarm_data->selftest_fault = true;
		}
		else
		{
			alarm_data->selftest_fault = false;
		}
	}
	else
	{
		if (E_FRAME_WARN_CLS_1 == alarm_cls)
		{
			switch(alarm_type)
			{
				//"�����¶�
			case E_FRAME_WARN_TYPE_PANTEM:
				alarm_data->pantograph_temperature_class_1 = alarm_exist;
				break;
				//"ȼ����
			case E_FRAME_WARN_TYPE_ARCRATE:
				alarm_data->arcing_rate_class_1 = alarm_exist;
				break;
				//"����¶�
			case E_FRAME_WARN_TYPE_SLOPE:
				alarm_data->span_slope_class_1 = alarm_exist;
				break;
				//"����ֵ����
			case E_FRAME_WARN_TYPE_GUIDE_HEIGHT:
				alarm_data->guide_height_value_overrun_class_1 = alarm_exist;
				break;
				//"����ֵ����
			case E_FRAME_WARN_TYPE_PULLOUT:
				alarm_data->pull_out_value_oversize_class_1 = alarm_exist;
				break;
				//"��������
			case E_FRAME_WARN_TYPE_FOREIGN_MATTER:
				alarm_data->foreign_matter_invade_class_1 = alarm_exist;
				break;
				//"�ܵ繭�ṹ�쳣
			case E_FRAME_WARN_TYPE_STRUCTURE_ABNORMAL:
				alarm_data->pantograph_structure_abnormal_class_1 = alarm_exist;
				break;
				//"�Ӵ��߸߶Ȳ�
			case E_FRAME_WARN_TYPE_CONTACT_LINE_HEIGHT_DIFFERENCE:
				alarm_data->contact_line_height_difference_class_1 = alarm_exist;
				break;
				//"������б
			case E_FRAME_WARN_TYPE_SLIDE_TILT:
				alarm_data->slide_tilt_class_1 = alarm_exist;
				break;
				//"���嶪ʧ
			case E_FRAME_WARN_TYPE_SKATEBOARD_MISSING:
				alarm_data->skateboard_missing_class_1 = alarm_exist;
				break;
				//"����쳣
			case E_FRAME_WARN_TYPE_HORN_ANOMALY:
				alarm_data->horn_anomaly_class_1 = alarm_exist;
				break;
			default:
				break;
			}

			if (
				alarm_data->pantograph_temperature_class_1 ||
				alarm_data->arcing_rate_class_1 ||
				alarm_data->span_slope_class_1 ||
				alarm_data->guide_height_value_overrun_class_1 ||
				alarm_data->pull_out_value_oversize_class_1 ||
				alarm_data->foreign_matter_invade_class_1 ||
				alarm_data->pantograph_structure_abnormal_class_1 ||
				alarm_data->contact_line_height_difference_class_1 ||
				alarm_data->slide_tilt_class_1 ||
				alarm_data->skateboard_missing_class_1 ||
				alarm_data->horn_anomaly_class_1
				)
			{
				alarm_data->pantograph_and_catenary_alarm = true;
			}
			else
			{
				alarm_data->pantograph_and_catenary_alarm = false;
			}
		}
	}

	mvb_alarm_lock->Unlock();

	return 0;
}

unsigned int CpuMaskExceptBit( unsigned int except_bit )
{
	return 0xF;
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	DWORD processors_num = system_info.dwNumberOfProcessors;
	int max_mask = (int)(0x01 << processors_num) - 1;

	unsigned int new_mask = max_mask ^ except_bit;
	return new_mask;
}

unsigned int CpuMaskRemain()
{
	return 0xF;
	return CpuMaskExceptBit(NSP_STP_CM::E_CPU_MASK_1 | NSP_STP_CM::E_CPU_MASK_2 | NSP_STP_CM::E_CPU_MASK_3);
}

int GetSpareCarrigeNumByCurrent( int carriage_num )
{
	int spare_carriage_num = 0;
	if (carriage_num == 2)
	{
		spare_carriage_num = 5;
	}
	else if (carriage_num == 5)
	{
		spare_carriage_num = 2;
	}
	return spare_carriage_num;
}