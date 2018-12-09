#pragma once
#include <string>
#ifndef M_NSP_STP_TYPE_REMANE
#define M_NSP_STP_TYPE_REMANE
typedef char                MS_CHAR;
typedef signed char         MS_INT8;
typedef signed short int    MS_INT16;
typedef signed int          MS_INT32;
typedef signed long long    MS_INT64;
typedef unsigned char       MU_CHAR;
typedef unsigned char       MU_INT8;
typedef unsigned short int  MU_INT16;
typedef unsigned int        MU_INT32;
typedef unsigned long long  MU_INT64;
typedef float               M_FLOAT;
typedef double              M_DOUBLE;
typedef bool                M_BOOL;
#else
#endif

namespace NSP_STP_CM
{
#define M_FREE(mm)                          \
	{                                           \
	if (mm != NULL)                         \
	{                                       \
	free(mm);                      \
	mm = NULL;                          \
}                                       \
}

	typedef struct
	{
		M_BOOL      canfree;
		MS_INT32    size;
		MS_INT32    used;
		MS_INT32    align;
		MS_CHAR    *buf;
	} BUFOBJ;

	extern MS_CHAR    *ST_fskip_space(const MS_CHAR * str);
	extern MU_INT32    ST_strlcpy(MS_CHAR *const dst, const MS_CHAR *src, MU_INT32 siz);
	extern MS_INT32    ST_filt_space_copy(MS_CHAR *const  _src, MS_CHAR *const  _dst, MS_INT32 dst_siz, M_BOOL filt_head_space, M_BOOL filt_tail_space);
	extern MS_INT32    ST_filt_token_copy(MS_CHAR *const _src, MS_CHAR *const _dst, const MS_CHAR *token_stt, const MS_CHAR *token_end);
	extern MU_INT32	   Hash_Str(const void *  value, MU_INT32 len);
	extern BUFOBJ     *f_bufobj_new(BUFOBJ *const bufobj, MS_INT32 size, MS_INT32 align);
	extern MS_INT32    f_bufobj_resize(BUFOBJ *const bufobj, MS_INT32 size);
	extern void        f_bufobj_del(BUFOBJ *bufobj);
	extern void   *mmem_malloc(MU_INT32 siz, MU_INT32 align = 8);
	extern void    mmem_free(void *real);

	static MS_CHAR    *ST_bskip_space(const MS_CHAR * str);
}
