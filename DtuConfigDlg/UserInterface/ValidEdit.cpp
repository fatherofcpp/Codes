#include "../StdAfx.h"
#include "ValidEdit.h"
#include "dataConvert.h"


ValidEdit::ValidEdit()
{
	maxInputLength = 10;
	maxInputValueInt = 0x7FFFFFFF;
	minInputValueInt = 0;
	floatPrecision = 2;
	validType = VALID_ALL_TYPE;
	isModify = false;
	isPermitNull = false;
}

ValidEdit::~ValidEdit(void)
{
}

BEGIN_MESSAGE_MAP(ValidEdit, CEdit)
	//{{AFX_MSG_MAP(CValidEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void ValidEdit::OnChar( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if(validType == VALID_INT)
	{
		ValidCheckInt(nChar, nRepCnt, nFlags);
	}
	else if(validType == VALID_HTTP_ADDR)
	{
		ValidCheckHttpAddr(nChar, nRepCnt, nFlags);
	}
	else if(validType == VALID_FLOAT)
	{
		ValidCheckFloat(nChar, nRepCnt, nFlags);
	}
	else if(validType == VALID_ALL_TYPE)
	{
		ValidCheckALL(nChar, nRepCnt, nFlags);
	}
	else if (validType == VALID_HEX)
	{
		ValidCheckHex(nChar, nRepCnt, nFlags);
	}
	else if (validType == VALID_HOTSPOT_PASS)
	{
		ValidCheckHotSpotPass(nChar, nRepCnt, nFlags);
	}

}

void ValidEdit::SetValidType(ValidType type)
{
	validType = type;
}

void ValidEdit::ValidCheckInt(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bConitue  = (nChar >= '0' && nChar <= '9') 
		|| nChar == VK_BACK;

	if(!bConitue)
		return;
	CString sBefore,sAfter;
	GetWindowText(sBefore);
	//保存光标位置
	int nPosCurbgn, nPosCurend;
	GetSel(nPosCurbgn, nPosCurend);
	CEdit::OnChar(  nChar,  nRepCnt,  nFlags);
	GetWindowText(sAfter);
	int nLength = sAfter.GetLength();
	//如果长度超过设置长度则返回
	if(nLength > maxInputLength)
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}
}

void ValidEdit::ValidCheckHttpAddr(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bConitue  = (nChar >= 'A' && nChar <= 'Z') 
		|| (nChar >= 'a' && nChar <= 'z') 
		|| (nChar >= '0' && nChar <= '9') 
		|| (nChar == ':') 
		|| (nChar == '/') 
		|| (nChar == '.') 
		|| nChar == VK_BACK;

	if(!bConitue)
		return;
	CString sBefore,sAfter;
	GetWindowText(sBefore);
	//保存光标位置
	int nPosCurbgn, nPosCurend;
	GetSel(nPosCurbgn, nPosCurend);
	CEdit::OnChar(  nChar,  nRepCnt,  nFlags);
	GetWindowText(sAfter);
	int nLength = sAfter.GetLength();
	//如果长度超过设置长度则返回
	if(nLength > maxInputLength)
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}
}

void ValidEdit::ValidCheckALL(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bConitue  =  
		nChar != VK_SPACE
		&& nChar != ',';

	if(!bConitue)
		return;

	CString sBefore,sAfter;
	GetWindowText(sBefore);
	//保存光标位置
	int nPosCurbgn, nPosCurend;
	GetSel(nPosCurbgn, nPosCurend);
	CEdit::OnChar(  nChar,  nRepCnt,  nFlags);
	GetWindowText(sAfter);
	int nLength = sAfter.GetLength();
	nLength = get_lenth_gb_to_utf8(sAfter.GetString(),sAfter.GetLength());
	//如果长度超过设置长度则返回
	if(nLength > maxInputLength)
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}
}

void ValidEdit::ValidCheckFloat(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//有效按键是数字和负号正号小数点和回退键
	BOOL bConitue  = (nChar >= '0' && nChar <= '9') 
		|| nChar == '.'
		|| nChar == VK_BACK;

	if(!bConitue)
		return;
	CString sBefore,sAfter;
	GetWindowText(sBefore);
	//保存光标位置
	int nPosCurbgn, nPosCurend;
	GetSel(nPosCurbgn, nPosCurend);
	CEdit::OnChar(  nChar,  nRepCnt,  nFlags);
	GetWindowText(sAfter);
	int nLength = sAfter.GetLength();
	int nPos = sAfter.Find(".");
	//如果长度超过设置长度则返回
	if(nLength > maxInputLength)
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}
	//如果精度超过设置的精度则返回
	if(nPos != CB_ERR && nLength - nPos -1 > floatPrecision)
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}
	//小数点不在首位
	if(nLength > 0 && sAfter.Left(1) == ".")
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}
	//只有一个小数点
	if(sBefore.Find(".") != CB_ERR && nChar == '.')
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}
}

void ValidEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);  
	CString cur_value;
	GetWindowText(cur_value);  
	if(cur_value != preInputValue)
	{
		isModify = true;
	}

	int nLen = cur_value.GetLength();  
	if(nLen == 0)  
	{
		if (!isPermitNull)
		{
			SetWindowText(preInputValue);
		}
	}

	if (validType == VALID_INT)
	{
		MU_INT64 nNum = 0;
		sscanf(cur_value.GetString(),"%lld",&nNum);
		if (nNum > maxInputValueInt || nNum < minInputValueInt)
		{
			if (!isPermitNull)
			{
				SetWindowText(preInputValue);
			}
		}
	}

	if (validType == VALID_HEX)
	{
		if (cur_value.Find("x") != CB_ERR &&
			cur_value.Left(2) != "0x"
			)
		{
			//有x,前两位必须是0x
			SetWindowText(preInputValue);
		}

	}

}

void ValidEdit::OnSetFocus(CWnd* pOldWnd)
{
	GetWindowText(preInputValue);
	CEdit::OnSetFocus(pOldWnd);  
}

bool ValidEdit::GetModify()
{
	return isModify;
}

void ValidEdit::SetMaxLen( int len )
{
	maxInputLength = len;
}

void ValidEdit::SetMaxNum( MU_INT64 num )
{
	maxInputValueInt = num;
}

void ValidEdit::SetMinNum( MU_INT64 num )
{
	minInputValueInt = num;
}


BOOL ValidEdit::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_RBUTTONDOWN) 
	{  
		//	return TRUE;
	} 
	return CEdit::PreTranslateMessage(pMsg);
}

void ValidEdit::ValidCheckHex( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	BOOL bConitue  = (nChar >= '0' && nChar <= '9') 
		|| (nChar >= 'a' && nChar <= 'f')
		|| (nChar >= 'A' && nChar <= 'F')
		|| (nChar == 'x')
		|| nChar == VK_BACK;

	if(!bConitue)
		return;
	CString sBefore,sAfter;
	GetWindowText(sBefore);
	//保存光标位置
	int nPosCurbgn, nPosCurend;
	GetSel(nPosCurbgn, nPosCurend);
	CEdit::OnChar(  nChar,  nRepCnt,  nFlags);
	GetWindowText(sAfter);
	int nLength = sAfter.GetLength();
	//如果长度超过设置长度则返回
	if(nLength > maxInputLength)
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}

	//add
	////只有一个x
	if(sBefore.Find("x") != CB_ERR && nChar == 'x')
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}

}

void ValidEdit::SetPermitNull( bool is_permit )
{
	isPermitNull = is_permit;
}

void ValidEdit::ValidCheckHotSpotPass( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	BOOL bConitue  = (nChar >= 'A' && nChar <= 'Z') 
		|| (nChar >= 'a' && nChar <= 'z') 
		|| (nChar >= '0' && nChar <= '9') 
		|| nChar == VK_BACK;

	if(!bConitue)
		return;
	CString sBefore,sAfter;
	GetWindowText(sBefore);
	//保存光标位置
	int nPosCurbgn, nPosCurend;
	GetSel(nPosCurbgn, nPosCurend);
	CEdit::OnChar(  nChar,  nRepCnt,  nFlags);
	GetWindowText(sAfter);
	int nLength = sAfter.GetLength();
	//如果长度超过设置长度则返回
	if(nLength > maxInputLength)
	{
		SetWindowText(sBefore);
		SetSel(nPosCurbgn,nPosCurend,true);
		return;
	}
}
