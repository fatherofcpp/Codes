#pragma once
#include "DataTypeDef.h"
#include "ValidEdit.h"
#include "ModifyTime.h"
#include "pos.h"

#include <map>
#include "afxwin.h"
using namespace std;

// CColtor �Ի���
//�ɼ�����Ϣ
class CColtor : public CDialog
{
	DECLARE_DYNAMIC(CColtor)

public:
	CColtor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CColtor();

// �Ի�������
	enum { IDD = IDD_DIALOG_COLLECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	////////////////////////////////////////////////////////////
public:
	friend class CDtuConfigDlgDlg;

private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	afx_msg void OnBnClickedSelfUpdataTime();
	afx_msg void OnEnChangeID();
	afx_msg void OnEnChangeSample();
	afx_msg void OnEnChangeName();
	afx_msg void OnBnClickedButtonAutoUpdata();
	afx_msg void OnEnChangeHeart();
	afx_msg void OnCbnSelchangeComboProvince();
	afx_msg void OnCbnSelchangeComboCity();
	afx_msg void OnEnChangeEditJzname();
	afx_msg void OnCbnSelchangeComboCounty();
	afx_msg void OnEnChangeEditJzcode();
	afx_msg void OnEnChangeEditBuildName();
	afx_msg void OnEnChangeEditCollectorCode();
	afx_msg void OnEnChangeEditReport();
private:
	ValidEdit mColtorId;//�ɼ������
	ValidEdit mColtorName;//�ɼ�������
	CComboBox mCombColtorType;// �ɼ�������

	CEdit mColtorDevTime;//�ɼ���ʱ��
	ValidEdit mSamplePeriod;//��������
	ValidEdit mHeartPeriod;//��������

	CComboBox mProvinceCombox;//ʡ,����������
	CComboBox mCityCombox;//��
	CComboBox mCountyCombox;//��
	ValidEdit mBuildAreaName;//������������
	ValidEdit mBuildCode;//�������
	ValidEdit mBuildName;//��������
	ValidEdit editBuildType;// ��������


	ValidEdit mColtorAreaName;//�ɼ�����������
	ValidEdit mReportPeriod;//�ϱ�����
	CEdit mColtorAge;//�ɼ�������
	CEdit mColtorSoftVersion;//�ɼ�������汾��
	CEdit mColtorHardVersion;//�ɼ���Ӳ���汾��
	CButton mUpdataTimeByHands;//�ֶ�Уʱ
	CButton mAutoUpdataTime;//�Զ�Уʱ

private:
	void DisplaySaveButton();//��ʾ���水ť
	void InitPositon();//ʡ����������ֵ
	void SetProvinceCombox();//ʡ������ֵ
	void SetCityCombox(int province_ind);//��������ֵ
	void SetCountyCombox(int province_ind,int city_ind);//��������ֵ
	int GetComboxIndById(int id, map<int, int>& map_pos);//���Ҳ���2�в���1��Ӧ��ֵ
	int GetIdByComboxInd(int ind, map<int, int>& map_pos);//���Ҳ���2�в���1��Ӧ��ֵ
	void ShowInput(bool show);//��ʾ
private:
	map<int, int> mapProvince;//id,ComboxInd
	map<int, int> mapCity;
	map<int, int> mapCounty;

	CModifyTime* pMoifyTime;//ʱ���޸�
	CPosition* pPosition;//ʡ����
	st_coltor_inf* pColtorInf;//�ɼ�����Ϣ 

public:
	void UpdataDevTime();//����ʱ��
	void SetColtorInf(st_coltor_inf* coltor_inf);//��������
	void InitDlg();//����->�Ի���
	int SaveDlg();//�Ի���->����

};
