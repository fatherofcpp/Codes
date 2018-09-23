#include "myqlabel.h"
#include <QPainter>
#include <QTimer>

CMyQLabel::CMyQLabel(QWidget *parent)
    : QLabel(parent)
{
    offset = 0;
    charWidth = fontMetrics().width("a");//每个字符的宽度

    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
//    pa.setColor(QPalette::Background, Qt::red);
    setPalette(pa);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(slot_update_index()));
    OpenTimer();
}

CMyQLabel::~CMyQLabel()
{
}

void CMyQLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    int w = width();
    int text_width = fontMetrics().width(showText);
    if (text_width < 1)
    {
        return;
    }

    int x = - offset * charWidth;
    while (x < w)
    {
        painter.drawText(x, 0, text_width, height(),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         showText);
        x += text_width;

        if(x > text_width)
        {
            break;
        }
    }
}

void CMyQLabel::SetShowText(QString &text)
{
    showText = text;
    update();
}

void CMyQLabel::OpenTimer()
{
    timer->start(500);
}

void CMyQLabel::CloseTimer()
{
    timer->stop();
}

void CMyQLabel::slot_update_index()
{
    update();

    offset ++;
    if (offset * charWidth > width())
    {
        offset = 0;
    }
}
