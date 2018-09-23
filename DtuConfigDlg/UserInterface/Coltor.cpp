// Coltor.cpp : 实现文件
//

#include "stdafx.h"
#include "DtuConfigDlg.h"
#include "Coltor.h"
#include "dataConvert.h"
#include "CommonFunction.h"

#define P_PROVINCE pPosition->pNation->province
// CColtor 对话框

IMPLEMENT_DYNAMIC(CColtor, CDialog)

	CColtor::CColtor(CWnd* pParent /*=NULL*/)
	: CDialog(CColtor::IDD, pParent)
{
	pColtorInf = new st_coltor_inf;
	pMoifyTime = new CModifyTime;
	pPosition = new CPosition;
	pPosition->parser_db();

	memset(pColtorInf, 0, sizeof(*pColtorInf));
	mColtorId.SetValidType(VALID_INT);
	mColtorId.SetMaxLen(8);
	mColtorId.SetMaxNum(99999998);
	mColtorId.SetMinNum(1);
	mColtorName.SetMaxLen(MAX_NAME_LEN);

	mBuildAreaName.SetMaxLen(MAX_NAME_LEN);
	mBuildName.SetMaxLen(MAX_NAME_LEN);
	mBuildCode.SetValidType(VALID_INT);
	mBuildCode.SetMaxLen(MAX_JZCODE_NUM);
	mBuildCode.SetMaxNum(99999);
	mBuildCode.SetMinNum(10000);
	editBuildType.SetValidType(VALID_INT);
	editBuildType.SetMaxLen(8);
	editBuildType.SetMaxNum(99999998);
	editBuildType.SetMinNum(1);

	mSamplePeriod.SetValidType(VALID_INT);
	mSamplePeriod.SetMaxNum(24*60);
	mSamplePeriod.SetMinNum(1);

	mHeartPeriod.SetValidType(VALID_INT);
	mHeartPeriod.SetMaxNum(60);
	mHeartPeriod.SetMinNum(15);

	mColtorAreaName.SetMaxLen(MAX_NAME_LEN);

	mReportPeriod.SetValidType(VALID_INT);
	mReportPeriod.SetMaxNum(24*60);
	mReportPeriod.SetMinNum(1);
}

CColtor::~CColtor()
{
	delete pColtorInf;
	delete pMoifyTime;
	delete pPosition;
}

void CColtor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, mColtorId);
	DDX_Control(pDX, IDC_EDIT_NAME, mColtorName);
	DDX_Control(pDX, IDC_EDIT_DEVTIME, mColtorDevTime);
	DDX_Control(pDX, IDC_EDIT_SAMPLE, mSamplePeriod);
	DDX_Control(pDX, IDC_BUTTON_SELF_UPDATA, mUpdataTimeByHands);
	DDX_Control(pDX, IDC_EDIT_SAMPLE, mSamplePeriod);
	DDX_Control(pDX, IDC_EDIT_HEART, mHeartPeriod);
	DDX_Control(pDX, IDC_COMBO_PROVINCE, mProvinceCombox);
	DDX_Control(pDX, IDC_COMBO_CITY, mCityCombox);
	DDX_Control(pDX, IDC_COMBO_COUNTY, mCountyCombox);
	DDX_Control(pDX, IDC_EDIT_JZNAME, mBuildAreaName);
	DDX_Control(pDX, IDC_EDIT_JZCODE, mBuildCode);
	DDX_Control(pDX, IDC_BUTTON_AUTO_UPDATA, mAutoUpdataTime);
	DDX_Control(pDX, IDC_EDIT_BUILD_NAME, mBuildName);
	DDX_Control(pDX, IDC_EDIT_COLLECTOR_CODE, mColtorAreaName);
	DDX_Control(pDX, IDC_EDIT_REPORT, mReportPeriod);
	DDX_Control(pDX, IDC_EDIT8, mColtorAge);
	DDX_Control(pDX, IDC_EDIT9, mColtorSoftVersion);
	DDX_Control(pDX, IDC_EDIT10, mColtorHardVersion);
	DDX_Control(pDX, IDC_EDIT1, editBuildType);
	DDX_Control(pDX, IDC_COMBO_COLTOR_TYPE, mCombColtorType);
}


BEGIN_MESSAGE_MAP(CColtor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELF_UPDATA, &CColtor::OnBnClickedSelfUpdataTime)
	ON_EN_CHANGE(IDC_EDIT_ID, &CColtor::OnEnChangeID)
	ON_EN_CHANGE(IDC_EDIT_SAMPLE, &CColtor::OnEnChangeSample)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CColtor::OnEnChangeName)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_UPDATA, &CColtor::OnBnClickedButtonAutoUpdata)
	ON_EN_CHANGE(IDC_EDIT_HEART, &CColtor::OnEnChangeHeart)
	ON_CBN_SELCHANGE(IDC_COMBO_PROVINCE, &CColtor::OnCbnSelchangeComboProvince)
	ON_CBN_SELCHANGE(IDC_COMBO_CITY, &CColtor::OnCbnSelchangeComboCity)
	ON_EN_CHANGE(IDC_EDIT_JZNAME, &CColtor::OnEnChangeEditJzname)
	ON_CBN_SELCHANGE(IDC_COMBO_COUNTY, &CColtor::OnCbnSelchangeComboCounty)
	ON_EN_CHANGE(IDC_EDIT_JZCODE, &CColtor::OnEnChangeEditJzcode)
	ON_EN_CHANGE(IDC_EDIT_BUILD_NAME, &CColtor::OnEnChangeEditBuildName)
	ON_EN_CHANGE(IDC_EDIT_COLLECTOR_CODE, &CColtor::OnEnChangeEditCollectorCode)
	ON_EN_CHANGE(IDC_EDIT_REPORT, &CColtor::OnEnChangeEditReport)
END_MESSAGE_MAP()


// CColtor 消息处理程序
void CColtor::SetColtorInf(st_coltor_inf* coltorInf)
{
	memcpy(pColtorInf, coltorInf, sizeof(*pColtorInf));
}

void CColtor::InitDlg()
{
	CString str;
	int ret = 0;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	ShowInput(true);

	str.Format("%d",pColtorInf->col_conf.id);
	mColtorId.SetWindowText(str);

	mCombColtorType.ResetContent();
	for (int i = 0; i < gColtorTypeNum; i++)
	{
		str.Format("%s",gColtorTypeName[i]);
		mCombColtorType.InsertString(i,str);
	}
	int coltor_type_ind = GetColtorTypeIndByValue(pColtorInf->col_conf.type);
	if (coltor_type_ind <= 0)
	{
		coltor_type_ind = 0;
	}
	mCombColtorType.SetCurSel(coltor_type_ind);

	InitPositon();

	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,pColtorInf->col_conf.name, strlen(pColtorInf->col_conf.name));
	mColtorName.SetWindowText(name);
	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,pColtorInf->build_conf.build_pos.pos_name, strlen(pColtorInf->build_conf.build_pos.pos_name));
	mBuildAreaName.SetWindowText(name);
	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,pColtorInf->build_conf.build_name, strlen(pColtorInf->build_conf.build_name));
	mBuildName.SetWindowText(name);

	str.Format("%d", pColtorInf->build_conf.build_code);
	mBuildCode.SetWindowText(str);

	str.Format("%d", pColtorInf->build_conf.build_type);
	editBuildType.SetWindowText(str);

	int yy = pColtorInf->dev_time.dev_time.YY;
	int mm = pColtorInf->dev_time.dev_time.MM;
	int dd = pColtorInf->dev_time.dev_time.DD;
	int hh = pColtorInf->dev_time.dev_time.HH;
	int mi = pColtorInf->dev_time.dev_time.MI;
	int ss = pColtorInf->dev_time.dev_time.SS;
	char devtime[64];
	sprintf(devtime,"%d-%d-%d %d:%d:%d",yy,mm,dd,hh,mi,ss);
	mColtorDevTime.SetWindowText(devtime);
	mColtorDevTime.EnableWindow(false);

	str.Format("%d",pColtorInf->col_conf.sample_m);
	mSamplePeriod.SetWindowText(str);

	str.Format("%d",pColtorInf->col_conf.heart_s);
	mHeartPeriod.SetWindowText(str);

	str.Format("%d",pColtorInf->col_conf.report_m);
	mReportPeriod.SetWindowText(str);

	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,pColtorInf->col_conf.pos_name, strlen(pColtorInf->col_conf.pos_name));
	mColtorAreaName.SetWindowText(name);

	str.Format("%d",pColtorInf->col_conf.age);
	mColtorAge.SetWindowText(str);

	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,pColtorInf->col_conf.sw_ver, strlen(pColtorInf->col_conf.sw_ver));
	mColtorSoftVersion.SetWindowText(name);

	ZeroMemory(name,sizeof(name));
	ret = utf8_to_gbk(name,pColtorInf->col_conf.hw_ver, strlen(pColtorInf->col_conf.hw_ver));
	mColtorHardVersion.SetWindowText(name);

}

int CColtor::SaveDlg()
{
	//memset(&pColtorInf, 0, sizeof(strColtorInf));
	int ret = 0;
	int ind = 0;
	CString str;
	char name[MAX_NAME_LEN];
	ZeroMemory(name,sizeof(name));

	mColtorId.GetWindowText(str);
	int id = 0;
	str_to_int(str.GetString(),id,gHexType);
	if (id < 1 || id >99999998)
	{
		MessageBox("id不合法");
		return -1;
	}

	if (pColtorInf->col_conf.id != id)
	{
		pColtorInf->col_conf.id = id;
		pColtorInf->col_conf.is_modify = true;
	}

	int col_type = 0;
	int col_type_ind = mCombColtorType.GetCurSel();
	assert(col_type_ind >= 0);
	col_type = gColtorTypeValue[col_type_ind];
	if (pColtorInf->col_conf.type != col_type)
	{
		pColtorInf->col_conf.type = col_type;
		pColtorInf->col_conf.is_modify = true;
	}

	ind = mProvinceCombox.GetCurSel();
	if (ind < 0)
	{
		MessageBox("省份不合法");
		return -2;
	}
	if (pColtorInf->build_conf.build_pos.province != GetIdByComboxInd(ind,mapProvince))
	{
		pColtorInf->build_conf.build_pos.province = GetIdByComboxInd(ind,mapProvince);
		pColtorInf->build_conf.is_modify = true;
	}
	ind = mCityCombox.GetCurSel();
	if (ind < 0)
	{
		MessageBox("市不合法");
		return -3;
	}
	if(pColtorInf->build_conf.build_pos.city != GetIdByComboxInd(ind,mapCity))
	{
		pColtorInf->build_conf.build_pos.city = GetIdByComboxInd(ind,mapCity);
		pColtorInf->build_conf.is_modify = true;
	}
	ind = mCountyCombox.GetCurSel();
	if (ind < 0)
	{
		MessageBox("县不合法");
		return -3;
	}
	if(pColtorInf->build_conf.build_pos.county != GetIdByComboxInd(ind,mapCounty))
	{
		pColtorInf->build_conf.build_pos.county = GetIdByComboxInd(ind,mapCounty);
		pColtorInf->build_conf.is_modify = true;
	}

	mColtorName.GetWindowText(str);
	if (0 == str.GetLength())
	{
		MessageBox("采集器名称不合法");
		return -4;
	}
	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);
	if(0 != strcmp(pColtorInf->col_conf.name,name))
	{
		sprintf(pColtorInf->col_conf.name,name);
		pColtorInf->col_conf.is_modify = true;
	}

	mBuildAreaName.GetWindowText(str);
	if (0 == str.GetLength())
	{
		MessageBox("建筑编号不合法");
		return -5;
	}
	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);
	if(0 != strcmp(pColtorInf->build_conf.build_pos.pos_name,name))
	{
		sprintf(pColtorInf->build_conf.build_pos.pos_name,name);
		pColtorInf->build_conf.is_modify = true;
	}

	mBuildName.GetWindowText(str);
	if (0 == str.GetLength())
	{
		MessageBox("建筑名称不合法");
		return -51;
	}
	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);
	if(0 != strcmp(pColtorInf->build_conf.build_name,name))
	{
		sprintf(pColtorInf->build_conf.build_name,name);
		pColtorInf->build_conf.is_modify = true;
	}

	mBuildCode.GetWindowText(str);
	int pos_code = 0;
	str_to_int(str.GetString(), pos_code, gHexType);
	if(pos_code != pColtorInf->build_conf.build_code)
	{
		pColtorInf->build_conf.build_code = pos_code;
		pColtorInf->build_conf.is_modify = true;
	}

	editBuildType.GetWindowText(str);
	int build_type = 0;
	str_to_int(str.GetString(), build_type, gHexType);
	if(build_type != pColtorInf->build_conf.build_type)
	{
		pColtorInf->build_conf.build_type = build_type;
		pColtorInf->build_conf.is_modify = true;
	}
	;

	int yy = 0;
	int mm = 0;
	int dd = 0;
	int hh = 0;
	int mi = 0;
	int ss = 0;
	char devtime[64];
	mColtorDevTime.GetWindowText(str);
	if (0 == str.GetLength())
	{
		MessageBox("时间不合法");
		return -5;
	}
	sprintf(devtime,"%s",str.GetString());
	sscanf(devtime,"%d-%d-%d %d:%d:%d",&yy,&mm,&dd,&hh,&mi,&ss);
	pColtorInf->dev_time.dev_time.YY = yy;
	pColtorInf->dev_time.dev_time.MM = mm;
	pColtorInf->dev_time.dev_time.DD = dd;
	pColtorInf->dev_time.dev_time.HH = hh;
	pColtorInf->dev_time.dev_time.MI = mi;
	pColtorInf->dev_time.dev_time.SS = ss;

	mSamplePeriod.GetWindowText(str);
	int sample = 0;
	str_to_int(str.GetString(),sample,gHexType);
	if (sample < 1 || sample > 24*60)
	{
		MessageBox("采样周期不合法");
		return -6;
	}
	if (pColtorInf->col_conf.sample_m != sample)
	{
		pColtorInf->col_conf.sample_m = sample;
		pColtorInf->col_conf.is_modify = true;
	}

	mHeartPeriod.GetWindowText(str);
	int heart = 0;
	str_to_int(str.GetString(),heart,gHexType);
	if (heart < 15 || heart > 60)
	{
		MessageBox("心跳周期不合法");
		return -7;
	}
	if (pColtorInf->col_conf.heart_s != heart)
	{
		pColtorInf->col_conf.heart_s = heart;
		pColtorInf->col_conf.is_modify = true;
	}

	mReportPeriod.GetWindowText(str);
	int report = 0;
	str_to_int(str.GetString(),report,gHexType);
	if (report < 1 || report > 24*60)
	{
		MessageBox("上报周期不合法");
		return -71;
	}
	//增加紫衡对上报周期的限制
	if (gCompanyType == ZI_HENG_COMPANY)
	{
		if (report != 15)
		{
			MessageBox("上报周期必须是15分钟");
			return -72;
		}
	}

	if (pColtorInf->col_conf.report_m != report)
	{
		pColtorInf->col_conf.report_m = report;
		pColtorInf->col_conf.is_modify = true;
	}

	mColtorAreaName.GetWindowText(str);
	if (0 == str.GetLength())
	{
		MessageBox("采集器区域名称不合法");
		return -5;
	}
	ZeroMemory(name,sizeof(name));
	ret = gb2312_to_utf8(str.GetString(),
		str.GetLength(), name);
	if(0 != strcmp(pColtorInf->col_conf.pos_name,name))
	{
		sprintf(pColtorInf->col_conf.pos_name,name);
		pColtorInf->col_conf.is_modify = true;
	}

	return 0;
}

void CColtor::UpdataDevTime()
{
	char devtime[64];
	CString str;

	int yy = pColtorInf->dev_time.dev_time.YY;
	if (yy <= 1971)
	{
		yy = 1971;
	}

	int mm = pColtorInf->dev_time.dev_time.MM;
	int dd = pColtorInf->dev_time.dev_time.DD;
	int hh = pColtorInf->dev_time.dev_time.HH;
	int mi = pColtorInf->dev_time.dev_time.MI;
	int ss = pColtorInf->dev_time.dev_time.SS;

	st_time devTime;
	ZeroMemory(&devTime,sizeof(devTime));
	if (0 == memcmp(&devTime,&pColtorInf->dev_time.dev_time,sizeof(st_time)))
	{
		return;
	}

	DWORD tmCur = GetTickCount();
	DWORD delay = tmCur - pColtorInf->dev_time.local_time;
	delay /= 1000;
	CTime tm(yy, mm, dd, hh, mi, ss); 
	tm = tm + delay;

	sprintf(devtime,"%d-%d-%d %d:%d:%d",
		tm.GetYear(),tm.GetMonth(),tm.GetDay(),
		tm.GetHour(),tm.GetMinute(),tm.GetSecond());

	mColtorDevTime.SetWindowText(devtime);
}


void CColtor::OnBnClickedSelfUpdataTime()
{
	int ret = pMoifyTime->DoModal();
	if(IDOK == ret)
	{
		pColtorInf->dev_time.is_modify = true;
		DisplaySaveButton();
		memcpy(&pColtorInf->dev_time.dev_time, pMoifyTime->pTime, sizeof(pColtorInf->dev_time.dev_time)); 
		pColtorInf->dev_time.local_time = GetTickCount();
	}
}

void CColtor::DisplaySaveButton()
{
	bool modify = false;
	CString str;
	int id = 0;
	mColtorId.GetWindowText(str);
	str_to_int(str.GetString(),id,gHexType);
	if (pColtorInf->col_conf.id != id)
	{
		modify = true;
	}

	int col_type = 0;
	int col_type_ind = mCombColtorType.GetCurSel();
	assert(col_type_ind >= 0);
	col_type = gColtorTypeValue[col_type_ind];
	if (pColtorInf->col_conf.type != col_type)
	{
		modify = true;
	}
	

	mColtorName.GetWindowText(str);
	if (0 != strcmp(pColtorInf->col_conf.name,str.GetString()))
	{
		modify = true;
	}

	mBuildAreaName.GetWindowText(str);
	if (0 != strcmp(pColtorInf->build_conf.build_pos.pos_name,str.GetString()))
	{
		modify = true;
	}

	mBuildName.GetWindowText(str);
	if (0 != strcmp(pColtorInf->build_conf.build_name,str.GetString()))
	{
		modify = true;
	}

	mBuildCode.GetWindowText(str);
	int pos_code = 0;
	str_to_int(str.GetString(), pos_code, gHexType);
	if (pos_code != pColtorInf->build_conf.build_code)
	{
		modify = true;
	}

	editBuildType.GetWindowText(str);
	int build_type = 0;
	str_to_int(str.GetString(), build_type, gHexType);
	if (pos_code != pColtorInf->build_conf.build_type)
	{
		modify = true;
	}

	id = GetIdByComboxInd(mProvinceCombox.GetCurSel(),mapProvince);
	if (id != pColtorInf->build_conf.build_pos.province)
	{
		modify = true;
	}

	id = GetIdByComboxInd(mCityCombox.GetCurSel(),mapCity);
	if (id != pColtorInf->build_conf.build_pos.city)
	{
		modify = true;
	}

	id = GetIdByComboxInd(mCountyCombox.GetCurSel(),mapCounty);
	if (id != pColtorInf->build_conf.build_pos.county)
	{
		modify = true;
	}

	if (pColtorInf->dev_time.is_modify == true)
	{
		modify = true;
	}

	mSamplePeriod.GetWindowText(str);
	if(pColtorInf->col_conf.sample_m != atoi(str.GetString()))
	{
		modify = true;
	}

	mHeartPeriod.GetWindowText(str);
	if(pColtorInf->col_conf.heart_s != atoi(str.GetString()))
	{
		modify = true;
	}

	mReportPeriod.GetWindowText(str);
	if(pColtorInf->col_conf.report_m != atoi(str.GetString()))
	{
		modify = true;
	}

	mColtorAreaName.GetWindowText(str);
	if (0 != strcmp(pColtorInf->col_conf.pos_name,str.GetString()))
	{
		modify = true;
	}


	if (modify == true)
	{
		if (gComNum != 0)
		{
			GetParent()->GetDlgItem(IDC_BUTTON_SAVE_COM)->EnableWindow(true);
		}
	}
	else
	{

	}
}

void CColtor::OnEnChangeID()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}

void CColtor::OnEnChangeSample()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CColtor::OnEnChangeName()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}



void CColtor::OnBnClickedButtonAutoUpdata()
{
	pColtorInf->dev_time.is_modify = true;
	DisplaySaveButton();

	CTime tm; 
	tm = CTime::GetCurrentTime();//获得当前时间 

	pColtorInf->dev_time.dev_time.YY = tm.GetYear();
	pColtorInf->dev_time.dev_time.MM= tm.GetMonth();
	pColtorInf->dev_time.dev_time.DD= tm.GetDay(); 
	pColtorInf->dev_time.dev_time.HH= tm.GetHour();
	pColtorInf->dev_time.dev_time.MI= tm.GetMinute();
	pColtorInf->dev_time.dev_time.SS= tm.GetSecond();

	pColtorInf->dev_time.local_time = GetTickCount();
}


void CColtor::OnEnChangeHeart()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}

void CColtor::InitPositon()
{
	CString str;
	int ind = 0;
	int province_ind = 0;
	int city_ind = 0;

	SetProvinceCombox();
	ind = GetComboxIndById(pColtorInf->build_conf.build_pos.province,mapProvince);
	mProvinceCombox.SetCurSel(ind);
	province_ind = ind;

	if (province_ind < 0)
	{
		return;
	}

	SetCityCombox(province_ind);
	ind = GetComboxIndById(pColtorInf->build_conf.build_pos.city,mapCity);
	mCityCombox.SetCurSel(ind);
	city_ind = ind;

	if (city_ind < 0)
	{
		return;
	}
	SetCountyCombox(province_ind,city_ind);
	ind = GetComboxIndById(pColtorInf->build_conf.build_pos.county,mapCounty);
	mCountyCombox.SetCurSel(ind);
}

int CColtor::GetComboxIndById( int id, map<int, int>& map_pos )
{
	map<int, int>::iterator  iter;
	for(iter = map_pos.begin(); iter != map_pos.end(); iter++)
	{
		if (iter->first == id)
		{
			return iter->second;
		}
	}
	return -1;
}


void CColtor::OnCbnSelchangeComboProvince()
{
	int ind = 0;
	ind = mProvinceCombox.GetCurSel();
	SetCityCombox(ind);

	mCountyCombox.ResetContent();
	mapCounty.clear();

	DisplaySaveButton();
}


void CColtor::OnCbnSelchangeComboCity()
{
	int pro = 0;
	int city = 0;
	pro = mProvinceCombox.GetCurSel();

	city = mCityCombox.GetCurSel();
	SetCountyCombox(pro, city);

	DisplaySaveButton();
}

void CColtor::SetProvinceCombox()
{
	CString str;
	mProvinceCombox.ResetContent();
	mapProvince.clear();
	for (int i = 0; i < pPosition->pNation->num_province; i++)
	{
		str.Format("%s",P_PROVINCE[i].name);
		mProvinceCombox.InsertString(i,str);
		mapProvince.insert
			(pair<int, int>(atoi(P_PROVINCE[i].code_id), i));
	}
}

void CColtor::SetCityCombox(int province_ind)
{
	CString str;
	mCityCombox.ResetContent();
	mapCity.clear();

	for (int j = 0; j < P_PROVINCE[province_ind].num_city; j++)
	{
		str.Format("%s",P_PROVINCE[province_ind].city[j].name);
		mCityCombox.InsertString(j,str);
		mapCity.insert
			(pair<int, int>(atoi(P_PROVINCE[province_ind].city[j].code_id), j));
	}
}

void CColtor::SetCountyCombox(int province_ind,int city_ind)
{
	CString str;
	mCountyCombox.ResetContent();
	mapCounty.clear();

	for (int k = 0; k < P_PROVINCE[province_ind].city[city_ind].num_county; k++)
	{
		str.Format("%s",P_PROVINCE[province_ind].city[city_ind].county[k].name);
		mCountyCombox.InsertString(k,str);
		mapCounty.insert
			(pair<int, int>(atoi(P_PROVINCE[province_ind].city[city_ind].county[k].code_id), k));
	}
}

int CColtor::GetIdByComboxInd( int ind, map<int, int>& map_pos )
{
	map<int, int>::iterator  iter;
	for(iter = map_pos.begin(); iter != map_pos.end(); iter++)
	{
		if (iter->second == ind)
		{
			return iter->first;
		}
	}
	return -1;
}


void CColtor::OnEnChangeEditJzname()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}



void CColtor::OnCbnSelchangeComboCounty()
{
	DisplaySaveButton();
}


BOOL CColtor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN) 
	{  
		switch(pMsg->wParam) 
		{  
		case VK_RETURN: //回车  
			return TRUE;  
		case VK_ESCAPE: //ESC  
			return TRUE;  
		}  
	} 
	if(pMsg->message == WM_RBUTTONDOWN) 
	{  
		return TRUE;
	} 
	return CDialog::PreTranslateMessage(pMsg);
}


void CColtor::OnEnChangeEditJzcode()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


BOOL CColtor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	mColtorAge.EnableWindow(FALSE);
	mColtorSoftVersion.EnableWindow(FALSE);
	mColtorHardVersion.EnableWindow(FALSE);

	ShowInput(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CColtor::ShowInput( bool show )
{
	mColtorId.EnableWindow(show);
	mCombColtorType.EnableWindow(show);
	mColtorName.EnableWindow(show);
	mColtorDevTime.EnableWindow(show);
	mSamplePeriod.EnableWindow(show);
	mHeartPeriod.EnableWindow(show);
	mReportPeriod.EnableWindow(show);
	mBuildAreaName.EnableWindow(show);
	mBuildName.EnableWindow(show);
	mBuildCode.EnableWindow(show);
	editBuildType.EnableWindow(show);
	mColtorAreaName.EnableWindow(show);

	mUpdataTimeByHands.EnableWindow(show);
	mAutoUpdataTime.EnableWindow(show);

	mProvinceCombox.EnableWindow(show);
	mCityCombox.EnableWindow(show);
	mCountyCombox.EnableWindow(show);
}


void CColtor::OnEnChangeEditBuildName()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CColtor::OnEnChangeEditCollectorCode()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}


void CColtor::OnEnChangeEditReport()
{
	static int enter_time = 0;
	if (++enter_time > 1)
	{
		DisplaySaveButton();
	}
}
