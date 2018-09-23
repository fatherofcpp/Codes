/*
次线程以消息的形式通知主线程更新ui,无法直接操作主线程的ui
此线程为后台线程，必须同步执行，如按键调用，初始阀值，量程加载;
    //注意：模态调用进度条，只能在ui线程执行(主对话框及其子对话框都可以，但是非ui线程可能会报错)
*/
#ifndef MYQTHREAD_H
#define MYQTHREAD_H

#include <QThread>
#include "DataTypeClass.h"
#include "csocketunix.h"

class CTransfThread : public QObject
{
    Q_OBJECT
public:
    explicit CTransfThread();
    virtual ~CTransfThread();

signals:
    //异步信号
    void signal_write_i_range_one_finish(st_signal_parm* p_sig_parm);
    void signal_write_i_range_all_finish(st_signal_parm* p_sig_parm);
    void signal_read_threshold_finish(st_signal_parm* p_sig_parm);
    void signal_write_threshold_finish(st_signal_parm* p_sig_parm);
    void signal_read_warn_history_finish(
            st_signal_parm* p_sig_parm,
            st_thread_param_warn_history_get* param_warn_history_get);
    void signal_write_zero_energy_finish(st_signal_parm* p_sig_parm);
    void signal_read_col_conf_finish(st_signal_parm* p_sig_parm);
    void signal_write_col_conf_finish(st_signal_parm* p_sig_parm);
    void signal_write_col_time_finish(st_signal_parm* p_sig_parm);
    void signal_upgrade_finish(st_signal_parm* p_sig_parm);
private slots:
    //异步槽
    void slot_write_i_range_one(st_signal_parm* p_sig_parm,
                                const st_thread_param_i_range_one* param_i_range_one);
    void slot_write_i_range_all(st_signal_parm* p_sig_parm,
                                const st_thread_param_i_range_all* param_i_range_all);
    void slot_read_threshold(st_signal_parm* p_sig_parm);
    void slot_write_threshold(st_signal_parm* p_sig_parm,
                              const st_thread_param_threshold_set* param_threshold_set);
    void slot_read_warn_history(st_signal_parm* p_sig_parm,
                               st_thread_param_warn_history_get* param_warn_history_get);
    void slot_write_zero_energy(st_signal_parm* p_sig_parm,
                                const st_thread_param_zero_energy_set* param_zero_energy_set);
    void slot_read_col_conf(st_signal_parm* p_sig_parm);
    void slot_write_col_conf(st_signal_parm* p_sig_parm);
    void slot_write_col_time(st_signal_parm* p_sig_parm);

    void slot_write_warn_history(const st_warn_history_inf* p_warn_history_inf);
    void slot_write_relay(const st_write_relay_param* p_st_write_relay_param);
    void slot_upgrade(st_signal_parm* p_sig_parm);
};

#endif // MYQTHREAD_H
