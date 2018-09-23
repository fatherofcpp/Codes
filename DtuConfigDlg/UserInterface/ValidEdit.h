#pragma once
#include "include/mty.h"

enum ValidType
{
	VALID_INT = 0,//int
	VALID_HTTP_ADDR,//domain
	VALID_FLOAT,//预留
	VALID_HEX,//16进制输入
	VALID_HOTSPOT_PASS,//wifi密码
	VALID_ALL_TYPE,//所有类型
};

class ValidEdit :
	public CEdit
{
public:
	ValidEdit();
	~ValidEdit(void);
protected:
	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

private:
	void ValidCheckInt(UINT nChar, UINT nRepCnt, UINT nFlags);
	void ValidCheckFloat(UINT nChar, UINT nRepCnt, UINT nFlags);
	void ValidCheckHttpAddr(UINT nChar, UINT nRepCnt, UINT nFlags);
	void ValidCheckALL(UINT nChar, UINT nRepCnt, UINT nFlags);
	void ValidCheckHex(UINT nChar, UINT nRepCnt, UINT nFlags);
	void ValidCheckHotSpotPass(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	void SetValidType(ValidType type);
	void SetMaxLen(int len);
	void SetMaxNum(MU_INT64 num);
	void SetMinNum(MU_INT64 num);
	bool GetModify();
	void SetPermitNull(bool is_permit);
private:
	//通用
	int maxInputLength;//最大输入长度
	bool isModify;//值是否修改
	CString preInputValue;//修改前的值
	ValidType validType;//验证类型
	bool isPermitNull;//是否允许输入空值

	//int
	MU_INT64 maxInputValueInt;//Int输入的最大值
	MU_INT64 minInputValueInt;//Int输入的最小值
	//float
	int floatPrecision;//精度

};

