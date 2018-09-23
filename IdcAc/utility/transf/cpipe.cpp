#include "cpipe.h"

CPipe gCpipe;

CPipe::CPipe()
{
    int ret = pipe(pipeFd);
    if(ret != 0)
    {
        MY_TRACE("pipe error!\n");
        exit(1);
    }

    //通过fcntl()修改为使得读管道数据具有非阻塞的特性
//    for(int i = 0; i < 2; i++)
//    {
//        int flag=fcntl(pipeFd[i],F_GETFL,0);
//        flag |= O_NONBLOCK;
//        if(fcntl(pipeFd[i],F_SETFL,flag) < 0)
//        {
//            perror("fcntl");
//            exit(1);
//        }
//    }
}

CPipe::~CPipe()
{

}

int CPipe::GetReadFd()
{
    return pipeFd[READ_FD];
}

int CPipe::GetWriteFd()
{
    return pipeFd[WRITE_FD];
}

int CPipe::ReadPipe(void *buf, size_t bytes)
{
    return read(gCpipe.GetReadFd(), buf, bytes);
}

int CPipe::WritePipe(const void *buf, size_t bytes)
{
    return write(gCpipe.GetWriteFd(), buf, bytes);
}
