#pragma once
#include "DataTypeDef.h"
#include "DataTypeClass.h"

#include "include/xmlparse.h"
class CXmlAnalyze
{
// 构造
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
	void ClearBuff();//清空
	bool IsAdd(st_dev_inf& dev,int port_ind);//根据当前设备信息判断是否为新增
	bool IsDelete(st_dev_inf& dev,int port_ind);//根据初始设备信息，判断该设备是否被删除
	bool IsModify(st_dev_inf& dev,int port_ind);//根据当前设备信息判断是否修改,前提是该设备id已经存在
	int GetPreDevIndByIdSubType(MU_INT64 id,e_dev_sub_type sub_type,int port_ind);//根据设备id子类型找到原来的设备下标
	int GetSubNodeNum(XMLNODE *base_root, const char* name);//在主节点中通过子节点名称查询子节点个数
	int InitCmdIdName();//初始化命令id名称
	int ParseDevInf(XMLNODE *base_root, int port_ind, int dev_ind);//解析xml子节点中的设备信息
	int ParseWayInf(XMLNODE *base_root, int port_ind, int dev_ind, int way_ind);//解析xml子节点中的设备信息
	int ParseServiceInf(XMLNODE *base_root, int service_ind);//解析xml子节点中的设备信息
	int ParseReportFormatInf(XMLNODE *base_root, int dev_type_ind);//解析xml子节点中的设备类型上报方式
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
	//消息头
	void FormMessage(str_communicate_data& data);
	int FormCmdByType(string type_name);
	int FormCmdByXmlData(string xml_data);
	XMLNODE * ParseRspParam(XML& xml);

	//写-开始串口上报
	int FormSetStartReportCmd();
	int ParseSetStartReportRsp();
	//写-停止串口上报
	int FormSetEndReportCmd();
	int ParseSetEndReportRsp();

	//设置配置年龄
	int FormGetAgeCmd();
	int ParseGetAgeRsp();
	int FormSetAgeCmd();
	int ParseSetAgeRsp();

	//读-时间配置
	int FormGetTimeInfCmd();
	int ParseGetTimeInfRsp();
	int FormSetTimeInfCmd();
	int ParseSetTimeInfRsp();

	//读-建筑配置
	int FormGetBuildCmd();
	int ParseGetBuildRsp();
	int FormSetBuildCmd();
	int ParseSetBuildRsp();
	//读-采集器基本配置
	int FormGetCollectorInfCmd();
	int ParseGetCollectorInfRsp();
	int FormSetCollectorInfCmd();
	int ParseSetCollectorInfRsp();
	//读-eth网配置
	int FormGetEthCmd();
	int ParseGetEthRsp();
	int FormSetEthCmd();
	int ParseSetEthRsp();
	//读热点配置：
	int FormGetHotSpotCmd();
	int ParseGetHotSpotRsp();
	int FormSetHotSpotCmd();
	int ParseSetHotSpotRsp();
	//读-服务器配置
	int FormGetServInfCmd();
	int ParseGetServInfRsp();
	int FormSetServInfCmd();
	int ParseSetServInfRsp();
	//读接口设备配置：
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

	//读上报格式配置:
	int FormGetReportFormatCmd();
	int ParseGetReportFormatRsp();
	int FormSetReportFormatCmd();
	int ParseSetReportFormatRsp();

	//写立即生效某项配置：
	int FormSetEffectNowCmd(VEC_STRING& vec_name_string);
	int ParseSetEffectNowRsp();
	//重启设备
	int FormSetReStartCmd();
	int ParseSetReStartRsp();
	//读设备出厂信息
	int FormGetDevFactoryCmd(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type);
	int ParseGetDevFactoryRsp(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type);

	//读自定义子类型信息
	int FormGetDefSubTypeNumCmd();
	int ParseGetDefSubTypeNumRsp(int& num);
	int FormGetDefSubTypeConfCmd(int ind);
	int ParseGetDefSubTypeConfRsp(int ind);
	int FormGetDefSubTypeWayCmd(int def_sub_type_ind, int way_ind);
	int ParseGetDefSubTypeWayRsp(int def_sub_type_ind, int way_ind);


	//写自定义子类型信息
	int FormStartSetDefSubTypeCmd();
	int ParseStartSetDefSubTypeCmd();
	int FormSetDefSubTypeConfCmd(int def_sub_type_ind, int order_ind);
	int ParseSetDefSubTypeConfRsp();
	int FormSetDefSubTypeWayCmd(int order_ind, int def_sub_type_ind, int way_ind);
	int ParseSetDefSubTypeWayRsp();
	int FormEndSetDefSubTypeCmd();
	int ParseEndSetDefSubTypeCmd();

	//自定义子类型本地加载保存
	int FormDefSubTypeLocal(CProtModbus* def_sub_type_modbus);
	int ParseDefSubTypeLocal(CProtModbus* def_sub_type_modbus);
	int DefSubTypeLocalLoad(const char *pfile, CProtModbus* def_sub_type_modbus);
	int DefSubTypeLocalSave(const char *pfile, CProtModbus* def_sub_type_modbus);

	//读-设备实时数据	
	int FormGetRealTimeDataCmd(int port_ind, MU_INT64 dev_id, e_dev_sub_type sub_type, int way_ind);
	int ParseGetRealTimeDataRsp(u_w_measure& w_measure,e_dev_type dev_type);

	//写-清设备历史数据
	int FormSetCleanHisDataCmd();
	int ParseSetCleanHisDataRsp();

	//写-导出历史数据到u盘
	int FormSetCopyHisDataUDiskCmd(st_time& date_time);
	int ParseSetCopyHisDataUDiskRsp();
	
	int FormGetMonthHisDataInfCmd(st_time& date_time);
	int ParseGetMonthHisDataInfRsp(VEC_TIME& data_vec);

	//写-导出日志到u盘
	int FormSetCopyLogUDiskCmd();
	int ParseSetCopyLogUDiskRsp();

	//写-恢复出厂设置
	int FormSetBackFactoryCmd();
	int ParseSetBackFactoryRsp();

	//写-开始远程升级
	int FormSetStartUpgradCmd(int pack_num, int pack_len_max);
	int ParseSetStartUpgradRsp();
	//写-远程升级数据
	int FormSetUpgradDataCmd(int pack_ind,char* data,int len);
	int ParseSetUpgradDataRsp();
	//写-结束远程升级
	int FormSetEndUpgradCmd();
	int ParseSetEndUpgradRsp();

	//写-自适应
	int FormAdaptDevCmd(st_rs485& rs485, CDevAdaptSendFrame* send_frame, int port_ind);
	int ParseAdaptDevRsp();

	//网络通信，心跳
	int FormSetHeartBeatCmd();
	int ParseSetHeartBeatRsp();

	//写-格式化SD卡
	int FormResetSDCardCmd();
	int ParseResetSDCardRsp();

	//////////////////////////////////////////////////////////////////////////
	//读-历史数据
	int FormGetHistoryDataCmd(int pre_result);
	int ParseGetHistoryDataRsp(VEC_HISTORY_XML& vec_history_xml);
	//写-开始读设备历史数据（非删除方式）
	int FormSetStartSaveHistoryDataCmd();
	int ParseSetStartSaveHistoryRsp();
	//保存-历史数据（非删除方式）
	int FormSaveHistoryDataCmd();
	int ParseSaveHistoryDataRsp(CString path, bool use_head);
	//写-结束读设备历史数据（非删除方式）
	int FormSetEndSaveHistoryDataCmd();
	int ParseSetEndSaveHistoryRsp();
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//预留
	//增
	int FormAddDevInfCmd(st_dev_change_inf & dev_inf);
	int ParseAddDevInfRsp();
	//删
	int FormDelDevInfCmd(st_dev_change_inf & dev_inf);
	int ParseDelDevInfRsp();
	//改
	int FormModDevInfCmd(st_dev_change_inf & dev_inf);
	int ParseModDevInfRsp();
	//清空
	int FormClearDevInfCmd();
	int ParseClearDevInfRsp();
	//预留

	////////////////////////////////////////////////////////////////////////////////////////////////
public:
	//信息的修改由1.本地配置2.串口数据3.修改后的界面;
	//其中本地配置和修改后的界面会调用SetColtorInf();串口数据在类内部设置参数。
	void SetColtorInf(st_coltor_inf* coltorInf);
	void SetOuterDev(str_outer_dev* outerDev);
	void SetServiceInf(st_net_conf* serviceInf);
	void SetReportFormatInf(st_report_store_inf* report_format);
	void SetCParamDevDefModbusSet(CProtModbusSet* param_dev_def_modbus_set);

	int LoadLocalXml(const char *pfile);//加载xml本地配置文件
	int SaveLocalXml(const char *pfile);//保存本地xml配置
	void SavePreOuterDev();
	void GetDevChange();

private:
	char* pBuff;//内存
	int buffMallocLen;//内存长度
	//采集器信息
	st_coltor_inf* pColtorInf;
	//外设信息
	str_outer_dev* pOuterDev;
	//服务器信息
	st_net_conf* pServiceInf;
	//上报格式信息
	st_report_store_inf* pReportFormatStoreInf;
	//自定义子类型TODO
	CProtModbusSet* pCProtModbusSet;//协议数组

	//采集器初始外设信息	
	str_outer_dev* pPreOuterDev;
	////////命令类型与命令名称的对应关系
	MAP_INT_STRING mapCmdIdName;//类型，名称

	///////////////////////////////
	//外设增删改信息
	VEC_DEV_CHANGE vecDevComp;//增删，设备信息
	VEC_DEV_CHANGE vecDevAddDelMod;//设备增删改
	///////////////////////////////////////////////////add

};
extern CXmlAnalyze gXmlAnalyze;