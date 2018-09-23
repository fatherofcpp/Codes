#ifndef TIMETHREAD_H
#define TIMETHREAD_H
#include <QThread>
#include <QString>
class CTimeThread : public QThread
{
    Q_OBJECT
public:
    explicit CTimeThread(QObject* parent = 0);
    virtual ~CTimeThread();

    virtual void run();

signals:
    void signal_time_update(QString date, QString sys_time, QString week_day);
private slots:
    void slot_exit_thread();
private:
    bool isExit;
};

#endif // TIMETHREAD_H
