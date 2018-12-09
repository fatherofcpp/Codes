/*
* All rights reserved.
*
*/
#include "stdafx.h"

#include "mty.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "xmlnode.h"
#include "utils.h"
#include "hash.h"
#include "MMmem.h"

static MS_INT32 xmlnode_used = 0;
static MS_INT32 xmlnode_free = 0;

static MS_INT32 xmlnode_merr = 0;
#define XERR_SET(x) xmlnode_merr = x

#define DEF_STR_LEN_STD         256
#define MAKE_E_XR(c) (c)

namespace NSP_STP_CM
{
	MS_INT32 f_xmlnode_enc(const MS_CHAR * src, MS_CHAR * dst)
	{
		MS_CHAR *save = dst;

		while (*src != 0x00)
		{
			switch (*src)
			{
			case '&':
				memcpy(dst, "&amp;", 5);
				dst += 5;
				break;
			case '>':
				memcpy(dst, "&gt;", 4);
				dst += 4;
				break;
			case '<':
				memcpy(dst, "&lt;", 4);
				dst += 4;
				break;
			case '\'':
				memcpy(dst, "&apos;", 6);
				dst += 6;
				break;
			case '"':
				memcpy(dst, "&quot;", 6);
				dst += 6;
				break;

			default:
				*dst++ = *src;
				break;
			}
			src++;
		}
		*dst = 0x00;

		return (dst - save);
	}

	MS_INT32 f_xmlnode_dec(const MS_CHAR * src, MS_CHAR * dst)
	{
		MS_CHAR *save = dst;

		while (*src != 0x00)
		{
			switch (*src)
			{
			case '&':
				if (memcmp(src, "&amp;", 5) == 0)
				{
					*dst++ = '&';
					src += 5;
				}
				else if (memcmp(src, "&gt;", 4) == 0)
				{
					*dst++ = '>';
					src += 4;
				}
				else if (memcmp(src, "&lt;", 4) == 0)
				{
					*dst++ = '<';
					src += 4;
				}
				else if (memcmp(src, "&apos;", 6) == 0)
				{
					*dst++ = '\'';
					src += 6;
				}
				else if (memcmp(src, "&quot;", 6) == 0)
				{
					*dst++ = '"';
					src += 6;
				}
				else
				{
					*dst++ = *src++;
				}
				break;

			default:
				*dst++ = *src++;
				break;
			}
		}
		*dst = 0x00;

		return (dst - save);
	}

	MS_CHAR *f_xmlnode_chk_name(MS_CHAR * name)
	{
		MS_CHAR *const save = name;
		MS_INT32 flag = 0;

		NSP_STP_CM::ST_filt_space_copy(name, NULL, 0, true, true);
		if ((*name != '_') && (isalpha(*name) == 0))
		{
			XERR_SET(MAKE_E_XR(0x0001));
			return (NULL);
		}

		name++;
		while (*name != 0x00)
		{
			flag = 0;
			if ((*name == '_') || (*name == '-') || (*name == '.') || (*name == ':'))
			{
				flag++;
			}
			if ((isalpha(*name) != 0) || (isdigit(*name) != 0))
			{
				flag++;
			}
			if (flag == 0)
			{
				XERR_SET(MAKE_E_XR(0x0002));
				return (NULL);
			}
			name++;
		}

		return (save);
	}

	static XMLNODE *_f_xmlnode_new(XMLNodeType type, MS_CHAR * _name, const MS_CHAR * data)
	{
		NSP_STP_CM::XMLNODE *node;
		MS_CHAR name[DEF_STR_LEN_MIN];

		if (strlen(_name) >= sizeof(name))
		{
			XERR_SET(MAKE_E_XR(0x0003));
			return (NULL);
		}
		strcpy(name, _name);

		if (NSP_STP_CM::f_xmlnode_chk_name(name) == NULL)
		{
			XERR_SET(MAKE_E_XR(0x0004));
			return (NULL);
		}
		node = (NSP_STP_CM::XMLNODE *)NSP_STP_CM::mmem_malloc(sizeof(NSP_STP_CM::XMLNODE));
		if (node == NULL)
		{
			XERR_SET(MAKE_E_XR(0x0005));
			return (NULL);
		}
		memset(node, 0, sizeof(NSP_STP_CM::XMLNODE));
		NSP_STP_CM::ST_strlcpy(node->name, name, sizeof(node->name));
		if (data != NULL)
		{
			f_xmlnode_set_node_data(node, data);
		}
		else
		{
			f_xmlnode_set_node_data(node, "");
		}
		node->type = type;
		node->hashid = NSP_STP_CM::Hash_Str(name, strlen(name));
		node->seq = xmlnode_used;
		xmlnode_used++;

		return (node);
	}

	XMLNODE *f_xmlnode_new_elem(MS_CHAR * name, const MS_CHAR * data, XMLNodeType type)
	{
		switch (type)
		{
		case NSP_STP_CM::XMLNODE_TYPE_DECLARE:
		case NSP_STP_CM::XMLNODE_TYPE_ROOT:
		case NSP_STP_CM::XMLNODE_TYPE_ELEMENT:
		case NSP_STP_CM::XMLNODE_TYPE_IDATA:
			break;

		default:
			XERR_SET(MAKE_E_XR(0x0006));
			return (NULL);
		}

		return (_f_xmlnode_new(type, name, data));
	}

	XMLNODE *f_xmlnode_new_elem_c(const MS_CHAR * name, const MS_CHAR * data, XMLNodeType type)
	{
		MS_CHAR _name[DEF_STR_LEN_MIN];

		strncpy(_name, name, sizeof(_name)-1);
		_name[sizeof(_name)-1] = 0x00;
		switch (type)
		{
		case NSP_STP_CM::XMLNODE_TYPE_DECLARE:
		case NSP_STP_CM::XMLNODE_TYPE_ROOT:
		case NSP_STP_CM::XMLNODE_TYPE_ELEMENT:
			break;

		default:
			XERR_SET(MAKE_E_XR(0x0007));
			return (NULL);
		}

		return (_f_xmlnode_new(type, _name, data));
	}

	XMLNODE *f_xmlnode_new_elem_ci(const MS_CHAR * name, const MS_INT32 data, XMLNodeType type)
	{
		MS_CHAR _data[DEF_STR_LEN_MIN];

		sprintf(_data, "%d", data);
		switch (type)
		{
		case NSP_STP_CM::XMLNODE_TYPE_DECLARE:
		case NSP_STP_CM::XMLNODE_TYPE_ROOT:
		case NSP_STP_CM::XMLNODE_TYPE_ELEMENT:
			break;

		default:
			XERR_SET(MAKE_E_XR(0x0008));
			return (NULL);
		}

		return (NSP_STP_CM::f_xmlnode_new_elem_c(name, _data, type));
	}

	MS_INT32 f_xmlnode_ins_elem_next(XMLNODE *const  node, XMLNODE *const  elem)
	{
		NSP_STP_CM::XMLNODE *parent = node->parent;

		if ((node->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) != NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			XERR_SET(MAKE_E_XR(0x000D));
			return (-1);
		}
		if ((elem->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) != NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			XERR_SET(MAKE_E_XR(0x0010));
			return (-1);
		}
		if (parent->data_siz > 0)
		{
			XERR_SET(MAKE_E_XR(0x0011));
			return (-1);
		}
		if ((parent->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) != NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			XERR_SET(MAKE_E_XR(0x0012));
			return (-1);
		}

		elem->depth = parent->depth + 1;

		elem->parent = parent;
		elem->prev = node;
		elem->next = node->next;

		node->next->prev = elem;
		node->next = elem;

		return (0);
	}

	MS_INT32 f_xmlnode_ins_elem_head(XMLNODE *const  parent, XMLNODE *const  elem)
	{
		if ((elem->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) != NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			XERR_SET(MAKE_E_XR(0x0009));
			return (-1);
		}
		if (parent->data_siz > 0)
		{
			XERR_SET(MAKE_E_XR(0x000A));
			return (-1);
		}
		if ((parent->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) != NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			XERR_SET(MAKE_E_XR(0x000B));
			return (-1);
		}

		elem->depth = parent->depth + 1;
		if (elem->type == NSP_STP_CM::XMLNODE_TYPE_IDATA)
		{
			parent->idata = 1;
		}

		elem->parent = parent;
		elem->prev = NULL;
		elem->next = parent->elem;

		if (parent->elem != NULL)
		{
			parent->elem->prev = elem;
		}
		parent->elem = elem;

		return (0);
	}

	MS_INT32 f_xmlnode_ins_elem_tail(XMLNODE *const  parent, XMLNODE *const  elem)
	{
		if ((elem->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) != NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			XERR_SET(MAKE_E_XR(0x000C));
			return (-1);
		}
		if (parent->data_siz > 0)
		{
			XERR_SET(MAKE_E_XR(0x000D));
			return (-1);
		}
		if ((parent->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) != NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			XERR_SET(MAKE_E_XR(0x000E));
			return (-1);
		}

		elem->depth = parent->depth + 1;
		if (elem->type == NSP_STP_CM::XMLNODE_TYPE_IDATA)
		{
			parent->idata = 1;
		}

		elem->parent = parent;
		elem->prev = parent->elem_tail;
		elem->next = NULL;

		if (parent->elem == NULL)
		{
			parent->elem = elem;
		}
		else
		{
			parent->elem_tail->next = elem;
		}
		parent->elem_tail = elem;

		return (0);
	}

	XMLNODE *f_xmlnode_get_elem_head(XMLNODE *const  parent)
	{
		return (parent->elem);
	}

	XMLNODE *f_xmlnode_get_elem_tail(XMLNODE *const  parent)
	{
		return (parent->elem_tail);
	}

	XMLNODE *f_xmlnode_sch_elem(XMLNODE *const  parent, const MS_CHAR * name)
	{
		MU_INT32 hashid;
		NSP_STP_CM::XMLNODE *node = parent->elem;

		hashid = NSP_STP_CM::Hash_Str(name, strlen(name));
		while (node != NULL)
		{
			if (node->hashid == hashid)
			{
				if (strcmp(node->name, name) == 0)
				{
					return (node);
				}
			}
			node = node->next;
		}

		return (NULL);
	}

	XMLNODE *f_xmlnode_sch_elem_ext(XMLNODE *const  parent, const MS_CHAR * name, MS_INT32 max_siz)
	{
		NSP_STP_CM::XMLNODE *node = NULL;

		node = NSP_STP_CM::f_xmlnode_sch_elem(parent, name);
		if (node == NULL)
		{
			XERR_SET(MAKE_E_XR(0x000F));
			return (NULL);
		}

		if (node->data_siz >= max_siz)
		{
			XERR_SET(MAKE_E_XR(0x0010));
			return (NULL);
		}

		return (node);
	}

	XMLNODE *f_xmlnode_new_attr(MS_CHAR * name, const MS_CHAR * data)
	{
		return (_f_xmlnode_new(NSP_STP_CM::XMLNODE_TYPE_ATTRIB, name, data));
	}

	XMLNODE *f_xmlnode_new_attr_c(const MS_CHAR * name, const MS_CHAR * data)
	{
		MS_CHAR _name[DEF_STR_LEN_MIN];

		strncpy(_name, name, sizeof(_name)-1);
		_name[sizeof(_name)-1] = 0x00;

		return (_f_xmlnode_new(NSP_STP_CM::XMLNODE_TYPE_ATTRIB, _name, data));
	}

	XMLNODE *f_xmlnode_new_attr_ci(const MS_CHAR * name, const MS_INT32 data)
	{
		MS_CHAR _data[DEF_STR_LEN_MIN];

		sprintf(_data, "%d", data);

		return (f_xmlnode_new_attr_c(name, _data));
	}

	MS_INT32 f_xmlnode_ins_attr_head(XMLNODE *const  parent, XMLNODE *const  attr)
	{
		if (attr->type != NSP_STP_CM::XMLNODE_TYPE_ATTRIB)
		{
			XERR_SET(MAKE_E_XR(0x0011));
			return (-1);
		}
		switch (parent->type)
		{
		case NSP_STP_CM::XMLNODE_TYPE_DECLARE:
		case NSP_STP_CM::XMLNODE_TYPE_ELEMENT:
		case NSP_STP_CM::XMLNODE_TYPE_ROOT:
			break;

		default:
			XERR_SET(MAKE_E_XR(0x0012));
			return (-1);
		}

		attr->depth = parent->depth + 1;

		attr->parent = parent;
		attr->prev = NULL;
		attr->next = parent->attr;

		if (parent->attr != NULL)
		{
			parent->attr->prev = attr;
		}
		parent->attr = attr;

		return (0);
	}

	MS_INT32 f_xmlnode_ins_attr_tail(XMLNODE *const  parent, XMLNODE *const  attr)
	{
		if (attr->type != NSP_STP_CM::XMLNODE_TYPE_ATTRIB)
		{
			XERR_SET(MAKE_E_XR(0x0013));
			return (-1);
		}
		switch (parent->type)
		{
		case NSP_STP_CM::XMLNODE_TYPE_DECLARE:
		case NSP_STP_CM::XMLNODE_TYPE_ELEMENT:
		case NSP_STP_CM::XMLNODE_TYPE_ROOT:
			break;

		default:
			XERR_SET(MAKE_E_XR(0x0014));
			return (-1);
		}

		attr->depth = parent->depth + 1;
		attr->parent = parent;
		attr->prev = parent->attr_tail;
		attr->next = NULL;

		if (parent->attr == NULL)
		{
			parent->attr = attr;
		}
		else
		{
			parent->attr_tail->next = attr;
		}
		parent->attr_tail = attr;

		return (0);
	}

	XMLNODE *f_xmlnode_get_attr_head(XMLNODE *const  parent)
	{
		return (parent->attr);
	}

	XMLNODE *f_xmlnode_get_attr_tail(XMLNODE *const  parent)
	{
		return (parent->attr_tail);
	}

	XMLNODE *f_xmlnode_sch_attr(XMLNODE *const  parent, const MS_CHAR * name)
	{
		MU_INT32 hashid;
		NSP_STP_CM::XMLNODE *node = parent->attr;

		hashid = NSP_STP_CM::Hash_Str(name, strlen(name));
		while (node != NULL)
		{
			if (node->hashid == hashid)
			{
				if (strcmp(node->name, name) == 0)
				{
					return (node);
				}
			}
			node = node->next;
		}

		return (NULL);
	}

	XMLNODE *f_xmlnode_sch_attr_ext(XMLNODE *const  parent, const MS_CHAR * name, MS_INT32 max_siz)
	{
		NSP_STP_CM::XMLNODE *node = NULL;

		node = NSP_STP_CM::f_xmlnode_sch_attr(parent, name);
#if 0
		printf("f_xmlnode_chk_attr: parent: %08X name: %s max_siz: %d node: %08X\n", (MU_INT32)parent, name, max_siz, (MU_INT32)node);
#endif

		if (node == NULL)
		{
			XERR_SET(MAKE_E_XR(0x0015));
			return (NULL);
		}

		if (node->data_siz >= max_siz)
		{
			XERR_SET(MAKE_E_XR(0x0016));
			return (NULL);
		}

		return (node);
	}

	XMLNODE *f_xmlnode_nxt_node(XMLNODE *const  node, const MS_CHAR * name)
	{
		MU_INT32 hashid;
		NSP_STP_CM::XMLNODE *tnode = node->next;

		if (name == NULL)
		{
			return (tnode);
		}

		hashid = NSP_STP_CM::Hash_Str(name, strlen(name));
		while (tnode != NULL)
		{
			if (tnode->hashid == hashid)
			{
				if (strcmp(tnode->name, name) == 0)
				{
					return (tnode);
				}
			}
			tnode = tnode->next;
		}

		return (NULL);
	}

	MS_INT32 f_xmlnode_set_node_data(XMLNODE *const  node, const MS_CHAR * data)
	{
		MS_INT32 data_siz;

		if (node->elem != NULL)
		{
			XERR_SET(MAKE_E_XR(0x0017));
			return (-1);
		}

		data_siz = strlen(data);
		if (node->data == NULL)
		{
			if (f_bufobj_new(&node->bufobj, data_siz + 1, 32) == NULL)
			{
				XERR_SET(MAKE_E_XR(0x0018));
				return (-1);
			}
			node->data = node->bufobj.buf;
			node->data_siz = data_siz;
		}

		if (data_siz >= node->data_siz)
		{
			if (f_bufobj_resize(&node->bufobj, data_siz + 1) != 0)
			{
				XERR_SET(MAKE_E_XR(0x0019));
				return (-1);
			}
			node->data = node->bufobj.buf;
			node->data_siz = data_siz;
		}

		node->bufobj.used = data_siz;

		memcpy(node->data, data, node->data_siz);
		node->data[node->data_siz] = 0x00;

		return (0);
	}

	MS_INT32 f_xmlnode_rmv_node(XMLNODE *const  node)
	{
		if (node->prev != NULL)
		{
			node->prev->next = node->next;
		}
		if (node->next != NULL)
		{
			node->next->prev = node->prev;
		}

		if (node->parent != NULL)
		{
			if ((node->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) == NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
			{
				if (node->parent->elem == node)
				{
					node->parent->elem = node->next;
				}
				if (node->parent->elem_tail == node)
				{
					node->parent->elem_tail = node->prev;
				}
			}
			else if (node->type == NSP_STP_CM::XMLNODE_TYPE_ATTRIB)
			{
				if (node->parent->attr == node)
				{
					node->parent->attr = node->next;
				}
				if (node->parent->attr_tail == node)
				{
					node->parent->attr_tail = node->prev;
				}
			}
		}

		node->depth = 0;
		node->parent = NULL;
		node->next = NULL;

		return (0);
	}

	MS_INT32 f_xmlnode_del_node(XMLNODE *const  node)
	{
		NSP_STP_CM::XMLNODE *tnode;
		NSP_STP_CM::XMLNODE *snode;

		NSP_STP_CM::f_xmlnode_rmv_node(node);

		tnode = node->attr;
		node->attr = NULL;
		while (tnode != NULL)
		{
			snode = tnode->next;
#if 0
			printf("FREE ATTR: %08X SEQ: %8d HASHID: %08X NAME: [%s]\n", (MU_INT32)tnode, tnode->seq, tnode->hashid, tnode->name);
#endif
			f_bufobj_del(&tnode->bufobj);
			memset(tnode, 0, sizeof(NSP_STP_CM::XMLNODE));
			M_FREE(tnode);
			xmlnode_free++;
			tnode = snode;
		}

		tnode = node->elem;
		node->elem = NULL;
		while (tnode != NULL)
		{
			snode = tnode->next;
			NSP_STP_CM::f_xmlnode_del_node(tnode);
			tnode = snode;
		}

		xmlnode_free++;
#if 0
		printf("FREE ELEM: %08X SEQ: %8d HASHID: %08X NAME: [%s]\n", (MU_INT32)node, node->seq, node->hashid, node->name);
#endif
		f_bufobj_del(&node->bufobj);
		memset(node, 0, sizeof(NSP_STP_CM::XMLNODE));
		mmem_free(node);

		return (0);
	}

	static MS_INT32 _f_xmlnode_out_decl(const XMLNODE * node, MS_CHAR *const  buf)
	{
		NSP_STP_CM::XMLNODE *tnode;
		NSP_STP_CM::XMLNODE *snode;
		MS_INT32 len;

		len = 0;
		len += sprintf(buf+len, "<?%s", node->name);

		tnode = node->attr;
		while (tnode != NULL)
		{
			snode = tnode->next;
			len += sprintf(buf+len, " %s=\"%s\"", tnode->name, tnode->data);
			tnode = snode;
		}
		len += sprintf(buf+len, " ?>\n");

		return (len);
	}

	static MS_INT32 _f_xmlnode_out_node(const XMLNODE * node, MS_CHAR *const  buf)
	{
		NSP_STP_CM::XMLNODE *tnode;
		NSP_STP_CM::XMLNODE *snode;
		MS_INT32 len;
		MS_CHAR obuf[DEF_STR_LEN_STD];

		memset(obuf, ' ', sizeof(obuf));
		obuf[sizeof(obuf)-1] = 0x00;
		len = 0;
		if (node->type == NSP_STP_CM::XMLNODE_TYPE_IDATA)
		{
			len += sprintf(buf+len, "%s%s\n", &obuf[sizeof(obuf) - 1 - (node->depth * 4)], node->data);
			return (len);
		}
		len += sprintf(buf+len, "%s<%s", &obuf[sizeof(obuf) - 1 - (node->depth * 4)], node->name);

		tnode = node->attr;
		while (tnode != NULL)
		{
			snode = tnode->next;
			len += sprintf(buf+len, " %s=\"%s\"", tnode->name, tnode->data);
			tnode = snode;
		}
		if (node->elem == NULL)
		{
			if (node->data_siz == 0)
			{
				len += sprintf(buf+len, " />\n");
			}
			else
			{
				len += sprintf(buf+len, ">%s</%s>\n", node->data, node->name);
			}
		}
		else
		{
			len += sprintf(buf+len, ">\n");

			tnode = node->elem;
			while (tnode != NULL)
			{
				snode = tnode->next;
				len += _f_xmlnode_out_node(tnode, buf+len);
				tnode = snode;
			}
			len += sprintf(buf+len, "%s</%s>\n", &obuf[sizeof(obuf) - 1 - (node->depth * 4)], node->name);
		}

		return (len);
	}

	MS_INT32 f_xmlnode_out_node(const XMLNODE * node, MS_CHAR *const  buf)
	{
		MS_INT32 len = 0;

		if ((node->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) == NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			len = _f_xmlnode_out_node(node, buf);
		}
		else if (node->type == NSP_STP_CM::XMLNODE_TYPE_DECLARE)
		{
			len = _f_xmlnode_out_decl(node, buf);
		}
		else
		{
			abort();
		}

		return (len);
	}

	MS_INT32 f_xmlnode_out(const XML * xml, MS_CHAR *const  buf)
	{
		MS_INT32 i;
		MS_INT32 len;

		len = 0;
		for (i=0; i<xml->size; i++)
		{
			len += NSP_STP_CM::f_xmlnode_out_node(xml->node[i], buf+len);
		}

		return (len);
	}

	static MS_INT32 _f_xmlnode_siz_decl(const XMLNODE * node)
	{
		NSP_STP_CM::XMLNODE *tnode;
		NSP_STP_CM::XMLNODE *snode;
		MS_INT32 len;

		len = 0;
		len += 2 + strlen(node->name);

		tnode = node->attr;
		while (tnode != NULL)
		{
			snode = tnode->next;
			len += 1 + strlen(tnode->name) + 1 + 1 + tnode->data_siz + 1;
			tnode = snode;
		}
		len += 4;

		return (len);
	}

	static MS_INT32 _f_xmlnode_siz_node(const XMLNODE * node)
	{
		NSP_STP_CM::XMLNODE *tnode;
		NSP_STP_CM::XMLNODE *snode;
		MS_INT32 len;
		MS_CHAR obuf[DEF_STR_LEN_STD];

		memset(obuf, ' ', sizeof(obuf));
		obuf[sizeof(obuf)-1] = 0x00;
		len = 0;
		if (node->type == NSP_STP_CM::XMLNODE_TYPE_IDATA)
		{
			len += (node->depth * 4) + node->data_siz + 1;
			return (len);
		}
		len += (node->depth * 4) + 1 + strlen(node->name);

		tnode = node->attr;
		while (tnode != NULL)
		{
			snode = tnode->next;
			len += 1 + strlen(tnode->name) + 1 + 1 + tnode->data_siz + 1;
			tnode = snode;
		}
		if (node->elem == NULL)
		{
			if (node->data_siz == 0)
			{
				len += 4;
			}
			else
			{
				len += 1 + node->data_siz + 2 + strlen(node->name) + 2;
			}
		}
		else
		{
			len += 2;
			tnode = node->elem;
			while (tnode != NULL)
			{
				snode = tnode->next;
				len += _f_xmlnode_siz_node(tnode);
				tnode = snode;
			}
			len += ( node->depth * 4) + 2 + strlen(node->name) + 2;
		}

		return (len);
	}

	MS_INT32 f_xmlnode_siz_node(const XMLNODE * node)
	{
		MS_INT32 len = 0;

		if ((node->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) == NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			len = _f_xmlnode_siz_node(node);
		}
		else if (node->type == NSP_STP_CM::XMLNODE_TYPE_DECLARE)
		{
			len = _f_xmlnode_siz_decl(node);
		}
		else
		{
			abort();
		}

		return (len);
	}

	MS_INT32 f_xmlnode_siz(const XML * xml)
	{
		MS_INT32 i;
		MS_INT32 len;

		len = 0;
		for (i=0; i<xml->size; i++)
		{
			len += f_xmlnode_siz_node(xml->node[i]);
		}

		return (len);
	}

	MS_INT32 f_xmlnode_del(XML * xml)
	{
		MS_INT32 i;

		for (i=0; i<xml->size; i++)
		{
			f_xmlnode_del_node(xml->node[i]);
		}
		M_FREE(xml->node);

		return (0);
	}

	XMLNODE * f_xmlnode_get_elem_next( XMLNODE *const  node )
	{
		return (node->next);
	}

}
