
#include "FileReadWrite.h"
#include "dataConvert.h"

int GetFileLen( const char *pfile )
{
	FILE *pf=NULL;
	int len = 0;

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




