#include "ccommonchild.h"
#include "transfthread.h"


CCommonChild::CCommonChild()
{
}

CCommonChild::~CCommonChild()
{
}

void CCommonChild::SetDialogInd(int val)
{
    dialogInd = val;
}

QString GetTaskNameByType(int tsk_type)
{
    QString pro_name = "";
    switch(tsk_type)
    {
    case E_READ_CURRENT_RANGE_CMD:
        pro_name = "读电流量程";
        break;
    case E_WRITE_CURRENT_RANGE_ONE_CMD:
        pro_name = "设置电流量程";
        break;
    case E_WRITE_CURRENT_RANGE_ALL_CMD:
        pro_name = "设置电流量程";
        break;
    case E_READ_THRESHOLD_CMD:
        pro_name = "读报警阀值";
        break;
    case E_WRITE_THRESHOLD_CMD:
        pro_name = "写报警阀值";
        break;
    case E_READ_WARN_HISTORY_CMD:
        pro_name = "读历史告警";
        break;
    case E_WRITE_ZERO_ENERGY_CMD:
        pro_name = "清空电能";
        break;
    case E_READ_COLTOR_CMD:
        pro_name = "读采集器信息";
        break;
    case E_WRITE_COLTOR_CMD:
        pro_name = "写采集器信息";
        break;
    case E_WRITE_COLTOR_TIME_CMD:
        pro_name = "写采集器时间";
        break;
    case E_WRITE_UPGRADE_CMD:
        pro_name = "优盘升级";
        break;
    default:
        pro_name = "";
        break;
    }
    return pro_name;
}

void messagebox(QWidget *parent, int res, bool show_suc)
{
    if(res == 0)
    {
        if(show_suc)
        {
//            QMessageBox::information(parent, "操作", "成功");
        }
    }
    else
    {
        ParseTskResult(res);
        QMessageBox::information(parent, "操作", "失败");
    }
}

void close_progress(CProgressDialog* p_CProgressDialog)
{
    p_CProgressDialog->ControlTimer(false);
    p_CProgressDialog->close();
}

void open_progress(CProgressDialog* p_CProgressDialog, QString pro_name)
{
    p_CProgressDialog->setModal(true);
    p_CProgressDialog->SetProgressName(pro_name);
    p_CProgressDialog->SetAutoProgress(true);
    p_CProgressDialog->SetEnterTime(0);
    p_CProgressDialog->ControlTimer(true);
    p_CProgressDialog->exec();
}


int LogIn(QWidget *parent, CLoginDialog* p_CLoginDialog)
{
    p_CLoginDialog->setModal(true);
    int ret = 0;
    ret = p_CLoginDialog->exec();
    if (ret == QDialog::Accepted)
    {
        ret = 0;
    }
    else
    {
        ret = -1;        
    }
    return ret;
}
