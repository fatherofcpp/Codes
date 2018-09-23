#include "cwarnsetdialog.h"
#include "ui_cwarnsetdialog.h"

#include "CommonFunction.h"

static int s_fuse_ind = 12;
CWarnSetDialog::CWarnSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CWarnSetDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    pOuterDev = NULL;

    pCProgressDialog = new CProgressDialog(this);
    memset(&signalParm, 0, sizeof(signalParm));

    InitUi();

    connect(ui->pushButton_pre_page, SIGNAL(clicked()),
            this, SLOT(slot_pre_page_clicked()));
    connect(ui->pushButton_next_page, SIGNAL(clicked()),
            this, SLOT(slot_next_page_clicked()));
    connect(ui->pushButton_return_back, SIGNAL(clicked()),
            this, SLOT(slot_to_main_page_clicked()));

    for(int i = 0; i < MAIN_WAY_PARAM_NUM; i++)
    {
        lightBt[i]->SetCode(i);
        saveBt[i]->SetCode(i);
        connect(lightBt[i], SIGNAL(signal_key_clicked(const st_bt_inf&)),
                this, SLOT(slot_light_clicked(const st_bt_inf&)));
        connect(saveBt[i], SIGNAL(signal_key_clicked(const st_bt_inf&)),
                this, SLOT(slot_save_clicked(const st_bt_inf&)));
    }
}

CWarnSetDialog::~CWarnSetDialog()
{
    delete ui;
}

void CWarnSetDialog::SetDialogInd(int val)
{
    dialogInd = val;
    signalParm.frm_sig = dialogInd;
}

void CWarnSetDialog::slot_to_main_page_clicked()
{
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CWarnSetDialog::slot_pre_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_WARN_SET_DIALOG_A)
    {
        return;
    }
    else if(dialogInd == E_WARN_SET_DIALOG_B)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_CONTENT;
    }
    emit signal_change_child_dialog(ind);
}

void CWarnSetDialog::slot_next_page_clicked()
{
    int ind = dialogInd;
    if(dialogInd == E_WARN_SET_DIALOG_A)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_I_DOWN;
    }
    else if(dialogInd == E_WARN_SET_DIALOG_B)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_I_DOWN;
    }
    emit signal_change_child_dialog(ind);
}

void CWarnSetDialog::slot_light_clicked(const st_bt_inf& pos)
{
    int light_ind = pos.code;
    bool is_on_now = !pos.is_on;
    SetLight(light_ind, is_on_now);
}


void CWarnSetDialog::slot_save_clicked(const st_bt_inf& pos)
{
    int light_ind = pos.code;
    MU_INT32 enable = 0;
    if(lightBt[light_ind]->GetOnOff())
    {
        enable = 1;
    }

    int way_code = 0;
    if(dialogInd == E_WARN_SET_DIALOG_A)
    {
        way_code = MAIN_A_CODE;
    }
    else if(dialogInd == E_WARN_SET_DIALOG_B)
    {
        way_code = MAIN_B_CODE;
    }

    st_way_data* p_st_way_data = NULL;
    int dev_type = -1;
    if(light_ind != s_fuse_ind)
    {
        //电
        dev_type = E_DEV_ELECTRIC;
    }
    else
    {
        //开关,主路开关状态放在电设备
//        dev_type = E_DEV_SWITCH;
        dev_type = E_DEV_ELECTRIC;
    }

    p_st_way_data = GetWayInfByCode(pOuterDev, way_code, dev_type);
    if(p_st_way_data)
    {
        MU_INT32* p_val = MapUi2Val(p_st_way_data, light_ind);
        MU_INT32 thre = threEdit[light_ind]->text().toFloat() * gOneReg;
        if(p_val)
        {
            st_threshold_item threshold_set;
            memset(&threshold_set, 0, sizeof(threshold_set));
            threshold_set.dev_type = dev_type;
            threshold_set.is_enable = enable;
            threshold_set.thre_val = thre;
            threshold_set.warn_type = MapUi2WarnType(light_ind);
            threshold_set.way_code = way_code;
            st_thread_param_threshold_set param_threshold_set;
            param_threshold_set.vec_threshold_item.clear();
            param_threshold_set.vec_threshold_item.push_back(threshold_set);

            int ret = 0;

            emit signal_write_threshold(&signalParm, &param_threshold_set);
            open_progress(pCProgressDialog,
                          GetTaskNameByType(E_WRITE_THRESHOLD_CMD));
            ret = signalParm.is_suc;

            messagebox(this, ret);
            if(ret == 0)
            {
                p_val[0] = thre;
                p_val[1] = enable;
            }
        }
    }
}

void CWarnSetDialog::SetOuterDev(str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;

    int way_code = 0;
    if(dialogInd == E_WARN_SET_DIALOG_A)
    {
        way_code = MAIN_A_CODE;
    }
    else if(dialogInd == E_WARN_SET_DIALOG_B)
    {
        way_code = MAIN_B_CODE;
    }

    {
        //电
        st_way_data* p_st_way_data = GetWayInfByCode(pOuterDev, way_code, E_DEV_ELECTRIC);
        if(p_st_way_data)
        {
            MU_INT32 thre = 0;
            bool is_on = true;
            MU_INT32 enable = 0;
            for(int i = 0; i < MAIN_WAY_PARAM_NUM; i ++)
            {
                MU_INT32* p_val = MapUi2Val(p_st_way_data, i);
                if(p_val)
                {
                    thre = p_val[0];
                    enable = p_val[1];
                    is_on = (enable != 0);
                    if(i != s_fuse_ind)
                    {
                        threEdit[i]->setText(Uint2DoubleQStr(thre, gOneUnit));
                    }
                    SetLight(i, is_on);
                }
            }
        }
    }
}


void CWarnSetDialog::InitUi()
{
    int ind = 0;

    ind = 0;
    lightBt[ind++] = ui->pushButton_u_down_light;
    lightBt[ind++] = ui->pushButton_u_up_light;
    lightBt[ind++] = ui->pushButton_i_down_light;
    lightBt[ind++] = ui->pushButton_i_up_light;
    lightBt[ind++] = ui->pushButton_lose_phase_light;
    lightBt[ind++] = ui->pushButton_i_zero_up_light;
    lightBt[ind++] = ui->pushButton_i_unbalance_up_light;
    lightBt[ind++] = ui->pushButton_u_harm_percent_up_light;
    lightBt[ind++] = ui->pushButton_i_down2_light;
    lightBt[ind++] = ui->pushButton_i_up2_light;
    lightBt[ind++] = ui->pushButton_f_up_light;
    lightBt[ind++] = ui->pushButton_f_down_light;
    lightBt[ind++] = ui->pushButton_fuse_light;
    lightBt[ind++] = ui->pushButton_i_harm_percent_light;

    ind = 0;
    saveBt[ind++] = ui->pushButton_u_down_save;
    saveBt[ind++] = ui->pushButton_u_up_save;
    saveBt[ind++] = ui->pushButton_i_down_save;
    saveBt[ind++] = ui->pushButton_i_up_save;
    saveBt[ind++] = ui->pushButton_lose_phase_save;
    saveBt[ind++] = ui->pushButton_i_zero_up_save;
    saveBt[ind++] = ui->pushButton_i_unbalance_up_save;
    saveBt[ind++] = ui->pushButton_u_harm_percent_up_save;
    saveBt[ind++] = ui->pushButton_i_down2_save;
    saveBt[ind++] = ui->pushButton_i_up2_save;
    saveBt[ind++] = ui->pushButton_f_up_save;
    saveBt[ind++] = ui->pushButton_f_down_save;
    saveBt[ind++] = ui->pushButton_fuse_save;
    saveBt[ind++] = ui->pushButton_i_harm_percent_save;

    ind = 0;
    threEdit[ind++] = ui->lineEdit_u_down;
    threEdit[ind++] = ui->lineEdit_u_up;
    threEdit[ind++] = ui->lineEdit_i_down;
    threEdit[ind++] = ui->lineEdit_i_up;
    threEdit[ind++] = ui->lineEdit_lose_phase;
    threEdit[ind++] = ui->lineEdit_i_zero_up;
    threEdit[ind++] = ui->lineEdit_i_unbalance_up;
    threEdit[ind++] = ui->lineEdit_u_harm_percent_up;
    threEdit[ind++] = ui->lineEdit_i_down2;
    threEdit[ind++] = ui->lineEdit_i_up2;
    threEdit[ind++] = ui->lineEdit_f_up;
    threEdit[ind++] = ui->lineEdit_f_down;
    threEdit[ind++] = ui->lineEdit_fuse;
    threEdit[ind++] = ui->lineEdit_i_harm_percent;

    for(int i = 0; i < MAIN_WAY_PARAM_NUM; i++)
    {
        threEdit[i]->SetValid(E_VALID_FLOAT);
    }

    ui->pushButton_pre_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_pre_page->setStyleSheet(gResArrowLeft);
    ui->pushButton_next_page->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_next_page->setStyleSheet(gResArrowRight);
    ui->pushButton_return_back->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_return_back->setStyleSheet(gResReturnBack);

    ui->widget_help->setStyleSheet(".QWidget{border:1px solid yellow}");
    ui->widget_body->setStyleSheet(".QWidget{border:1px solid yellow}");

    ui->pushButton_help_light->setStyleSheet(gResLightGreen);
    ui->pushButton_help_light_on->setStyleSheet(gResLightGreen);
    ui->pushButton_help_light_off->setStyleSheet(gResLightRed);
    ui->pushButton_help_light->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_help_light_on->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_help_light_off->setFocusPolicy(Qt::NoFocus);

    for(int i = 0; i < MAIN_WAY_PARAM_NUM; i++)
    {
        lightBt[i]->setStyleSheet(gResLightGreen);
        lightBt[i]->setFocusPolicy(Qt::NoFocus);
    }


    {
        QString str_label_color = "color:yellow";
        ui->label->setStyleSheet(str_label_color);
        ui->label_10->setStyleSheet(str_label_color);
        ui->label_11->setStyleSheet(str_label_color);
        ui->label_12->setStyleSheet(str_label_color);
        ui->label_13->setStyleSheet(str_label_color);
        ui->label_14->setStyleSheet(str_label_color);
        ui->label_15->setStyleSheet(str_label_color);
        ui->label_17->setStyleSheet(str_label_color);
        ui->label_19->setStyleSheet(str_label_color);
        ui->label_2->setStyleSheet(str_label_color);
        ui->label_20->setStyleSheet(str_label_color);
        ui->label_21->setStyleSheet(str_label_color);
        ui->label_23->setStyleSheet(str_label_color);
        ui->label_25->setStyleSheet(str_label_color);
        ui->label_26->setStyleSheet(str_label_color);
        ui->label_27->setStyleSheet(str_label_color);
        ui->label_29->setStyleSheet(str_label_color);
        ui->label_3->setStyleSheet(str_label_color);
        ui->label_31->setStyleSheet(str_label_color);
        ui->label_32->setStyleSheet(str_label_color);
        ui->label_35->setStyleSheet(str_label_color);
        ui->label_36->setStyleSheet(str_label_color);
        ui->label_39->setStyleSheet(str_label_color);
        ui->label_4->setStyleSheet(str_label_color);
        ui->label_40->setStyleSheet(str_label_color);
        ui->label_44->setStyleSheet(str_label_color);
        ui->label_47->setStyleSheet(str_label_color);
        ui->label_48->setStyleSheet(str_label_color);
        ui->label_49->setStyleSheet(str_label_color);
        ui->label_5->setStyleSheet(str_label_color);
        ui->label_50->setStyleSheet(str_label_color);
        ui->label_9->setStyleSheet(str_label_color);
        ui->label_51->setStyleSheet(str_label_color);
    }

    ui->lineEdit_fuse->setVisible(false);
}

void CWarnSetDialog::SetLight(int ui_ind, bool is_on)
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

MU_INT32 *CWarnSetDialog::MapUi2Val(st_way_data *p_st_way_data, int ui_ind)
{
    MU_INT32* p_val = NULL;
    if(!p_st_way_data)
    {
        return p_val;
    }
    switch(ui_ind)
    {
    case 0:
        p_val = &p_st_way_data->way_threshold.u_down_limit[0] ;
        break;
    case 1:
        p_val = &p_st_way_data->way_threshold.u_up_limit[0] ;
        break;
    case 2:
        p_val = &p_st_way_data->way_threshold.i_down_limit[0] ;
        break;
    case 3:
        p_val = &p_st_way_data->way_threshold.i_up_limit[0] ;
        break;
    case 4:
        p_val = &p_st_way_data->way_threshold.lose_phase_limit[0] ;
        break;
    case 5:
        p_val = &p_st_way_data->way_threshold.i_zero_up_limit[0] ;
        break;
    case 6:
        p_val = &p_st_way_data->way_threshold.i_unbalance_up_limit[0] ;
        break;
    case 7:
        p_val = &p_st_way_data->way_threshold.u_harm_percent_up_limit[0] ;
        break;
    case 8:
        p_val = &p_st_way_data->way_threshold.i_down2_limit[0] ;
        break;
    case 9:
        p_val = &p_st_way_data->way_threshold.i_up2_limit[0] ;
        break;
    case 10:
        p_val = &p_st_way_data->way_threshold.f_up_limit[0] ;
        break;
    case 11:
        p_val = &p_st_way_data->way_threshold.f_down_limit[0] ;
        break;
    case 12:
        p_val = &p_st_way_data->way_threshold.fuse_limit[0] ;
        break;
    case 13:
        p_val = &p_st_way_data->way_threshold.i_harm_percent_limit[0] ;
        break;
    default:
        p_val = NULL;
        break;
    }
    return p_val;
}

MU_INT32 CWarnSetDialog::MapUi2WarnType(int ui_ind)
{
    MU_INT32 warn_type = -1;
    switch(ui_ind)
    {
    case 0:
        warn_type = E_U_DOWN_LIMIT;
        break;
    case 1:
        warn_type = E_U_UP_LIMIT;
        break;
    case 2:
        warn_type = E_I_DOWN_LIMIT;
        break;
    case 3:
        warn_type = E_I_UP_LIMIT;
        break;
    case 4:
        warn_type = E_LOSE_PHASE_LIMIT;
        break;
    case 5:
        warn_type = E_I_ZERO_UP_LIMIT;
        break;
    case 6:
        warn_type = E_I_UNBALANCE_UP_LIMIT;
        break;
    case 7:
        warn_type = E_U_HARM_PERCENT_UP_LIMIT;
        break;
    case 8:
        warn_type = E_I_DOWN2_LIMIT;
        break;
    case 9:
        warn_type = E_I_UP2_LIMIT;
        break;
    case 10:
        warn_type = E_F_UP_LIMIT;
        break;
    case 11:
        warn_type = E_F_DOWN_LIMIT;
        break;
    case 12:
        warn_type = E_FUSE_LIMIT;
        break;
    case 13:
        warn_type = E_I_HARM_PERCENT_LIMIT;
        break;
    default:
        break;
    }
    return warn_type;
}

void CWarnSetDialog::slot_write_threshold_finish(st_signal_parm* p_sig_parm)
{
    if(p_sig_parm->frm_sig != dialogInd)
    {
        return;
    }

    close_progress(pCProgressDialog);

//    static int cnt = 0;
//    qDebug("[%s] after close cnt=%d", __FUNCTION__, cnt++);
}
