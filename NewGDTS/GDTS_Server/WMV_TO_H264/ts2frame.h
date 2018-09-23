#ifndef _DEF_TS_FRAME_H_
#define _DEF_TS_FRAME_H_

typedef int (*TS2FRAMECALLBACK)(char* pBuffer, int size,  int width, int height,long ltime,void *pUserData,int nType);

typedef struct 
{
	
	void*			pUserData;
	
	int			canStart;
	int				m_programId;
	int				m_nH264ID;
	int				m_nAccID;
	int				m_width;
	int				m_height;

	void			*m_pH264Frame;
	void			*m_pAccFrame;	


	//TS2FRAMECALLBACK	*CallBackFunc;					//»Øµ÷º¯Êý
	int (*CallBackFunc)(char* pBuffer, int size,  int width, int height,long ltime,void *pUserData,int nType);
	
}TS2FRAME;

#if 0
extern "C" void InitTsFunc(void *pParams,void *CALLFUNC);

extern "C" void ProcessTSData(void *pParams,unsigned char *data, int len);

extern "C" void ParsePES(void *hHandle,unsigned char* buf, int len, void* pES);
#endif



#endif



