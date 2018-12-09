#pragma once
#include "mty.h"
#include "bufobj.h"
namespace NSP_STP_CM
{
const int DEF_STR_LEN_MIN = 64;
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

	MS_INT32 f_xmlnode_enc(const MS_CHAR * src, MS_CHAR * dst);

	MS_INT32 f_xmlnode_dec(const MS_CHAR * src, MS_CHAR * dst);
	extern MS_CHAR *f_xmlnode_chk_name(MS_CHAR * name);

	extern XMLNODE *f_xmlnode_new_elem(MS_CHAR * name, const MS_CHAR * data, XMLNodeType type);
	XMLNODE *f_xmlnode_new_elem_c(const MS_CHAR * name, const MS_CHAR * data, XMLNodeType type);
	XMLNODE *f_xmlnode_new_elem_ci(const MS_CHAR * name, const MS_INT32 data, XMLNodeType type);
	MS_INT32 f_xmlnode_ins_elem_next(XMLNODE *const  node, XMLNODE *const  elem);
	MS_INT32 f_xmlnode_ins_elem_head(XMLNODE *const  parent, XMLNODE *const  elem);
	extern MS_INT32 f_xmlnode_ins_elem_tail(XMLNODE *const  parent, XMLNODE *const  elem);
	XMLNODE *f_xmlnode_get_elem_head(XMLNODE *const  parent);
	XMLNODE *f_xmlnode_get_elem_tail(XMLNODE *const  parent);
	XMLNODE *f_xmlnode_get_elem_next(XMLNODE *const  node);
	extern XMLNODE *f_xmlnode_sch_elem(XMLNODE *const  parent, const MS_CHAR * name);
	XMLNODE *f_xmlnode_sch_elem_ext(XMLNODE *const  parent, const MS_CHAR * name, MS_INT32 max_siz);

	extern XMLNODE *f_xmlnode_new_attr(MS_CHAR * name, const MS_CHAR * data);
	XMLNODE *f_xmlnode_new_attr_c(const MS_CHAR * name, const MS_CHAR * data);
	XMLNODE *f_xmlnode_new_attr_ci(const MS_CHAR * name, const MS_INT32 data);
	MS_INT32 f_xmlnode_ins_attr_head(XMLNODE *const  parent, XMLNODE *const  attr);
	extern MS_INT32 f_xmlnode_ins_attr_tail(XMLNODE *const  parent, XMLNODE *const  attr);
	XMLNODE *f_xmlnode_get_attr_head(XMLNODE *const  parent);
	XMLNODE *f_xmlnode_get_attr_tail(XMLNODE *const  parent);
	XMLNODE *f_xmlnode_sch_attr(XMLNODE *const  parent, const MS_CHAR * name);
	XMLNODE *f_xmlnode_sch_attr_ext(XMLNODE *const  parent, const MS_CHAR * name, MS_INT32 max_siz);

	extern MS_INT32 f_xmlnode_set_node_data(XMLNODE *const  node, const MS_CHAR * data);
	XMLNODE *f_xmlnode_nxt_node(XMLNODE *const  node, const MS_CHAR * name);
	MS_INT32 f_xmlnode_rmv_node(XMLNODE *const  node);
	extern MS_INT32 f_xmlnode_del_node(XMLNODE *const  node);
	MS_INT32 f_xmlnode_out_node(const XMLNODE * node, MS_CHAR *const  buf);
	MS_INT32 f_xmlnode_siz_node(const XMLNODE * node);

	MS_INT32 f_xmlnode_out(const XML * xml, MS_CHAR *const  buf);
	MS_INT32 f_xmlnode_siz(const XML * xml);
	extern MS_INT32 f_xmlnode_del(XML * xml);
}

