#pragma once
#include "PortA.h"

// COuterDev 对话框
//外设信息
class COuterDev : public CDialog
{
	DECLARE_DYNAMIC(COuterDev)

public:
	COuterDev(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COuterDev();

// 对话框数据
	enum { IDD = IDD_DIALOG_DEV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	////////////////////////////////////////////////////////
public:
	friend class CDtuConfigDlgDlg;
	friend class COuterDevPort;
private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CTabCtrl mTabPort;
private:
	void CreateChildDlg();//创建pPort子对话框
public:
	void SetOuterDev(void*pOuterDev);//设置端口信息
	void InitDlg();//pPort数据->对话框
	void SaveDlg();//pPort对话框->数据
	void SetParentPointer(CDtuConfigDlgDlg* parent);//设置父类指针
	void SetModbusDefArray(CProtModbusSet* param_def_modbus_set);//设置数据
	CProtModbusSet* GetDevDefModbusSet(){return pCProtModbusSet;};
	CDtuConfigDlgDlg* GetParent(){return pParent;};
private:
	bool modify;//对话框是否被修改
	int portNum;//端口个数
	COuterDevPort* pPort[MAX_PORT_NUM];//端口对话框指针
	CDtuConfigDlgDlg* pParent;//父类指针
	CProtModbusSet* pCProtModbusSet;//自定义子类型数组

///////////////////////////////////////////add
};
