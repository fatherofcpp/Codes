#ifndef CMAINPAGEDIALOG_H
#define CMAINPAGEDIALOG_H

#include <QDialog>

#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>

#include "ccommonchild.h"
#include "DataTypeDef.h"

typedef enum
{
    E_COM_STATE_MONITOR_HOST = 0,
    E_COM_STATE_SWITCH_UNIT,
    E_COM_STATE_POWER_MODULE,
    E_COM_STATE_NUM
}e_com_state_bt;

typedef enum
{
    E_PAGE_CHANGE_MAIN_WAY_PARAM = 0,
    E_PAGE_CHANGE_HARM_PARAM,
    E_PAGE_CHANGE_SUB_WAY_PARAM,
    E_PAGE_CHANGE_POWER_CHECK,
    E_PAGE_CHANGE_SWITCH_STATE,
    E_PAGE_CHANGE_LOAD_PERCENT,
    E_PAGE_CHANGE_WARN_INF,
    E_PAGE_CHANGE_PARAM_SET,
    E_PAGE_CHANGE_CLEAR,
    E_PAGE_CHANGE_NUM
}e_page_change_bt;

typedef enum
{
    E_METER_WIGDET_U_A = 0,
    E_METER_WIGDET_U_B,
    E_METER_WIGDET_U_C,
    E_METER_WIGDET_I_A,
    E_METER_WIGDET_I_B,
    E_METER_WIGDET_I_C,
    E_METER_WIGDET_NUM
}e_meter_wigdet;

typedef struct
{
    QLineF linef;
    int ui_ind;
    double meter_val;
    int meter_range;
}st_line_meter_inf;

namespace Ui {
class CMainPageDialog;
}

class CMainPageDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CMainPageDialog(QWidget *parent = 0);
    ~CMainPageDialog();
    
private:
    Ui::CMainPageDialog *ui;
protected:
    virtual void paintEvent(QPaintEvent *);
signals:
    void signal_change_child_dialog(int ind);
    void signal_current_user(st_user_inf user_inf);
private slots:
    void slot_bt_main_way_param_clicked();
    void slot_bt_harm_param_clicked();
    void slot_bt_sub_way_param_clicked();
    void slot_bt_power_check_clicked();
    void slot_bt_switch_state_clicked();
    void slot_bt_load_percent_clicked();
    void slot_bt_warn_inf_clicked();
    void slot_bt_param_set_clicked();
    void slot_bt_clear_clicked();

    void slot_communicate_state_update(const st_dev_inf* p_outer_dev);
public:
    void SetOuterDev(const str_outer_dev* outer_dev);
    void SetUi(const st_dev_inf* p_dev_inf);
private:
    void InitUi();
    void SetLinef(int ind, double val);
private:
    const str_outer_dev* pOuterDev;
    st_line_meter_inf lineFMeter[E_METER_WIGDET_NUM];
};

#endif // CMAINPAGEDIALOG_H
