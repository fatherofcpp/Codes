#include "realtimedatathread.h"
#include "include_all_qt.h"
#include "xmlAnalyze.h"
#include "Log.h"
#include "csocketunix.h"
#include "protocol/promodbus.h"
#include "report_data.h"
#include "csharemem.h"

static bool s_is_read_range = true;

static int GetRealData(u_dev_measure* p_u_dev_measure, const st_dev_inf* p_dev_inf);
static int test_real_data(st_dev_inf* p_dev_inf);
static int handle_dev_real_data(const st_dev_inf* p_dev_inf);

CRealTimeDataThread::CRealTimeDataThread(QObject *parent) :
    QThread(parent)
{
    isExit = false;
}

CRealTimeDataThread::~CRealTimeDataThread()
{

}

void CRealTimeDataThread::run()
{
    int sleep_real_delay_m = FRAME_DTU_DELAY_M;
    int sleep_err_m = 1000 * 2;

    int ret = 0;

    while(1)
    {
        if(isExit)
        {
            return;
        }

        Sleep(sleep_real_delay_m);

        str_outer_dev* p_str_outer_dev = gXmlAnalyze.GetOuterDev();
        int port_num = p_str_outer_dev->port_num;
        for(int port_ind = 0; port_ind < port_num; port_ind ++)
        {
            st_port_dev* p_st_port_dev = &p_str_outer_dev->port_dev[port_ind];
            int dev_num = p_st_port_dev->dev_num;
            for(int dev_ind = 0; dev_ind < dev_num; dev_ind ++)
            {
                st_dev_inf* p_dev_inf = &p_st_port_dev->dev_inf[dev_ind];
                MU_INT8 dev_id = p_dev_inf->dev_addr.id;
                //                gLog.Add("%s before GetRealData dev_id=%d,port_ind=%d",
                //                         __FUNCTION__, dev_id, port_ind);
                //                ret = test_real_data(p_dev_inf);
                ret = GetRealData(&p_dev_inf->dev_measure, p_dev_inf);
                //                gLog.Add("%s after GetRealData dev_id=%d,port_ind=%d",
                //                         __FUNCTION__, dev_id, port_ind);
                if(ret == 0)
                {
                    p_dev_inf->comm_state = true;
                    emit this->signal_realtime_data_update(p_dev_inf);
                    emit this->signal_communicate_state_update(p_dev_inf);
                    //                    gLog.Add("%s suc", __FUNCTION__);
                }
                else
                {
                    p_dev_inf->comm_state = false;
                    emit this->signal_communicate_state_update(p_dev_inf);
                    gLog.Add("%s fail", __FUNCTION__);
                    Sleep(sleep_err_m);
                }
            }//end dev_ind
        }//end port_ind

    }//end while
}

int GetRealData(u_dev_measure* p_u_dev_measure, const st_dev_inf* p_dev_inf)
{
    int rtn = 0;
    const int delay_for_sock_lock_m = 10;
    CProModbus* p_CProModbus = new CProModbus;
    MU_INT8 dev_addr = p_dev_inf->dev_addr.id;
    MU_INT32 dev_type = p_dev_inf->dev_type;

    if(dev_type == E_DEV_ELECTRIC)
    {
        rtn = p_CProModbus->get_basic(*p_u_dev_measure, dev_addr);
        if(rtn != 0)
        {
            rtn = -1;
            goto LEAVE;
        }
        Sleep(delay_for_sock_lock_m);
        //        gLog.Add("%s get_basic suc", __FUNCTION__);

        rtn = p_CProModbus->get_line_main(*p_u_dev_measure, dev_addr);
        if(rtn != 0)
        {
            rtn = -2;
            goto LEAVE;
        }
        Sleep(delay_for_sock_lock_m);
        //        gLog.Add("%s get_line_main suc", __FUNCTION__);

        rtn = p_CProModbus->get_line_sub_phase_one(*p_u_dev_measure, dev_addr);
        if(rtn != 0)
        {
            rtn = -3;
            goto LEAVE;
        }
        Sleep(delay_for_sock_lock_m);
        //        gLog.Add("%s get_line_sub_phase_one suc", __FUNCTION__);

        rtn = p_CProModbus->get_line_sub_phase_three(*p_u_dev_measure, dev_addr);
        if(rtn != 0)
        {
            rtn = -4;
            goto LEAVE;
        }
        Sleep(delay_for_sock_lock_m);
        //        gLog.Add("%s get_line_sub_phase_three suc", __FUNCTION__);

        rtn = p_CProModbus->get_harm(*p_u_dev_measure, dev_addr);
        if(rtn != 0)
        {
            rtn = -6;
            goto LEAVE;
        }
        Sleep(delay_for_sock_lock_m);
        //        gLog.Add("%s get_harm suc", __FUNCTION__);

        rtn = p_CProModbus->get_event(*p_u_dev_measure, dev_addr);
        if(rtn != 0)
        {
            rtn = -5;
            goto LEAVE;
        }
        Sleep(delay_for_sock_lock_m);
        //        gLog.Add("%s get_event suc", __FUNCTION__);
    }
    else if(dev_type == E_DEV_SWITCH)
    {
        //开关量0-8reg
        rtn = p_CProModbus->get_switch(*p_u_dev_measure, dev_addr);
        if(rtn != 0)
        {
            rtn = -7;
            goto LEAVE;
        }
        Sleep(delay_for_sock_lock_m);
        //        gLog.Add("%s get_switch suc", __FUNCTION__);
    }
    else
    {
        rtn = -8;
        goto LEAVE;
    }

    handle_dev_real_data(p_dev_inf);

LEAVE:
    delete p_CProModbus;
    return rtn;
}

int handle_dev_real_data(const st_dev_inf* p_dev_inf)
{
    static st_ac_reg ac_reg;
    static bool is_init = false;
    if(!is_init)
    {
        memset(&ac_reg, 0, sizeof(ac_reg));
        shm_init();
        is_init = true;
    }

    int rtn = 0;
    int dev_type = p_dev_inf->dev_type;

    if(dev_type == E_DEV_ELECTRIC)
    {
        const st_ac_meter* p_st_ac_meter = &p_dev_inf->dev_measure.ac_meter;
        st_ac_ml* p_st_ac_ml = NULL;
        for(int way_ind = 0; way_ind < MAX_WAY_NUM; way_ind++)
        {
            bool is_way_use = IsWayUsedByMask(way_ind,
                                              p_dev_inf->way_inf.way_mask);
            if(is_way_use)
            {
                const st_way_data* p_st_way_data = &p_dev_inf->way_inf.way_data[way_ind];
                int way_id = p_st_way_data->way_id;
                int report_ind = -1;
                int real_data_ind = -1;
                //支路
                if(way_id > MAIN_A_CODE && way_id <= MAIN_A_CODE + SUB_WAY_PARAM_NUM)
                {
                    report_ind = way_id - MAIN_A_CODE - 1;
                    real_data_ind = report_ind;
                    if(report_ind < REPORT_SUB_NUM)
                    {
                        ac_reg.sl_a_current[report_ind] = p_st_ac_meter->i_out_line[real_data_ind];    // B.3.2.13       /* 支路电流                */
                        ac_reg.sl_a_active1[report_ind] = p_st_ac_meter->power_out_line[0][real_data_ind];    // B.3.2.14       /* 支路有功功率            */
                        ac_reg.sl_a_active2[report_ind] = p_st_ac_meter->power_out_line[1][real_data_ind];    // B.3.2.15       /* 支路无功功率            */
                        ac_reg.sl_a_active3[report_ind] = p_st_ac_meter->power_out_line[2][real_data_ind];    // B.3.2.16       /* 支路视在功率            */
                        ac_reg.sl_a_factor[report_ind] = p_st_ac_meter->power_factor_out_line[real_data_ind];     // B.3.2.17       /* 支路功率因数            */
                        ac_reg.sl_a_combine1[report_ind] = p_st_ac_meter->energy_out_line[0][real_data_ind];   // B.3.2.18       /* 支路有功电能            */
                        ac_reg.sl_a_combine2[report_ind] = p_st_ac_meter->energy_out_line[1][real_data_ind];    // B.3.2.14 /* 支路功率*/
                    }
                }
                else if(way_id > MAIN_B_CODE && way_id <= MAIN_B_CODE + SUB_WAY_PARAM_NUM)
                {
                    report_ind = way_id - MAIN_B_CODE - 1;
                    report_ind += SUB_WAY_PARAM_NUM;
                    real_data_ind = way_id - MAIN_B_CODE - 1 + MAIN_B_OFF_ELECTRIC;

                    if(report_ind < REPORT_SUB_NUM)
                    {
                        ac_reg.sl_b_current[report_ind] = p_st_ac_meter->i_out_line[real_data_ind];    // B.3.2.13       /* 支路电流                */
                        ac_reg.sl_b_active1[report_ind] = p_st_ac_meter->power_out_line[0][real_data_ind];    // B.3.2.14       /* 支路有功功率            */
                        ac_reg.sl_b_active2[report_ind] = p_st_ac_meter->power_out_line[1][real_data_ind];    // B.3.2.15       /* 支路无功功率            */
                        ac_reg.sl_b_active3[report_ind] = p_st_ac_meter->power_out_line[2][real_data_ind];    // B.3.2.16       /* 支路视在功率            */
                        ac_reg.sl_b_factor[report_ind] = p_st_ac_meter->power_factor_out_line[real_data_ind];     // B.3.2.17       /* 支路功率因数            */
                        ac_reg.sl_b_combine1[report_ind] = p_st_ac_meter->energy_out_line[0][real_data_ind];   // B.3.2.18       /* 支路有功电能            */
                        ac_reg.sl_b_combine2[report_ind] = p_st_ac_meter->energy_out_line[1][real_data_ind];
                    }
                }
                else if(way_id == MAIN_A_CODE || way_id == MAIN_B_CODE)
                {
                    //主路
                    int m_ind = 0;
                    if(way_id == MAIN_A_CODE)
                    {
                        p_st_ac_ml = &ac_reg.ml_a;
                        m_ind = 0;
                    }
                    else if(way_id == MAIN_B_CODE)
                    {
                        p_st_ac_ml = &ac_reg.ml_b;
                        m_ind = 1;
                    }
                    p_st_ac_ml->Avolt = p_st_ac_meter->inline_m_param[m_ind].u_phase[0];   /* 主路的A相电压            */
                    p_st_ac_ml->Bvolt = p_st_ac_meter->inline_m_param[m_ind].u_phase[1];   /* 主路的B相电压            */
                    p_st_ac_ml->Cvolt = p_st_ac_meter->inline_m_param[m_ind].u_phase[2];   /* 主路的C相电压            */
                    p_st_ac_ml->ABvolt = p_st_ac_meter->inline_m_param[m_ind].u_line[0];   /* 主路的AB线电压           */
                    p_st_ac_ml->BCvolt = p_st_ac_meter->inline_m_param[m_ind].u_line[1];   /* 主路的BC线电压           */
                    p_st_ac_ml->CAvolt = p_st_ac_meter->inline_m_param[m_ind].u_line[2];   /* 主路的CA线电压           */
                    p_st_ac_ml->Acurrent = p_st_ac_meter->inline_m_param[m_ind].i_phase[0];   /* 主路的A相电流            */
                    p_st_ac_ml->Bcurrent = p_st_ac_meter->inline_m_param[m_ind].i_phase[1];   /* 主路的B相电流            */
                    p_st_ac_ml->Ccurrent = p_st_ac_meter->inline_m_param[m_ind].i_phase[2];   /* 主路的C相电流            */
                    p_st_ac_ml->Ncurrent = p_st_ac_meter->inline_m_param[m_ind].i_phase[3];   /* 主路的N相电流            */
                    p_st_ac_ml->Tcombine1 = p_st_ac_meter->inline_m_param[m_ind].energy_total[0];   /* 主路总有功总电能         */
                    p_st_ac_ml->Tcombine2 = p_st_ac_meter->inline_m_param[m_ind].energy_total[1];   /* 主路总无功总电能         */
                    p_st_ac_ml->Acombine1 = p_st_ac_meter->inline_m_param[m_ind].energy_phase[0][0];   /* 主路的A相有功电能        */
                    p_st_ac_ml->Bcombine1 = p_st_ac_meter->inline_m_param[m_ind].energy_phase[1][0];   /* 主路的B相有功电能        */
                    p_st_ac_ml->Ccombine1 = p_st_ac_meter->inline_m_param[m_ind].energy_phase[2][0];   /* 主路的C相有功电能        */
                    p_st_ac_ml->Acombine2 = p_st_ac_meter->inline_m_param[m_ind].energy_phase[0][1];   /* 主路的A相无功电能        */
                    p_st_ac_ml->Bcombine2 = p_st_ac_meter->inline_m_param[m_ind].energy_phase[1][1];   /* 主路的B相无功电能        */
                    p_st_ac_ml->Ccombine2 = p_st_ac_meter->inline_m_param[m_ind].energy_phase[2][1];   /* 主路的C相无功电能        */
                    p_st_ac_ml->Tactive1 = p_st_ac_meter->inline_m_param[m_ind].power_total[0];   /* 主路总有功功率           */
                    p_st_ac_ml->Tactive2 = p_st_ac_meter->inline_m_param[m_ind].power_total[1];   /* 主路总无功功率           */
                    p_st_ac_ml->Tactive3 = p_st_ac_meter->inline_m_param[m_ind].power_apparent;   /* 主路总视在功率           */
                    p_st_ac_ml->factor = p_st_ac_meter->inline_m_param[m_ind].power_factor_total;   /* 主路总功率因数           */
                    p_st_ac_ml->Aactive1 = p_st_ac_meter->inline_m_param[m_ind].power_phase[0][0];   /* 主路的A相有功功率        */
                    p_st_ac_ml->Bactive1 = p_st_ac_meter->inline_m_param[m_ind].power_phase[1][0];   /* 主路的B相有功功率        */
                    p_st_ac_ml->Cactive1 = p_st_ac_meter->inline_m_param[m_ind].power_phase[2][0];   /* 主路的C相有功功率        */
                    p_st_ac_ml->Aactive2 = p_st_ac_meter->inline_m_param[m_ind].power_phase[0][1];   /* 主路的A相无功功率        */
                    p_st_ac_ml->Bactive2 = p_st_ac_meter->inline_m_param[m_ind].power_phase[1][1];   /* 主路的B相无功功率        */
                    p_st_ac_ml->Cactive2 = p_st_ac_meter->inline_m_param[m_ind].power_phase[2][1];   /* 主路的C相无功功率        */
                    p_st_ac_ml->Aactive3 = p_st_ac_meter->inline_m_param[m_ind].power_phase_apparent[0];   /* 主路的A相视在功率        */
                    p_st_ac_ml->Bactive3 = p_st_ac_meter->inline_m_param[m_ind].power_phase_apparent[1];   /* 主路的B相视在功率        */
                    p_st_ac_ml->Cactive3 = p_st_ac_meter->inline_m_param[m_ind].power_phase_apparent[2];   /* 主路的C相视在功率        */
                    p_st_ac_ml->Afactor = p_st_ac_meter->inline_m_param[m_ind].power_factor_phase[0];   /* 主路的A相功率因数        */
                    p_st_ac_ml->Bfactor = p_st_ac_meter->inline_m_param[m_ind].power_factor_phase[1];   /* 主路的B相功率因数        */
                    p_st_ac_ml->Cfactor = p_st_ac_meter->inline_m_param[m_ind].power_factor_phase[2];   /* 主路的C相功率因数        */
                    p_st_ac_ml->frequency = p_st_ac_meter->inline_m_param[m_ind].frequence;   /* 主路的频率               */
                    p_st_ac_ml->Switch =
                            (unsigned short)((p_st_ac_meter->switch_on_ele[m_ind]));   /* 主路的开关量输入状态     */
                    p_st_ac_ml->Relays = p_st_ac_meter->relay[m_ind];   /* 主路的继电器输出状态     */
                    p_st_ac_ml->Avoltharm = p_st_ac_meter->harm_param[m_ind].u_harm[0];   /* 主路的A相电压谐波百分比  */
                    p_st_ac_ml->Bvoltharm = p_st_ac_meter->harm_param[m_ind].u_harm[1];   /* 主路的B相电压谐波百分比  */
                    p_st_ac_ml->Cvoltharm = p_st_ac_meter->harm_param[m_ind].u_harm[2];   /* 主路的C相电压谐波百分比  */
                    p_st_ac_ml->Acurrentharm = p_st_ac_meter->harm_param[m_ind].i_harm[0];   /* 主路的A相电流谐波百分比  */
                    p_st_ac_ml->Bcurrentharm = p_st_ac_meter->harm_param[m_ind].i_harm[1];   /* 主路的B相电流谐波百分比  */
                    p_st_ac_ml->Ccurrentharm = p_st_ac_meter->harm_param[m_ind].i_harm[2];   /* 主路的C相电流谐波百分比  */
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
            if(is_way_use)
            {
                const st_way_data* p_st_way_data = &p_dev_inf->way_inf.way_data[way_ind];
                const st_switch_meter* p_switch_meter = &p_dev_inf->dev_measure.switch_meter;
                int way_id = p_st_way_data->way_id;
                int report_ind = -1;
                int real_data_ind = -1;
                if(way_id > MAIN_A_CODE && way_id <= MAIN_A_CODE + SUB_WAY_PARAM_NUM)
                {
                    //支路
                    report_ind = way_id - MAIN_A_CODE - 1;
                    real_data_ind = report_ind;

                    if(report_ind < REPORT_SUB_NUM)
                    {
                        int off_sub = real_data_ind;
                        MU_INT64 switch_state = p_switch_meter->switch_state;
                        MU_INT64 src_val = 0x01;
                        MU_INT8 state = (MU_INT8)((switch_state >> off_sub) & src_val);
                        ac_reg.sl_a_switch[report_ind] = state;
                    }
                }
                else if(way_id > MAIN_B_CODE && way_id <= MAIN_B_CODE + SUB_WAY_PARAM_NUM)
                {
                    report_ind = way_id - MAIN_B_CODE - 1;
                    real_data_ind = report_ind;

                    if(report_ind < REPORT_SUB_NUM)
                    {
                        int off_sub = real_data_ind;
                        MU_INT64 switch_state = p_switch_meter->switch_state;
                        MU_INT64 src_val = 0x01;
                        MU_INT8 state = (MU_INT8)((switch_state >> off_sub) & src_val);
                        ac_reg.sl_b_switch[report_ind] = state;
                    }
                }
            }
        }
    }
    else
    {
        rtn = -1;
        goto LEAVE;
    }

    //    qDebug("%s sl_a_rel=%d",
    //           __FUNCTION__, ac_reg.ml_a.Relays);
    //    qDebug("%s sl_b_rel=%d",
    //           __FUNCTION__, ac_reg.ml_b.Relays);

    //report
    rtn = shm_write(&ac_reg);
LEAVE:
    return rtn;
}

int test_real_data(st_dev_inf *p_dev_inf)
{
    //    return -1;

    static MU_INT32 enter_cnt = 0;
    if(++enter_cnt >= 128)
    {
        enter_cnt = 0;
    }
    MU_INT32 add_off = enter_cnt % 16;

    sleep(1);
    Sleep(FRAME_DTU_DELAY_M);
    st_ac_meter* p_st_ac_meter = &p_dev_inf->dev_measure.ac_meter;
    for(int i = 0; i < 84; i++)
    {
        p_st_ac_meter->i_range[i] = gOneReg*(i+100+add_off);
    }
    for(int m_ind = 0; m_ind < 2; m_ind++)
    {
        for(int i = 0; i < 3; i++)
        {
            p_st_ac_meter->inline_m_param[m_ind].u_phase[i] = gOneReg*(i+51+add_off);
        }
        for(int i = 0; i < 3; i++)
        {
            p_st_ac_meter->inline_m_param[m_ind].u_line[i] = gOneReg*(i+1+add_off);
        }
        p_st_ac_meter->inline_m_param[m_ind].frequence = gTwoReg*10;

        for(int i = 0; i < 4; i++)
        {
            p_st_ac_meter->inline_m_param[m_ind].i_phase[i] = gThreeReg*(i+71+add_off);
        }

        for(int i = 0; i < 2; i++)
        {
            p_st_ac_meter->inline_m_param[m_ind].power_total[i] = gThreeReg*(i+1+add_off);
        }
        p_st_ac_meter->inline_m_param[m_ind].power_apparent = gThreeReg*(1+add_off);
        p_st_ac_meter->inline_m_param[m_ind].power_factor_total = gFourReg*(1+add_off);


        for(int i = 0; i < 2; i++)
        {
            p_st_ac_meter->inline_m_param[m_ind].energy_total[i] = gTwoReg*(i+1+add_off);
        }

        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                p_st_ac_meter->inline_m_param[m_ind].power_phase[i][j] = gThreeReg*(i+1+add_off);
            }
        }
        for(int i = 0; i < 3; i++)
        {
            p_st_ac_meter->inline_m_param[m_ind].power_phase_apparent[i] = gThreeReg*(i+1+add_off);
        }

        for(int i = 0; i < 3; i++)
        {
            p_st_ac_meter->inline_m_param[m_ind].power_factor_phase[i] = gFourReg*(i+1+add_off);
        }

        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                p_st_ac_meter->inline_m_param[m_ind].energy_phase[i][j] = gTwoReg*(i+1+add_off);
            }
        }
        for(int i = 0; i < 2; i++)
        {
            p_st_ac_meter->inline_m_param[m_ind].unbanlance_phase[i] = gTwoReg*(i+1+add_off);
        }
    }

    for(int i = 0; i < 84; i++)
    {
        p_st_ac_meter->i_out_line[i] = gTwoReg*(i+1+add_off);
    }

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 84; j++)
        {
            p_st_ac_meter->power_out_line[i][j] = gTwoReg*(i+1+add_off);
        }
    }

    for(int i = 0; i < 84; i++)
    {
        p_st_ac_meter->power_factor_out_line[i] = gTwoReg*(i+1+add_off);
    }

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 84; j++)
        {
            p_st_ac_meter->energy_out_line[i][j] = gTwoReg*(i+1+add_off);
        }
    }

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 28; j++)
        {
            p_st_ac_meter->power_total_3_phase_out_line[i][j] = gTwoReg*(i+1+add_off);
        }
    }
    for(int i = 0; i < 28; i++)
    {
        p_st_ac_meter->power_total_3_phase_apparent[i] = gTwoReg*(i+1+add_off);
    }
    for(int i = 0; i < 28; i++)
    {
        p_st_ac_meter->power_factor_total_3_phase_out_line[i] = gTwoReg*(i+1+add_off);
    }

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 28; j++)
        {
            p_st_ac_meter->energy_total_3_phase_out_line[i][j] = gTwoReg*(i+1+add_off);
        }
    }

    for(int m_ind = 0; m_ind < 2; m_ind++)
    {
        for(int i = 0; i < 4; i++)
        {
            p_st_ac_meter->harm_param[m_ind].u_harm[i] = gTwoReg*(i+1+add_off);
        }
        for(int i = 0; i < 4; i++)
        {
            p_st_ac_meter->harm_param[m_ind].i_harm[i] = gTwoReg*(i+1+add_off);
        }
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 30; j++)
            {
                p_st_ac_meter->harm_param[m_ind].u_harm_percent[i][j] = gTwoReg*(i*10+j+add_off);
            }
        }

        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                p_st_ac_meter->harm_param[m_ind].u_harm_ratio[i][j] = gTwoReg*(i+1+add_off);
            }
        }
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 30; j++)
            {
                p_st_ac_meter->harm_param[m_ind].i_harm_percent[i][j] = gTwoReg*(i*10+j+add_off);
            }
        }
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                p_st_ac_meter->harm_param[m_ind].i_harm_ratio[i][j] = gTwoReg*(i+1+add_off);
            }
        }
    }
    return 0;
}

void CRealTimeDataThread::slot_exit_thread()
{
    isExit = true;
}


