#pragma once
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"avcodec.lib")
#include <avcodec.h>
#include <avformat.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <avformat.h>
#include <swscale.h>
#include <mmsystem.h>
static int sws_flags = SWS_BICUBIC;
#define STREAM_DURATION   5.0
#define STREAM_FRAME_RATE 25 
#define STREAM_NB_FRAMES  ((int)(STREAM_DURATION * STREAM_FRAME_RATE))
#define STREAM_PIX_FMT PIX_FMT_YUV420P


class MyMpeg
{
public:
	MyMpeg(void);
	~MyMpeg(void);
	struct SwsContext *img_convert_ctx;
	struct SwsContext *img_convert_ctx2;
	AVFrame *FirstRGB_picture;
	AVFrame *FirstI420_picture;
	AVFrame *SecRGB_picture;
	AVFrame *SecI420_picture;

	AVFormatContext *oc;
	AVStream *st;
	AVCodecContext *c;
	AVCodec *codec;
	BYTE * video_outbuf;
	int video_outbuf_size;

	AVFormatContext *oc2;
	AVStream *st2;
	AVCodecContext *c2;
	AVCodec *decodec;

	AVFormatContext *oc_Audio;
	AVStream *st_Audio;
	AVCodecContext *c_Audio;
	AVCodec *codec_Audio;

	AVFormatContext *oc_Audio2;
	AVStream *st_Audio2;
	AVCodecContext *c_Audio2;
	AVCodec *codec_Audio2;

	ReSampleContext *SampleContext;
/*	AVFormatContext *oc_Audio3;
	AVStream *st_Audio3;
	AVCodecContext *c_Audio3;
	AVCodec *codec_Audio3;*/
	int Init(int width,int height);
	int Encode(BYTE* inputbuf, BYTE* outputbuf);
	int Encode(AVFrame* inputbuf, BYTE* outputbuf);
	int m_Width;
	int m_Height;
	int Decode(BYTE* inputbuf,int inputbufsize, BYTE* outputbuf);
	int EncodeAudio(BYTE* inputbuf,  BYTE* outputbuf,int outputbufsize);
	int MyMpeg::DecodeAudio(BYTE* inputbuf,int inputbufsize, BYTE* outputbuf, int outputbufsize);
	int audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf,int buf_size);
	BYTE* m_pData;
	//≤•∑≈“Ù∆µ
	void RenderSound(BYTE* pBuf, int Len, UINT time,HWND m_hwnd);
	bool SoundFirst;
	BYTE* destbuf;
//	BYTE* buf;
};
