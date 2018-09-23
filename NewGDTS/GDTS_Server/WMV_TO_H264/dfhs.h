

#ifndef DEF_DFHS_H_
#define DEF_DFHS_H_

typedef  unsigned char  u8;
//typedef  char BYTE;

/*媒体类型定义*/
#define   MEDIA_TYPE_NULL                 (u8)255  /*媒体类型为空*/	
/*音频*/
#define	  MEDIA_TYPE_MP3	              (u8)96  /*mp3 mode 0-4*/
#define   MEDIA_TYPE_G7221C	              (u8)98  /*G722.1.C Siren14*/
#define   MEDIA_TYPE_PCMA		          (u8)8   /*G.711 Alaw  mode 5*/
#define   MEDIA_TYPE_PCMU		          (u8)0   /*G.711 ulaw  mode 6*/
#define   MEDIA_TYPE_G721		          (u8)2   /*G.721*/
#define   MEDIA_TYPE_G722		          (u8)9   /*G.722*/
#define	  MEDIA_TYPE_G7231		          (u8)4   /*G.7231*/
#define   MEDIA_TYPE_ADPCM                (u8)5   /*DVI4 ADPCM*/
#define	  MEDIA_TYPE_G728		          (u8)15  /*G.728*/
#define	  MEDIA_TYPE_G729		          (u8)18  /*G.729*/
#define   MEDIA_TYPE_G7221                (u8)13  /*G.7221*/
/*视频*/
#define   MEDIA_TYPE_MP4	              (u8)97  /*MPEG-4*/
#define   MEDIA_TYPE_H261	              (u8)31  /*H.261*/
#define   MEDIA_TYPE_H262	              (u8)33  /*H.262 (MPEG-2)*/
#define   MEDIA_TYPE_H263	              (u8)34  /*H.263*/
#define   MEDIA_TYPE_H263PLUS             (u8)101 /*H.263+*/
#define   MEDIA_TYPE_H264	              (u8)106 /*H.264*/

typedef struct tagTRawFrameInfo
{
    BYTE     m_byMediaType; //媒体类型(参见/*媒体类型定义*/)
    BYTE     m_byFrameRate; //帧率
    DWORD    m_dwFrameID;   //帧标识
    DWORD    m_dwTimeStamp; //时间戳
    DWORD    m_dwSSRC;      //同步源
    union
    {
        struct{
                   BOOL       m_bKeyFrame;    //频帧类型（I or P）
                   WORD       m_wVideoWidth;  //视频帧宽
                   WORD       m_wVideoHeight; //视频帧宽
              }m_tVideoParam;
        BYTE    m_byAudioMode;				//音频模式
    };	
}TRawFrameInfo,*PTRawFrameInfo;
//解码后帧信息结构（包括音频和视频）
typedef struct tagTFrameInfo
{
    BYTE     m_byMediaType; 	//媒体类型(PCM或YUV420)
    DWORD    m_dwTimeStamp; 	//时间戳   
    union
    {
    	BYTE     m_byBitCount;	//为YUV时的颜色bit位数 
    	BYTE     m_bStereo;		//为PCM时的声道值
    };
    union
    {
   		WORD     m_wVideoWidth;  //视频帧宽
   		WORD	 m_wBitRate;	//采样bit率(如8000)
   	};
   	union
   	{
   		WORD     m_wVideoHeight; //视频帧宽
   		WORD 	 m_wBand;		//采样的位数(如16)
   	};
}TFrameInfo,*PFrameInfo;


typedef struct tagDEV_DFHS_INFO{
 CString  m_deviceId;		//设备ID
 LONG	  m_deviceChn;	//设备通道
 CString  m_dstIp;		//目标服务器IP
 CString  m_dstPub;		//发布名
 LONG	  m_dstStream;	//流下标
}DEV_DFHS_INFO;

typedef struct tagDEV_DFHS{//地方海事使用参数结构
 CString  m_proxyIp;		//转发服务器IP
 LONG	  m_proxyPort;		//转发服务器端口
 CString  m_userName;		//用户名
 CString  m_passWord;		//密码

 DEV_DFHS_INFO m_devInfo[16];


}DEV_DFHS_PARAMS;



#endif//

