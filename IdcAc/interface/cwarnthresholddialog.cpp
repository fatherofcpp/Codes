#include "cwarnthresholddialog.h"
#include "ui_cwarnthresholddialog.h"

#include "CommonFunction.h"
#include "protocol/promodbus.h"

CWarnThresholdDialog::CWarnThresholdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CWarnThresholdDialog)
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

    connect(ui->pushButton_batch_change, SIGNAL(clicked()),
            this, SLOT(slot_batch_change_clicked()));
    connect(ui->pushButton_batch_set, SIGNAL(clicked()),
            this, SLOT(slot_batch_set_clicked()));
    connect(ui->pushButton_save_m, SIGNAL(signal_key_clicked(const st_bt_inf&)),
            this, SLOT(slot_save_m_clicked(const st_bt_inf&)));


    for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
    {
        lightBt[i]->SetCode(i);
        saveBt[i]->SetCode(i);
        connect(lightBt[i], SIGNAL(signal_key_clicked(const st_bt_inf&)),
                this, SLOT(slot_light_clicked(const st_bt_inf&)));
        connect(saveBt[i], SIGNAL(signal_key_clicked(const st_bt_inf&)),
                this, SLOT(slot_save_clicked(const st_bt_inf&)));
    }
}

CWarnThresholdDialog::~CWarnThresholdDialog()
{
    delete ui;
}

void CWarnThresholdDialog::SetDialogInd(int val)
{
    dialogInd = val;
    signalParm.frm_sig = dialogInd;
}

void CWarnThresholdDialog::slot_to_main_page_clicked()
{
    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_CONTENT
            ||  dialogInd == E_WARN_THRESHOLD_DIALOG_B_CONTENT
            )
    {
        bool is_update = true;
        emit signal_i_range_update(&signalParm, is_update);
    }
    emit signal_change_child_dialog(E_MAIN_PAGE_DIALOG);
}

void CWarnThresholdDialog::slot_pre_page_clicked()
{
    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_CONTENT
            ||  dialogInd == E_WARN_THRESHOLD_DIALOG_B_CONTENT
            )
    {
        bool is_update = true;
        emit signal_i_range_update(&signalParm, is_update);
    }
    if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_DOWN
            )
    {
        bool is_update = false;
        emit signal_i_range_update(&signalParm, is_update);
    }

    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_I_DOWN
            )
    {
        bool is_get = false;
        emit signal_is_get_threshold(&signalParm, is_get);
    }

    int ind = dialogInd;
    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_I_DOWN)
    {
        ind = E_WARN_SET_DIALOG_A;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_I_UP)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_I_DOWN;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_I_DOWN2)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_I_UP;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_I_UP2)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_I_DOWN2;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_FUSE)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_I_UP2;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_CONTENT)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_FUSE;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_DOWN)
    {
        ind = E_WARN_SET_DIALOG_B;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_UP)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_I_DOWN;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_DOWN2)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_I_UP;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_UP2)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_I_DOWN2;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_FUSE)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_I_UP2;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_CONTENT)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_FUSE;
    }
    emit signal_change_child_dialog(ind);
}

void CWarnThresholdDialog::slot_next_page_clicked()
{
    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_FUSE
            || dialogInd == E_WARN_THRESHOLD_DIALOG_B_FUSE
            )
    {
        bool is_update = false;
        emit signal_i_range_update(&signalParm, is_update);
    }
    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_CONTENT
            )
    {
        bool is_update = true;
        emit signal_i_range_update(&signalParm, is_update);
    }
    int ind = dialogInd;
    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_I_DOWN)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_I_UP;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_I_UP)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_I_DOWN2;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_I_DOWN2)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_I_UP2;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_I_UP2)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_FUSE;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_FUSE)
    {
        ind = E_WARN_THRESHOLD_DIALOG_A_CONTENT;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_CONTENT)
    {
        ind = E_WARN_SET_DIALOG_B;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_DOWN)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_I_UP;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_UP)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_I_DOWN2;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_DOWN2)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_I_UP2;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_UP2)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_FUSE;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_FUSE)
    {
        ind = E_WARN_THRESHOLD_DIALOG_B_CONTENT;
    }
    else if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_CONTENT)
    {
        return;
    }
    emit signal_change_child_dialog(ind);
}

void CWarnThresholdDialog::slot_batch_change_clicked()
{
    QString text = threEdit[0]->text();
    bool is_on = lightBt[0]->GetOnOff();
    for(int i = 0; i < SUB_WAY_PARAM_NUM; i ++)
    {
        threEdit[i]->setText(text);
        lightBt[i]->SetOnOff(is_on);
        SetLight(i, is_on);
    }
}

void CWarnThresholdDialog::slot_batch_set_clicked()
{
    slot_batch_change_clicked();

    int way_code = -1;
    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_CONTENT
            || dialogInd == E_WARN_THRESHOLD_DIALOG_B_CONTENT)
    {
        if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_CONTENT)
        {
            way_code = MAIN_A_CODE;
        }
        else
        {
            way_code = MAIN_B_CODE;
        }
        st_dev_inf* p_st_dev_inf = GetDevInfByCode(pOuterDev, way_code, E_DEV_ELECTRIC);
        if(!p_st_dev_inf)
        {
            return;
        }
        MU_INT8 dev_addr = p_st_dev_inf->dev_addr.id;

        int rtn = 0;
        VEC_INT16 vec_val;
        MU_INT16 val = 0;
        vec_val.clear();
        for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
        {
            val = threEdit[i]->text().toFloat() * gOneReg;
            vec_val.push_back(val);
        }

        MU_INT16 reg_addr = REG_RANGE_BEGIN;
        if(way_code == MAIN_B_CODE)
        {
            reg_addr = REG_RANGE_BEGIN + MAIN_B_OFF_ELECTRIC;
        }
        st_thread_param_i_range_all param_i_range_all;
        memset(&param_i_range_all, 0, sizeof(param_i_range_all));
        param_i_range_all.dev_addr = dev_addr;
        param_i_range_all.vec_val = vec_val;
        param_i_range_all.reg_addr = reg_addr;

        emit signal_write_i_range_all(&signalParm, &param_i_range_all);
        open_progress(pCProgressDialog,
                      GetTaskNameByType(E_WRITE_CURRENT_RANGE_ALL_CMD));
        rtn = signalParm.is_suc;

        messagebox(this, rtn);
        if(rtn != 0)
        {
            goto LEAVE;
        }
    }
    else
    {
        MU_INT32 enable = 0;
        if(lightBt[0]->GetOnOff())
        {
            enable = 1;
        }

        if(dialogInd ==  E_WARN_THRESHOLD_DIALOG_A_I_DOWN
                ||  dialogInd ==  E_WARN_THRESHOLD_DIALOG_A_I_UP
                ||  dialogInd ==  E_WARN_THRESHOLD_DIALOG_A_I_DOWN2
                ||  dialogInd ==  E_WARN_THRESHOLD_DIALOG_A_I_UP2
                ||  dialogInd ==  E_WARN_THRESHOLD_DIALOG_A_FUSE
                ||  dialogInd == E_WARN_THRESHOLD_DIALOG_A_LOSE_PHASE
                )
        {
            way_code = MAIN_A_CODE;
        }
        else
        {
            way_code = MAIN_B_CODE;
        }
        int dev_type = -1;
        if(dialogInd ==  E_WARN_THRESHOLD_DIALOG_A_FUSE
                || dialogInd ==  E_WARN_THRESHOLD_DIALOG_B_FUSE)
        {
            dev_type = E_DEV_SWITCH;
        }
        else
        {
            dev_type = E_DEV_ELECTRIC;
        }
        st_dev_inf* p_st_dev_inf = GetDevInfByCode(pOuterDev, way_code, dev_type);
        if(!p_st_dev_inf)
        {
            return;
        }
        MU_INT8 dev_addr = p_st_dev_inf->dev_addr.id;
        //sub
        st_threshold_item threshold_set;
        memset(&threshold_set, 0, sizeof(threshold_set));
        st_thread_param_threshold_set param_threshold_set;
        param_threshold_set.vec_threshold_item.clear();
        for(int light_ind = 0; light_ind < SUB_WAY_PARAM_NUM; light_ind++)
        {
            MU_INT32 thre = threEdit[light_ind]->text().toFloat() * gOneReg;
            memset(&threshold_set, 0, sizeof(threshold_set));
            threshold_set.dev_type = dev_type;
            threshold_set.is_enable = enable;
            threshold_set.thre_val = thre;
            threshold_set.warn_type = MapDialogInd2WarnType();
            threshold_set.way_code = way_code + 1 + light_ind;
            param_threshold_set.vec_threshold_item.push_back(threshold_set);
        }
        int ret = 0;

        emit signal_write_threshold(&signalParm, &param_threshold_set);
        open_progress(pCProgressDialog,
                      GetTaskNameByType(E_WRITE_THRESHOLD_CMD));
        ret = signalParm.is_suc;

        messagebox(this, ret);
        if(ret == 0)
        {
            for(int light_ind = 0; light_ind < SUB_WAY_PARAM_NUM; light_ind++)
            {
                st_way_data* p_st_way_data =
                        GetWayInfByCode(pOuterDev, way_code + 1 + light_ind, dev_type);
                if(p_st_way_data)
                {
                    MU_INT32 thre = threEdit[light_ind]->text().toFloat() * gOneReg;
                    MU_INT32* p_val = MapDialogInd2Val(p_st_way_data);
                    if(p_val)
                    {
                        p_val[0] = thre;
                        p_val[1] = enable;
                    }
                }
            }
        }
    }

LEAVE:
    return;
}

void CWarnThresholdDialog::slot_save_m_clicked(const st_bt_inf& pos)
{
    int ret = 0;
    int way_code = -1;
    switch(dialogInd)
    {
    case E_WARN_THRESHOLD_DIALOG_A_CONTENT:
        way_code = MAIN_A_CODE;
        break;
    case E_WARN_THRESHOLD_DIALOG_B_CONTENT:
        way_code = MAIN_B_CODE;
        break;
    default:
        break;
    }
    if(way_code < 0)
    {
        return;
    }

    st_dev_inf* p_st_dev_inf = GetDevInfByCode(pOuterDev, way_code, E_DEV_ELECTRIC);
    if(!p_st_dev_inf)
    {
        return;
    }
    MU_INT8 dev_addr = p_st_dev_inf->dev_addr.id;
    st_way_data* p_st_way_data = GetWayInfByCode(pOuterDev, way_code, E_DEV_ELECTRIC);
    if(p_st_way_data)
    {
        MU_INT16 thre = ui->lineEdit_m->text().toFloat() * gOneReg;
        MU_INT16 val = thre;
        MU_INT16 reg_addr = -1;
        if(way_code == MAIN_A_CODE)
        {
            reg_addr = REG_RANGE_M_1;
        }
        else if(way_code == MAIN_B_CODE)
        {
            reg_addr = REG_RANGE_M_2;
        }

        st_thread_param_i_range_one param_i_range_one;
        memset(&param_i_range_one, 0, sizeof(param_i_range_one));
        param_i_range_one.dev_addr = dev_addr;
        param_i_range_one.reg_addr = reg_addr;
        param_i_range_one.reg_val = val;

        emit signal_write_i_range_one(&signalParm, &param_i_range_one);
        open_progress(pCProgressDialog,
                      GetTaskNameByType(E_WRITE_CURRENT_RANGE_ONE_CMD));
        ret = signalParm.is_suc;

        messagebox(this, ret);
    }
    return;
}

void CWarnThresholdDialog::slot_light_clicked(const st_bt_inf &pos)
{
    int light_ind = pos.code;
    bool is_on_now = !pos.is_on;
    SetLight(light_ind, is_on_now);
}

void CWarnThresholdDialog::slot_save_clicked(const st_bt_inf &pos)
{
    int light_ind = pos.code;
    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_CONTENT
            || dialogInd == E_WARN_THRESHOLD_DIALOG_B_CONTENT)
    {
        int way_code = -1;
        int m_way_code = -1;
        switch(dialogInd)
        {
        case E_WARN_THRESHOLD_DIALOG_A_CONTENT:
            m_way_code = MAIN_A_CODE;
            way_code = MAIN_A_CODE + 1 + light_ind;
            break;
        case E_WARN_THRESHOLD_DIALOG_B_CONTENT:
            m_way_code = MAIN_B_CODE;
            way_code = MAIN_B_CODE + 1 + light_ind;
            break;
        default:
            break;
        }
        if(way_code < 0)
        {
            return;
        }
        st_dev_inf* p_st_dev_inf = GetDevInfByCode(pOuterDev, way_code, E_DEV_ELECTRIC);
        if(!p_st_dev_inf)
        {
            return;
        }
        MU_INT8 dev_addr = p_st_dev_inf->dev_addr.id;
        st_way_data* p_st_way_data = GetWayInfByCode(pOuterDev, way_code, E_DEV_ELECTRIC);
        if(p_st_way_data)
        {
            MU_INT16 thre = threEdit[light_ind]->text().toFloat() * gOneReg;
            MU_INT16 val = thre;
            MU_INT16 reg_addr = REG_RANGE_BEGIN + light_ind;
            if(m_way_code == MAIN_B_CODE)
            {
                reg_addr = REG_RANGE_BEGIN + light_ind + MAIN_B_OFF_ELECTRIC;
            }
            st_thread_param_i_range_one param_i_range_one;
            memset(&param_i_range_one, 0, sizeof(param_i_range_one));
            param_i_range_one.dev_addr = dev_addr;
            param_i_range_one.reg_addr = reg_addr;
            param_i_range_one.reg_val = val;
            int ret = 0;

            emit signal_write_i_range_one(&signalParm, &param_i_range_one);
            open_progress(pCProgressDialog,
                          GetTaskNameByType(E_WRITE_CURRENT_RANGE_ONE_CMD));
            ret = signalParm.is_suc;

            messagebox(this, ret);
        }
        return;
    }

    MU_INT32 enable = 0;
    if(lightBt[light_ind]->GetOnOff())
    {
        enable = 1;
    }

    int way_code = -1;
    switch(dialogInd)
    {
    case E_WARN_THRESHOLD_DIALOG_A_I_DOWN:
    case E_WARN_THRESHOLD_DIALOG_A_I_UP:
    case E_WARN_THRESHOLD_DIALOG_A_I_DOWN2:
    case E_WARN_THRESHOLD_DIALOG_A_I_UP2:
    case E_WARN_THRESHOLD_DIALOG_A_FUSE:
    case E_WARN_THRESHOLD_DIALOG_A_LOSE_PHASE:
        way_code = MAIN_A_CODE + 1 + light_ind;
        break;
    case E_WARN_THRESHOLD_DIALOG_B_I_DOWN:
    case E_WARN_THRESHOLD_DIALOG_B_I_UP:
    case E_WARN_THRESHOLD_DIALOG_B_I_DOWN2:
    case E_WARN_THRESHOLD_DIALOG_B_I_UP2:
    case E_WARN_THRESHOLD_DIALOG_B_FUSE:
    case E_WARN_THRESHOLD_DIALOG_B_LOSE_PHASE:
        way_code = MAIN_B_CODE + 1 + light_ind;
        break;
    default:
        break;
    }

    st_way_data* p_st_way_data = NULL;
    int dev_type = -1;
    if(dialogInd != E_WARN_THRESHOLD_DIALOG_A_FUSE
            && dialogInd != E_WARN_THRESHOLD_DIALOG_B_FUSE)
    {
        //电
        dev_type = E_DEV_ELECTRIC;
    }
    else
    {
        //开关
        dev_type = E_DEV_SWITCH;
    }
    p_st_way_data = GetWayInfByCode(pOuterDev, way_code, dev_type);
    if(p_st_way_data)
    {
        MU_INT32* p_val = MapDialogInd2Val(p_st_way_data);
        MU_INT32 thre = threEdit[light_ind]->text().toFloat() * gOneReg;
        if(p_val)
        {
            st_threshold_item threshold_set;
            memset(&threshold_set, 0, sizeof(threshold_set));
            threshold_set.dev_type = dev_type;
            threshold_set.is_enable = enable;
            threshold_set.thre_val = thre;
            threshold_set.warn_type = MapDialogInd2WarnType();
            threshold_set.way_code = way_code;
            st_thread_param_threshold_set param_threshold_set;
            param_threshold_set.vec_threshold_item.push_back(threshold_set);
            int ret = 0;

            emit signal_write_threshold(&signalParm, &param_threshold_set);
            open_progress(pCProgressDialog,
                          GetTaskNameByType(E_WRITE_THRESHOLD_CMD));
            ret = signalParm.is_suc;

            if(ret == 0)
            {
                p_val[0] = thre;
                p_val[1] = enable;
            }
        }
    }
}

void CWarnThresholdDialog::SetTitle(QString &str_title)
{
    ui->label_title->setText(str_title);

    if(dialogInd != E_WARN_THRESHOLD_DIALOG_A_CONTENT
            && dialogInd != E_WARN_THRESHOLD_DIALOG_B_CONTENT)
    {
        ui->label_m->setVisible(false);
        ui->lineEdit_m->setVisible(false);
        ui->pushButton_save_m->setVisible(false);
    }
    else
    {
        for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
        {
            lightBt[i]->setVisible(false);
        }
    }

    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_FUSE
            || dialogInd == E_WARN_THRESHOLD_DIALOG_B_FUSE)
    {
        for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
        {
            threEdit[i]->setVisible(false);
        }
        ui->pushButton_batch_change->setVisible(false);
        ui->pushButton_batch_set->setVisible(false);
        ui->textEdit_help->setVisible(false);
    }


    if(dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_DOWN ||
            dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_UP ||
            dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_DOWN2 ||
            dialogInd == E_WARN_THRESHOLD_DIALOG_B_I_UP2 ||
            dialogInd == E_WARN_THRESHOLD_DIALOG_B_FUSE ||
            dialogInd == E_WARN_THRESHOLD_DIALOG_B_LOSE_PHASE ||
            dialogInd == E_WARN_THRESHOLD_DIALOG_B_CONTENT)
    {
        for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
        {
            QString str = QString("B") + QString::number(i + 1);
            labelW[i]->setText(str);
        }
    }

    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_FUSE
            || dialogInd == E_WARN_THRESHOLD_DIALOG_B_FUSE)
    {
        int left = 0;
        int top = 0;
        int width = 0;
        int height = 0;
        int right = 0;
        for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
        {
            right = labelW[i]->geometry().right();
            left = right - 5;
            top = lightBt[i]->geometry().top();
            width = lightBt[i]->geometry().width();
            height = lightBt[i]->geometry().height();
            lightBt[i]->setGeometry(left, top, width, height);

            right = lightBt[i]->geometry().right();
            left = right + 2;
            top = saveBt[i]->geometry().top();
            width = saveBt[i]->geometry().width();
            height = saveBt[i]->geometry().height();
            saveBt[i]->setGeometry(left, top, width, height);
        }
    }
}

void CWarnThresholdDialog::SetUi(const st_dev_inf* p_dev_inf)
{
    int way_code = -1;
    int m_ind = -1;
    switch(dialogInd)
    {
    case E_WARN_THRESHOLD_DIALOG_A_CONTENT:
        way_code = MAIN_A_CODE;
        m_ind = 0;
        break;
    case E_WARN_THRESHOLD_DIALOG_B_CONTENT:
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
            int way_id = p_st_way_data->way_id;
            if(way_id == way_code)
            {
                //主路
                MU_INT16 thre = p_ac_meter->i_m_range[m_ind];
                ui->lineEdit_m->setText(Uint2DoubleQStr(thre, gOneUnit));
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
                MU_INT16 thre = p_ac_meter->i_range[sub_way_param_ind];
                threEdit[sub_way_param_ind]->setText(Uint2DoubleQStr(thre, gOneUnit));
            }
        }
    }
}

void CWarnThresholdDialog::SetOuterDev(str_outer_dev *outer_dev)
{
    pOuterDev = outer_dev;

    if(dialogInd == E_WARN_THRESHOLD_DIALOG_A_CONTENT
            || dialogInd == E_WARN_THRESHOLD_DIALOG_B_CONTENT)
    {
        return;
    }


    MU_INT32 thre = 0;
    bool is_on = true;
    MU_INT32 enable = 0;
    for(int i = 0; i < SUB_WAY_PARAM_NUM; i ++)
    {
        int way_code = -1;
        switch(dialogInd)
        {
        case E_WARN_THRESHOLD_DIALOG_A_I_DOWN:
        case E_WARN_THRESHOLD_DIALOG_A_I_UP:
        case E_WARN_THRESHOLD_DIALOG_A_I_DOWN2:
        case E_WARN_THRESHOLD_DIALOG_A_I_UP2:
        case E_WARN_THRESHOLD_DIALOG_A_FUSE:
        case E_WARN_THRESHOLD_DIALOG_A_LOSE_PHASE:
            way_code = MAIN_A_CODE + 1 + i;
            break;
        case E_WARN_THRESHOLD_DIALOG_B_I_DOWN:
        case E_WARN_THRESHOLD_DIALOG_B_I_UP:
        case E_WARN_THRESHOLD_DIALOG_B_I_DOWN2:
        case E_WARN_THRESHOLD_DIALOG_B_I_UP2:
        case E_WARN_THRESHOLD_DIALOG_B_FUSE:
        case E_WARN_THRESHOLD_DIALOG_B_LOSE_PHASE:
            way_code = MAIN_B_CODE + 1 + i;
            break;
        default:
            break;
        }

        if(dialogInd != E_WARN_THRESHOLD_DIALOG_A_FUSE
                && dialogInd != E_WARN_THRESHOLD_DIALOG_B_FUSE)
        {
            //电
            st_way_data* p_st_way_data = GetWayInfByCode(pOuterDev, way_code, E_DEV_ELECTRIC);
            if(p_st_way_data)
            {
                MU_INT32* p_val = MapDialogInd2Val(p_st_way_data);
                if(p_val)
                {
                    thre = p_val[0];
                    enable = p_val[1];
                    is_on = (enable != 0);
                    threEdit[i]->setText(Uint2DoubleQStr(thre, gOneUnit));
                    SetLight(i, is_on);
                }
            }
        }
        else
        {
            //开关
            st_way_data* p_st_way_data = GetWayInfByCode(pOuterDev, way_code, E_DEV_SWITCH);
            if(p_st_way_data)
            {
                MU_INT32* p_val = MapDialogInd2Val(p_st_way_data);
                if(p_val)
                {
                    enable = p_val[1];
                    is_on = (enable != 0);
                    SetLight(i, is_on);
                }
            }
        }
    }
}

void CWarnThresholdDialog::InitUi()
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
        saveBt[ind++] = ui->pushButton_save_w_1;
        saveBt[ind++] = ui->pushButton_save_w_2;
        saveBt[ind++] = ui->pushButton_save_w_3;
        saveBt[ind++] = ui->pushButton_save_w_4;
        saveBt[ind++] = ui->pushButton_save_w_5;
        saveBt[ind++] = ui->pushButton_save_w_6;
        saveBt[ind++] = ui->pushButton_save_w_7;
        saveBt[ind++] = ui->pushButton_save_w_8;
        saveBt[ind++] = ui->pushButton_save_w_9;
        saveBt[ind++] = ui->pushButton_save_w_10;
        saveBt[ind++] = ui->pushButton_save_w_11;
        saveBt[ind++] = ui->pushButton_save_w_12;
        saveBt[ind++] = ui->pushButton_save_w_13;
        saveBt[ind++] = ui->pushButton_save_w_14;
        saveBt[ind++] = ui->pushButton_save_w_15;
        saveBt[ind++] = ui->pushButton_save_w_16;
        saveBt[ind++] = ui->pushButton_save_w_17;
        saveBt[ind++] = ui->pushButton_save_w_18;
        saveBt[ind++] = ui->pushButton_save_w_19;
        saveBt[ind++] = ui->pushButton_save_w_20;
        saveBt[ind++] = ui->pushButton_save_w_21;
        saveBt[ind++] = ui->pushButton_save_w_22;
        saveBt[ind++] = ui->pushButton_save_w_23;
        saveBt[ind++] = ui->pushButton_save_w_24;
        saveBt[ind++] = ui->pushButton_save_w_25;
        saveBt[ind++] = ui->pushButton_save_w_26;
        saveBt[ind++] = ui->pushButton_save_w_27;
        saveBt[ind++] = ui->pushButton_save_w_28;
        saveBt[ind++] = ui->pushButton_save_w_29;
        saveBt[ind++] = ui->pushButton_save_w_30;

        ind = 0;
        threEdit[ind++] = ui->lineEdit_w_1;
        threEdit[ind++] = ui->lineEdit_w_2;
        threEdit[ind++] = ui->lineEdit_w_3;
        threEdit[ind++] = ui->lineEdit_w_4;
        threEdit[ind++] = ui->lineEdit_w_5;
        threEdit[ind++] = ui->lineEdit_w_6;
        threEdit[ind++] = ui->lineEdit_w_7;
        threEdit[ind++] = ui->lineEdit_w_8;
        threEdit[ind++] = ui->lineEdit_w_9;
        threEdit[ind++] = ui->lineEdit_w_10;
        threEdit[ind++] = ui->lineEdit_w_11;
        threEdit[ind++] = ui->lineEdit_w_12;
        threEdit[ind++] = ui->lineEdit_w_13;
        threEdit[ind++] = ui->lineEdit_w_14;
        threEdit[ind++] = ui->lineEdit_w_15;
        threEdit[ind++] = ui->lineEdit_w_16;
        threEdit[ind++] = ui->lineEdit_w_17;
        threEdit[ind++] = ui->lineEdit_w_18;
        threEdit[ind++] = ui->lineEdit_w_19;
        threEdit[ind++] = ui->lineEdit_w_20;
        threEdit[ind++] = ui->lineEdit_w_21;
        threEdit[ind++] = ui->lineEdit_w_22;
        threEdit[ind++] = ui->lineEdit_w_23;
        threEdit[ind++] = ui->lineEdit_w_24;
        threEdit[ind++] = ui->lineEdit_w_25;
        threEdit[ind++] = ui->lineEdit_w_26;
        threEdit[ind++] = ui->lineEdit_w_27;
        threEdit[ind++] = ui->lineEdit_w_28;
        threEdit[ind++] = ui->lineEdit_w_29;
        threEdit[ind++] = ui->lineEdit_w_30;

        for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
        {
            threEdit[i]->SetValid(E_VALID_FLOAT);
        }

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

    for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
    {
        lightBt[i]->setStyleSheet(gResLightGreen);
        lightBt[i]->setFocusPolicy(Qt::NoFocus);
    }

    QString str_label_color = "color:yellow";
    ui->label_title->setStyleSheet(str_label_color);
    ui->label_m->setStyleSheet(str_label_color);
    for(int i = 0; i < SUB_WAY_PARAM_NUM; i++)
    {
        labelW[i]->setStyleSheet(str_label_color);
        QString str = QString("A") + QString::number(i + 1);
        labelW[i]->setText(str);
    }

    ui->textEdit_help->setText(
                "输入支路1数值,按\"批量设置及\"确认\"按钮,\r\n后面各支路会按支路1进行批量设置"
                );
    ui->label_m->setText("主路");
}

void CWarnThresholdDialog::SetLight(int ui_ind, bool is_on)
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

MU_INT32 *CWarnThresholdDialog::MapDialogInd2Val(st_way_data *p_st_way_data)
{
    MU_INT32* p_val = NULL;
    if(!p_st_way_data)
    {
        return p_val;
    }

    switch(dialogInd)
    {
    case E_WARN_THRESHOLD_DIALOG_A_I_DOWN:
    case E_WARN_THRESHOLD_DIALOG_B_I_DOWN:
        p_val = &p_st_way_data->way_threshold.i_down_limit[0];
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_UP:
    case E_WARN_THRESHOLD_DIALOG_B_I_UP:
        p_val = &p_st_way_data->way_threshold.i_up_limit[0];
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_DOWN2:
    case E_WARN_THRESHOLD_DIALOG_B_I_DOWN2:
        p_val = &p_st_way_data->way_threshold.i_down2_limit[0];
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_UP2:
    case E_WARN_THRESHOLD_DIALOG_B_I_UP2:
        p_val = &p_st_way_data->way_threshold.i_up2_limit[0];
        break;
    case E_WARN_THRESHOLD_DIALOG_A_FUSE:
    case E_WARN_THRESHOLD_DIALOG_B_FUSE:
        p_val = &p_st_way_data->way_threshold.fuse_limit[0];
        break;
    case E_WARN_THRESHOLD_DIALOG_A_LOSE_PHASE:
    case E_WARN_THRESHOLD_DIALOG_B_LOSE_PHASE:
        p_val = &p_st_way_data->way_threshold.lose_phase_limit[0];
        break;
    default:
        p_val = NULL;
        break;
    }

    return p_val;
}

MU_INT32 CWarnThresholdDialog::MapDialogInd2WarnType()
{
    MU_INT32 warn_type = -1;

    switch(dialogInd)
    {
    case E_WARN_THRESHOLD_DIALOG_A_I_DOWN:
    case E_WARN_THRESHOLD_DIALOG_B_I_DOWN:
        warn_type = E_I_DOWN_LIMIT;
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_UP:
    case E_WARN_THRESHOLD_DIALOG_B_I_UP:
        warn_type = E_I_UP_LIMIT;
        break;
    case E_WARN_THRESHOLD_DIALOG_A_LOSE_PHASE:
    case E_WARN_THRESHOLD_DIALOG_B_LOSE_PHASE:
        warn_type = E_LOSE_PHASE_LIMIT;
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_DOWN2:
    case E_WARN_THRESHOLD_DIALOG_B_I_DOWN2:
        warn_type = E_I_DOWN2_LIMIT;
        break;
    case E_WARN_THRESHOLD_DIALOG_A_I_UP2:
    case E_WARN_THRESHOLD_DIALOG_B_I_UP2:
        warn_type = E_I_UP2_LIMIT;
        break;
    case E_WARN_THRESHOLD_DIALOG_A_FUSE:
    case E_WARN_THRESHOLD_DIALOG_B_FUSE:
        warn_type = E_FUSE_LIMIT;
        break;
    default:
        break;
    }
    return warn_type;
}

void CWarnThresholdDialog::slot_write_i_range_one_finish(st_signal_parm* p_sig_parm)
{
    if(p_sig_parm->frm_sig != dialogInd)
    {
        return;
    }

    close_progress(pCProgressDialog);

//    static int cnt = 0;
//    qDebug("[%s] after close cnt=%d", __FUNCTION__, cnt++);
}

void CWarnThresholdDialog::slot_write_i_range_all_finish(st_signal_parm* p_sig_parm)
{
    if(p_sig_parm->frm_sig != dialogInd)
    {
        return;
    }

    close_progress(pCProgressDialog);

//    static int cnt = 0;
//    qDebug("[%s] after close cnt=%d", __FUNCTION__, cnt++);
}

void CWarnThresholdDialog::slot_write_threshold_finish(st_signal_parm* p_sig_parm)
{
    if(p_sig_parm->frm_sig != dialogInd)
    {
        return;
    }

    close_progress(pCProgressDialog);

//    static int cnt = 0;
//    qDebug("[%s] after close cnt=%d", __FUNCTION__, cnt++);
}

