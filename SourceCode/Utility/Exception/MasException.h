#pragma once

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

void HE_SetUnhandledException(PUrgencyHandling pUrgencyHandling, PMessageBox pMasMessageBox, PVOID pParam, const wchar_t* pErrorText, const wchar_t* pErrorCaption, UINT uMessageBoxType);
