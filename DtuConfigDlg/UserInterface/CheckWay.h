#pragma once
#include "DataTypeDef.h"

// CCheckWay �Ի���

class CShowDevAllWayData : public CDialog
{
	DECLARE_DYNAMIC(CShowDevAllWayData)

public:
	CShowDevAllWayData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowDevAllWayData();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHECK_WAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	/////////////////////////////////////////////////////////////////////////////
private:
	virtual BOOL OnInitDialog();
private:
	afx_msg void OnBnClickedButtonVailidData();
	afx_msg void OnBnClickedButtonInvalidData();
private:
	CListCtrl mList;
private:
	void CreateList();//�����б�
public:
	void SetDevAllWayData(int dev_type, VEC_DEV_ALL_WAY_DATA_INF & way_check_inf, st_report_format* report_format);//�������л�·������
private:
	VEC_DEV_ALL_WAY_DATA_INF devAllWayData;//�豸���л�·�����ݼ���
	e_dev_type devType;//�豸����
	st_report_format* pReportFormat;//��ǰ�ϱ�����
};
