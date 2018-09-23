#ifndef MYQLINEEDITOR_H
#define MYQLINEEDITOR_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

#include "keyboard.h"

class CMyQLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CMyQLineEdit(QWidget *parent = 0);
    virtual ~CMyQLineEdit();
    
signals:
    
public:
    void SetLab(QString lab);
    void SetValid(MU_INT32 type);
    void SetShowText(QLineEdit::EchoMode mode);
public slots:
    void slot_insert_character(QString val);
    void slot_hide_Key(QString text);
    void slot_delete_key();
private:
    void showKey();
protected:
    virtual void focusInEvent(QFocusEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
private:
    CKeyBoard* keyBoard;
    QString lineLab;
    MU_INT32 validType;
    QLineEdit::EchoMode textShowMode;
};

#endif // MYQLINEEDITOR_H
