/*
	�������ļ�
*/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Noncopyable.h"
using namespace std;


class Ccfg: 
	public noncopyable
{
protected:
	
	//static Ccfg *m_Instance;
	static shared_ptr<Ccfg> m_Instance;
public:
	Ccfg(void);
	virtual ~Ccfg(void);
	static shared_ptr<Ccfg> GetInstance();
	int Load(string filepath);
	int Save();
	bool CreateCfg(string filepath);
		
	//����
	// ��ȡϵͳ����ģʽ
	string GetDevID(){return deviceID;} //ADD 2017/02/16
	int GetRunningMode() {return running_model;}
	int GetMemeoryMode() {return memory_model;}	//ADD 2017/04/11
	long GetTrainRoutine() {return trainRoutine;}
	string  GetTrainLW()		{return trainlw;}

	string GetLDS1Com() {return lds1com;}
	int GetLDS1BD() {return lds1bd;}

	string GetLDS2Com() {return lds2com;}
	int GetLDS2BD() {return lds2bd;}

	string GetDpjCmdCom() {return dpjcmdcom;}
	int GetDpjCmdBD() {return dpjcmdbd;}

	string GetDpjDataCom() {return dpjdatacom;}
	int GetDpjDataBD() {return dpjdatabd;}

	int GetHandpadPort() {return handpad_port;}
	string GetServerIP() {return serverip;}
	int GetServerPort()	 {return serverport;}
	int GetServerDisPort() {return serverdisport;}

	string GetLinecameraIP() {return linecameraip;}
	int GetLinecameraPort()  {return linecameraport;}

	string GetAreacameraIP() {return areacameraip;}
	int GetAreacameraPort()  {return areacameraport;}

	string GetPlcIP()        {return plcip;}
	int GetPlcPort()		 {return plcport;}	
/*
	�������������������� 2017-12-07
*/
	
	int GetTrainLW1pci()	{return trainlw1pci;}
	int GetTrainLW2pci()	{return trainlw2pci;}
	int GetTrHeadSearchWid(){return trainheadsearchwidth;}
/*
	�������������������� 2017-12-07
*/
	string Getxike1ip()		{return xike1ip;}
	int    Getxike1pport()  {return xike1port;}
	string Getctrl1ip()		{return ctr1ip;}
	int	   Getctrl1port()	{return ctr1port;}

	string Getxike2ip()		{return xike2ip;}
	int    Getxike2pport()  {return xike2port;}
	string Getctrl2ip()		{return ctr2ip;}
	int	   Getctrl2port()	{return ctr2port;}

	string GetDemoAction() {return demoaction;}
protected:	
	string deviceID;			//�豸����ID
	int running_model;			//ϵͳ����ģʽ��0 �ֶ��� 1 �Զ�
	int memory_model;			//�ᶨλ����ģʽ
	long trainRoutine;			//С����ʻ���
	string trainlw;				//������λ

	string lds1com;				//LDS1 COM
	int lds1bd;					//LDS1 BD

	string lds2com;				//LDS2 COM
	int lds2bd;					//LDS2 BD

	string dpjcmdcom;			//��Ƭ�������
	int dpjcmdbd;				//��Ƭ������BD

	string dpjdatacom;			//��Ƭ�����ݿ�
	int dpjdatabd;				//��Ƭ������BD

	int handpad_port;			//�ֳֻ��㲥�˿� 
	
	string serverip;			//������IP
	int serverport;				//�������˿�
	int serverdisport;			//���������������ݶ˿�

	string linecameraip;		//����IP
	int linecameraport;			//����˿�

	string areacameraip;		//����IP
	int areacameraport;			//����˿�
	
	string plcip;				//PLC IP
	int plcport;				//PLC�˿�
	/*
		�������������������� 2017-12-07
	*/
	
	int trainlw1pci;			//����1��λPCI
	int trainlw2pci;			//����2��λPCI
	int trainheadsearchwidth;	//������ͷ����������
	/*
		�������������������� 2017-12-07
	*/
	string xike1ip;				//����1
	int xike1port;				//
	string ctr1ip;				//
	int ctr1port;				//

	string xike2ip;				//
	int xike2port;				//	
	string ctr2ip;				//
	int ctr2port;				//

	string demoaction;			//��ʾ�õĶ����ͺ�
};

