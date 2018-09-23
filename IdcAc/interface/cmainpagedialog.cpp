#include "cmainpagedialog.h"
#include "ui_cmainpagedialog.h"

#include <QPainter>
#include "CommonFunction.h"
#include <math.h>
#define PI (3.1415926)
#include "xmlAnalyze.h"

static QString s_res_button_red = "color: black;border-image: url(:/images/button_red.png)";
static QString s_res_button_green = "color: black;border-image: url(:/images/button_green.png)";
CMainPageDialog::CMainPageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMainPageDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    pOuterDev = NULL;

    for(int i = 0; i < E_METER_WIGDET_NUM; i++)
    {
        QLineF linef;
        int x1 = gWidthChild*(15+(i%3)*23)/100;
        int y1 = gHeightChild*(22+(i/3)*30)/100;
        int x2 = x1 - gWidthChild*(5)/100;
        int y2 = y1;
        linef.setPoints(QPoint(x1, y1),
                        QPoint(x2, y2));
        lineFMeter[i].meter_range = 1;
        lineFMeter[i].meter_val = 0;
        lineFMeter[i].ui_ind = i;
        lineFMeter[i].linef = linef;
    }

    InitUi();

    connect(ui->pushButton_main_way_param, SIGNAL(clicked()),
            this, SLOT(slot_bt_main_way_param_clicked()));
    connect(ui->pushButton_harm_param, SIGNAL(clicked()),
            this, SLOT(slot_bt_harm_param_clicked()));
    connect(ui->pushButton_sub_way_param, SIGNAL(clicked()),
            this, SLOT(slot_bt_sub_way_param_clicked()));
    connect(ui->pushButton_power_check, SIGNAL(clicked()),
            this, SLOT(slot_bt_power_check_clicked()));
    connect(ui->pushButton_switch_state, SIGNAL(clicked()),
            this, SLOT(slot_bt_switch_state_clicked()));
    connect(ui->pushButton_load_percent, SIGNAL(clicked()),
            this, SLOT(slot_bt_load_percent_clicked()));
    connect(ui->pushButton_warn_inf, SIGNAL(clicked()),
            this, SLOT(slot_bt_warn_inf_clicked()));
    connect(ui->pushButton_param_set, SIGNAL(clicked()),
            this, SLOT(slot_bt_param_set_clicked()));
    connect(ui->pushButton_clear_set, SIGNAL(clicked()),
            this, SLOT(slot_bt_clear_clicked()));

}

CMainPageDialog::~CMainPageDialog()
{
    delete ui;
}

void CMainPageDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::green));//设置画笔形式

//    for(int i = 0; i < E_METER_WIGDET_NUM; i++)
//    {
//        painter.drawLine(lineFMeter[i].linef);//画直线
//    }
    //    return;

    for(int i = 0; i < E_METER_WIGDET_NUM; i++)
    {
        st_line_meter_inf* p_st_line_meter_inf = &lineFMeter[i];
        int ind = p_st_line_meter_inf->ui_ind;
        int x1 = gWidthChild*(15+(ind%3)*23)/100;
        int y1 = gHeightChild*(22+(ind/3)*30)/100;
        int x2 = x1 - gWidthChild*(5)/100;
        int y2 = y1;
        int line_len = x1 - x2;

        qreal left = x2;
        qreal top = y2 - line_len;
        qreal width = 2 * line_len;
        qreal height = width;
        painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));//设置画刷形式
        QRectF rect_top(left, top, width, height);
        // 扇形起始角度
        double val = p_st_line_meter_inf->meter_val;
        int range = p_st_line_meter_inf->meter_range;
        int start_angle = 16 * (180 - 180*val/range);
        // 扇形覆盖范围（120度的）
        int span_angle = 180*16 - start_angle;
        painter.drawPie(rect_top, start_angle, span_angle);
    }
}

void CMainPageDialog::slot_bt_main_way_param_clicked()
{
    emit signal_change_child_dialog(E_ELE_MAIN_WAY_DIALOG_A);
}

void CMainPageDialog::slot_bt_harm_param_clicked()
{
    emit signal_change_child_dialog(E_HARM_PERCENT_DIALOG);
}

void CMainPageDialog::slot_bt_sub_way_param_clicked()
{
    emit signal_change_child_dialog(E_ELE_SUB_WAY_DIALOG);
}

void CMainPageDialog::slot_bt_power_check_clicked()
{
    emit signal_change_child_dialog(E_POWER_CHECK_DIALOG_A);
}

void CMainPageDialog::slot_bt_switch_state_clicked()
{
    emit signal_change_child_dialog(E_SWITCH_STATE_DIALOG_A);
}

void CMainPageDialog::slot_bt_load_percent_clicked()
{
    emit signal_change_child_dialog(E_LOAD_PERCENT_DIALOG_A);
}

void CMainPageDialog::slot_bt_warn_inf_clicked()
{
    emit signal_change_child_dialog(E_WARN_CURRENT_DIALOG);
}

void CMainPageDialog::slot_bt_param_set_clicked()
{
    CLoginDialog* p_CLoginDialog = new CLoginDialog;
    DEQ_ST_USER_INF* p_deqStUserInf = gXmlAnalyze.GetUserInf();
    p_CLoginDialog->SetDeqUserInf(p_deqStUserInf);
    int ret = LogIn(this, p_CLoginDialog);
    if(ret != 0)
    {
        goto LEAVE;
    }
    else
    {
        emit signal_change_child_dialog(E_WARN_SET_DIALOG_A);
    }
LEAVE:
    delete p_CLoginDialog;
}

void CMainPageDialog::slot_bt_clear_clicked()
{
    CLoginDialog* p_CLoginDialog = new CLoginDialog;
    DEQ_ST_USER_INF* p_deqStUserInf = gXmlAnalyze.GetUserInf();
    p_CLoginDialog->SetDeqUserInf(p_deqStUserInf);
    int ret = LogIn(this, p_CLoginDialog);
    if(ret != 0)
    {
        goto LEAVE;
    }
    else
    {
        st_user_inf* p_st_user_inf = p_CLoginDialog->GetCurrentUserInf();
        emit signal_current_user(*p_st_user_inf);
        emit signal_change_child_dialog(E_ClEAR_DIALOG);
    }
LEAVE:
    delete p_CLoginDialog;
}

void CMainPageDialog::slot_communicate_state_update(const st_dev_inf *p_dev_inf)
{
    int dev_type = p_dev_inf->dev_type;
    if(dev_type != E_DEV_ELECTRIC && dev_type != E_DEV_SWITCH)
    {
        return;
    }

    int way_code = -1;
    for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)
    {
        bool is_way_use = IsWayUsedByMask(way_ind,
                                          p_dev_inf->way_inf.way_mask);
        if(is_way_use)
        {
            const st_way_data* p_st_way_data = &p_dev_inf->way_inf.way_data[way_ind];
            int way_id = p_st_way_data->way_id;
            if(way_id >= MAIN_A_CODE && way_id <= MAIN_A_CODE + SUB_WAY_PARAM_NUM)
            {
                way_code = MAIN_A_CODE;
                break;
            }
            else if(way_id >= MAIN_B_CODE && way_id <= MAIN_B_CODE + SUB_WAY_PARAM_NUM)
            {
                way_code = MAIN_B_CODE;
                break;
            }
        }
    }
    if(way_code < 0)
    {
        return;
    }

    if(dev_type == E_DEV_SWITCH)
    {
        if(p_dev_inf->comm_state)
        {
            if(way_code == MAIN_A_CODE)
            {
                ui->pushButton_switch_unit_a->setStyleSheet(s_res_button_green);
            }
            else
            {
                ui->pushButton_switch_unit_b->setStyleSheet(s_res_button_green);
            }
        }
        else
        {
            if(way_code == MAIN_A_CODE)
            {
                ui->pushButton_switch_unit_a->setStyleSheet(s_res_button_red);
            }
            else
            {
                ui->pushButton_switch_unit_b->setStyleSheet(s_res_button_red);
            }
        }
    }

    if(dev_type == E_DEV_ELECTRIC)
    {
        if(p_dev_inf->comm_state)
        {
            if(way_code == MAIN_A_CODE)
            {
                ui->pushButton_monitor_host_a->setStyleSheet(s_res_button_green);
                ui->pushButton_monitor_host_b->setStyleSheet(s_res_button_green);
            }
        }
        else
        {
            if(way_code == MAIN_A_CODE)
            {
                ui->pushButton_monitor_host_a->setStyleSheet(s_res_button_red);
                ui->pushButton_monitor_host_b->setStyleSheet(s_res_button_red);
            }
        }
    }
}

void CMainPageDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;
}

void CMainPageDialog::SetUi(const st_dev_inf* p_dev_inf)
{
    int way_code = MAIN_A_CODE;

    int dev_type = p_dev_inf->dev_type;
    if(dev_type != E_DEV_ELECTRIC)
    {
        return;
    }

    for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)
    {
        bool is_way_use = IsWayUsedByMask(way_ind,
                                          p_dev_inf->way_inf.way_mask);
        if(is_way_use)
        {
            const st_way_data* p_st_way_data = &p_dev_inf->way_inf.way_data[way_ind];
            int way_id = p_st_way_data->way_id;
            //0主路,1-24支路
            if(way_id == way_code)
            {
                const st_ac_meter* p_ac_meter = &p_dev_inf->dev_measure.ac_meter;
                const st_inline_m_param* p_st_inline_m_param = &p_ac_meter->inline_m_param[0];
                QString str_val;
                str_val = Uint2DoubleQStr(p_st_inline_m_param->u_phase[0] * 10, gTwoUnit);
                ui->lineEdit_u_a->setText(str_val);
                ui->lineEdit_u_a_meter->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->u_phase[1] * 10, gTwoUnit);
                ui->lineEdit_u_b->setText(str_val);
                ui->lineEdit_u_b_meter->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->u_phase[2] * 10, gTwoUnit);
                ui->lineEdit_u_c->setText(str_val);
                ui->lineEdit_u_c_meter->setText(str_val);

                str_val = Uint2DoubleQStr(p_st_inline_m_param->i_phase[0] * 10, gThreeUnit);
                ui->lineEdit_i_a->setText(str_val);
                ui->lineEdit_i_a_meter->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->i_phase[1] * 10, gThreeUnit);
                ui->lineEdit_i_b->setText(str_val);
                ui->lineEdit_i_b_meter->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->i_phase[2] * 10, gThreeUnit);
                ui->lineEdit_i_c->setText(str_val);
                ui->lineEdit_i_c_meter->setText(str_val);

                str_val = Uint2DoubleQStr(p_st_inline_m_param->energy_total[0] * 10, gThreeUnit);
                ui->lineEdit_energy->setText(str_val);

                for(int i = 0; i < E_METER_WIGDET_NUM; i++)
                {
                    if(i < 3)
                    {
                        SetLinef(i, p_st_inline_m_param->u_phase[i % 3] * gOneUnit);
                    }
                    else
                    {
                        SetLinef(i, p_st_inline_m_param->i_phase[i % 3] * gTwoUnit);
                    }
                }

            }
        }
    }
    repaint();
}

void CMainPageDialog::InitUi()
{
    QFont ft;
    ft.setPointSize(10);
    ui->textEdit_phase_u->setFont(ft);
    ui->textEdit_phase_i->setFont(ft);
    ui->textEdit_phase_state->setFont(ft);

    ui->textEdit_phase_u->append(" 相");
    ui->textEdit_phase_u->append(" 电");
    ui->textEdit_phase_u->append(" 压");
    ui->textEdit_phase_u->append("(V)");

    ui->textEdit_phase_i->append(" 相");
    ui->textEdit_phase_i->append(" 电");
    ui->textEdit_phase_i->append(" 流");
    ui->textEdit_phase_i->append("(A)");

    ui->textEdit_phase_state->append("通");
    ui->textEdit_phase_state->append("信");
    ui->textEdit_phase_state->append("状");
    ui->textEdit_phase_state->append("态");

    ui->widget_u_a->setStyleSheet(gResVoltage);
    ui->widget_u_b->setStyleSheet(gResVoltage);
    ui->widget_u_c->setStyleSheet(gResVoltage);
    ui->widget_i_a->setStyleSheet(gResCurrent);
    ui->widget_i_b->setStyleSheet(gResCurrent);
    ui->widget_i_c->setStyleSheet(gResCurrent);

    ui->widget_phase_u->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_phase_i->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_energy->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_state->setStyleSheet(".QWidget{border:1px solid white}");

    ui->pushButton_monitor_host_a->setStyleSheet(s_res_button_red);
    ui->pushButton_switch_unit_a->setStyleSheet(s_res_button_red);
    ui->pushButton_monitor_host_b->setStyleSheet(s_res_button_red);
    ui->pushButton_switch_unit_b->setStyleSheet(s_res_button_red);
    ui->pushButton_com_nor->setStyleSheet(gResButtonGreen);
    ui->pushButton_com_unnor->setStyleSheet(gResButtonRed);
    ui->label_com_nor->setStyleSheet("color:white");
    ui->label_com_unnor->setStyleSheet("color:white");

    ui->pushButton_main_way_param->setStyleSheet(gResButtonBlue);
    ui->pushButton_harm_param->setStyleSheet(gResButtonBlue);
    ui->pushButton_sub_way_param->setStyleSheet(gResButtonBlue);
    ui->pushButton_power_check->setStyleSheet(gResButtonBlue);
    ui->pushButton_switch_state->setStyleSheet(gResButtonBlue);
    ui->pushButton_load_percent->setStyleSheet(gResButtonBlue);
    ui->pushButton_warn_inf->setStyleSheet(gResButtonBlue);
    ui->pushButton_param_set->setStyleSheet(gResButtonBlue);
    ui->pushButton_clear_set->setStyleSheet(gResButtonBlue);


    ui->label->setStyleSheet("color:white");
    ui->label_2->setStyleSheet("color:white");
    ui->label_3->setStyleSheet("color:white");
    ui->label_4->setStyleSheet("color:white");
    ui->label_5->setStyleSheet("color:white");
    ui->label_6->setStyleSheet("color:white");
    ui->label_19->setStyleSheet("color:white");
    ui->label_8->setStyleSheet("color:white");
    ui->label_9->setStyleSheet("color:white");
    ui->label_10->setStyleSheet("color:white");
    ui->label_11->setStyleSheet("color:white");
    ui->label_12->setStyleSheet("color:white");
    ui->label_13->setStyleSheet("color:white");
    ui->label_14->setStyleSheet("color:white");
    ui->label_15->setStyleSheet("color:white");
    ui->label_16->setStyleSheet("color:white");
    ui->label_17->setStyleSheet("color:white");
    ui->label_18->setStyleSheet("color:white");


    QString str_val;
    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_u_a->setText(str_val);
    ui->lineEdit_u_a_meter->setText(str_val);
    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_u_b->setText(str_val);
    ui->lineEdit_u_b_meter->setText(str_val);
    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_u_c->setText(str_val);
    ui->lineEdit_u_c_meter->setText(str_val);

    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_i_a->setText(str_val);
    ui->lineEdit_i_a_meter->setText(str_val);
    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_i_b->setText(str_val);
    ui->lineEdit_i_b_meter->setText(str_val);
    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_i_c->setText(str_val);
    ui->lineEdit_i_c_meter->setText(str_val);

    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_energy->setText(str_val);

    ui->lineEdit_13->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_energy->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_a->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_a_meter->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_b->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_b_meter->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_c->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_c_meter->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_a->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_a_meter->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_b->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_b_meter->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_c->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_c_meter->setFocusPolicy(Qt::NoFocus);
    ui->textEdit_phase_i->setFocusPolicy(Qt::NoFocus);
    ui->textEdit_phase_state->setFocusPolicy(Qt::NoFocus);
    ui->textEdit_phase_u->setFocusPolicy(Qt::NoFocus);
}

void CMainPageDialog::SetLinef(int ind, double val)
{
    int range = 0;
    if(ind < 3)
    {
        range = 300;
    }
    else
    {
        range = 300;
    }


    QLineF linef;
    int x1 = gWidthChild*(15+(ind%3)*23)/100;
    int y1 = gHeightChild*(22+(ind/3)*30)/100;
    //todo change x2,y2
    int x2 = x1 - gWidthChild*(5)/100;
    int y2 = y1;

    int line_len = x1 - x2;
    double radian = PI * val / range;
    double off_x = line_len * cos(radian);
    double off_y = line_len * sin(radian);
    x2 = x1 - off_x;
    y2 = y1 - off_y;

    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    lineFMeter[ind].linef = linef;
    lineFMeter[ind].meter_range = range;
    lineFMeter[ind].meter_val = val;
    lineFMeter[ind].ui_ind= ind;
}

