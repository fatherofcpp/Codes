#ifndef CCOMMONCHILD_H
#define CCOMMONCHILD_H
#include <QDialog>
#include <QMessageBox>
#include "progressdialog.h"
#include "DataTypeDef.h"
#include "logindialog.h"
//////////////////////
QString GetTaskNameByType(int tsk_type);
////////////////////
//信号槽函数接口
////////////////////
void messagebox(QWidget *parent, int res, bool show_suc = true);
void close_progress(CProgressDialog* p_CProgressDialog);
void open_progress(CProgressDialog* p_CProgressDialog, QString pro_name);
int LogIn(QWidget *parent, CLoginDialog* p_CLoginDialog);
///////////////////////////

///////////////////////////
typedef enum
{
    E_SUB_WAY_A = 0,
    E_SUB_WAY_B,
    E_SUB_WAY_NUM
}e_sub_way;

typedef enum
{
    E_U_A = 0,
    E_U_B,
    E_U_C,
    E_I_A,
    E_I_B,
    E_I_C,
    E_U_I_NUM
}e_u_i;

typedef enum
{
    E_CURRENT_LOW_DIALOG = 0,
    E_CURRENT_HIGH_DIALOG,
    E_CURRENT_LOW2_DIALOG,
    E_CURRENT_HIGH2_DIALOG,
    E_FUSE_DIALOG,
    E_LOSE_PHASE_DIALOG,
    E_CONTENT_DIALOG,
    E_SUB_WAY_THRESHOLD_NUM
}e_sub_way_threshold_dialog;

typedef enum
{
    E_INIT_DIALOG = 0,//初始化
    E_MAIN_PAGE_DIALOG,//主页
    E_ELE_MAIN_WAY_DIALOG_A,//主路A电参数;主路B电参数
    E_ELE_MAIN_WAY_DIALOG_B,
    E_ELE_SUB_WAY_DIALOG,//支路电参数,列表中包含A，B
    E_HARM_PERCENT_DIALOG,//谐波百分比
    E_HARM_U_I_DIALOG_A_UA,//主路A电压谐波;主路A电流谐波;主路B电压谐波;主路B电流谐波
    E_HARM_U_I_DIALOG_A_UB,
    E_HARM_U_I_DIALOG_A_UC,
    E_HARM_U_I_DIALOG_A_IA,
    E_HARM_U_I_DIALOG_A_IB,
    E_HARM_U_I_DIALOG_A_IC,
    E_HARM_U_I_DIALOG_B_UA,
    E_HARM_U_I_DIALOG_B_UB,
    E_HARM_U_I_DIALOG_B_UC,
    E_HARM_U_I_DIALOG_B_IA,
    E_HARM_U_I_DIALOG_B_IB,
    E_HARM_U_I_DIALOG_B_IC,
    E_SWITCH_STATE_DIALOG_A,//主路A及其支路开关状态;主路B及其支路开关状态
    E_SWITCH_STATE_DIALOG_B,
    E_LOAD_PERCENT_DIALOG_A,//主路A及其支路负载百分比;主路B及其支路负载百分比
    E_LOAD_PERCENT_DIALOG_B,
    E_POWER_CHECK_DIALOG_A,//主路A及其支路功率判断;主路B及其支路功率判断
    E_POWER_CHECK_DIALOG_B,
    E_WARN_CURRENT_DIALOG,//当前报警
    E_WARN_HISTORY_DIALOG,//历史报警
    E_WARN_SET_DIALOG_A,//主路A报警设置;主路B报警设置
    E_WARN_SET_DIALOG_B,
    //支路A电流下限报警值;支路B电流下限报警值
    //支路A电流上限报警值;支路B电流上限报警值
    //支路A电流下下限报警值;支路B电流下下限报警值
    //支路A电流上上限报警值;支路B电流上上限报警值
    //支路A漏电流上限报警值;支路B漏电流上限报警值
    //支路A熔断器报警值;支路B熔断器报警值
    //支路A缺相值（电压）;支路B缺相值（电压）
    //支路A容量(电流）;支路B容量(电流）
    E_WARN_THRESHOLD_DIALOG_A_I_DOWN,
    E_WARN_THRESHOLD_DIALOG_A_I_UP,
    E_WARN_THRESHOLD_DIALOG_A_I_DOWN2,
    E_WARN_THRESHOLD_DIALOG_A_I_UP2,
    E_WARN_THRESHOLD_DIALOG_A_FUSE,
    E_WARN_THRESHOLD_DIALOG_A_LOSE_PHASE,
    E_WARN_THRESHOLD_DIALOG_A_CONTENT,
    E_WARN_THRESHOLD_DIALOG_B_I_DOWN,
    E_WARN_THRESHOLD_DIALOG_B_I_UP,
    E_WARN_THRESHOLD_DIALOG_B_I_DOWN2,
    E_WARN_THRESHOLD_DIALOG_B_I_UP2,
    E_WARN_THRESHOLD_DIALOG_B_FUSE,
    E_WARN_THRESHOLD_DIALOG_B_LOSE_PHASE,
    E_WARN_THRESHOLD_DIALOG_B_CONTENT,

    E_ClEAR_DIALOG,//清除功能
    E_USER_MANAGE_DIALOG,//用户管理
    E_CHILD_DIALOG_NUM
}e_child_dialog_ind;


class CCommonChild
{
public:
    CCommonChild();
    virtual ~CCommonChild();

public:
    virtual void SetDialogInd(int val);
public:
    MU_INT32 dialogInd;
};

#endif // CCOMMONCHILD_H
