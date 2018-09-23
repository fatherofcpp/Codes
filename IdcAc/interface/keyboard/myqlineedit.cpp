#include "myqlineedit.h"
#include <QtGui>

CMyQLineEdit::CMyQLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    keyBoard = NULL;
    lineLab = "全文";
    validType = E_VALID_ALL_TYPE;
    textShowMode = QLineEdit::Normal;
}

CMyQLineEdit::~CMyQLineEdit()
{
    if(keyBoard)
    {
        delete keyBoard;
    }
}

void CMyQLineEdit::SetLab(QString lab)
{
    lineLab = lab;
}

void CMyQLineEdit::SetValid(MU_INT32 type)
{
    validType = type;
}

void CMyQLineEdit::SetShowText(QLineEdit::EchoMode mode)
{
    textShowMode = mode;
}

void CMyQLineEdit::slot_insert_character(QString val)
{
//    this->insert(val);
}

void CMyQLineEdit::slot_hide_Key(QString text)
{
    this->setText(text);
    keyBoard->hide();

    if(keyBoard)
    {
        delete keyBoard;
        keyBoard = NULL;
    }
}

void CMyQLineEdit::slot_delete_key()
{
//    QString str = this->text();
//    str = str.left(str.length() - 1);
//    this->setText(str);
}

void CMyQLineEdit::showKey()
{
    if(!keyBoard)
    {
        keyBoard = new CKeyBoard;
        keyBoard->setWindowModality(Qt::ApplicationModal);
        connect(keyBoard, SIGNAL(signal_character_selected(QString)),
                this, SLOT(slot_insert_character(QString)));
        connect(keyBoard, SIGNAL(signal_edit_finished(QString)),
                this, SLOT(slot_hide_Key(QString)));
        connect(keyBoard, SIGNAL(signal_edit_delete()),
                this, SLOT(slot_delete_key()));
        keyBoard->SetValid(validType);
        keyBoard->SetShowText(textShowMode);
        QFont ft;
        ft.setPointSize(10);
        keyBoard->SetFont(&ft);
    }

    keyBoard->SetLine(this->text(), lineLab);
    keyBoard->show();
}

void CMyQLineEdit::focusInEvent(QFocusEvent *event)
{
    if (event->gotFocus())
    {

    }
    QLineEdit::focusInEvent(event);
}

void CMyQLineEdit::mousePressEvent(QMouseEvent *event)
{
    static int enter_cnt = 0;
   // qDebug("[%s] enter_cnt=%d", __FUNCTION__, enter_cnt++);
    if (event->button() == Qt::LeftButton)
    {
        showKey();
    }
    QLineEdit::mousePressEvent(event);
}

