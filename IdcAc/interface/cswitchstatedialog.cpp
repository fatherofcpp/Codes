#include "cswitchstatedialog.h"
#include "ui_cswitchstatedialog.h"
#include <QPainter>
#include "CommonFunction.h"

CSwitchStateDialog::CSwitchStateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSwitchStateDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    pOuterDev = NULL;

    InitUi();

    connect(ui->pushButton_pre_page, SIGNAL(clicked()),
            this, SLOT(slot_pre_page_clicked()));
    connect(ui->pushButton_next_page, SIGNAL(clicked()),
            this, SLOT(slot_next_page_clicked()));
    connect(ui->pushButton_return_back, SIGNAL(clicked()),
            this, SLOT(slot_to_main_page_clicked()));

    for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
    {
        lightBt[i]->SetCode(i);
    }
}

CSwitchStateDialog::~CSwitchStateDialog()
{
    delete ui;
}

void CSwitchStateDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::green));//设置画笔形式

    QLineF linef;
    int x1;
    int y1;
    int x2;
    int y2;

    x1 = ui->widget_line_1->geometry().left();
    y1 = ui->widget_line_1->geometry().top();
    x2 = ui->widget_line_1->geometry().right();
    y2 = y1;
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_2->geometry().left();
    y1 = ui->widget_line_2->geometry().top();
    x2 = ui->widget_line_2->geometry().right();
    y2 = y1;
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_3->geometry().left();
    y1 = ui->widget_line_3->geometry().top();
    x2 = ui->widget_line_3->geometry().right();
    y2 = y1;
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_4->geometry().left();
    y1 = ui->widget_line_4->geometry().top();
    x2 = ui->widget_line_4->geometry().right();
    y2 = y1;
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_5->geometry().left();
    y1 = ui->widget_line_5->geometry().top();
    x2 = ui->widget_line_5->geometry().right();
    y2 = y1;
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_left->geometry().right();
    y1 = ui->widget_left->geometry().top();
    x2 = x1;
    y2 = ui->widget_left->geometry().bottom();
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线
}

void CSwitchStateDialog::slot_to_main_page_clicked()
{
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CSwitchStateDialog::slot_pre_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_SWITCH_STATE_DIALOG_A)
    {
        ind = dialogInd;
    }
    else if(dialogInd == E_SWITCH_STATE_DIALOG_B)
    {
        ind = E_SWITCH_STATE_DIALOG_A;
    }
    emit signal_change_child_dialog(ind);
}

void CSwitchStateDialog::slot_next_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_SWITCH_STATE_DIALOG_A)
    {
        ind = E_SWITCH_STATE_DIALOG_B;
    }
    else if(dialogInd == E_SWITCH_STATE_DIALOG_B)
    {
        ind = dialogInd;
    }
    emit signal_change_child_dialog(ind);
}

void CSwitchStateDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;

    if(dialogInd == E_SWITCH_STATE_DIALOG_B)
    {
        ui->label_main->setText("B主路");
        for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
        {
            QString str = QString("B") + QString::number(i + 1);
            labelW[i]->setText(str);
        }
    }
}

void CSwitchStateDialog::SetUi(const st_dev_inf* p_dev_inf)
{
    int way_code = -1;
    int m_ind = -1;
    switch(dialogInd)
    {
    case E_SWITCH_STATE_DIALOG_A:
        m_ind = 0;
        way_code = MAIN_A_CODE;
        break;
    case E_SWITCH_STATE_DIALOG_B:
        m_ind = 1;
        way_code = MAIN_B_CODE;
        break;
    default:
        break;
    }
    if(way_code < 0)
    {
        return;
    }

    int dev_type = p_dev_inf->dev_type;
    if(dev_type != E_DEV_SWITCH && dev_type != E_DEV_ELECTRIC)
    {
        return;
    }

    if(dev_type == E_DEV_SWITCH)
    {
        for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)
        {
            bool is_way_use = IsWayUsedByMask(way_ind,
                                              p_dev_inf->way_inf.way_mask);
            if(is_way_use)
            {
                const st_way_data* p_st_way_data = &p_dev_inf->way_inf.way_data[way_ind];
                const st_switch_meter* p_switch_meter = &p_dev_inf->dev_measure.switch_meter;
                int way_id = p_st_way_data->way_id;
                if(way_id > way_code && way_id <= way_code + SUB_WAY_PARAM_NUM)
                {
                    //支路
                    int sub_way_param_ind = way_id - way_code - 1;
                    int off_sub = sub_way_param_ind;
                    MU_INT64 switch_state = p_switch_meter->switch_state;
                    if(((MU_INT8)(switch_state >> off_sub) & 0x01) == 0)
                    {
                        lightBt[sub_way_param_ind]->setStyleSheet(gResSwitchOff);
                        lightBt[sub_way_param_ind]->SetOnOff(false);
                    }
                    else
                    {
                        lightBt[sub_way_param_ind]->setStyleSheet(gResSwitchOn);
                        lightBt[sub_way_param_ind]->SetOnOff(true);
                    }
                }
            }
        }
    }
    else if(dev_type == E_DEV_ELECTRIC)
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
                if(way_id == way_code)
                {
                    //主路
                    //bit0、bit1表示进线开关量1、2；为1表示开关量输入闭合，为0表示断开
                    MU_INT16 switch_state = p_ac_meter->switch_on_ele[m_ind];
                    if((MU_INT8)(switch_state & 0x01) == 0)
                    {
                        ui->pushButton_s_m->setStyleSheet(gResSwitchOff);
                        ui->pushButton_s_m->SetOnOff(false);
                    }
                    else
                    {
                        ui->pushButton_s_m->setStyleSheet(gResSwitchOn);
                        ui->pushButton_s_m->SetOnOff(true);
                    }
                }
            }
        }
    }
    repaint();
}

void CSwitchStateDialog::InitUi()
{
    int ind = 0;

    {
        ind = 0;
        lightBt[ind++] = ui->pushButton_s_w_1;
        lightBt[ind++] = ui->pushButton_s_w_2;
        lightBt[ind++] = ui->pushButton_s_w_3;
        lightBt[ind++] = ui->pushButton_s_w_4;
        lightBt[ind++] = ui->pushButton_s_w_5;
        lightBt[ind++] = ui->pushButton_s_w_6;
        lightBt[ind++] = ui->pushButton_s_w_7;
        lightBt[ind++] = ui->pushButton_s_w_8;
        lightBt[ind++] = ui->pushButton_s_w_9;
        lightBt[ind++] = ui->pushButton_s_w_10;
        lightBt[ind++] = ui->pushButton_s_w_11;
        lightBt[ind++] = ui->pushButton_s_w_12;
        lightBt[ind++] = ui->pushButton_s_w_13;
        lightBt[ind++] = ui->pushButton_s_w_14;
        lightBt[ind++] = ui->pushButton_s_w_15;
        lightBt[ind++] = ui->pushButton_s_w_16;
        lightBt[ind++] = ui->pushButton_s_w_17;
        lightBt[ind++] = ui->pushButton_s_w_18;
        lightBt[ind++] = ui->pushButton_s_w_19;
        lightBt[ind++] = ui->pushButton_s_w_20;
        lightBt[ind++] = ui->pushButton_s_w_21;
        lightBt[ind++] = ui->pushButton_s_w_22;
        lightBt[ind++] = ui->pushButton_s_w_23;
        lightBt[ind++] = ui->pushButton_s_w_24;
        lightBt[ind++] = ui->pushButton_s_w_25;
        lightBt[ind++] = ui->pushButton_s_w_26;
        lightBt[ind++] = ui->pushButton_s_w_27;
        lightBt[ind++] = ui->pushButton_s_w_28;
        lightBt[ind++] = ui->pushButton_s_w_29;
        lightBt[ind++] = ui->pushButton_s_w_30;

        ind = 0;
        labelW[ind++] = ui->label_sub_1;
        labelW[ind++] = ui->label_sub_2;
        labelW[ind++] = ui->label_sub_3;
        labelW[ind++] = ui->label_sub_4;
        labelW[ind++] = ui->label_sub_5;
        labelW[ind++] = ui->label_sub_6;
        labelW[ind++] = ui->label_sub_7;
        labelW[ind++] = ui->label_sub_8;
        labelW[ind++] = ui->label_sub_9;
        labelW[ind++] = ui->label_sub_10;
        labelW[ind++] = ui->label_sub_11;
        labelW[ind++] = ui->label_sub_12;
        labelW[ind++] = ui->label_sub_13;
        labelW[ind++] = ui->label_sub_14;
        labelW[ind++] = ui->label_sub_15;
        labelW[ind++] = ui->label_sub_16;
        labelW[ind++] = ui->label_sub_17;
        labelW[ind++] = ui->label_sub_18;
        labelW[ind++] = ui->label_sub_19;
        labelW[ind++] = ui->label_sub_20;
        labelW[ind++] = ui->label_sub_21;
        labelW[ind++] = ui->label_sub_22;
        labelW[ind++] = ui->label_sub_23;
        labelW[ind++] = ui->label_sub_24;
        labelW[ind++] = ui->label_sub_25;
        labelW[ind++] = ui->label_sub_26;
        labelW[ind++] = ui->label_sub_27;
        labelW[ind++] = ui->label_sub_28;
        labelW[ind++] = ui->label_sub_29;
        labelW[ind++] = ui->label_sub_30;
    }
    ui->pushButton_s_m->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_s_m->setStyleSheet(gResSwitchOn);

    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);

    ui->label_main->setStyleSheet("color:white");
    ui->label_off->setStyleSheet("color:white");
    ui->label_on->setStyleSheet("color:white");

    ui->label_main->setText("A主路");
    ui->label_off->setText("开关断开");
    ui->label_on->setText("开关闭合");

    for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
    {
        lightBt[i]->setStyleSheet(gResSwitchOff);
        lightBt[i]->setFocusPolicy(Qt::NoFocus);
        labelW[i]->setStyleSheet("color:white");
        QString str = QString("A") + QString::number(i + 1);
        labelW[i]->setText(str);
    }
    ui->pushButton_s_m->setStyleSheet(gResSwitchOff);
    ui->pushButton_s_m->setFocusPolicy(Qt::NoFocus);

    ui->widget_off->setStyleSheet(gResButtonRed);
    ui->widget_on->setStyleSheet(gResButtonGreen);

}

void CSwitchStateDialog::SetLight(int ui_ind, bool is_on)
{
}
