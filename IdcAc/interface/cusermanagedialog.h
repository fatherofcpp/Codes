#ifndef CUSERMANAGEDIALOG_H
#define CUSERMANAGEDIALOG_H

#include <QDialog>
#include "ccommonchild.h"
#include "DataTypeDef.h"
#include "cuserinfdialog.h"
#include "cusergroupdialog.h"

namespace Ui {
class CUserManageDialog;
}

class CUserManageDialog : public QDialog, public CCommonChild
{
    Q_OBJECT

public:
    explicit CUserManageDialog(QWidget *parent = 0);
    ~CUserManageDialog();

private:
    Ui::CUserManageDialog *ui;
signals:
    void signal_change_child_dialog(int ind);
private slots:
    void slot_to_main_page_clicked();
    void slot_pre_page_clicked();

    void slot_bt_edit_user_clicked();
    void slot_bt_modify_pwd_clicked();

    void slot_current_user(st_user_inf user_inf);
public:
    void SetUserInf(st_user_inf* p_st_user_inf);
    void SetDeqUserInf(DEQ_ST_USER_INF* p_deqStUserInf);
    void SetOuterDev(const str_outer_dev* outer_dev);
private:
    void InitUi();
private:
    const str_outer_dev* pOuterDev;
    st_user_inf* pStUserInf;//当前用户
    DEQ_ST_USER_INF deqStUserInf;//所有用户
    CUserInfDialog* pCUserInfDialog;
    CUserGroupDialog* pCUserGroupDialog;
};

#endif // CUSERMANAGEDIALOG_H
