#ifndef CWARNCURRENTDIALOG_H
#define CWARNCURRENTDIALOG_H

#include <QDialog>
#include "ccommonchild.h"
#include "DataTypeDef.h"

namespace Ui {
class CWarnCurrentDialog;
}

class CWarnCurrentDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CWarnCurrentDialog(QWidget *parent = 0);
    ~CWarnCurrentDialog();
    
private:
    Ui::CWarnCurrentDialog *ui;
signals:
    void signal_change_child_dialog(int ind);
    void signal_write_warn_history(const st_warn_history_inf* p_st_warn_history);
    void signal_write_relay(const st_write_relay_param* p_write_relay_param);
private slots:
    void slot_to_main_page_clicked();
    void slot_pre_page_clicked();
    void slot_next_page_clicked();
    void slot_pre_list_clicked();
    void slot_next_list_clicked();
    void slot_is_auto_2_warn_dialog(bool is_auto_2_warn_dialog);
    void slot_bt_close_sound_clicked();
public:
    void SetUi(const st_dev_inf* p_dev_inf);
    void SetOuterDev(const str_outer_dev* outer_dev);
private:
    void InitUi();
    void SetList();
    void WarnCheck(st_warn_history_inf* p_warn_history_inf);
    void AddWarn(st_warn_history_inf* p_warn_history_inf);
    void DelWarn(st_warn_history_inf* p_warn_history_inf);
private:
    MU_INT32 rowMax;
    const str_outer_dev* pOuterDev;
    DEQ_ST_WARN_HISTORY_INF deqStWarnCurrentInf;
    bool isAuto2WarnDialog;
};

#endif // CWARNCURRENTDIALOG_H
