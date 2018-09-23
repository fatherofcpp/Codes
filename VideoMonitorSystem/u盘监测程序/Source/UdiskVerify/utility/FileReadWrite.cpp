#include "stdafx.h"
#include "FileReadWrite.h"

int GetFileLen( const char *pfile )
{
	FILE *pf=NULL;
	int len = 0;
	int rtn = 0;

	pf = fopen(pfile,"rb");
	if(pf)
	{
		fseek(pf,0,SEEK_END);
		len = ftell(pf);	
		fseek(pf,0,SEEK_SET);

		fclose(pf);	
		return len;
	}
	else
	{
		return -1;
	}	
}

int GetFileData( const char *pfile, int len ,char* dst)
{
	FILE *pf=NULL;
	int rtn = 0;

	pf = fopen(pfile,"rb");
	if(pf)
	{
		fread(dst,1,len,pf);
		fclose(pf);	
		return 0;
	}
	else
	{
		return -1;
	}	
}


void encry_lisence( unsigned char *buf, int len )
{
	int i;
	for(i=0; i<len; i++, buf++)
	{
		*buf = (0xff - *buf);
	}
}

void decry_lisence( unsigned char *buf, int len )
{
	int i;
	for(i=0; i<len; i++, buf++)
	{
		*buf = (0xff - *buf);
	}
}

int LoadFile( const char *pfile, char* pBuff )
{
	FILE *pf=NULL;
	int len = 0;
	int rtn = 0;

	len = GetFileLen(pfile);
	if (len == -1)
	{
		return -1;
	}

	rtn = GetFileData(pfile,len,pBuff);
	if (rtn == 0)
	{
		decry_lisence((unsigned char *)pBuff,  len );

		return 0;
	}
	else
	{
		return -1;
	}
}