#pragma once
#include "DataTypeDef.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ModBusDefParam.h"

// CProtocolTemplate �Ի���

class CParamDefModbusSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CParamDefModbusSetDlg)

public:
	CParamDefModbusSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParamDefModbusSetDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROTO_TEMPLATE };
	//
	friend class CDtuConfigDlgDlg;
public:
	void SetModbusDefArray(CProtModbusSet* param_def_modbus_set);//��������
	void InitDlg();//����->�Ի���
	int SaveDlg();//�Ի���->����
	bool GetIsModify(){return isModify;};
	void SetOuterDev(str_outer_dev* outer_dev);
private:
	void CreateList();//�����б�
	void DisplaySaveButton();//��ʾ���水ť
	void ShowDelModCheckButton(bool show);//��ʾ�޸ģ�ɾ����ʵʱ���ݵİ�ť
	int  GetListInf(int row_ind, CProtModbus& mod_temp);//��list��Ӧ����ȡ�豸��Ϣ
	void SetListInf(int row_ind, CProtModbus& mod_temp);//�����豸��Ϣ����list��Ӧ����Ϣ
	void InitMapSubTypeProt();//��ʼ��Э��id�������Ϣӳ��
	void AddMapIdProt(int id, int prot_modbus_arr_ind);//����Э��id�������Ϣӳ��
	void DelMapIdProt(int id);//ɾ��Э��id�������Ϣӳ��
	void GetAllIdExceptParam(VEC_INT& vec_id,int& except_id);//ͳ�����ж˿ڳ��˲���֮�����������id����
	int GetParamInfById(int id, CParamModbus& param_modbus);//ͨ��id��ȡģ��Ĳ�����Ϣ
	int SetParamInfById(int id, CParamModbus& param_modbus);//ͨ��id����ģ��Ĳ�����Ϣ
	void ShowInput(bool show);//��ʾ

private:
	CProtModbusSet* pCProtModbusSet;//�Զ�������������

	CParamDefModbusDlg* pCParamDefModbusDlg;//Э��Ի���
	MAP_INT_INT mapSubTypeProt;//������,CParamDevSet
	bool isModify;
	str_outer_dev* outerDev;//��ǰ����豸���Զ���������ʹ�����
	//
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton btAdd;
	CButton btDelete;
	CButton btModify;
	// Э��ģ���б�
	CListCtrl listProtTemplate;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnNMClickListProtocolTemplate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListProtocolTemplate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLoadLocalDefSubtype();
	afx_msg void OnBnClickedButtonSaveLocalDefSubtype();
	// ���ر����Զ���������
	CButton btLoadLocalDef;
	// ���汾���Զ���������
	CButton btSaveLocalDef;
};
