#include "obj.h" 	
#include "misc.h"


static void my_Init_OBJProperty(struct OBJProperty *pOBJPropert)
{
	pOBJPropert->Reset(pOBJPropert);
}

static void my_Close_OBJProperty(struct OBJProperty *pOBJPropert)
{
	pOBJPropert->Reset(pOBJPropert);
}

static char* my_GetPropName(const struct OBJProperty *pOBJProperty)
{
	return ((char *)&pOBJProperty->m_strName[0]);
} 	

static int my_GetType(const struct OBJProperty *pOBJProperty) 
{ 	
	return pOBJProperty->m_type; 	
} 	

static char* my_GetString(const struct OBJProperty *pOBJProperty) 
{ 	
	return ((char *)&pOBJProperty->m_strVal[0]); 	
} 	

static struct OBJ* my_GetObject(const struct OBJProperty *pOBJProperty)
{ 	
	return pOBJProperty->m_objVal; 	
} 	
	
static double my_GetNumber(const struct OBJProperty *pOBJProperty)  	
{ 	
	return pOBJProperty->m_dNumVal; 	
}

static int my_Decode_OBJProperty(struct OBJProperty *pOBJProperty, const char *pBuffer, int nSize, int bDecodeName)
{
	int nArrayNum;
	int nRes;
	short nStringSize;
	short nNameSize;
	int nOriginalSize = nSize;
	if (nSize == 0 || !pBuffer)
		return -1;
	if (*pBuffer == 0x05){
		pOBJProperty->m_type = 0x05;
		return 1;
	}
	if (bDecodeName && nSize < 4)
		return -1;

	if (bDecodeName){
		nNameSize = ReadInt16(pBuffer);
		if (nNameSize > nSize - (short)sizeof(short))
			return -1;
		//strcpy(pOBJProperty->m_strName, ReadString(pBuffer));
		ReadString(pBuffer, pOBJProperty->m_strName);
		nSize -= sizeof(short) + strlen(pOBJProperty->m_strName);
		pBuffer += sizeof(short) + strlen(pOBJProperty->m_strName);
	}
	if (nSize == 0)
		return -1;
	nSize--;

	switch (*pBuffer){
		case 0x00:
			if (nSize < (int)sizeof(double))
				return -1;
			pOBJProperty->m_dNumVal = ReadNumber(pBuffer+1);
			nSize -= sizeof(double);
			pOBJProperty->m_type = 0x00;
			break;
		case 0x01:
			if (nSize < 1)
				return -1;
			pOBJProperty->m_dNumVal = (double)ReadBool(pBuffer+1);
			nSize--;
			pOBJProperty->m_type = 0x01;
			break;
		case 0x02:
			nStringSize = ReadInt16(pBuffer+1);
			if ((nSize < nStringSize + (int)sizeof(short)) ||(nStringSize < 0))
				return -1;
			//strcpy(pOBJProperty->m_strVal, ReadString(pBuffer+1));
			ReadString((pBuffer+1), pOBJProperty->m_strVal);
			nSize -= sizeof(short) + nStringSize;
			pOBJProperty->m_type = 0x02;
			break;
		case 0x03:
			pOBJProperty->m_objVal = (COBJ *) malloc(sizeof(COBJ));
			*pOBJProperty->m_objVal = g_OBJ;
			pOBJProperty->m_objVal->Init(pOBJProperty->m_objVal);
			nRes = pOBJProperty->m_objVal->Decode(pOBJProperty->m_objVal, pBuffer+1, nSize, 1);
			if (nRes == -1)
				return -1;
			nSize -= nRes;
			pOBJProperty->m_type = 0x03;
			break;
		case 0x06:
			pOBJProperty->m_type = 0x06;
			break;
		case 0x08:
// 			FILE * pFile;
// 			pFile = fopen ( "d:\\myfile.bin" , "wb" );
// 			fwrite (pBuffer , 1 , nSize+1 , pFile );
// 			fclose (pFile);

			nArrayNum = ReadInt32(pBuffer+1);
			pOBJProperty->m_objVal = (COBJ *) malloc(sizeof(COBJ));
			*pOBJProperty->m_objVal = g_OBJ;
			pOBJProperty->m_objVal->Init(pOBJProperty->m_objVal);
			nRes = pOBJProperty->m_objVal->Decode(pOBJProperty->m_objVal, pBuffer+1 + 4, nSize - 4, 1);
			if (nRes == -1)
				return -1;
			nSize -= nRes;
			pOBJProperty->m_type = 0x08;
			break;
		default:
			return -1;
	}
	return nOriginalSize - nSize;
}

static int my_Decode_OBJProperty_first(struct OBJProperty *pOBJProperty, const char *pBuffer, int nSize, int bDecodeName)
{ 	
	int nRes;
	short nStringSize;
	short nNameSize;
	int nOriginalSize = nSize; 	
	if (nSize == 0 || !pBuffer) 	
		return -1; 	
	if (*pBuffer == 0x05){ 	
		pOBJProperty->m_type = 0x05; 	
		return 1; 	
	} 	
	if (bDecodeName && nSize < 4) 	
		return -1; 
    
	if (bDecodeName){ 	
		nNameSize = ReadInt16(pBuffer); 	
		if (nNameSize > nSize - (short)sizeof(short)) 	
			return -1; 	
		//strcpy(pOBJProperty->m_strName, ReadString(pBuffer)); 
		ReadString(pBuffer, pOBJProperty->m_strName);	
		nSize -= sizeof(short) + strlen(pOBJProperty->m_strName); 	
		pBuffer += sizeof(short) + strlen(pOBJProperty->m_strName); 	
	} 	
	if (nSize == 0) 	
		return -1; 	
	nSize--; 

	switch (*pBuffer){ 	
		case 0x00:  	
			if (nSize < (int)sizeof(double)) 	
				return -1; 	
			pOBJProperty->m_dNumVal = ReadNumber(pBuffer+1); 	
			nSize -= sizeof(double); 	
			pOBJProperty->m_type = 0x00; 	
			break; 
		case 0x01:
			if (nSize < 1)
				return -1;
			pOBJProperty->m_dNumVal = (double)ReadBool(pBuffer+1);
			nSize--;
			pOBJProperty->m_type = 0x01;
			break;
		case 0x02:  	
			nStringSize = ReadInt16(pBuffer+1); 	
			if (nSize < nStringSize + (int)sizeof(short)) 	
				return -1; 	
			//strcpy(pOBJProperty->m_strVal, ReadString(pBuffer+1)); 	
			ReadString((pBuffer+1), pOBJProperty->m_strVal);
			nSize -= sizeof(short) + nStringSize; 	
			pOBJProperty->m_type = 0x02; 	
			break; 	
		case 0x03:  	
// 			if (pOBJProperty->m_objVal != NULL){
// 				pOBJProperty->m_objVal->Close(pOBJProperty->m_objVal);
// 				free(pOBJProperty->m_objVal);
// 				pOBJProperty->m_objVal = NULL;
// 			}
			pOBJProperty->m_objVal = (COBJ *) malloc(sizeof(COBJ));
			*pOBJProperty->m_objVal = g_OBJ;
			pOBJProperty->m_objVal->Init(pOBJProperty->m_objVal);
			nRes = pOBJProperty->m_objVal->Decode(pOBJProperty->m_objVal, pBuffer+1, nSize, 1); 	
			if (nRes == -1) 	
				return -1; 	
			nSize -= nRes; 	
			pOBJProperty->m_type = 0x03; 	
			break; 	
		default: 	
			return -1; 	
	} 	
	return nOriginalSize - nSize; 	
} 	

static void my_Reset_OBJProperty(struct OBJProperty *pOBJProperty)
{ 	
	pOBJProperty->m_dNumVal = 0.0; 	
	memset(pOBJProperty->m_strVal, 0, sizeof(pOBJProperty->m_strVal));
	memset(pOBJProperty->m_strName, 0, sizeof(pOBJProperty->m_strName)); 
	if (pOBJProperty->m_objVal != NULL){
		pOBJProperty->m_objVal->Close(pOBJProperty->m_objVal);
		free(pOBJProperty->m_objVal);
		pOBJProperty->m_objVal = NULL;
	} 	

	pOBJProperty->m_type = -1; 	
} 	

COBJProperty g_OBJProperty = 
{	
	my_Init_OBJProperty, 
	my_Close_OBJProperty, 
	my_GetPropName, 
	my_GetType, 
	my_GetString, 
	my_GetObject,
	my_Decode_OBJProperty,
	my_Reset_OBJProperty,
	my_GetNumber,
	NULL,
	-1,
};


static void my_Init_OBJ(struct OBJ *pOBJ)
{
	pOBJ->Reset(pOBJ); 	
}

static void my_Close_OBJ(struct OBJ *pOBJ) 	
{ 	
	pOBJ->Reset(pOBJ); 	
} 	

static int my_Decode_OBJ(struct OBJ *pOBJ, const char *pBuffer, int nSize, int bDecodeName)
{ 	
	int nRes;
	int nOriginalSize = nSize; 	
	int bError = 0; 
	COBJProperty prop = g_OBJProperty;
	prop.Init(&prop);
	
	while (nSize > 3){ 	
		if (ReadInt24(pBuffer) == 0x00000009){ 	
		  nSize -= 3; 	
		  bError = 0; 	
		  break; 	
		} 	
		if (bError){ 	
			nSize--; 	
			pBuffer++; 	
			continue; 	
		} 	

		nRes = prop.Decode(&prop, pBuffer, nSize, bDecodeName); 	
		if (nRes == -1) 	
			bError = 1; 	
		else{ 	
			nSize -= nRes; 	
			pBuffer += nRes; 	
			pOBJ->m_properties[pOBJ->m_nProperties] = prop; 	
			pOBJ->m_nProperties++; 
			prop = g_OBJProperty;
			prop.Init(&prop);
		} 	
	} 	

	if (bError) 	
		return -1; 	
	return nOriginalSize - nSize; 

} 	

static int my_GetPropertyCount(const struct OBJ *pOBJ)
{ 	
	return pOBJ->m_nProperties; 	
} 	
 	
static struct OBJProperty* my_GetProperty(const struct OBJ *pOBJ, int nIndex) 	
{ 	
	if (nIndex > (pOBJ->m_nProperties-1)) 	
		return &g_OBJProperty; 	

	return ((struct OBJProperty *)&pOBJ->m_properties[nIndex]); 	
} 	

static void my_Reset_OBJ(struct OBJ *pOBJ)
{ 	
	int n;
	COBJProperty OBJProperty;
	for (n = 0; n < pOBJ->m_nProperties; n++){
		OBJProperty = pOBJ->m_properties[n];
		OBJProperty.Close(&OBJProperty);
	}
	pOBJ->m_nProperties = 0;

	for (n = 0; n < MAX_PROPERTY; n++)
	 pOBJ->m_properties[n] = g_OBJProperty;
	
} 	

COBJ g_OBJ = 
{	
	my_Init_OBJ, 
	my_Close_OBJ, 
	my_Decode_OBJ, 
	my_GetPropertyCount, 
	my_GetProperty, 
	my_Reset_OBJ,
	0,
};											
