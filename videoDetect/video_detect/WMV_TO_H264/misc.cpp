#include "StdAfx.h"
#include "misc.h"


short  ReadInt16(const char *data) 	
{ 	
  short val; 	
  memcpy(&val,data,sizeof(short)); 	
  return ntohs(val); 	
} 

int ReadInt24(const char *data) 	
{ 	
	int val;	
	char tmp[4] = {0}; 	
	memcpy(tmp+1, data, 3); 	
		
	memcpy(&val, tmp, sizeof(int)); 	
	return ntohl(val); 	
} 

int  ReadInt32(const char *data) 	
{ 	
  int val; 	
  memcpy(&val, data, sizeof(int)); 	
  return ntohl(val); 	
} 

int ReadString(const char *data, char *pCopy) 	
{ 	
	short len;
	len = ReadInt16(data); 	
	if (len > 0){  	
		memcpy(pCopy, data + sizeof(short), len);
		return 1;
	} 	
	 	
	return 0; 	
}

double ReadNumber(const char *data) 	
{ 	
	int i;
	double val;
	char *dPtr = (char *)&val; 	
	for (i=7;i>=0;i--){ 	
		*dPtr = data[i]; 	
		dPtr++; 	
	} 	
	return val; 
}

int EncodeString(char *output, const char *strName, const char *strValue) 	
{ 	
  char *buf = output; 	
  short length = htons(strlen(strName)); 	
  memcpy(buf, &length, 2); 	
  buf += 2; 	
  memcpy(buf, strName, strlen(strName)); 	
  buf += strlen(strName); 	
  	
  buf += EncodeString_short(buf, strValue); 	
  return buf - output; 	
}

int EncodeString_short(char *output, const char *strValue) 	
{ 	
  char *buf = output;
  short length;
  *buf = 0x02;	
  buf++; 	
  length = htons(strlen(strValue)); 	
  memcpy(buf, &length, 2); 	
  buf += 2; 	
  memcpy(buf, strValue, strlen(strValue)); 	
  buf += strlen(strValue); 	
  return buf - output; 	
} 

int EncodeNumber(char *output, const char *strName, double dVal) 	
{ 	
  char *buf = output; 	
  unsigned short length = htons(strlen(strName)); 	
  memcpy(buf, &length, 2); 	
  buf += 2; 	
  memcpy(buf, strName, strlen(strName)); 	
  buf += strlen(strName); 	
  buf += EncodeNumber_short(buf, dVal); 	
  return buf - output; 	
}

int EncodeNumber_short(char *output, double dVal) 	
{ 	
	int i;
	char *dPtr = NULL;
  char *buf = output;  	
  *buf = 0x00; 	
  buf++; 	
  dPtr = (char *)&dVal; 	
  for (i=7;i>=0;i--){ 	
    buf[i] = *dPtr; 	
    dPtr++; 	
  } 	
  buf += 8; 	
  return buf - output; 	
}

int EncodeInt16(char *output, short nVal) 	
{ 	
  nVal = htons(nVal); 	
  memcpy(output, &nVal, sizeof(short)); 	
  return sizeof(short); 	
}

int EncodeInt24(char *output, int nVal) 	
{ 	
	char *ptr = NULL;
  nVal = htonl(nVal); 	
  ptr = (char *)&nVal; 	
  ptr++; 	
  memcpy(output, ptr, 3); 	
  return 3; 	
}

int EncodeInt32(char *output, int nVal) 	
{ 	
  nVal = htonl(nVal); 	
  memcpy(output, &nVal, sizeof(int)); 	
  return sizeof(int); 	
}

int EncodeBoolean_short(char *output, bool bVal) 	
{ 	
	char *buf = output;  	
	*buf = 0x01;	
	buf++; 	
	*buf = bVal?0x01:0x00; 	
	buf++; 	
	return buf - output; 	
} 

int EncodeBoolean(char *output, char *strName, bool bVal) 	
{ 	
	char *buf = output; 	
	unsigned short length = htons(strlen(strName)); 	
	memcpy(buf, &length, 2); 	
	buf += 2; 	
	memcpy(buf, strName, strlen(strName)); 	
	buf += strlen(strName); 	
	buf += EncodeBoolean_short(buf, bVal); 	
	return buf - output; 	
}

void put_byte(char **s, int b)
{
    *(*s)++ = b;
}

void put_le32(char **s, unsigned int val)
{
    put_byte(s, val);
    put_byte(s, val >> 8);
    put_byte(s, val >> 16);
    put_byte(s, val >> 24);
}

void put_be32(char **s, unsigned int val)
{
    put_byte(s, val >> 24);
    put_byte(s, val >> 16);
    put_byte(s, val >> 8);
    put_byte(s, val);
}

void put_le16(char **s, unsigned int val)
{
    put_byte(s, val);
    put_byte(s, val >> 8);
}

void put_be16(char **s, unsigned int val)
{
    put_byte(s, val >> 8);
    put_byte(s, val);
}

void put_le24(char **s, unsigned int val)
{
    put_le16(s, val & 0xffff);
    put_byte(s, val >> 16);
}

void put_be24(char **s, unsigned int val)
{
    put_be16(s, val >> 8);
    put_byte(s, val);
}

int get_byte(char **s)
{
	return *(*s)++;
}

unsigned int get_le16(char **s)
{
    unsigned int val;
    val = get_byte(s);
    val |= get_byte(s) << 8;
    return val;
}

unsigned int get_be16(char **s)
{
    unsigned int val;
    val = get_byte(s) << 8;
    val |= get_byte(s);
    return val;
}

unsigned int get_le24(char **s)
{
    unsigned int val;
    val = get_le16(s);
    val |= get_byte(s) << 16;
    return val;
}

unsigned int get_be24(char **s)
{
    unsigned int val;
    val = get_be16(s) << 8;
    val |= get_byte(s);
    return val;
}

unsigned int get_le32(char **s)
{
    unsigned int val;
    val = get_le16(s);
    val |= get_le16(s) << 16;
    return val;
}

unsigned int get_be32(char **s)
{
    unsigned int val;
    val = get_be16(s) << 16;
    val |= get_be16(s);
    return val;
}