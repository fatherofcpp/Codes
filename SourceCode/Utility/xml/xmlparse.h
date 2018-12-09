#pragma once
#include "xmltype.h"
#include "xmlnode.h"
namespace NSP_STP_CM
{
	extern MS_INT32 f_xmlparse_init(XML * xml, const MS_CHAR * src);
	static MS_CHAR *get_name_end(const MS_CHAR * buf);
	static MS_CHAR *get_next_token(const MS_CHAR * src, MS_INT32 * xtt, MS_INT32 chkeq);
	static XMLNODE *f_xmlparse(XML * xml, const MS_CHAR * buf);
}
