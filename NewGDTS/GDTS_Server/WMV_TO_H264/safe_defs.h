//
// safe_defs.h
// 
/**
 ** Copyright (C) 2005 EnjoyView Inc., all rights reserved.
 **           Your View, Our Passion. Just Enjoy It!
 **
 **            http://spaces.msn.com/members/jemylu
 **
 **/

/*************************************************************************/

#ifndef __h_safe_defs__
#define __h_safe_defs__

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
   if (x != NULL)       \
   {                    \
      x->Release();     \
      x = NULL;         \
   }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) \
   if (x != NULL)      \
   {                   \
      delete x;        \
      x = NULL;        \
   }
#endif

#ifndef SAFE_ARRAY_DELETE
#define SAFE_ARRAY_DELETE(x) \
   if (x != NULL)            \
   {                         \
	  delete[] x;            \
	  x = NULL;              \
   }
#endif

#ifndef SAFE_CLOSEHANDLE
#define SAFE_CLOSEHANDLE(h)         \
    if (h != NULL)                  \
    {                               \
        CloseHandle(h);             \
        h = NULL;                   \
    }
#endif

#ifndef SAFE_CLOSEFILEHANDLE
#define SAFE_CLOSEFILEHANDLE(h)     \
    if (h != INVALID_HANDLE_VALUE)  \
    {                               \
        CloseHandle(h);             \
        h = INVALID_HANDLE_VALUE;   \
    }
#endif 



#ifndef GOTO_EXIT_IF_FAILED
#define GOTO_EXIT_IF_FAILED(hr) if(FAILED(hr)) goto Exit;
#endif

#ifndef BREAK_IF_FAILED
#define BREAK_IF_FAILED(hr) if(FAILED(hr)) break;
#endif

#ifndef RETURN_IF_FAILED
#define RETURN_IF_FAILED(hr) if(FAILED(hr)) return hr;
#endif

#endif // __h_safe_defs__