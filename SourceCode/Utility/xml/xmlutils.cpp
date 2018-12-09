#include "xmlutils.h"
#include "malloc.h"
#pragma warning (disable: 4127)
namespace NSP_STP_CM
{
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

	static MU_INT32 _HashAdd(MU_INT32 h, MU_CHAR c)
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

	const MS_INT32 DEF_BUFOBJ_SIZE_MAX = (5*1024 * 1024);
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

	void   *mmem_malloc(MU_INT32 siz, MU_INT32 /*alignment*/)
	{
		return (void *)malloc(siz);
	}

	void    mmem_free(void *real)
	{
		free(real);
	}
}
