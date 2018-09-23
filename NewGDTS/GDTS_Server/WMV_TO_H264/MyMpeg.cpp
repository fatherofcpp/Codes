#include "StdAfx.h"
#include "MyMpeg.h"

static AVFrame *alloc_picture(int pix_fmt, int width, int height)
{
	AVFrame *picture;
	uint8_t *picture_buf;
	int size;

	picture = avcodec_alloc_frame();
	if (!picture)
		return NULL;
	size = avpicture_get_size(pix_fmt, width, height);
	picture_buf =(uint8_t*) av_malloc(size);
	if (!picture_buf) {
		av_free(picture);
		return NULL;
	}
	avpicture_fill((AVPicture *)picture, picture_buf,
		pix_fmt, width, height);
	return picture;
}
MyMpeg::MyMpeg(void)
{

}

MyMpeg::~MyMpeg(void)
{
	delete[] m_pData;
}

int MyMpeg::Init(int width,int height)
{
	m_Width=width;
	m_Height=height;
	img_convert_ctx = sws_getContext(width, height,
		PIX_FMT_RGB24,
		width,height,
		PIX_FMT_YUV420P,
		sws_flags, NULL, NULL, NULL);
	img_convert_ctx2 = sws_getContext(width, height,
		PIX_FMT_YUV420P,
		width,height,
		PIX_FMT_RGB24,
		sws_flags, NULL, NULL, NULL);

	FirstRGB_picture= alloc_picture(PIX_FMT_RGB24, width, height);
	FirstI420_picture = alloc_picture(PIX_FMT_YUV420P, width,height);
	SecRGB_picture= alloc_picture(PIX_FMT_RGB24,width, height);
	SecI420_picture = alloc_picture(PIX_FMT_YUV420P,width, height);

	av_register_all();
	oc = av_alloc_format_context(); 
	st = av_new_stream(oc, 0);
	c = st->codec;
	c->codec_id = CODEC_ID_H264;
	c->codec_type = CODEC_TYPE_VIDEO;
	c->bit_rate =500;//500; //500000;//500;
//	c->bit_rate_tolerance=400;
	c->width = width;
	c->height = height;
	c->time_base.den = STREAM_FRAME_RATE;
	c->time_base.num = 1;
	c->gop_size = 12; /* emit one intra frame every twelve frames at most */
// 	c->IFrame_distance=25;
// 	c->
// 	c->framerate=30;
	c->pix_fmt = STREAM_PIX_FMT;
	video_outbuf=new BYTE [width*height*3];
	video_outbuf_size=width*height*3;
	codec = avcodec_find_encoder(CODEC_ID_H264);
	int z=avcodec_open(c, codec);

	
	oc2 = av_alloc_format_context(); 
	st2 = av_new_stream(oc2, 0);
	c2 = st2->codec;
	c2->codec_id = CODEC_ID_H264;
	c2->codec_type = CODEC_TYPE_VIDEO;
	c2->bit_rate = 400;
//	c2->bit_rate_tolerance=400;
	c2->width = width;
	c2->height = height;

	c2->time_base.den = STREAM_FRAME_RATE;
	c2->time_base.num = 1;
	c2->gop_size = 12; /* emit one intra frame every twelve frames at most */
	c2->pix_fmt = STREAM_PIX_FMT;
	decodec = avcodec_find_decoder(CODEC_ID_H264);
	int z9=avcodec_open(c2, decodec);

	
	
	destbuf=new BYTE[88200];
//	buf=new BYTE[88200];
	m_pData=new BYTE[width*height*4];
	return 0;
}

int MyMpeg::Encode(BYTE* inputbuf, BYTE* outputbuf)
{
	int out_size=0;
	memcpy(FirstRGB_picture->data[0],inputbuf,m_Width*m_Height*3);
	int d=sws_scale(img_convert_ctx,FirstRGB_picture->data,FirstRGB_picture->linesize,
		0,m_Width,FirstI420_picture->data,FirstI420_picture->linesize);
	uint8_t * p=FirstI420_picture->data[1];
	FirstI420_picture->data[1]=FirstI420_picture->data[2];
	FirstI420_picture->data[2]=p;
	out_size = avcodec_encode_video(c,outputbuf,m_Width*m_Height*3,FirstI420_picture); 
	return out_size;
}
int MyMpeg::Encode(AVFrame* inputbuf, BYTE* outputbuf)
{
	int out_size=0;
	out_size = avcodec_encode_video(c,outputbuf,m_Width*m_Height*3,inputbuf); 
	return out_size;
}

int MyMpeg::Decode(BYTE* inputbuf,int inputbufsize, BYTE* outputbuf)
{
	int z3=0;
	int fd9=avcodec_decode_video(c2,SecI420_picture,&z3,inputbuf,inputbufsize);
	/////////µßµ¹Í¼Ïó
/*	SecI420_picture->data[0] += SecI420_picture->linesize[0] * (m_Height - 1);
	SecI420_picture->linesize[0] *= -1;                      
	SecI420_picture->data[1] += SecI420_picture->linesize[1] * (m_Height / 2 - 1);
	SecI420_picture->linesize[1] *= -1;
	SecI420_picture->data[2] += SecI420_picture->linesize[2] * (m_Height / 2 - 1);
	SecI420_picture->linesize[2] *= -1;*/
	/////////////////
	if (fd9<0)
	{
		return 0;
	}
	int p23=sws_scale(img_convert_ctx2,SecI420_picture->data,SecI420_picture->linesize,
		0,320,SecRGB_picture->data,SecRGB_picture->linesize);
	memcpy(outputbuf,SecRGB_picture->data[0],m_Width*m_Height*3);
	return p23;
}
