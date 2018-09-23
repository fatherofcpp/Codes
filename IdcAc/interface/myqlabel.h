#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>
class CMyQLabel : public QLabel
{
    Q_OBJECT

public:
    CMyQLabel(QWidget *parent = 0);
    ~CMyQLabel();

protected:
    virtual void paintEvent(QPaintEvent *event);

public:
    void SetShowText(QString& text);
    void OpenTimer();
    void CloseTimer();

private slots:
    void slot_update_index();

private:
    QTimer *timer;
    QString showText;
    int charWidth ;
    int offset;
};

#endif // MYQLABEL_H
