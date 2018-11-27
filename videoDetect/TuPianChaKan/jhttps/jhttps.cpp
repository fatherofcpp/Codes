// jhttps.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "in.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//add log
#include "MyLog.h"

//end log

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include<afxwin.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include<string>
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

extern "C"
{
#include "event2/event.h"
#include "event2/http.h"
#include "event2/buffer.h"
#include "event2/util.h"
#include "event2/keyvalq_struct.h"
#include "util-internal.h"
}

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libevent.lib")

#ifdef EVENT__HAVE_NETINET_IN_H
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#endif

#ifdef _WIN32
#ifndef stat
#define stat _stat
#endif
#ifndef fstat
#define fstat _fstat
#endif
#ifndef open
#define open _open
#endif
#ifndef close
#define close _close
#endif
#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#endif
#endif



char uri_root[512];

static const struct table_entry {
	const char *extension;
	const char *content_type;
} content_type_table[] = {
	{ "txt", "text/plain" },
	{ "c", "text/plain" },
	{ "h", "text/plain" },
	{ "html", "text/html" },
	{ "htm", "text/htm" },
	{ "css", "text/css" },
	{ "gif", "image/gif" },
	{ "jpg", "image/jpeg" },
	{ "jpeg", "image/jpeg" },
	{ "png", "image/png" },
	{ "pdf", "application/pdf" },
	{ "ps", "application/postsript" },
	{ NULL, NULL },
};

/*
function：transfer string to int
argument1：address of the string 
argument2：lenth of the string
char 0 == asc 48
return：the int data transfered to
*/
int str_to_int(char* p, int len)
{
	int i = 0, ret = 0;
	while(i < len)
	{
		if ((p[i] >= '0') && (p[i] <= '9'))
		{
			int value = p[i]-48;
			ret = ret*10 +value;
			i++;
		}
		else
		  break;
	}
	return ret;
}



/* Try to guess a good content-type for 'path' */
static const char *
guess_content_type(const char *path)
{
	const char *last_period, *extension;
	const struct table_entry *ent;
	last_period = strrchr(path, '.');
	if (!last_period || strchr(last_period, '/'))
		goto not_found; /* no exension */
	extension = last_period + 1;
	for (ent = &content_type_table[0]; ent->extension; ++ent) {
		if (!evutil_ascii_strcasecmp(ent->extension, extension))
			return ent->content_type;
	}

not_found:
	return "application/misc";
}

/* Callback used for the /dump URI, and for every non-GET request:
 * dumps all information to stdout and gives back a trivial 200 ok */
static void
dump_request_cb(struct evhttp_request *req, void *arg)
{
	const char *cmdtype;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	struct evbuffer *buf;

	switch (evhttp_request_get_command(req)) {
	case EVHTTP_REQ_GET: cmdtype = "GET"; break;
	case EVHTTP_REQ_POST: cmdtype = "POST"; break;
	case EVHTTP_REQ_HEAD: cmdtype = "HEAD"; break;
	case EVHTTP_REQ_PUT: cmdtype = "PUT"; break;
	case EVHTTP_REQ_DELETE: cmdtype = "DELETE"; break;
	case EVHTTP_REQ_OPTIONS: cmdtype = "OPTIONS"; break;
	case EVHTTP_REQ_TRACE: cmdtype = "TRACE"; break;
	case EVHTTP_REQ_CONNECT: cmdtype = "CONNECT"; break;
	case EVHTTP_REQ_PATCH: cmdtype = "PATCH"; break;
	default: cmdtype = "unknown"; break;
	}

	printf("Received a %s request for %s\nHeaders:\n",
	    cmdtype, evhttp_request_get_uri(req));

	headers = evhttp_request_get_input_headers(req);
	for (header = headers->tqh_first; header;
	    header = header->next.tqe_next) {
		printf("  %s: %s\n", header->key, header->value);
	}

	buf = evhttp_request_get_input_buffer(req);
	puts("Input data: <<<");
	while (evbuffer_get_length(buf)) {
		int n;
		char cbuf[128];
		n = evbuffer_remove(buf, cbuf, sizeof(cbuf));
		if (n > 0)
			(void) fwrite(cbuf, 1, n, stdout);
	}
	puts(">>>");

	evhttp_send_reply(req, 200, "OK", NULL);
}
int ParUrl(const char*uri,PicParam*pic,int IsJpg,ConfParam *conf)
{
//IsJpg==0 uri /vqd/service/videodetectphotoService?stream=ipcamera&id=1BD5AD26-13E4-4ce6-9643-F0375AC50878&time=20131212164941
	if(IsJpg==0)
	{
		const char*p0=uri;
		const char*pstr=NULL;
		const char*pid=NULL;
		const char*ptim=NULL;
		const char*pEnd=p0+strlen(uri);

		const char dstStr[16]="?stream=";
		const char dstId[16]="&id=";
		const char dstTim[16]="&time=";

		pstr=strstr(p0,dstStr);
		if(!pstr){
			return -1;
		}
		pid=strstr(pstr,dstId);
		if(!pid){
			return -1;
		}
		ptim=strstr(pid,dstTim);
		if(!ptim){
			return -1;
		}
		memcpy(pic->id,pid+strlen(dstId),ptim-pid-strlen(dstId));
		memcpy(pic->stream,pstr+strlen(dstStr),pid-pstr-strlen(dstStr));
		memcpy(pic->time,ptim+strlen(dstTim),pEnd-ptim-strlen(dstTim));
		memcpy(pic->picData,pic->time,strlen("20131212"));
		memcpy(pic->SerNam,p0,pstr-p0);
	}
//IsJpg==1 /20131212/ipcamera_1BD5AD26-13E4-4ce6-9643-F0375AC50878_20131212164941_1.jpg
	//	  /20140107/suE_13526_2BB58758-8837-4e90-BB07-66EC485DD1AB_20140107112336_1.jpg
	if(IsJpg==1)
	{
		int _num=0;
		int str_num=0;
		const char*p0=uri;
		const char*pdata=NULL;
		const char*pstr=NULL;
		const char*pid=NULL;
		const char*ptim=NULL;
		const char*pEnd=p0+strlen(uri);
		const char*pInd0=NULL;
		const char*pInd1=NULL;
		const char*ptmp=NULL;

		const char dst0[16]="/";
		const char dst1[16]="_";
		const char dst2[16]=".jpg";
		char ind[4];
		memset(ind,0,sizeof(ind));

		pdata=strstr(p0,dst0);
		if(!pdata){
			return -1;
		}
		//   "_" number
		ptmp=pdata;
		while(ptmp){
			ptmp=strstr(ptmp+1,dst1);//"_"
			if(ptmp){
				_num++;
			}
		}
		str_num=_num-2;
		//end "_"number

		pstr=strstr(pdata+1,dst0);
		if(!pstr){
			return -1;
		}
		//cmId   /20140107/suE_13526_2BB58758-8837-4e90-BB07-66EC485DD1AB_20140107112336_1.jpg
		pid=strstr(pstr,dst1);
		if(!pid){
			return -1;
		}
		str_num--;
		while(str_num>0){
			pid=strstr(pid+1,dst1);
			if(!pid){
				return -1;
			}
			str_num--;
		}
		//end cmId
		ptim=strstr(pid+1,dst1);
		if(!ptim){
			return -1;
		}
		pInd0=strstr(ptim+1,dst1);
		if(!pInd0){
			return -1;
		}
		pInd1=strstr(pInd0,dst2);
		if(!pInd1){
			return -1;
		}


		memcpy(pic->id,pid+strlen(dst1),ptim-pid-strlen(dst1));
		memcpy(pic->stream,pstr+strlen(dst0),pid-pstr-strlen(dst0));
		memcpy(pic->time,ptim+strlen(dst1),pInd0-ptim-strlen(dst1));
		memcpy(pic->picData,pdata+strlen(dst0),pstr-pdata-strlen(dst0));

		memcpy(pic->SerNam,conf->SerNam,strlen(conf->SerNam));

		memcpy(ind,pInd0+strlen(dst1),pInd1-pInd0-strlen(dst1));
		pic->PicInd=str_to_int(ind,strlen(ind));
	}
	return 0;
}

BOOL FolderExist(CString strPath)
{
    WIN32_FIND_DATA wfd;
    BOOL rValue = FALSE;
    HANDLE hFind = FindFirstFile(strPath, &wfd);
    if ((hFind!=INVALID_HANDLE_VALUE) && 
        (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
    {
        rValue = TRUE;
    }
    FindClose(hFind);
    return rValue;
}

int	TestFileExist(char*whole_path,PicParam*pic,char*dst)
{
	FILE   *pf=NULL;
	FILE   *ptest=NULL;
	int picMiss=0;
	int fLen=0;
	int ret=0,i=0;
	char tmp[1024];
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"%s/%s",whole_path,pic->picData);
	CString fPath=tmp;
	ret=FolderExist(fPath);
	if(ret==1)
	{//文件夹存在
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"%s/%s/%s_%s_%s_%d.jpg",whole_path,pic->picData,pic->stream,pic->id,pic->time,pic->PicInd);

		pf=fopen(tmp,"rb");
		if(pf!=NULL)
		{//该图片存在
			fseek(pf,0,SEEK_END);
			fLen= ftell(pf);	
			fseek(pf,0,SEEK_SET);

			//fread
			 ret = fread(dst,1,fLen,pf);
			 if(ret!=fLen){
				fclose(pf);
				return -1;
			}
			 //endfread
		}
		else{//该图片不存在
			if(pf){
				fclose(pf);
			}
			return -1;
		}	
	}else
	{//文件夹不存在
		if(pf){
			fclose(pf);
		}
		return -1;
	}

	if(pf){
		fclose(pf);
	}
	if(ptest){
		fclose(ptest);
	}
	return fLen;
}

int	TestFolderExist(char*whole_path,PicParam*pic,char senPic[3][1024])
{
	FILE   *fp=NULL;
	int picMissTim=0;
	int ret=0,i=0;
	char tmp[1024];
	char msg[1024];
	memset(tmp,0,sizeof(tmp));
	sprintf(tmp,"%s/%s",whole_path,pic->picData);
	CString fPath=tmp;
	ret=FolderExist(fPath);
	if(ret==1)
	{//文件夹存在
		for(i=0;i<3;i++)
		{
			memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%s/%s/%s_%s_%s_%d.jpg",whole_path,pic->picData,pic->stream,pic->id,pic->time,i+1);
			fp=fopen(tmp,"r");
			if(fp!=NULL){//该图片存在
		//		memcpy(senPic[i],tmp,sizeof(tmp));
				sprintf(senPic[i],"http://%s:%d/%s/%s_%s_%s_%d.jpg",pic->outIp,pic->outPort,pic->picData,pic->stream,pic->id,pic->time,i+1);
			}
			else{//该图片不存在
				picMissTim++;
			}	
		}
	}else
	{//文件夹不存在
		sprintf(msg,"TestFolderExist:FolderExist[%s] fail\n",fPath.GetString());
		LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
		return -1;
	}
	if(picMissTim==3){//three picture all not exist
		sprintf(msg,"TestFolderExist:fopen[%s] fail\n",tmp);
		LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
		return -1;
	}
	if(fp){
		fclose(fp);
	}
	return 0;
}
/* This callback gets invoked when we get any http request that doesn't match
 * any other callback.  Like any evhttp server callback, it has a simple job:
 * it must eventually call evhttp_send_error() or evhttp_send_reply().
 */
static void
send_document_cb(struct evhttp_request *req, void *arg)
{
	struct evbuffer *evb = NULL;
	ConfParam *conf = (ConfParam *)arg;
	struct evhttp_uri *decoded = NULL;
	const char *path;
	const char* ptm;
	char *decoded_path;
	char *whole_path = NULL;
	size_t len;
	int fd = -1;
	struct stat st;
	int i;
	FILE   *fp=NULL; 
	int ret=0;
	int IsJpg=0;
	int picLen=0;

	char xmlDat[1024];
	char tmp[1024];
	char senPic[3][1024];
	char PicData[1024*100];
	char msg[1024];

	memset(PicData,0,sizeof(PicData));
	memset(tmp,0,sizeof(tmp));
	memset(xmlDat,0,sizeof(xmlDat));
	memset(senPic,0,sizeof(senPic));
	PicParam picParam;
	memset(picParam.id,0,sizeof(picParam.id));
	memset(picParam.stream,0,sizeof(picParam.stream));
	memset(picParam.time,0,sizeof(picParam.time));
	memset(picParam.picData,0,sizeof(picParam.picData));
	memset(picParam.outIp,0,sizeof(picParam.outIp));
	picParam.outPort=0;
	memset(picParam.SerNam,0,sizeof(picParam.SerNam));
	picParam.PicInd=0;
	memset(picParam.picPath,0,sizeof(picParam.picPath));

	if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {//get or post
		dump_request_cb(req, arg);
		return;
	}

	const char *host=evhttp_request_get_host(req);
	if(!host){
		return;
	}
	memcpy(picParam.outIp,host,strlen(host));

	const char *uri = evhttp_request_get_uri(req);
//	printf("Got a GET request for <%s>\n",  uri);
	sprintf(msg,"Got a GET request for <%s>\n",  uri);
    LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
	ptm=strstr(uri,".jpg");
	if(ptm){//.jpg request
		IsJpg=1;
	}else{// first request
		IsJpg=0;
	}
	
	ret=ParUrl(uri,&picParam,IsJpg,conf);
	if(ret!=0){
			return;
	}
	picParam.outPort=conf->outsidePort;
	ret=strcmp(picParam.SerNam,conf->SerNam);
	if(ret!=0){
		return;
	}
	/* Decode the URI */
	decoded = evhttp_uri_parse(uri);
	if (!decoded) {
		printf("It's not a good URI. Sending BADREQUEST\n");
		evhttp_send_error(req, HTTP_BADREQUEST, 0);
		return;
	}

	/* Let's see what path the user asked for. */
	path = evhttp_uri_get_path(decoded);
	if (!path) 
	{
		path = "/";
	}
	/* We need to decode it, to see what path the user really wanted. */
	decoded_path = evhttp_uridecode(path, 0, NULL);
	if (decoded_path == NULL){
	//	printf("decoded_path == NULL\n");
		sprintf(msg,"decoded_path == NULL\n");
		LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
		goto err;
	}
	/* Don't allow any ".."s in the path, to avoid exposing stuff outside
	 * of the docroot.  This test is both overzealous and underzealous:
	 * it forbids aceptable paths like "/this/one..here", but it doesn't
	 * do anything to prevent symlink following." */
	if (strstr(decoded_path, "..")){
	//	printf("strstr-decoded_path, ..\n");
		sprintf(msg,"strstr-decoded_path, ..\n");
		LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
		goto err;
	}

	len = strlen(decoded_path)+strlen(conf->picPath)+2;
	if (!(whole_path = (char *)malloc(len))) {
	//	printf("malloc\n");
		sprintf(msg,"malloc\n");
		LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
		perror("malloc");
		goto err;
	}
	evutil_snprintf(whole_path, len, "%s", conf->picPath);
//whole_path c:/pic
	if (stat(whole_path, &st)<0) {
	//	printf("stat-whole_path\n");
		sprintf(msg,"stat-whole_path\n");
		LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
		goto err;
	}

	if(IsJpg==0){
		ret=TestFolderExist(whole_path,&picParam,senPic);
		if(ret==-1){
			sprintf(msg,"TestFolderExist\n");
			LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
		//	printf("TestFolderExist\n");
			goto err;
		}
	}else{
		picLen=TestFileExist(whole_path,&picParam,PicData);
		if(picLen==-1){
		//	printf("TestFileExist\n");
			sprintf(msg,"TestFileExist\n");
			LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
			goto err;
		}
	}
	/* This holds the content we're sending. */
	evb = evbuffer_new();

	if (S_ISDIR(st.st_mode)) 
	{
		if(IsJpg==0)
		{
			evbuffer_add_printf(evb,
								"<backresult>\r\n"
								"	<id>%d</id>\r\n"
								"	<info>Success</info>\r\n",
							0);
			for(i=0;i<3;i++)
			{
				if(strlen(senPic[i])>0)
				{
					evbuffer_add_printf(evb,"<photo>%s</photo>\r\n",
										senPic[i]);
				}
			}
			evbuffer_add_printf(evb,"</backresult>\r\n");
			evhttp_add_header(evhttp_request_get_output_headers(req),
			//    "Content-Type", "text/html");
			 "Content-Type", "text/xml");
		}
		else
		{
			evbuffer_add(evb,PicData,picLen);
			evhttp_add_header(evhttp_request_get_output_headers(req),
			    "Content-Type", "image/jpeg");
		}
	} 
	else 
	{
		/* Otherwise it's a file; add it to the buffer to get
		 * sent via sendfile */
		const char *type = guess_content_type(decoded_path);
		if ((fd = open(whole_path, O_RDONLY)) < 0) {
		//	printf("open\n");
			sprintf(msg,"open\n");
			LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
			perror("open");
			goto err;
		}

		if (fstat(fd, &st)<0) {
			/* Make sure the length still matches, now that we
			 * opened the file :/ */
		//	printf("fstat\n");
			sprintf(msg,"fstat\n");
			LogPrintf(msg,DEF_MSG_PRINT | DEF_MSG_SAVE,0);
			perror("fstat");
			goto err;
		}
		evhttp_add_header(evhttp_request_get_output_headers(req),
		    "Content-Type", type);
		evbuffer_add_file(evb, fd, 0, st.st_size);
	}

	evhttp_send_reply(req, 200, "OK", evb);
	goto done;
err:
#if 0
	evhttp_send_error(req, 404, "Document was not found");
#else	
	evb = evbuffer_new();
	evbuffer_add_printf(evb,
								"<backresult>\r\n"
								"	<id>%d</id>\r\n"
								"	<info>XMLFormatInvalid</info>\r\n",
							1);
	evbuffer_add_printf(evb,"</backresult>\r\n");
	evhttp_add_header(evhttp_request_get_output_headers(req),
			//    "Content-Type", "text/html");
			 "Content-Type", "text/xml");
	evhttp_send_reply(req, 200, "OK", evb);
#endif
	if (fd>=0)
		close(fd);
done:
	if (decoded)
		evhttp_uri_free(decoded);
	if (decoded_path)
		free(decoded_path);
	if (whole_path)
		free(whole_path);
	if (evb)
		evbuffer_free(evb);
}

static void
syntax(void)
{
	fprintf(stdout, "Syntax: http-server <docroot>\n");
}
HMODULE   GetCurrentModule() 
{    
	MEMORY_BASIC_INFORMATION   mbi; 
	static   int   dummy; 
	VirtualQuery(   &dummy,   &mbi,   sizeof(mbi)   ); 
	return   reinterpret_cast <HMODULE> (mbi.AllocationBase); 
}
/*2013-12-25-jc
Function:parse con.ini which contains localport serNam picPath outsideport
Param1:point to ConfParam to save information from con.ini
return:suc returns 0;fail returns -1
*/
int ReadCon(ConfParam*conf)
{
	FILE   *pf=NULL;
	char*pCon=NULL;
	char*p11=NULL;
	char*p12=NULL;
	char*p21=NULL;
	char*p22=NULL;
	char*p31=NULL;
	char*p32=NULL;
	char*p41=NULL;
	char*p42=NULL;
	int ret=0;
	int fLen=0;
	char dst11[32]="<localport>";
	char dst12[32]="</localport>";
	char dst21[32]="<serNam>";
	char dst22[32]="</serNam>";
	char dst31[32]="<picPath>";
	char dst32[32]="</picPath>";
	char dst41[32]="<outsideport>";
	char dst42[32]="</outsideport>";
	char tmp[256];
	memset(tmp,0,sizeof(tmp));

// f:\\我的电脑\\项目\\jhttps\\Debug\\config.ini";

	CString thisDir;
	GetModuleFileName(GetCurrentModule(),thisDir.GetBufferSetLength(1000+1),1000);   
	//thisDir.ReleaseBuffer();   
	thisDir=thisDir.Left(thisDir.ReverseFind(L'\\'));
	//add log
	sprintf(g_CurrentPath,"%s",thisDir.GetString());
	//end add log
	thisDir+="\\config.ini";
	char fpath[512];
	memset(fpath,0,sizeof(fpath));
	sprintf(fpath,"%s",thisDir.GetBuffer(thisDir.GetLength()));
	

	pf=fopen(fpath,"rb");
	if(pf==NULL){
		goto err;
	}
	fseek(pf,0,SEEK_END);
	fLen= ftell(pf);	
	fseek(pf,0,SEEK_SET);
	pCon=(char*)malloc(fLen);
	if(!pCon){
		goto err;
	}
	ret=fread(pCon,1,fLen,pf);
	if(ret!=fLen){
		goto err;
	}
	fclose(pf);
	p11=strstr(pCon,dst11);
	if(!p11){
		goto err;
	}
	p12=strstr(pCon,dst12);
	if(!p12){
		goto err;
	}
	p21=strstr(pCon,dst21);
	if(!p21){
		goto err;
	}
	p22=strstr(pCon,dst22);
	if(!p22){
		goto err;
	}
	p31=strstr(pCon,dst31);
	if(!p31){
		goto err;
	}
	p32=strstr(pCon,dst32);
	if(!p32){
		goto err;
	}
	p41=strstr(pCon,dst41);
	if(!p41){
		goto err;
	}
	p42=strstr(pCon,dst42);
	if(!p42){
		goto err;
	}
	memset(tmp,0,sizeof(tmp));
	memcpy(tmp,p11+strlen(dst11),p12-p11-strlen(dst11));
	conf->locPort=str_to_int(tmp,strlen(tmp));
	memcpy(conf->SerNam,p21+strlen(dst21),p22-p21-strlen(dst21));
	memcpy(conf->picPath,p31+strlen(dst31),p32-p31-strlen(dst31));
	memset(tmp,0,sizeof(tmp));
	memcpy(tmp,p41+strlen(dst41),p42-p41-strlen(dst41));
	conf->outsidePort=str_to_int(tmp,strlen(tmp));
	return 0;
err:
	if(pf){
		fclose(pf);
	}
	if(pCon){
		free(pCon);
		pCon=NULL;
	}
	return -1;
}

int
main(int argc, char **argv)
{
//	argc=2;
//	argv[1]="c:/pic";
	int ret=0,port=0;
	struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handle;
	ConfParam confParams;
	memset(confParams.picPath,0,sizeof(confParams.picPath));
	memset(confParams.SerNam,0,sizeof(confParams.SerNam));
	confParams.locPort=0;
	confParams.outsidePort=0;
	ret=ReadCon(&confParams);
	if(ret==-1){
		printf("[main]:ReadCon_ret==-1\n");
		return 0;
	}
	port=confParams.locPort;
#ifdef _WIN32
	WSADATA WSAData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &WSAData);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return (1);
#endif
/*
	if (argc < 2) {
		syntax();
		return 1;
	}
*/

	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		return 1;
	}

	/* Create a new evhttp object to handle requests. */
	http = evhttp_new(base);
	if (!http) {
		fprintf(stderr, "couldn't create evhttp. Exiting.\n");
		return 1;
	}

	/* The /dump URI will dump all requests to stdout and say 200 ok. */
//	evhttp_set_cb(http, "/dump", dump_request_cb, NULL);

	/* We want to accept arbitrary requests, so we need to set a "generic"
	 * cb.  We can also add callbacks for specific paths. */
	evhttp_set_gencb(http, send_document_cb, &confParams);

	/* Now we tell the evhttp what port to listen on */
	handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", port);
	if (!handle) {
		fprintf(stderr, "couldn't bind to port %d. Exiting.\n",
		    (int)port);
		return 1;
	}

	{
		/* Extract and display the address we're listening on. */
		struct sockaddr_storage ss;
		evutil_socket_t fd;
		ev_socklen_t socklen = sizeof(ss);
		char addrbuf[128];
		void *inaddr;
		const char *addr;
		int got_port = -1;
		fd = evhttp_bound_socket_get_fd(handle);
		memset(&ss, 0, sizeof(ss));
		if (getsockname(fd, (struct sockaddr *)&ss, &socklen)) {
			perror("getsockname() failed");
			return 1;
		}
		if (ss.ss_family == AF_INET) {
			got_port = ntohs(((struct sockaddr_in*)&ss)->sin_port);
			inaddr = &((struct sockaddr_in*)&ss)->sin_addr;
		} else if (ss.ss_family == AF_INET6) {
			got_port = ntohs(((struct sockaddr_in6*)&ss)->sin6_port);
			inaddr = &((struct sockaddr_in6*)&ss)->sin6_addr;
		} else {
			fprintf(stderr, "Weird address family %d\n",
			    ss.ss_family);
			return 1;
		}
		addr = evutil_inet_ntop(ss.ss_family, inaddr, addrbuf,
		    sizeof(addrbuf));
		if (addr) {
			printf("Listening on %s:%d\n", addr, got_port);
			evutil_snprintf(uri_root, sizeof(uri_root),
			    "http://%s:%d",addr,got_port);
		} else {
			fprintf(stderr, "evutil_inet_ntop failed\n");
			return 1;
		}
	}

	event_base_dispatch(base);

	return 0;
}
