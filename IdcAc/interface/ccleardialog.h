#ifndef CCLEARDIALOG_H
#define CCLEARDIALOG_H

#include <QDialog>
#include "ccommonchild.h"
#include "DataTypeDef.h"
namespace Ui {
class CClearDialog;
}

class CClearDialog : public QDialog, public CCommonChild
{
    Q_OBJECT
    
public:
    explicit CClearDialog(QWidget *parent = 0);
    ~CClearDialog();
    
private:
    Ui::CClearDialog *ui;

signals:
    void signal_write_zero_energy(st_signal_parm* p_sig_parm,
                                  const st_thread_param_zero_energy_set* param_zero_energy_set);
    void signal_write_col_conf(st_signal_parm* p_sig_parm);
    void signal_write_col_time(st_signal_parm* p_sig_parm);
    void signal_upgrade(st_signal_parm* p_sig_parm);
private slots:
    void slot_write_zero_energy_finish(st_signal_parm* p_sig_parm);
    void slot_write_col_conf_finish(st_signal_parm* p_sig_parm);
    void slot_write_col_time_finish(st_signal_parm* p_sig_parm);
    void slot_upgrade_finish(st_signal_parm* p_sig_parm);
signals:
    void signal_change_child_dialog(int ind);
private slots:
    void slot_to_main_page_clicked();

    void slot_id_clicked();
    void slot_time_clicked();
    void slot_clear_a_all_clicked();
    void slot_clear_a_m_clicked();
    void slot_clear_b_m_clicked();
    void slot_clear_a_sub_clicked();
    void slot_clear_b_sub_clicked();
    void slot_clear_a_frozen_energy_clicked();
    void slot_upgrade_clicked();
    void slot_modify_pwd_clicked();
    void slot_reboot_clicked();
    void slot_bt_version_clicked();
public:
    void SetOuterDev(const str_outer_dev* outer_dev);
    void SetColConf(st_coltor_inf* coltor_inf);
private:
    void InitUi();

private:
    const str_outer_dev* pOuterDev;
    st_coltor_inf* pColtorInf;

    CProgressDialog* pCProgressDialog;
    st_signal_parm signalParm;
};

#endif // CCLEARDIALOG_H
