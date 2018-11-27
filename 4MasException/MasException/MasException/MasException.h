#pragma once

#ifdef MASEXCEPTION_EXPORTS
#define MASEXCEPTION_EXPORTS_API(x)			extern "C" __declspec(dllexport) x
#else
#define MASEXCEPTION_EXPORTS_API(x)			extern "C" __declspec(dllimport) x
#endif

//����������
typedef void (CALLBACK *PUrgencyHandling)(PVOID);
typedef INT_PTR (*PMessageBox)(const wchar_t* pstrText, const wchar_t* pstrCaption, UINT, BOOL, BOOL*);

/*
*���ܣ�		�����쳣
*���������������쳣
*����˵����
*			pUrgencyHandling:	����������
*			pMasMessageBox:		������Ϣ����
*			pParam:				��������
*			pErrorText:			������Ϣ
*			pErrorCaption:		�������
*			uMessageBoxType:	��Ϣ�������ͣ�һ�������Ϣ��
*����ֵ��	��
*/

MASEXCEPTION_EXPORTS_API(void) HE_SetUnhandledException(PUrgencyHandling pUrgencyHandling, PMessageBox pMasMessageBox, PVOID pParam, const wchar_t* pErrorText, const wchar_t* pErrorCaption, UINT uMessageBoxType);
