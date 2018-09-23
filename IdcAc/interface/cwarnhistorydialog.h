#ifndef CWARNHISTORYDIALOG_H
#define CWARNHISTORYDIALOG_H

#include <QDialog>
#include "ccommonchild.h"
#include "DataTypeDef.h"

typedef struct
{
    st_signal_parm sig_parm;
    st_thread_param_warn_history_get warn_history_get;
}st_signal_parm_history_warn;

namespace Ui {
class CWarnHistoryDialog;
}

class CWarnHistoryDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CWarnHistoryDialog(QWidget *parent = 0);
    ~CWarnHistoryDialog();
    
private:
    Ui::CWarnHistoryDialog *ui;
signals:
    void signal_read_warn_history(st_signal_parm* p_sig_parm,
                                 st_thread_param_warn_history_get* param_warn_history_get);
private slots:
    void slot_read_warn_history_finish(st_signal_parm* p_sig_parm,
            st_thread_param_warn_history_get* param_warn_history_get);
signals:
    void signal_change_child_dialog(int ind);
private slots:
    void slot_to_main_page_clicked();
    void slot_pre_page_clicked();
    void slot_next_page_clicked();
    void slot_pre_list_clicked();
    void slot_next_list_clicked();
public:
    int GetHistoryWarn();
private:
    void InitUi();
    void SetList(int scroller);
    void ClearWarnInf();
    void SetScrollPosition(int scroller);
private: 
    CProgressDialog* pCProgressDialog;
    st_signal_parm_history_warn signalParmHisWarn;
};

#endif // CWARNHISTORYDIALOG_H
