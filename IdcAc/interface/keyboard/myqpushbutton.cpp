#include "myqpushbutton.h"

CMyQPushButton::CMyQPushButton(QWidget *parent)
    : QPushButton(parent)
{
    btInf.colum_ind = 0;
    btInf.list_ind = 0;
    btInf.row_ind = 0;
    btInf.code = 0;
    btInf.is_on = true;
    btInf.text = "";

    pauseMsecs = 400;
    intervalMsecs = 30;
    tm = new QTimer;
    connect(tm, SIGNAL(timeout()),
            this, SLOT(slot_pressed_last()));
    connect(this, SIGNAL(pressed()),
            this, SLOT(slot_pressed()));
    connect(this, SIGNAL(released()),
            this, SLOT(slot_released()));
    connect(this, SIGNAL(clicked()),
            this, SLOT(slot_clicked()));
}

CMyQPushButton::~CMyQPushButton()
{
    delete tm;
}

void CMyQPushButton::slot_pressed()
{
    emit this->signal_key_pressed(this->text());
    tm->start(pauseMsecs);
}

void CMyQPushButton::slot_pressed_last()
{
    emit this->signal_key_pressed(this->text());
    tm->setInterval(intervalMsecs);
}

void CMyQPushButton::slot_released()
{
    tm->stop();
    emit this->signal_key_released(this->text());
}

void CMyQPushButton::slot_clicked()
{
    QString text = this->text();
    btInf.text = text;
    emit this->signal_key_clicked(btInf);
}

void CMyQPushButton::SetPosUi(st_bt_inf *bt_inf)
{
    btInf.colum_ind = bt_inf->colum_ind;
    btInf.list_ind = bt_inf->list_ind;
    btInf.row_ind = bt_inf->row_ind;
    btInf.text = bt_inf->text;
    btInf.code = bt_inf->code;
    btInf.is_on = bt_inf->is_on;
}

void CMyQPushButton::SetOnOff(bool is_on)
{
    btInf.is_on = is_on;
}

void CMyQPushButton::SetCode(int code)
{
    btInf.code = code;
}
