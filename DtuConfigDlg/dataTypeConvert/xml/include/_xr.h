/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: _xr.h 1908 2009-05-13 05:04:46Z dengxf $
 */
#include "mty.h"

#ifndef __XR_H_
#define __XR_H_

typedef MS_INT32 XR;

/*
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+---+-------------------------+-----+-------------------------+
 * |S|INF|         DEV             | EXT |         CODE            |
 * +-+---+-------------------------+-----+-------------------------+
 *
 * DESC:
 *  
 *      S       1   [0 - 1      ]   --  Severity
 *                                      0 - SUCCESS
 *                                      1 - FAILURE
 *
 *      INF     2   [0 - 3      ]   --  is the Customer code flag
 *                                      0 - None
 *                                      1 - General Info
 *                                      2 - Warning info
 *                                      3 - Debug info
 *
 *      Dev     13  [0 - 1FFF   ]   --  is the dev code
 *
 *      Ext     3   [0 - 7      ]   --  is the Ext Flag
 *
 *      Code    13  [0 - 1FFF   ]   --  is the dev's status code
 *
 */

#define XR_S                0x00000000
#define XR_F                0x80000000

#define XR_NINF             0x00
#define XR_GINF             0x01
#define XR_WINF             0x02
#define XR_DINF             0x03

#define M_XR_SUCCEEDED(xr)  ((XR)(xr) >= 0)

#define M_XR_FAILED(xr)     ((XR)(xr) < 0)

#define M_XR_INF(xr)        (((xr) >> 29) & 0x3)

#define M_XR_DEV(xr)        (((xr) >> 16) & 0x1FFF)

#define M_XR_EXT(xr)        (((xr) >> 13) & 0x7)

#define M_XR_CODE(xr)       ((xr) & 0x1FFF)

/*
 * following 4 macros must be use after XR_DEV is defined
 */
#define MAKE_S_XR(c) \
    ((XR) (XR_S | ((MU_INT32)(XR_DEV & 0x1FFF) << 16) | ((MU_INT32)(c & 0x1FFF))) )

#define MAKE_E_XR(c) \
    ((XR) (XR_F | ((MU_INT32)(XR_DEV & 0x1FFF) << 16) | ((MU_INT32)(c & 0x1FFF))) )

#define MAKE_SX_XR(inf, ext, c) \
    ((XR) (XR_S | ((inf & 0x3) << 29) | ((MU_INT32)(XR_DEV & 0x1FFF) << 16) | ((MU_INT32)(ext & 0x7) << 13) | ((MU_INT32)(c & 0x1FFF))) )

#define MAKE_EX_XR(inf, ext, c) \
    ((XR) (XR_F | ((inf & 0x3) << 29) | ((MU_INT32)(XR_DEV & 0x1FFF) << 16) | ((MU_INT32)(ext & 0x7) << 13) | ((MU_INT32)(c & 0x1FFF))) )

/*
 * following 4 macros can use freely
 */
#define MAKE_S_XR_EXT(dev, c) \
    ((XR) (XR_S | ((MU_INT32)(dev & 0x1FFF) << 16) | ((MU_INT32)(c & 0xFFFF))) )

#define MAKE_E_XR_EXT(dev, c) \
    ((XR) (XR_F | ((MU_INT32)(dev & 0x1FFF) << 16) | ((MU_INT32)(c & 0xFFFF))) )

#define MAKE_SX_XR_EXT(inf, dev, ext, c) \
    ((XR) (XR_S | ((inf & 0x3) << 29) | ((MU_INT32)(dev & 0x1FFF) << 16) | ((MU_INT32)(ext & 0x7) << 13) | ((MU_INT32)(c & 0x1FFF))) )

#define MAKE_EX_XR_EXT(inf, dev, ext, c) \
    ((XR) (XR_F | ((inf & 0x3) << 29) | ((MU_INT32)(dev & 0x1FFF) << 16) | ((MU_INT32)(ext & 0x7) << 13) | ((MU_INT32)(c & 0x1FFF))) )

#endif
