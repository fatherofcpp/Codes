#pragma once
#include "AddDev.h"
#include "xmlAnalyze.h"
#include "Transfer.h"
#include "CheckWay.h"
#include "DataTypeDef.h"
#include "DataTypeClass.h"
#include "Progress.h"

// COuterDevPort 对话框

class COuterDevPort : public CDialog
{
	DECLARE_DYNAMIC(COuterDevPort)

public:
	COuterDevPort(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COuterDevPort();

// 对话框数据
	enum { IDD = IDD_DIALOG_PORTA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	void DisplaySaveButton();//显示保存按钮
	void ShowDelModCheckButton(bool show);//显示修改，删除，实时数据的按钮
public:
	//对外接口
	void InitDlg();//数据->对话框
	void SaveDlg();//对话框->数据
	void CreateList();//创建列表
	void SetPortDev(st_port_dev* pportDev);//设置端口信息 
	void SetPortInd(int ind);//设置端口号
	void SetParentPointer(COuterDev* parent);//设置父类指针
	COuterDev* GetParentPointer(){return pParent;};
public:
	//文件内部访问
	CTaskParam* dataFrame;
	CProgress* pProgress;
public:
	//供文件内部访问
	int GetRealData(int port_ind, MU_INT64 dev_id,e_dev_sub_type sub_type, int way_ind,u_w_measure& w_measure);
	int GetPortInd();
	void CloseProgress();//关闭进度条

private:
	void GetWayInfByIdSubType(st_devid_subtype& id_subtype,st_way_inf& way_inf);//通过id subtype获取回路信息
	void SetWayInfByIdSubType(st_devid_subtype& id_subtype,st_way_inf& way_inf);//通过id subtype保存回路信息
	void InitWayInfWithIdSubType();//保存本接口中所有设备的id subtype与回路信息到容器中
	void GetAllPortDevIdSubTypeExceptParam(VEC_DEVID_SUBTYPE& vec_id_subtype,st_devid_subtype& id_subtype);//统计所有端口除了参数id_subtype之外的其它所有设备id subtype集合
	void GetAllPortWayNameExceptParam(VEC_CSTRING& vec_way_name,st_way_inf way_inf);//统计除了参数way_inf之外的所有接口设备的回路名称集合
	void GetAllPortWayIdExceptParam(VEC_INT& vec_way_id,st_way_inf way_inf);//统计除了参数way_inf之外的所有接口设备的回路id集合
	int  GetListInf(int row_ind, st_dev_inf& dev_inf);//从list相应行提取设备信息
	void SetListInf(int row_ind, st_dev_inf& dev_inf);//根据设备信息设置list相应行信息
	int AddThreadTsk(e_thread_type type,st_dev_inf* dev_inf,VEC_DEV_ALL_WAY_DATA_INF* way_check_inf);//线程任务
private:
	int portInd;//0-3端口号
	COuterDev* pParent;//父类指针
	CAddDev* pAddDev;//子类指针设备回路对话框
	CShowDevAllWayData* pCheckWay;//子类指针实时数据对话框
	CCriticalSection taskLock;//线程任务锁

	st_port_dev* pPortDev;//端口信息
	VEC_WAY_NAME_ID_SUBTYPE vecWayInfWithIdSubType;//所有回路信息与id subtype映射关系
	//////////////////////////////////add
};
