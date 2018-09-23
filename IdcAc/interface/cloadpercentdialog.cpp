#include "cloadpercentdialog.h"
#include "ui_cloadpercentdialog.h"

#include "CommonFunction.h"

CLoadPercentDialog::CLoadPercentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoadPercentDialog)
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
}

CLoadPercentDialog::~CLoadPercentDialog()
{
    delete ui;
}

void CLoadPercentDialog::slot_to_main_page_clicked()
{
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CLoadPercentDialog::slot_pre_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_LOAD_PERCENT_DIALOG_A)
    {
        ind = dialogInd;
    }
    else if(dialogInd == E_LOAD_PERCENT_DIALOG_B)
    {
        ind = E_LOAD_PERCENT_DIALOG_A;
    }
    emit signal_change_child_dialog(ind);
}

void CLoadPercentDialog::slot_next_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_LOAD_PERCENT_DIALOG_A)
    {
        ind = E_LOAD_PERCENT_DIALOG_B;
    }
    else if(dialogInd == E_LOAD_PERCENT_DIALOG_B)
    {
        ind = dialogInd;
    }
    emit signal_change_child_dialog(ind);
}

void CLoadPercentDialog::SetUi(const st_dev_inf* p_dev_inf)
{
    int way_code = -1;
    int m_ind = -1;
    switch(dialogInd)
    {
    case E_LOAD_PERCENT_DIALOG_A:
        way_code = MAIN_A_CODE;
        m_ind = 0;
        break;
    case E_LOAD_PERCENT_DIALOG_B:
        way_code = MAIN_B_CODE;
        m_ind = 1;
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
            const st_inline_m_param* p_st_inline_m_param = &p_ac_meter->inline_m_param[m_ind];
            double current = 0;
            double percent = 0.0;
            double current_range = p_dev_inf->dev_measure.ac_meter.i_m_range[m_ind] * gOneUnit;
            int way_id = p_st_way_data->way_id;
            if(way_id == way_code)
            {
                //主路
                current = p_st_inline_m_param->i_phase[0] * gTwoUnit;
                percent = 100 * 100 * current / current_range;
                ui->lineEdit_main_a->setText(Uint2DoubleQStr(percent, gTwoUnit));

                current = p_st_inline_m_param->i_phase[1] * gTwoUnit;
                percent = 100 * 100 * current / current_range;
                ui->lineEdit_main_b->setText(Uint2DoubleQStr(percent, gTwoUnit));

                current = p_st_inline_m_param->i_phase[2] * gTwoUnit;
                percent = 100 * 100 * current / current_range;
                ui->lineEdit_main_c->setText(Uint2DoubleQStr(percent, gTwoUnit));
            }
            else if(way_id > way_code && way_id <= way_code + SUB_WAY_PARAM_NUM)
            {
                //支路
                int sub_way_param_ind = way_id - way_code - 1;
                int real_data_ind = sub_way_param_ind;
                if(way_code == MAIN_B_CODE)
                {
                    real_data_ind = sub_way_param_ind + MAIN_B_OFF_ELECTRIC;
                }
                current = p_ac_meter->i_out_line[real_data_ind] * gTwoUnit;
                percent = 100 * 100 * current / current_range;
                lineEdit[sub_way_param_ind]->setText(Uint2DoubleQStr(percent, gTwoUnit));
            }
        }
    }
}

void CLoadPercentDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;

    if(dialogInd == E_LOAD_PERCENT_DIALOG_B)
    {
        ui->textEdit_title->setText("主\r\n路\r\nB\r\n");
        for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
        {
            label[i]->setStyleSheet("color:white");
            QString str = "B";
            str += QString::number(i+1);
            label[i]->setText(str);
        }
    }
}

void CLoadPercentDialog::InitUi()
{
    int ind = 0;

    {
        ind = 0;
        lineEdit[ind++] = ui->lineEdit_w1;
        lineEdit[ind++] = ui->lineEdit_w2;
        lineEdit[ind++] = ui->lineEdit_w3;
        lineEdit[ind++] = ui->lineEdit_w4;
        lineEdit[ind++] = ui->lineEdit_w5;
        lineEdit[ind++] = ui->lineEdit_w6;
        lineEdit[ind++] = ui->lineEdit_w7;
        lineEdit[ind++] = ui->lineEdit_w8;
        lineEdit[ind++] = ui->lineEdit_w9;
        lineEdit[ind++] = ui->lineEdit_w10;
        lineEdit[ind++] = ui->lineEdit_w11;
        lineEdit[ind++] = ui->lineEdit_w12;
        lineEdit[ind++] = ui->lineEdit_w13;
        lineEdit[ind++] = ui->lineEdit_w14;
        lineEdit[ind++] = ui->lineEdit_w15;
        lineEdit[ind++] = ui->lineEdit_w16;
        lineEdit[ind++] = ui->lineEdit_w17;
        lineEdit[ind++] = ui->lineEdit_w18;
        lineEdit[ind++] = ui->lineEdit_w19;
        lineEdit[ind++] = ui->lineEdit_w20;
        lineEdit[ind++] = ui->lineEdit_w21;
        lineEdit[ind++] = ui->lineEdit_w22;
        lineEdit[ind++] = ui->lineEdit_w23;
        lineEdit[ind++] = ui->lineEdit_w24;
        lineEdit[ind++] = ui->lineEdit_w25;
        lineEdit[ind++] = ui->lineEdit_w26;
        lineEdit[ind++] = ui->lineEdit_w27;
        lineEdit[ind++] = ui->lineEdit_w28;
        lineEdit[ind++] = ui->lineEdit_w29;
        lineEdit[ind++] = ui->lineEdit_w30;
    }

    {
        ind = 0;
        label[ind++] = ui->label_w1;
        label[ind++] = ui->label_w2;
        label[ind++] = ui->label_w3;
        label[ind++] = ui->label_w4;
        label[ind++] = ui->label_w5;
        label[ind++] = ui->label_w6;
        label[ind++] = ui->label_w7;
        label[ind++] = ui->label_w8;
        label[ind++] = ui->label_w9;
        label[ind++] = ui->label_w10;
        label[ind++] = ui->label_w11;
        label[ind++] = ui->label_w12;
        label[ind++] = ui->label_w13;
        label[ind++] = ui->label_w14;
        label[ind++] = ui->label_w15;
        label[ind++] = ui->label_w16;
        label[ind++] = ui->label_w17;
        label[ind++] = ui->label_w18;
        label[ind++] = ui->label_w19;
        label[ind++] = ui->label_w20;
        label[ind++] = ui->label_w21;
        label[ind++] = ui->label_w22;
        label[ind++] = ui->label_w23;
        label[ind++] = ui->label_w24;
        label[ind++] = ui->label_w25;
        label[ind++] = ui->label_w26;
        label[ind++] = ui->label_w27;
        label[ind++] = ui->label_w28;
        label[ind++] = ui->label_w29;
        label[ind++] = ui->label_w30;
    }

    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);

    ui->widget->setStyleSheet(".QWidget{border:1px solid yellow}");
    ui->label_title->setStyleSheet("color:yellow");
    ui->label_main_a->setStyleSheet("color:white");
    ui->label_main_b->setStyleSheet("color:white");
    ui->label_main_c->setStyleSheet("color:white");
    for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
    {
        label[i]->setStyleSheet("color:white");
        QString str = "A";
        str += QString::number(i+1);
        label[i]->setText(str);
    }

    QFont ft;
    ft.setPointSize(10);
    ui->textEdit_title->setFont(ft);
    ui->textEdit_title->setText("主\r\n路\r\nA\r\n");
    ui->textEdit_title->setFocusPolicy(Qt::NoFocus);
    ui->label_main_a->setText("A相负载百分比(%)");
    ui->label_main_b->setText("B相负载百分比(%)");
    ui->label_main_c->setText("C相负载百分比(%)");

    ui->lineEdit_main_a->setText(Uint2DoubleQStr(0, gTwoUnit));
    ui->lineEdit_main_a->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_main_b->setText(Uint2DoubleQStr(0, gTwoUnit));
    ui->lineEdit_main_b->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_main_c->setText(Uint2DoubleQStr(0, gTwoUnit));
    ui->lineEdit_main_c->setFocusPolicy(Qt::NoFocus);
    for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
    {
        lineEdit[i]->setText(Uint2DoubleQStr(0, gTwoUnit));
        lineEdit[i]->setFocusPolicy(Qt::NoFocus);
    }
}
