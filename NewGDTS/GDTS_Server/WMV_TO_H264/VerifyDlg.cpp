// VerifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WMV_TO_H264.h"
#include "VerifyDlg.h"
#include <windows.h>
#include <nb30.h>
typedef struct _ASTAT_
{
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
}ASTAT, * PASTAT;

ASTAT Adapter; 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"Iphlpapi.lib")

char szme[]="HAOABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"\
"/+9876543210zyxwvutsrqponmlkjhgfdebcaZXCVBNMASDFGHJKLQWERTYUIOP";
/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg dialog

CVerifyDlg::CVerifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVerifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVerifyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVerifyDlg)
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVerifyDlg, CDialog)
	//{{AFX_MSG_MAP(CVerifyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg message handlers

BOOL CVerifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nlocate = GetLocalinfo();
	if (nlocate != 0)
	{
		//G_Flag = -1 ;
		AfxMessageBox("����û��ͨ����֤����������֤��!");
//		GetDlgItem(IDCANCEL)->SetWindowText("�Ѿ�ͨ����֤");
		char temp[32];
		memset(temp,0,32);
		GetParam(temp);
	}
	else
	{
		//G_Flag = 0 ;
		//GetDlgItem(IDCANCEL)->SetWindowText("�Ѿ�ͨ����֤");
		//GetDlgItem(IDOK)->EnableWindow(FALSE);
		//GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CVerifyDlg::GetLocalinfo()
{
	int bRet = 0 ;
	FILE *pf ;
	unsigned char temp_Loc[16];
	pf = fopen("encode.data","rb");
	if (pf == NULL)
	{
		return -1 ;
	}
	else
	{
		fread(temp_Loc,1,16,pf);
		fclose(pf);
	}

	char temp_Old[20],temp_new[1024];
	unsigned char dst[16];
	GetParam(temp_Old);
	memcpy(temp_new,temp_Old,16);
	memcpy(temp_new+16,szme,strlen(szme));
	memcpy(temp_new+16+strlen(szme),"\0",1);
	md.MessageDigest((const unsigned char*)temp_new,strlen(temp_new),dst);

	int idx = 0;
	while ((idx < 16) && (temp_Loc[idx] == dst[idx]) )
	{
		idx++;
	}
	if (idx < 16)
	{
		bRet = -2;
	}
	return bRet ;
}

int CVerifyDlg::GetParam(char dst[])
{
	unsigned long s1,s2,s3,s4;
	CString str1,str2;
	
	unsigned char tmpPauth[16];
	unsigned char vendor_id[]="------------";
	__asm xor eax,eax
		__asm cpuid
		__asm mov dword ptr vendor_id,ebx
		__asm mov dword ptr vendor_id[+4],edx
		__asm mov dword ptr vendor_id[+8],ecx
	str1.Format("%s-",vendor_id);
	__asm mov eax,01h
		__asm xor edx,edx
		__asm cpuid
		__asm mov s1,edx
		__asm mov s2,eax
	__asm mov eax,03h
		__asm xor ecx,ecx
		__asm xor edx,edx
		__asm cpuid
		__asm mov s3,edx
		__asm mov s4,ecx
	str2.Format("%08X%08X%08X%08X",s1,s2,s3,s4);
	Get_MAC();
	sprintf(szParam,"%s%s%s",str1,str2,szMAC);	
	//sprintf(szParam,"%s%s",str1,str2);
	
	md.MessageDigest((const unsigned char*)szParam,strlen(szParam),tmpPauth);
	char_convert((const char*)tmpPauth,dst);

	CString str3;
	str3.Format("%s",dst);
	m_edit1.SetWindowText(str3);
	return 1 ;
}

void CVerifyDlg::Get_MAC()
{
	int i=0;
	PIP_ADAPTER_INFO pAdapterInfo;ULONG ulOutbufLen = sizeof(IP_ADAPTER_INFO);//ע����ULONG����,����PULONG����
	pAdapterInfo = ( IP_ADAPTER_INFO *)malloc( sizeof(IP_ADAPTER_INFO) );//Ϊ�䶯̬�����ڴ�,����С����,������е���

	if ( ERROR_BUFFER_OVERFLOW == GetAdaptersInfo( pAdapterInfo, &ulOutbufLen) )
	{//MSDN�Ͻ���,��Ϊ��ȡ����Ϣ,���ܳ���Ԥ�����õĻ���������,��ɷ���,��һ�����ػ�ȡ�����Ͻ�Ϊ��,�˴�Ϊ��ֹ�����������,
		free(pAdapterInfo);//���Բ���һ�½��,ȥ���ⲿ�ִ����,����ֵΪ��.�˴��ع�,ֱ���ɹ�
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutbufLen); //��1�δ�С����,��2�η������賤��
	}
	if ( ERROR_SUCCESS == GetAdaptersInfo( pAdapterInfo, &ulOutbufLen ))//MSDN��������д,Ҳ��д��NO_ERROR,����ȷ



		sprintf(szMAC,"%02x%02x-%02x%02x-%02x%02x\n",
		pAdapterInfo->Address[0], //ע��˴��������ʽ,�����е�û��[0][1]��,str.Format��ʽ����sprintf��MFC��
		pAdapterInfo->Address[1],
		pAdapterInfo->Address[2],
		pAdapterInfo->Address[3],
		pAdapterInfo->Address[4],
		pAdapterInfo->Address[5]);//,pAdapterInfo->Address[6],�������֪����7���Ǹ�ʲôֵ?����һ��,����ֵΪ00(����ֵ)

	//m_Mac.SetWindowText(str);

}


void CVerifyDlg::char_convert(LPCTSTR lpszStr, char *str)
{
	char temp[32];
	int k;
	memcpy(temp,lpszStr,16);
	for (k = 0 ; k < 16 ;k++)
	{
		if (temp[k]<0)
		{
			temp[k] *= (-1);
		}
		if ((temp[k]%0x10)<10)
		{
			str[k]=(temp[k]%0x10)+0x30;
		}
		else
		{
			str[k]=(temp[k]%0x10)-10+0x40;
		}		
	}
	str[k]='\0';
}

void CVerifyDlg::OnOK() 
{
	// TODO: Add extra validation here
	char tmp[1024];
	char dstF[20],dstEdit2[20];
	GetDlgItemText(IDC_EDIT1,tmp,17);
	GetDlgItemText(IDC_EDIT2,dstEdit2,17);
	memcpy(tmp+16,szme,strlen(szme));
	memcpy(tmp+16+strlen(szme),"\0",1);
	unsigned char pauth1[16];
	md.MessageDigest((const unsigned char*)tmp,strlen(tmp),pauth1);

	char_convert((const char*)pauth1,dstF);

	int n = strcmp(dstF,dstEdit2);
	if (n == 0)
	{
//		G_Flag = 0 ;
		FILE *pf;
		pf = fopen("encode.data","wb+");
		if (pf)
		{
			fwrite(pauth1,1,16,pf);
			fclose(pf);
		}	
		
	}
	else
	{
		AfxMessageBox("��֤���������������!");
		return;
	}

	CDialog::OnOK();
}

void CVerifyDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
