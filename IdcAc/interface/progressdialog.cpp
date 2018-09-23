#include "include_all_qt.h"
#include "progressdialog.h"
#include "CommonFunction.h"
#include "ui_progressdialog.h"

CProgressDialog::CProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CProgressDialog)
{
    ui->setupUi(this);

    minRangle = 0;
    maxRangle = 100;
    curStep = 0;
    progressName = "";
    AutoProgress = true;
    progressRun = false;

    setWindowTitle("进度条");
    ui->progressBar->setRange(minRangle, maxRangle);
    ui->label->setText(progressName);

    tm = new QTimer;
    connect(tm, SIGNAL(timeout()),
            this, SLOT(slot_time_point()));

    enterTime = 0;
}

CProgressDialog::~CProgressDialog()
{
    delete ui;
    delete tm;
}

void CProgressDialog::closeEvent(QCloseEvent* event)
{
    if(!permitClose)
    {
        event->ignore();
    }
}

void CProgressDialog::ControlTimer(bool is_start)
{
    if(is_start)
    {
        tm->start(200);
    }
    else
    {
        tm->stop();
        Sleep(250);//确保定时间关闭
        progressRun = false;
    }
    SetPermitClose(!is_start);
}

void CProgressDialog::slot_time_point()
{
    QString str;
    enterTime ++;
    int cur_step = 0;
    //    qDebug("[%s]enter_time=%d", __FUNCTION__, enterTime);
    if(AutoProgress)
    {
        cur_step = enterTime % maxRangle;
        str = progressName + "中...";
    }
    else
    {
        cur_step = curStep;
        str = progressName + "中...已完成" + QString::number(cur_step, 'f', 2) + "%%";
    }

    ui->progressBar->setValue(cur_step);
    ui->label->setText(str);

    progressRun = true;
}
