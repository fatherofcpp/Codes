#pragma once
#include "DataTypeDef.h"
#include "DataTypeClass.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ValidEdit.h"
// CModBusDefParam �Ի���
#define IDC_LIST_PARAM_DYNAMIC 1
class CParamDefModbusDlg : public CDialog
{
	DECLARE_DYNAMIC(CParamDefModbusDlg)

public:
	CParamDefModbusDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParamDefModbusDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROT_PARAM };
	//
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnNMClickListParam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListParam(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnCbnSelchangeComboDevType();
	afx_msg void OnCbnSelchangeComboWayInd();
	afx_msg void OnEnChangeEditWayNum();
public:
	friend class CParamDefModbusSetDlg;
public:
	void SetModbusParam(CProtModbus* mod_def, VEC_INT vec_id);
	void SetOuterDev(str_outer_dev* outer_dev);//ָ�뿽����Ϊ�˶�̬��ȡ���Ի����е�����

private:
	void ShowModButton(bool show);//��ʾ�޸ģ�ɾ���İ�ť
	void SetCombDefProt();// �����Զ��������ͼ���,�����豸�����±�
	void SetCombWayChoos();// ���û�·ѡ�񼯺�,�����豸�����±�,ע�⣺�����ڻ�·����ʾ����������

	//list
	void CreateAllWayList();//�����б�
	void ShowListByWayInd();//�����豸�����±���ʾ�б�
	void SetListParamAllWay();// ���ò�������,�����豸�����±�
	void ClearListAllWay();//��������б���������
	void GetListInf(int row_ind, st_param_info& param_info, int way_ind);//�б�->˽������
	void SetListInf(int row_ind, st_param_info& param_info, int way_ind);//˽������->�б�
private:
	CProtModbus* pCProtModbus;//Э��
	VEC_INT vecId;//�Ѵ��ڵ�id
	//��ǰ����豸���Զ���������ʹ�����
	//ָ�뿽����Ϊ�˶�̬��ȡ���Ի����е�����
	str_outer_dev* pOuterDev;
	int maxWayNum;//����·��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	// Э������
	CComboBox combSubType;
	// Э������
	ValidEdit editProtName;
	// ������λ
	CComboBox combUnit;
	// �Ĵ�����ַ
	ValidEdit editRegisterAddr;
	// �Ĵ�������
	ValidEdit editRegisterLen;
	CButton btModify;
	// �������
	CListCtrl* listParamArr;
	// �豸����
	CComboBox combDevType;
	CButton btOk;
	// ��·��
	ValidEdit editWayNum;
	// ������Ϣӳ��
	CStatic labParamMapInf;
	// ��·��
	CComboBox combWayInd;
	// ����·�Ĵ�����Ϣ���յ�һ��·
	CButton checkRegIsSame;
	//add

};
