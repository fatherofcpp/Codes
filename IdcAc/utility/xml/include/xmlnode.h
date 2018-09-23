/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: xmlnode.h 1943 2009-06-19 09:57:49Z dengxf $
 */
#include "mty.h"

#include "bufobj.h"

#ifndef _XMLNODE_H_
#define _XMLNODE_H_

extern MS_INT32 xmlnode_merr;
#define XERR_GET    xmlnode_merr

typedef struct S_XMLNODE XMLNODE;

typedef enum
{
    XMLNODE_TYPE_DECLARE    = 0x10,
    XMLNODE_TYPE_ELEMENT    = 0x20,
    XMLNODE_TYPE_ROOT       = 0x21,
    XMLNODE_TYPE_IDATA      = 0x22,
    XMLNODE_TYPE_ATTRIB     = 0x40
} XMLNodeType;

struct S_XMLNODE
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

    XMLNODE        *parent;
    XMLNODE        *prev;
    XMLNODE        *next;

    XMLNODE        *attr;
    XMLNODE        *attr_tail;

    XMLNODE        *elem;
    XMLNODE        *elem_tail;
};

typedef struct
{
    MS_INT32        size;
    XMLNODE       **node;
    XMLNODE        *decl;
    XMLNODE        *root;

    const MS_CHAR  *bufend;
    MS_INT32        ecode;
} XML;

MS_INT32 f_xmlnode_enc(const MS_CHAR *M_RESTRICT src, MS_CHAR *M_RESTRICT dst);

MS_INT32 f_xmlnode_dec(const MS_CHAR *M_RESTRICT src, MS_CHAR *M_RESTRICT dst);
MS_CHAR *f_xmlnode_chk_name(MS_CHAR *M_RESTRICT name);

XMLNODE *f_xmlnode_new_elem(MS_CHAR *M_RESTRICT name, const MS_CHAR *M_RESTRICT data, XMLNodeType type);
XMLNODE *f_xmlnode_new_elem_c(const MS_CHAR *M_RESTRICT name, const MS_CHAR *M_RESTRICT data, XMLNodeType type);
XMLNODE *f_xmlnode_new_elem_ci(const MS_CHAR *M_RESTRICT name, const MS_INT32 data, XMLNodeType type);
MS_INT32 f_xmlnode_ins_elem_next(XMLNODE *const M_RESTRICT node, XMLNODE *const M_RESTRICT elem);
MS_INT32 f_xmlnode_ins_elem_head(XMLNODE *const M_RESTRICT parent, XMLNODE *const M_RESTRICT elem);
MS_INT32 f_xmlnode_ins_elem_tail(XMLNODE *const M_RESTRICT parent, XMLNODE *const M_RESTRICT elem);
XMLNODE *f_xmlnode_get_elem_head(XMLNODE *const M_RESTRICT parent);
XMLNODE *f_xmlnode_get_elem_tail(XMLNODE *const M_RESTRICT parent);
XMLNODE *f_xmlnode_get_elem_next(XMLNODE *const M_RESTRICT node);//add by jic,查询当前节点的下一个节点
XMLNODE *f_xmlnode_sch_elem(XMLNODE *const M_RESTRICT parent, const MS_CHAR *M_RESTRICT name);
XMLNODE *f_xmlnode_sch_elem_ext(XMLNODE *const M_RESTRICT parent, const MS_CHAR *M_RESTRICT name, MS_INT32 max_siz);

XMLNODE *f_xmlnode_new_attr(MS_CHAR *M_RESTRICT name, const MS_CHAR *M_RESTRICT data);
XMLNODE *f_xmlnode_new_attr_c(const MS_CHAR *M_RESTRICT name, const MS_CHAR *M_RESTRICT data);
XMLNODE *f_xmlnode_new_attr_ci(const MS_CHAR *M_RESTRICT name, const MS_INT32 data);
MS_INT32 f_xmlnode_ins_attr_head(XMLNODE *const M_RESTRICT parent, XMLNODE *const M_RESTRICT attr);
MS_INT32 f_xmlnode_ins_attr_tail(XMLNODE *const M_RESTRICT parent, XMLNODE *const M_RESTRICT attr);
XMLNODE *f_xmlnode_get_attr_head(XMLNODE *const M_RESTRICT parent);
XMLNODE *f_xmlnode_get_attr_tail(XMLNODE *const M_RESTRICT parent);
XMLNODE *f_xmlnode_sch_attr(XMLNODE *const M_RESTRICT parent, const MS_CHAR *M_RESTRICT name);
XMLNODE *f_xmlnode_sch_attr_ext(XMLNODE *const M_RESTRICT parent, const MS_CHAR *M_RESTRICT name, MS_INT32 max_siz);

MS_INT32 f_xmlnode_set_node_data(XMLNODE *const M_RESTRICT node, const MS_CHAR *M_RESTRICT data);
XMLNODE *f_xmlnode_nxt_node(XMLNODE *const M_RESTRICT node, const MS_CHAR *M_RESTRICT name);
MS_INT32 f_xmlnode_rmv_node(XMLNODE *const M_RESTRICT node);
MS_INT32 f_xmlnode_del_node(XMLNODE *const M_RESTRICT node);
MS_INT32 f_xmlnode_out_node(const XMLNODE *M_RESTRICT node, MS_CHAR *const M_RESTRICT buf);
MS_INT32 f_xmlnode_siz_node(const XMLNODE *M_RESTRICT node);

MS_INT32 f_xmlnode_out(const XML *M_RESTRICT xml, MS_CHAR *const M_RESTRICT buf);
MS_INT32 f_xmlnode_siz(const XML *M_RESTRICT xml);
MS_INT32 f_xmlnode_del(XML *M_RESTRICT xml);

#endif

