#pragma once
#include "include/mty.h"

enum ValidType
{
	VALID_INT = 0,//int
	VALID_HTTP_ADDR,//domain
	VALID_FLOAT,//Ԥ��
	VALID_HEX,//16��������
	VALID_HOTSPOT_PASS,//wifi����
	VALID_ALL_TYPE,//��������
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
	//ͨ��
	int maxInputLength;//������볤��
	bool isModify;//ֵ�Ƿ��޸�
	CString preInputValue;//�޸�ǰ��ֵ
	ValidType validType;//��֤����
	bool isPermitNull;//�Ƿ����������ֵ

	//int
	MU_INT64 maxInputValueInt;//Int��������ֵ
	MU_INT64 minInputValueInt;//Int�������Сֵ
	//float
	int floatPrecision;//����

};

