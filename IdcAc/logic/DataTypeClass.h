#pragma once
#include "DataTypeDef.h"
#include "CommonFunction.h"
#include "dataConvert.h"
class CTaskParam
{
public:
    CTaskParam(st_thread_tsk_param& thread_param)
    {
        tskResult = 0;
        tskEnd = false;
        memcpy(&threadParam, &thread_param, sizeof(threadParam));
    }

    ~CTaskParam()
    {

    }

    st_thread_tsk_param threadParam;//线程参数

    int tskResult;//任务执行返回值
    bool tskEnd;//线程轮询的判断
};//任务线程

