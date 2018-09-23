#pragma once
#include "DataTypeDef.h"
#include "DataTypeClass.h"
#include "ValidEdit.h"
#include "afxwin.h"
#include "Progress.h"

// CAddDev �Ի���
class CAddDev : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDev)

public:
	CAddDev(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddDev();

	// �Ի�������
	enum { IDD = IDD_DIALOG_ADDDEV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	/////////////////////////////////////////////////////////////////
public:
	friend class COuterDevPort;
private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedWMBit1();
	afx_msg void OnBnClickedWMBit2();
	afx_msg void OnBnClickedWMBit3();
	afx_msg void OnBnClickedWMBit4();
	afx_msg void OnBnClickedCheckWm5();
	afx_msg void OnBnClickedCheckWm6();
	afx_msg void OnBnClickedCheckWm7();
	afx_msg void OnBnClickedCheckWm8();
	afx_msg void OnBnClickedCheckWm9();
	afx_msg void OnBnClickedCheckWm10();
	afx_msg void OnBnClickedCheckWm11();
	afx_msg void OnBnClickedCheckWm12();
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnBnClickedButtonGetFactory();
	afx_msg void OnBnClickedButtonAdapt();

public:
	// ����λ
	CComboBox mDataBit;
	// ֹͣλ
	CComboBox mStop;
	CComboBox mBaud;//	9600;115200
	CComboBox mParity;//NONE;ODD;EVEN;MARK;SPACE;
private:
	ValidEdit mDevId;
	ValidEdit mDevName;
	ValidEdit mWayMask;

	CButton btAdapt;// ����Ӧ����ͨѶ����,�����ʣ�У�飬����λ��ֹͣλ

	CComboBox mDevSubType;
	CEdit mDevType;

	// �豸��������
	ValidEdit mDevPos;
	// Ӳ���汾��
	ValidEdit mHardVersion;
	// ����汾��
	ValidEdit mSoftVersion;

	CButton mWayBit1;
	CButton mWayBit2;
	CButton mWayBit3;
	CButton mWayBit4;
	CButton mWayBit5;
	CButton mWayBit6;
	CButton mWayBit7;
	CButton mWayBit8;
	CButton mWayBit9;
	CButton mWayBit10;
	CButton mWayBit11;
	CButton mWayBit12;

	ValidEdit mWayPos1;
	ValidEdit mWayPos2;
	ValidEdit mWayPos3;
	ValidEdit mWayPos4;
	ValidEdit mWayPos5;
	ValidEdit mWayPos6;
	ValidEdit mWayPos7;
	ValidEdit mWayPos8;
	ValidEdit mWayPos9;
	ValidEdit mWayPos10;
	ValidEdit mWayPos11;
	ValidEdit mWayPos12;

	ValidEdit mWayName1;
	ValidEdit mWayName2;
	ValidEdit mWayName3;
	ValidEdit mWayName4;
	ValidEdit mWayName5;
	ValidEdit mWayName6;
	ValidEdit mWayName7;
	ValidEdit mWayName8;
	ValidEdit mWayName9;
	ValidEdit mWayName10;
	ValidEdit mWayName11;
	ValidEdit mWayName12;

	CComboBox mWayDevType1;
	CComboBox mWayDevType2;
	CComboBox mWayDevType3;
	CComboBox mWayDevType4;
	CComboBox mWayDevType5;
	CComboBox mWayDevType6;
	CComboBox mWayDevType7;
	CComboBox mWayDevType8;
	CComboBox mWayDevType9;
	CComboBox mWayDevType10;
	CComboBox mWayDevType11;
	CComboBox mWayDevType12;

	ValidEdit mGdNodeId1;
	ValidEdit mGdNodeId2;
	ValidEdit mGdNodeId3;
	ValidEdit mGdNodeId4;
	ValidEdit mGdNodeId5;
	ValidEdit mGdNodeId6;
	ValidEdit mGdNodeId7;
	ValidEdit mGdNodeId8;
	ValidEdit mGdNodeId9;
	ValidEdit mGdNodeId10;
	ValidEdit mGdNodeId11;
	ValidEdit mGdNodeId12;

	ValidEdit mGdClassId1;
	ValidEdit mGdClassId2;
	ValidEdit mGdClassId3;
	ValidEdit mGdClassId4;
	ValidEdit mGdClassId5;
	ValidEdit mGdClassId6;
	ValidEdit mGdClassId7;
	ValidEdit mGdClassId8;
	ValidEdit mGdClassId9;
	ValidEdit mGdClassId10;
	ValidEdit mGdClassId11;
	ValidEdit mGdClassId12;

	ValidEdit mGdItemId1;
	ValidEdit mGdItemId2;
	ValidEdit mGdItemId3;
	ValidEdit mGdItemId4;
	ValidEdit mGdItemId5;
	ValidEdit mGdItemId6;
	ValidEdit mGdItemId7;
	ValidEdit mGdItemId8;
	ValidEdit mGdItemId9;
	ValidEdit mGdItemId10;
	ValidEdit mGdItemId11;
	ValidEdit mGdItemId12;

	ValidEdit mWayDevAddr1;
	ValidEdit mWayDevAddr2;
	ValidEdit mWayDevAddr3;
	ValidEdit mWayDevAddr4;
	ValidEdit mWayDevAddr5;
	ValidEdit mWayDevAddr6;
	ValidEdit mWayDevAddr7;
	ValidEdit mWayDevAddr8;
	ValidEdit mWayDevAddr9;
	ValidEdit mWayDevAddr10;
	ValidEdit mWayDevAddr11;
	ValidEdit mWayDevAddr12;

	ValidEdit mWayDetail1;
	ValidEdit mWayDetail2;
	ValidEdit mWayDetail3;
	ValidEdit mWayDetail4;
	ValidEdit mWayDetail5;
	ValidEdit mWayDetail6;
	ValidEdit mWayDetail7;
	ValidEdit mWayDetail8;
	ValidEdit mWayDetail9;
	ValidEdit mWayDetail10;
	ValidEdit mWayDetail11;
	ValidEdit mWayDetail12;

private:
	int wayCount;//��·����
	int wayMask;//��·����
	CButton* pWayBit[MAX_WAY_NUM];//�����·���
	ValidEdit* pWayPos[MAX_WAY_NUM]; //��·���
	ValidEdit* pWayName[MAX_WAY_NUM];//��·����
	CComboBox* pWayDevType[MAX_WAY_NUM];//��·�豸����
	ValidEdit* pGdNodeId[MAX_WAY_NUM];//��·gd_node_id
	ValidEdit* pGdClassId[MAX_WAY_NUM];//��·gd_class_id
	ValidEdit* pGdItemId[MAX_WAY_NUM];//��·gd_item_id
	ValidEdit* pWayDevAddr[MAX_WAY_NUM];//��·�豸��ַ
	ValidEdit* pWayDetail[MAX_WAY_NUM];//��·��ϸ˵��

private:
	void InitDevSubTypeName();//����pOuterDev->typeֵ�ҵ����������±�
	int SetDevSubTypeValue();
	void InitDataBitName();//
	int SetDataBitValue();
	void InitStopName();//
	int SetStopValue();
	void InitBaudName();//����pOuterDev->baudRateֵ�ҵ����������±�
	int SetBaudValue();
	void InitParityName();//����pOuterDev->parityֵ�ҵ����������±�
	int SetParityValue();
	void ShowWayControlByWayCount();//���ݻ�·����,��ʾ���õĻ�·״̬�ؼ�,�ؼ�ֵΪ��
	void InitWayControl();//���ݻ�·���������룬��ʼ����·�ؼ���ֵ��״̬
	void EnableWayControlByMaskChanged();//��·ѡ��״̬�ı�ʱ��������ֵ���ؼ�ʹ��Ȩ��
	MU_INT64 GetDevAddr();//��ȡ�����豸��ַ

	void ControlLayout();//��·�ؼ�����
private:
	int SetWayInf();//�����·��Ϣ
	int GetDevFactoryInf(int port_ind, MU_INT64 dev_id,  e_dev_sub_type sub_type);//��ȡ�豸������Ϣ

public:
	//�豸��Ϣ�����ж˿ڵ�id subtype���ϣ�
	void SetDevInf(st_dev_inf* dev_inf, 
		VEC_DEVID_SUBTYPE& vec_id_subtype,
		VEC_CSTRING& vec_way_name, 
		VEC_INT vec_way_id,
		COuterDevPort* p_parent);

	int Adapt(st_rs485& rs485, CDevAdaptSendFrame* send_frame);

	CTaskParam* GetDataFrame(){return dataFrame;};
	CProgress* GetProgress(){return pProgress;};
	COuterDevPort* GetParent()
	{
		return pParent;
	};
	void CloseProgress();//�رս�����

	CDevAdaptRs485Cont* GetCDevAdaptRs485Cont(){return devRs485Cont;};
private:
	VEC_DEVID_SUBTYPE vecIdSubType;//�˶˿������豸id subtype����
	VEC_CSTRING vecWayName;//���л�·���Ƽ���
	VEC_INT vecWayId;//���л�·��ţ�Ψһ
	st_dev_inf* pDevInf;//�豸��Ϣ 
	CDevAdaptRs485Cont* devRs485Cont;//485ͨѶ����

	CTaskParam* dataFrame;
	CProgress* pProgress;
	COuterDevPort* pParent;//

	///////////////////////////////////////////////////////////////////////////////////add

};
