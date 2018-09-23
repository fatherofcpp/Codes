#include "cpowercheckdialog.h"
#include "ui_cpowercheckdialog.h"
#include <QPainter>
#include "CommonFunction.h"

CPowerCheckDialog::CPowerCheckDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPowerCheckDialog)
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

CPowerCheckDialog::~CPowerCheckDialog()
{
    delete ui;
}

void CPowerCheckDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::yellow));//设置画笔形式

    QLineF linef;
    int x1;
    int y1;
    int x2;
    int y2;

    x1 = ui->widget_line_1->geometry().left();
    y1 = ui->widget_line_1->geometry().top();
    x2 = x1;
    y2 = ui->widget_line_1->geometry().bottom();
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_2->geometry().left();
    y1 = ui->widget_line_2->geometry().top();
    x2 = x1;
    y2 = ui->widget_line_2->geometry().bottom();
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_3->geometry().left();
    y1 = ui->widget_line_3->geometry().top();
    x2 = x1;
    y2 = ui->widget_line_3->geometry().bottom();
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_4->geometry().left();
    y1 = ui->widget_line_4->geometry().top();
    x2 = x1;
    y2 = ui->widget_line_4->geometry().bottom();
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_5->geometry().left();
    y1 = ui->widget_line_5->geometry().top();
    x2 = x1;
    y2 = ui->widget_line_5->geometry().bottom();
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_6->geometry().left();
    y1 = ui->widget_line_6->geometry().top();
    x2 = x1;
    y2 = ui->widget_line_6->geometry().bottom();
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_7->geometry().left();
    y1 = ui->widget_line_7->geometry().top();
    x2 = x1;
    y2 = ui->widget_line_7->geometry().bottom();
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线

    x1 = ui->widget_line_top->geometry().left();
    y1 = ui->widget_line_top->geometry().top();
    x2 = ui->widget_line_top->geometry().right();
    y2 = y1;
    linef.setPoints(QPoint(x1, y1),
                    QPoint(x2, y2));
    painter.drawLine(linef);//画直线
}

void CPowerCheckDialog::slot_to_main_page_clicked()
{
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CPowerCheckDialog::slot_pre_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_POWER_CHECK_DIALOG_A)
    {
        ind = dialogInd;
    }
    else if(dialogInd == E_POWER_CHECK_DIALOG_B)
    {
        ind = E_POWER_CHECK_DIALOG_A;
    }
    emit signal_change_child_dialog(ind);
}

void CPowerCheckDialog::slot_next_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_POWER_CHECK_DIALOG_A)
    {
        ind = E_POWER_CHECK_DIALOG_B;
    }
    else if(dialogInd == E_POWER_CHECK_DIALOG_B)
    {
        ind = dialogInd;
    }
    emit signal_change_child_dialog(ind);
}

void CPowerCheckDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;

    if(dialogInd == E_POWER_CHECK_DIALOG_B)
    {
        for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
        {
            QString str = QString("B") + QString::number(i + 1);
            labelW[i]->setText(str);
        }
    }
}

void CPowerCheckDialog::SetUi(const st_dev_inf* p_dev_inf)
{
    int way_code = -1;
    int power_off = -1;
    switch(dialogInd)
    {
    case E_POWER_CHECK_DIALOG_A:
        way_code = MAIN_A_CODE;
        power_off = 0;
        break;
    case E_POWER_CHECK_DIALOG_B:
        way_code = MAIN_B_CODE;
        power_off = 3;
        break;
    default:
        break;
    }
    if(way_code < 0)
    {
        return;
    }

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
            const st_ac_meter* p_ac_meter = &p_dev_inf->dev_measure.ac_meter;
            int way_id = p_st_way_data->way_id;
            if(way_id == way_code)
            {
                //主路
                //为1则为负，为0则不为负
                //bit0-bit2表示主进线一的三相，bit3-bit5表示主进线的三相
                MU_INT16 power_state = p_ac_meter->u_err_power_negative;//B 1100
                MU_INT8 val = 0;
                val = (MU_INT8)((power_state >> power_off) & 0x01);
                if( val == 0x01 )
                {
                    //负
                    ui->pushButton_l_m_a->setStyleSheet(gResLightRed);
                    ui->pushButton_l_m_a->SetOnOff(false);
                }
                else
                {
                    ui->pushButton_l_m_a->setStyleSheet(gResLightGreen);
                    ui->pushButton_l_m_a->SetOnOff(true);
                }
                val = (MU_INT8)((power_state >> (power_off+1)) & 0x01);
                if( val == 0x01 )
                {
                    //负
                    ui->pushButton_l_m_b->setStyleSheet(gResLightRed);
                    ui->pushButton_l_m_b->SetOnOff(false);
                }
                else
                {
                    ui->pushButton_l_m_b->setStyleSheet(gResLightGreen);
                    ui->pushButton_l_m_b->SetOnOff(true);
                }
                val = (MU_INT8)((power_state >> (power_off+2)) & 0x01);
                if( val == 0x01 )
                {
                    //负
                    ui->pushButton_l_m_c->setStyleSheet(gResLightRed);
                    ui->pushButton_l_m_c->SetOnOff(false);
                }
                else
                {
                    ui->pushButton_l_m_c->setStyleSheet(gResLightGreen);
                    ui->pushButton_l_m_c->SetOnOff(true);
                }
            }
            else if(way_id > way_code && way_id <= way_code + SUB_WAY_PARAM_NUM)
            {
                //支路
                int sub_way_param_ind = way_id - way_code - 1;
                int power_way_ind = sub_way_param_ind;
                if(way_code == MAIN_B_CODE)
                {
                    power_way_ind = sub_way_param_ind + MAIN_B_OFF_ELECTRIC;
                }
                bool is_on = false;
                const int num_power = 16;
                const int off_bit = power_way_ind % num_power;
                const int power_ind = power_way_ind / num_power;
                MU_INT16 power_negative = p_ac_meter->power_negative[power_ind];
                if((MU_INT8)((power_negative >> off_bit) & 0x01) == 0)
                {
                    is_on = true;
                }
                else
                {
                    is_on = false;
                }
                SetLight(sub_way_param_ind, is_on);
            }
        }
    }
    repaint();
}

void CPowerCheckDialog::InitUi()
{
    int ind = 0;

    {
        ind = 0;
        lightBt[ind++] = ui->pushButton_l_w_1;
        lightBt[ind++] = ui->pushButton_l_w_2;
        lightBt[ind++] = ui->pushButton_l_w_3;
        lightBt[ind++] = ui->pushButton_l_w_4;
        lightBt[ind++] = ui->pushButton_l_w_5;
        lightBt[ind++] = ui->pushButton_l_w_6;
        lightBt[ind++] = ui->pushButton_l_w_7;
        lightBt[ind++] = ui->pushButton_l_w_8;
        lightBt[ind++] = ui->pushButton_l_w_9;
        lightBt[ind++] = ui->pushButton_l_w_10;
        lightBt[ind++] = ui->pushButton_l_w_11;
        lightBt[ind++] = ui->pushButton_l_w_12;
        lightBt[ind++] = ui->pushButton_l_w_13;
        lightBt[ind++] = ui->pushButton_l_w_14;
        lightBt[ind++] = ui->pushButton_l_w_15;
        lightBt[ind++] = ui->pushButton_l_w_16;
        lightBt[ind++] = ui->pushButton_l_w_17;
        lightBt[ind++] = ui->pushButton_l_w_18;
        lightBt[ind++] = ui->pushButton_l_w_19;
        lightBt[ind++] = ui->pushButton_l_w_20;
        lightBt[ind++] = ui->pushButton_l_w_21;
        lightBt[ind++] = ui->pushButton_l_w_22;
        lightBt[ind++] = ui->pushButton_l_w_23;
        lightBt[ind++] = ui->pushButton_l_w_24;
        lightBt[ind++] = ui->pushButton_l_w_25;
        lightBt[ind++] = ui->pushButton_l_w_26;
        lightBt[ind++] = ui->pushButton_l_w_27;
        lightBt[ind++] = ui->pushButton_l_w_28;
        lightBt[ind++] = ui->pushButton_l_w_29;
        lightBt[ind++] = ui->pushButton_l_w_30;

        ind = 0;
        labelW[ind++] = ui->label_w_1;
        labelW[ind++] = ui->label_w_2;
        labelW[ind++] = ui->label_w_3;
        labelW[ind++] = ui->label_w_4;
        labelW[ind++] = ui->label_w_5;
        labelW[ind++] = ui->label_w_6;
        labelW[ind++] = ui->label_w_7;
        labelW[ind++] = ui->label_w_8;
        labelW[ind++] = ui->label_w_9;
        labelW[ind++] = ui->label_w_10;
        labelW[ind++] = ui->label_w_11;
        labelW[ind++] = ui->label_w_12;
        labelW[ind++] = ui->label_w_13;
        labelW[ind++] = ui->label_w_14;
        labelW[ind++] = ui->label_w_15;
        labelW[ind++] = ui->label_w_16;
        labelW[ind++] = ui->label_w_17;
        labelW[ind++] = ui->label_w_18;
        labelW[ind++] = ui->label_w_19;
        labelW[ind++] = ui->label_w_20;
        labelW[ind++] = ui->label_w_21;
        labelW[ind++] = ui->label_w_22;
        labelW[ind++] = ui->label_w_23;
        labelW[ind++] = ui->label_w_24;
        labelW[ind++] = ui->label_w_25;
        labelW[ind++] = ui->label_w_26;
        labelW[ind++] = ui->label_w_27;
        labelW[ind++] = ui->label_w_28;
        labelW[ind++] = ui->label_w_29;
        labelW[ind++] = ui->label_w_30;
    }

    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);

    ui->label_m_a->setStyleSheet("color:white");
    ui->label_m_b->setStyleSheet("color:white");
    ui->label_m_c->setStyleSheet("color:white");
    ui->label_power_normal->setStyleSheet("color:yellow");
    ui->label_power_abnormal->setStyleSheet("color:yellow");

    ui->label_m_a->setText("A相");
    ui->label_m_b->setText("B相");
    ui->label_m_c->setText("C相");
    ui->label_power_normal->setText("功率正常");
    ui->label_power_abnormal->setText("功率异常");

    ui->pushButton_l_m_a->setStyleSheet(gResLightRed);
    ui->pushButton_l_m_a->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_l_m_b->setStyleSheet(gResLightRed);
    ui->pushButton_l_m_b->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_l_m_c->setStyleSheet(gResLightRed);
    ui->pushButton_l_m_c->setFocusPolicy(Qt::NoFocus);
    for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
    {
        lightBt[i]->setStyleSheet(gResLightRed);
        lightBt[i]->setFocusPolicy(Qt::NoFocus);
        labelW[i]->setStyleSheet("color:white");
        QString str = QString("A") + QString::number(i + 1);
        labelW[i]->setText(str);
    }

    ui->pushButton_power_normal->setStyleSheet(gResLightGreen);
    ui->pushButton_power_normal->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_power_innormal->setStyleSheet(gResLightRed);
    ui->pushButton_power_innormal->setFocusPolicy(Qt::NoFocus);
}

void CPowerCheckDialog::SetLight(int ui_ind, bool is_on)
{
    if(is_on)
    {
        lightBt[ui_ind]->setStyleSheet(gResLightGreen);
    }
    else
    {
        lightBt[ui_ind]->setStyleSheet(gResLightRed);
    }
    lightBt[ui_ind]->SetOnOff(is_on);
}

