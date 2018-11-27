//#include "stdafx.h"
#include <string.h>

#include "H2Ctype.h"
#include "../../comm/include/io.h"
#include "MMmem.h"

#include "../include/xmlparse.h"
#include "../include/xml.h"

//extern MS_CHAR		g_ComCfgFile[DEF_MAX_NAMESTRING_LENGHT2];//= "comcfg.xlm";
//extern MS_CHAR		g_CommIndexFileName[DEF_MAX_NAMESTRING_LENGHT2];
//extern MS_CHAR		g_BrdProgFileName[DEF_MAX_NAMESTRING_LENGHT2];

//extern MS_CHAR		g_StreamCfgFileName[DEF_MAXSUPPORT_STREAM][DEF_MAX_NAMESTRING_LENGHT2];
//extern MS_CHAR		g_BrdCfgFileName[DEF_MAXSUPPORT_SUBCARDS][DEF_MAX_NAMESTRING_LENGHT2];


extern XML_INFO 	g_BrdCfgXml[DEF_MAXSUPPORT_SUBCARDS];

MS_INT32 getValueInXml(M_HANDLE pxml,MS_CHAR *Value,MS_CHAR *FlagStr,MS_CHAR *FatherStr,MU_INT32 maxlen)
{

	XMLNODE *node;
	XML *xml = (XML *)pxml;
	MS_CHAR msg[DEF_MAX_MSG_LENGHT];


	if (xml->root == NULL)
	{
		myLOGprint("xml->root is NULL in func(getValueInXml).\n",DEF_MSG_PRINT,0);

		return MD_FAILED;
	}

	if (FatherStr != NULL)
	{

		node = f_xmlnode_sch_elem(xml->root, FatherStr);

		//return MD_FAILED;


	}
	else
		node = xml->root;

	if (node == NULL )
	{

		sprintf(msg,"Cann't find '%s' element in xml.\n",FatherStr); 

		myLOGprint("xml->root is NULL in func(getValueInXml).\n",DEF_MSG_PRINT,0);

		return MD_FAILED;
	}


	node = f_xmlnode_sch_elem(node, FlagStr);

	if (node == NULL)
	{
		sprintf(msg,"Cann't find %s node in XML file",FlagStr);

		myLOGprint(msg,DEF_MSG_PRINT,0);

		return MD_FAILED;

	}

	//strcpy(Value,node->data);

	memcpy(Value,node->data,min(maxlen-1,strlen(node->data)+1));

	#if 0

	node = f_xmlnode_get_elem_head(xml->root);

	while (node != NULL)
	{

		if (strcmp(node->name,FlagStr) == 0)
		{
			strcpy(FileName,node->data);

			break;
		}
		else
			node = f_xmlnode_nxt_node(node, NULL);
			
	}
	#endif

	return MD_OK;
	
}



MS_INT32 getFileNamesInXml(M_HANDLE pxml,MS_CHAR FileName[][DEF_MAX_NAMESTRING_LENGHT2],MS_CHAR *FlagStr,MS_CHAR *FatherStr,MU_INT32 *nums,MU_INT32 maxNums,MU_INT32 brdFlag)
{

	XMLNODE *node,*attr;
	XML *xml = (XML *)pxml;
	MS_CHAR msg[DEF_MAX_MSG_LENGHT];
	MU_INT32 m_nums=0;

	*nums = 0;

	if (xml->root == NULL)
	{
		myLOGprint("xml->root is NULL in func(getValueInXml).\n",DEF_MSG_PRINT,0);

		return MD_FAILED;
	}

	if (FatherStr != NULL)
	{

		node = f_xmlnode_sch_elem(xml->root, FatherStr);

		//return MD_FAILED;


	}
	else
		node = xml->root;

	if (node == NULL )
	{

		sprintf(msg,"Cann't find '%s' element in xml.\n",FatherStr); 

		myLOGprint("xml->root is NULL in func(getValueInXml).\n",DEF_MSG_PRINT,0);

		return MD_FAILED;
	}


	node = f_xmlnode_sch_elem(node, FlagStr);


	//node = f_xmlnode_sch_elem(xml->root, FlagStr);

	if (node == NULL)
	{
		sprintf(msg,"Cann't find %s node in XML file",FlagStr);

		myLOGprint(msg,DEF_MSG_PRINT,0);

		return MD_FAILED;

	}



	node = f_xmlnode_get_elem_head(node);

	if (node == NULL)
	{
		myLOGprint("No element.\n",DEF_MSG_PRINT,0);

		return MD_FAILED;

	}

	while (node != NULL)
	{


		if (m_nums > maxNums)
		{
			sprintf(msg,"Exceed the max support on %s,max is %d",FlagStr,maxNums);

			myLOGprint(msg,DEF_MSG_PRINT,0);

			break;

		}
		attr = f_xmlnode_sch_attr(node, "DESC");

		if (attr != NULL)
		{
			if (brdFlag)
			{
	
				g_BrdCfgXml[m_nums].brdID = atoi(attr->data);
	
				g_BrdCfgXml[m_nums].xmlName = FileName[m_nums];
			}


			//strcpy(FileName[*nums],node->data);
			memcpy(FileName[m_nums],node->data,min(DEF_MAX_NAMESTRING_LENGHT2-1,strlen(node->data)+1));

			m_nums ++;
		}

		node = f_xmlnode_nxt_node(node, NULL);
			
	}

	*nums = m_nums;
	
	return MD_OK;
	
}



MS_INT32 getMediaInXml(M_HANDLE pxml,MEDIA_CFG Value[],MS_CHAR *FlagStr,MS_CHAR *FatherStr,MS_CHAR *AttrStr1,MS_CHAR *AttrStr2,MU_INT32 maxlen,MU_INT32 *rtn)
{

	MU_INT32 nums=0,brdNum=0,channel=0,index;

	XMLNODE *node,*attr;
	XML *xml = (XML *)pxml;
	MS_CHAR msg[DEF_MAX_MSG_LENGHT];

	*rtn = 0;

	if (xml->root == NULL)
	{
		myLOGprint("xml->root is NULL in func(getValueInXml).\n",DEF_MSG_PRINT,0);

		return MD_FAILED;
	}

	if (FatherStr != NULL)
	{

		node = f_xmlnode_sch_elem(xml->root, FatherStr);

		//return MD_FAILED;


	}
	else
		node = xml->root;

	if (node == NULL )
	{

		sprintf(msg,"Cann't find '%s' element in xml.\n",FatherStr); 

		myLOGprint("xml->root is NULL in func(getValueInXml).\n",DEF_MSG_PRINT,0);

		return MD_FAILED;
	}

	node = f_xmlnode_get_elem_head(node);

	while (node != NULL)
	{

		if (strcmp(node->name,FlagStr) == 0)
		{
			//strcpy(FileName,node->data);
			attr = f_xmlnode_sch_elem(node, AttrStr1);

			if (attr != NULL)
			{
				brdNum = atoi(attr->data);

			}
			else
			{
				node = f_xmlnode_nxt_node(node, NULL);
				continue;
			}

			attr = f_xmlnode_sch_elem(node, AttrStr2);

			if (attr != NULL)
			{
				channel = atoi(attr->data);

			}
			else
			{
				node = f_xmlnode_nxt_node(node, NULL);
				continue;
			}

			for (index=0;index<nums;index++)
			{
				if (Value[index].BrdId == brdNum && Value[nums].ChanID == channel)
					break;
			}

			if (index == nums)
			{
				Value[nums].BrdId = brdNum;
				Value[nums].ChanID = channel;
				nums++;

				if (nums>=maxlen)
					break;
			}
		}

		node = f_xmlnode_nxt_node(node, NULL);
			
	}

	*rtn = nums;

	return MD_OK;

}



MS_INT32 CreateXmlTree(M_HANDLE *xml,MS_CHAR *xml_buf)
{
	XML	*pxml;

	*xml = NULL;

	pxml = (XML *)MM_alloc(sizeof(XML),16);

	memset(pxml,0,sizeof(XML));

	if (pxml == NULL)
	{
		myLOGprint("Memory is not enough in func CreateXmlTree.\n",DEF_MSG_PRINT,0);

		return MD_FAILED;
	}

	
	if (0 > f_xmlparse_init(pxml, xml_buf))
	{
		MM_free(pxml,sizeof(XML));

		return MD_FAILED;
	}
	
	*xml = pxml;

	return MD_OK;



}

MS_INT32 DeleteXmlTree(M_HANDLE xml)
{
	XML *pxml = (XML *)xml;

	if (pxml != NULL)
		f_xmlnode_del(pxml);

	return MD_OK;


}



