#include "ccleardialog.h"
#include "ui_ccleardialog.h"
#include "logindialog.h"

#include "CommonFunction.h"
#include "dataConvert.h"
#include "ccommonchild.h"
#include "xmlAnalyze.h"
#include "cpipe.h"

CClearDialog::CClearDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CClearDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    pOuterDev = NULL;
    pColtorInf = NULL;

    pCProgressDialog = new CProgressDialog(this);

    memset(&signalParm, 0, sizeof(signalParm));

    InitUi();

    connect(ui->pushButton_return_back, SIGNAL(clicked()),
            this, SLOT(slot_to_main_page_clicked()));
    connect(ui->pushButton_id, SIGNAL(clicked()), this, SLOT(slot_id_clicked()));
    connect(ui->pushButton_time, SIGNAL(clicked()), this, SLOT(slot_time_clicked()));

    connect(ui->pushButton_clear_a_all, SIGNAL(clicked()),
            this, SLOT(slot_clear_a_all_clicked()));
    connect(ui->pushButton_clear_a_m, SIGNAL(clicked()),
            this, SLOT(slot_clear_a_m_clicked()));
    connect(ui->pushButton_clear_b_m, SIGNAL(clicked()),
            this, SLOT(slot_clear_b_m_clicked()));
    connect(ui->pushButton_clear_a_sub, SIGNAL(clicked()),
            this, SLOT(slot_clear_a_sub_clicked()));
    connect(ui->pushButton_clear_b_sub, SIGNAL(clicked()),
            this, SLOT(slot_clear_b_sub_clicked()));
    connect(ui->pushButton_clear_a_frozen_energy, SIGNAL(clicked()),
            this, SLOT(slot_clear_a_frozen_energy_clicked()));
    connect(ui->pushButton_upgrade, SIGNAL(clicked()),
            this, SLOT(slot_upgrade_clicked()));
    connect(ui->pushButton_pwd_modify, SIGNAL(clicked()),
            this, SLOT(slot_modify_pwd_clicked()));
    connect(ui->pushButton_reboot, SIGNAL(clicked()),
            this, SLOT(slot_reboot_clicked()));
    connect(ui->pushButton_version, SIGNAL(clicked()),
            this, SLOT(slot_bt_version_clicked()));

    ui->lineEdit_id->SetValid(E_VALID_INT);
    ui->lineEdit_time->SetValid(E_VALID_ALL_TYPE);
}

CClearDialog::~CClearDialog()
{
    delete ui;
}

void CClearDialog::slot_to_main_page_clicked()
{
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CClearDialog::slot_id_clicked()
{
    int ret = 0;

    QString text;
    text = ui->lineEdit_id->text();
    int id = text.toInt();
    if(id < 1 || id > 255)
    {
        QMessageBox::information(this, "提示", "地址非法");
        return;
    }

    pColtorInf->col_conf.id = id;

    emit signal_write_col_conf(&signalParm);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_WRITE_COLTOR_CMD));
    ret = signalParm.is_suc;
    messagebox(this, ret);
}

void CClearDialog::slot_time_clicked()
{
    int ret = 0;

    QString text;
    text = ui->lineEdit_time->text();
    //2016-10-26 16:14:30
    st_time time_now;
    GetNowTime(time_now);
    int yy = time_now.YY;
    int mm = time_now.MM;
    int dd = time_now.DD;
    int hh = time_now.HH;
    int mi = time_now.MI;
    int ss = time_now.SS;
    sscanf(qstr_2_str(text),"%d-%d-%d %d:%d:%d",&yy,&mm,&dd,&hh,&mi,&ss);
    if((yy < 1980 || yy > 2037)
            || (mm < 1 || mm > 12)
            || (dd < 1 || dd > 31)
            || (hh < 0 || hh > 23)
            || (mi < 0 || mi > 59)
            || (ss < 0 || ss > 59)
            )
    {
        messagebox(this, -1);
        return;
    }

    pColtorInf->dev_time.dev_time.YY = yy;
    pColtorInf->dev_time.dev_time.MM = mm;
    pColtorInf->dev_time.dev_time.DD = dd;
    pColtorInf->dev_time.dev_time.HH = hh;
    pColtorInf->dev_time.dev_time.MI = mi;
    pColtorInf->dev_time.dev_time.SS = ss;

    emit signal_write_col_time(&signalParm);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_WRITE_COLTOR_TIME_CMD));
    ret = signalParm.is_suc;
    messagebox(this, ret);
}

void CClearDialog::slot_clear_a_all_clicked()
{
    int ret = 0;

    int way_code = MAIN_A_CODE;
    const st_dev_inf* p_st_dev_inf
            = GetDevInfByCode((str_outer_dev*)pOuterDev, way_code, E_DEV_ELECTRIC);
    if(!p_st_dev_inf)
    {
        return;
    }
    st_thread_param_zero_energy_set param_zero_energy_set;
    memset(&param_zero_energy_set, 0, sizeof(param_zero_energy_set));
    param_zero_energy_set.dev_addr = p_st_dev_inf->dev_addr.id;
    param_zero_energy_set.reg_val = 1;

    emit signal_write_zero_energy(&signalParm, &param_zero_energy_set);
    open_progress(pCProgressDialog, GetTaskNameByType(E_WRITE_ZERO_ENERGY_CMD));
    ret = signalParm.is_suc;

    messagebox(this, ret);
}

void CClearDialog::slot_clear_a_m_clicked()
{
    int ret = 0;

    int way_code = MAIN_A_CODE;
    const st_dev_inf* p_st_dev_inf
            = GetDevInfByCode((str_outer_dev*)pOuterDev, way_code, E_DEV_ELECTRIC);
    if(!p_st_dev_inf)
    {
        return;
    }
    st_thread_param_zero_energy_set param_zero_energy_set;
    memset(&param_zero_energy_set, 0, sizeof(param_zero_energy_set));
    param_zero_energy_set.dev_addr = p_st_dev_inf->dev_addr.id;
    param_zero_energy_set.reg_val = 2;

    emit signal_write_zero_energy(&signalParm, &param_zero_energy_set);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_WRITE_ZERO_ENERGY_CMD));
    ret = signalParm.is_suc;

    messagebox(this, ret);
}

void CClearDialog::slot_clear_b_m_clicked()
{
    int ret = 0;

    int way_code = MAIN_B_CODE;
    const st_dev_inf* p_st_dev_inf = GetDevInfByCode((str_outer_dev*)pOuterDev,
                                                     way_code, E_DEV_ELECTRIC);
    if(!p_st_dev_inf)
    {
        return;
    }
    st_thread_param_zero_energy_set param_zero_energy_set;
    memset(&param_zero_energy_set, 0, sizeof(param_zero_energy_set));
    param_zero_energy_set.dev_addr = p_st_dev_inf->dev_addr.id;
    param_zero_energy_set.reg_val = 3;

    emit signal_write_zero_energy(&signalParm, &param_zero_energy_set);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_WRITE_ZERO_ENERGY_CMD));
    ret = signalParm.is_suc;

    messagebox(this, ret);
}

void CClearDialog::slot_clear_a_sub_clicked()
{
    int ret = 0;

    int way_code = MAIN_A_CODE;
    const st_dev_inf* p_st_dev_inf = GetDevInfByCode((str_outer_dev*)pOuterDev,
                                                     way_code, E_DEV_ELECTRIC);
    if(!p_st_dev_inf)
    {
        return;
    }
    st_thread_param_zero_energy_set param_zero_energy_set;
    memset(&param_zero_energy_set, 0, sizeof(param_zero_energy_set));
    param_zero_energy_set.dev_addr = p_st_dev_inf->dev_addr.id;
    param_zero_energy_set.reg_val = 4;

    emit signal_write_zero_energy(&signalParm, &param_zero_energy_set);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_WRITE_ZERO_ENERGY_CMD));
    ret = signalParm.is_suc;

    messagebox(this, ret);
}

void CClearDialog::slot_clear_b_sub_clicked()
{
    int ret = 0;

    int way_code = MAIN_B_CODE;
    const st_dev_inf* p_st_dev_inf = GetDevInfByCode((str_outer_dev*)pOuterDev, way_code, E_DEV_ELECTRIC);
    if(!p_st_dev_inf)
    {
        return;
    }
    st_thread_param_zero_energy_set param_zero_energy_set;
    memset(&param_zero_energy_set, 0, sizeof(param_zero_energy_set));
    param_zero_energy_set.dev_addr = p_st_dev_inf->dev_addr.id;
    param_zero_energy_set.reg_val = 5;

    emit signal_write_zero_energy(&signalParm, &param_zero_energy_set);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_WRITE_ZERO_ENERGY_CMD));
    ret = signalParm.is_suc;

    messagebox(this, ret);
}

void CClearDialog::slot_clear_a_frozen_energy_clicked()
{
    int ret = 0;

    int way_code = MAIN_A_CODE;
    const st_dev_inf* p_st_dev_inf = GetDevInfByCode((str_outer_dev*)pOuterDev,
                                                     way_code, E_DEV_ELECTRIC);
    if(!p_st_dev_inf)
    {
        return;
    }
    st_thread_param_zero_energy_set param_zero_energy_set;
    memset(&param_zero_energy_set, 0, sizeof(param_zero_energy_set));
    param_zero_energy_set.dev_addr = p_st_dev_inf->dev_addr.id;
    param_zero_energy_set.reg_val = 6;

    emit signal_write_zero_energy(&signalParm, &param_zero_energy_set);
    open_progress(pCProgressDialog,
                  GetTaskNameByType(E_WRITE_ZERO_ENERGY_CMD));
    ret = signalParm.is_suc;

    messagebox(this, ret);
}

void CClearDialog::slot_upgrade_clicked()
{
    int ret = 0;

    emit signal_upgrade(&signalParm);
    open_progress(pCProgressDialog, GetTaskNameByType(E_WRITE_UPGRADE_CMD));
    ret = signalParm.is_suc;
    if(ret == 0)
    {
        QMessageBox::information(this, "操作结果", "升级成功，重启生效");
    }
    else
    {
        QMessageBox::information(this, "操作结果", "升级失败");
    }
}

void CClearDialog::slot_modify_pwd_clicked()
{
    emit signal_change_child_dialog(E_USER_MANAGE_DIALOG);
}

void CClearDialog::slot_reboot_clicked()
{
    //arm
    CLoginDialog* p_CLoginDialog = new CLoginDialog;
    DEQ_ST_USER_INF* p_deqStUserInf = gXmlAnalyze.GetUserInf();
    p_CLoginDialog->SetDeqUserInf(p_deqStUserInf);
    int ret = LogIn(this, p_CLoginDialog);
    if(ret == 0)
    {
#if __arm__
//        ret = system("ls");
//        qDebug("[%s] ls,ret=%d", __FUNCTION__, ret);
//        ret = system("reboot");
//        if(0 != ret)
//        {
//            qDebug("[%s] reboot,ret=%d,err=%s",
//                   __FUNCTION__, ret, strerror(errno));
//        }

        char send_msg[32];
        sprintf(send_msg, "reboot");
        gCpipe.WritePipe(send_msg, strlen(send_msg));

#endif
    }
    delete p_CLoginDialog;
}

void CClearDialog::slot_bt_version_clicked()
{
    QString text = "V_1.0";
    text = "20170209114030";
    QMessageBox::information(this, "版本", text);
}

void CClearDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;
}

void CClearDialog::SetColConf(st_coltor_inf *coltor_inf)
{
    pColtorInf = coltor_inf;

    int id = pColtorInf->col_conf.id;
    QString text;
    text = QString::number(id);
    ui->lineEdit_id->setText(text);


    st_time time_now;
    GetNowTime(time_now);
    QString str_time_date;
    str_time_date.sprintf("%04d-%02d-%02d %02d:%02d:%02d",
                          time_now.YY, time_now.MM, time_now.DD,
                          time_now.HH, time_now.MI, time_now.SS);
    ui->lineEdit_time->setText(str_time_date);
}

void CClearDialog::InitUi()
{
    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);

    QString str_label_color = "color:yellow";
    ui->label_id->setStyleSheet(str_label_color);
    ui->label_time->setStyleSheet(str_label_color);
    ui->label_time_example->setStyleSheet(str_label_color);

    ui->label_id->setText("本机地址");
    ui->label_time->setText("本机时间");
    ui->label_time_example->setText("时间格式(yyyy-mm-dd hh:mi:ss)");

    //ui->pushButton_reboot->setVisible(false);

    QFont ft;
    ft.setPointSize(10);
    ui->lineEdit_time->setFont(ft);
}

void CClearDialog::slot_write_zero_energy_finish(st_signal_parm* p_sig_parm)
{
    close_progress(pCProgressDialog);
}

void CClearDialog::slot_write_col_conf_finish(st_signal_parm *p_sig_parm)
{
    close_progress(pCProgressDialog);
}

void CClearDialog::slot_write_col_time_finish(st_signal_parm *p_sig_parm)
{
    close_progress(pCProgressDialog);
}

void CClearDialog::slot_upgrade_finish(st_signal_parm *p_sig_parm)
{
    close_progress(pCProgressDialog);
}
