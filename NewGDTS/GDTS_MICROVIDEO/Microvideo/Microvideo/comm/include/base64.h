/*
 * Copyright (c) Deng XueFeng <dengxf@dengh.com>
 * All rights reserved.
 *
 * $Id: base64.h 2365 2010-02-10 07:15:49Z dengxf $
 */
#include "mty.h"

#ifndef _BASE64_H_
#define _BASE64_H_

/*
 * 作用: BASE64 编码
 *
 * 输入: src 数据指针
 *     : src_len 数据长度
 *
 * 输出: dst 数据指针
 *
 * 返回: 编码后的数据长度,
 * 注释: 无
 */
MS_INT32 f_base64_encode(MS_CHAR *const dst, const void *src, MU_INT32 src_len);

/*
 * 作用: BASE64 解码
 *
 * 输入: src 字符串指针
 *
 * 输出: dst 数据指针
 *
 * 返回: n <  0 错误
 *     : n >= 0 解码后的数据长度,
 * 注释: 无
 */
MS_INT32 f_base64_decode(void *const dst, MS_CHAR *const src);

#endif
