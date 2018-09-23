#pragma once
#include <QPushButton>
#include <QTimer>
#include "DataTypeStruct.h"

typedef struct
{
    int row_ind;//行号
    int colum_ind;//列号
    int list_ind;//列表号
    int code;//编号
    bool is_on;//开/关
    QString text;//文本
}st_bt_inf;

class CMyQPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CMyQPushButton(QWidget *parent = 0);
    virtual ~CMyQPushButton();

private:
    QTimer *tm;
    long pauseMsecs;
    long intervalMsecs;

signals:
    void signal_key_pressed(const QString &msg);
    void signal_key_released(const QString &msg);
    void signal_key_clicked(const st_bt_inf& pos);
public slots:
    void slot_pressed();
    void slot_pressed_last();
    void slot_released();
    void slot_clicked();
public:
    void SetPosUi(st_bt_inf* bt_inf);
    void SetOnOff(bool is_on);
    void SetCode(int code);
    bool GetOnOff(){return btInf.is_on;}
private:
    st_bt_inf btInf;
};

