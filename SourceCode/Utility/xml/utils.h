#pragma once
#include "mty.h"
namespace NSP_STP_CM
{
	MS_CHAR    *ST_fskip_non_space(const MS_CHAR * str);
	extern MS_CHAR    *ST_fskip_space(const MS_CHAR * str);
	MS_CHAR    *ST_bskip_space(const MS_CHAR * str);
	MS_CHAR    *ST_fskip_char(const MS_CHAR * str, MS_CHAR c);

	MS_CHAR    *ST_next_area(const MS_CHAR *  src, MS_CHAR *  dst, const MS_CHAR s);
	MS_INT32    ST_parse_kv(const MS_CHAR *  buf, MS_CHAR *  key, MS_CHAR *  value, const MS_CHAR x);
	MU_INT32    ST_strulen(const MS_CHAR *  str);
	MS_INT32    ST_itoa(MS_CHAR *  dst, MS_INT32 num);
	MS_INT32    ST_hex2bin(const MS_CHAR * src, void *const  dst);
	MS_INT32    ST_hex2color(const MS_CHAR * src, MU_INT32 *const  dst);
	MS_INT32    ST_hex2int(const MS_CHAR * src, MU_INT32 *const  dst);
	MS_INT32    ST_bin2hex(MS_CHAR *  dst, const void *const src, MS_INT32 len);
	MU_INT32    ST_power2_ceil(MU_INT32 x);

	MS_INT32    ST_parse_args(MS_CHAR *argp[], MS_CHAR *const cmd);
	MS_INT32    ST_isalldigit(const MS_CHAR * buf, MS_INT32 siz);
	MS_INT32    ST_is_MU_INT32(const MS_CHAR * buf);
	MS_INT32    ST_is_MS_INT32(const MS_CHAR * buf);
	MS_INT32    ST_remove_dup(MS_CHAR *src, MS_CHAR c, M_BOOL remove_last);
	MS_INT32    ST_isname(const MS_CHAR * buf);
	MS_INT32    ST_is_asc_name(const MS_CHAR * buf);

	MS_INT32    ST_get_div(MU_INT32 src, MU_INT32 di);
	extern MU_INT32    ST_strlcpy(MS_CHAR *const dst, const MS_CHAR *src, MU_INT32 siz);
	MS_INT32    ST_cvtchar(MS_CHAR * buf, MS_CHAR f, MS_CHAR t);
	MS_INT32    ST_isdigit_enum(const MS_CHAR * buf);
	MS_INT32    ST_strto_upper(MS_CHAR *s);
	MS_INT32    ST_strto_lower(MS_CHAR *s);

	extern MS_INT32    ST_filt_space_copy(MS_CHAR *const  _src, MS_CHAR *const  _dst, MS_INT32 dst_siz, M_BOOL filt_head_space, M_BOOL filt_tail_space);
	extern MS_INT32    ST_filt_token_copy(MS_CHAR *const _src, MS_CHAR *const _dst, const MS_CHAR *token_stt, const MS_CHAR *token_end);

}
