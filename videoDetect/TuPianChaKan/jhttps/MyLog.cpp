#include "stdafx.h"

#include "MyLog.h"


static MS_CHAR s_LogFileNameTmp[255]="C:\\VideoDetectPicture.tmp";
static MS_CHAR s_LogFileName[255]="C:\\VideoDetectPicture.txt";
//add
static int FirstSetPath=1;

static FILE *sp_LogFile=NULL;
static MS_INT32 s_LogFileLength = 0;
static MS_INT32 write_log(MS_CHAR* pbuf,MS_INT32 len)
{
	if(FirstSetPath==1)
	{
		sprintf(s_LogFileNameTmp,"%s\\VideoDetectPicture.tmp",g_CurrentPath);
		sprintf(s_LogFileName,"%s\\VideoDetectPicture.txt",g_CurrentPath);
		FirstSetPath=0;
		printf("first set path %s\n",s_LogFileNameTmp);
	}

	MS_CHAR msg[DEF_MAX_CHARSTR_LENGHT2];
    if(sp_LogFile==NULL)
    {
        sp_LogFile = fopen(s_LogFileNameTmp,"a+");
        fseek(sp_LogFile,0,SEEK_END);
        s_LogFileLength = ftell(sp_LogFile);
        sprintf(msg,"\nOpen log file the length is %d\n",s_LogFileLength);
        LogPrintf(msg,DEF_MSG_PRINT,0);

    }

    if((s_LogFileLength+len)>DEF_BUFFER_LENGTH_1M)
    //if((s_LogFileLength+len)>(10240))
    {

        fclose(sp_LogFile);
        sp_LogFile = NULL;
		remove(s_LogFileName);
        rename(s_LogFileNameTmp,s_LogFileName);
        sprintf(msg,"\nOpen new log file\n");
        LogPrintf(msg,DEF_MSG_PRINT,0);

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
	{
        printf("%s",msg);
	}
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
    time_t   now;         //实例化time_t结构

    struct   tm     *timenow;         //实例化tm结构指针

    time(&now);

    //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now

    timenow   =   localtime(&now);

    sprintf(pBuf,"%04d-%02d-%02d %02d:%02d:%02d ",timenow->tm_year+1900,timenow->tm_mon+1,timenow->tm_mday,timenow->tm_hour,timenow->tm_min,timenow->tm_sec);

}
