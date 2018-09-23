#ifndef CSOCKETLOCAL_H
#define CSOCKETLOCAL_H

#include "csockettcp.h"

class CSocketUnix :
        public CSocketTcp
{
public:
    CSocketUnix();
    virtual ~CSocketUnix();

    virtual int connect_service();
};

#endif // CSOCKETLOCAL_H
