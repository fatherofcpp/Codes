#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "progressdialog.h"
#include "transfthread.h"
#include "timethread.h"
#include "realtimedatathread.h"
#include "DataTypeStruct.h"

#include "cinitdialog.h"
#include "cmainpagedialog.h"
#include "celemainwaydialog.h"
#include "celesubwaydialog.h"
#include "charmpercentdialog.h"
#include "charmiudialog.h"
#include "cswitchstatedialog.h"
#include "cloadpercentdialog.h"
#include "cpowercheckdialog.h"
#include "cwarncurrentdialog.h"
#include "cwarnhistorydialog.h"
#include "cwarnsetdialog.h"
#include "cwarnthresholddialog.h"
#include "ccleardialog.h"
#include "cusermanagedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
protected:
    virtual void closeEvent(QCloseEvent *);
signals:
    void signal_read_threshold(st_signal_parm* p_sig_parm);
    void signal_read_col_conf(st_signal_parm* p_sig_parm);
    void signal_exit_thread();
private slots:
    void slot_read_threshold_finish(st_signal_parm* p_sig_parm);
    void slot_read_col_conf_finish(st_signal_parm* p_sig_parm);
private slots:
    void slot_time_update(QString date, QString sys_time, QString week_day);
    void slot_realtime_data_update(const st_dev_inf* p_outer_dev);

    void slot_change_child_dialog(int child_dialog_ind);
    void slot_i_range_update(st_signal_parm* p_sig_parm,
                               bool is_update);
    void slot_is_get_threshold(st_signal_parm* p_sig_parm,
                               bool is_get);
private:
    void SetDate(QString date);
    void SetSysTime(QString sys_time);
    void SetWeekDay(QString week_day);
    void LoadOuterDev();
    void SetChildDialog(CCommonChild* p_CCommonChild, int dialog_ind);
    void hideChild();
    void SetChildGeometry(QRect& rect);
    void CreatChildDialog();
    void SetChildOuterDev();
    void SetConnectTrans();

    int doGetThreShold();
    int doGetColConf();
    //test
    int test_communicate();
private:
    //dialog
    CInitDialog* pCInitDialog;
    CMainPageDialog* pCMainPageDialog;
    CEleMainWayDialog* pCEleMainWayDialog[E_SUB_WAY_NUM];
    CEleSubWayDialog* pCEleSubWayDialog;
    CHarmPercentDialog* pCHarmPercentDialog;
    CHarmIUDialog* pCHarmIUDialog[E_SUB_WAY_NUM][E_U_I_NUM];
    CSwitchStateDialog* pCSwitchStateDialog[E_SUB_WAY_NUM];
    CLoadPercentDialog* pCLoadPercentDialog[E_SUB_WAY_NUM];
    CPowerCheckDialog* pCPowerCheckDialog[E_SUB_WAY_NUM];
    CWarnCurrentDialog* pCWarnCurrentDialog;
    CWarnHistoryDialog* pCWarnHistoryDialog;
    CWarnSetDialog* pCWarnSetDialog[E_SUB_WAY_NUM];
    CWarnThresholdDialog* pCWarnThresholdDialog[E_SUB_WAY_NUM][E_SUB_WAY_THRESHOLD_NUM];
    CClearDialog* pCClearDialog;
    CUserManageDialog* pCUserManageDialog;

    //thread
    CTimeThread* timeThread;
    CRealTimeDataThread* realTimeDataThread;
    CTransfThread* pCTransfThread;


    //size
    int leftWin;
    int topWin;
    int widthWin;
    int heightWin;
    int hTop1 ;
    int hTop2 ;
    int hBottom ;
    int wLeftRight ;

    CProgressDialog* pCProgressDialog;
    st_signal_parm signalParm;
    bool isUpdateIRange;
    bool isGetThreshold;
};

#endif // MAINWINDOW_H
