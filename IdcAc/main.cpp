#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QTextCodec>
#include <QWSServer>
#include <QMessageBox>
#include "CommonFunction.h"
#include "cpipe.h"

#include <fcntl.h>

class filterObject:public QObject
{

#define BACKLIGHT_OFF    0   //关
#define BACKLIGHT_WEAK   40  //四分之一
#define BACKLIGHT_HALF   60  //二分之一
#define BACKLIGHT_BRIGHT 80  //四分之三
#define BACKLIGHT_FULL   100 //最大

#define BACKLIGHT_KEEP_S 60*5

private:
    bool is_pressed;
    bool is_first_on;
    int bk_fd;
    char buf[4];
    int timer_id;
    unsigned int light;

private:
    void timerEvent(QTimerEvent *event)
    {
        Q_UNUSED(event);

        if(0 != timer_id)
        {
            killTimer(timer_id);
            timer_id = 0;
        }
        light = BACKLIGHT_OFF;
        sprintf(buf, "%u", light);
        ::write(bk_fd, buf, sizeof(buf));
        is_first_on = true;
    }

    bool eventFilter(QObject *obj, QEvent *event)
    {
        if(event->type() == QEvent::MouseButtonPress && !is_pressed)
        {
            is_pressed = true;
            light = BACKLIGHT_FULL;
            sprintf(buf, "%u", light);
            ::write(bk_fd, buf, sizeof(buf));
        }
        else if(event->type() == QEvent::MouseButtonRelease && is_pressed)
        {
            is_pressed = false;
            if(0 != timer_id)
            {
                killTimer(timer_id);
                timer_id = 0;
            }
            timer_id = startTimer(BACKLIGHT_KEEP_S * 1000);
            if (is_first_on)
            {
                is_first_on = false;
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

public:
    explicit filterObject(QObject *parent = 0)
    {
        Q_UNUSED(parent);

        is_pressed = false;
        is_first_on = false;
        bk_fd = ::open("/sys/class/backlight/pwm-backlight/brightness", O_WRONLY);
        memset(buf, 0, sizeof(buf));
        timer_id = startTimer(BACKLIGHT_KEEP_S * 1000);
    }

    ~filterObject()
    {
        ::close(bk_fd);
    }
};

int create_child_progress()
{
    pid_t pid_f;
    char pipe_buf[256] = {'a','0'};
    memset(pipe_buf, 0, sizeof(pipe_buf));

    pid_f = fork();
    if (pid_f < 0)
    {
        printf("fork error!\n");
        exit(1);
    }
    else if (pid_f == 0)
    {
        //子进程
        //printf("Child:pid=%d\n", getpid());
        int new_len = 1;//1024 * 1024 * 1024 * (1 /10);
        char* p_new = new char[new_len];
        memset(p_new, 1, new_len);
        while(1)
        {
            //printf("Child:Reading\n");
            memset(pipe_buf, 0, sizeof(pipe_buf));
            int read_ret = gCpipe.ReadPipe(pipe_buf, sizeof(pipe_buf));//没阻塞的读
            int sys_ret = system(pipe_buf);
//            printf("pipe_buf %s,read_ret=%d,sys_ret=%d\n",
//                     pipe_buf,read_ret,sys_ret);
        }
    }
    else
    {
        //父进程
        //printf("Father:pid=%d\n", getpid());
    }
    return 0;
}

int main(int argc, char *argv[])
{
    create_child_progress();

    QApplication a(argc, argv);

    filterObject *filterObj= new filterObject;
    a.installEventFilter(filterObj);

    //GB18030
    //UTF-8
    char code_type[32] = "UTF-8";
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(code_type));
    QTextCodec::setCodecForTr(QTextCodec::codecForName(code_type));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(code_type));

    //chinese for am335x
    QFont font = a.font();
    font.setFamily("unifont");
    a.setFont(font);

    //running path
    char resolved_path[256];
    realpath(argv[0], resolved_path);
    std::string path = resolved_path;
    int pos = path.find_last_of('/');
    std::string path_db(path.substr(0, pos));
    gCurrentDir = path_db;
    path_db = path_db + "/zhpy_table.db";
    //qDebug("[%s]db_path=%s\n", __FUNCTION__, path_db.c_str());
    //QMessageBox::information(NULL, "提示", path_db.c_str());

#if __arm__
    //arm
    QWSServer::setCursorVisible(false);
#endif

    MainWindow w;
    w.show();
    return a.exec();
}
