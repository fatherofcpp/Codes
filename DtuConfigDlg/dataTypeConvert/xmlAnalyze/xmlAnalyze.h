#pragma once
#include "DataTypeDef.h"
#include "DataTypeClass.h"

#include "include/xmlparse.h"
class CXmlAnalyze
{
// ����
public:
	CXmlAnalyze();	
	~CXmlAnalyze();
public:
	friend class CDtuConfigDlgDlg;
	friend class CSerialAPI;
	friend class CNetApi;
	friend class CTransfer;
	friend class COuterDevPort;
	friend class CAddDev;
private:
	void ClearBuff();//���
	bool IsAdd(st_dev_inf& dev,int port_ind);//���ݵ�ǰ�豸��Ϣ�ж��Ƿ�Ϊ����
	bool IsDelete(st_dev_inf& dev,int port_ind);//���ݳ�ʼ�豸��Ϣ���жϸ��豸�Ƿ�ɾ��
	bool IsModify(st_dev_inf& dev,int port_ind);//���ݵ�ǰ�豸��Ϣ�ж��Ƿ��޸�,ǰ���Ǹ��豸id�Ѿ�����
	int GetPreDevIndByIdSubType(MU_INT64 id,e_dev_sub_type sub_type,int port_ind);//�����豸id�������ҵ�ԭ�����豸�±�
	int GetSubNodeNum(XMLNODE *base_root, const char* name);//�����ڵ���ͨ���ӽڵ����Ʋ�ѯ�ӽڵ����
	int InitCmdIdName();//��ʼ������id����
	int ParseDevInf(XMLNODE *base_root, int port_ind, int dev_ind);//����xml�ӽڵ��е��豸��Ϣ
	int ParseWayInf(XMLNODE *base_root, int port_ind, int dev_ind, int way_ind);//����xml�ӽڵ��е��豸��Ϣ
	int ParseServiceInf(XMLNODE *base_root, int service_ind);//����xml�ӽڵ��е��豸��Ϣ
	int ParseReportFormatInf(XMLNODE *base_root, int dev_type_ind);//����xml�ӽڵ��е��豸�����ϱ���ʽ
	int ParseRealTimeDataInf(XMLNODE *base_root, int param_ind, u_w_measure& w_measure, e_dev_type dev_type);
	
	int LoadFile(const char *pfile);
	int FormLocalConfigXml();
	int ParseLocalXml();

	int ParseTimeNode(XMLNODE* base_root);
	int ParseBuildNode(XMLNODE* base_root);
	int ParseCollectorNode(XMLNODE* base_root);
	int ParseEthNode(XMLNODE* base_root);
	int ParseHotSpotNode(XMLNODE* base_root);
	int ParseServInfNode(XMLNODE* base_root);
	int ParsePortDevNode(XMLNODE* base_root,int port_ind, int dev_ind);
	int ParseReportFormatNode(XMLNODE* base_root);
	int ParseRealTimeDataNode(XMLNODE* base_root, u_w_measure& w_measure, e_dev_type dev_type);
	int ParseDefSubTypeConfNode(XMLNODE* base_root,int def_ind);
	int ParseRegisterInfNode(XMLNODE* base_root, int def_ind, int way_ind);
	int ParseRegNode(XMLNODE* base_root, int def_ind, int way_ind, int param_ind);

public:
	//��Ϣͷ
	void FormMessage(str_communicate_data& data);
	int FormCmdByType(string type_name);
	int FormCmdByXmlData(string xml_data);
	XMLNODE * ParseRspParam(XML& xml);

	//д-��ʼ�����ϱ�
	int FormSetStartReportCmd();
	int ParseSetStartReportRsp();
	//д-ֹͣ�����ϱ�
	int FormSetEndReportCmd();
	int ParseSetEndReportRsp();

	//������������
	int FormGetAgeCmd();
	int ParseGetAgeRsp();
	int FormSetAgeCmd();
	int ParseSetAgeRsp();

	//��-ʱ������
	int FormGetTimeInfCmd();
	int ParseGetTimeInfRsp();
	int FormSetTimeInfCmd();
	int ParseSetTimeInfRsp();

	//��-��������
	int FormGetBuildCmd();
	int ParseGetBuildRsp();
	int FormSetBuildCmd();
	int ParseSetBuildRsp();
	//��-�ɼ�����������
	int FormGetCollectorInfCmd();
	int ParseGetCollectorInfRsp();
	int FormSetCollectorInfCmd();
	int ParseSetCollectorInfRsp();
	//��-eth������
	int FormGetEthCmd();
	int ParseGetEthRsp();
	int FormSetEthCmd();
	int ParseSetEthRsp();
	//���ȵ����ã�
	int FormGetHotSpotCmd();
	int ParseGetHotSpotRsp();
	int FormSetHotSpotCmd();
	int ParseSetHotSpotRsp();
	//��-����������
	int FormGetServInfCmd();
	int ParseGetServInfRsp();
	int FormSetServInfCmd();
	int ParseSetServInfRsp();
	//���ӿ��豸���ã�
	int FormGetPortDevNumCmd(int port_ind);
	int ParseGetPortDevNumRsp(int port_ind);

	int FormGetPortDevCmd(int port_ind, int dev_ind);
	int ParseGetPortDevRsp(int port_ind, int dev_ind);

	int FormStartSetPortCmd(int port_ind);
	int ParseStartSetPortCmd();

	int FormSetPortDevCmd(int port_ind, int dev_ind);
	int ParseSetPortDevRsp();

	int FormEndSetPortCmd(int port_ind);
	int ParseEndSetPortRsp();

	//���ϱ���ʽ����:
	int FormGetReportFormatCmd();
	int ParseGetReportFormatRsp();
	int FormSetReportFormatCmd();
	int ParseSetReportFormatRsp();

	//д������Чĳ�����ã�
	int FormSetEffectNowCmd(VEC_STRING& vec_name_string);
	int ParseSetEffectNowRsp();
	//�����豸
	int FormSetReStartCmd();
	int ParseSetReStartRsp();
	//���豸������Ϣ
	int FormGetDevFactoryCmd(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type);
	int ParseGetDevFactoryRsp(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type);

	//���Զ�����������Ϣ
	int FormGetDefSubTypeNumCmd();
	int ParseGetDefSubTypeNumRsp(int& num);
	int FormGetDefSubTypeConfCmd(int ind);
	int ParseGetDefSubTypeConfRsp(int ind);
	int FormGetDefSubTypeWayCmd(int def_sub_type_ind, int way_ind);
	int ParseGetDefSubTypeWayRsp(int def_sub_type_ind, int way_ind);


	//д�Զ�����������Ϣ
	int FormStartSetDefSubTypeCmd();
	int ParseStartSetDefSubTypeCmd();
	int FormSetDefSubTypeConfCmd(int def_sub_type_ind, int order_ind);
	int ParseSetDefSubTypeConfRsp();
	int FormSetDefSubTypeWayCmd(int order_ind, int def_sub_type_ind, int way_ind);
	int ParseSetDefSubTypeWayRsp();
	int FormEndSetDefSubTypeCmd();
	int ParseEndSetDefSubTypeCmd();

	//�Զ��������ͱ��ؼ��ر���
	int FormDefSubTypeLocal(CProtModbus* def_sub_type_modbus);
	int ParseDefSubTypeLocal(CProtModbus* def_sub_type_modbus);
	int DefSubTypeLocalLoad(const char *pfile, CProtModbus* def_sub_type_modbus);
	int DefSubTypeLocalSave(const char *pfile, CProtModbus* def_sub_type_modbus);

	//��-�豸ʵʱ����	
	int FormGetRealTimeDataCmd(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type, int way_ind);
	int ParseGetRealTimeDataRsp(u_w_measure& w_measure,e_dev_type dev_type);

	//д-���豸��ʷ����
	int FormSetCleanHisDataCmd();
	int ParseSetCleanHisDataRsp();

	//д-������ʷ���ݵ�u��
	int FormSetCopyHisDataUDiskCmd(st_time& date_time);
	int ParseSetCopyHisDataUDiskRsp();
	
	int FormGetMonthHisDataInfCmd(st_time& date_time);
	int ParseGetMonthHisDataInfRsp(VEC_TIME& data_vec);

	//д-������־��u��
	int FormSetCopyLogUDiskCmd();
	int ParseSetCopyLogUDiskRsp();

	//д-�ָ���������
	int FormSetBackFactoryCmd();
	int ParseSetBackFactoryRsp();

	//д-��ʼԶ������
	int FormSetStartUpgradCmd(int pack_num, int pack_len_max);
	int ParseSetStartUpgradRsp();
	//д-Զ����������
	int FormSetUpgradDataCmd(int pack_ind,char* data,int len);
	int ParseSetUpgradDataRsp();
	//д-����Զ������
	int FormSetEndUpgradCmd();
	int ParseSetEndUpgradRsp();

	//д-����Ӧ
	int FormAdaptDevCmd(st_rs485& rs485, CDevAdaptSendFrame* send_frame, int port_ind);
	int ParseAdaptDevRsp();

	//����ͨ�ţ�����
	int FormSetHeartBeatCmd();
	int ParseSetHeartBeatRsp();

	//д-��ʽ��SD��
	int FormResetSDCardCmd();
	int ParseResetSDCardRsp();

	//////////////////////////////////////////////////////////////////////////
	//��-��ʷ����
	int FormGetHistoryDataCmd(int pre_result);
	int ParseGetHistoryDataRsp(VEC_HISTORY_XML& vec_history_xml);
	//д-��ʼ���豸��ʷ���ݣ���ɾ����ʽ��
	int FormSetStartSaveHistoryDataCmd();
	int ParseSetStartSaveHistoryRsp();
	//����-��ʷ���ݣ���ɾ����ʽ��
	int FormSaveHistoryDataCmd();
	int ParseSaveHistoryDataRsp(CString path, bool use_head);
	//д-�������豸��ʷ���ݣ���ɾ����ʽ��
	int FormSetEndSaveHistoryDataCmd();
	int ParseSetEndSaveHistoryRsp();
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Ԥ��
	//��
	int FormAddDevInfCmd(st_dev_change_inf & dev_inf);
	int ParseAddDevInfRsp();
	//ɾ
	int FormDelDevInfCmd(st_dev_change_inf & dev_inf);
	int ParseDelDevInfRsp();
	//��
	int FormModDevInfCmd(st_dev_change_inf & dev_inf);
	int ParseModDevInfRsp();
	//���
	int FormClearDevInfCmd();
	int ParseClearDevInfRsp();
	//Ԥ��

	////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//��Ϣ���޸���1.��������2.��������3.�޸ĺ�Ľ���;
	//���б������ú��޸ĺ�Ľ�������SetColtorInf();�������������ڲ����ò�����
	void SetColtorInf(st_coltor_inf* coltorInf);
	void SetOuterDev(str_outer_dev* outerDev);
	void SetServiceInf(st_net_conf* serviceInf);
	void SetReportFormatInf(st_report_store_inf* report_format);
	void SetCParamDevDefModbusSet(CProtModbusSet* param_dev_def_modbus_set);

	int LoadLocalXml(const char *pfile);//����xml���������ļ�
	int SaveLocalXml(const char *pfile);//���汾��xml����
	void SavePreOuterDev();
	void GetDevChange();

private:
	char* pBuff;//�ڴ�
	int buffMallocLen;//�ڴ泤��
	//�ɼ�����Ϣ
	st_coltor_inf* pColtorInf;
	//������Ϣ
	str_outer_dev* pOuterDev;
	//��������Ϣ
	st_net_conf* pServiceInf;
	//�ϱ���ʽ��Ϣ
	st_report_store_inf* pReportFormatStoreInf;
	//�Զ���������TODO
	CProtModbusSet* pCProtModbusSet;//Э������

	//�ɼ�����ʼ������Ϣ	
	str_outer_dev* pPreOuterDev;
	////////�����������������ƵĶ�Ӧ��ϵ
	MAP_INT_STRING mapCmdIdName;//���ͣ�����

	///////////////////////////////
	//������ɾ����Ϣ
	VEC_DEV_CHANGE vecDevComp;//��ɾ���豸��Ϣ
	VEC_DEV_CHANGE vecDevAddDelMod;//�豸��ɾ��
	///////////////////////////////////////////////////add

};
extern CXmlAnalyze gXmlAnalyze;