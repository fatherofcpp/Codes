#pragma once
#include "stdafx.h"
#include "H264Stream.h"

#include "common.h"
#include "dvsstreamplayerctrl1.h"
#include "ts2frame.h"

typedef long long        		Int64;

//#include "common/types.h"
//#include "sdk_Config.h"
//#include "sdk_Header.h"
//#include "md5.h"
#include "MD5/MD5.h"
typedef struct tagDEV_VMS_INFO{
 char		m_dstIp[32];		//Ŀ�������IP
 char		m_dstPub[32];		//������
 LONG		m_dstStream;	//���±�
 LONG		m_userId;//UserID
 LONG		m_realPlayHandle;//RealPlayHandle
 TS2FRAME	ts;
 int		bFindStart;
 int		m_width;
 int		m_height;

}DEV_VMS_INFO;

typedef struct tagDEV_VMS{//����ʹ�ò����ṹ
 char		m_proxyIp[64];		//ת��������IP
 LONG		m_proxyPort;		//ת���������˿�
 char		m_userName[32];		//�û���
 char		m_passWord[32];		//����
 __int64		m_deviceChn;	//�豸ͨ��
 LONG		m_playChn;		//����ͨ��



 DEV_VMS_INFO m_devInfo;

}DEV_VMS_PARAMS;

typedef struct tagVideoRawInfo
{
	int            width;
	int            height;
	long           length;
	int            pitch;
	char           *data;
}VideoRawInfo, *PVideoRawInfo;
typedef int (*UserDataFuc)();
enum StreamType
{
	TYPE_UK = 0x0,

	VIDEO_RTP_H264 = 0x1,
	TS_PACKET_DATA = 0x2,

	AUDIO_RTP_G711_A = 0x4,
	AUDIO_RTP_G711_U = 0x8,
	AUDIO_RTP_G726 = 0x10,

	VIDEO_YUV12_RAW = 0x20,
	AUDIO_RAW_G711A = 0x40,
	AUDIO_RAW_G711U = 0x80,
	AUDIO_RAW_G726 = 0x100
};

#define  WM_GLOBAL_EXIT (WM_USER+103)
#define  WM_LIVEPLAY_PLAYSTOP (WM_USER+104)
#define  WM_LIVEPLAY_SETMN (WM_USER + 105)
#define  WM_LIVE_FINAL  (WM_USER + 109)

#define ERR_REGISTERCB_FAILED   (-12)  //�ص�ע��ʧ��
#define ERR_STATE_WRONG         (-4)   //״̬���󣬱��粥��֮���ٵ���Play��ֹ֮ͣ���ٵ���Stop�������������ִ˴���
// CVmsDlg �Ի���
class CVmsDlg : public CDialog, public CCommon
{
	DECLARE_DYNAMIC(CVmsDlg)

public:
	CVmsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVmsDlg();

	DEV_VMS_PARAMS m_devParams[16];
	CCriticalSection m_rawlock[16];

	CArray<MyAvFrame*,MyAvFrame*> p_rawArray[16];
	CArray<MyAvFrame*,MyAvFrame*> p_tsArray[16];

	char       m_udpDst[64];
	short		m_udpPort;

// �Ի�������
	enum { IDD = IDD_VMS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	int GetInforFromIni4Company(void * pThisDir,int nIndex);
//	int GetInforFromIni(void* iniPath);
	int SetDefaultParams(void * pParams);
	int ParseRequest(char * pXmlBuf,CString *str);

	CString CheckExit(CString str);
	CString ChangeChannel(CString sId,int CH,CString str);

	// ������������
	int SetParams(void *pStream,void * pParams);
	int StartPull(void *pStream,void* pParams);
	int ClosePull(void *pStream,void * pParams);

//add
public:
	HANDLE m_Lock;
	static HWND m_wnd;//�Ի�����������������Ϣ��DemoDlg
	BOOL m_Logflag;//�Ƿ��½:0 δ��¼;1 ��½

public:
	static int CBKeepAliveFail(int CBType, void* p1, void* p2);
	static int ClientCallBackFunc(int CBType, void* p1, void* p2);

	LRESULT OnGlobalExit( WPARAM wParam, LPARAM lParam );

	HRESULT OnPlayStop( WPARAM wParam, LPARAM lParam );
	HRESULT OnSetMN( WPARAM wParam, LPARAM lParam );
	HRESULT OnLiveFinal( WPARAM wParam, LPARAM lParam );

	void LogOut(void);

public:
//	CDvsstreamplayerctrl1 m_player;

//end add
};
