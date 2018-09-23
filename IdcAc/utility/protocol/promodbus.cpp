#include "promodbus.h"
#include "dataConvert.h"
#include "xmlAnalyze.h"
#include "csocketunix.h"
#include "Log.h"

CProModbus::CProModbus()
{
    frameLenMax = 12 * 1024;
    transfFrame = new MU_INT8[frameLenMax];
    ClearFrame();

    regBegin = 0;
    regEnd = 0;

    devAddr = 0;
    funCode = 0;
    regAddr = 0;
    crcParity = 0;
    replyLen = 0;
    regNum = 0;
    vecRegValWrite.clear();
    vecRegValRead.clear();
}

CProModbus::~CProModbus()
{
    delete[] transfFrame;
}

string CProModbus::frame_format()
{
    ClearFrame();
    if(funCode == FUN_REG_READ)
    {
        //读取
        int frame_ind = 0;//帧下标
        int pari_start = 0;//校验起始偏移
        int pari_end = 0;//校验结束偏移

        pari_start = frame_ind;
        transfFrame[frame_ind ++] = devAddr & 0xFF;
        transfFrame[frame_ind ++] = funCode;
        transfFrame[frame_ind ++] = (regAddr >> (8*1)) & 0xFF;//高字节在前
        transfFrame[frame_ind ++] = (regAddr >> (8*0)) & 0xFF;
        transfFrame[frame_ind ++] = (regNum >> (8*1)) & 0xFF;//高字节在前;
        transfFrame[frame_ind ++] = (regNum >> (8*0)) & 0xFF;
        pari_end = frame_ind;
        crcParity = ModbusCrc(&transfFrame[pari_start], (pari_end - pari_start));
        transfFrame[frame_ind ++] = ((crcParity >> (8*0)) & 0xFF);//crc校验,低字节在前
        transfFrame[frame_ind ++] = ((crcParity >> (8*1)) & 0xFF);
        frameLen = frame_ind;
    }
    else if(funCode == FUN_REG_WRITE_ONE)
    {
        //单寄存器写
        int frame_ind = 0;//帧下标
        int pari_start = 0;//校验起始偏移
        int pari_end = 0;//校验结束偏移

        pari_start = frame_ind;
        transfFrame[frame_ind ++] = devAddr & 0xFF;
        transfFrame[frame_ind ++] = funCode;
        transfFrame[frame_ind ++] = (regAddr >> (8*1)) & 0xFF;//高字节在前
        transfFrame[frame_ind ++] = (regAddr >> (8*0)) & 0xFF;
        transfFrame[frame_ind ++] = (vecRegValWrite[0] >> (8*1)) & 0xFF;//高字节在前;
        transfFrame[frame_ind ++] = (vecRegValWrite[0] >> (8*0)) & 0xFF;
        pari_end = frame_ind;
        crcParity = ModbusCrc(&transfFrame[pari_start], (pari_end - pari_start));
        transfFrame[frame_ind ++] = ((crcParity >> (8*0)) & 0xFF);//crc校验,低字节在前
        transfFrame[frame_ind ++] = ((crcParity >> (8*1)) & 0xFF);
        frameLen = frame_ind;
    }
    else if(funCode == FUN_REG_WRITE_MORE)
    {
        //多寄存器写
        int frame_ind = 0;//帧下标
        int pari_start = 0;//校验起始偏移
        int pari_end = 0;//校验结束偏移

        pari_start = frame_ind;
        transfFrame[frame_ind ++] = devAddr & 0xFF;
        transfFrame[frame_ind ++] = funCode;
        transfFrame[frame_ind ++] = (regAddr >> (8*1)) & 0xFF;//高字节在前
        transfFrame[frame_ind ++] = (regAddr >> (8*0)) & 0xFF;
        transfFrame[frame_ind ++] = (regNum >> (8*1)) & 0xFF;//高字节在前
        transfFrame[frame_ind ++] = (regNum >> (8*0)) & 0xFF;
        transfFrame[frame_ind ++] = regNum * 2;
        for(MU_INT8 i = 0; i < regNum; i++)
        {
            transfFrame[frame_ind ++] = (vecRegValWrite[i] >> (8*1)) & 0xFF;//高字节在前;
            transfFrame[frame_ind ++] = (vecRegValWrite[i] >> (8*0)) & 0xFF;
        }
        pari_end = frame_ind;
        crcParity = ModbusCrc(&transfFrame[pari_start], (pari_end - pari_start));
        transfFrame[frame_ind ++] = ((crcParity >> (8*0)) & 0xFF);//crc校验,低字节在前
        transfFrame[frame_ind ++] = ((crcParity >> (8*1)) & 0xFF);
        frameLen = frame_ind;
    }

    string str = GetFrameAsStr();
    return str;
}

int CProModbus::frame_parse()
{
    int ret = 0;
    vecRegValWrite.clear();
    if(funCode == FUN_REG_WRITE_ONE
            || funCode == FUN_REG_WRITE_MORE)
    {
        //write
        int frame_ind = 0;//帧下标
        int dev_addr = transfFrame[frame_ind ++];
        if(dev_addr != devAddr)
        {
            ret = -1;
            goto LEAVE;
        }
        int fun_code = transfFrame[frame_ind ++];
        if(fun_code != funCode)
        {
            ret = -1;
            goto LEAVE;
        }
    }

LEAVE:
    return ret;
}

int CProModbus::get_basic(u_dev_measure &dev_measure, const MU_INT8 dev_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();

    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_step = 0;
    MU_INT32 reg_remain = 0;
    MU_INT32 reg_begin = 0;
    MU_INT32 reg_end = 0;
    MU_INT32 reg_num = MAX_REG_NUM;

    //读量程,1-85reg
    ClearRegValRead();
    reg_begin = REG_BASIC_BEGIN;
    reg_end = REG_BASIC_END;
    reg_step = (reg_end - reg_begin) / reg_num;
    reg_remain = (reg_end - reg_begin) % reg_num;
    if(reg_remain != 0)
    {
        reg_step ++;
    }

    for(MU_INT32 i = 0; i < reg_step; i ++)
    {
        SetRegBegin(reg_begin + i * reg_num);
        if(reg_begin + i * reg_num + reg_num > reg_end)
        {
            SetRegEnd(reg_end);
        }
        else
        {
            SetRegEnd(reg_begin + i * reg_num + reg_num);
        }
        dtu_data = format_read_basic();

        len = gXmlAnalyze.FormDtuCmd(dtu_data);
        rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
        if(rtn == 0)
        {
            rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
            if(rtn == 0)
            {
//                gLog.Add("[%s]ParseDtuRsp dtu=[%s]", __FUNCTION__, dtu_data.c_str());
                rtn = AddFrameByDtuData(dtu_data);
            }
        }


        if(rtn != 0)
        {
            gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            goto LEAVE;
        }
    }

    rtn = parse_read_basic(dev_measure);
    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::get_line_main(u_dev_measure &dev_measure, const MU_INT8 dev_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();

    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_step = 0;
    MU_INT32 reg_remain = 0;
    MU_INT32 reg_begin = 0;
    MU_INT32 reg_end = 0;
    MU_INT32 reg_num = MAX_REG_NUM;

    //读量程,1-85reg
    ClearRegValRead();
    reg_begin = REG_LINE_MAIN_BEGIN;
    reg_end = REG_LINE_MAIN_END;
    reg_step = (reg_end - reg_begin) / reg_num;
    reg_remain = (reg_end - reg_begin) % reg_num;
    if(reg_remain != 0)
    {
        reg_step ++;
    }

    for(MU_INT32 i = 0; i < reg_step; i ++)
    {
        SetRegBegin(reg_begin + i * reg_num);
        if(reg_begin + i * reg_num + reg_num > reg_end)
        {
            SetRegEnd(reg_end);
        }
        else
        {
            SetRegEnd(reg_begin + i * reg_num + reg_num);
        }
        dtu_data = format_read_line_main();

        len = gXmlAnalyze.FormDtuCmd(dtu_data);
        rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
        if(rtn == 0)
        {
            rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
            if(rtn == 0)
            {
//                gLog.Add("[%s]ParseDtuRsp dtu=[%s]", __FUNCTION__, dtu_data.c_str());
                rtn = AddFrameByDtuData(dtu_data);
            }
        }


        if(rtn != 0)
        {
            gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            goto LEAVE;
        }
    }

    rtn = parse_read_line_main(dev_measure);
    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::get_line_sub_phase_one(u_dev_measure &dev_measure, const MU_INT8 dev_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();

    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_step = 0;
    MU_INT32 reg_remain = 0;
    MU_INT32 reg_begin = 0;
    MU_INT32 reg_end = 0;
    MU_INT32 reg_num = MAX_REG_NUM;

    //读量程,1-85reg
    ClearRegValRead();
    reg_begin = REG_LINE_SUB_PHASE_ONE_BEGIN;
    reg_end = REG_LINE_SUB_PHASE_ONE_END;
    reg_step = (reg_end - reg_begin) / reg_num;
    reg_remain = (reg_end - reg_begin) % reg_num;
    if(reg_remain != 0)
    {
        reg_step ++;
    }

    for(MU_INT32 i = 0; i < reg_step; i ++)
    {
        SetRegBegin(reg_begin + i * reg_num);
        if(reg_begin + i * reg_num + reg_num > reg_end)
        {
            SetRegEnd(reg_end);
        }
        else
        {
            SetRegEnd(reg_begin + i * reg_num + reg_num);
        }
        dtu_data = format_read_line_sub_phase_one();

        len = gXmlAnalyze.FormDtuCmd(dtu_data);
        rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
        if(rtn == 0)
        {
            rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
            if(rtn == 0)
            {
//                gLog.Add("[%s]ParseDtuRsp dtu=[%s]", __FUNCTION__, dtu_data.c_str());
                rtn = AddFrameByDtuData(dtu_data);
            }
        }


        if(rtn != 0)
        {
            gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            goto LEAVE;
        }
    }

    rtn = parse_read_line_sub_phase_one(dev_measure);
    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::get_line_sub_phase_three(u_dev_measure &dev_measure, const MU_INT8 dev_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();

    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_step = 0;
    MU_INT32 reg_remain = 0;
    MU_INT32 reg_begin = 0;
    MU_INT32 reg_end = 0;
    MU_INT32 reg_num = MAX_REG_NUM;

    //读量程,1-85reg
    ClearRegValRead();
    reg_begin = REG_LINE_SUB_PHASE_THREE_BEGIN;
    reg_end = REG_LINE_SUB_PHASE_THREE_END;
    reg_step = (reg_end - reg_begin) / reg_num;
    reg_remain = (reg_end - reg_begin) % reg_num;
    if(reg_remain != 0)
    {
        reg_step ++;
    }

    for(MU_INT32 i = 0; i < reg_step; i ++)
    {
        SetRegBegin(reg_begin + i * reg_num);
        if(reg_begin + i * reg_num + reg_num > reg_end)
        {
            SetRegEnd(reg_end);
        }
        else
        {
            SetRegEnd(reg_begin + i * reg_num + reg_num);
        }
        dtu_data = format_read_line_sub_phase_three();

        len = gXmlAnalyze.FormDtuCmd(dtu_data);
        rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
        if(rtn == 0)
        {
            rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
            if(rtn == 0)
            {
//                gLog.Add("[%s]ParseDtuRsp dtu=[%s]", __FUNCTION__, dtu_data.c_str());
                rtn = AddFrameByDtuData(dtu_data);
            }
        }


        if(rtn != 0)
        {
            gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            goto LEAVE;
        }
    }

    rtn = parse_read_line_sub_phase_three(dev_measure);
    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::get_harm(u_dev_measure &dev_measure, const MU_INT8 dev_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_step = 0;
    MU_INT32 reg_remain = 0;
    MU_INT32 reg_begin = 0;
    MU_INT32 reg_end = 0;
    MU_INT32 reg_num = MAX_REG_NUM;

    ClearRegValRead();
    reg_begin = REG_HARM_BEGIN;
    reg_end = REG_HARM_END;
    reg_step = (reg_end - reg_begin) / reg_num;
    reg_remain = (reg_end - reg_begin) % reg_num;
    if(reg_remain != 0)
    {
        reg_step ++;
    }

    for(MU_INT32 i = 0; i < reg_step; i ++)
    {
        SetRegBegin(reg_begin + i * reg_num);
        if(reg_begin + i * reg_num + reg_num > reg_end)
        {
            SetRegEnd(reg_end);
        }
        else
        {
            SetRegEnd(reg_begin + i * reg_num + reg_num);
        }
        dtu_data = format_read_harm();


        len = gXmlAnalyze.FormDtuCmd(dtu_data);
        rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
        if(rtn == 0)
        {
            rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
            if(rtn == 0)
            {
                rtn = AddFrameByDtuData(dtu_data);
            }
        }

        if(rtn != 0)
        {
            gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            goto LEAVE;
        }
    }
    rtn = parse_read_harm(dev_measure);
    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::get_event(u_dev_measure &dev_measure, const MU_INT8 dev_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();

    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_step = 0;
    MU_INT32 reg_remain = 0;
    MU_INT32 reg_begin = 0;
    MU_INT32 reg_end = 0;
    MU_INT32 reg_num = MAX_REG_NUM;

    //读量程,1-85reg
    ClearRegValRead();
    reg_begin = REG_EVENT_BEGIN;
    reg_end = REG_EVENT_END;
    reg_step = (reg_end - reg_begin) / reg_num;
    reg_remain = (reg_end - reg_begin) % reg_num;
    if(reg_remain != 0)
    {
        reg_step ++;
    }

    for(MU_INT32 i = 0; i < reg_step; i ++)
    {
        SetRegBegin(reg_begin + i * reg_num);
        if(reg_begin + i * reg_num + reg_num > reg_end)
        {
            SetRegEnd(reg_end);
        }
        else
        {
            SetRegEnd(reg_begin + i * reg_num + reg_num);
        }
        dtu_data = format_read_event();

        len = gXmlAnalyze.FormDtuCmd(dtu_data);
        rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
        if(rtn == 0)
        {
            rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
            if(rtn == 0)
            {
//                gLog.Add("[%s]ParseDtuRsp dtu=[%s]", __FUNCTION__, dtu_data.c_str());
                rtn = AddFrameByDtuData(dtu_data);
            }
        }


        if(rtn != 0)
        {
            gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            goto LEAVE;
        }
    }

    rtn = parse_read_event(dev_measure);
    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::get_switch(u_dev_measure &dev_measure, const MU_INT8 dev_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_step = 0;
    MU_INT32 reg_remain = 0;
    MU_INT32 reg_begin = 0;
    MU_INT32 reg_end = 0;
    MU_INT32 reg_num = MAX_REG_NUM;

    //读开关量,0-8reg
    ClearRegValRead();
    reg_begin = REG_SWITCH_BEGIN;
    reg_end = REG_SWITCH_END;
    reg_step = (reg_end - reg_begin) / reg_num;
    reg_remain = (reg_end - reg_begin) % reg_num;
    if(reg_remain != 0)
    {
        reg_step ++;
    }

    for(MU_INT32 i = 0; i < reg_step; i ++)
    {
        SetRegBegin(reg_begin + i * reg_num);
        if(reg_begin + i * reg_num + reg_num > reg_end)
        {
            SetRegEnd(reg_end);
        }
        else
        {
            SetRegEnd(reg_begin + i * reg_num + reg_num);
        }
        dtu_data = fomart_read_switch();


        len = gXmlAnalyze.FormDtuCmd(dtu_data);
        rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
        if(rtn == 0)
        {
            rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
            if(rtn == 0)
            {
//                gLog.Add("[%s]ParseDtuRsp dtu=[%s]", __FUNCTION__, dtu_data.c_str());
                rtn = AddFrameByDtuData(dtu_data);
            }
        }


        if(rtn != 0)
        {
            gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            goto LEAVE;
        }
    }

//    gLog.Add("[%s] before parse_read_range", __FUNCTION__);
    rtn = parse_read_switch(dev_measure);
//    gLog.Add("[%s] after parse_read_range", __FUNCTION__);
    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::set_range_one(const MU_INT16 val,
                              const MU_INT16 reg_addr,
                              const MU_INT8 dev_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_begin = 0;

    VEC_INT16 vec_reg_val;
    vec_reg_val.push_back(val);
    SetVecRegValWrite(vec_reg_val);

    SetFunCode(FUN_REG_WRITE_ONE);

    reg_begin = reg_addr;
    SetRegBegin(reg_begin);

    dtu_data = format_write_range();


    len = gXmlAnalyze.FormDtuCmd(dtu_data);
    rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
        if(rtn == 0)
        {
            rtn = SetFrameByStr(dtu_data);
            if(rtn == 0)
            {
                rtn = parse_write_range();
                if(rtn != 0)
                {
                    rtn = -2;
                }
            }
            else
            {
                rtn = -1;
            }
        }
    }

    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::set_range_all(const VEC_INT16& vec_val, const MU_INT8 dev_addr, const MU_INT16 reg_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_step = 0;
    MU_INT32 reg_remain = 0;
    MU_INT32 reg_begin = 0;
    MU_INT32 reg_end = 0;
    MU_INT32 reg_num = MAX_REG_NUM;

    SetVecRegValWrite(vec_val);
    SetFunCode(FUN_REG_WRITE_MORE);

    reg_begin = reg_addr;
    reg_end = reg_begin + vec_val.size();
    reg_step = (reg_end - reg_begin) / reg_num;
    reg_remain = (reg_end - reg_begin) % reg_num;
    if(reg_remain != 0)
    {
        reg_step ++;
    }

    for(MU_INT32 i = 0; i < reg_step; i ++)
    {
        SetRegBegin(reg_begin + i * reg_num);
        if(reg_begin + i * reg_num + reg_num > reg_end)
        {
            SetRegEnd(reg_end);
        }
        else
        {
            SetRegEnd(reg_begin + i * reg_num + reg_num);
        }
        dtu_data = format_write_range();


        len = gXmlAnalyze.FormDtuCmd(dtu_data);
        rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
        if(rtn == 0)
        {
            rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
            if(rtn == 0)
            {
                rtn = SetFrameByStr(dtu_data);
                if(rtn == 0)
                {
                    rtn = parse_write_range();
                    if(rtn != 0)
                    {
                        rtn = -2;
                    }
                }
            }
        }

        if(rtn != 0)
        {
            gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                     (char*)(gXmlAnalyze.GetBuf()));
            goto LEAVE;
        }
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::set_relay(const MU_INT16 val, const MU_INT8 dev_addr, const MU_INT16 reg_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    SetDevAddr(dev_addr);

    int rtn = 0;
    int len = 0;

    string dtu_data;
    MU_INT32 reg_begin = 0;

    VEC_INT16 vec_reg_val;
    vec_reg_val.push_back(val);
    SetVecRegValWrite(vec_reg_val);

    reg_begin = reg_addr;
    SetRegBegin(reg_begin);

    dtu_data = format_write_relay();


    len = gXmlAnalyze.FormDtuCmd(dtu_data);
    rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
        if(rtn == 0)
        {
            rtn = SetFrameByStr(dtu_data);
            if(rtn == 0)
            {
                rtn = parse_write_relay();
                if(rtn != 0)
                {
                    rtn = -1;
                }
            }
        }
    }

    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

int CProModbus::set_zero_energy(const MU_INT16 val, const MU_INT8 dev_addr)
{
//    gLog.Add("[%s] before doLock", __FUNCTION__);
    g_pTransf->doLock();
    SetDevAddr(dev_addr);
    int rtn = 0;
    int len = 0;

    string dtu_data;

    VEC_INT16 vec_reg_val;
    vec_reg_val.push_back(val);
    SetVecRegValWrite(vec_reg_val);
    SetRegBegin(REG_ZERO_ENERGY);

    dtu_data = format_write_zero_energy();


    len = gXmlAnalyze.FormDtuCmd(dtu_data);
    rtn = g_pTransf->SendReqAndReceiveBody(len, E_DTU_CMD);
    if(rtn == 0)
    {
        rtn = gXmlAnalyze.ParseDtuRsp(dtu_data);
        if(rtn == 0)
        {
            rtn = SetFrameByStr(dtu_data);
            if(rtn == 0)
            {
                rtn = parse_write_zero_energy();
                if(rtn != 0)
                {
                    rtn = -2;
                }
            }
            else
            {
                rtn = -1;
            }
        }
    }
    if(rtn != 0)
    {
        gLog.Add("[%s]Parse fail[%s]", __FUNCTION__,
                 (char*)(gXmlAnalyze.GetBuf()));
        goto LEAVE;
    }

LEAVE:
    g_pTransf->doUnLock();
//    gLog.Add("[%s] after doUnLock", __FUNCTION__);
    return rtn;
}

string CProModbus::format_read()
{
    string str;
    SetFunCode(FUN_REG_READ);

    SetRegAddr(regBegin);
    SetRegNum(regEnd - regBegin);
    str = frame_format();

    return str;
}

string CProModbus::format_read_basic()
{
    string str = format_read();
    return str;
}

int CProModbus::parse_read_basic(u_dev_measure &dev_measure)
{
    MU_INT32 vec_ind = 0;
    MU_INT32 byte_size = 0;
    MU_INT8 val_byte[4] = {0};//最高位在前
    MU_INT32 param_num = 0;
    MU_INT32 vec_off = 0;
    const MU_INT32 c_reg_size = 2;
    MU_INT32 reg_num = 0;

    param_num = 1;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.meter_addr = val;
    }
    vec_off += param_num * byte_size;

    param_num = 3;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.cur_time[i] = val;
    }
    vec_off += param_num * byte_size;

    param_num = 3 * 2;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);

        MU_INT32 param_ind = i % 2;
        MU_INT32 com_ind = i / 2;
        dev_measure.ac_meter.rs_485[com_ind][param_ind] = val;
    }
    vec_off += param_num * byte_size;

    param_num = 2;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.relay[i] = val;
    }
    vec_off += param_num * byte_size;

    param_num = 2;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.switch_on_ele[i] = val;
    }
    vec_off += param_num * byte_size;

    param_num = 2;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.v_range[i] = val;
    }
    vec_off += param_num * byte_size;

    param_num = 2;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.i_m_range[i] = val;
    }
    vec_off += param_num * byte_size;

    param_num = 84;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.i_range[i] = val;
    }
    vec_off += param_num * byte_size;

    param_num = 2;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.v_ratio[i] = val;
    }
    vec_off += param_num * byte_size;

    param_num = 2;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.i_ratio[i] = val;
    }
    vec_off += param_num * byte_size;

    param_num = 1;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.version = val;
    }
    vec_off += param_num * byte_size;

    return 0;
}

string CProModbus::format_read_line_main()
{
    string str = format_read();
    return str;
}

int CProModbus::parse_read_line_main(u_dev_measure &dev_measure)
{
    MU_INT32 vec_ind = 0;
    MU_INT32 byte_size = 0;
    MU_INT8 val_byte[4] = {0};//最高位在前
    MU_INT32 param_num = 0;
    MU_INT32 vec_off = 0;
    const MU_INT32 c_reg_size = 2;
    MU_INT32 reg_num = 0;

    for(int m_ind = 0; m_ind < 2; m_ind++)
    {
        param_num = 3;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].u_phase[i] = val;
        }
        vec_off += param_num * byte_size;

        param_num = 3;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].u_line[i] = val;
        }
        vec_off += param_num * byte_size;

        param_num = 1;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].frequence = val;
        }
        vec_off += param_num * byte_size;

        param_num = 4;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].i_phase[i] = val;
        }
        vec_off += param_num * byte_size;

        param_num = 2;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].power_total[i] = (MS_INT16)val;
        }
        vec_off += param_num * byte_size;

        param_num = 1;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].power_apparent = val;
        }
        vec_off += param_num * byte_size;

        param_num = 1;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].power_factor_total = (MS_INT16)val;
        }
        vec_off += param_num * byte_size;

        param_num = 2;
        reg_num = 2;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            val_byte[2] = vecRegValRead[vec_ind + 2];
            val_byte[3] = vecRegValRead[vec_ind + 3];
            MU_INT32 val = uint8_2_uint32(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].energy_total[i] = val;
        }
        vec_off += param_num * byte_size;

        param_num = 3 * 2;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            MU_INT32 phase_ind = i % 3;
            MU_INT32 param_ind = i / 3;
            dev_measure.ac_meter.inline_m_param[m_ind].power_phase[phase_ind][param_ind] = (MS_INT16)val;
        }
        vec_off += param_num * byte_size;

        param_num = 3;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].power_phase_apparent[i] = val;
        }
        vec_off += param_num * byte_size;

        param_num = 3;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.inline_m_param[m_ind].power_factor_phase[i] = (MS_INT16)val;
        }
        vec_off += param_num * byte_size;

        param_num = 3 * 2;
        reg_num = 2;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            val_byte[2] = vecRegValRead[vec_ind + 2];
            val_byte[3] = vecRegValRead[vec_ind + 3];
            MU_INT32 val = uint8_2_uint32(val_byte);
            MU_INT32 phase_ind = i % 3;
            MU_INT32 param_ind = i / 3;
            dev_measure.ac_meter.inline_m_param[m_ind].energy_phase[phase_ind][param_ind] = val;
        }
        vec_off += param_num * byte_size;
    }

    for(int m_ind = 0; m_ind < 2; m_ind++)
    {
        param_num = 2;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            MU_INT32 param_ind = i;
            dev_measure.ac_meter.inline_m_param[m_ind].unbanlance_phase[param_ind] = val;
        }
        vec_off += param_num * byte_size;
    }

    return 0;
}

string CProModbus::format_read_line_sub_phase_one()
{
    string str = format_read();
    return str;
}

int CProModbus::parse_read_line_sub_phase_one(u_dev_measure &dev_measure)
{
    MU_INT32 vec_ind = 0;
    MU_INT32 byte_size = 0;
    MU_INT8 val_byte[4] = {0};//最高位在前
    MU_INT32 param_num = 0;
    MU_INT32 vec_off = 0;
    const MU_INT32 c_reg_size = 2;
    MU_INT32 reg_num = 0;

    for(int sub_ind = 0; sub_ind < 84; sub_ind++)
    {
        param_num = 1;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.i_out_line[sub_ind] = val;
        }
        vec_off += param_num * byte_size;

        param_num = 3;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.power_out_line[i][sub_ind] = (MS_INT16)val;
        }
        vec_off += param_num * byte_size;

        param_num = 1;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.power_factor_out_line[sub_ind] = (MS_INT16)val;
        }
        vec_off += param_num * byte_size;

        param_num = 2;
        reg_num = 2;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            val_byte[2] = vecRegValRead[vec_ind + 2];
            val_byte[3] = vecRegValRead[vec_ind + 3];
            MU_INT32 val = uint8_2_uint32(val_byte);
            dev_measure.ac_meter.energy_out_line[i][sub_ind] = val;
        }
        vec_off += param_num * byte_size;
    }

    return 0;
}

string CProModbus::format_read_line_sub_phase_three()
{
    string str = format_read();
    return str;
}

int CProModbus::parse_read_line_sub_phase_three(u_dev_measure &dev_measure)
{
    MU_INT32 vec_ind = 0;
    MU_INT32 byte_size = 0;
    MU_INT8 val_byte[4] = {0};//最高位在前
    MU_INT32 param_num = 0;
    MU_INT32 vec_off = 0;
    const MU_INT32 c_reg_size = 2;
    MU_INT32 reg_num = 0;

    for(int sub_ind = 0; sub_ind < 28; sub_ind++)
    {
        param_num = 2;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.power_total_3_phase_out_line[i][sub_ind] = (MS_INT16)val;
        }
        vec_off += param_num * byte_size;

        param_num = 1;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.power_total_3_phase_apparent[sub_ind] = val;
        }
        vec_off += param_num * byte_size;

        param_num = 1;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.power_factor_total_3_phase_out_line[sub_ind] = (MS_INT16)val;
        }
        vec_off += param_num * byte_size;

        param_num = 2;
        reg_num = 2;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            val_byte[2] = vecRegValRead[vec_ind + 2];
            val_byte[3] = vecRegValRead[vec_ind + 3];
            MU_INT32 val = uint8_2_uint32(val_byte);
            dev_measure.ac_meter.energy_total_3_phase_out_line[i][sub_ind] = val;
        }
        vec_off += param_num * byte_size;
    }

    return 0;
}

string CProModbus::format_read_harm()
{
    string str = format_read();
    return str;
}

int CProModbus::parse_read_harm(u_dev_measure &dev_measure)
{
    MU_INT32 vec_ind = 0;
    MU_INT32 byte_size = 0;
    MU_INT8 val_byte[4] = {0};//最高位在前
    MU_INT32 param_num = 0;
    MU_INT32 vec_off = 0;
    const MU_INT32 c_reg_size = 2;
    MU_INT32 reg_num = 0;

    for(int m_ind = 0; m_ind < 2; m_ind++)
    {
        //A,B,C,合相 电压谐波分量
        param_num = 4;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.harm_param[m_ind].u_harm[i] = val;
        }
        vec_off += param_num * byte_size;

        //A,B,C,合相 电流谐波分量
        param_num = 4;
        reg_num = 1;
        byte_size = reg_num * c_reg_size;
        for(MU_INT32 i = 0; i < param_num; i ++)
        {
            vec_ind = i*byte_size + vec_off;
            val_byte[0] = vecRegValRead[vec_ind];
            val_byte[1] = vecRegValRead[vec_ind + 1];
            MU_INT16 val = uint8_2_uint16(val_byte);
            dev_measure.ac_meter.harm_param[m_ind].i_harm[i] = val;
        }
        vec_off += param_num * byte_size;

        for(int j = 0; j < 3; j++)
        {
            //UA,UB,UC 2-31次谐波含量百分比
            param_num = 30;
            reg_num = 1;
            byte_size = reg_num * c_reg_size;
            for(MU_INT32 i = 0; i < param_num; i ++)
            {
                vec_ind = i*byte_size + vec_off;
                val_byte[0] = vecRegValRead[vec_ind];
                val_byte[1] = vecRegValRead[vec_ind + 1];
                MU_INT16 val = uint8_2_uint16(val_byte);
                MU_INT32 phase_ind = j;
                MU_INT32 param_ind = i;
                dev_measure.ac_meter.harm_param[m_ind].u_harm_percent[phase_ind][param_ind] = val;
            }
            vec_off += param_num * byte_size;

            //UA,UB,UC 奇次，偶次，波峰系数,电话谐波波形因数
            param_num = 4;
            reg_num = 1;
            byte_size = reg_num * c_reg_size;
            for(MU_INT32 i = 0; i < param_num; i ++)
            {
                vec_ind = i*byte_size + vec_off;
                val_byte[0] = vecRegValRead[vec_ind];
                val_byte[1] = vecRegValRead[vec_ind + 1];
                MU_INT16 val = uint8_2_uint16(val_byte);
                MU_INT32 phase_ind = j;
                MU_INT32 param_ind = i;
                dev_measure.ac_meter.harm_param[m_ind].u_harm_ratio[phase_ind][param_ind] = val;
            }
            vec_off += param_num * byte_size;
        }

        for(int j = 0; j < 3; j++)
        {
            //IA,IB,IC 2-31次谐波含量百分比
            param_num = 30;
            reg_num = 1;
            byte_size = reg_num * c_reg_size;
            for(MU_INT32 i = 0; i < param_num; i ++)
            {
                vec_ind = i*byte_size + vec_off;
                val_byte[0] = vecRegValRead[vec_ind];
                val_byte[1] = vecRegValRead[vec_ind + 1];
                MU_INT16 val = uint8_2_uint16(val_byte);
                MU_INT32 phase_ind = j;
                MU_INT32 param_ind = i;
                dev_measure.ac_meter.harm_param[m_ind].i_harm_percent[phase_ind][param_ind] = val;
            }
            vec_off += param_num * byte_size;

            //IA,IB,IC 奇次，偶次， K系数
            param_num = 3;
            reg_num = 1;
            byte_size = reg_num * c_reg_size;
            for(MU_INT32 i = 0; i < param_num; i ++)
            {
                vec_ind = i*byte_size + vec_off;
                val_byte[0] = vecRegValRead[vec_ind];
                val_byte[1] = vecRegValRead[vec_ind + 1];
                MU_INT16 val = uint8_2_uint16(val_byte);
                MU_INT32 phase_ind = j;
                MU_INT32 param_ind = i;
                dev_measure.ac_meter.harm_param[m_ind].i_harm_ratio[phase_ind][param_ind] = val;
            }
            vec_off += param_num * byte_size;
        }

    }
    return 0;
}

string CProModbus::format_read_event()
{
    string str = format_read();
    return str;
}

int CProModbus::parse_read_event(u_dev_measure &dev_measure)
{
    MU_INT32 vec_ind = 0;
    MU_INT32 byte_size = 0;
    MU_INT8 val_byte[4] = {0};//最高位在前
    MU_INT32 param_num = 0;
    MU_INT32 vec_off = 0;
    const MU_INT32 c_reg_size = 2;
    MU_INT32 reg_num = 0;

    param_num = 1;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.u_err_power_negative = val;
    }
    vec_off += param_num * byte_size;

    //出线 1~16,17~32,33~48,49~64,65~80,81~84路功率是否为负
    param_num = 6;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.ac_meter.power_negative[i] = val;
    }
    vec_off += param_num * byte_size;

    return 0;
}

string CProModbus::fomart_read_switch()
{
    string str;
    SetFunCode(FUN_REG_READ);

    SetRegAddr(regBegin);
    SetRegNum(regEnd - regBegin);
    str = frame_format();

    return str;
}

int CProModbus::parse_read_switch(u_dev_measure &dev_measure)
{
    MU_INT32 vec_ind = 0;
    MU_INT32 byte_size = 0;
    MU_INT8 val_byte[8] = {0};//最高位在前
    MU_INT32 param_num = 0;
    MU_INT32 vec_off = 0;
    const MU_INT32 c_reg_size = 2;
    MU_INT32 reg_num = 0;
    //地址
    param_num = 1;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.switch_meter.meter_addr = val;
    }
    vec_off += param_num * byte_size;

    //波特校验
    param_num = 4;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.switch_meter.baud_parity[i] = val;
    }
    vec_off += param_num * byte_size;

    //版本号
    param_num = 1;
    reg_num = 1;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        val_byte[0] = vecRegValRead[vec_ind];
        val_byte[1] = vecRegValRead[vec_ind + 1];
        MU_INT16 val = uint8_2_uint16(val_byte);
        dev_measure.switch_meter.version = val;
    }
    vec_off += param_num * byte_size;

    //开关状态
    param_num = 1;
    reg_num = 3;
    byte_size = reg_num * c_reg_size;
    for(MU_INT32 i = 0; i < param_num; i ++)
    {
        vec_ind = i*byte_size + vec_off;
        MU_INT64 byte[8] = {0};
        byte[0] = vecRegValRead[vec_ind];
        byte[1] = vecRegValRead[vec_ind + 1];
        byte[2] = vecRegValRead[vec_ind + 2];
        byte[3] = vecRegValRead[vec_ind + 3];
        byte[4] = vecRegValRead[vec_ind + 4];
        byte[5] = vecRegValRead[vec_ind + 5];
        //开关量低字节在前
        MU_INT64 val =
                (MU_INT64)(byte[1] << (8*0))
                + (MU_INT64)(byte[0] << (8*1))
                + (MU_INT64)(byte[3] << (8*2))
                + (MU_INT64)(byte[2] << (8*3))
                + (MU_INT64)(byte[5] << (8*4))
                + (MU_INT64)(byte[4] << (8*5));
        dev_measure.switch_meter.switch_state = val;
    }
    vec_off += param_num * byte_size;

    return 0;
}

string CProModbus::format_write_range()
{
    string str;

    SetRegAddr(regBegin);
    SetRegNum(regEnd - regBegin);

    str = frame_format();
    return str;
}

int CProModbus::parse_write_range()
{
    int rtn = frame_parse();
    return rtn;
}

string CProModbus::format_write_relay()
{
    //进线继电器输出状态 reg 366
    string str;
    SetFunCode(FUN_REG_WRITE_ONE);
    SetRegAddr(regBegin);

    str = frame_format();
    return str;
}

int CProModbus::parse_write_relay()
{
    int rtn = frame_parse();
    return rtn;
}

string CProModbus::format_write_zero_energy()
{
    string str;
    SetFunCode(FUN_REG_WRITE_ONE);
    SetRegAddr(regBegin);

    str = frame_format();
    return str;
}

int CProModbus::parse_write_zero_energy()
{
    int rtn = frame_parse();
    return rtn;
}

void CProModbus::SetDevAddr(MU_INT8 dev_addr)
{
    devAddr = dev_addr;
}

void CProModbus::SetFunCode(MU_INT8 func_code)
{
    funCode = func_code;
}

void CProModbus::SetRegAddr(MU_INT16 reg_addr)
{
    regAddr = reg_addr;
}

void CProModbus::SetRegNum(MU_INT16 reg_num)
{
    if(reg_num >= 1 && reg_num <= MAX_REG_NUM)
    {
        regNum = reg_num;
    }
}

VEC_INT16 &CProModbus::GetVecRegValWrite()
{
    return vecRegValWrite;
}

void CProModbus::SetVecRegValWrite(const VEC_INT16 &vec_reg_val)
{
    vecRegValWrite = vec_reg_val;
}

MU_INT32 CProModbus::GetFrameLen()
{
    return frameLen;
}

string CProModbus::GetFrameAsStr()
{
    string str_frame;
    int frame_len = frameLenMax * 2;
    char* p_frame = new char[frame_len];
    memset(p_frame , 0, frame_len);
    int tmp_len = 0;
    for (MU_INT32 i = 0; i < GetFrameLen(); i++)
    {
        tmp_len += sprintf(p_frame + tmp_len, "%02x", transfFrame[i]);
    }
    str_frame = p_frame;
    delete p_frame;

    return str_frame;
}

int CProModbus::SetFrameByStr(string &str)
{
    const char* p_str = str.c_str();
    char tmp[3];
    memset(tmp, 0, sizeof(tmp));
    int len = str.length();
    if(len % 2 != 0 || len == 0)
    {
        return -1;
    }

    ClearFrame();
    int frame_ind = 0;
    for (int i = 0; i < len; i += 2)
    {
        memset(tmp, 0, sizeof(tmp));
        memcpy(tmp, &p_str[i], 2);
        MU_INT32 val = 0;
        sscanf(tmp, "%x", &val);
        transfFrame[frameLen + frame_ind++] = val;
    }
    frameLen += frame_ind;
    return 0;
}

int CProModbus::AddFrameByDtuData(string &str)
{
    int ret = -1;
    ret = SetFrameByStr(str);
    if(ret != 0)
    {
        gLog.Add("%s SetFrameByStr fail str=%s", __FUNCTION__, str.c_str());
        return ret;
    }

    if(funCode == FUN_REG_READ)
    {
        int frame_ind = 0;//帧下标
        int pari_start = 0;//校验起始偏移
        int pari_end = 0;//校验结束偏移

        pari_start = frame_ind;
        MU_INT8 dev_addr = transfFrame[frame_ind ++];
        if(dev_addr != devAddr)
        {
            gLog.Add("%s dev_addr != devAddr", __FUNCTION__);
            ret = -1;
            goto LEAVE;
        }
        MU_INT8 fun_code = transfFrame[frame_ind ++];
        if(fun_code != funCode)
        {
            gLog.Add("%s fun_code != funCode", __FUNCTION__);
            ret = -1;
            goto LEAVE;
        }
        MU_INT8 reply_len = transfFrame[frame_ind ++];
        MU_INT8 data_ind = frame_ind;

        frame_ind += reply_len;
        pari_end = frame_ind;
        crcParity = ModbusCrc(&transfFrame[pari_start], (pari_end - pari_start));
        MU_INT8 low_crc = transfFrame[frame_ind ++];
        MU_INT8 hign_crc = transfFrame[frame_ind ++];
        MU_INT16 crc = MU_INT16(low_crc) + MU_INT16(hign_crc << (8*1));
        if(crc != crcParity)
        {
            gLog.Add("%s crc != crcParity", __FUNCTION__);
            ret = -1;
            goto LEAVE;
        }

        for(int i = 0; i < reply_len; i++)
        {
            MU_INT8 reg_val = transfFrame[data_ind + i];
            vecRegValRead.push_back(reg_val);
        }
    }

    ret = 0;
LEAVE:
    return ret;
}

void CProModbus::SetRegBegin(MU_INT32 begin)
{
    regBegin = begin;
}

void CProModbus::SetRegEnd(MU_INT32 end)
{
    regEnd = end;
}

void CProModbus::ClearRegValRead()
{
    vecRegValRead.clear();
}


void CProModbus::ClearFrame()
{
    memset(transfFrame, 0, frameLenMax);
    frameLen = 0;
}
