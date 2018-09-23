#include "csocketunixqt.h"

CSocketUnixQt::CSocketUnixQt()
{
    pSocketLocal = new QLocalSocket();
}

CSocketUnixQt::~CSocketUnixQt()
{
    delete pSocketLocal;
}

int CSocketUnixQt::connect_service()
{
    pSocketLocal->connectToServer(serverIp);
    return 0;
}

int CSocketUnixQt::disconnect_service()
{
    pSocketLocal->disconnectFromServer();
    return 0;
}

int CSocketUnixQt::send_service(const char *p_data, int i_data)
{
    int write_len_cur = 1;
    int off_len = 0;
    while(write_len_cur > 0)
    {
        write_len_cur = pSocketLocal->write(p_data + off_len, i_data - off_len);
        off_len += write_len_cur;
        if(off_len >= i_data)
        {
            break;
        }
    }
    return (i_data - off_len);
}

int CSocketUnixQt::recv_service(char *p_data, int i_data)
{
    int off_len = 0;
    while(pSocketLocal->bytesAvailable())
    {
        off_len += pSocketLocal->read(p_data + off_len, i_data - off_len);
        if(off_len >= i_data)
        {
            break;
        }
    }
    return (i_data - off_len);
}

void CSocketUnixQt::SetServerIp(const char *ip)
{
    if (ip)
    {
        strcpy(serverIp,ip);
    }
}
