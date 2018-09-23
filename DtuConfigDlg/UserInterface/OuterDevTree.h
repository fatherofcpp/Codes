#pragma once
#include "DataTypeDef.h"

#include "afxcmn.h"

// COuterDevTree �Ի���

class COuterDevTreeShow : public CDialog
{
	DECLARE_DYNAMIC(COuterDevTreeShow)

public:
	COuterDevTreeShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COuterDevTreeShow();

// �Ի�������
	enum { IDD = IDD_DIALOG_TREE_SHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	
private:
	HTREEITEM CreateTreeElement(CString & name,HTREEITEM hCurItem);
	void SetTreeStyle();
	void ExpandAllNode(HTREEITEM hItem, CTreeCtrl& treeShow);

private:
	CTreeCtrl m_tree;
private:
	str_outer_dev* pOuterDev;
	
public:
	void SetOuterDev(str_outer_dev* outer_dev);
	void ShowTree();
public:
	friend class CDtuConfigDlgDlg;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
