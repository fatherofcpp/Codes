#ifndef CSOCKETUNIXQT_H
#define CSOCKETUNIXQT_H

#include "itransf.h"

class CSocketUnixQt :
        public ITransf
{
public:
    CSocketUnixQt();
    virtual ~CSocketUnixQt();

    virtual int connect_service();
    virtual int disconnect_service();
    virtual int send_service(const char *p_data, int i_data);
    virtual int recv_service(char *p_data, int i_data);

public:
    QLocalSocket * GetLocalSocket(){return pSocketLocal;}
    void SetServerIp(const char* ip);
private:
    QLocalSocket *pSocketLocal;
    char serverIp[32];
};

#endif // CSOCKETUNIXQT_H
