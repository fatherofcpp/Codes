
#pragma once

typedef enum             		    /**** The XML node type. ****/
{
	MXML_IGNORE = -1,			    /* Ignore/throw away node*/
	MXML_ELEMENT,				    /* XML element with attributes */
	MXML_INTEGER,				    /* Integer value */
	MXML_TEXT  				        /* Text fragment */
} mxml_type_t;

typedef struct
{
	char* element;      
	char* text;
	int integer;      
} mxml_value_t;

typedef struct mxml_node_s 
{
	mxml_type_t type;               /* Node type */
	mxml_value_t value;             /* Node value */
	struct mxml_node_s *next;		/* Next node under same parent */
	struct mxml_node_s *prev;		/* Previous node under same parent */
	struct mxml_node_s *parent;     /* Parent node */
	struct mxml_node_s *child;      /* First child node */
	struct mxml_node_s *last_child; /* Last child node */
} mxml_node_t;

typedef mxml_type_t (*mxml_load_cb_t)(mxml_node_t *);

mxml_node_t *mxmlLoadString(const char *s, mxml_load_cb_t cb);
int	mxmlRelease(mxml_node_t *node);
mxml_node_t	*mxmlWalkNext(mxml_node_t *node, mxml_node_t *top);

