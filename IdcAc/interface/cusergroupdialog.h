#ifndef CUSERGROUPDIALOG_H
#define CUSERGROUPDIALOG_H

#include <QDialog>
#include "ccommonchild.h"
#include "DataTypeDef.h"
#include "cuserinfdialog.h"

namespace Ui {
class CUserGroupDialog;
}

class CUserGroupDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CUserGroupDialog(QWidget *parent = 0);
    ~CUserGroupDialog();
    
private:
    Ui::CUserGroupDialog *ui;

signals:

private slots:
    void slot_bt_add_clicked();
    void slot_bt_delete_clicked();
    void slot_bt_ok_clicked();
    void slot_bt_cancel_clicked();

    void slot_pre_list_clicked();
    void slot_next_list_clicked();
public:
    void SetDeqUserInf(DEQ_ST_USER_INF* p_deqStUserInf);
    DEQ_ST_USER_INF* GetDeqUserInf(){return &deqStUserInf;}
private:
    void InitUi();
    void SetList();
private:
    DEQ_ST_USER_INF deqStUserInf;//所有用户
    CUserInfDialog* pCUserInfDialog;
};

#endif // CUSERGROUPDIALOG_H
