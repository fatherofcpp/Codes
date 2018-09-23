#ifndef ITRANSF_H
#define ITRANSF_H
#include "include_all_qt.h"

class ITransf
{
public:
    ITransf();
    virtual ~ITransf();

    virtual int connect_service()=0;
    virtual int disconnect_service()=0;
    virtual int send_service(const char *p_data, int i_data)=0;
    virtual int recv_service(char* p_data, int i_data)=0;

public:
    int SendReqAndReceiveBody(int send_len,int type);//串口，网络,需加锁
    void doLock();
    void doUnLock();
private:
    void ClearSendData();
private:
    pthread_mutex_t socketLock;
    int sendDataLen;
    char* sendData;//备份的发送数据
};
extern ITransf *g_pTransf;
extern bool isLogCommunicate;
#endif // ITRANSF_H
