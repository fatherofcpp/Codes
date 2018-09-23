#ifndef CPROGRESSDIALOG_H
#define CPROGRESSDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QCloseEvent>
using namespace std;
namespace Ui {
class CProgressDialog;
}

class CProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CProgressDialog(QWidget *parent = 0);
    ~CProgressDialog();

protected:
    virtual void closeEvent(QCloseEvent *);
private:
    Ui::CProgressDialog *ui;

public:
    void SetMinRangle(int val){minRangle = val;}
    void SetMaxRangle(int val){maxRangle = val;}
    void SetCurStep(int val){curStep = val;}
    void SetProgressName(QString progress_name){progressName = progress_name;}
    void SetAutoProgress(bool is_auto){AutoProgress = is_auto;}
    bool GetRunFlag(){return progressRun;}
    void ControlTimer(bool is_start);//打开与关闭之前调用
    void SetEnterTime(int val){enterTime = val;}
    void SetPermitClose(bool is_permit){permitClose = is_permit;}
private slots:
    void slot_time_point();

private:
    int minRangle;//最小进度值
    int maxRangle;//最大进度值
    int curStep;//当前进度值
    bool AutoProgress;//无法估算进度，自动进度
    QString progressName;//进度条名称
    bool progressRun;//线程中关闭progressRun为true的进度对话框
    bool permitClose;//是否允许关闭

    QTimer *tm;
    int enterTime;
};

#endif // CPROGRESSDIALOG_H
