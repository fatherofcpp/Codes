#pragma once
#include "PortA.h"

// COuterDev �Ի���
//������Ϣ
class COuterDev : public CDialog
{
	DECLARE_DYNAMIC(COuterDev)

public:
	COuterDev(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COuterDev();

// �Ի�������
	enum { IDD = IDD_DIALOG_DEV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	void CreateChildDlg();//����pPort�ӶԻ���
public:
	void SetOuterDev(void*pOuterDev);//���ö˿���Ϣ
	void InitDlg();//pPort����->�Ի���
	void SaveDlg();//pPort�Ի���->����
	void SetParentPointer(CDtuConfigDlgDlg* parent);//���ø���ָ��
	void SetModbusDefArray(CProtModbusSet* param_def_modbus_set);//��������
	CProtModbusSet* GetDevDefModbusSet(){return pCProtModbusSet;};
	CDtuConfigDlgDlg* GetParent(){return pParent;};
private:
	bool modify;//�Ի����Ƿ��޸�
	int portNum;//�˿ڸ���
	COuterDevPort* pPort[MAX_PORT_NUM];//�˿ڶԻ���ָ��
	CDtuConfigDlgDlg* pParent;//����ָ��
	CProtModbusSet* pCProtModbusSet;//�Զ�������������

///////////////////////////////////////////add
};
