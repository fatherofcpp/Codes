

#ifndef DEF_DFHS_H_
#define DEF_DFHS_H_

typedef  unsigned char  u8;
//typedef  char BYTE;

/*ý�����Ͷ���*/
#define   MEDIA_TYPE_NULL                 (u8)255  /*ý������Ϊ��*/	
/*��Ƶ*/
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
/*��Ƶ*/
#define   MEDIA_TYPE_MP4	              (u8)97  /*MPEG-4*/
#define   MEDIA_TYPE_H261	              (u8)31  /*H.261*/
#define   MEDIA_TYPE_H262	              (u8)33  /*H.262 (MPEG-2)*/
#define   MEDIA_TYPE_H263	              (u8)34  /*H.263*/
#define   MEDIA_TYPE_H263PLUS             (u8)101 /*H.263+*/
#define   MEDIA_TYPE_H264	              (u8)106 /*H.264*/

typedef struct tagTRawFrameInfo
{
    BYTE     m_byMediaType; //ý������(�μ�/*ý�����Ͷ���*/)
    BYTE     m_byFrameRate; //֡��
    DWORD    m_dwFrameID;   //֡��ʶ
    DWORD    m_dwTimeStamp; //ʱ���
    DWORD    m_dwSSRC;      //ͬ��Դ
    union
    {
        struct{
                   BOOL       m_bKeyFrame;    //Ƶ֡���ͣ�I or P��
                   WORD       m_wVideoWidth;  //��Ƶ֡��
                   WORD       m_wVideoHeight; //��Ƶ֡��
              }m_tVideoParam;
        BYTE    m_byAudioMode;				//��Ƶģʽ
    };	
}TRawFrameInfo,*PTRawFrameInfo;
//�����֡��Ϣ�ṹ��������Ƶ����Ƶ��
typedef struct tagTFrameInfo
{
    BYTE     m_byMediaType; 	//ý������(PCM��YUV420)
    DWORD    m_dwTimeStamp; 	//ʱ���   
    union
    {
    	BYTE     m_byBitCount;	//ΪYUVʱ����ɫbitλ�� 
    	BYTE     m_bStereo;		//ΪPCMʱ������ֵ
    };
    union
    {
   		WORD     m_wVideoWidth;  //��Ƶ֡��
   		WORD	 m_wBitRate;	//����bit��(��8000)
   	};
   	union
   	{
   		WORD     m_wVideoHeight; //��Ƶ֡��
   		WORD 	 m_wBand;		//������λ��(��16)
   	};
}TFrameInfo,*PFrameInfo;


typedef struct tagDEV_DFHS_INFO{
 CString  m_deviceId;		//�豸ID
 LONG	  m_deviceChn;	//�豸ͨ��
 CString  m_dstIp;		//Ŀ�������IP
 CString  m_dstPub;		//������
 LONG	  m_dstStream;	//���±�
}DEV_DFHS_INFO;

typedef struct tagDEV_DFHS{//�ط�����ʹ�ò����ṹ
 CString  m_proxyIp;		//ת��������IP
 LONG	  m_proxyPort;		//ת���������˿�
 CString  m_userName;		//�û���
 CString  m_passWord;		//����

 DEV_DFHS_INFO m_devInfo[16];


}DEV_DFHS_PARAMS;



#endif//

