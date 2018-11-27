// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tchar.h>
#include <string>



// TODO: reference additional headers your program requires here

#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")


#define MAS_IS_VALID_HANDLE(x) (x != NULL && x != INVALID_HANDLE_VALUE)
