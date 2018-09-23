#include "cuserinfdialog.h"
#include "ui_cuserinfdialog.h"
#include <QMessageBox>

CUserInfDialog::CUserInfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CUserInfDialog)
{
    ui->setupUi(this);
    setWindowTitle("改变用户密码");

    pStUserInf = new st_user_inf;
    memset(pStUserInf, 0, sizeof(*pStUserInf));

    SetUserInfType(E_PWD_MODIFY);

    connect(ui->pushButton_ok, SIGNAL(clicked()),
            this, SLOT(slot_login_clicked()));
    connect(ui->pushButton_cancel, SIGNAL(clicked()),
            this, SLOT(slot_cancle_clicked()));

}

CUserInfDialog::~CUserInfDialog()
{
    delete ui;
}

void CUserInfDialog::slot_login_clicked()
{
    int r = QDialog::Rejected;
    QString new_pwd = ui->lineEdit_new_pwd->text();
    QString ensure_pwd = ui->lineEdit_ensure_new_pwd->text();
    QString now_pwd = ui->lineEdit_pwd->text();
    if(userInfType == E_PWD_MODIFY)
    {
        if(now_pwd != pStUserInf->user_pwd)
        {
            QMessageBox::information(this, "提示", "密码错误");
            return;
        }

        if(new_pwd != ensure_pwd)
        {
            QMessageBox::information(this, "提示", "新密码不一致");
            return;
        }

        strcpy(pStUserInf->user_pwd, qstr_2_str(new_pwd));
        r = QDialog::Accepted;

        this->done(r);
    }
    else if(userInfType == E_USER_ADD)
    {
        QString user_name = ui->lineEdit_user_name->text();
        QString user_des = ui->lineEdit_descript->text();
        if(0 == user_name.length())
        {
            QMessageBox::information(this, "提示", "用户名无效");
            return;
        }
        if(0 == user_des.length())
        {
            QMessageBox::information(this, "提示", "用户描述无效");
            return;
        }
        if(now_pwd != ensure_pwd)
        {
            QMessageBox::information(this, "提示", "密码不一致");
            return;
        }

        strcpy(pStUserInf->user_name, qstr_2_str(user_name));
        strcpy(pStUserInf->user_descript, qstr_2_str(user_des));
        strcpy(pStUserInf->user_pwd, qstr_2_str(now_pwd));
        r = QDialog::Accepted;

        this->done(r);
    }

}

void CUserInfDialog::slot_cancle_clicked()
{
    this->close();
}

void CUserInfDialog::SetUserInf(st_user_inf *p_st_user_inf)
{
    memcpy(pStUserInf, p_st_user_inf, sizeof(*p_st_user_inf));
    ui->lineEdit_user_name->setText(pStUserInf->user_name);
    ui->lineEdit_pwd->setText("");
    ui->lineEdit_new_pwd->setText("");
    ui->lineEdit_ensure_new_pwd->setText("");
    ui->lineEdit_descript->setText(pStUserInf->user_descript);
}

void CUserInfDialog::SetUserInfType(int type)
{
    userInfType = type;
    ui->lineEdit_descript->setEnabled(false);
    if(userInfType == E_PWD_MODIFY)
    {
        ui->lineEdit_user_name->setEnabled(false);
    }
    else if(userInfType == E_USER_ADD)
    {
        ui->lineEdit_user_name->setEnabled(true);
        ui->lineEdit_descript->setText("普通用户");
        ui->lineEdit_new_pwd->setVisible(false);
        ui->label_3->setVisible(false);
    }
}
