/********************************************************************
	created:	2014/03/27
	filename: 	avLib.h
	file base:	ffmepg
	file ext:	h
	author:		zhh
	
	purpose:	实现动态加载ffmpeg中提取的H264编码和解码操作
*********************************************************************/

#ifndef DEF_AVLIB_H_
#define DEF_AVLIB_H_

#if 1
#include <avcodec.h>
#include <avformat.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <avformat.h>
#include <swscale.h>
#include <mmsystem.h>
#include <process.h>
//#include "public_for_encoder.h"
#endif



typedef int (*GD_avcodec_encode_video)(AVCodecContext *avctx, uint8_t *buf, int buf_size,const AVFrame *pict);
typedef int (*GD_avcodec_open)(AVCodecContext *avctx, AVCodec *codec);

typedef AVCodec *(*GD_avcodec_find_encoder)(enum CodecID id);
typedef AVStream *(*GD_av_new_stream)(AVFormatContext *s, int id);
typedef AVFormatContext *(*GD_av_alloc_format_context)(void);
typedef int (*GD_avpicture_fill)(AVPicture *picture, uint8_t *ptr,int pix_fmt, int width, int height);
typedef void (*GD_av_free)(void *ptr);
typedef void *(*GD_av_malloc)(unsigned int size) av_malloc_attrib av_alloc_size(1);
typedef int (*GD_avpicture_get_size)(int pix_fmt, int width, int height);
typedef AVFrame *(*GD_avcodec_alloc_frame)(void);
typedef void (*GD_avcodec_init)(void);
typedef void (*GD_av_register_all)(void);
///////////////////
typedef AVCodec *(*GD_avcodec_find_decoder)(enum CodecID id);
typedef int (*GD_avcodec_decode_video)(AVCodecContext *avctx, AVFrame *picture,int *got_picture_ptr,const uint8_t *buf, int buf_size);
typedef int (*GD_avcodec_close)(AVCodecContext *avctx);
//////////
extern GD_avcodec_encode_video GDavcodec_encode_video;
extern GD_avcodec_open GDavcodec_open;
extern GD_avcodec_find_encoder GDavcodec_find_encoder;
extern GD_av_new_stream GDav_new_stream;
extern GD_av_alloc_format_context GDav_alloc_format_context;
extern GD_avpicture_fill GDavpicture_fill;
extern GD_av_free GDav_free;
extern GD_av_malloc GDav_malloc;
extern GD_avpicture_get_size GDavpicture_get_size;
extern GD_avcodec_alloc_frame GDavcodec_alloc_frame;
extern GD_avcodec_init GDavcodec_init;
extern GD_av_register_all GDav_register_all;

extern GD_avcodec_find_decoder GDavcodec_find_decoder;
extern GD_avcodec_decode_video GDavcodec_decode_video;
extern GD_avcodec_close GDavcodec_close;

int loadAVLib();

#endif
