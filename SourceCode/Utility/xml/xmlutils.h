#pragma once
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
	const int DEF_STR_LEN_MIN = 64;
	const int DEF_STR_LEN_STD = 256;
	const int DEF_BUFOBJ_SIZE_MAX = (5*1024 * 1024);

	const int DEF_XTT_NONE = 0;
	const int DEF_XTT_TAG_START = 0x10; // <
	const int DEF_XTT_TAG_CLOSE = 0x20;// >
	const int DEF_XTT_EQUAL = 0x30;// =
	const int  DEF_XTT_TAG_TYPE_END = 0x01;// /
	const int  DEF_XTT_TAG_TYPE_DECL = 0x02;// ?
	const int  DEF_XTT_TAG_TYPE_CDATA = 0x03;// ![CDATA[
	const int  DEF_XTT_TAG_TYPE_COMMENT = 0x04;// !--

	typedef struct
	{
		M_BOOL      canfree;
		MS_INT32    size;
		MS_INT32    used;
		MS_INT32    align;
		MS_CHAR    *buf;
	} BUFOBJ;

	typedef enum
	{
		XMLNODE_TYPE_DECLARE    = 0x10,
		XMLNODE_TYPE_ELEMENT    = 0x20,
		XMLNODE_TYPE_ROOT       = 0x21,
		XMLNODE_TYPE_IDATA      = 0x22,
		XMLNODE_TYPE_ATTRIB     = 0x40
	} XMLNodeType;

	typedef struct S_XMLNODE
	{
		XMLNodeType     type;
		MS_CHAR         name[DEF_STR_LEN_MIN];
		BUFOBJ          bufobj;
		MS_CHAR        *data;
		MS_INT32        data_siz;
		MS_INT32        seq;
		MU_INT32        hashid;

		MS_INT32        depth;
		MS_INT32        idata;

		S_XMLNODE        *parent;
		S_XMLNODE        *prev;
		S_XMLNODE        *next;

		S_XMLNODE        *attr;
		S_XMLNODE        *attr_tail;

		S_XMLNODE        *elem;
		S_XMLNODE        *elem_tail;
	}XMLNODE;

	typedef struct
	{
		MS_INT32        size;
		XMLNODE       **node;
		XMLNODE        *decl;
		XMLNODE        *root;
		const MS_CHAR  *bufend;
		MS_INT32        ecode;
	} XML;

	extern MS_INT32 f_xmlparse_init(XML * xml, const MS_CHAR * src);
	extern XMLNODE *f_xmlnode_sch_elem(XMLNODE *const  parent, const MS_CHAR * name);

	static MS_CHAR    *ST_fskip_space(const MS_CHAR * str);
	static MU_INT32    ST_strlcpy(MS_CHAR *const dst, const MS_CHAR *src, MU_INT32 siz);
	static MS_INT32    ST_filt_space_copy(MS_CHAR *const  _src, MS_CHAR *const  _dst, MS_INT32 dst_siz, M_BOOL filt_head_space, M_BOOL filt_tail_space);
	static MS_INT32    ST_filt_token_copy(MS_CHAR *const _src, MS_CHAR *const _dst, const MS_CHAR *token_stt, const MS_CHAR *token_end);
	static MU_INT32	   Hash_Str(const void *  value, MU_INT32 len);
	static BUFOBJ     *f_bufobj_new(BUFOBJ *const bufobj, MS_INT32 size, MS_INT32 align);
	static MS_INT32    f_bufobj_resize(BUFOBJ *const bufobj, MS_INT32 size);
	static void        f_bufobj_del(BUFOBJ *bufobj);
	static void   *mmem_malloc(MU_INT32 siz, MU_INT32 align = 8);
	static void    mmem_free(void *real);
	static MS_CHAR    *ST_bskip_space(const MS_CHAR * str);
	static MS_CHAR *f_xmlnode_chk_name(MS_CHAR * name);
	static XMLNODE *f_xmlnode_new_elem(MS_CHAR * name, const MS_CHAR * data, XMLNodeType type);
	static MS_INT32 f_xmlnode_ins_elem_tail(XMLNODE *const  parent, XMLNODE *const  elem);
	static XMLNODE *f_xmlnode_new_attr(MS_CHAR * name, const MS_CHAR * data);
	static MS_INT32 f_xmlnode_ins_attr_tail(XMLNODE *const  parent, XMLNODE *const  attr);
	static MS_INT32 f_xmlnode_set_node_data(XMLNODE *const  node, const MS_CHAR * data);
	static MS_INT32 f_xmlnode_del_node(XMLNODE *const  node);
	static MS_INT32 f_xmlnode_del(XML * xml);
	static MS_INT32 f_xmlnode_rmv_node(XMLNODE *const  node);
	static XMLNODE *_f_xmlnode_new(XMLNodeType type, MS_CHAR * _name, const MS_CHAR * data);
	static MS_INT32 _f_xmlnode_out_node(const XMLNODE * node, MS_CHAR *const  buf);
	static MS_INT32 _f_xmlnode_siz_node(const XMLNODE * node);
	static MS_CHAR *get_name_end(const MS_CHAR * buf);
	static MS_CHAR *get_next_token(const MS_CHAR * src, MS_INT32 * xtt, MS_INT32 chkeq);
	static XMLNODE *f_xmlparse(XML * xml, const MS_CHAR * buf);
	static MU_INT32 _HashAdd(MU_INT32 h, MU_CHAR c);
}
