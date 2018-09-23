#ifndef __OBJ_OBJECT__H__  	 	 	 	 	 	 	 	 	
#define __OBJ_OBJECT__H__ 	
 
#include <stdio.h> 	
#include <string.h>
#include <stdlib.h>

#include "misc.h"
#define MAX_PROPERTY	32
#define MAX_STR 255
struct OBJ; 	

typedef struct OBJProperty
{ 	
	void (*Init)(struct OBJProperty *pOBJProperty);
	void (*Close)(struct OBJProperty *pOBJProperty); 	
		

	char* (*GetPropName)(const struct OBJProperty *pOBJProperty); 	
	int	(*GetType)(const struct OBJProperty *pOBJProperty); 	

	char* (*GetString)(const struct OBJProperty *pOBJProperty); 	
	struct OBJ* (*GetObject)(const struct OBJProperty *pOBJProperty); 	    	
	int (*Decode)(struct OBJProperty *pOBJProperty, const char *pBuffer, int nSize, int bDecodeName); 	
	void (*Reset)(struct OBJProperty *pOBJProperty); 
	double (*GetNumber)(const struct OBJProperty *pOBJProperty);
	struct OBJ		*m_objVal;
	int 			m_type; 
	char 			m_strName[MAX_STR]; 
	char			m_strVal[MAX_STR]; 	
	double			m_dNumVal;			
}COBJProperty; 

typedef struct OBJ 	
{ 	
	void (*Init)(struct OBJ *pOBJ);
	void (*Close)(struct OBJ *pOBJ); 
	int (*Decode)(struct OBJ *pOBJ, const char *pBuffer, int nSize, int bDecodeName); 	
	int (*GetPropertyCount)(const struct OBJ *pOBJ);  	
	struct OBJProperty* (*GetProperty)(const struct OBJ *pOBJ, int nIndex); 
	void (*Reset)(struct OBJ *pOBJ); 	
	int					m_nProperties; 	
	COBJProperty		m_properties[MAX_PROPERTY];
}COBJ; 



extern COBJ g_OBJ;	
extern COBJProperty g_OBJProperty;	
#endif 	
											
