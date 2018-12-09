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

#include <string>
#pragma warning (disable: 4127)

namespace NSP_STP_CM
{
	class MXml
	{
	private:
#define M_FREE(mm)                          \
		{                                           \
		if (mm != NULL)                         \
		{                                       \
		free(mm);                      \
		mm = NULL;                          \
	}                                       \
	}

#define DEF_STR_LEN_MIN  64
		#define DEF_STR_LEN_STD  256
		#define DEF_BUFOBJ_SIZE_MAX  (5*1024 * 1024)

		#define DEF_XTT_NONE  0
		#define DEF_XTT_TAG_START  0x10 // <
		#define DEF_XTT_TAG_CLOSE  0x20// >
		#define DEF_XTT_EQUAL  0x30// =
		#define DEF_XTT_TAG_TYPE_END 0x01// /
		#define DEF_XTT_TAG_TYPE_DECL  0x02// ?
		#define DEF_XTT_TAG_TYPE_CDATA  0x03// ![CDATA[
		#define DEF_XTT_TAG_TYPE_COMMENT  0x04// !--
	public:
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
	public:
		MXml(){};
		~MXml(){};
	public:
		static MXml* GetInstance()
		{
			static MXml obj;
			return &obj;
		}

		MS_INT32 f_xmlparse_init(XML * xml, const MS_CHAR * buf)
		{
			XMLNODE **pnode;

			memset(xml, 0, sizeof(XML));
			while (*buf != 0x00)
			{
				pnode = (XMLNODE **)mmem_malloc((xml->size+1) * sizeof(XMLNODE *));
				if (pnode == NULL)
				{
					xml->ecode = (0x0026);
					goto ERR;
				}
				if (xml->node != NULL)
				{
					memcpy(pnode, xml->node, xml->size * sizeof(XMLNODE *));
					M_FREE(xml->node);
				}
				xml->node = pnode;

				if (xml->size == 0)
				{
					/* BOM */
					MU_CHAR S_UTF8[4] = {0xEF, 0xBB, 0xBF, 0x3C};
					if (memcmp(buf, S_UTF8, 3) == 0)
					{
						buf += 3;
					}
				}

				xml->node[xml->size] = f_xmlparse(xml, buf);
				if (xml->node[xml->size] == NULL)
				{
					goto ERR;
				}
				buf = ST_fskip_space(xml->bufend);
				xml->bufend = NULL;

				switch (xml->node[xml->size]->type)
				{
				case XMLNODE_TYPE_DECLARE:
					if (xml->decl != NULL)
					{
						xml->ecode = (0x0027);
						goto ERR;
					}
					xml->decl = xml->node[xml->size];
					break;

				case XMLNODE_TYPE_ROOT:
					if (xml->root != NULL)
					{
						xml->ecode = (0x0028);
						goto ERR;
					}
					xml->root = xml->node[xml->size];
					break;

				default:
					break;
				}

				xml->size++;
			}

			if (xml->node == NULL)
			{
				xml->ecode = (0x0029);
				goto ERR;
			}

			return (0);

ERR:
			f_xmlnode_del(xml);

			return (xml->ecode);
		}

		XMLNODE *f_xmlnode_sch_elem(XMLNODE *const  parent, const MS_CHAR * name)
		{
			MU_INT32 hashid;
			XMLNODE *node = parent->elem;

			hashid = Hash_Str(name, strlen(name));
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

	protected:

		MS_CHAR *ST_fskip_space(const MS_CHAR * str)
		{
			while (*str != 0x00)
			{
				switch (*str)
				{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					str++;
					break;

				default:
					return ((MS_CHAR *)str);
					break;
				}
			}
			return ((MS_CHAR *)str);
		}

		MS_CHAR *ST_bskip_space(const MS_CHAR * str)
		{
			const MS_CHAR *last = NULL;

			while (*str != 0x00)
			{
				switch (*str)
				{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
					break;

				default:
					last = str;
					break;
				}
				str++;
			}
			if (last == NULL)
			{
				last = str - 1;
			}
			return ((MS_CHAR *)(last+1));
		}

		MU_INT32 ST_strlcpy(MS_CHAR *const dst, const MS_CHAR *src, MU_INT32 siz)
		{
			MS_CHAR *d = dst;
			const MS_CHAR *s = src;
			MU_INT32 n = siz;

			/* Copy as many bytes as will fit */
			if (n != 0 && --n != 0)
			{
				do
				{
					if ((*d++ = *s++) == 0)
					{
						break;
					}
				} while (--n != 0);
			}

			/* Not enough room in dst, add NUL and traverse rest of src */
			if (n == 0)
			{
				if (siz != 0)
				{
					*d = '\0';		/* NUL-terminate dst */
				}
				while (*s++)
				{
					;
				}
			}

			return (s - src - 1);	/* count does not include NUL */
		}

		MS_INT32 ST_filt_space_copy(MS_CHAR *const  _src, MS_CHAR *const  _dst, MS_INT32 dst_siz, M_BOOL filt_head_space, M_BOOL filt_tail_space)
		{
			MS_INT32 len;
			MS_CHAR *dst = _dst;
			MS_CHAR *src = _src;
			MS_CHAR *end = _src;

			if (_dst == NULL)
			{
				dst = src;
			}

			if (filt_head_space == true)
			{
				src = ST_fskip_space(src);
			}
			if (filt_tail_space == true)
			{
				end = ST_bskip_space(src);
				len = end - src;
			}
			else
			{
				len = strlen(src);
				end = src + len;
			}
			if ((dst_siz > 0) && (len > dst_siz))
			{
				len = dst_siz;
				end = src + len;
			}
			if ((dst > src) && (dst < end))
			{
				return (-1);
			}
			if (dst != src)
			{
				memcpy(dst, src, len);
			}
			dst[len] = 0x00;

			return (len);
		}

		MS_INT32 ST_filt_token_copy(MS_CHAR *const _src, MS_CHAR *const _dst, const MS_CHAR *token_stt, const MS_CHAR *token_end)
		{
			MS_CHAR *src = _src;
			MS_CHAR *dst = _dst;
			MS_CHAR *save = NULL;
			MS_CHAR *p = NULL;
			MS_INT32 len;
			MS_INT32 elen;

			if (dst == NULL)
			{
				dst = src;
			}
			len = strlen(src);
			if ((dst > src) && (dst < src + len))
			{
				return (-1);
			}

			save = dst;
			elen = strlen(token_end);
			while (1)
			{
				p = strstr(src, token_stt);
				if (p == NULL)
				{
					len = strlen(src);
					if (src != dst)
					{
						memcpy(dst, src, len);
					}
					dst += len;
					break;
				}
				else
				{
					if (p > src)
					{
						len = p - src;
						if (src != dst)
						{
							memcpy(dst, src, len);
						}
						dst += len;
					}

					p = strstr(p, token_end);
					if (p == NULL)
					{
						return (-1);
					}
					p += elen;

					src = p;
				}
			}
			*dst = 0x00;

			return (dst - save);
		}

		MU_INT32 _HashAdd(MU_INT32 h, MU_CHAR c)
		{
			h += (h << 5);
			return (h ^ c);
		}

		MU_INT32 Hash_Str(const void *  value, MU_INT32 len)
		{
			MU_INT32 h;
			const MS_INT8 *p = (const MS_INT8 *)value;

			h = 5381;
			while (len != 0)
			{
				h = _HashAdd(h, *p++);
				--len;
			}
			return (h);
		}

		BUFOBJ *f_bufobj_new(BUFOBJ *const _bufobj, MS_INT32 size, MS_INT32 align)
		{
			BUFOBJ *bufobj = _bufobj;

			if ((size < 0) || (size > DEF_BUFOBJ_SIZE_MAX))
			{
				return (NULL);
			}

			switch (align)
			{
			case 4:
			case 8:
			case 16:
			case 32:
			case 64:
			case 128:
			case 256:
			case 512:
			case 1024:
			case 2048:
			case 4096:
				break;

			default:
				return (NULL);
				break;
			}

			if (bufobj == NULL)
			{
				bufobj = (BUFOBJ *)mmem_malloc(sizeof(BUFOBJ));
				if (bufobj == NULL)
				{
					return (NULL);
				}
				memset(bufobj, 0, sizeof(BUFOBJ));
				bufobj->canfree = true;
			}
			else
			{
				memset(bufobj, 0, sizeof(BUFOBJ));
				bufobj->canfree = false;
			}
			bufobj->align = align;

			if (size > 0)
			{
				bufobj->size = size + bufobj->align - (size % bufobj->align);
				bufobj->buf = (MS_CHAR *)mmem_malloc(bufobj->size);
				if (bufobj->buf == NULL)
				{
					if (_bufobj == NULL)
					{
						M_FREE(bufobj);
					}
					return (NULL);
				}
			}

			return (bufobj);
		}

		MS_INT32 f_bufobj_resize(BUFOBJ *const bufobj, MS_INT32 size)
		{
			MS_CHAR *buf;

			if ((size < 0) || (size > DEF_BUFOBJ_SIZE_MAX))
			{
				return (-1);
			}

			size += bufobj->align - (size % bufobj->align);
			if (size <= bufobj->size)
			{
				return (0);
			}

			buf = (MS_CHAR *)mmem_malloc(size);
			if (buf == NULL)
			{
				return (-1);
			}

			if (size > bufobj->size)
			{
				memcpy(buf, bufobj->buf, bufobj->size);
				memset(buf + bufobj->size, 0, size - bufobj->size);
			}

			M_FREE(bufobj->buf);
			bufobj->buf = buf;
			bufobj->size = size;

			return (0);
		}

		void f_bufobj_del(BUFOBJ *bufobj)
		{
			M_FREE(bufobj->buf);
			if (bufobj->canfree == true)
			{
				M_FREE(bufobj);
			}
		}

		void   *mmem_malloc(MU_INT32 siz, MU_INT32 alignment = 8)
		{
			alignment;
			return (void *)malloc(siz);
		}

		void    mmem_free(void *real)
		{
			free(real);
		}

		MS_CHAR *f_xmlnode_chk_name(MS_CHAR * name)
		{
			MS_CHAR *const save = name;
			MS_INT32 flag = 0;

			ST_filt_space_copy(name, NULL, 0, true, true);
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
			XMLNODE *node;
			MS_CHAR name[DEF_STR_LEN_MIN];

			if (strlen(_name) >= sizeof(name))
			{
				return (NULL);
			}
			strcpy(name, _name);

			if (f_xmlnode_chk_name(name) == NULL)
			{
				return (NULL);
			}
			node = (XMLNODE *)mmem_malloc(sizeof(XMLNODE));
			if (node == NULL)
			{
				return (NULL);
			}
			memset(node, 0, sizeof(XMLNODE));
			ST_strlcpy(node->name, name, sizeof(node->name));
			if (data != NULL)
			{
				f_xmlnode_set_node_data(node, data);
			}
			else
			{
				f_xmlnode_set_node_data(node, "");
			}
			node->type = type;
			node->hashid = Hash_Str(name, strlen(name));
			return (node);
		}

		XMLNODE *f_xmlnode_new_elem(MS_CHAR * name, const MS_CHAR * data, XMLNodeType type)
		{
			switch (type)
			{
			case XMLNODE_TYPE_DECLARE:
			case XMLNODE_TYPE_ROOT:
			case XMLNODE_TYPE_ELEMENT:
			case XMLNODE_TYPE_IDATA:
				break;

			default:
				return (NULL);
			}

			return (_f_xmlnode_new(type, name, data));
		}

		MS_INT32 f_xmlnode_ins_elem_tail(XMLNODE *const  parent, XMLNODE *const  elem)
		{
			if ((elem->type & XMLNODE_TYPE_ELEMENT) != XMLNODE_TYPE_ELEMENT)
			{
				return (-1);
			}
			if (parent->data_siz > 0)
			{
				return (-1);
			}
			if ((parent->type & XMLNODE_TYPE_ELEMENT) != XMLNODE_TYPE_ELEMENT)
			{
				return (-1);
			}

			elem->depth = parent->depth + 1;
			if (elem->type == XMLNODE_TYPE_IDATA)
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

		XMLNODE *f_xmlnode_new_attr(MS_CHAR * name, const MS_CHAR * data)
		{
			return (_f_xmlnode_new(XMLNODE_TYPE_ATTRIB, name, data));
		}

		MS_INT32 f_xmlnode_ins_attr_tail(XMLNODE *const  parent, XMLNODE *const  attr)
		{
			if (attr->type != XMLNODE_TYPE_ATTRIB)
			{
				return (-1);
			}
			switch (parent->type)
			{
			case XMLNODE_TYPE_DECLARE:
			case XMLNODE_TYPE_ELEMENT:
			case XMLNODE_TYPE_ROOT:
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
				if ((node->type & XMLNODE_TYPE_ELEMENT) == XMLNODE_TYPE_ELEMENT)
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
				else if (node->type == XMLNODE_TYPE_ATTRIB)
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
			XMLNODE *tnode;
			XMLNODE *snode;

			f_xmlnode_rmv_node(node);

			tnode = node->attr;
			node->attr = NULL;
			while (tnode != NULL)
			{
				snode = tnode->next;
				f_bufobj_del(&tnode->bufobj);
				memset(tnode, 0, sizeof(XMLNODE));
				M_FREE(tnode);
				tnode = snode;
			}

			tnode = node->elem;
			node->elem = NULL;
			while (tnode != NULL)
			{
				snode = tnode->next;
				f_xmlnode_del_node(tnode);
				tnode = snode;
			}

			f_bufobj_del(&node->bufobj);
			memset(node, 0, sizeof(XMLNODE));
			mmem_free(node);

			return (0);
		}

		MS_INT32 _f_xmlnode_out_node(const XMLNODE * node, MS_CHAR *const  buf)
		{
			XMLNODE *tnode;
			XMLNODE *snode;
			MS_INT32 len;
			MS_CHAR obuf[DEF_STR_LEN_STD];

			memset(obuf, ' ', sizeof(obuf));
			obuf[sizeof(obuf)-1] = 0x00;
			len = 0;
			if (node->type == XMLNODE_TYPE_IDATA)
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
			XMLNODE *tnode;
			XMLNODE *snode;
			MS_INT32 len;
			MS_CHAR obuf[DEF_STR_LEN_STD];

			memset(obuf, ' ', sizeof(obuf));
			obuf[sizeof(obuf)-1] = 0x00;
			len = 0;
			if (node->type == XMLNODE_TYPE_IDATA)
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

		MS_CHAR *get_name_end(const MS_CHAR * buf)
		{
			while (*buf != 0x00)
			{
				switch (*buf)
				{
				case ' ':
				case '\t':
				case '\r':
				case '\n':
				case '>':
				case '/':
				case '?':
				case '<':
				case '=':
					return ((MS_CHAR *)(buf));

				default:
					buf++;
					break;
				}
			}

			return ((MS_CHAR *)(buf));
		}

		MS_CHAR *get_next_token(const MS_CHAR * src, MS_INT32 * xtt, MS_INT32 chkeq)
		{
			MS_CHAR pc;
			MS_CHAR c;
			MS_CHAR nc;
			const MS_CHAR *buf = src;

			*xtt = 0;
			while (*buf != 0x00)
			{
				pc = 0x00;
				if (buf > src)
				{
					pc = *(buf-1);
				}
				c = *buf;
				nc = *(buf+1);

				switch (c)
				{
				case '<':
					*xtt = DEF_XTT_TAG_START;
					switch (nc)
					{
					case '/':   /* </ */
						*xtt |= DEF_XTT_TAG_TYPE_END;
						break;

					case '?':   /* <? */
						*xtt |= DEF_XTT_TAG_TYPE_DECL;
						break;

					case '!':   /* <! */
						if (strncmp(buf, "<![CDATA[", 9) == 0)
						{
							*xtt |= DEF_XTT_TAG_TYPE_CDATA;
						}
						else if (strncmp(buf, "<!--", 4) == 0)
						{
							*xtt |= DEF_XTT_TAG_TYPE_COMMENT;
						}
						break;

					default:
						break;
					}
					break;

				case '>':   /* >  */
					*xtt = DEF_XTT_TAG_CLOSE;
					switch (pc)
					{
					case '/':
						*xtt |= DEF_XTT_TAG_TYPE_END;
						buf--;
						break;

					case '?':
						*xtt |= DEF_XTT_TAG_TYPE_DECL;
						buf--;
						break;

					default:
						break;
					}
					break;

				case '=':
					if (chkeq == 1)
					{
						*xtt = DEF_XTT_EQUAL;
					}
					break;

				default:
					break;
				}

				if (*xtt != DEF_XTT_NONE)
				{
					return ((MS_CHAR *)(buf));
				}
				buf++;
			}

			return ((MS_CHAR *)(buf));
		}

		XMLNODE *f_xmlparse(XML * xml, const MS_CHAR * buf)
		{
			XMLNODE *root = NULL;
			XMLNODE *parent = NULL;
			XMLNODE *node = NULL;
			const MS_CHAR *ps;
			const MS_CHAR *pe;
			const MS_CHAR *psn;
			MS_CHAR name[DEF_STR_LEN_MIN];
			BUFOBJ bufobj;
			MU_INT32 len;
			MS_CHAR match;
			MS_INT32 xtt = DEF_XTT_NONE;
			MS_INT32 chkeq = 1;

			xml->ecode = 0;
			xml->bufend = NULL;

			if (*buf != '<')
			{
				xml->ecode = (0x0001);
				return (NULL);
			}
			if (f_bufobj_new(&bufobj, 256, 4) == NULL)
			{
				xml->ecode = (0x0002);
				goto ERR;
			}

			ps = buf;
			psn = NULL;

			while (1)
			{
				ps = ST_fskip_space(ps);
				if (*ps == 0x00)
				{
					break;
				}
				if (psn == NULL)
				{
					pe = get_next_token(ps, &xtt, chkeq);
				}
				else
				{
					pe = get_next_token(psn, &xtt, chkeq);
					psn = NULL;
				}
				chkeq = 1;
				switch (xtt)
				{
				case DEF_XTT_TAG_START: // <
					len = pe - ps;
					if (len > 0)
					{
						if (f_bufobj_resize(&bufobj, len + 1) != 0)
						{
							xml->ecode = (0x0003);
							goto ERR;
						}

						memcpy(bufobj.buf, ps, len);
						bufobj.buf[len] = 0x00;

						len = ST_filt_token_copy(bufobj.buf, NULL, "<!--", "-->");
						len = ST_filt_space_copy(bufobj.buf, NULL, 0, true, true);
					}
					if (len > 0)
					{
						node = f_xmlnode_new_elem(parent->name, NULL, XMLNODE_TYPE_IDATA);
						if (node == NULL)
						{
							xml->ecode = (0x0004);
							goto ERR;
						}
						if (f_xmlnode_set_node_data(node, bufobj.buf) != 0)
						{
							xml->ecode = (0x0005);
							goto ERR;
						}
						if (f_xmlnode_ins_elem_tail(parent, node) != 0)
						{
							xml->ecode = (0x0006);
							goto ERR;
						}
						node = NULL;
					}

					ps = pe + 1;
					pe = get_name_end(ps);
					len = pe - ps;
					if (len == 0)
					{
						xml->ecode = (0x0007);
						goto ERR;
					}
					if (len >= sizeof(name))
					{
						xml->ecode = (0x0008);
						goto ERR;
					}
					memcpy(name, ps, len);
					name[len] = 0x00;
					ps = pe;
					node = f_xmlnode_new_elem(name, NULL, (root == NULL) ? XMLNODE_TYPE_ROOT : XMLNODE_TYPE_ELEMENT);
					if (node == NULL)
					{
						xml->ecode = (0x0009);
						goto ERR;
					}
					if (root == NULL)
					{
						root = node;
					}
					else
					{
						if (f_xmlnode_ins_elem_tail(parent, node) != 0)
						{
							xml->ecode = (0x000A);
							goto ERR;
						}
					}
					parent = node;
					node = NULL;
					break;

				case DEF_XTT_TAG_START | DEF_XTT_TAG_TYPE_END: // </
					if (root == NULL)
					{
						xml->ecode = (0x000B);
						goto ERR;
					}
					if (pe > ps)
					{
						len = pe - ps;
						if (len > 0)
						{
							if (f_bufobj_resize(&bufobj, len + 1) != 0)
							{
								xml->ecode = (0x000C);
								goto ERR;
							}

							memcpy(bufobj.buf, ps, len);
							bufobj.buf[len] = 0x00;

							len = ST_filt_token_copy(bufobj.buf, NULL, "<!--", "-->");
							len = ST_filt_space_copy(bufobj.buf, NULL, 0, true, true);
						}
						if (len > 0)
						{
							if ((parent->elem == NULL) && (parent->idata == 0))
							{
								if (f_xmlnode_set_node_data(parent, bufobj.buf) != 0)
								{
									xml->ecode = (0x000D);
									goto ERR;
								}
							}
							else
							{
								node = f_xmlnode_new_elem(parent->name, NULL, XMLNODE_TYPE_IDATA);
								if (node == NULL)
								{
									xml->ecode = (0x000E);
									goto ERR;
								}
								if (f_xmlnode_set_node_data(node, bufobj.buf) != 0)
								{
									xml->ecode = (0x000F);
									goto ERR;
								}
								if (f_xmlnode_ins_elem_tail(parent, node) != 0)
								{
									xml->ecode = (0x0010);
									goto ERR;
								}
								node = NULL;
							}
						}
					}
					ps = pe + 2;
					pe = get_name_end(ps);
					len = pe - ps;
					if (len == 0)
					{
						xml->ecode = (0x0011);
						goto ERR;
					}
					if (len >= sizeof(name))
					{
						xml->ecode = (0x0012);
						goto ERR;
					}
					memcpy(name, ps, len);
					name[len] = 0x00;

					if (f_xmlnode_chk_name(name) == NULL)
					{
						xml->ecode = (0x0013);
						goto ERR;
					}

					pe = ST_fskip_space(pe);
					if (*pe != '>')
					{
						xml->ecode = (0x0014);
						goto ERR;
					}
					if (strcmp(parent->name, name) != 0)
					{
						xml->ecode = (0x0015);
						goto ERR;
					}
					ps = pe + 1;
					parent = parent->parent;
					break;

				case DEF_XTT_TAG_START | DEF_XTT_TAG_TYPE_CDATA: // <![CDATA[
					psn = strstr(pe+9, "]]>");
					if (psn == NULL)
					{
						xml->ecode = (0x0016);
						goto ERR;
					}
					psn += 3;
					break;

				case DEF_XTT_TAG_START | DEF_XTT_TAG_TYPE_COMMENT: // <!--
					psn = strstr(pe + 4, "--");
					if (psn == NULL)
					{
						xml->ecode = (0x0017);
						goto ERR;
					}
					if (*(psn+2) != '>')
					{
						xml->ecode = (0x0018);
						goto ERR;
					}
					psn += 3;
					break;

				case DEF_XTT_TAG_START | DEF_XTT_TAG_TYPE_DECL: // <?
					if (root != NULL)
					{
						xml->ecode = (0x0019);
						goto ERR;
					}
					ps = pe + 2;
					pe = get_name_end(ps);
					len = pe - ps;
					if (len == 0)
					{
						xml->ecode = (0x001A);
						goto ERR;
					}
					if (len >= sizeof(name))
					{
						xml->ecode = (0x001B);
						goto ERR;
					}
					memcpy(name, ps, len);
					name[len] = 0x00;
					ps = pe;
					root = f_xmlnode_new_elem(name, NULL, XMLNODE_TYPE_DECLARE);
					if (root == NULL)
					{
						xml->ecode = (0x001C);
						goto ERR;
					}
					parent = root;
					break;

				case DEF_XTT_TAG_CLOSE: // >
					ps = pe + 1;;
					chkeq = 0;
					break;

				case DEF_XTT_TAG_CLOSE | DEF_XTT_TAG_TYPE_END: // />
				case DEF_XTT_TAG_CLOSE | DEF_XTT_TAG_TYPE_DECL: // ?>
					ps = pe + 2;;
					parent = parent->parent;
					break;

				case DEF_XTT_EQUAL:
					len = pe - ps;
					if ((len == 0) || (len >= sizeof(name)))
					{
						xml->ecode = (0x001D);
						goto ERR;
					}
					memcpy(name, ps, len);
					name[len] = 0x00;
					ps = ST_fskip_space(pe + 1);
					match = *ps;
					ps++;
					if ((match != '\'') && (match != '"'))
					{
						xml->ecode = (0x001E);
						goto ERR;
					}
					pe = strchr(ps, match);
					if (pe == NULL)
					{
						xml->ecode = (0x001F);
						goto ERR;
					}
					len = pe - ps;
					if (f_bufobj_resize(&bufobj, len + 1) != 0)
					{
						xml->ecode = (0x0020);
						goto ERR;
					}

					memcpy(bufobj.buf, ps, len);
					bufobj.buf[len] = 0x00;
					ps = pe + 1;
					node = f_xmlnode_new_attr(name, bufobj.buf);
					if (node == NULL)
					{
						xml->ecode = (0x0021);
						goto ERR;
					}
					if (f_xmlnode_ins_attr_tail(parent, node) != 0)
					{
						xml->ecode = (0x0022);
						goto ERR;
					}
					node = NULL;
					break;

				default:
					xml->ecode = (0x0023);
					goto ERR;
				}
				if (parent == NULL)
				{
					break;
				}
			}
			xml->bufend = ps;

			if (parent != NULL)
			{
				xml->ecode = (0x0024);
				goto ERR;
			}

			if (root == NULL)
			{
				xml->ecode = (0x0025);
				goto ERR;
			}
			f_bufobj_del(&bufobj);

			return (root);

ERR:
			f_bufobj_del(&bufobj);

			if (root != NULL)
			{
				f_xmlnode_del_node(root);
			}

			if (node != NULL)
			{
				f_xmlnode_del_node(node);
			}

			return (NULL);
		}
	};
}
