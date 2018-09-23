#ifndef CPWDMODIFYDIALOG_H
#define CPWDMODIFYDIALOG_H

#include <QDialog>
#include "DataTypeDef.h"
namespace Ui {
class CUserInfDialog;
}

typedef enum
{
    E_PWD_MODIFY = 0,
    E_USER_ADD,
    E_USER_INF_TYPE_NUM
}e_user_inf_type;

class CUserInfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CUserInfDialog(QWidget *parent = 0);
    ~CUserInfDialog();

private:
    Ui::CUserInfDialog *ui;

private slots:
    void slot_login_clicked();
    void slot_cancle_clicked();
public:
    void SetUserInf(st_user_inf* p_st_user_inf);
    st_user_inf* GetUserInf(){return pStUserInf;}
    void SetUserInfType(int type);
private:
    st_user_inf* pStUserInf;//当前用户信息

    int userInfType;

};

#endif // CPWDMODIFYDIALOG_H
