#include "charmpercentdialog.h"
#include "ui_charmpercentdialog.h"

#include "CommonFunction.h"

CHarmPercentDialog::CHarmPercentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CHarmPercentDialog)
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

CHarmPercentDialog::~CHarmPercentDialog()
{
    delete ui;
}

void CHarmPercentDialog::slot_to_main_page_clicked()
{
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CHarmPercentDialog::slot_pre_page_clicked()
{
}

void CHarmPercentDialog::slot_next_page_clicked()
{
    emit signal_change_child_dialog(E_HARM_U_I_DIALOG_A_UA);
}

void CHarmPercentDialog::SetUi(const st_dev_inf* p_dev_inf)
{
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
            if(way_id == MAIN_A_CODE)
            {
                //主路A
                const st_ac_meter* p_ac_meter = &p_dev_inf->dev_measure.ac_meter;
                const st_harm_param* p_st_harm_param = &p_ac_meter->harm_param[0];
                QString str_val;
                str_val = Uint2DoubleQStr(p_st_harm_param->u_harm[0], gTwoUnit);
                ui->label_u_a_way_a->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->u_harm[1], gTwoUnit);
                ui->label_u_b_way_a->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->u_harm[2], gTwoUnit);
                ui->label_u_c_way_a->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->i_harm[0], gTwoUnit);
                ui->label_i_a_way_a->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->i_harm[1], gTwoUnit);
                ui->label_i_b_way_a->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->i_harm[2], gTwoUnit);
                ui->label_i_c_way_a->setText(str_val);
            }
            else if(way_id == MAIN_B_CODE)
            {
                //主路B
                const st_ac_meter* p_ac_meter = &p_dev_inf->dev_measure.ac_meter;
                const st_harm_param* p_st_harm_param = &p_ac_meter->harm_param[1];
                QString str_val;
                str_val = Uint2DoubleQStr(p_st_harm_param->u_harm[0], gTwoUnit);
                ui->label_u_a_way_b->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->u_harm[1], gTwoUnit);
                ui->label_u_b_way_b->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->u_harm[2], gTwoUnit);
                ui->label_u_c_way_b->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->i_harm[0], gTwoUnit);
                ui->label_i_a_way_b->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->i_harm[1], gTwoUnit);
                ui->label_i_b_way_b->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_harm_param->i_harm[2], gTwoUnit);
                ui->label_i_c_way_b->setText(str_val);
            }
        }
    }
}

void CHarmPercentDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;
}

void CHarmPercentDialog::InitUi()
{
    //bt
    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);

    ui->widget_a->setStyleSheet(".QWidget{border:1px solid yellow}");
    ui->widget_b->setStyleSheet(".QWidget{border:1px solid yellow}");
    ui->label_title_a->setStyleSheet("color:yellow");
    ui->label_title_b->setStyleSheet("color:yellow");

    ui->label_11->setStyleSheet("color:white");
    ui->label_13->setStyleSheet("color:white");
    ui->label_15->setStyleSheet("color:white");
    ui->label_17->setStyleSheet("color:white");
    ui->label_18->setStyleSheet("color:white");
    ui->label_20->setStyleSheet("color:white");
    ui->label_22->setStyleSheet("color:white");
    ui->label_24->setStyleSheet("color:white");
    ui->label_26->setStyleSheet("color:white");
    ui->label_28->setStyleSheet("color:white");
    ui->label_7->setStyleSheet("color:white");
    ui->label_9->setStyleSheet("color:white");
    ui->label_i_a_way_a->setStyleSheet("color:white");
    ui->label_i_b_way_a->setStyleSheet("color:white");
    ui->label_i_c_way_a->setStyleSheet("color:white");
    ui->label_u_a_way_a->setStyleSheet("color:white");
    ui->label_u_b_way_a->setStyleSheet("color:white");
    ui->label_u_c_way_a->setStyleSheet("color:white");
    ui->label_i_a_way_b->setStyleSheet("color:white");
    ui->label_i_b_way_b->setStyleSheet("color:white");
    ui->label_i_c_way_b->setStyleSheet("color:white");
    ui->label_u_a_way_b->setStyleSheet("color:white");
    ui->label_u_b_way_b->setStyleSheet("color:white");
    ui->label_u_c_way_b->setStyleSheet("color:white");

    ui->label_i_a_way_a->setText("0.00");
    ui->label_i_b_way_a->setText("0.00");
    ui->label_i_c_way_a->setText("0.00");
    ui->label_u_a_way_a->setText("0.00");
    ui->label_u_b_way_a->setText("0.00");
    ui->label_u_c_way_a->setText("0.00");
    ui->label_i_a_way_b->setText("0.00");
    ui->label_i_b_way_b->setText("0.00");
    ui->label_i_c_way_b->setText("0.00");
    ui->label_u_a_way_b->setText("0.00");
    ui->label_u_b_way_b->setText("0.00");
    ui->label_u_c_way_b->setText("0.00");

}
