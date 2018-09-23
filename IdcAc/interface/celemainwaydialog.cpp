#include "celemainwaydialog.h"
#include "ui_celemainwaydialog.h"

#include "CommonFunction.h"

CEleMainWayDialog::CEleMainWayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEleMainWayDialog)
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

CEleMainWayDialog::~CEleMainWayDialog()
{
    delete ui;
}

void CEleMainWayDialog::slot_to_main_page_clicked()
{
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CEleMainWayDialog::slot_pre_page_clicked()
{
    emit signal_change_child_dialog(E_ELE_MAIN_WAY_DIALOG_A);
}

void CEleMainWayDialog::slot_next_page_clicked()
{
    emit signal_change_child_dialog(E_ELE_MAIN_WAY_DIALOG_B);
}

void CEleMainWayDialog::SetOuterDev(const str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;
}

void CEleMainWayDialog::SetUi(const st_dev_inf* p_dev_inf)
{
    int way_code = -1;
    int m_ind = -1;
    switch(dialogInd)
    {
    case E_ELE_MAIN_WAY_DIALOG_A:
        way_code = MAIN_A_CODE;
        m_ind = 0;
        break;
    case E_ELE_MAIN_WAY_DIALOG_B:
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
            int way_id = p_st_way_data->way_id;
            //0主路,1-24支路
            if(way_id == way_code)
            {
                const st_ac_meter* p_ac_meter = &p_dev_inf->dev_measure.ac_meter;
                const st_inline_m_param* p_st_inline_m_param = &p_ac_meter->inline_m_param[m_ind];
                QString str_val;
                str_val = Uint2DoubleQStr(p_st_inline_m_param->u_phase[0] * 10, gTwoUnit);
                ui->lineEdit_u_a->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->u_phase[1] * 10, gTwoUnit);
                ui->lineEdit_u_b->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->u_phase[2] * 10, gTwoUnit);
                ui->lineEdit_u_c->setText(str_val);

                str_val = Uint2DoubleQStr(p_st_inline_m_param->u_line[0] * 10, gTwoUnit);
                ui->lineEdit_u_ab->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->u_line[1] * 10, gTwoUnit);
                ui->lineEdit_u_bc->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->u_line[2] * 10, gTwoUnit);
                ui->lineEdit_u_ca->setText(str_val);

                str_val = Uint2DoubleQStr(p_st_inline_m_param->i_phase[0] * 10, gThreeUnit);
                ui->lineEdit_i_a->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->i_phase[1] * 10, gThreeUnit);
                ui->lineEdit_i_b->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->i_phase[2] * 10, gThreeUnit);
                ui->lineEdit_i_c->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->i_phase[3] * 10, gThreeUnit);
                ui->lineEdit_i_zero->setText(str_val);

                str_val = Uint2DoubleQStr(p_st_inline_m_param->frequence, gTwoUnit);
                ui->lineEdit_frequence->setText(str_val);

                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_phase[0][0] * 10, gThreeUnit);
                ui->lineEdit_power_active_a->setText(str_val);
                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_phase[1][0] * 10, gThreeUnit);
                ui->lineEdit_power_active_b->setText(str_val);
                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_phase[2][0] * 10, gThreeUnit);
                ui->lineEdit_power_active_c->setText(str_val);
                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_total[0] * 10, gThreeUnit);
                ui->lineEdit_power_active_all->setText(str_val);

                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_phase[0][1] * 10, gThreeUnit);
                ui->lineEdit_power_reactive_a->setText(str_val);
                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_phase[1][1] * 10, gThreeUnit);
                ui->lineEdit_power_reactive_b->setText(str_val);
                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_phase[2][1] * 10, gThreeUnit);
                ui->lineEdit_power_reactive_c->setText(str_val);
                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_total[1] * 10, gThreeUnit);
                ui->lineEdit_power_reactive_all->setText(str_val);

                //总有功电能；总无功电能；
                str_val = Uint2DoubleQStr(p_st_inline_m_param->energy_total[0] * 10, gThreeUnit);
                ui->lineEdit_energy_active->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->energy_total[1] * 10, gThreeUnit);
                ui->lineEdit_energy_reactive->setText(str_val);

                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_factor_phase[0], gTwoUnit);
                ui->lineEdit_power_factor_a->setText(str_val);
                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_factor_phase[1], gTwoUnit);
                ui->lineEdit_power_factor_b->setText(str_val);
                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_factor_phase[2], gTwoUnit);
                ui->lineEdit_power_factor_c->setText(str_val);
                str_val = Sint2DoubleQStr(p_st_inline_m_param->power_factor_total, gTwoUnit);
                ui->lineEdit_power_factor_all->setText(str_val);

                str_val = Uint2DoubleQStr(p_st_inline_m_param->power_phase_apparent[0] * 10, gThreeUnit);
                ui->lineEdit_power_apparent_a->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->power_phase_apparent[1] * 10, gThreeUnit);
                ui->lineEdit_power_apparent_b->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->power_phase_apparent[2] * 10, gThreeUnit);
                ui->lineEdit_power_apparent_c->setText(str_val);
                str_val = Uint2DoubleQStr(p_st_inline_m_param->power_apparent * 10, gThreeUnit);
                ui->lineEdit_power_apparent_all->setText(str_val);
            }
        }
    }
}

void CEleMainWayDialog::InitUi()
{
    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);

    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);

    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);

    ui->widget->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_2->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_3->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_4->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_5->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_6->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_7->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_8->setStyleSheet(".QWidget{border:1px solid white}");
    ui->widget_9->setStyleSheet(".QWidget{border:1px solid white}");

    ui->label_11->setStyleSheet("color:white");
    ui->label_12->setStyleSheet("color:white");
    ui->label_13->setStyleSheet("color:white");
    ui->label_14->setStyleSheet("color:white");
    ui->label_15->setStyleSheet("color:white");
    ui->label_16->setStyleSheet("color:white");
    ui->label_17->setStyleSheet("color:white");
    ui->label_18->setStyleSheet("color:white");
    ui->label_28->setStyleSheet("color:white");
    ui->label_30->setStyleSheet("color:white");
    ui->label_32->setStyleSheet("color:white");
    ui->label_34->setStyleSheet("color:white");
    ui->label_35->setStyleSheet("color:white");
    ui->label_36->setStyleSheet("color:white");
    ui->label_37->setStyleSheet("color:white");
    ui->label_38->setStyleSheet("color:white");
    ui->label_39->setStyleSheet("color:white");
    ui->label_40->setStyleSheet("color:white");
    ui->label_41->setStyleSheet("color:white");
    ui->label_42->setStyleSheet("color:white");
    ui->label_43->setStyleSheet("color:white");
    ui->label_44->setStyleSheet("color:white");
    ui->label_45->setStyleSheet("color:white");
    ui->label_46->setStyleSheet("color:white");
    ui->label_47->setStyleSheet("color:white");
    ui->label_48->setStyleSheet("color:white");
    ui->label_49->setStyleSheet("color:white");
    ui->label_50->setStyleSheet("color:white");
    ui->label_53->setStyleSheet("color:white");
    ui->label_54->setStyleSheet("color:white");
    ui->label_55->setStyleSheet("color:white");
    ui->label_56->setStyleSheet("color:white");
    ui->label_59->setStyleSheet("color:white");
    ui->label_60->setStyleSheet("color:white");
    ui->label_61->setStyleSheet("color:white");
    ui->label_62->setStyleSheet("color:white");
    ui->label_63->setStyleSheet("color:white");
    ui->label_64->setStyleSheet("color:white");
    ui->label_65->setStyleSheet("color:white");
    ui->label_66->setStyleSheet("color:white");
    ui->label_67->setStyleSheet("color:white");
    ui->label_68->setStyleSheet("color:white");
    ui->label_69->setStyleSheet("color:white");
    ui->label_70->setStyleSheet("color:white");
    ui->label_71->setStyleSheet("color:white");
    ui->label_72->setStyleSheet("color:white");
    ui->label_73->setStyleSheet("color:white");
    ui->label_74->setStyleSheet("color:white");
    ui->label_75->setStyleSheet("color:white");
    ui->label_76->setStyleSheet("color:white");
    ui->label_77->setStyleSheet("color:white");
    ui->label_78->setStyleSheet("color:white");
    ui->label_79->setStyleSheet("color:white");
    ui->label_80->setStyleSheet("color:white");


    QString str_val;
    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_u_a->setText(str_val);
    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_u_b->setText(str_val);
    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_u_c->setText(str_val);

    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_u_ab->setText(str_val);
    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_u_bc->setText(str_val);
    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_u_ca->setText(str_val);

    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_i_a->setText(str_val);
    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_i_b->setText(str_val);
    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_i_c->setText(str_val);
    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_i_zero->setText(str_val);

    str_val = Uint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_frequence->setText(str_val);

    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_active_a->setText(str_val);
    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_active_b->setText(str_val);
    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_active_c->setText(str_val);
    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_active_all->setText(str_val);

    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_reactive_a->setText(str_val);
    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_reactive_b->setText(str_val);
    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_reactive_c->setText(str_val);
    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_reactive_all->setText(str_val);

    //总有功电能；总无功电能；
    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_energy_active->setText(str_val);
    str_val = Uint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_energy_reactive->setText(str_val);

    str_val = Sint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_power_factor_a->setText(str_val);
    str_val = Sint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_power_factor_b->setText(str_val);
    str_val = Sint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_power_factor_c->setText(str_val);
    str_val = Sint2DoubleQStr(0, gTwoUnit);
    ui->lineEdit_power_factor_all->setText(str_val);

    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_apparent_a->setText(str_val);
    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_apparent_b->setText(str_val);
    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_apparent_c->setText(str_val);
    str_val = Sint2DoubleQStr(0, gThreeUnit);
    ui->lineEdit_power_apparent_all->setText(str_val);


    ui->lineEdit_energy_active->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_energy_reactive->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_frequence->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_a->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_b->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_c->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_i_zero->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_active_a->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_active_all->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_active_b->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_active_c->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_apparent_a->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_apparent_all->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_apparent_b->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_apparent_c->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_factor_a->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_factor_all->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_factor_b->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_factor_c->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_reactive_a->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_reactive_all->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_reactive_b->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_power_reactive_c->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_a->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_ab->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_b->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_bc->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_c->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_u_ca->setFocusPolicy(Qt::NoFocus);
}

