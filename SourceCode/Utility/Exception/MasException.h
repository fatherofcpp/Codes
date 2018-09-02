#pragma once

//紧急处理函数
typedef void (CALLBACK *PUrgencyHandling)(PVOID);
typedef INT_PTR (*PMessageBox)(const wchar_t* pstrText, const wchar_t* pstrCaption, UINT, BOOL, BOOL*);

/*
*功能：		捕获异常
*功能描述：捕获异常
*参数说明：
*			pUrgencyHandling:	紧急处理函数
*			pMasMessageBox:		弹出信息函数
*			pParam:				描述变量
*			pErrorText:			错误信息
*			pErrorCaption:		错误标题
*			uMessageBoxType:	信息窗口类型（一般错误信息框）
*返回值：	无
*/

void HE_SetUnhandledException(PUrgencyHandling pUrgencyHandling, PMessageBox pMasMessageBox, PVOID pParam, const wchar_t* pErrorText, const wchar_t* pErrorCaption, UINT uMessageBoxType);
