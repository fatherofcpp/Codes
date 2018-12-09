#include "MMmem.h"
#include "malloc.h"

namespace NSP_STP_CM
{
	void   *mmem_malloc(MU_INT32 siz, MU_INT32 /*alignment*/)
	{
		return (void *)malloc(siz);
	}

	void    mmem_free(void *real)
	{
		free(real);
	}
}





