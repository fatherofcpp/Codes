
#ifndef _DEF_MICROVIDEO_H_
#define _DEF_MICROVIDEO_H_
//
#include "stdafx.h"

typedef struct tagDEV_MICROVIDEO{//南京感动使用参数结构
 
	CString SrcUrl;
	CString ServerIP;//dst server ip to report
	int nPort;//dst server port to report
	CString PubName;
	int StreamNumber;
	
}DEV_MICROVIDEO_PARAMS;


#endif//_DEF_MICROVIDEO_H_





