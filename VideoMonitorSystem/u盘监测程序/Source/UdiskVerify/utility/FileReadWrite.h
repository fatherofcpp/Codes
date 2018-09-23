#pragma once
int GetFileLen(const char *pfile);
int GetFileData(const char *pfile, int len,char* dst);

int LoadFile( const char *pfile, char* pBuff );

void decry_lisence( unsigned char *buf, int len );
void encry_lisence( unsigned char *buf, int len );





