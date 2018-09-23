#include "common_qt.h"

#include <QTextCodec>
#include <QWSServer>

QString utf_2_gbk(const char* str)
{
    return QString::fromLocal8Bit(str);
    return QString::fromUtf8(str);
}

const char* qstr_2_str(const QString& qstr)
{
    std::string str = qstr.toStdString();
    const char* p = str.c_str();
    return p;
}

QString str_2_qstr(const string &s)
{
    return QString(QString::fromLocal8Bit(s.c_str()));
}

QString pos_2_val(int pos)
{
    QString qstr;
    if(pos >= 0 && pos <= KEYNUM - 1)
    {
        qstr = gKeyboardVal[pos];
    }
    else
    {
        qstr = "";
    }
    return qstr;
}

int val_2_pos(QString val)
{
    int pos = -1;
    for(int i = 0; i < KEYNUM; i++)
    {
        if(gKeyboardVal[i] == val)
        {
            pos = i;
            break;
        }
    }
    return pos;
}
