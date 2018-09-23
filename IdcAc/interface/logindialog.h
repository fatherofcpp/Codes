#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "DataTypeDef.h"

namespace Ui {
class CLoginDialog;
}

class CLoginDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CLoginDialog(QWidget *parent = 0);
    ~CLoginDialog();

signals:
    void signal_login(QString, QString);
    void signal_cancle();
private slots:
    void slot_login_clicked();
    void slot_cancle_clicked();
    void slot_user_changed(int ind);
public:
    QString GetUser();
    QString GetPass();
    st_user_inf* GetCurrentUserInf(){return pStUserInf;}
    void SetDeqUserInf(DEQ_ST_USER_INF* p_deqStUserInf);
private:
    void SetCombUser();
private:
    Ui::CLoginDialog *ui;
private:
    QString strUser;
    QString strPass;
    DEQ_ST_USER_INF deqStUserInf;
    st_user_inf* pStUserInf;//当前用户信息
};

#endif // LOGINDIALOG_H
