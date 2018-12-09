#pragma once
#include "xmltype.h"
#include "xmlutils.h"
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

	extern MS_CHAR *f_xmlnode_chk_name(MS_CHAR * name);
	extern XMLNODE *f_xmlnode_new_elem(MS_CHAR * name, const MS_CHAR * data, XMLNodeType type);
	extern MS_INT32 f_xmlnode_ins_elem_tail(XMLNODE *const  parent, XMLNODE *const  elem);
	extern XMLNODE *f_xmlnode_sch_elem(XMLNODE *const  parent, const MS_CHAR * name);
	extern XMLNODE *f_xmlnode_new_attr(MS_CHAR * name, const MS_CHAR * data);
	extern MS_INT32 f_xmlnode_ins_attr_tail(XMLNODE *const  parent, XMLNODE *const  attr);
	extern MS_INT32 f_xmlnode_set_node_data(XMLNODE *const  node, const MS_CHAR * data);
	extern MS_INT32 f_xmlnode_del_node(XMLNODE *const  node);
	extern MS_INT32 f_xmlnode_del(XML * xml);

	static MS_INT32 f_xmlnode_rmv_node(XMLNODE *const  node);
	static XMLNODE *_f_xmlnode_new(XMLNodeType type, MS_CHAR * _name, const MS_CHAR * data);
	static MS_INT32 _f_xmlnode_out_node(const XMLNODE * node, MS_CHAR *const  buf);
	static MS_INT32 _f_xmlnode_siz_node(const XMLNODE * node);
}

