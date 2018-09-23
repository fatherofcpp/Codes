#pragma once
#include "DataTypeDef.h"
#include "ValidEdit.h"
#include "ModifyTime.h"
#include "pos.h"

#include <map>
#include "afxwin.h"
using namespace std;

// CColtor 对话框
//采集器信息
class CColtor : public CDialog
{
	DECLARE_DYNAMIC(CColtor)

public:
	CColtor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CColtor();

// 对话框数据
	enum { IDD = IDD_DIALOG_COLLECTOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	ValidEdit mColtorId;//采集器编号
	ValidEdit mColtorName;//采集器名称
	CComboBox mCombColtorType;// 采集器类型

	CEdit mColtorDevTime;//采集器时间
	ValidEdit mSamplePeriod;//采样周期
	ValidEdit mHeartPeriod;//心跳周期

	CComboBox mProvinceCombox;//省,建筑区域编号
	CComboBox mCityCombox;//市
	CComboBox mCountyCombox;//县
	ValidEdit mBuildAreaName;//建筑区域名称
	ValidEdit mBuildCode;//建筑编号
	ValidEdit mBuildName;//建筑名称
	ValidEdit editBuildType;// 建筑类型


	ValidEdit mColtorAreaName;//采集器区域名称
	ValidEdit mReportPeriod;//上报周期
	CEdit mColtorAge;//采集器年龄
	CEdit mColtorSoftVersion;//采集器软件版本号
	CEdit mColtorHardVersion;//采集器硬件版本号
	CButton mUpdataTimeByHands;//手动校时
	CButton mAutoUpdataTime;//自动校时

private:
	void DisplaySaveButton();//显示保存按钮
	void InitPositon();//省市县下拉框赋值
	void SetProvinceCombox();//省下拉框赋值
	void SetCityCombox(int province_ind);//市下拉框赋值
	void SetCountyCombox(int province_ind,int city_ind);//县下拉框赋值
	int GetComboxIndById(int id, map<int, int>& map_pos);//查找参数2中参数1对应的值
	int GetIdByComboxInd(int ind, map<int, int>& map_pos);//查找参数2中参数1对应的值
	void ShowInput(bool show);//显示
private:
	map<int, int> mapProvince;//id,ComboxInd
	map<int, int> mapCity;
	map<int, int> mapCounty;

	CModifyTime* pMoifyTime;//时间修改
	CPosition* pPosition;//省市县
	st_coltor_inf* pColtorInf;//采集器信息 

public:
	void UpdataDevTime();//更新时间
	void SetColtorInf(st_coltor_inf* coltor_inf);//设置数据
	void InitDlg();//数据->对话框
	int SaveDlg();//对话框->数据

};
