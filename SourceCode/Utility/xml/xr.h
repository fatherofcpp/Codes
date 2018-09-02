/*
 * All rights reserved.
 *
 */

#pragma once

#include "mty.h"

//
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


//

/* DEV NONE */
#define XR_DEV_NONE                 0x0000

/* DEV SLIB */
#define XR_DEV_XASFH_CORE           0x0101
#define XR_DEV_XASFH                0x0102
#define XR_DEV_XASFP                0x0103
#define XR_DEV_XASFI                0x0104
#define XR_DEV_ASF_EDITOR           0x0105
#define XR_DEV_ASF_PUSH             0x0106
#define XR_DEV_ASF_PULL             0x0107
#define XR_DEV_ASF_PVS              0x0108
#define XR_DEV_AEXT                 0x0109
#define XR_DEV_AUDIT_ASF            0x010A
#define XR_DEV_AUDIT_API            0x010B
#define XR_DEV_AUDIT_PLAYLIST       0x010C
#define XR_DEV_CHARSET              0x010D
#define XR_DEV_TAA                  0x010E
#define XR_DEV_MMILNODE             0x010F
#define XR_DEV_DMB                  0x0110
#define XR_DEV_MSND                 0x0111
#define XR_DEV_FORKER               0x0112
#define XR_DEV_MTIMER               0x0113
#define XR_DEV_XQUEUE               0x0114
#define XR_DEV_LICENSE              0x0115
#define XR_DEV_MEXIT                0x0116

#define XR_DEV_MIO                  0x0117
#define XR_DEV_MYDAEMON             0x0118
#define XR_DEV_MYLOG                0x0119
#define XR_DEV_PARSE_HTTP           0x011A
#define XR_DEV_PARSE_RFC            0x011B
#define XR_DEV_PARSE_RTSP           0x011C
#define XR_DEV_PROG                 0x011D
#define XR_DEV_MHTABLE              0x011E
#define XR_DEV_MUSER                0x011F
#define XR_DEV_TEST_AIO             0x0120
#define XR_DEV_TIME_UTILS           0x0121

#define XR_DEV_XLOCK                0x0122
#define XR_DEV_XMEM                 0x0123
#define XR_DEV_XMLNODE              0x0124
#define XR_DEV_XMLPARSE             0x0125
#define XR_DEV_XUMTX                0x0126
#define XR_DEV_MSS_CORE_STATUS      0x0127
#define XR_DEV_READ_LIVE            0x0128
#define XR_DEV_MHOST                0x0129
#define XR_DEV_SQLLOG               0x012A
#define XR_DEV_MSS_RECV_TABLE       0x012B
#define XR_DEV_WAVE_API             0x012C

#define XR_DEV_PARSE_AUTH_PARAM     0x012D
#define XR_DEV_WOCTL                0x012E
#define XR_DEV_DOCTL                0x012F
#define XR_DEV_XEVT                 0x0130
#define XR_DEV_SCOM                 0x0131
#define XR_DEV_MNETIO               0x0132
#define XR_DEV_MSYSIO               0x0133
#define XR_DEV_MMEM                 0x0134
#define XR_DEV_MTPL                 0x0135
#define XR_DEV_CONF                 0x0136
#define XR_DEV_XMLEXT               0x0137
#define XR_DEV_MPIXEL               0x0138
#define XR_DEV_MXGL                 0x0139
#define XR_DEV_LFBLIB               0x013A
#define XR_DEV_MFB                  0x013B
#define XR_DEV_MPIXEL_BLIT_COMMON   0x013C
#define XR_DEV_GDPT                 0x013D

/* DEV CORE */
#define XR_DEV_MHOST_CACHE          0x0201
#define XR_DEV_MMSH                 0x0202
#define XR_DEV_MMST                 0x0203
#define XR_DEV_MSS_CORE             0x0204
#define XR_DEV_MSS_CORE_AUDIT       0x0205
#define XR_DEV_MSS_CORE_MAIN        0x0206
#define XR_DEV_MSS_CORE_READER      0x0207
#define XR_DEV_MSS_FORKER_CHILD     0x0208
#define XR_DEV_NESS                 0x0209
#define XR_DEV_PESS                 0x020A
#define XR_DEV_READ_ASF             0x020B
#define XR_DEV_RTSP                 0x020C
#define XR_DEV_SEND_ASF             0x020D
#define XR_DEV_STRM_SERV            0x020E
#define XR_DEV_TQUEUE               0x020F
#define XR_DEV_MLRU                 0x0210
#define XR_DEV_MHAQ                 0x0211
#define XR_DEV_HELP_THR             0x0212
#define XR_DEV_ASF_PRELOAD          0x0213
#define XR_DEV_PARSE_CORE_PARAM     0x0214
#define XR_DEV_ASF_CACHE            0x0215

/* DEV AUDIT */
#define XR_DEV_MSS_AUDIT            0x0301
#define XR_DEV_MSS_AUDIT_PLUGIN     0x0302

/* DEV CCS */
#define XR_DEV_MSS_CCS              0x0401
#define XR_DEV_MSS_CCS_MAIN         0x0402

/* DEV RECV AND SAVE */
#define XR_DEV_MSS_RECV             0x0501
#define XR_DEV_MSS_RECV_MAIN        0x0502
#define XR_DEV_MSAVER               0x0503
#define XR_DEV_MSAVER_BODY          0x0504
#define XR_DEV_MSS_PULL             0x0505
#define XR_DEV_MSS_PULL_MAIN        0x0506
#define XR_DEV_PARSE_RECV_PARAM     0x0507
#define XR_DEV_MSS_SAVE_MAIN        0x0508
#define XR_DEV_PARSE_SAVE_PARAM     0x0509

/* DEV ADMIN */
#define XR_DEV_MSS_ADMIN            0x0601
#define XR_DEV_MSS_ADMIN_MAIN       0x0602
#define XR_DEV_MSS_ADMIN_PLUGIN     0x0603
#define XR_DEV_PARSE_ADMIN_PARAM    0x0604
#define XR_DEV_ADMIN_SUB_XLIVE      0x0605
#define XR_DEV_ADMIN_SUB_XDOWN      0x0606
#define XR_DEV_ADMIN_SUB_XVIEW      0x0607
#define XR_DEV_ADMIN_SUB_XWEB       0x0608
#define XR_DEV_ADMIN_SUB_MCTL       0x0609
#define XR_DEV_ADMIN_SUB_BASE64     0x060A
#define XR_DEV_ADMIN_SUB_CONFIG     0x060B
#define XR_DEV_FILE_LIST            0x060C

/* PLUGIN */
#define XR_DEV_AUTH                 0x0701

/* MOU */
#define XR_DEV_MOU                  0x0801
#define XR_DEV_MWIN                 0x0802
#define XR_DEV_MITM_PIC             0x0803
#define XR_DEV_MITM_WMV             0x0804
#define XR_DEV_WMVDEC               0x0805
#define XR_DEV_MOU_ADMIN            0x0806

/* VDS */
#define XR_DEV_MSS_SVP              0x0901
#define XR_DEV_MSS_SVP_MAIN         0x0902
#define XR_DEV_VDS_ADMIN            0x0903
#define XR_DEV_VDS_ADMIN_MAIN       0x0904

