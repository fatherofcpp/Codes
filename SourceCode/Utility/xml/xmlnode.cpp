#include "xmlnode.h"
namespace NSP_STP_CM
{
const int DEF_STR_LEN_STD = 256;
#define MAKE_E_XR(c) (c)

	MS_CHAR *f_xmlnode_chk_name(MS_CHAR * name)
	{
		MS_CHAR *const save = name;
		MS_INT32 flag = 0;

		NSP_STP_CM::ST_filt_space_copy(name, NULL, 0, true, true);
		if ((*name != '_') && (isalpha(*name) == 0))
		{
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
				return (NULL);
			}
			name++;
		}

		return (save);
	}

	XMLNODE *_f_xmlnode_new(XMLNodeType type, MS_CHAR * _name, const MS_CHAR * data)
	{
		NSP_STP_CM::XMLNODE *node;
		MS_CHAR name[DEF_STR_LEN_MIN];

		if (strlen(_name) >= sizeof(name))
		{
			return (NULL);
		}
		strcpy(name, _name);

		if (NSP_STP_CM::f_xmlnode_chk_name(name) == NULL)
		{
			return (NULL);
		}
		node = (NSP_STP_CM::XMLNODE *)NSP_STP_CM::mmem_malloc(sizeof(NSP_STP_CM::XMLNODE));
		if (node == NULL)
		{
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
			return (NULL);
		}

		return (_f_xmlnode_new(type, name, data));
	}

	MS_INT32 f_xmlnode_ins_elem_tail(XMLNODE *const  parent, XMLNODE *const  elem)
	{
		if ((elem->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) != NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
			return (-1);
		}
		if (parent->data_siz > 0)
		{
			return (-1);
		}
		if ((parent->type & NSP_STP_CM::XMLNODE_TYPE_ELEMENT) != NSP_STP_CM::XMLNODE_TYPE_ELEMENT)
		{
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

	XMLNODE *f_xmlnode_new_attr(MS_CHAR * name, const MS_CHAR * data)
	{
		return (_f_xmlnode_new(NSP_STP_CM::XMLNODE_TYPE_ATTRIB, name, data));
	}

	MS_INT32 f_xmlnode_ins_attr_tail(XMLNODE *const  parent, XMLNODE *const  attr)
	{
		if (attr->type != NSP_STP_CM::XMLNODE_TYPE_ATTRIB)
		{
			return (-1);
		}
		switch (parent->type)
		{
		case NSP_STP_CM::XMLNODE_TYPE_DECLARE:
		case NSP_STP_CM::XMLNODE_TYPE_ELEMENT:
		case NSP_STP_CM::XMLNODE_TYPE_ROOT:
			break;

		default:
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

	MS_INT32 f_xmlnode_set_node_data(XMLNODE *const  node, const MS_CHAR * data)
	{
		MS_INT32 data_siz;

		if (node->elem != NULL)
		{
			return (-1);
		}

		data_siz = strlen(data);
		if (node->data == NULL)
		{
			if (f_bufobj_new(&node->bufobj, data_siz + 1, 32) == NULL)
			{
				return (-1);
			}
			node->data = node->bufobj.buf;
			node->data_siz = data_siz;
		}

		if (data_siz >= node->data_siz)
		{
			if (f_bufobj_resize(&node->bufobj, data_siz + 1) != 0)
			{
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

#if 0
		printf("FREE ELEM: %08X SEQ: %8d HASHID: %08X NAME: [%s]\n", (MU_INT32)node, node->seq, node->hashid, node->name);
#endif
		f_bufobj_del(&node->bufobj);
		memset(node, 0, sizeof(NSP_STP_CM::XMLNODE));
		mmem_free(node);

		return (0);
	}

	MS_INT32 _f_xmlnode_out_node(const XMLNODE * node, MS_CHAR *const  buf)
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

	MS_INT32 _f_xmlnode_siz_node(const XMLNODE * node)
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

}
