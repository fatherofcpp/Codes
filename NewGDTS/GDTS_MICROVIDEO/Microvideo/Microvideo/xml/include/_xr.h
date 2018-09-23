/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: _xr.h 1699 2009-02-04 06:33:53Z dengxf $
 */
#include "mty.h"

#ifndef __XR_H_
#define __XR_H_

typedef MS_INT32 XR;

/*
 * XRs are 32 bit values layed out as follows:
 *
 *  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-----------------------------+-------------------------------+
 * |S|             DEV             |               CODE            |
 * +-+-----------------------------+-------------------------------+
 *
 *  where
 *
 *      S           --  Severity    -   indicates success/failure
 *
 *                              0   -   SUCCESS
 *                              1   -   FAILURE
 *
 *      Dev         --  is the device code
 *
 *      Code        --  is the facility's status code
 *
 */

#define XR_F                0x80000000

#define FXR_SUCCEEDED(xr)   ((XR)(xr) >= 0)

#define FXR_FAILED(xr)      ((XR)(xr) < 0)

#define FXR_DEV(xr)         (((xr) >> 16) & 0x7FFF)

#define FXR_CODE(xr)        ((xr) & 0xFFFF)

#define MAKE_E_XR(c) \
    ((XR) (XR_F | ((MU_INT32)(XR_DEV&0x7FFF)<<16) | ((MU_INT32)(c&0xFFFF))) )

#endif
