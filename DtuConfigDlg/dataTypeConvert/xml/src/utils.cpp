/*
* Copyright (c) Deng XueFeng <dengxf@dengh.com>
* All rights reserved.
*
* $Id: utils.c 1941 2009-06-19 02:56:53Z dengxf $
*/
#include "stdafx.h"

#include "include/mty.h"

#include <string.h>

#include "../include/utils.h"
#include "../include/c_defs.h"

static const MS_CHAR *hex = "0123456789ABCDEF";

MS_CHAR *ST_fskip_non_space(const MS_CHAR *M_RESTRICT str)
{
	while (*str != 0x00)
	{
		switch (*str)
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			return ((MS_CHAR *)(M_MOVECONST(str)));
			break;

		default:
			str++;
			break;
		}
	}
	return ((MS_CHAR *)(M_MOVECONST(str)));
}

MS_CHAR *ST_fskip_space(const MS_CHAR *M_RESTRICT str)
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
			return ((MS_CHAR *)(M_MOVECONST(str)));
			break;
		}
	}
	return ((MS_CHAR *)(M_MOVECONST(str)));
}

MS_CHAR *ST_bskip_space(const MS_CHAR *M_RESTRICT str)
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
	return ((MS_CHAR *)(M_MOVECONST(last+1)));
}

MS_CHAR *ST_fskip_char(const MS_CHAR *M_RESTRICT str, MS_CHAR ch)
{
	while (*str != 0x00)
	{
		if (*str == ch)
		{
			str++;
		}
		else
		{
			return ((MS_CHAR *)(M_MOVECONST(str)));
		}
	}

	return ((MS_CHAR *)(M_MOVECONST(str)));
}

MS_CHAR *ST_next_area(const MS_CHAR * M_RESTRICT src, MS_CHAR * M_RESTRICT dst, const MS_CHAR x)
{
	MS_CHAR match = 0;

	src = ST_fskip_space(src);
	while (*src != 0x00)
	{
		*dst = *src;
		switch (*src)
		{
		case '\'':
		case '"':
			if (match == 0)
			{
				match = *src;
			}
			else if (match == *src)
			{
				match = 1;
			}
			else
			{
				return (NULL);
			}
			break;

		default:
			if (match == 0)
			{
				if (*src == x)
				{
					match = 9;
				}
			}
			else if (match == 1)
			{
				src = ST_fskip_space(src);
				if (*src != x)
				{
					return (NULL);
				}
				match = 9;
			}
			break;
		}
		src++;
		if (match == 9)
		{
			*dst = 0x00;
			match = 0;
			break;
		}
		dst++;
	}
	*dst = 0x00;
	if (match > 1)
	{
		return (NULL);
	}
	return ((MS_CHAR *)(M_MOVECONST(src)));
}

MS_INT32 ST_parse_kv(const MS_CHAR * M_RESTRICT src, MS_CHAR * M_RESTRICT key, MS_CHAR * M_RESTRICT value, const MS_CHAR x)
{
	MS_CHAR *p;

	*key = 0x00;
	*value = 0x00;
	p = ST_next_area(src, key, x);
	if (key[0] == 0x00)
	{
		return (-1);
	}
	if (*p == 0x00)
	{
		return (0);
	}
	strcpy(value, p);
	ST_filt_space_copy(key, NULL, 0, M_TRUE, M_TRUE);
	ST_filt_space_copy(value, NULL, 0, M_TRUE, M_TRUE);
	return (1);
}

MU_INT32 ST_strulen(const MS_CHAR * M_RESTRICT str)
{
	const MS_CHAR *s;

	for (s = str; *s || *(s+1); ++s);
	return(s + 3 - str);
}

static MS_CHAR H2B(MS_CHAR s)
{
	MS_CHAR d;

	if ((s >= '0') && (s <= '9'))
	{
		d = s - '0';
		return (d);
	}
	if ((s >= 'a') && (s <= 'f'))
	{
		d = s - 'a' + 10;
		return (d);
	}
	if ((s >= 'A') && (s <= 'F'))
	{
		d = s - 'A' + 10;
		return (d);
	}

	return (0x7F);
}

MS_INT32 ST_hex2color(const MS_CHAR *M_RESTRICT src, MU_INT32 *const M_RESTRICT dst)
{
	MS_CHAR buf[9];
	MU_INT32 ulen;

	ulen = strlen(src);
	if (ulen > 8)
	{
		return (-1);
	}
	memset(buf, '0', 8);
	memset(buf, 'F', 2);
	buf[8] = 0x00;
	memcpy(buf+8-ulen, src, ulen);
	if (ST_hex2bin(buf, &ulen) != 0)
	{
		return (-1);
	}
	ulen = M_HTONL(ulen);
	*dst = ulen;
	return (0);
}

MS_INT32 ST_hex2bin(const MS_CHAR *M_RESTRICT src, void *const M_RESTRICT dst)
{
	MS_INT32 i;
	MS_INT32 len;
	MU_CHAR  c;
	MU_CHAR  tc;
	MU_CHAR *_dst = (MU_CHAR *)dst;

	len = strlen(src);
	if ((len == 0) || (len%2 != 0))
	{
		return (-1);
	}
	len >>= 1;
	for(i=0; i<len; i++)
	{
		tc = H2B((MU_CHAR)src[i<<1]);
		if (tc > 0xF)
		{
			return (-1);
		}
		c = tc;

		c <<= 4;
		tc = H2B((MU_CHAR)src[(i<<1) + 1]);
		if (tc > 0xF)
		{
			return (-1);
		}
		c += tc;
		_dst[i] = c;
	}
	return (0);
}

MS_INT32 ST_hex2int(const MS_CHAR *M_RESTRICT src, MU_INT32 *const M_RESTRICT dst)
{
	MS_CHAR buf[9];
	MU_INT32 ulen;

	ulen = strlen(src);
	if (ulen > 8)
	{
		return (-1);
	}
	memset(buf, '0', 8);
	buf[8] = 0x00;
	memcpy(buf+8-ulen, src, ulen);
	if (ST_hex2bin(buf, &ulen) != 0)
	{
		return (-1);
	}
	ulen = M_HTONL(ulen);
	*dst = ulen;
	return (0);
}

MS_INT32 ST_bin2hex(MS_CHAR * M_RESTRICT dst, const void *const src, MS_INT32 len)
{
	const MU_CHAR *_src = (MU_CHAR *)src;

	for (;len--; _src++)
	{
		*dst++ = hex[_src[0] >> 4];
		*dst++ = hex[_src[0] & 0x0F];
	}
	*dst = 0x00;

	return (len<<1);
}

MS_INT32 ST_itoa(MS_CHAR * M_RESTRICT dst, MS_INT32 num)
{
	MS_INT32 res;
	MS_INT32 iv;
	MS_INT32 flag;
	MS_INT32 len;

	len = flag = 0;
	if (num < 0)
	{
		*dst++ = '-';
		num = -num;
		len++;
	}
	if (num == 0)
	{
		*dst++ = '0';
		len++;
	}
	else
	{
		iv = (num < 100000) ? 100000 : 1000000000;
		while (iv > 0)
		{
			res = num / iv;
			if ((flag == 1) || (res > 0))
			{
				*dst++ = res + '0';
				len++;
				flag = 1;
			}
			num -= res * iv;
			iv /= 10;
		}
	}
	*dst = '\0';
	return (len);
}

#if 0
MU_INT32 ST_get2m(MU_INT32 x)
{
	MU_INT32 m = 0;

	m += !(x & 0xFF000000) << 3;
	m += !((x << m) & 0xFF000000) << 3;
	m += !((x << m) & 0xFF000000) << 3;
	m += !((x << m) & 0xF0000000) << 2;
	m += !((x << m) & 0xC0000000) << 1;
	m += !((x << m) & 0x80000000);

	m += !((x << m) & 0x7FFFFFFF);

	return (32 - m);
}

MU_INT32 ST_get2mv(MU_INT32 x)
{
	return (1 << ST_get2m(x));
}
#endif

/*
* 得到大于到一个x的最小的2^n, n = 32 - m
*/
MU_INT32 ST_power2_ceil(MU_INT32 x)
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;

	x++;

	return (x);
}

MS_INT32 ST_parse_args(MS_CHAR *argp[], MS_CHAR *const cmd)
{
	MS_INT32 i = 0;
	MS_CHAR *p = cmd;

	while (1)
	{
		p = ST_fskip_space(p);
		if (*p == 0x00)
		{
			break;
		}
		argp[i++] = p;
		p = ST_fskip_non_space(p);
		if (*p == 0x00)
		{
			break;
		}
		*p++ = 0x00;
	}
	argp[i] = NULL;

	return (i);
}

MS_INT32 ST_isalldigit(const MS_CHAR *M_RESTRICT buf, MS_INT32 siz)
{
	const MS_CHAR *save = buf;
	const MS_CHAR *end = (siz <= 0) ? buf - 1 : buf + siz;

	while (*buf != 0x00)
	{
		if (buf == end)
		{
			break;
		}
		if ((*buf < '0') || (*buf > '9'))
		{
			return (-1);
		}
		buf++;
	}
	if (buf < end)
	{
		return (-1);
	}

	return (buf - save);
}

MS_INT32 ST_is_MU_INT32(const MS_CHAR *M_RESTRICT buf)
{
	MS_INT32 len;
	MS_INT64 t;

	if (*buf == 0x00)
	{
		return (-1);
	}

	buf = ST_fskip_char(buf, '0');
	if (*buf == 0x00)
	{
		return (0);
	}

	len = ST_isalldigit(buf, -1);
	if ((len < 0) || (len > 11))
	{
		return (-1);
	}


	t = _atoi64(buf);

	if ((t > 0xFFFFFFFFU))
	{
		return (-1);
	}

	return (0);
}

MS_INT32 ST_is_MS_INT32(const MS_CHAR *M_RESTRICT buf)
{
	MS_INT32 len;
	MS_INT64 t;
	M_BOOL neg = M_FALSE;

	if (*buf == 0x00)
	{
		return (-1);
	}

	buf = ST_fskip_char(buf, '0');
	if (*buf == 0x00)
	{
		return (0);
	}
	if (*buf == '-')
	{
		neg = M_TRUE;
		buf++;
	}
	len = ST_isalldigit(buf, -1);
	if ((len < 0) || (len > 11))
	{
		return (-1);
	}


	t = _atoi64(buf);

	if (neg == M_FALSE)
	{
		if (t > 0x7FFFFFFF)
		{
			return (-1);
		}
	}
	else
	{
		if (t > 0x80000000)
		{
			return (-1);
		}
	}
	return (0);
}

MS_INT32 ST_isdigit_enum(const MS_CHAR *M_RESTRICT buf)
{
	MS_INT32 ng = 1;

	if (*buf == '-')
	{
		ng = -1;
		buf++;
	}
	if (buf[1] != 0x00)
	{
		return (-2);
	}
	if (buf[0] == '0')
	{
		if (ng == -1)
		{
			return (-3);
		}
		return (0);
	}
	else if (buf[0] == '1')
	{
		return (ng);
	}
	else
	{
		return (-4);
	}
}

MS_INT32 ST_remove_dup(MS_CHAR *src, MS_CHAR c, M_BOOL remove_last)
{
	MS_INT32 len;
	MS_CHAR *dst = src;
	MS_CHAR *save = src;

	src++;
	while(*src != 0x00)
	{
		if ((*src == c) && (*dst == c))
		{
			src++;
			continue;
		}
		dst++;
		if (src != dst)
		{
			*dst = *src;
		}
		src++;
	}
	if ((*dst != c) || (dst == save))
	{
		dst++;
	}
	*dst = 0x00;
	len = dst - save;

	if (remove_last == M_TRUE)
	{
		if (save[len-1] == c)
		{
			save[--len] = 0x00;
		}
	}

	return (len);
}

MS_INT32 ST_isname(const MS_CHAR *M_RESTRICT _buf)
{
	const MU_CHAR *buf = (const MU_CHAR *)_buf;

	for (; *buf != 0x00; buf++)
	{
		if (*buf < 0x20)
		{
			return (-1);
		}
		/* PRINTABLE */
		switch (*buf)
		{
#if 0
		case 0x2F:  /* / */
		case 0x3A:  /* : */
#endif
		case 0x5C:  /* \ */
		case 0x2A:  /* * */
		case 0x3F:  /* ? */
		case 0x22:  /* " */
		case 0x3C:  /* < */
		case 0x3E:  /* > */
		case 0x7C:  /* | */

		case 0x7F:
		case 0xFF:
			return (-1);
			break;

		default:
			break;
		}
	}
	return (0);
}

MS_INT32 ST_is_asc_name(const MS_CHAR *M_RESTRICT buf)
{
	for (; *buf != 0x00; buf++)
	{
		if ((*buf >= 'A') && (*buf <= 'Z'))
		{
			continue;
		}
		if ((*buf >= 'a') && (*buf <= 'z'))
		{
			continue;
		}
		if ((*buf >= '0') && (*buf <= '9'))
		{
			continue;
		}
		if (*buf == '_')
		{
			continue;
		}
		return (-1);
	}
	return (0);
}

MS_INT32 ST_get_div(MU_INT32 src, MU_INT32 di)
{
	MU_INT32 rt;
	MU_INT32 y;

	y = src % di;
	rt = src / di;
	if (y > 0)
	{
		rt += 1;
	}
	return (rt);
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

MS_INT32 ST_cvtchar(MS_CHAR *M_RESTRICT buf, MS_CHAR f, MS_CHAR t)
{
	MS_INT32 c = 0;

	for (; *buf != 0x00; buf++)
	{
		if (*buf == f)
		{
			*buf = t;
			c++;
		}
	}

	return (c);
}

MS_INT32 ST_strto_upper(MS_CHAR *s)
{
	MS_INT32 c = 0;

	for (; *s != 0x00; s++)
	{
		if ((*s >= 'a') && (*s <= 'z'))
		{
			*s -= 0x20;
			c++;
		}
	}

	return (c);
}

MS_INT32 ST_strto_lower(MS_CHAR *s)
{
	MS_INT32 c = 0;

	for (; *s != 0x00; s++)
	{
		if ((*s >= 'A') && (*s <= 'Z'))
		{
			*s += 0x20;
			c++;
		}
	}

	return (c);
}

MS_INT32 ST_filt_space_copy(MS_CHAR *const M_RESTRICT _src, MS_CHAR *const M_RESTRICT _dst, MS_INT32 dst_siz, M_BOOL filt_head_space, M_BOOL filt_tail_space)
{
	MS_INT32 len;
	MS_CHAR *dst = _dst;
	MS_CHAR *src = _src;
	MS_CHAR *end = _src;

	if (_dst == NULL)
	{
		dst = src;
	}

	if (filt_head_space == M_TRUE)
	{
		src = ST_fskip_space(src);
	}
	if (filt_tail_space == M_TRUE)
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
	MS_CHAR *p;
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
