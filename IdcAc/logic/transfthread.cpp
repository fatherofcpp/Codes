#include "transfthread.h"
#include "include_all_qt.h"
#include "xmlAnalyze.h"
#include "Log.h"

static int GetThreShold();
static int SetThreShold(const st_thread_param_threshold_set& param_threshold_set);
static int GetWarnHistory(st_thread_param_warn_history_get& param_warn_history_get);
static int GetColConf();
static int SetColConf();
static int SetColTime();
static int SetWarnHistory(const st_warn_history_inf* warn_history_inf);
static int SetUpgrade();

CTransfThread::CTransfThread()
{

}

CTransfThread::~CTransfThread()
{

}

int GetThreShold()
{
    //    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    int len = gXmlAnalyze.FormGetThresholdCmd();
    int rtn = g_pTransf->SendReqAndReceiveBody(len, E_READ_THRESHOLD_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseGetThresholdRsp();
        if(rtn == 0)
        {
            rtn = 0;
            goto LEAVE;
        }
        else
        {
            gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            rtn = -1;
            goto LEAVE;
        }
    }
    else
    {
        rtn = -2;
        goto LEAVE;
    }
LEAVE:
    g_pTransf->doUnLock();
    //    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int SetThreShold(const st_thread_param_threshold_set& param_threshold_set)
{
    //    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    int len = gXmlAnalyze.FormSetThresholdCmd(param_threshold_set.vec_threshold_item);
    int rtn = g_pTransf->SendReqAndReceiveBody(len, E_WRITE_THRESHOLD_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseSetThresholdRsp();
        if(rtn == 0)
        {
            goto LEAVE;
        }
        else
        {
            gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            rtn = -1;
            goto LEAVE;
        }
    }
    else
    {
        rtn = -2;
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
    //    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int GetWarnHistory(st_thread_param_warn_history_get& param_warn_history_get)
{
    //    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    int len = gXmlAnalyze.FormGetWarnHistoryCmd(param_warn_history_get.off_set);
    int rtn = g_pTransf->SendReqAndReceiveBody(len, E_READ_WARN_HISTORY_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseGetWarnHistoryRsp(param_warn_history_get.deq_st_warn_history);
        if(rtn == 0)
        {
            rtn = 0;
            goto LEAVE;
        }
        else
        {
            gLog.Add("[%s]Parse fail rtn=%d[%s]", __FUNCTION__, rtn,
                     (char*)(gXmlAnalyze.GetBuf()));
            rtn = -1;
            goto LEAVE;
        }
    }
    else
    {
        gLog.Add("%s 2",__FUNCTION__);
        rtn = -2;
        goto LEAVE;
    }
LEAVE:
    g_pTransf->doUnLock();
    //    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int GetColConf()
{
    //    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    int len = gXmlAnalyze.FormGetCollectorInfCmd();
    int rtn = g_pTransf->SendReqAndReceiveBody(len, E_READ_COLTOR_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseGetCollectorInfRsp();
        if(rtn == 0)
        {
            rtn = 0;
            goto LEAVE;
        }
        else
        {
            gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            rtn = -1;
            goto LEAVE;
        }
    }
    else
    {
        rtn = -2;
        goto LEAVE;
    }
LEAVE:
    g_pTransf->doUnLock();
    //    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int SetColConf()
{
    //    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    int len = gXmlAnalyze.FormSetCollectorInfCmd();
    int rtn = g_pTransf->SendReqAndReceiveBody(len, E_WRITE_COLTOR_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseSetCollectorInfRsp();
        if(rtn == 0)
        {
            rtn = 0;
            goto LEAVE;
        }
        else
        {
            gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            rtn = -1;
            goto LEAVE;
        }
    }
    else
    {
        rtn = -2;
        goto LEAVE;
    }
LEAVE:
    g_pTransf->doUnLock();
    //    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int SetColTime()
{
    //    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    int len = gXmlAnalyze.FormSetTimeInfCmd();
    int rtn = g_pTransf->SendReqAndReceiveBody(len, E_WRITE_COLTOR_TIME_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseSetTimeInfRsp();
        if(rtn == 0)
        {
            rtn = 0;
            goto LEAVE;
        }
        else
        {
            gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            rtn = -1;
            goto LEAVE;
        }
    }
    else
    {
        rtn = -2;
        goto LEAVE;
    }
LEAVE:
    g_pTransf->doUnLock();
    //    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

void CTransfThread::slot_write_i_range_one(st_signal_parm* p_sig_parm,
                                           const st_thread_param_i_range_one *param_i_range_one)
{
    CProModbus* p_CProModbus = new CProModbus;
    p_sig_parm->is_suc = p_CProModbus->set_range_one(
                param_i_range_one->reg_val,
                param_i_range_one->reg_addr,
                param_i_range_one->dev_addr);
    emit this->signal_write_i_range_one_finish(p_sig_parm);
    delete p_CProModbus;
}

void CTransfThread::slot_write_i_range_all(st_signal_parm* p_sig_parm,
                                           const st_thread_param_i_range_all *param_i_range_all)
{
    CProModbus* p_CProModbus = new CProModbus;
    p_sig_parm->is_suc = p_CProModbus->set_range_all(
                param_i_range_all->vec_val,
                param_i_range_all->dev_addr,
                param_i_range_all->reg_addr);
    emit this->signal_write_i_range_all_finish(p_sig_parm);
    delete p_CProModbus;
}

void CTransfThread::slot_read_threshold(st_signal_parm* p_sig_parm)
{
    p_sig_parm->is_suc = GetThreShold();
    emit this->signal_read_threshold_finish(p_sig_parm);
}

void CTransfThread::slot_write_threshold(st_signal_parm* p_sig_parm,
                                         const st_thread_param_threshold_set *param_threshold_set)
{
    p_sig_parm->is_suc = SetThreShold(*param_threshold_set);
    emit this->signal_write_threshold_finish(p_sig_parm);
}

void CTransfThread::slot_read_warn_history(st_signal_parm* p_sig_parm,
                                           st_thread_param_warn_history_get* param_warn_history_get)
{
    p_sig_parm->is_suc = GetWarnHistory(*param_warn_history_get);
    emit this->signal_read_warn_history_finish(p_sig_parm, param_warn_history_get);
}

void CTransfThread::slot_write_zero_energy(st_signal_parm* p_sig_parm,
                                           const st_thread_param_zero_energy_set *param_zero_energy_set)
{
    CProModbus* p_CProModbus = new CProModbus;
    p_sig_parm->is_suc = p_CProModbus->set_zero_energy(
                param_zero_energy_set->reg_val,
                param_zero_energy_set->dev_addr);
    emit this->signal_write_zero_energy_finish(p_sig_parm);
    delete p_CProModbus;
}

void CTransfThread::slot_read_col_conf(st_signal_parm *p_sig_parm)
{
    p_sig_parm->is_suc = GetColConf();
    emit this->signal_read_col_conf_finish(p_sig_parm);
}

void CTransfThread::slot_write_col_conf(st_signal_parm *p_sig_parm)
{
    p_sig_parm->is_suc = SetColConf();
    emit this->signal_write_col_conf_finish(p_sig_parm);
}

void CTransfThread::slot_write_col_time(st_signal_parm *p_sig_parm)
{
    p_sig_parm->is_suc = SetColTime();
    emit this->signal_write_col_time_finish(p_sig_parm);
}

void CTransfThread::slot_write_warn_history(const st_warn_history_inf *p_warn_history_inf)
{
    SetWarnHistory(p_warn_history_inf);
    delete p_warn_history_inf;

    //    static int cnt = 0;
    //    qDebug("[%s] enter cnt=%d", __FUNCTION__, cnt++);
}

void CTransfThread::slot_write_relay(const st_write_relay_param *p_st_write_relay_param)
{
    CProModbus* p_CProModbus = new CProModbus;
    p_CProModbus->set_relay(p_st_write_relay_param->relay_val,
                            p_st_write_relay_param->dev_addr,
                            p_st_write_relay_param->reg_addr);
    delete p_CProModbus;
    delete p_st_write_relay_param;

    //    static int cnt = 0;
    //    qDebug("[%s] enter cnt=%d", __FUNCTION__, cnt++);
}

void CTransfThread::slot_upgrade(st_signal_parm *p_sig_parm)
{
    p_sig_parm->is_suc = SetUpgrade();
    emit this->signal_upgrade_finish(p_sig_parm);
}

int SetWarnHistory(const st_warn_history_inf* warn_history_inf)
{
    //    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    int len = gXmlAnalyze.FormSetWarnHistoryCmd(warn_history_inf);
    int rtn = g_pTransf->SendReqAndReceiveBody(len, E_WRITE_WARN_HISTORY_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseSetWarnHistoryRsp();
        if(rtn == 0)
        {
            rtn = 0;
            goto LEAVE;
        }
        else
        {
            gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            rtn = -1;
            goto LEAVE;
        }
    }
    else
    {
        rtn = -2;
        goto LEAVE;
    }
LEAVE:
    g_pTransf->doUnLock();
    //    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int SetUpgrade()
{
    //    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    int len = gXmlAnalyze.FormSetUpgradeCmd();
    int rtn = g_pTransf->SendReqAndReceiveBody(len, E_WRITE_UPGRADE_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseSetUpgradRsp();
        if(rtn == 0)
        {
            rtn = 0;
            goto LEAVE;
        }
        else
        {
            gLog.Add("[%s]Parse fail -1[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            rtn = -1;
            goto LEAVE;
        }
    }
    else
    {
        rtn = -2;
        goto LEAVE;
    }
LEAVE:
    g_pTransf->doUnLock();
    //    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}


