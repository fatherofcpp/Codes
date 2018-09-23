#ifndef CWARNTHRESHOLDDIALOG_H
#define CWARNTHRESHOLDDIALOG_H

#include <QDialog>
#include "keyboard/myqpushbutton.h"
#include "keyboard/myqlineedit.h"
#include "ccommonchild.h"
#include "DataTypeDef.h"

namespace Ui {
class CWarnThresholdDialog;
}


class CWarnThresholdDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CWarnThresholdDialog(QWidget *parent = 0);
    ~CWarnThresholdDialog();
    
    virtual void SetDialogInd(int val);
private:
    Ui::CWarnThresholdDialog *ui;
signals:
    void signal_write_i_range_one(st_signal_parm* p_sig_parm,
                                  const st_thread_param_i_range_one* param_i_range_one);
    void signal_write_i_range_all(st_signal_parm* p_sig_parm,
                                  const st_thread_param_i_range_all* param_i_range_all);
    void signal_write_threshold(st_signal_parm* p_sig_parm,
                                const st_thread_param_threshold_set* param_threshold_set);
    void signal_i_range_update(st_signal_parm* p_sig_parm,
                               bool is_update);
    void signal_is_get_threshold(st_signal_parm* p_sig_parm,
                                 bool is_get);
private slots:
    void slot_write_i_range_one_finish(st_signal_parm* p_sig_parm);
    void slot_write_i_range_all_finish(st_signal_parm* p_sig_parm);
    void slot_write_threshold_finish(st_signal_parm* p_sig_parm);
signals:
    void signal_change_child_dialog(int ind);
private slots:
    void slot_to_main_page_clicked();
    void slot_pre_page_clicked();
    void slot_next_page_clicked();
    void slot_batch_change_clicked();
    void slot_batch_set_clicked();
    void slot_save_m_clicked(const st_bt_inf& pos);

    void slot_light_clicked(const st_bt_inf& pos);
    void slot_save_clicked(const st_bt_inf& pos);
public:
    void SetTitle(QString& str_title);
    void SetUi(const st_dev_inf* p_dev_inf);
    void SetOuterDev(str_outer_dev *outer_dev);
private:
    void InitUi();
    void SetLight(int ui_ind, bool is_on);
    MU_INT32* MapDialogInd2Val(st_way_data* p_st_way_data);
    MU_INT32 MapDialogInd2WarnType();
private:
    str_outer_dev* pOuterDev;//阀值使用

    CMyQPushButton* lightBt[SUB_WAY_PARAM_NUM];
    CMyQPushButton* saveBt[SUB_WAY_PARAM_NUM];
    CMyQLineEdit* threEdit[SUB_WAY_PARAM_NUM];
    QLabel* labelW[SUB_WAY_PARAM_NUM];

    CProgressDialog* pCProgressDialog;
    st_signal_parm signalParm;
};

#endif // CWARNTHRESHOLDDIALOG_H
