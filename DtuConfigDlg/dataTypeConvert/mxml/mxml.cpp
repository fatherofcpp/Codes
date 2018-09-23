#include "stdafx.h"
#include "mxml.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//    porting modify func
//
//-----------------------------------------------------------------------------------------------
#define mxml_trace TRACE
#define mxml_err_trace TRACE

static void* mxml_malloc(unsigned int size)
{
	void *p;

	p = malloc(size);
	if (p)
	{
		memset(p,0,size);
	}

	return p;
}

static void mxml_free(void *p)
{
	return free(p);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
#define MXML_EOF            (-1)

#define MXML_ADD_BEFORE	    (0)	        /* Add node before specified node */
#define MXML_ADD_AFTER	    (1)	        /* Add node after specified node */
#define MXML_ADD_TO_PARENT	(NULL)	    /* Add node relative to parent */

typedef int (*_mxml_getc_cb_t)(void *);
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
static void* mxml_realloc(void *p, unsigned int old_size, unsigned int new_size)
{
	void* p1;

	p1 = mxml_malloc(new_size);
	if (p1)
	{
		memcpy(p1, p, old_size);
		mxml_free(p);
	}

	return p1;
}

static char* mxml_strdup(const char* src)
{
    unsigned int len = 0;
    char* new_str = NULL;

    if(src == NULL)
    {
        return NULL;
    }
    
    len = strlen(src);
    if(len == 0)
    {
        return NULL;
    }

    new_str = (char*)mxml_malloc(len + 1);
    if(new_str)
    {
        strncpy(new_str,src,len);
    }
    return new_str;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
static inline int mxml_isspace(int ch)
{
    return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n');
}

static inline int mxml_bad_char(int ch)
{
    return ((ch) < ' ' && (ch) != '\n' && (ch) != '\r' && (ch) != '\t');
}

static int mxml_add_char(int ch, char **bufptr, char **buffer, int  *bufsize)
{
    char *newbuffer;	
	char pre_bufsize;

    if (*bufptr >= (*buffer + *bufsize - 4))
    {
		pre_bufsize=*bufsize;

        if (*bufsize < 1024)
            (*bufsize) *= 2;
        else
            (*bufsize) += 1024;

        if ((newbuffer = (char*)mxml_realloc(*buffer, pre_bufsize, *bufsize)) == NULL)
        {
            mxml_free(*buffer);

            mxml_err_trace("Unable to expand string buffer to %d bytes!", *bufsize);

            return (-1);
        }

        *bufptr = newbuffer + (*bufptr - *buffer);
        *buffer = newbuffer;
    }

    if (ch < 0x80)
    {
        /*
        * Single byte ASCII...
        */

        *(*bufptr)++ = ch;
    }
    else if (ch < 0x800)
    {
        /*
        * Two-byte UTF-8...
        */

        *(*bufptr)++ = 0xc0 | (ch >> 6);
        *(*bufptr)++ = 0x80 | (ch & 0x3f);
    }
    else if (ch < 0x10000)
    {
        /*
        * Three-byte UTF-8...
        */

        *(*bufptr)++ = 0xe0 | (ch >> 12);
        *(*bufptr)++ = 0x80 | ((ch >> 6) & 0x3f);
        *(*bufptr)++ = 0x80 | (ch & 0x3f);
    }
    else
    {
        /*
        * Four-byte UTF-8...
        */

        *(*bufptr)++ = 0xf0 | (ch >> 18);
        *(*bufptr)++ = 0x80 | ((ch >> 12) & 0x3f);
        *(*bufptr)++ = 0x80 | ((ch >> 6) & 0x3f);
        *(*bufptr)++ = 0x80 | (ch & 0x3f);
    }

    return (0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
void mxmlRemove(mxml_node_t *node)
{
    mxml_trace("mxmlRemove(node=%p)\n", node);

    if (!node || !node->parent)
        return;

    /*
    * Remove from parent...
    */
    if (node->prev)
        node->prev->next = node->next;
    else
        node->parent->child = node->next;

    if (node->next)
        node->next->prev = node->prev;
    else
        node->parent->last_child = node->prev;

    node->parent = NULL;
    node->prev   = NULL;
    node->next   = NULL;
}

void mxmlDelete(mxml_node_t *node)
{
    if (!node)
        return;

    /*
    * Remove the node from its parent, if any...
    */
    mxmlRemove(node);

    /*
    * Delete children...
    */
    while (node->child)
        mxmlDelete(node->child);

    /*
    * Now delete any node data...
    */
    switch (node->type)
    {
    case MXML_ELEMENT :
        if (node->value.element)
            mxml_free(node->value.element);
        break;
    case MXML_INTEGER :
        /* Nothing to do */
        break;

    case MXML_TEXT :
        if (node->value.text)
            mxml_free(node->value.text);
        break;

    default :
        break;
    }

    /*
    * Free this node...
    */
    mxml_free(node);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
void
mxmlAdd(mxml_node_t *parent,    /* I - Parent node */
        int         where,		/* I - Where to add, MXML_ADD_BEFORE or MXML_ADD_AFTER */
        mxml_node_t *child,		/* I - Child node for where or MXML_ADD_TO_PARENT */
        mxml_node_t *node)		/* I - Node to add */
{
    mxml_trace("mxmlAdd(parent=%p, where=%d, child=%p, node=%p)\n", parent,
            where, child, node);

    if (!parent || !node)
        return;

    /*
    * Remove the node from any existing parent...
    */
    if (node->parent)
        mxmlRemove(node);

    /*
    * Reset pointers...
    */
    node->parent = parent;

    switch (where)
    {
    case MXML_ADD_BEFORE :
        if (!child || child == parent->child || child->parent != parent)
        {
            /*
            * Insert as first node under parent...
            */
            node->next = parent->child;

            if (parent->child)
                parent->child->prev = node;
            else
                parent->last_child = node;

            parent->child = node;
        }
        else
        {
            /*
            * Insert node before this child...
            */
            node->next = child;
            node->prev = child->prev;

            if (child->prev)
                child->prev->next = node;
            else
                parent->child = node;

            child->prev = node;
        }
        break;

    case MXML_ADD_AFTER :
        if (!child || child == parent->last_child || child->parent != parent)
        {
            /*
            * Insert as last node under parent...
            */

            node->parent = parent;
            node->prev   = parent->last_child;

            if (parent->last_child)
                parent->last_child->next = node;
            else
                parent->child = node;

            parent->last_child = node;
        }
        else
        {
            /*
            * Insert node after this child...
            */
            node->prev = child;
            node->next = child->next;

            if (child->next)
                child->next->prev = node;
            else
                parent->last_child = node;

            child->next = node;
        }
        break;
    }
}

static mxml_node_t *			/* O - New node */
mxml_new(mxml_node_t *parent,	/* I - Parent node */
         mxml_type_t type)		/* I - Node type */
{
    mxml_node_t	*node;			/* New node */

    mxml_trace("mxml_new(parent=%p, type=%d)\n", parent, type);

    if ((node = (mxml_node_t *)mxml_malloc(sizeof(mxml_node_t))) == NULL)
    {
        mxml_trace(" returning NULL\n");

        return (NULL);
    }
    
    mxml_trace(" returning %p\n", node);

    node->type      = type;

    if (parent)
        mxmlAdd(parent, MXML_ADD_AFTER, MXML_ADD_TO_PARENT, node);

    return (node);
}

mxml_node_t *				        /* O - New node */
mxmlNewInteger(mxml_node_t *parent,	/* I - Parent node or MXML_NO_PARENT */
               int         integer)	/* I - Integer value */
{
    mxml_node_t	*node;			    /* New node */

    mxml_trace("mxmlNewInteger(parent=%p, integer=%d)\n", parent, integer);

    if ((node = mxml_new(parent, MXML_INTEGER)) != NULL)
        node->value.integer = integer;

    return (node);
}

mxml_node_t *				        /* O - New node */
mxmlNewText(mxml_node_t *parent,	/* I - Parent node or MXML_NO_PARENT */
            int         whitespace,	/* I - 1 = leading whitespace, 0 = no whitespace */
            const char  *string)	/* I - String */
{
    mxml_node_t	*node;			    /* New node */

    mxml_trace("mxmlNewText(parent=%p, whitespace=%d, string=\"%s\")\n",
            parent, whitespace, string ? string : "(null)");

    if (!string)
        return (NULL);

    if ((node = mxml_new(parent, MXML_TEXT)) != NULL)
    {
        node->value.text     = mxml_strdup(string);
    }

    return (node);
}

mxml_node_t *				        /* O - New node */
mxmlNewElement(mxml_node_t *parent,	/* I - Parent node or MXML_NO_PARENT */
               const char  *name)	/* I - Name of element */
{
    mxml_node_t	*node;			    /* New node */

    mxml_trace("mxmlNewElement(parent=%p, name=\"%s\")\n", parent, name ? name : "(null)");

    if (!name)
        return (NULL);

    if ((node = mxml_new(parent, MXML_ELEMENT)) != NULL)
        node->value.element = mxml_strdup(name);

    return (node);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
static int                          /* O  - Character or MXML_EOF */
mxml_string_getc(void *p)		    /* I  - Pointer to file */
{
    int ch;                         /* Character */
    const char	**s;                /* Pointer to string pointer */

    s = (const char **)p;

    if ((ch = (*s)[0] & 255) != 0)
    {
        /*
        * Got character; convert UTF-8 to integer and return...
        */

        (*s)++;

        if (!(ch & 0x80))
        {
            mxml_trace("mxml_string_getc: %c (0x%04x)\n", ch < ' ' ? '.' : ch, ch);

            if (mxml_bad_char(ch))
            {
                mxml_err_trace("Bad control character 0x%02x not allowed by XML standard!",
                           ch);
                return (MXML_EOF);
            }

            return (ch);
        }
        else if ((ch & 0xe0) == 0xc0)
        {
            /*
            * Two-byte value...
            */

            if (((*s)[0] & 0xc0) != 0x80)
                return (MXML_EOF);

            ch = ((ch & 0x1f) << 6) | ((*s)[0] & 0x3f);

            (*s)++;

            if (ch < 0x80)
            {
                mxml_err_trace("Invalid UTF-8 sequence for character 0x%04x!", ch);
                return (MXML_EOF);
            }

            mxml_trace("mxml_string_getc: %c (0x%04x)\n", ch < ' ' ? '.' : ch, ch);

            return (ch);
        }
        else if ((ch & 0xf0) == 0xe0)
        {
            /*
            * Three-byte value...
            */

            if (((*s)[0] & 0xc0) != 0x80 ||
                    ((*s)[1] & 0xc0) != 0x80)
                return (MXML_EOF);

            ch = ((((ch & 0x0f) << 6) | ((*s)[0] & 0x3f)) << 6) | ((*s)[1] & 0x3f);

            (*s) += 2;

            if (ch < 0x800)
            {
                mxml_err_trace("Invalid UTF-8 sequence for character 0x%04x!", ch);
                return (MXML_EOF);
            }

            /*
            * Ignore (strip) Byte Order Mark (BOM)...
            */

            if (ch == 0xfeff)
                return (mxml_string_getc(p));

            mxml_trace("mxml_string_getc: %c (0x%04x)\n", ch < ' ' ? '.' : ch, ch);

            return (ch);
        }
        else if ((ch & 0xf8) == 0xf0)
        {
            /*
            * Four-byte value...
            */

            if (((*s)[0] & 0xc0) != 0x80 ||
                    ((*s)[1] & 0xc0) != 0x80 ||
                    ((*s)[2] & 0xc0) != 0x80)
                return (MXML_EOF);

            ch = ((((((ch & 0x07) << 6) | ((*s)[0] & 0x3f)) << 6) |
                   ((*s)[1] & 0x3f)) << 6) | ((*s)[2] & 0x3f);

            (*s) += 3;

            if (ch < 0x10000)
            {
                mxml_err_trace("Invalid UTF-8 sequence for character 0x%04x!", ch);
                return (MXML_EOF);
            }

            mxml_trace("mxml_string_getc: %c (0x%04x)\n", ch < ' ' ? '.' : ch, ch);

            return (ch);
        }
        else
            return (MXML_EOF);
    }

    return (MXML_EOF);
}

/*
* 'mxml_load_data()' - Load data into an XML node tree.
*/

static mxml_node_t *			/* O - First node or NULL if the file could not be read. */
mxml_load_data(
    mxml_node_t     *top,		/* I - Top node */
    void            *p,			/* I - Pointer to data */
    mxml_load_cb_t  cb,			/* I - Callback function or MXML_NO_CALLBACK */
    _mxml_getc_cb_t getc_cb)	/* I - Read function */
{
    mxml_node_t	*node,			/* Current node */
                *first,			/* First node added */
                *parent;		/* Current parent node */
    int		ch,			        /* Character from file */
         whitespace;		    /* Non-zero if whitespace seen */
    char		*buffer,		/* String buffer */
             *bufptr;		    /* Pointer into buffer */
    int		bufsize;		    /* Size of buffer */
    mxml_type_t	type;			/* Current node type */

    /*
    * Read elements and other nodes from the file...
    */

    if ((buffer = (char*)mxml_malloc(64)) == NULL)
    {
        mxml_err_trace("Unable to allocate string buffer!");
        return (NULL);
    }

    bufsize    = 64;
    bufptr     = buffer;
    parent     = top;
    first      = NULL;
    whitespace = 0;

    if (cb && parent)
        type = (*cb)(parent);
    else if (parent)
        type = MXML_TEXT;
    else
        type = MXML_IGNORE;

    while ((ch = (*getc_cb)(p)) != MXML_EOF)
    {
        if ((ch == '<' ||
                (mxml_isspace(ch))) &&
                bufptr > buffer)
        {
            /*
            * Add a new value node...
            */

            *bufptr = '\0';

            switch (type)
            {
            case MXML_INTEGER :
                node = mxmlNewInteger(parent, strtol(buffer, &bufptr, 10));
                break;

            case MXML_TEXT :
                node = mxmlNewText(parent, whitespace, buffer);
                break;

            default : /* Ignore... */
                node = NULL;
                break;
            }

            if (*bufptr)
            {
                /*
                * Bad integer number value...
                */

                mxml_err_trace("Bad integer value '%s' in parent <%s>!",buffer,
                           parent ? parent->value.element : "null");
                break;
            }

            bufptr     = buffer;
            whitespace = mxml_isspace(ch) && type == MXML_TEXT;

            if (!node && type != MXML_IGNORE)
            {
                /*
                * Print error and return...
                */

                mxml_err_trace("Unable to add value node of type %d to parent <%s>!",
                           type, parent ? parent->value.element : "null");
                goto error;
            }


            if (!first && node)
                first = node;
        }
        else if (mxml_isspace(ch) && type == MXML_TEXT)
            whitespace = 1;

        /*
        * Add lone whitespace node if we have an element and existing
        * whitespace...
        */

        if (ch == '<' && whitespace && type == MXML_TEXT)
        {
            if (parent)
            {
                node = mxmlNewText(parent, whitespace, "");

                if (!first && node)
                    first = node;
            }

            whitespace = 0;
        }

        if (ch == '<')
        {
            /*
            * Start of open/close tag...
            */

            bufptr = buffer;

            while ((ch = (*getc_cb)(p)) != MXML_EOF)
                if (mxml_isspace(ch) || ch == '>' || (ch == '/' && bufptr > buffer))
                    break;
                else if (ch == '<')
                {
                    mxml_err_trace("Bare < in element!");
                    goto error;
                }
                else if (mxml_add_char(ch, &bufptr, &buffer, &bufsize))
                    goto error;
                else if ((bufptr - buffer) == 8 && !strncmp(buffer, "![CDATA[", 8))
                    break;

            *bufptr = '\0';

            if (!strcmp(buffer, "![CDATA["))
            {
                /*
                * Gather CDATA section...
                */

                while ((ch = (*getc_cb)(p)) != MXML_EOF)
                {
                    if (ch == '>' && !strncmp(bufptr - 2, "]]", 2))
                        break;
                    else if (mxml_add_char(ch, &bufptr, &buffer, &bufsize))
                        goto error;
                }

                /*
                * Error out if we didn't get the whole comment...
                */

                if (ch != '>')
                {
                    /*
                    * Print error and return...
                    */

                    mxml_err_trace("Early MXML_EOF in CDATA node!");
                    goto error;
                }


                /*
                * Otherwise add this as an element under the current parent...
                */

                *bufptr = '\0';

                if (!parent && first)
                {
                    /*
                    * There can only be one root element!
                    */

                    mxml_err_trace("<%s> cannot be a second root node after <%s>",
                               buffer, first->value.element);
                    goto error;
                }

                if ((node = mxmlNewElement(parent, buffer)) == NULL)
                {
                    /*
                    * Print error and return...
                    */

                    mxml_err_trace("Unable to add CDATA node to parent <%s>!",
                               parent ? parent->value.element : "null");
                    goto error;
                }


                if (node && !first)
                    first = node;
            }
            else if (buffer[0] == '/')
            {
                /*
                * Handle close tag...
                */

                if (!parent || strcmp(buffer + 1, parent->value.element))
                {
                    /*
                    * Close tag doesn't match tree; print an error for now...
                    */

                    mxml_err_trace("Mismatched close tag <%s> under parent <%s>!",
                               buffer, parent ? parent->value.element : "(null)");
                    goto error;
                }

                /*
                * Keep reading until we see >...
                */

                while (ch != '>' && ch != MXML_EOF)
                    ch = (*getc_cb)(p);

                node   = parent;
                parent = parent->parent;


                /*
                * Ascend into the parent and set the value type as needed...
                */

                if (cb && parent)
                    type = (*cb)(parent);
            }
            else
            {
                /*
                * Handle open tag...
                */

                if (!parent && first)
                {
                    /*
                    * There can only be one root element!
                    */

                    mxml_err_trace("<%s> cannot be a second root node after <%s>",
                               buffer, first->value.element);
                    goto error;
                }

                if ((node = mxmlNewElement(parent, buffer)) == NULL)
                {
                    /*
                    * Just print error for now...
                    */

                    mxml_err_trace("Unable to add element node to parent <%s>!",
                               parent ? parent->value.element : "null");
                    goto error;
                }

                if (!first)
                    first = node;

                if (ch == MXML_EOF)
                    break;

                if (ch != '/')
                {
                    /*
                    * Descend into this node, setting the value type as needed...
                    */

                    parent = node;

                    if (cb && parent)
                        type = (*cb)(parent);
                }
            }

            bufptr  = buffer;
        }
        else if (!mxml_isspace(ch))
        {
            /*
            * Add character to current buffer...
            */

            if (mxml_add_char(ch, &bufptr, &buffer, &bufsize))
                goto error;
        }
    }

    /*
    * Free the string buffer - we don't need it anymore...
    */

    mxml_free(buffer);

    /*
    * Find the top element and return it...
    */

    if (parent)
    {
        node = parent;

        while (parent != top && parent->parent)
            parent = parent->parent;

        if (node != parent)
        {
            mxml_err_trace("Missing close tag </%s> under parent <%s>!",
                       node->value.element,
                       node->parent ? node->parent->value.element : "(null)");

            mxmlDelete(first);

            return (NULL);
        }
    }

    if (parent)
        return (parent);
    else
        return (first);

    /*
    * Common error return...
    */

error:

    mxmlDelete(first);

    mxml_free(buffer);

    return (NULL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
mxml_node_t *                           /* O - First node or NULL if the string has errors. */
mxmlLoadString(const char *s,           /* I - String to load */
               mxml_load_cb_t cb)       /* I - Callback function or MXML_NO_CALLBACK */
{
    return (mxml_load_data(NULL, (void *)&s, cb, mxml_string_getc));
}

int                                     /* O - New reference count */
mxmlRelease(mxml_node_t *node)          /* I - Node */
{
    if (node)
    {
        mxmlDelete(node);
        return (0);
    }
    else
        return (-1);
}

mxml_node_t *                           /* O - Next node or NULL */
mxmlWalkNext(mxml_node_t *node,		    /* I - Current node */
             mxml_node_t *top)          /* I - Top node */
{
    if (!node)
        return (NULL);
    else if (node->child)
        return (node->child);
    else if (node->next)
        return (node->next);
    else if (node->parent && node->parent != top)
    {
        node = node->parent;

        while (!node->next)
            if (node->parent == top || !node->parent)
                return (NULL);
            else
                node = node->parent;

        return (node->next);
    }
    else
        return (NULL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

