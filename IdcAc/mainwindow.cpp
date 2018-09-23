#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "common_qt.h"
#include "Log.h"
#include "DataTypeDef.h"
#include "dataConvert.h"
#include "CommonFunction.h"
#include "xmlAnalyze.h"


#include <QMessageBox>
#include <QCloseEvent>

static QString static_str_backgroud =
        "\
        QListWidget {\
        border-width: 1px;\
        border-style: solid;\
        border-radius:1px;\
        border-color: lightGray;\
        background-color: transparent ;\
        }\
        \
        #centralWidget{\
            border-image: url(:/images/backgroud.png);\
            }\
            ";

        MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#if __arm__
    //arm
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif

    isUpdateIRange = true;
    isGetThreshold = true;
    leftWin = 0;
    topWin = 0;
    widthWin = 800;
    heightWin = 480;
    hTop1 = 20;
    hTop2 = 40;
    hBottom = 20;
    wLeftRight = 6;
    memset(&signalParm, 0, sizeof(signalParm));

    pCProgressDialog = new CProgressDialog(this);

    //global
    gLeftChild = 0;
    gTopChild = 0;
    gWidthChild = widthWin - 2*wLeftRight;
    gHeightChild = heightWin - hTop1 - hTop2 - hBottom;
    //init log
    string log_path = gCurrentDir + "/save.log";
    gLog.InitLog((char*)log_path.c_str());
    gLog.Add("%s init", __FUNCTION__);
    //golbal socket
    g_pTransf = new CSocketUnix;
    CSocketUnix* p_CSocketLocal = dynamic_cast<CSocketUnix*>(g_pTransf);
    p_CSocketLocal->SetServerIp("/var/tmp/mmi_socket");
    struct timeval time_out;
    time_out.tv_sec = RS_DTU_TIMEOUT;
    time_out.tv_usec = 500 * 1000;
    p_CSocketLocal->SetReadTimeOut(time_out);

    //ui
    this->setStyleSheet(static_str_backgroud);

    ui->widget_top->setGeometry(leftWin, topWin, widthWin, hTop1);
    int w = (widthWin - 2*wLeftRight);
    int w1 = w* 10/100;
    int w2 = w* 65/100;
    int w3 = w* 25/100;
    ui->widget_nhb->setGeometry(leftWin + wLeftRight, topWin + hTop1, w1, hTop2);
    ui->widget_title->setGeometry(leftWin + wLeftRight + w1,
                                  topWin + hTop1, w2, hTop2);
    ui->widget_time->setGeometry(leftWin + wLeftRight + w1 + w2, topWin + hTop1,
                                 w3, hTop2);
    ui->widget_bottom->setGeometry(leftWin+ wLeftRight, topWin + heightWin - hBottom,
                                   widthWin - 2*wLeftRight, hBottom);
    ui->widget_left->setGeometry(leftWin, topWin + hTop1, wLeftRight, heightWin - hTop1);
    ui->widget_right->setGeometry(leftWin + widthWin - wLeftRight, topWin + hTop1,
                                  wLeftRight, heightWin - hTop1);

    ui->widget_left->setStyleSheet(gResLeftRight);
    ui->widget_right->setStyleSheet(gResLeftRight);
    ui->widget_top->setStyleSheet(gResTop);
    ui->widget_bottom->setStyleSheet(gResBottom);
    ui->widget_nhb->setStyleSheet(gResNhb);
    ui->widget_title->setStyleSheet(gResTitleInit);

    QPalette pal;
    QFont ft;
    ft.setPointSize(18);
    pal.setColor(QPalette::WindowText,Qt::white);
    ui->label_date->setPalette(pal);
    ui->label_system_time->setPalette(pal);
    ui->label_week->setPalette(pal);

    pal.setColor(QPalette::WindowText,Qt::green);
    ft.setPointSize(12);
    ui->label_title_main->setText(QString::fromLocal8Bit("交流精密配电监控系统"));//
    ui->label_title_main->setPalette(pal);
    ui->label_title_main->setFont(ft);
    ft.setPointSize(11);
    ui->label_title_sub->setPalette(pal);
    ui->label_title_sub->setFont(ft);
    ui->label_title_main->hide();
    ui->label_title_sub->hide();

    ft.setPointSize(10);
    ui->label_date->setFont(ft);
    ui->label_system_time->setFont(ft);
    ui->label_week->setFont(ft);

    ft.setPointSize(10);
    ui->lineEdit_date->setFont(ft);

    setWindowTitle("NHB");
    ui->label_date->setFocus();
    CreatChildDialog();
    //time thread
    timeThread = new CTimeThread(this);
    timeThread->start();
    connect(timeThread, SIGNAL(signal_time_update(QString,QString,QString)),
            this, SLOT(slot_time_update(QString,QString,QString)));

    //transf thread
    pCTransfThread = new CTransfThread;
    QThread* thread = new QThread(this);
    pCTransfThread->moveToThread(thread);
    thread->start();
    SetConnectTrans();

    //读外设信息
    LoadOuterDev();
    //读阀值，用于告警判断
    doGetThreShold();

    SetChildOuterDev();

    //realtime thread
    realTimeDataThread = new CRealTimeDataThread(this);
    realTimeDataThread->start();
    connect(realTimeDataThread, SIGNAL(signal_realtime_data_update(const st_dev_inf*)),
            this, SLOT(slot_realtime_data_update(const st_dev_inf*)));
    connect(realTimeDataThread, SIGNAL(signal_communicate_state_update(const st_dev_inf*)),
            pCMainPageDialog, SLOT(slot_communicate_state_update(const st_dev_inf*)));

    connect(this, SIGNAL(signal_exit_thread()),
            timeThread, SLOT(slot_exit_thread()));
    connect(this, SIGNAL(signal_exit_thread()),
            realTimeDataThread, SLOT(slot_exit_thread()));

    //初始化
    slot_change_child_dialog(E_INIT_DIALOG);

    //init ui
    ui->lineEdit_date->setFocusPolicy(Qt::NoFocus);

    //test
    test_communicate();

}

MainWindow::~MainWindow()
{
    //exit child thread
    emit signal_exit_thread();
    while(timeThread->isRunning() || realTimeDataThread->isRunning())
    {
        gLog.Add("%s running", __FUNCTION__);
        Sleep(50);
    }

    delete ui;
    delete pCTransfThread;
    delete g_pTransf;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //    event->ignore();
}

void MainWindow::SetDate(QString date)
{
    ui->lineEdit_date->setText(date);
}

void MainWindow::SetSysTime(QString sys_time)
{
    ui->label_system_time->setText(sys_time);
}

void MainWindow::SetWeekDay(QString week_day)
{
    ui->label_week->setText(week_day);
}

void MainWindow::LoadOuterDev()
{
    str_outer_dev* pouter_dev = gXmlAnalyze.GetOuterDev();
    pouter_dev->port_num = 1;

    int port_ind = 0;
    int dev_ind = -1;
    const MU_INT64 way_mask31 = 0x7FFFFFFF;//31个1
    const MU_INT64 way_mask62 = 0x3FFFFFFFFFFFFFFF;//62个1
    const int num_way_one = MAX_WAY_NUM/2;
    pouter_dev->port_dev[port_ind].dev_num = 3;

    {
        //主路电表
        dev_ind ++;
        st_dev_inf* p_st_dev_inf = &pouter_dev->port_dev[port_ind].dev_inf[dev_ind];
        p_st_dev_inf->dev_addr.id = (MU_INT8)(DEVID_ELECTRIC_M);
        p_st_dev_inf->dev_type = E_DEV_ELECTRIC;

        p_st_dev_inf->way_inf.way_mask = way_mask62;//62个1
        for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)//
        {
            st_way_data* p_st_way_data = &p_st_dev_inf->way_inf.way_data[way_ind];
            p_st_way_data->way_id = MAIN_A_CODE + way_ind;

            if(way_ind < num_way_one)
            {
                p_st_way_data->way_id = MAIN_A_CODE + way_ind;
            }
            else
            {
                p_st_way_data->way_id = MAIN_B_CODE + way_ind - num_way_one;
            }
        }
    }

    {
        //主路A开关
        dev_ind ++;
        st_dev_inf* p_st_dev_inf = &pouter_dev->port_dev[port_ind].dev_inf[dev_ind];
        p_st_dev_inf->dev_addr.id = (MU_INT8)(DEVID_SWITCH_M_A);
        p_st_dev_inf->dev_type = E_DEV_SWITCH;

        p_st_dev_inf->way_inf.way_mask = way_mask31;//31个1
        for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)//
        {
            st_way_data* p_st_way_data = &p_st_dev_inf->way_inf.way_data[way_ind];
            p_st_way_data->way_id = MAIN_A_CODE + way_ind;
        }
    }

    {
        //主路B开关
        dev_ind ++;
        st_dev_inf* p_st_dev_inf = &pouter_dev->port_dev[port_ind].dev_inf[dev_ind];
        p_st_dev_inf->dev_addr.id = (MU_INT8)(DEVID_SWITCH_M_B);
        p_st_dev_inf->dev_type = E_DEV_SWITCH;

        p_st_dev_inf->way_inf.way_mask = way_mask31;//31个1
        for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)//
        {
            st_way_data* p_st_way_data = &p_st_dev_inf->way_inf.way_data[way_ind];
            p_st_way_data->way_id = MAIN_B_CODE + way_ind;
        }
    }

    string user_group_file = gCurrentDir + "/config.xml";
    gXmlAnalyze.LoadUserGroup(user_group_file.c_str());
}

void MainWindow::SetChildDialog(CCommonChild* p_CCommonChild, int dialog_ind)
{
    p_CCommonChild->SetDialogInd(dialog_ind);
}

void MainWindow::hideChild()
{
    pCInitDialog->hide();
    pCMainPageDialog->hide();
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCEleMainWayDialog[i]->hide();
    }
    pCEleSubWayDialog->hide();
    pCHarmPercentDialog->hide();
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_U_I_NUM; j++)
        {
            pCHarmIUDialog[i][j]->hide();
        }
    }
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCSwitchStateDialog[i]->hide();
        pCLoadPercentDialog[i]->hide();
        pCPowerCheckDialog[i]->hide();
    }
    pCWarnCurrentDialog->hide();
    pCWarnHistoryDialog->hide();
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCWarnSetDialog[i]->hide();
    }
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_SUB_WAY_THRESHOLD_NUM; j++)
        {
            pCWarnThresholdDialog[i][j]->hide();
        }
    }
    pCClearDialog->hide();
    pCUserManageDialog->hide();
}

void MainWindow::SetChildGeometry(QRect &rect)
{
    pCInitDialog->setGeometry(rect);
    pCMainPageDialog->setGeometry(rect);
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCEleMainWayDialog[i]->setGeometry(rect);
    }
    pCEleSubWayDialog->setGeometry(rect);
    pCHarmPercentDialog->setGeometry(rect);
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_U_I_NUM; j++)
        {
            pCHarmIUDialog[i][j]->setGeometry(rect);
        }
    }
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCSwitchStateDialog[i]->setGeometry(rect);
        pCLoadPercentDialog[i]->setGeometry(rect);
        pCPowerCheckDialog[i]->setGeometry(rect);
    }
    pCWarnCurrentDialog->setGeometry(rect);
    pCWarnHistoryDialog->setGeometry(rect);
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCWarnSetDialog[i]->setGeometry(rect);
    }
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_SUB_WAY_THRESHOLD_NUM; j++)
        {
            pCWarnThresholdDialog[i][j]->setGeometry(rect);
        }
    }

    pCClearDialog->setGeometry(rect);
    pCUserManageDialog->setGeometry(rect);
}

void MainWindow::CreatChildDialog()
{
    int dialog_off = 0;
    pCInitDialog = new CInitDialog(this);
    SetChildDialog(pCInitDialog, E_INIT_DIALOG);
    connect(pCInitDialog, SIGNAL(signal_change_child_dialog(int)),
            this, SLOT(slot_change_child_dialog(int)));

    pCMainPageDialog = new CMainPageDialog(this);
    SetChildDialog(pCMainPageDialog, E_MAIN_PAGE_DIALOG);
    connect(pCMainPageDialog, SIGNAL(signal_change_child_dialog(int)),
            this, SLOT(slot_change_child_dialog(int)));

    dialog_off = 0;
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCEleMainWayDialog[i] = new CEleMainWayDialog(this);
        SetChildDialog(pCEleMainWayDialog[i], E_ELE_MAIN_WAY_DIALOG_A + dialog_off);
        connect(pCEleMainWayDialog[i], SIGNAL(signal_change_child_dialog(int)),
                this, SLOT(slot_change_child_dialog(int)));
        dialog_off ++;
    }

    pCEleSubWayDialog = new CEleSubWayDialog(this);
    SetChildDialog(pCEleSubWayDialog, E_ELE_SUB_WAY_DIALOG);
    connect(pCEleSubWayDialog, SIGNAL(signal_change_child_dialog(int)),
            this, SLOT(slot_change_child_dialog(int)));

    pCHarmPercentDialog = new CHarmPercentDialog(this);
    SetChildDialog(pCHarmPercentDialog, E_HARM_PERCENT_DIALOG);
    connect(pCHarmPercentDialog, SIGNAL(signal_change_child_dialog(int)),
            this, SLOT(slot_change_child_dialog(int)));

    dialog_off = 0;
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_U_I_NUM; j++)
        {
            pCHarmIUDialog[i][j] = new CHarmIUDialog(this);
            SetChildDialog(pCHarmIUDialog[i][j], E_HARM_U_I_DIALOG_A_UA + dialog_off);
            connect(pCHarmIUDialog[i][j], SIGNAL(signal_change_child_dialog(int)),
                    this, SLOT(slot_change_child_dialog(int)));
            dialog_off ++;
        }
    }

    dialog_off = 0;
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCSwitchStateDialog[i] = new CSwitchStateDialog(this);
        SetChildDialog(pCSwitchStateDialog[i], E_SWITCH_STATE_DIALOG_A + dialog_off);
        connect(pCSwitchStateDialog[i], SIGNAL(signal_change_child_dialog(int)),
                this, SLOT(slot_change_child_dialog(int)));

        pCLoadPercentDialog[i] = new CLoadPercentDialog(this);
        SetChildDialog(pCLoadPercentDialog[i], E_LOAD_PERCENT_DIALOG_A + dialog_off);
        connect(pCLoadPercentDialog[i], SIGNAL(signal_change_child_dialog(int)),
                this, SLOT(slot_change_child_dialog(int)));

        pCPowerCheckDialog[i] = new CPowerCheckDialog(this);
        SetChildDialog(pCPowerCheckDialog[i], E_POWER_CHECK_DIALOG_A + dialog_off);
        connect(pCPowerCheckDialog[i], SIGNAL(signal_change_child_dialog(int)),
                this, SLOT(slot_change_child_dialog(int)));
        dialog_off ++;
    }

    pCWarnCurrentDialog = new CWarnCurrentDialog(this);
    SetChildDialog(pCWarnCurrentDialog, E_WARN_CURRENT_DIALOG);
    connect(pCWarnCurrentDialog, SIGNAL(signal_change_child_dialog(int)),
            this, SLOT(slot_change_child_dialog(int)));
    connect(pCInitDialog, SIGNAL(signal_is_auto_2_warn_dialog(bool)),
            pCWarnCurrentDialog, SLOT(slot_is_auto_2_warn_dialog(bool)));

    pCWarnHistoryDialog = new CWarnHistoryDialog(this);

    SetChildDialog(pCWarnHistoryDialog, E_WARN_HISTORY_DIALOG);
    connect(pCWarnHistoryDialog, SIGNAL(signal_change_child_dialog(int)),
            this, SLOT(slot_change_child_dialog(int)));

    dialog_off = 0;
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCWarnSetDialog[i] = new CWarnSetDialog(this);
        SetChildDialog(pCWarnSetDialog[i], E_WARN_SET_DIALOG_A + dialog_off);
        connect(pCWarnSetDialog[i], SIGNAL(signal_change_child_dialog(int)),
                this, SLOT(slot_change_child_dialog(int)));
        dialog_off ++;
    }

    QString title_lab[E_SUB_WAY_THRESHOLD_NUM];
    int title_lab_ind = 0;
    title_lab[title_lab_ind++] = "电流下限报警阀值设置:单位A";
    title_lab[title_lab_ind++] = "电流上限报警阀值设置:单位A";
    title_lab[title_lab_ind++] = "电流下下限报警阀值设置:单位A";
    title_lab[title_lab_ind++] = "电流上上限报警阀值设置:单位A";
    title_lab[title_lab_ind++] = "熔断器报警使能设置";
    title_lab[title_lab_ind++] = "缺相报警阀值设置:单位V";
    title_lab[title_lab_ind++] = "电流量程设置:单位A";

    dialog_off = 0;
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_SUB_WAY_THRESHOLD_NUM; j++)
        {
            pCWarnThresholdDialog[i][j] = new CWarnThresholdDialog(this);
            SetChildDialog(pCWarnThresholdDialog[i][j],
                           E_WARN_THRESHOLD_DIALOG_A_I_DOWN + dialog_off);
            connect(pCWarnThresholdDialog[i][j], SIGNAL(signal_change_child_dialog(int)),
                    this, SLOT(slot_change_child_dialog(int)));
            pCWarnThresholdDialog[i][j]->SetTitle(title_lab[j]);
            dialog_off ++;
        }
    }

    pCClearDialog = new CClearDialog(this);
    SetChildDialog(pCClearDialog, E_ClEAR_DIALOG);
    connect(pCClearDialog, SIGNAL(signal_change_child_dialog(int)),
            this, SLOT(slot_change_child_dialog(int)));

    pCUserManageDialog = new CUserManageDialog(this);
    SetChildDialog(pCUserManageDialog, E_USER_MANAGE_DIALOG);
    connect(pCUserManageDialog, SIGNAL(signal_change_child_dialog(int)),
            this, SLOT(slot_change_child_dialog(int)));

    connect(pCMainPageDialog, SIGNAL(signal_current_user(st_user_inf)),
            pCUserManageDialog, SLOT(slot_current_user(st_user_inf)));

    //set child geometry
    QRect rect;
    rect.setLeft(leftWin + wLeftRight);
    rect.setTop(topWin + hTop1 + hTop2);
    rect.setWidth(widthWin - 2*wLeftRight);
    rect.setHeight(heightWin - hTop1 - hTop2 - hBottom);

    SetChildGeometry(rect);

}

void MainWindow::SetChildOuterDev()
{
    str_outer_dev* pouter_dev = gXmlAnalyze.GetOuterDev();

    pCMainPageDialog->SetOuterDev(pouter_dev);


    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCEleMainWayDialog[i]->SetOuterDev(pouter_dev);
    }

    pCEleSubWayDialog->SetOuterDev(pouter_dev);

    pCHarmPercentDialog->SetOuterDev(pouter_dev);

    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_U_I_NUM; j++)
        {
            pCHarmIUDialog[i][j]->SetOuterDev(pouter_dev);
        }
    }

    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCSwitchStateDialog[i]->SetOuterDev(pouter_dev);

        pCLoadPercentDialog[i]->SetOuterDev(pouter_dev);

        pCPowerCheckDialog[i]->SetOuterDev(pouter_dev);
    }

    pCWarnCurrentDialog->SetOuterDev(pouter_dev);

    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCWarnSetDialog[i]->SetOuterDev(pouter_dev);
    }

    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_SUB_WAY_THRESHOLD_NUM; j++)
        {
            pCWarnThresholdDialog[i][j]->SetOuterDev(pouter_dev);
        }
    }

    pCClearDialog->SetOuterDev(pouter_dev);
    pCUserManageDialog->SetOuterDev(pouter_dev);
}

int MainWindow::doGetThreShold()
{
    int ret = -1;

    emit signal_read_threshold(&signalParm);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_READ_THRESHOLD_CMD));
    ret = signalParm.is_suc;
    messagebox(this, ret, false);
    if(ret == 0)
    {
        str_outer_dev* pouter_dev = gXmlAnalyze.GetOuterDev();
        for(int i = 0; i < E_SUB_WAY_NUM; i++)
        {
            pCWarnSetDialog[i]->SetOuterDev(pouter_dev);
        }

        for(int i = 0; i < E_SUB_WAY_NUM; i++)
        {
            for(int j = 0; j < E_SUB_WAY_THRESHOLD_NUM; j++)
            {
                pCWarnThresholdDialog[i][j]->SetOuterDev(pouter_dev);
            }
        }
    }

    return ret;
}

int MainWindow::doGetColConf()
{
    int ret = -1;
    emit signal_read_col_conf(&signalParm);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_READ_COLTOR_CMD));
    ret = signalParm.is_suc;
    //    messagebox(this, ret);
    st_coltor_inf* p_st_coltor_inf = gXmlAnalyze.GetColInf();
    pCClearDialog->SetColConf(p_st_coltor_inf);
    return ret;
}

int MainWindow::test_communicate()
{
    return 0;
    while(1)
    {
        u_dev_measure *p_u_dev_measure = NULL;

        str_outer_dev* p_str_outer_dev = gXmlAnalyze.GetOuterDev();
        int port_num = p_str_outer_dev->port_num;
        for(int port_ind = 0; port_ind < port_num; port_ind ++)
        {
            st_port_dev* p_st_port_dev = &p_str_outer_dev->port_dev[port_ind];
            int dev_num = p_st_port_dev->dev_num;
            for(int dev_ind = 0; dev_ind < dev_num; dev_ind ++)
            {
                st_dev_inf* p_dev_inf = &p_st_port_dev->dev_inf[dev_ind];

                p_u_dev_measure = &p_dev_inf->dev_measure;
                int rtn = 0;
                CProModbus* p_CProModbus = new CProModbus;
                MU_INT8 dev_addr = p_dev_inf->dev_addr.id;
                MU_INT32 dev_type = p_dev_inf->dev_type;

                if(dev_type == E_DEV_ELECTRIC)
                {
                    //读量程,1-85reg
                    rtn = p_CProModbus->get_basic(*p_u_dev_measure, dev_addr);
                    if(rtn == 0)
                    {
                        //                        QMessageBox::information(this, "操作结果", "get_basic成功");
                    }
                    else
                    {
                        QMessageBox::information(this, "操作结果", "get_basic失败");
                    }

                    //电压,频率,电流，功率，功率因数,电能,开关，功率判断300-799reg
                    rtn = p_CProModbus->get_line_main(*p_u_dev_measure, dev_addr);
                    if(rtn == 0)
                    {
                        //                        QMessageBox::information(this, "操作结果", "get_line_main成功");
                    }
                    else
                    {
                        QMessageBox::information(this, "操作结果", "get_line_main失败");
                    }

                    //出线电能850-1185reg
                    rtn = p_CProModbus->get_line_sub_phase_one(*p_u_dev_measure, dev_addr);
                    if(rtn == 0)
                    {
                        //                        QMessageBox::information(this, "操作结果", "get_line_sub_phase_one成功");
                    }
                    else
                    {
                        QMessageBox::information(this, "操作结果", "get_line_sub_phase_one失败");
                    }

                    //出线三相功率1200-1395reg
                    rtn = p_CProModbus->get_line_sub_phase_three(*p_u_dev_measure, dev_addr);
                    if(rtn == 0)
                    {
                        //                        QMessageBox::information(this, "操作结果", "get_line_sub_phase_three成功");
                    }
                    else
                    {
                        QMessageBox::information(this, "操作结果", "get_line_sub_phase_three失败");
                    }

                    //1600-1808谐波
                    rtn = p_CProModbus->get_harm(*p_u_dev_measure, dev_addr);
                    if(rtn == 0)
                    {
                        //                        QMessageBox::information(this, "操作结果", "get_harm成功");
                    }
                    else
                    {
                        QMessageBox::information(this, "操作结果", "get_harm失败");
                    }

                    rtn = p_CProModbus->get_event(*p_u_dev_measure, dev_addr);
                    if(rtn == 0)
                    {
                        //                        QMessageBox::information(this, "操作结果", "get_event成功");
                    }
                    else
                    {
                        QMessageBox::information(this, "操作结果", "get_event失败");
                    }
                }
                else if(dev_type == E_DEV_SWITCH)
                {
                    //开关量0-8reg
                    rtn = p_CProModbus->get_switch(*p_u_dev_measure, dev_addr);
                    if(rtn == 0)
                    {
                        //                        QMessageBox::information(this, "操作结果", "get_switch成功");
                    }
                    else
                    {
                        QMessageBox::information(this, "操作结果", "get_switch");
                    }
                }
                delete p_CProModbus;
            }
        }
    }
    return 0;
}

void MainWindow::SetConnectTrans()
{
    for(int i = 0; i < E_SUB_WAY_NUM; i ++)
    {
        for(int j = 0; j < E_SUB_WAY_THRESHOLD_NUM; j ++)
        {
            connect(pCWarnThresholdDialog[i][j],
                    SIGNAL(signal_write_i_range_one(st_signal_parm* , const st_thread_param_i_range_one*)),
                    pCTransfThread,
                    SLOT(slot_write_i_range_one(st_signal_parm* , const st_thread_param_i_range_one*)),
                    Qt::QueuedConnection);
            connect(pCTransfThread,
                    SIGNAL(signal_write_i_range_one_finish(st_signal_parm*)),
                    pCWarnThresholdDialog[i][j],
                    SLOT(slot_write_i_range_one_finish(st_signal_parm*)),
                    Qt::QueuedConnection);

            connect(pCWarnThresholdDialog[i][j],
                    SIGNAL(signal_write_i_range_all(st_signal_parm* , const st_thread_param_i_range_all*)),
                    pCTransfThread,
                    SLOT(slot_write_i_range_all(st_signal_parm* , const st_thread_param_i_range_all*)),
                    Qt::QueuedConnection);
            connect(pCTransfThread,
                    SIGNAL(signal_write_i_range_all_finish(st_signal_parm*)),
                    pCWarnThresholdDialog[i][j],
                    SLOT(slot_write_i_range_all_finish(st_signal_parm*)),
                    Qt::QueuedConnection);

            connect(pCWarnThresholdDialog[i][j],
                    SIGNAL(signal_write_threshold(st_signal_parm* , const st_thread_param_threshold_set*)),
                    pCTransfThread,
                    SLOT(slot_write_threshold(st_signal_parm* , const st_thread_param_threshold_set*)),
                    Qt::QueuedConnection);
            connect(pCTransfThread,
                    SIGNAL(signal_write_threshold_finish(st_signal_parm*)),
                    pCWarnThresholdDialog[i][j],
                    SLOT(slot_write_threshold_finish(st_signal_parm*)),
                    Qt::QueuedConnection);

            connect(pCWarnThresholdDialog[i][j],
                    SIGNAL(signal_i_range_update(st_signal_parm* , bool)),
                    this,
                    SLOT(slot_i_range_update(st_signal_parm* , bool)),
                    Qt::AutoConnection);

            connect(pCWarnThresholdDialog[i][j],
                    SIGNAL(signal_is_get_threshold(st_signal_parm* , bool)),
                    this,
                    SLOT(slot_is_get_threshold(st_signal_parm* , bool)),
                    Qt::AutoConnection);
        }
    }

    for(int i = 0; i < E_SUB_WAY_NUM; i ++)
    {
        connect(pCWarnSetDialog[i],
                SIGNAL(signal_write_threshold(st_signal_parm* , const st_thread_param_threshold_set*)),
                pCTransfThread,
                SLOT(slot_write_threshold(st_signal_parm* , const st_thread_param_threshold_set*)),
                Qt::QueuedConnection);
        connect(pCTransfThread,
                SIGNAL(signal_write_threshold_finish(st_signal_parm*)),
                pCWarnSetDialog[i],
                SLOT(slot_write_threshold_finish(st_signal_parm*)),
                Qt::QueuedConnection);
    }

    connect(this,
            SIGNAL(signal_read_threshold(st_signal_parm*)),
            pCTransfThread,
            SLOT(slot_read_threshold(st_signal_parm*)),
            Qt::QueuedConnection);
    connect(pCTransfThread,
            SIGNAL(signal_read_threshold_finish(st_signal_parm*)),
            this,
            SLOT(slot_read_threshold_finish(st_signal_parm*)),
            Qt::QueuedConnection);
    connect(this,
            SIGNAL(signal_read_col_conf(st_signal_parm*)),
            pCTransfThread,
            SLOT(slot_read_col_conf(st_signal_parm*)),
            Qt::QueuedConnection);
    connect(pCTransfThread,
            SIGNAL(signal_read_col_conf_finish(st_signal_parm*)),
            this,
            SLOT(slot_read_col_conf_finish(st_signal_parm*)),
            Qt::QueuedConnection);

    connect(pCWarnCurrentDialog, SIGNAL(signal_write_warn_history(const st_warn_history_inf*)),
            pCTransfThread, SLOT(slot_write_warn_history(const st_warn_history_inf*)),
            Qt::QueuedConnection);
    connect(pCWarnCurrentDialog, SIGNAL(signal_write_relay(const st_write_relay_param*)),
            pCTransfThread, SLOT(slot_write_relay(const st_write_relay_param*)),
            Qt::QueuedConnection);

    connect(pCWarnHistoryDialog,
            SIGNAL(signal_read_warn_history(st_signal_parm*, st_thread_param_warn_history_get*)),
            pCTransfThread,
            SLOT(slot_read_warn_history(st_signal_parm*, st_thread_param_warn_history_get*)),
            Qt::QueuedConnection);
    connect(pCTransfThread,
            SIGNAL(signal_read_warn_history_finish(st_signal_parm* , st_thread_param_warn_history_get*)),
            pCWarnHistoryDialog,
            SLOT(slot_read_warn_history_finish(st_signal_parm* , st_thread_param_warn_history_get*)),
            Qt::QueuedConnection);

    connect(pCClearDialog,
            SIGNAL(signal_write_zero_energy(st_signal_parm* , const st_thread_param_zero_energy_set*)),
            pCTransfThread,
            SLOT(slot_write_zero_energy(st_signal_parm* , const st_thread_param_zero_energy_set*)),
            Qt::QueuedConnection);
    connect(pCTransfThread,
            SIGNAL(signal_write_zero_energy_finish(st_signal_parm*)),
            pCClearDialog,
            SLOT(slot_write_zero_energy_finish(st_signal_parm*)),
            Qt::QueuedConnection);
    connect(pCClearDialog,
            SIGNAL(signal_write_col_conf(st_signal_parm*)),
            pCTransfThread,
            SLOT(slot_write_col_conf(st_signal_parm*)),
            Qt::QueuedConnection);
    connect(pCTransfThread,
            SIGNAL(signal_write_col_conf_finish(st_signal_parm*)),
            pCClearDialog,
            SLOT(slot_write_col_conf_finish(st_signal_parm*)),
            Qt::QueuedConnection);

    connect(pCClearDialog,
            SIGNAL(signal_write_col_time(st_signal_parm*)),
            pCTransfThread,
            SLOT(slot_write_col_time(st_signal_parm*)),
            Qt::QueuedConnection);
    connect(pCTransfThread,
            SIGNAL(signal_write_col_time_finish(st_signal_parm*)),
            pCClearDialog,
            SLOT(slot_write_col_time_finish(st_signal_parm*)),
            Qt::QueuedConnection);

    connect(pCClearDialog,
            SIGNAL(signal_upgrade(st_signal_parm*)),
            pCTransfThread,
            SLOT(slot_upgrade(st_signal_parm*)),
            Qt::QueuedConnection);
    connect(pCTransfThread,
            SIGNAL(signal_upgrade_finish(st_signal_parm*)),
            pCClearDialog,
            SLOT(slot_upgrade_finish(st_signal_parm*)),
            Qt::QueuedConnection);
}

void MainWindow::slot_change_child_dialog(int child_dialog_ind)
{
    hideChild();

    ui->widget_title->setStyleSheet(gResTitleGreen);
    ui->label_title_main->show();
    ui->label_title_sub->show();

    switch(child_dialog_ind)
    {
    case E_INIT_DIALOG:
        ui->widget_title->setStyleSheet(gResTitleInit);
        ui->label_title_main->hide();
        ui->label_title_sub->hide();
        pCInitDialog->show();
        break;
    case E_MAIN_PAGE_DIALOG:
        ui->label_title_sub->setText("主页");
        pCMainPageDialog->show();
        break;
    case E_ELE_MAIN_WAY_DIALOG_A:
        ui->label_title_sub->setText("主路A电参数");
        pCEleMainWayDialog[E_SUB_WAY_A]->show();
        break;
    case E_ELE_MAIN_WAY_DIALOG_B:
        ui->label_title_sub->setText("主路B电参数");
        pCEleMainWayDialog[E_SUB_WAY_B]->show();
        break;
    case E_ELE_SUB_WAY_DIALOG:
        ui->label_title_sub->setText("支路电参数");
        pCEleSubWayDialog->show();
        break;
    case E_HARM_PERCENT_DIALOG:
        ui->label_title_sub->setText("谐波百分比");
        pCHarmPercentDialog->show();
        break;
    case E_HARM_U_I_DIALOG_A_UA:
        ui->label_title_sub->setText("主路A_A电压谐波");
        pCHarmIUDialog[E_SUB_WAY_A][E_U_A]->show();
        break;
    case E_HARM_U_I_DIALOG_A_UB:
        ui->label_title_sub->setText("主路A_B电压谐波");
        pCHarmIUDialog[E_SUB_WAY_A][E_U_B]->show();
        break;
    case E_HARM_U_I_DIALOG_A_UC:
        ui->label_title_sub->setText("主路A_C电压谐波");
        pCHarmIUDialog[E_SUB_WAY_A][E_U_C]->show();
        break;
    case E_HARM_U_I_DIALOG_A_IA:
        ui->label_title_sub->setText("主路A_A电流谐波");
        pCHarmIUDialog[E_SUB_WAY_A][E_I_A]->show();
        break;
    case E_HARM_U_I_DIALOG_A_IB:
        ui->label_title_sub->setText("主路A_B电流谐波");
        pCHarmIUDialog[E_SUB_WAY_A][E_I_B]->show();
        break;
    case E_HARM_U_I_DIALOG_A_IC:
        ui->label_title_sub->setText("主路A_C电流谐波");
        pCHarmIUDialog[E_SUB_WAY_A][E_I_C]->show();
        break;
    case E_HARM_U_I_DIALOG_B_UA:
        ui->label_title_sub->setText("主路B_A电压谐波");
        pCHarmIUDialog[E_SUB_WAY_B][E_U_A]->show();
        break;
    case E_HARM_U_I_DIALOG_B_UB:
        ui->label_title_sub->setText("主路B_B电压谐波");
        pCHarmIUDialog[E_SUB_WAY_B][E_U_B]->show();
        break;
    case E_HARM_U_I_DIALOG_B_UC:
        ui->label_title_sub->setText("主路B_C电压谐波");
        pCHarmIUDialog[E_SUB_WAY_B][E_U_C]->show();
        break;
    case E_HARM_U_I_DIALOG_B_IA:
        ui->label_title_sub->setText("主路B_A电流谐波");
        pCHarmIUDialog[E_SUB_WAY_B][E_I_A]->show();
        break;
    case E_HARM_U_I_DIALOG_B_IB:
        ui->label_title_sub->setText("主路B_B电流谐波");
        pCHarmIUDialog[E_SUB_WAY_B][E_I_B]->show();
        break;
    case E_HARM_U_I_DIALOG_B_IC:
        ui->label_title_sub->setText("主路B_C电流谐波");
        pCHarmIUDialog[E_SUB_WAY_B][E_I_C]->show();
        break;
    case E_SWITCH_STATE_DIALOG_A:
        ui->label_title_sub->setText("主路A开关状态");
        pCSwitchStateDialog[E_SUB_WAY_A]->show();
        break;
    case E_SWITCH_STATE_DIALOG_B:
        ui->label_title_sub->setText("主路B开关状态");
        pCSwitchStateDialog[E_SUB_WAY_B]->show();
        break;
    case E_LOAD_PERCENT_DIALOG_A:
        ui->label_title_sub->setText("主路A负载百分比");
        pCLoadPercentDialog[E_SUB_WAY_A]->show();
        break;
    case E_LOAD_PERCENT_DIALOG_B:
        ui->label_title_sub->setText("主路B负载百分比");
        pCLoadPercentDialog[E_SUB_WAY_B]->show();
        break;
    case E_POWER_CHECK_DIALOG_A:
        ui->label_title_sub->setText("A功率判断");
        pCPowerCheckDialog[E_SUB_WAY_A]->show();
        break;
    case E_POWER_CHECK_DIALOG_B:
        ui->label_title_sub->setText("B功率判断");
        pCPowerCheckDialog[E_SUB_WAY_B]->show();
        break;
    case E_WARN_CURRENT_DIALOG:
        ui->label_title_sub->setText("当前告警");
        pCWarnCurrentDialog->show();
        break;
    case E_WARN_HISTORY_DIALOG:
        ui->label_title_sub->setText("历史告警");
        pCWarnHistoryDialog->GetHistoryWarn();
        pCWarnHistoryDialog->show();
        break;
    case E_WARN_SET_DIALOG_A:
        ui->label_title_sub->setText("主路A告警设置");
        if(isGetThreshold)
        {
            doGetThreShold();
        }
        isGetThreshold = true;
        pCWarnSetDialog[E_SUB_WAY_A]->show();
        break;
    case E_WARN_SET_DIALOG_B:
        ui->label_title_sub->setText("主路B告警设置");
        pCWarnSetDialog[E_SUB_WAY_B]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_DOWN:
        ui->label_title_sub->setText("主路A支路电流下限");
        pCWarnThresholdDialog[E_SUB_WAY_A][E_CURRENT_LOW_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_UP:
        ui->label_title_sub->setText("主路A支路电流上限");
        pCWarnThresholdDialog[E_SUB_WAY_A][E_CURRENT_HIGH_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_DOWN2:
        ui->label_title_sub->setText("主路A支路电流下下限");
        pCWarnThresholdDialog[E_SUB_WAY_A][E_CURRENT_LOW2_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_UP2:
        ui->label_title_sub->setText("主路A支路电流上上限");
        pCWarnThresholdDialog[E_SUB_WAY_A][E_CURRENT_HIGH2_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_A_FUSE:
        ui->label_title_sub->setText("主路A支路熔断器");
        pCWarnThresholdDialog[E_SUB_WAY_A][E_FUSE_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_A_LOSE_PHASE:
        ui->label_title_sub->setText("主路A支路缺相");
        pCWarnThresholdDialog[E_SUB_WAY_A][E_LOSE_PHASE_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_A_CONTENT:
        ui->label_title_sub->setText("主路A支路容量");
        pCWarnThresholdDialog[E_SUB_WAY_A][E_CONTENT_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_B_I_DOWN:
        ui->label_title_sub->setText("主路B支路电流下限");
        pCWarnThresholdDialog[E_SUB_WAY_B][E_CURRENT_LOW_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_B_I_UP:
        ui->label_title_sub->setText("主路B支路电流上限");
        pCWarnThresholdDialog[E_SUB_WAY_B][E_CURRENT_HIGH_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_B_I_DOWN2:
        ui->label_title_sub->setText("主路B支路电流下下限");
        pCWarnThresholdDialog[E_SUB_WAY_B][E_CURRENT_LOW2_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_B_I_UP2:
        ui->label_title_sub->setText("主路B支路电流上上限");
        pCWarnThresholdDialog[E_SUB_WAY_B][E_CURRENT_HIGH2_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_B_FUSE:
        ui->label_title_sub->setText("主路B支路熔断器");
        pCWarnThresholdDialog[E_SUB_WAY_B][E_FUSE_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_B_LOSE_PHASE:
        ui->label_title_sub->setText("主路B支路缺相");
        pCWarnThresholdDialog[E_SUB_WAY_B][E_LOSE_PHASE_DIALOG]->show();
        break;
    case E_WARN_THRESHOLD_DIALOG_B_CONTENT:
        ui->label_title_sub->setText("主路B支路容量");
        pCWarnThresholdDialog[E_SUB_WAY_B][E_CONTENT_DIALOG]->show();
        break;
    case E_ClEAR_DIALOG:
        ui->label_title_sub->setText("清除设置");
        doGetColConf();
        pCClearDialog->show();
        break;
    case E_USER_MANAGE_DIALOG:
        ui->label_title_sub->setText("用户管理");
        pCUserManageDialog->show();
    default:
        break;
    }
}

void MainWindow::slot_i_range_update(st_signal_parm *p_sig_parm, bool is_update)
{
    isUpdateIRange = is_update;
    //    qDebug("[%s] is_update=%d", __FUNCTION__, is_update);
}

void MainWindow::slot_is_get_threshold(st_signal_parm *p_sig_parm, bool is_get)
{
    isGetThreshold = is_get;
}

void MainWindow::slot_time_update(QString date, QString sys_time, QString week_day)
{
    SetDate(date);
    SetSysTime(sys_time);
    SetWeekDay(week_day);
}

void MainWindow::slot_realtime_data_update(const st_dev_inf* p_dev_inf)
{
    //update ui
    pCMainPageDialog->SetUi(p_dev_inf);
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCEleMainWayDialog[i]->SetUi(p_dev_inf);
    }
    pCEleSubWayDialog->SetUi(p_dev_inf);
    pCHarmPercentDialog->SetUi(p_dev_inf);
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_U_I_NUM; j++)
        {
            pCHarmIUDialog[i][j]->SetUi(p_dev_inf);
        }
    }
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCSwitchStateDialog[i]->SetUi(p_dev_inf);
    }
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCLoadPercentDialog[i]->SetUi(p_dev_inf);
    }
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        pCPowerCheckDialog[i]->SetUi(p_dev_inf);
    }
    pCWarnCurrentDialog->SetUi(p_dev_inf);
    for(int i = 0; i < E_SUB_WAY_NUM; i++)
    {
        for(int j = 0; j < E_SUB_WAY_THRESHOLD_NUM; j++)
        {
            if(j == E_CONTENT_DIALOG && !isUpdateIRange)
            {
                continue;
            }
            else
            {
                pCWarnThresholdDialog[i][j]->SetUi(p_dev_inf);
            }
        }
    }
}

void MainWindow::slot_read_threshold_finish(st_signal_parm* p_sig_parm)
{
    close_progress(pCProgressDialog);

    //    static int cnt = 0;
    //    qDebug("[%s] after close cnt=%d", __FUNCTION__, cnt++);
}

void MainWindow::slot_read_col_conf_finish(st_signal_parm *p_sig_parm)
{
    close_progress(pCProgressDialog);

    //    static int cnt = 0;
    //    qDebug("[%s] after close cnt=%d", __FUNCTION__, cnt++);
}


