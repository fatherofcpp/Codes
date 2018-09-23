#include "timethread.h"
#include "dataConvert.h"

CTimeThread::CTimeThread(QObject *parent)
    : QThread(parent)
{
    isExit = false;
}

CTimeThread::~CTimeThread()
{
}

void CTimeThread::run()
{
    int sleep_s = 1;

    st_time time_now;
    QString str_time_date;
    QString str_time_run;

    time_t time_start;
    time_t time_cur;
    time(&time_start);
    while(1)
    {
        if(isExit)
        {
            return;
        }

        GetNowTime(time_now);
        str_time_date.sprintf("%04d-%02d-%02d %02d:%02d:%02d",
                              time_now.YY, time_now.MM, time_now.DD,
                              time_now.HH, time_now.MI, time_now.SS);

        time(&time_cur);
        int time_run = time_cur - time_start;
        double time_run_hour = ((double)time_run / 60 / 60) ;
        str_time_run = "系统已运行" + QString::number(time_run_hour, 'f', 2) + "h";


        QString week_day = "";
        int y = time_now.YY;
        int m = time_now.MM;
        int d = time_now.DD;
        if(m == 1 || m == 2)
        {
            m += 12;
            y --;
        }
        int i_day = (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;
        switch(i_day)
        {
        case 0:
            week_day = "星期一";
            break;
        case 1:
            week_day = "星期二";
            break;
        case 2:
            week_day = "星期三";
            break;
        case 3:
            week_day = "星期四";
            break;
        case 4:
            week_day = "星期五";
            break;
        case 5:
            week_day = "星期六";
            break;
        case 6:
            week_day = "星期日";
            break;
        }

        emit signal_time_update(str_time_date, str_time_run, week_day);

        sleep(sleep_s);

    }

}

void CTimeThread::slot_exit_thread()
{
    isExit = true;
}
