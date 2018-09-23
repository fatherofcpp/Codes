#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
CLoginDialog::CLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoginDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);

    connect(ui->pushButton_login, SIGNAL(clicked()),
            this, SLOT(slot_login_clicked()));
    connect(ui->pushButton_cancle, SIGNAL(clicked()),
            this, SLOT(slot_cancle_clicked()));
    connect(ui->comboBox_user, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slot_user_changed(int)));

    setWindowTitle("用户登录");
    strUser = "";
    strPass = "000001";
    ui->lineEdit_pass->SetShowText(QLineEdit::PasswordEchoOnEdit);
    ui->lineEdit_pass->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->lineEdit_pass->setText(strPass);

    deqStUserInf.clear();

    pStUserInf = new st_user_inf;
    memset(pStUserInf, 0, sizeof(*pStUserInf));

}

CLoginDialog::~CLoginDialog()
{
    delete ui;
    delete pStUserInf;
}

void CLoginDialog::slot_login_clicked()
{
    strPass = ui->lineEdit_pass->text();
    strUser = ui->comboBox_user->currentText();

    int r = QDialog::Rejected;
    MU_INT32 cont_num = deqStUserInf.size();
    for(MU_INT32 i = 0; i < cont_num; i++)
    {
        st_user_inf user_inf = deqStUserInf[i];
        if(strUser == user_inf.user_name
                && strPass == user_inf.user_pwd)
        {
            memcpy(pStUserInf, &user_inf, sizeof(*pStUserInf));
            r = QDialog::Accepted;
            break;
        }
    }

    if(r == QDialog::Rejected)
    {
        QMessageBox::information(this, "提示", "用户名或密码错误");
        return;
    }

    this->done(r);
}

void CLoginDialog::slot_cancle_clicked()
{
    this->close();
}

void CLoginDialog::slot_user_changed(int ind)
{
    int cur_ind = ind;
    QString text;
    st_user_inf user_inf = deqStUserInf[cur_ind];
    text = user_inf.user_descript;
    ui->label_descript->setText(text);

}

QString CLoginDialog::GetUser()
{
    return strUser;
}

QString CLoginDialog::GetPass()
{
    return strPass;
}

void CLoginDialog::SetDeqUserInf(DEQ_ST_USER_INF* p_deqStUserInf)
{
    deqStUserInf = *p_deqStUserInf;
    SetCombUser();
}

void CLoginDialog::SetCombUser()
{
//    int ind = ui->comboBox_user->currentIndex();
    ui->comboBox_user->clear();
    MU_INT32  cont_num = deqStUserInf.size();
    for(MU_INT32 i = 0; i < cont_num; i++)
    {
        st_user_inf st_user_inf = deqStUserInf[i];
        ui->comboBox_user->addItem(st_user_inf.user_name, i);
    }
}
