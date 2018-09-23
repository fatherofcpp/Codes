/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: xmlparse.c 1942 2009-06-19 07:12:14Z dengxf $
 */
#include "../../stdafx.h"
#include "../include/mty.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/xmlnode.h"
#include "../include/xmlparse.h"
#include "../include/utils.h"
#include "../include/xr.h"
#include "../include/mmem.h"

#define DEF_XTT_NONE                0
#define DEF_XTT_TAG_START           0x10    // <
#define DEF_XTT_TAG_CLOSE           0x20    // >
#define DEF_XTT_EQUAL               0x30    // =

#define DEF_XTT_TAG_TYPE_END        0x01    // /
#define DEF_XTT_TAG_TYPE_DECL       0x02    // ?
#define DEF_XTT_TAG_TYPE_CDATA      0x03    // ![CDATA[
#define DEF_XTT_TAG_TYPE_COMMENT    0x04    // !--

#undef XR_DEV
#define XR_DEV XR_DEV_XMLPARSE
static MS_CHAR *get_name_end(const MS_CHAR *M_RESTRICT buf)
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
                return ((MS_CHAR *)(M_MOVECONST(buf)));

            default:
                buf++;
                break;
        }
    }

    return ((MS_CHAR *)(M_MOVECONST(buf)));
}

static MS_CHAR *get_next_token(const MS_CHAR *M_RESTRICT src, MS_INT32 *M_RESTRICT xtt, MS_INT32 chkeq)
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
            return ((MS_CHAR *)(M_MOVECONST(buf)));
        }
        buf++;
    }

    return ((MS_CHAR *)(M_MOVECONST(buf)));
}

static XMLNODE *f_xmlparse(XML *M_RESTRICT xml, const MS_CHAR *M_RESTRICT buf)
{
    XMLNODE *root = NULL;
    XMLNODE *parent = NULL;
    XMLNODE *node = NULL;
    const MS_CHAR *ps;
    const MS_CHAR *pe;
    const MS_CHAR *psn;
    MS_CHAR name[DEF_STR_LEN_MIN];
    MS_CHAR data[DEF_BUF_LEN_MIN];
    MU_INT32 len;
    MS_CHAR match;
    MS_INT32 xtt = DEF_XTT_NONE;
    MS_INT32 chkeq = 1;

    xml->ecode = 0;
    xml->bufend = NULL;

    if (*buf != '<')
    {
        xml->ecode = MAKE_E_XR(0x0001);
        return (NULL);
    }

    ps = buf;
    psn = NULL;

    while (1)
    {
        node = NULL;
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
                if (len >= sizeof(data))
                {
                    xml->ecode = MAKE_E_XR(0x0002);
                    goto ERR;
                }
                if (len > 0)
                {
                    memcpy(data, ps, len);
                    data[len] = 0x00;

                    len = ST_filt_token_copy(data, NULL, "<!--", "-->");
                    len = ST_filt_space_copy(data, NULL, 0, M_TRUE, M_TRUE);
                }
                if (len > 0)
                {
                    node = f_xmlnode_new_elem(parent->name, NULL, XMLNODE_TYPE_IDATA);
                    if (node == NULL)
                    {
                        xml->ecode = MAKE_E_XR(0x0003);
                        goto ERR;
                    }
                    if (f_xmlnode_set_node_data(node, data) != 0)
                    {
                        xml->ecode = MAKE_E_XR(0x0004);
                        goto ERR;
                    }
                    if (f_xmlnode_ins_elem_tail(parent, node) != 0)
                    {
                        xml->ecode = MAKE_E_XR(0x0005);
                        goto ERR;
                    }
                }

                ps = pe + 1;
                pe = get_name_end(ps);
                len = pe - ps;
                if (len == 0)
                {
                    xml->ecode = MAKE_E_XR(0x0006);
                    goto ERR;
                }
                if (len >= sizeof(name))
                {
                    xml->ecode = MAKE_E_XR(0x0007);
                    goto ERR;
                }
                memcpy(name, ps, len);
                name[len] = 0x00;
                ps = pe;
                node = f_xmlnode_new_elem(name, NULL, (root == NULL) ? XMLNODE_TYPE_ROOT : XMLNODE_TYPE_ELEMENT);
                if (node == NULL)
                {
                    xml->ecode = MAKE_E_XR(0x0008);
                    goto ERR;
                }
                if (root == NULL)
                {
                    root = node;
                    parent = root;
                }
                else
                {
                    if (f_xmlnode_ins_elem_tail(parent, node) != 0)
                    {
                        xml->ecode = MAKE_E_XR(0x0009);
                        goto ERR;
                    }
                    parent = node;
                }
                break;

            case DEF_XTT_TAG_START | DEF_XTT_TAG_TYPE_END: // </
                if (root == NULL)
                {
                    xml->ecode = MAKE_E_XR(0x000A);
                    goto ERR;
                }
                if (pe > ps)
                {
                    len = pe - ps;
                    if (len >= sizeof(data))
                    {
                        xml->ecode = MAKE_E_XR(0x000B);
                        goto ERR;
                    }
                    if (len > 0)
                    {
                        memcpy(data, ps, len);
                        data[len] = 0x00;

                        len = ST_filt_token_copy(data, NULL, "<!--", "-->");
                        len = ST_filt_space_copy(data, NULL, 0, M_TRUE, M_TRUE);
                    }
                    if (len > 0)
                    {
                        if ((parent->elem == NULL) && (parent->idata == 0))
                        {
                            if (f_xmlnode_set_node_data(parent, data) != 0)
                            {
                                xml->ecode = MAKE_E_XR(0x000C);
                                goto ERR;
                            }
                        }
                        else
                        {
                            node = f_xmlnode_new_elem(parent->name, NULL, XMLNODE_TYPE_IDATA);
                            if (node == NULL)
                            {
                                xml->ecode = MAKE_E_XR(0x000D);
                                goto ERR;
                            }
                            if (f_xmlnode_set_node_data(node, data) != 0)
                            {
                                xml->ecode = MAKE_E_XR(0x000E);
                                goto ERR;
                            }
                            if (f_xmlnode_ins_elem_tail(parent, node) != 0)
                            {
                                xml->ecode = MAKE_E_XR(0x000F);
                                goto ERR;
                            }
                        }
                    }
                }
                ps = pe + 2;
                pe = get_name_end(ps);
                len = pe - ps;
                if (len == 0)
                {
                    xml->ecode = MAKE_E_XR(0x0010);
                    goto ERR;
                }
                if (len >= sizeof(name))
                {
                    xml->ecode = MAKE_E_XR(0x0011);
                    goto ERR;
                }
                memcpy(name, ps, len);
                name[len] = 0x00;

                if (f_xmlnode_chk_name(name) == NULL)
                {
                    xml->ecode = MAKE_E_XR(0x0012);
                    goto ERR;
                }

                pe = ST_fskip_space(pe);
                if (*pe != '>')
                {
                    xml->ecode = MAKE_E_XR(0x0013);
                    goto ERR;
                }
                if (strcmp(parent->name, name) != 0)
                {
                    xml->ecode = MAKE_E_XR(0x0014);
                    goto ERR;
                }
                ps = pe + 1;
                parent = parent->parent;
                break;

            case DEF_XTT_TAG_START | DEF_XTT_TAG_TYPE_CDATA: // <![CDATA[
                psn = strstr(pe+9, "]]>");
                if (psn == NULL)
                {
                    xml->ecode = MAKE_E_XR(0x0015);
                    goto ERR;
                }
                psn += 3;
                break;

            case DEF_XTT_TAG_START | DEF_XTT_TAG_TYPE_COMMENT: // <!--
                psn = strstr(pe + 4, "--");
                if (psn == NULL)
                {
                    xml->ecode = MAKE_E_XR(0x0016);
                    goto ERR;
                }
                if (*(psn+2) != '>')
                {
                    xml->ecode = MAKE_E_XR(0x0017);
                    goto ERR;
                }
                psn += 3;
                break;

            case DEF_XTT_TAG_START | DEF_XTT_TAG_TYPE_DECL: // <?
                if (root != NULL)
                {
                    xml->ecode = MAKE_E_XR(0x0018);
                    goto ERR;
                }
                ps = pe + 2;
                pe = get_name_end(ps);
                len = pe - ps;
                if (len == 0)
                {
                    xml->ecode = MAKE_E_XR(0x0019);
                    goto ERR;
                }
                if (len >= sizeof(name))
                {
                    xml->ecode = MAKE_E_XR(0x001A);
                    goto ERR;
                }
                memcpy(name, ps, len);
                name[len] = 0x00;
                ps = pe;
                root = f_xmlnode_new_elem(name, NULL, XMLNODE_TYPE_DECLARE);
                if (root == NULL)
                {
                    xml->ecode = MAKE_E_XR(0x001B);
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
                    xml->ecode = MAKE_E_XR(0x001C);
                    goto ERR;
                }
                memcpy(name, ps, len);
                name[len] = 0x00;
                ps = ST_fskip_space(pe + 1);
                match = *ps;
                ps++;
                if ((match != '\'') && (match != '"'))
                {
                    xml->ecode = MAKE_E_XR(0x001D);
                    goto ERR;
                }
                pe = strchr(ps, match);
                if (pe == NULL)
                {
                    xml->ecode = MAKE_E_XR(0x001E);
                    goto ERR;
                }
                len = pe - ps;
                if (len >= sizeof(data))
                {
                    xml->ecode = MAKE_E_XR(0x001F);
                    goto ERR;
                }
                memcpy(data, ps, len);
                data[len] = 0x00;
                ps = pe + 1;
                node = f_xmlnode_new_attr(name, data);
                if (node == NULL)
                {
                    xml->ecode = MAKE_E_XR(0x0020);
                    goto ERR;
                }
                if (f_xmlnode_ins_attr_tail(parent, node) != 0)
                {
                    xml->ecode = MAKE_E_XR(0x0021);
                    goto ERR;
                }
                break;

            default:
                xml->ecode = MAKE_E_XR(0x0022);
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
        xml->ecode = MAKE_E_XR(0x0023);
        goto ERR;
    }

    if (root == NULL)
    {
        xml->ecode = MAKE_E_XR(0x0024);
        goto ERR;
    }

    return (root);

ERR:
	if (root != NULL)
	{
		f_xmlnode_del_node(root);
	}
    
	if (node != NULL)
	{
		f_xmlnode_del_node(node);
	}
    

	return NULL;
}

MS_INT32 f_xmlparse_init(XML *M_RESTRICT xml, const MS_CHAR *M_RESTRICT buf)
{
    XMLNODE **pnode;

    memset(xml, 0, sizeof(XML));
    while (*buf != 0x00)
    {
        pnode = (XMLNODE **)M_MALLOC((xml->size+1) * sizeof(XMLNODE *));
        if (pnode == NULL)
        {
            xml->ecode = MAKE_E_XR(0x0025);
            goto ERR;
        }
        if (xml->node != NULL)
        {
            memcpy(pnode, xml->node, xml->size * sizeof(XMLNODE *));
            M_FREE(xml->node);
        }
        xml->node = pnode;
        xml->size += 1;

        if (xml->size == 1)
        {
            
            MU_CHAR S_UTF8[4] = {0xEF, 0xBB, 0xBF, 0x3C};
            if (memcmp(buf, S_UTF8, 3) == 0)
            {
                buf += 3;
            }
        }

        xml->node[xml->size - 1] = f_xmlparse(xml, buf);
        if (xml->node[xml->size - 1] == NULL)
        {
            goto ERR;
        }
        buf = ST_fskip_space(xml->bufend);
        xml->bufend = NULL;

        switch (xml->node[xml->size - 1]->type)
        {
            case XMLNODE_TYPE_DECLARE:
                if (xml->decl != NULL)
                {
                    xml->ecode = MAKE_E_XR(0x0026);
                    goto ERR;
                }
                xml->decl = xml->node[xml->size - 1];
                break;

            case XMLNODE_TYPE_ROOT:
                if (xml->root != NULL)
                {
                    xml->ecode = MAKE_E_XR(0x0027);
                    goto ERR;
                }
                xml->root = xml->node[xml->size - 1];
                break;

            default:
                break;
        }
    }

    if (xml->node == NULL)
    {
        xml->ecode = MAKE_E_XR(0x0028);
        goto ERR;
    }



    return (0);

ERR:
	/*
	char temp[1024];
	memset(temp,0,sizeof(temp));
	f_xmlnode_out(xml,temp);
	FILE *p=fopen("1.txt","wb+");
	fwrite(temp,1000,1,p);
	fclose(p);   
*/
	f_xmlnode_del(xml);


    return (xml->ecode);
}

#undef XR_DEV
