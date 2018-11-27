// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <Tlhelp32.h>

#include "ntservmsg.h" // Event message ids
#include "NTService.h"
#include "watchdog.h"
#include "myservice.h"


// TODO: reference additional headers your program requires here
