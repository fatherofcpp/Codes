#pragma once

#include "DataTypeDef.h"
#include "DataTypeClass.h"
#include "ValidEdit.h"
#include "afxwin.h"
#include "afxcmn.h"
// CReportFormat �Ի���

class CReportFormat : public CDialog
{
	DECLARE_DYNAMIC(CReportFormat)

public:
	CReportFormat(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReportFormat();

	// �Ի�������
	enum { IDD = IDD_DIALOG_REPORT_FORMAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	/////////////////////////////////////////////////////////

private:
	//����
	virtual BOOL OnInitDialog();//�Ի����ʼ��
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	//��̬�ؼ�
	CComboBox mCombReportParam;
	CButton mBtAddParam;
	CButton mBtDeleteParam;
	CButton mBtModifyParam;
	CButton mBtAllDevDefaultParamSet;//�����豸Ĭ���ϱ�����
	CComboBox mComBDevType;
	// �����豸�ϱ�����Ԥ���б�
	CListCtrl listAllDevReportView;
	// ��ǰ�豸Ĭ���ϱ�����
	CButton btCurDevDefaultReportParam;
	//��̬�ؼ�
	CListCtrl* m_pListCtrl;//�б�����,�б��±��������豸���Ͷ�Ӧ

	//�ؼ���Ӧ
	afx_msg void OnBnClickedButtonAddParam();
	afx_msg void OnBnClickedButtonDeleteParam();
	afx_msg void OnBnClickedButtonModifyParam();
	afx_msg void OnCbnSelchangeComboDevtype();
	afx_msg void OnBnClickedButtonAllDevDefaultParamSet();
	afx_msg void OnBnClickedButtonCurDevDefaultParam();
	//�ؼ��ӿ�
	void SetCombReportParam();// ����������ѡ��,�����豸�����±�
	void SetCombDevType();

	void CreateAllDevList();//�����б�
	void ShowListByDevTypeInd();//�����豸�����±���ʾ�б�
	void ClearListDataAllDev();//��������б���������

	void GetListControlAllDev();//�б�->˽������
	void SetListControlAllDev();//˽������->�б�

	void DisplaySaveButton();//��ʾ���水ť

	bool IsParamNameExist(string param_name);//���Ӳ������޸Ĳ���ʱ�жϵ�ǰ�豸�������Ƿ���ڸĲ�����
	void SetAllDevDefaultParam();//��Ĭ��ȫѡ��ʽ����ÿ���豸�����µ��ϱ�����

	void CreateListAllDevReportView();//�����б�
	void ClearListDataAllDevReportView();//��������б���������
	void SetListControlAllDevReportView();//˽������->�б�
	void SetDefaultParamByListInd(int list_ind);//��Ĭ��ȫѡ��ʽ���õ�ǰ�豸�����µ��ϱ�����
	void ClearListDataByListInd(int list_ind);//��յ�ǰ�豸�б���������

	//˽�����ݽӿ�
	void SetModify(bool& modify);
	void ClearReportFormat();

	void ShowInput(bool show);//��ʾ

public:
	//����Ȩ��
	friend class CDtuConfigDlgDlg;

public:
	//����ӿ�
	void GetReportFormat(st_report_store_inf* report_format);
	void SetReportFormat(st_report_store_inf* report_format);
	void InitDlg();//˽������->�Ի���
	int SaveDlg();//�Ի���->˽������

private:
	//˽������
	int devTypeNum;//�豸���͸���
	int currentDevTypeInd;//��ǰ�б��±�,�豸���͵��±�	
	CParamDevTypeSet* pCParamDevSet;//����������,������CDevTypeReportParam
	st_report_store_inf* pReportFormatStore;//�洢�ϱ���ʽ,��ǰ����
	///////////////////////////////add
};
