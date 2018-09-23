#include "cwarncurrentdialog.h"
#include "ui_cwarncurrentdialog.h"
#include <QScrollBar>
#include <QLabel>
#include "CommonFunction.h"
#include "dataConvert.h"
#include "protocol/promodbus.h"

static MU_INT32 s_colum = 3;
static string s_phase[3] = {"A", "B", "C"};

CWarnCurrentDialog::CWarnCurrentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CWarnCurrentDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    isAuto2WarnDialog = true;
    pOuterDev = NULL;

    deqStWarnCurrentInf.clear();

    rowMax = MAX_WAY_NUM * E_SUB_WAY_NUM * E_WARN_EVENT_NUM;
    InitUi();

    connect(ui->pushButton_pre_page, SIGNAL(clicked()),
            this, SLOT(slot_pre_page_clicked()));
    connect(ui->pushButton_next_page, SIGNAL(clicked()),
            this, SLOT(slot_next_page_clicked()));
    connect(ui->pushButton_return_back, SIGNAL(clicked()),
            this, SLOT(slot_to_main_page_clicked()));
    connect(ui->pushButton_pre_list, SIGNAL(clicked()),
            this, SLOT(slot_pre_list_clicked()));
    connect(ui->pushButton_next_list, SIGNAL(clicked()),
            this, SLOT(slot_next_list_clicked()));

    connect(ui->pushButton_close_sound, SIGNAL(clicked()),
            this, SLOT(slot_bt_close_sound_clicked()));
}

CWarnCurrentDialog::~CWarnCurrentDialog()
{
    delete ui;
}

void CWarnCurrentDialog::slot_to_main_page_clicked()
{
    ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CWarnCurrentDialog::slot_pre_page_clicked()
{
    //test add history warn
    //    static int cnt = 0;
    //    cnt++;
    //    st_warn_history_inf* warn_his_old = new st_warn_history_inf;
    //    warn_his_old->dev_addr = 1;
    //    warn_his_old->dev_type = E_DEV_ELECTRIC;
    //    warn_his_old->is_warn = 1;
    //    warn_his_old->obj_name = "obj";
    //    warn_his_old->start_time = "19491001121314";
    //    warn_his_old->stop_time = "19591001121314";
    //    warn_his_old->warn_type = "1";
    //    warn_his_old->way_code = MAIN_A_CODE + 1 + cnt % SUB_WAY_PARAM_NUM ;
    //    emit signal_write_warn_history(warn_his_old);
}

void CWarnCurrentDialog::slot_next_page_clicked()
{
    int ind = dialogInd;
    ind = E_WARN_HISTORY_DIALOG;

    ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
    emit signal_change_child_dialog(ind);
}

static int pageValue = 10; // 一页显示条数
void CWarnCurrentDialog::slot_pre_list_clicked()
{
    int max_value = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
    int cur_scroller = ui->tableWidget->verticalScrollBar()->value();

    if(cur_scroller - pageValue >= 0)
    {
        ui->tableWidget->verticalScrollBar()->setSliderPosition(cur_scroller - pageValue);
    }
    else
    {
        ui->tableWidget->verticalScrollBar()->setSliderPosition(max_value);
    }
}

void CWarnCurrentDialog::slot_next_list_clicked()
{
    int max_value = ui->tableWidget->verticalScrollBar()->maximum(); // 当前SCROLLER最大显示值25
    int cur_scroller = ui->tableWidget->verticalScrollBar()->value(); //获得当前scroller值

    if(pageValue + cur_scroller <= max_value)
    {
        ui->tableWidget->verticalScrollBar()->setSliderPosition(pageValue + cur_scroller);
    }
    else
    {
        ui->tableWidget->verticalScrollBar()->setSliderPosition(0);
    }
}

void CWarnCurrentDialog::slot_is_auto_2_warn_dialog(bool is_auto_2_warn_dialog)
{
    isAuto2WarnDialog = is_auto_2_warn_dialog;
}

void CWarnCurrentDialog::slot_bt_close_sound_clicked()
{
    //新电表告警，写继电器闭合0x01
    st_write_relay_param* write_relay_param = new st_write_relay_param;
    memset(write_relay_param, 0, sizeof(*write_relay_param));
    MU_INT8 dev_addr = 1;
    write_relay_param->dev_addr = dev_addr;
    write_relay_param->relay_val = 0x00;
    write_relay_param->reg_addr = REG_RELAY_M_SOUND;
    emit signal_write_relay(write_relay_param);
    Sleep(10);
}

void CWarnCurrentDialog::SetUi(const st_dev_inf* p_dev_inf)
{
    int dev_type = p_dev_inf->dev_type;
    if(dev_type != E_DEV_ELECTRIC && dev_type != E_DEV_SWITCH)
    {
        return;
    }

    bool is_use = false;
    MU_INT32 thre = 0;
    MU_INT16 switch_state = 0;
    string now_time = GetNowTimeStr();
    string warn_type;
    string obj_name;
    int m_ind = -1;

    if(dev_type == E_DEV_ELECTRIC)
    {
        for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)
        {
            bool is_way_use = IsWayUsedByMask(way_ind,
                                              p_dev_inf->way_inf.way_mask);
            if(is_way_use)
            {
                const st_way_data* p_st_way_data = &p_dev_inf->way_inf.way_data[way_ind];
                const st_ac_meter* p_ac_meter = &p_dev_inf->dev_measure.ac_meter;
                int way_id = p_st_way_data->way_id;
                if(way_id == MAIN_A_CODE || way_id == MAIN_B_CODE)
                {
                    if(way_id == MAIN_A_CODE)
                    {
                        //主路A
                        obj_name = "主路A的";
                        m_ind = 0;
                    }
                    else if(way_id == MAIN_B_CODE)
                    {
                        //主路B
                        obj_name = "主路B的";
                        m_ind = 1;
                    }
                    //仅主路电阀值
                    thre = p_st_way_data->way_threshold.u_down_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.u_down_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电压下限";
                        for(int j = 0; j < 3; j++)
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;
                            sprintf(p_warn_history_inf->obj_name,
                                    "%s%s相电压", obj_name.c_str(), s_phase[j].c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    p_ac_meter->inline_m_param[m_ind].u_phase[j] * gOneUnit - thre * gOneUnit < gTwoUnit;
                            WarnCheck(p_warn_history_inf);
                        }
                    }

                    thre = p_st_way_data->way_threshold.u_up_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.u_up_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电压上限";
                        for(int j = 0; j < 3; j++)
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;
                            sprintf(p_warn_history_inf->obj_name,
                                    "%s%s相电压", obj_name.c_str(), s_phase[j].c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    p_ac_meter->inline_m_param[m_ind].u_phase[j] * gOneUnit - thre * gOneUnit > gTwoUnit;
                            WarnCheck(p_warn_history_inf);
                        }
                    }

                    thre = p_st_way_data->way_threshold.u_harm_percent_up_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.u_harm_percent_up_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电压谐波百分比上限";
                        for(int j = 0; j < 3; j++)
                        {
                            for(int k = 0; k < 30; k++)
                            {
                                st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;
                                strcpy(p_warn_history_inf->start_time, now_time.c_str());
                                strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                                strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                                sprintf(p_warn_history_inf->obj_name,
                                        "%sU%s%d次谐波含量百分比",
                                        obj_name.c_str(), s_phase[j].c_str(), k+2);
                                p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                                p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                                p_warn_history_inf->way_code = way_id;
                                p_warn_history_inf->is_warn =
                                        p_ac_meter->harm_param[m_ind].u_harm_percent[j][k] * gTwoUnit - thre * gOneUnit > gTwoUnit;
                                WarnCheck(p_warn_history_inf);
                            }
                        }
                    }

                    thre = p_st_way_data->way_threshold.i_zero_up_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_zero_up_limit[1]);
                    if(is_use)
                    {
                        warn_type = "零线电流上限";
                        st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;
                        sprintf(p_warn_history_inf->obj_name,
                                "%s零序电流",
                                obj_name.c_str());
                        strcpy(p_warn_history_inf->start_time, now_time.c_str());
                        strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                        strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                        p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                        p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                        p_warn_history_inf->way_code = way_id;
                        p_warn_history_inf->is_warn =
                                p_ac_meter->inline_m_param[m_ind].i_phase[3] * gTwoUnit - thre * gOneUnit > gTwoUnit;
                        WarnCheck(p_warn_history_inf);
                    }

                    thre = p_st_way_data->way_threshold.i_unbalance_up_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_unbalance_up_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流不平衡上限";
                        MU_INT32 i_a = p_ac_meter->inline_m_param[m_ind].i_phase[0] * gTwoUnit;
                        MU_INT32 i_b = p_ac_meter->inline_m_param[m_ind].i_phase[1] * gTwoUnit;
                        MU_INT32 i_c = p_ac_meter->inline_m_param[m_ind].i_phase[2] * gTwoUnit;
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;
                            sprintf(p_warn_history_inf->obj_name,
                                    "%sAB相电流不平衡",
                                    obj_name.c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    (i_a - i_b - thre * gOneUnit > gTwoUnit) ||
                                    (i_b - i_a - thre * gOneUnit > gTwoUnit);
                            WarnCheck(p_warn_history_inf);
                        }

                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;
                            sprintf(p_warn_history_inf->obj_name,
                                    "%sBC相电流不平衡",
                                    obj_name.c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    (i_a - i_c - thre * gOneUnit > gTwoUnit)
                                    ||  (i_c - i_a - thre * gOneUnit > gTwoUnit);
                            WarnCheck(p_warn_history_inf);
                        }

                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;
                            sprintf(p_warn_history_inf->obj_name,
                                    "%sCA相电流不平衡",
                                    obj_name.c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    (i_b - i_c - thre * gOneUnit > gTwoUnit) ||
                                    (i_c - i_b - thre * gOneUnit > gTwoUnit);
                            WarnCheck(p_warn_history_inf);
                        }
                    }

                    thre = p_st_way_data->way_threshold.i_harm_percent_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_harm_percent_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流谐波百分比";
                        for(int j = 0; j < 3; j++)
                        {
                            for(int k = 0; k < 30; k++)
                            {
                                st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                                sprintf(p_warn_history_inf->obj_name,
                                        "%sI%s%d次谐波含量百分比",
                                        obj_name.c_str(), s_phase[j].c_str(), k+2);
                                strcpy(p_warn_history_inf->start_time, now_time.c_str());
                                strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                                strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                                p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                                p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                                p_warn_history_inf->way_code = way_id;
                                p_warn_history_inf->is_warn =
                                        p_ac_meter->harm_param[m_ind].i_harm_percent[j][k] * gTwoUnit - thre * gOneUnit > gTwoUnit;
                                WarnCheck(p_warn_history_inf);
                            }
                        }
                    }

                    thre = p_st_way_data->way_threshold.f_up_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.f_up_limit[1]);
                    if(is_use)
                    {
                        warn_type = "频率上限";
                        st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                        sprintf(p_warn_history_inf->obj_name,
                                "%s频率",
                                obj_name.c_str());
                        strcpy(p_warn_history_inf->start_time, now_time.c_str());
                        strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                        strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                        p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                        p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                        p_warn_history_inf->way_code = way_id;
                        p_warn_history_inf->is_warn =
                                p_ac_meter->inline_m_param[m_ind].frequence * gTwoUnit - thre * gOneUnit > gTwoUnit;
                        WarnCheck(p_warn_history_inf);
                    }

                    thre = p_st_way_data->way_threshold.f_down_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.f_down_limit[1]);
                    if(is_use)
                    {
                        warn_type = "频率下限";
                        st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;
                        sprintf(p_warn_history_inf->obj_name,
                                "%s频率",
                                obj_name.c_str());
                        strcpy(p_warn_history_inf->start_time, now_time.c_str());
                        strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                        strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                        p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                        p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                        p_warn_history_inf->way_code = way_id;
                        p_warn_history_inf->is_warn =
                                p_ac_meter->inline_m_param[m_ind].frequence * gTwoUnit - thre * gOneUnit < gTwoUnit;
                        WarnCheck(p_warn_history_inf);
                    }

                    thre = p_st_way_data->way_threshold.lose_phase_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.lose_phase_limit[1]);
                    if(is_use)
                    {
                        warn_type = "缺相下限";
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                            sprintf(p_warn_history_inf->obj_name,
                                    "%sA相电压缺相",
                                    obj_name.c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    p_ac_meter->inline_m_param[m_ind].u_phase[0] * gOneUnit - thre * gOneUnit < gTwoUnit;
                            WarnCheck(p_warn_history_inf);
                        }
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                            sprintf(p_warn_history_inf->obj_name,
                                    "%sB相电压缺相",
                                    obj_name.c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    p_ac_meter->inline_m_param[m_ind].u_phase[1] * gOneUnit - thre * gOneUnit < gTwoUnit;
                            WarnCheck(p_warn_history_inf);
                        }
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                            sprintf(p_warn_history_inf->obj_name,
                                    "%sC相电压缺相",
                                    obj_name.c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    p_ac_meter->inline_m_param[m_ind].u_phase[2] * gOneUnit - thre * gOneUnit < gTwoUnit;
                            WarnCheck(p_warn_history_inf);
                        }
                    }

                    thre = p_st_way_data->way_threshold.i_down_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_down_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流下限";
                        for(int j = 0; j < 3; j++)
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                            sprintf(p_warn_history_inf->obj_name,
                                    "%s%s相电流",
                                    obj_name.c_str(), s_phase[j].c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    p_ac_meter->inline_m_param[m_ind].i_phase[j] * gTwoUnit - thre * gOneUnit < gTwoUnit;
                            WarnCheck(p_warn_history_inf);
                        }
                    }

                    thre = p_st_way_data->way_threshold.i_up_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_up_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流上限";
                        for(int j = 0; j < 3; j++)
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                            sprintf(p_warn_history_inf->obj_name,
                                    "%s%s相电流",
                                    obj_name.c_str(), s_phase[j].c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    p_ac_meter->inline_m_param[m_ind].i_phase[j] * gTwoUnit - thre * gOneUnit > gTwoUnit;
                            WarnCheck(p_warn_history_inf);
                        }
                    }

                    thre = p_st_way_data->way_threshold.i_down2_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_down2_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流下下限";
                        for(int j = 0; j < 3; j++)
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                            sprintf(p_warn_history_inf->obj_name,
                                    "%s%s相电流",
                                    obj_name.c_str(), s_phase[j].c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    p_ac_meter->inline_m_param[m_ind].i_phase[j] * gTwoUnit - thre * gOneUnit < gTwoUnit;
                            WarnCheck(p_warn_history_inf);
                        }
                    }

                    thre = p_st_way_data->way_threshold.i_up2_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_up2_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流上上限";
                        for(int j = 0; j < 3; j++)
                        {
                            st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                            sprintf(p_warn_history_inf->obj_name,
                                    "%s%s相电流",
                                    obj_name.c_str(), s_phase[j].c_str());
                            strcpy(p_warn_history_inf->start_time, now_time.c_str());
                            strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                            strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                            p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                            p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                            p_warn_history_inf->way_code = way_id;
                            p_warn_history_inf->is_warn =
                                    p_ac_meter->inline_m_param[m_ind].i_phase[j] * gTwoUnit - thre * gOneUnit > gTwoUnit;
                            WarnCheck(p_warn_history_inf);
                        }
                    }

                    //开关住进线
                    int off_set_m = 0;
                    if(way_id == MAIN_A_CODE)
                    {
                        off_set_m = 0;
                    }
                    else if(way_id == MAIN_B_CODE)
                    {
                        off_set_m = 1;
                    }
                    //bit0、bit1表示进线开关量1、2；为1表示开关量输入闭合，为0表示断开
                    is_use = (0 != p_st_way_data->way_threshold.fuse_limit[1]);
                    if(is_use)
                    {
                        switch_state = p_ac_meter->switch_on_ele[m_ind];

                        warn_type = "开关";
                        st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;
                        sprintf(p_warn_history_inf->obj_name,
                                "%s开关",
                                obj_name.c_str());
                        strcpy(p_warn_history_inf->start_time, now_time.c_str());
                        strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                        strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                        p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                        p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                        p_warn_history_inf->way_code = way_id;
                        p_warn_history_inf->is_warn = (MU_INT8)((switch_state >> 0) & 0x01) == 0;
                        WarnCheck(p_warn_history_inf);
                    }
                }
                else if((way_id > MAIN_A_CODE && way_id <= MAIN_A_CODE + SUB_WAY_PARAM_NUM)
                        || (way_id > MAIN_B_CODE && way_id <= MAIN_B_CODE + SUB_WAY_PARAM_NUM))
                {
                    int sub_way_param_ind = 0;
                    int real_data_ind = 0;
                    if(way_id > MAIN_A_CODE && way_id <= MAIN_A_CODE + SUB_WAY_PARAM_NUM)
                    {
                        //支路A
                        sub_way_param_ind = way_id - MAIN_A_CODE - 1;
                        real_data_ind = sub_way_param_ind;
                        obj_name = "支路A的";
                    }
                    else if(way_id > MAIN_B_CODE && way_id <= MAIN_B_CODE + SUB_WAY_PARAM_NUM)
                    {
                        //支路B
                        sub_way_param_ind = way_id - MAIN_B_CODE - 1;
                        real_data_ind = sub_way_param_ind + MAIN_B_OFF_ELECTRIC;
                        obj_name = "支路B的";
                    }

                    thre = p_st_way_data->way_threshold.i_down_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_down_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流下限";
                        st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                        sprintf(p_warn_history_inf->obj_name,
                                "%s第%d路电流",
                                obj_name.c_str(), sub_way_param_ind+1);
                        strcpy(p_warn_history_inf->start_time, now_time.c_str());
                        strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                        strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                        p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                        p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                        p_warn_history_inf->way_code = way_id;
                        p_warn_history_inf->is_warn =
                                p_ac_meter->i_out_line[real_data_ind] * gTwoUnit - thre * gOneUnit < gTwoUnit;
                        WarnCheck(p_warn_history_inf);
                    }

                    thre = p_st_way_data->way_threshold.i_up_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_up_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流上限";

                        st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                        sprintf(p_warn_history_inf->obj_name,
                                "%s第%d路电流",
                                obj_name.c_str(), sub_way_param_ind+1);
                        strcpy(p_warn_history_inf->start_time, now_time.c_str());
                        strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                        strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                        p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                        p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                        p_warn_history_inf->way_code = way_id;
                        p_warn_history_inf->is_warn =
                                p_ac_meter->i_out_line[real_data_ind] * gTwoUnit - thre * gOneUnit > gTwoUnit;
                        WarnCheck(p_warn_history_inf);
                    }

                    thre = p_st_way_data->way_threshold.i_down2_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_down2_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流下下限";

                        st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                        sprintf(p_warn_history_inf->obj_name,
                                "%s第%d路电流",
                                obj_name.c_str(), sub_way_param_ind+1);
                        strcpy(p_warn_history_inf->start_time, now_time.c_str());
                        strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                        strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                        p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                        p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                        p_warn_history_inf->way_code = way_id;
                        p_warn_history_inf->is_warn =
                                p_ac_meter->i_out_line[real_data_ind] * gTwoUnit - thre * gOneUnit < gTwoUnit;
                        WarnCheck(p_warn_history_inf);

                    }

                    thre = p_st_way_data->way_threshold.i_up2_limit[0];
                    is_use = (0 != p_st_way_data->way_threshold.i_up2_limit[1]);
                    if(is_use)
                    {
                        warn_type = "电流上上限";

                        st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                        sprintf(p_warn_history_inf->obj_name,
                                "%s第%d路电流",
                                obj_name.c_str(), sub_way_param_ind+1);
                        strcpy(p_warn_history_inf->start_time, now_time.c_str());
                        strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                        strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                        p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                        p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                        p_warn_history_inf->way_code = way_id;
                        p_warn_history_inf->is_warn =
                                p_ac_meter->i_out_line[real_data_ind] * gTwoUnit - thre * gOneUnit > gTwoUnit;
                        WarnCheck(p_warn_history_inf);

                    }
                }
            }
        }
    }
    else if(dev_type == E_DEV_SWITCH)
    {
        for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)
        {
            bool is_way_use = IsWayUsedByMask(way_ind,
                                              p_dev_inf->way_inf.way_mask);
            const st_switch_meter* p_switch_meter = &p_dev_inf->dev_measure.switch_meter;
            if(is_way_use)
            {
                const st_way_data* p_st_way_data = &p_dev_inf->way_inf.way_data[way_ind];
                int way_id = p_st_way_data->way_id;
                if(way_id == MAIN_A_CODE || way_id == MAIN_B_CODE)
                {
                    //在交流表中
                }
                else if((way_id > MAIN_A_CODE && way_id <= MAIN_A_CODE + SUB_WAY_PARAM_NUM)
                        || (way_id > MAIN_B_CODE && way_id <= MAIN_B_CODE + SUB_WAY_PARAM_NUM))
                {
                    int sub_way_param_ind = 0;
                    if(way_id > MAIN_A_CODE && way_id <= MAIN_A_CODE + SUB_WAY_PARAM_NUM)
                    {
                        //支路A
                        sub_way_param_ind = way_id - MAIN_A_CODE - 1;
                        obj_name = "支路A的";
                    }
                    else if(way_id > MAIN_B_CODE && way_id <= MAIN_B_CODE + SUB_WAY_PARAM_NUM)
                    {
                        //支路B
                        sub_way_param_ind = way_id - MAIN_B_CODE - 1;
                        obj_name = "支路B的";
                    }
                    //bit0~bit15表示出线开关量 1~16
                    int off_sub = sub_way_param_ind;
                    MU_INT64 switch_state = p_switch_meter->switch_state;
                    is_use = (0 != p_st_way_data->way_threshold.fuse_limit[1]);
                    if(is_use)
                    {
                        warn_type = "开关";

                        st_warn_history_inf* p_warn_history_inf = new st_warn_history_inf;

                        sprintf(p_warn_history_inf->obj_name,
                                "%s第%d路开关",
                                obj_name.c_str(), sub_way_param_ind+1);
                        strcpy(p_warn_history_inf->start_time, now_time.c_str());
                        strcpy(p_warn_history_inf->warn_type, warn_type.c_str());
                        strcpy(p_warn_history_inf->stop_time, now_time.c_str());
                        p_warn_history_inf->dev_addr = p_dev_inf->dev_addr.id;
                        p_warn_history_inf->dev_type = p_dev_inf->dev_type;
                        p_warn_history_inf->way_code = way_id;
                        p_warn_history_inf->is_warn = (MU_INT8)((switch_state >> off_sub) & 0x01) == 0;
                        WarnCheck(p_warn_history_inf);

                    }
                }
            }
        }
    }

    SetList();
}

void CWarnCurrentDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;
}

void CWarnCurrentDialog::InitUi()
{
    //bt
    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);
    ui->pushButton_pre_list->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_list->setStyleSheet(gResArrowUp);
    ui->pushButton_next_list->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_list->setStyleSheet(gResArrowDown);

    ui->widget_warn->setStyleSheet(gResBell);

    //table
    QTableWidget* table_arr[1];
    table_arr[0] = ui->tableWidget;

    for(int table_ind = 0; table_ind < 1; table_ind++)
    {
        QTableWidget* table = table_arr[table_ind];
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        int rows = rowMax;
        int colums = s_colum;

        table->setRowCount(rows);
        table->setColumnCount(colums);

        //设置左上角文字
        QLabel*label = new QLabel(table);
        label->setText("序号");
        label->move(5,5);
        label->installEventFilter(this);

        //设置表格行标题
        QStringList header_labels;
        header_labels.append("发生时间");
        header_labels.append("对象名");
        header_labels.append("报警类型");
        header_labels.append("报警事件");
        header_labels.append("报警描述");
        table->setHorizontalHeaderLabels(header_labels);

        //设置表格行标题的对齐方式
        table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

        //设置每一行的高度
        for(int i = 0; i < rows; i++)
        {
            //   table->setRowHeight(i, 22);
        }

        for(int i = 0; i < colums; i++)
        {
            table->setColumnWidth(i, 200);
        }

        //自动调整最后一列的宽度使它和表格的右边界对齐
        table->horizontalHeader()->setStretchLastSection(true);

        //设置表格的选择方式
        table->setSelectionBehavior(QAbstractItemView::SelectItems);
        table->setSelectionMode(QAbstractItemView::NoSelection);

        //设置编辑方式
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //设置宽度固定
        table->verticalHeader()->setResizeMode(QHeaderView::Fixed);
        table->horizontalHeader()->setResizeMode(QHeaderView::Fixed);

        //设置行表题
        QStringList row_labels;
        for(int i = 0; i < rows; i++)
        {
            QString str;
            str = QString::number(i + 1) + QString("    ");

            row_labels.append(str);
        }
        table->setVerticalHeaderLabels(row_labels);

        //设置表格的内容
        for (int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < colums; j++)
            {
                QTableWidgetItem* item = new QTableWidgetItem;
                QString txt = QString::number(10 * i + j);
                item->setText("");
                table->setItem(i, j, item);
            }
        }
    }
}

void CWarnCurrentDialog::SetList()
{
    QTableWidget* table = ui->tableWidget;
    //设置表格的内容
    QString txt;
    QTableWidgetItem* item = NULL;
    MU_INT32  warn_num = deqStWarnCurrentInf.size();

    for (MU_INT32 i = 0; i < rowMax; ++i)
    {
        for (MU_INT32 j = 0; j < s_colum; ++j)
        {
            txt = "";
            item = table->item(i, j);
            item->setText(txt);
        }
    }

    for (MU_INT32 i = 0; i < warn_num; ++i)
    {
        st_warn_history_inf* p_warn_current_inf = deqStWarnCurrentInf[i];
        int colum_ind = -1;
        int row_ind = i;

        colum_ind++;
        txt = str_2_qstr(p_warn_current_inf->start_time);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = str_2_qstr(p_warn_current_inf->obj_name);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);

        colum_ind++;
        txt = str_2_qstr(p_warn_current_inf->warn_type);
        item = table->item(row_ind, colum_ind);
        item->setText(txt);
    }
}

void CWarnCurrentDialog::WarnCheck(st_warn_history_inf* p_warn_history_inf)
{
    if(p_warn_history_inf->is_warn)
    {
        AddWarn(p_warn_history_inf);
    }
    else
    {
        DelWarn(p_warn_history_inf);
    }
}

void CWarnCurrentDialog::AddWarn(st_warn_history_inf *p_warn_history_inf)
{
    //如果当前界面为初始化界面，则跳转到当期告警界面
    if(isAuto2WarnDialog)
    {
        isAuto2WarnDialog = false;
        emit signal_change_child_dialog(E_WARN_CURRENT_DIALOG);
    }

    bool is_exist = false;
    MU_INT32 cont_size = 0;

    //增加告警事件
    is_exist = false;
    cont_size = deqStWarnCurrentInf.size();
    for(MU_INT32 i = 0; i < cont_size; i++)
    {
        st_warn_history_inf* warn_his_old = deqStWarnCurrentInf[i];
        if(warn_his_old->dev_type == p_warn_history_inf->dev_type
                && warn_his_old->way_code == p_warn_history_inf->way_code
                && (0 == strcmp(warn_his_old->obj_name, p_warn_history_inf->obj_name))
                && (0 == strcmp(warn_his_old->warn_type, p_warn_history_inf->warn_type))
                )
        {
            is_exist = true;
            break;
        }
    }
    if(!is_exist)
    {
        //not exist, add
        if(cont_size >= rowMax)
        {
            DEQ_ST_WARN_HISTORY_INF::iterator iter = deqStWarnCurrentInf.end();
            st_warn_history_inf* pop_ele = *iter;
            delete pop_ele;
            deqStWarnCurrentInf.erase(iter);
        }
        deqStWarnCurrentInf.push_front(p_warn_history_inf);

        //写入电表
        //bit0为继电器的输出状态；为1表示继电器闭合，为0表示断开
        //新电表告警，写继电器闭合0x01
        {
            st_write_relay_param* write_relay_param = new st_write_relay_param;
            memset(write_relay_param, 0, sizeof(*write_relay_param));
            MU_INT8 dev_addr = p_warn_history_inf->dev_addr;
            dev_addr = DEVID_ELECTRIC_M;
//            qDebug("%s way_code=%d, dev_addr=%d", __FUNCTION__, p_warn_history_inf->way_code, dev_addr);
            write_relay_param->dev_addr = dev_addr;
            write_relay_param->relay_val = 0x01;
            write_relay_param->reg_addr = REG_RELAY_M_LIGHT;
            emit signal_write_relay(write_relay_param);
        }

        {
            st_write_relay_param* write_relay_param = new st_write_relay_param;
            memset(write_relay_param, 0, sizeof(*write_relay_param));
            MU_INT8 dev_addr = p_warn_history_inf->dev_addr;
            dev_addr = DEVID_ELECTRIC_M;
//            qDebug("%s way_code=%d, dev_addr=%d", __FUNCTION__, p_warn_history_inf->way_code, dev_addr);
            write_relay_param->dev_addr = dev_addr;
            write_relay_param->relay_val = 0x01;
            write_relay_param->reg_addr = REG_RELAY_M_SOUND;
            emit signal_write_relay(write_relay_param);
        }
    }
    else
    {
        delete p_warn_history_inf;
        return;
    }
}

void CWarnCurrentDialog::DelWarn(st_warn_history_inf *p_warn_history_inf)
{
    bool bres = false;
    bool is_exist = false;
    for (DEQ_ST_WARN_HISTORY_INF::iterator iter = deqStWarnCurrentInf.begin();
         iter != deqStWarnCurrentInf.end();)
    {
        st_warn_history_inf* warn_his_old = *iter;
        if(warn_his_old->dev_type == p_warn_history_inf->dev_type
                && warn_his_old->way_code == p_warn_history_inf->way_code
                && (0 == strcmp(warn_his_old->obj_name, p_warn_history_inf->obj_name))
                && (0 == strcmp(warn_his_old->warn_type, p_warn_history_inf->warn_type))
                )
        {
            strcpy(warn_his_old->stop_time, p_warn_history_inf->stop_time);
            //槽函数释放,因为异步任务，没有返回信号
            emit signal_write_warn_history(warn_his_old);
            iter = deqStWarnCurrentInf.erase(iter);
            bres = true;
            break;
        }
        else
        {
            iter++;
        }
    }

    //该地址电表告警全部消失，写继电器断开0x00
    if(!bres)
    {
        goto LEAVE;
    }

    for (DEQ_ST_WARN_HISTORY_INF::iterator iter = deqStWarnCurrentInf.begin();
         iter != deqStWarnCurrentInf.end(); iter++)
    {
        if(1)
        {
            is_exist = true;
            break;
        }
    }
    if(!is_exist)
    {
        //无告警，写继电器闭合0x01
        {
            st_write_relay_param* write_relay_param = new st_write_relay_param;
            memset(write_relay_param, 0, sizeof(*write_relay_param));
            MU_INT8 dev_addr = p_warn_history_inf->dev_addr;
            dev_addr = DEVID_ELECTRIC_M;
//            qDebug("%s way_code=%d", __FUNCTION__, p_warn_history_inf->way_code);
            write_relay_param->dev_addr = dev_addr;
            write_relay_param->relay_val = 0x00;
            write_relay_param->reg_addr = REG_RELAY_M_LIGHT;
            emit signal_write_relay(write_relay_param);
        }

        {
            st_write_relay_param* write_relay_param = new st_write_relay_param;
            memset(write_relay_param, 0, sizeof(*write_relay_param));
            MU_INT8 dev_addr = p_warn_history_inf->dev_addr;
            dev_addr = DEVID_ELECTRIC_M;
//            qDebug("%s way_code=%d", __FUNCTION__, p_warn_history_inf->way_code);
            write_relay_param->dev_addr = dev_addr;
            write_relay_param->relay_val = 0x00;
            write_relay_param->reg_addr = REG_RELAY_M_SOUND;
            emit signal_write_relay(write_relay_param);
        }
    }

LEAVE:
    delete p_warn_history_inf;
    return;
}
