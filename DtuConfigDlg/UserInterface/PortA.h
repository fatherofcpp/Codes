#pragma once
#include "AddDev.h"
#include "xmlAnalyze.h"
#include "Transfer.h"
#include "CheckWay.h"
#include "DataTypeDef.h"
#include "DataTypeClass.h"
#include "Progress.h"

// COuterDevPort �Ի���

class COuterDevPort : public CDialog
{
	DECLARE_DYNAMIC(COuterDevPort)

public:
	COuterDevPort(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COuterDevPort();

// �Ի�������
	enum { IDD = IDD_DIALOG_PORTA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	/////////////////////////////////////////////
public:
	friend class CDtuConfigDlgDlg;
	friend class COuterDev;

private:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	CListCtrl mList;
	CButton mDeleteDev;
	CButton mModifyDev;
	CButton mCheckDev;
private:
	afx_msg void OnBnClickedDeleteDev();
	afx_msg void OnBnClickedAddDev();
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedCheckSelectedDev();
	afx_msg void OnNMClickListPorta(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListPorta(NMHDR *pNMHDR, LRESULT *pResult);
private:
	void DisplaySaveButton();//��ʾ���水ť
	void ShowDelModCheckButton(bool show);//��ʾ�޸ģ�ɾ����ʵʱ���ݵİ�ť
public:
	//����ӿ�
	void InitDlg();//����->�Ի���
	void SaveDlg();//�Ի���->����
	void CreateList();//�����б�
	void SetPortDev(st_port_dev* pportDev);//���ö˿���Ϣ 
	void SetPortInd(int ind);//���ö˿ں�
	void SetParentPointer(COuterDev* parent);//���ø���ָ��
	COuterDev* GetParentPointer(){return pParent;};
public:
	//�ļ��ڲ�����
	CTaskParam* dataFrame;
	CProgress* pProgress;
public:
	//���ļ��ڲ�����
	int GetRealData(int port_ind, MU_INT64 dev_id,e_dev_sub_type sub_type, int way_ind,u_w_measure& w_measure);
	int GetPortInd();
	void CloseProgress();//�رս�����

private:
	void GetWayInfByIdSubType(st_devid_subtype& id_subtype,st_way_inf& way_inf);//ͨ��id subtype��ȡ��·��Ϣ
	void SetWayInfByIdSubType(st_devid_subtype& id_subtype,st_way_inf& way_inf);//ͨ��id subtype�����·��Ϣ
	void InitWayInfWithIdSubType();//���汾�ӿ��������豸��id subtype���·��Ϣ��������
	void GetAllPortDevIdSubTypeExceptParam(VEC_DEVID_SUBTYPE& vec_id_subtype,st_devid_subtype& id_subtype);//ͳ�����ж˿ڳ��˲���id_subtype֮������������豸id subtype����
	void GetAllPortWayNameExceptParam(VEC_CSTRING& vec_way_name,st_way_inf way_inf);//ͳ�Ƴ��˲���way_inf֮������нӿ��豸�Ļ�·���Ƽ���
	void GetAllPortWayIdExceptParam(VEC_INT& vec_way_id,st_way_inf way_inf);//ͳ�Ƴ��˲���way_inf֮������нӿ��豸�Ļ�·id����
	int  GetListInf(int row_ind, st_dev_inf& dev_inf);//��list��Ӧ����ȡ�豸��Ϣ
	void SetListInf(int row_ind, st_dev_inf& dev_inf);//�����豸��Ϣ����list��Ӧ����Ϣ
	int AddThreadTsk(e_thread_type type,st_dev_inf* dev_inf,VEC_DEV_ALL_WAY_DATA_INF* way_check_inf);//�߳�����
private:
	int portInd;//0-3�˿ں�
	COuterDev* pParent;//����ָ��
	CAddDev* pAddDev;//����ָ���豸��·�Ի���
	CShowDevAllWayData* pCheckWay;//����ָ��ʵʱ���ݶԻ���
	CCriticalSection taskLock;//�߳�������

	st_port_dev* pPortDev;//�˿���Ϣ
	VEC_WAY_NAME_ID_SUBTYPE vecWayInfWithIdSubType;//���л�·��Ϣ��id subtypeӳ���ϵ
	//////////////////////////////////add
};
