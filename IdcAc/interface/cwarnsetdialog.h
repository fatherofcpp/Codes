#ifndef CWARNSETDIALOG_H
#define CWARNSETDIALOG_H

#include <QDialog>
#include "keyboard/myqpushbutton.h"
#include "keyboard/myqlineedit.h"
#include "ccommonchild.h"
#include "DataTypeDef.h"
#include "DataTypeDef.h"

namespace Ui {
class CWarnSetDialog;
}

#define MAIN_WAY_PARAM_NUM 14

class CWarnSetDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CWarnSetDialog(QWidget *parent = 0);
    ~CWarnSetDialog();

    virtual void SetDialogInd(int val);
private:
    Ui::CWarnSetDialog *ui;
signals:
    void signal_write_threshold(st_signal_parm* p_sig_parm,
                                const st_thread_param_threshold_set* param_threshold_set);
private slots:
    void slot_write_threshold_finish(st_signal_parm* p_sig_parm);
signals:
    void signal_change_child_dialog(int ind);
private slots:
    void slot_to_main_page_clicked();
    void slot_pre_page_clicked();
    void slot_next_page_clicked();

    void slot_light_clicked(const st_bt_inf& pos);
    void slot_save_clicked(const st_bt_inf& pos);
public:
    void SetOuterDev(str_outer_dev* outer_dev);
private:
    void InitUi();
    void SetLight(int ui_ind, bool is_on);
    MU_INT32* MapUi2Val(st_way_data* p_st_way_data, int ui_ind);
    MU_INT32 MapUi2WarnType(int ui_ind);
private:
    str_outer_dev* pOuterDev;
    CMyQPushButton* lightBt[MAIN_WAY_PARAM_NUM];
    CMyQPushButton* saveBt[MAIN_WAY_PARAM_NUM];
    CMyQLineEdit* threEdit[MAIN_WAY_PARAM_NUM];

    CProgressDialog* pCProgressDialog;
    st_signal_parm signalParm;
};

#endif // CWARNSETDIALOG_H
