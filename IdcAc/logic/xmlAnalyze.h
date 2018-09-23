#pragma once
#include "include/xmlparse.h"
#include "DataTypeDef.h"
#include "promodbus.h"

class CXmlAnalyze
{
    // 构造
public:
    CXmlAnalyze();
    ~CXmlAnalyze();
public:
    //消息头
    void FormMessage(str_communicate_data& data);
    int FormCmdByType(string type_name);
    int FormCmdByXmlData(string xml_data);
    XMLNODE* ParseRspParam(XML& xml);

    //设置配置年龄
    int FormGetAgeCmd();
    int ParseGetAgeRsp(int& age);
    int FormSetAgeCmd();
    int ParseSetAgeRsp();

    //dtu: 读实时数据、写实时报警数据、读电流量程、写电流量程、写电能清零
    int FormDtuCmd(string dtu_data);
    int ParseDtuRsp(string& dtu_data);

    //读历史报警数据
    int FormGetWarnHistoryCmd(int off_set);
    int ParseGetWarnHistoryRsp(DEQ_ST_WARN_HISTORY_INF& vec_st_warn_history);
    //写历史报警数据
    int FormSetWarnHistoryCmd(const st_warn_history_inf* warn_history_inf);
    int ParseSetWarnHistoryRsp();

    //读报警阀值
    int FormGetThresholdCmd();
    int ParseGetThresholdRsp();
    //写报警阀值
    int FormSetThresholdCmd(const VEC_THRESHOLD_ITEM& vec_threshold_item);
    int ParseSetThresholdRsp();

    //配置采集器基本配置
    int FormGetCollectorInfCmd();
    int ParseGetCollectorInfRsp();
    int FormSetCollectorInfCmd();
    int ParseSetCollectorInfRsp();

    //配置采集器时间
    int FormSetTimeInfCmd();
    int ParseSetTimeInfRsp();

    //升级
    int FormSetUpgradeCmd();
    int ParseSetUpgradRsp();

public:
    int LoadLocalXml(const char *pfile);//加载xml本地配置文件
    int LoadUserGroup(const char *pfile);
    int SaveUserGroup(const char *pfile);//保存本地xml配置
private:
    int LoadFile(const char *pfile);
    int ParseLocalXml();
    int ParsePortDevNode(XMLNODE* base_root,int port_ind, int dev_ind);
    int ParseDevInf(XMLNODE *base_root, int port_ind, int dev_ind);//解析xml子节点中的设备信息
    int ParseUserGroup();
    int ParseUserNode(XMLNODE* base_root, st_user_inf* p_user_inf);
private:
    int InitCmdIdName();//初始化命令id名称

    int GetSubNodeNum(XMLNODE *base_root, const char* name);//在主节点中通过子节点名称查询子节点个数
    int ParseWarnEventDataNode(XMLNODE* base_root, DEQ_ST_WARN_HISTORY_INF& vec_st_warn_history);
    int ParseWarnEventDataWNode(XMLNODE* base_root, int param_ind, st_warn_history_inf& warn_history);
    int ParseWayInf(XMLNODE *base_root, int port_ind, int dev_ind, int way_ind);//解析xml子节点中的设备信息
    int ParseThreSholdNode(XMLNODE *base_root);
    int ParseThreSholdINode(XMLNODE *base_root);
    int ParseCollectorNode(XMLNODE* base_root);
public:
    void ClearBuff();//清空
    int GetBufLen(){return buffMallocLen;}
    char* GetBuf(){return pBuff;}
    str_outer_dev* GetOuterDev(){return pOuterDev;}
    st_coltor_inf* GetColInf(){return pColtorInf;}
    DEQ_ST_USER_INF* GetUserInf(){return &deqStUserInf;}
private:
    char* pBuff;//内存
    int buffMallocLen;//内存长度

    ////////命令类型与命令名称的对应关系
    MAP_INT_STRING mapCmdIdName;//类型，名称

    str_outer_dev* pOuterDev;//外设信息
    //采集器信息
    st_coltor_inf* pColtorInf;

    //用户信息
    DEQ_ST_USER_INF deqStUserInf;
};
extern CXmlAnalyze gXmlAnalyze;
