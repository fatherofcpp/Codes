/*
此线程为后台线程，异步运行
*/
#ifndef REALTIMEDATATHREAD_H
#define REALTIMEDATATHREAD_H

#include <QThread>
#include "DataTypeClass.h"

class CRealTimeDataThread : public QThread
{
    Q_OBJECT
public:
    explicit CRealTimeDataThread(QObject *parent = 0);
    virtual ~CRealTimeDataThread();

    virtual void run();
signals:
    //同步信号
    void signal_realtime_data_update(const st_dev_inf* p_outer_dev);
    void signal_communicate_state_update(const st_dev_inf* p_outer_dev);
private slots:
    //同步槽
private slots:
    void slot_exit_thread();
private:
    bool isExit;
};

#endif // REALTIMEDATATHREAD_H
