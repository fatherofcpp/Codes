#include "StdAfx.h"
#include "Cfg.h"
#include <stdio.h>


//Ccfg* Ccfg::m_Instance = NULL;
shared_ptr<Ccfg> Ccfg::m_Instance = NULL;
Ccfg::Ccfg(void)
{
	demoaction = "";
}


Ccfg::~Ccfg(void)
{
	TRACE("destruct Ccfg class ...\r\n");
}

bool Ccfg::CreateCfg(string filepath)
{

	
	return false;
}

int Ccfg::Save()
{
	//保存

	
	return 0;
}

int Ccfg::Load(string filepath)
{		
	char buff[MAX_PATH] = {0};
	GetPrivateProfileString(_T("BASE"),_T("devID"),"HongQiao_C_J8",buff,MAX_PATH,filepath.c_str());
	deviceID = buff;
	running_model = GetPrivateProfileInt("BASE","running_model",0, filepath.c_str());
	memory_model = GetPrivateProfileInt("BASE","memory_model",0, filepath.c_str());
	trainRoutine = GetPrivateProfileInt("BASE","train_routine",0, filepath.c_str());
	//trainlw = GetPrivateProfileInt("BASE","train_lw",1, filepath.c_str());
	GetPrivateProfileString(_T("BASE"),"train_lw","1",buff,MAX_PATH,filepath.c_str());
	trainlw = buff;

	GetPrivateProfileString(_T("BASE"),"lds1_com","com1",buff,MAX_PATH,filepath.c_str());
	lds1com = buff;
	lds1bd = GetPrivateProfileInt("BASE","lds1_bd",0, filepath.c_str());

	GetPrivateProfileString(_T("BASE"),"lds2_com","com1",buff,MAX_PATH,filepath.c_str());
	lds2com = buff;
	lds2bd = GetPrivateProfileInt("BASE","lds2_bd",0, filepath.c_str());

	GetPrivateProfileString(_T("BASE"),"dpjcmd_com","com1",buff,MAX_PATH,filepath.c_str());
	dpjcmdcom = buff;
	dpjcmdbd = GetPrivateProfileInt("BASE","dpjcmd_bd",0, filepath.c_str());

	GetPrivateProfileString(_T("BASE"),"dpjdata_com","com1",buff,MAX_PATH,filepath.c_str());
	dpjdatacom = buff;
	dpjdatabd = GetPrivateProfileInt("BASE","dpjdata_bd",0, filepath.c_str());

	handpad_port = GetPrivateProfileInt("BASE","handpad_port",0, filepath.c_str());
	GetPrivateProfileString(_T("BASE"),"server_ip","127.0.0.1",buff,MAX_PATH,filepath.c_str());
	serverip = buff;
	serverport = GetPrivateProfileInt("BASE","server_port",0, filepath.c_str());
	serverdisport = GetPrivateProfileInt("BASE","serverdis_port",0, filepath.c_str());

	GetPrivateProfileString(_T("BASE"),"linecamera_ip","127.0.0.1",buff,MAX_PATH,filepath.c_str());
	linecameraip = buff;
	linecameraport = GetPrivateProfileInt("BASE","linecamera_port",0, filepath.c_str());

	GetPrivateProfileString(_T("BASE"),"areacamera_ip","127.0.0.1",buff,MAX_PATH,filepath.c_str());
	areacameraip = buff;
	areacameraport = GetPrivateProfileInt("BASE","areacamera_port",0, filepath.c_str());

	GetPrivateProfileString(_T("BASE"),"plc_ip","127.0.0.1",buff,MAX_PATH,filepath.c_str());
	plcip = buff;
	plcport = GetPrivateProfileInt("BASE","plc_port",0, filepath.c_str());
/*
	库检二代机器人新增配置 2017-12-07
*/
	
	trainlw1pci = GetPrivateProfileInt("BASE","train_lw1_pci",0, filepath.c_str());
	trainlw2pci = GetPrivateProfileInt("BASE","train_lw2_pci",0, filepath.c_str());
	trainheadsearchwidth = GetPrivateProfileInt("BASE","train_search_width",0, filepath.c_str());
/*
	库检二代机器人新增配置 2017-12-09
*/
	GetPrivateProfileString(_T("BASE"),"xike1_ip","127.0.0.1",buff,MAX_PATH,filepath.c_str());
	xike1ip = buff;
	xike1port = GetPrivateProfileInt("BASE","xike1_port",0, filepath.c_str());
	GetPrivateProfileString(_T("BASE"),"xike2_ip","127.0.0.1",buff,MAX_PATH,filepath.c_str());
	xike2ip = buff;
	xike2port = GetPrivateProfileInt("BASE","xike2_port",0, filepath.c_str());

	GetPrivateProfileString(_T("BASE"),"ctr1_ip","127.0.0.1",buff,MAX_PATH,filepath.c_str());
	ctr1ip = buff;
	ctr1port = GetPrivateProfileInt("BASE","ctr1_port",0, filepath.c_str());
	GetPrivateProfileString(_T("BASE"),"ctr2_ip","127.0.0.1",buff,MAX_PATH,filepath.c_str());
	ctr2ip = buff;
	ctr2port = GetPrivateProfileInt("BASE","ctr2_port",0, filepath.c_str());

	GetPrivateProfileString(_T("BASE"),"DemoAction","CRH380B",buff,MAX_PATH,filepath.c_str());
	demoaction = buff;
	return true;
}

shared_ptr<Ccfg> Ccfg::GetInstance()
{
	if (m_Instance == NULL)
		//m_Instance = shared_ptr<Ccfg>(new Ccfg);
		m_Instance = make_shared<Ccfg>();
	return m_Instance;
}

