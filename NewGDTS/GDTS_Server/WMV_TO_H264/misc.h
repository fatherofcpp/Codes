#ifndef __MISC__H__  	 	 	 	 	 	 	 	 	
#define __MISC__H__

#include <string.h>

#define HTONS(x) ( ( (((unsigned short)(x)) >>8) & 0xff) | \
	((((unsigned short)(x)) & 0xff)<<8) )
#define NTOHS(x) ( ( (((unsigned short)(x)) >>8) & 0xff) | \
	((((unsigned short)(x)) & 0xff)<<8) )
#define HTONL(x) ((((x)>>24) & 0xffL) | (((x)>>8) & 0xff00L) | \
	(((x)<<8) & 0xff0000L) | (((x)<<24) & 0xff000000L))
#define NTOHL(x) ((((x)>>24) & 0xffL) | (((x)>>8) & 0xff00L) | \
	(((x)<<8) & 0xff0000L) | (((x)<<24) & 0xff000000L))

#define htons(x) HTONS(x)
#define ntohs(x) NTOHS(x)
#define htonl(x) HTONL(x)
#define ntohl(x) NTOHL(x)

extern short ReadInt16(const char *data);
extern int  ReadInt24(const char *data);
extern int  ReadInt32(const char *data);
extern int ReadString(const char *data, char *pCopy);
extern double ReadNumber(const char *data);
extern int EncodeString(char *output, const char *strName, const char *strValue);
extern int EncodeString_short(char *output, const char *strValue);
extern int EncodeNumber(char *output, const char *strName, double dVal);
extern int EncodeNumber_short(char *output, double dVal); 
extern int EncodeInt16(char *output, short nVal);
extern int EncodeInt24(char *output, int nVal);
extern int EncodeInt32(char *output, int nVal); 
extern int EncodeBoolean(char *output, char *strName, bool bVal); 
extern int EncodeBoolean_short(char *output, bool bVal);

extern void put_byte(char **s, int b);
extern void put_le32(char **s, unsigned int val);
extern void put_be32(char **s, unsigned int val);
extern void put_le16(char **s, unsigned int val);
extern void put_be16(char **s, unsigned int val);
extern void put_le24(char **s, unsigned int val);
extern void put_be24(char **s, unsigned int val);

extern int get_byte(char **s);
extern unsigned int get_le16(char **s);
extern unsigned int get_be16(char **s);
extern unsigned int get_le24(char **s);
extern unsigned int get_be24(char **s);
extern unsigned int get_le32(char **s);
extern unsigned int get_be32(char **s);

 
#endif 
