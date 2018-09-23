#ifndef IPROTOCOL_H
#define IPROTOCOL_H
#include "include/mty.h"
#include <string>
using namespace std;
class IProtocol
{
public:
    IProtocol();
    virtual ~IProtocol();

    virtual string frame_format()=0;
    virtual int frame_parse()=0;
};

#endif // IPROTOCOL_H
