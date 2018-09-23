#include "C2Htype.h"
#include <stdio.h>
#ifndef WIN32
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#endif
#include "../xml/include/xml.h"
#include "../xml/include/xmlnode.h"
#include "../xml/include/xmlparse.h"

#include "../include/dev_init.h"
#include "../include/maintsk.h"
#include "../include/parse_http.h"

HTTP_URL sHttpUrl[DEF_MAX_NO_SUPPORT];

static MS_CHAR *s_FileBuffer=NULL;
static MU_INT32 s_BufferLen = 0;


static MS_CHAR s_LogFileNameTmp[]="log_detection.tmp";
static MS_CHAR s_LogFileName[]="log_detection.txt";

static FILE *sp_LogFile=NULL;
static MS_INT32 s_LogFileLength = 0;
static MS_INT32 write_log(MS_CHAR* pbuf,MS_INT32 len)
{
	MS_CHAR msg[DEF_MAX_CHARSTR_LENGHT2];
    if(sp_LogFile==NULL)
    {
        sp_LogFile = fopen(s_LogFileNameTmp,"a+");
        fseek(sp_LogFile,0,SEEK_END);
        s_LogFileLength = ftell(sp_LogFile);
        sprintf(msg,"\nOpen log file the length is %d\n",s_LogFileLength);
        //LogPrintf(msg,DEF_MSG_PRINT,0);

    }

    if((s_LogFileLength+len)>DEF_BUFFER_LENGTH_1M)
    //if((s_LogFileLength+len)>(10240))
    {

        fclose(sp_LogFile);
        sp_LogFile = NULL;
        rename(s_LogFileNameTmp,s_LogFileName);
        sprintf(msg,"\nOpen new log file\n");
        //LogPrintf(msg,DEF_MSG_PRINT,0);

    }
        
        //FILE *pf = fopen("mylog.txt","a+");
    if(sp_LogFile)
    {
         s_LogFileLength += fwrite(pbuf,1,len,sp_LogFile);
        fflush(sp_LogFile);

     }

    return (0);
}
/*
MS_INT32 close_log(void)
{
    if(sp_LogFile!=NULL)
    {
        fclose(sp_LogFile);
        sp_LogFile = NULL;

    }
    return MD_OK;
}
*/
MS_INT32 LogPrintf(MS_CHAR *msg,MU_INT32 flag,MU_INT32 type)
{
    //BOARDINFO bi;
    MS_CHAR timeBuf[32];//,devName[32];

    if(flag&DEF_MSG_PRINT)
        printf("%s",msg);
    if(flag&DEF_MSG_SAVE)
    {
        memset(timeBuf,0,32);
        getTimeBuf(timeBuf);
        write_log(timeBuf,strlen(timeBuf));
        write_log(msg,strlen(msg));
    }
    return MD_OK;
}

void getTimeBuf(MS_CHAR *pBuf)
{
    time_t   now;         //ʵ����time_t�ṹ

    struct   tm     *timenow;         //ʵ����tm�ṹָ��

    time(&now);

    //time������ȡ���ڵ�ʱ��(���ʱ�׼ʱ��Ǳ���ʱ��)��Ȼ��ֵ��now

    timenow   =   localtime(&now);

    sprintf(pBuf,"%04d-%02d-%02d %02d:%02d:%02d ",timenow->tm_year+1900,timenow->tm_mon+1,timenow->tm_mday,timenow->tm_hour,timenow->tm_min,timenow->tm_sec);

}

