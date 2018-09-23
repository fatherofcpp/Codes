#ifndef CPIPE_H
#define CPIPE_H

#include <QDebug>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define MY_TRACE qDebug
#define READ_FD 0
#define WRITE_FD 1

class CPipe
{
public:
    CPipe();
    virtual ~CPipe();

public:
    int GetReadFd();
    int GetWriteFd();

    int ReadPipe(void* buf, size_t bytes);
    int WritePipe(const void* buf, size_t bytes);
private:
    int pipeFd[2];
};
extern CPipe gCpipe;
#endif // CPIPE_H
