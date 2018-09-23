#ifndef _XML_H_
#define _XML_H_

typedef struct
{
	//MU_INT32  fileID;
	MU_INT8	  *xmlBuf;
	MU_INT32  xmlLen;
	MU_INT32  brdID;
	MS_CHAR   *xmlName;
}XML_INFO;

typedef struct
{
	MU_INT32 BrdId;
	MU_INT32 ChanID;
}MEDIA_CFG;



#define min(x,y) (x<y)?x:y



MS_INT32 getValueInXml(M_HANDLE pxml,MS_CHAR *Value,MS_CHAR *FlagStr,MS_CHAR *FatherStr,MU_INT32 maxlen);
MS_INT32 getFileNamesInXml(M_HANDLE pxml,MS_CHAR FileName[][DEF_MAX_NAMESTRING_LENGHT2],MS_CHAR *FlagStr,MS_CHAR *FatherStr,MU_INT32 *nums,MU_INT32 maxNums,MU_INT32 brdFlag);
MS_INT32 getMediaInXml(M_HANDLE pxml,MEDIA_CFG Value[],MS_CHAR *FlagStr,MS_CHAR *FatherStr,MS_CHAR *AttrStr1,MS_CHAR *AttrStr2,MU_INT32 maxlen,MU_INT32 *rtn);
MS_INT32 CreateXmlTree(M_HANDLE *xml,MS_CHAR *xml_buf);
MS_INT32 DeleteXmlTree(M_HANDLE xml);

#endif

