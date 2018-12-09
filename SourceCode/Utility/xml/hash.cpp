#include "hash.h"
namespace NSP_STP_CM
{
	const MU_INT32 DEF_MDB_HASHSTART = 5381;
	static MU_INT32 _HashAdd(MU_INT32 h, MU_CHAR c)
	{
		h += (h << 5);
		return (h ^ c);
	}

	MU_INT32 Hash_Str(const void *  value, MU_INT32 len)
	{
		MU_INT32 h;
		const MS_INT8 *p = (const MS_INT8 *)value;

		h = DEF_MDB_HASHSTART;
		while (len != 0)
		{
			h = _HashAdd(h, *p++);
			--len;
		}
		return (h);
	}
}
