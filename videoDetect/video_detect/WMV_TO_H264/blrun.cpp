// blrun.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream> 
#include <windows.h> 
#include <stdio.h> 
#include <tchar.h> 
#include <time.h>

using namespace std;

void coutTime()
{

	time_t rawtime; 
	struct tm * timeinfo; 

	time ( &rawtime ); 
	timeinfo = localtime ( &rawtime ); 
	cout<<endl<<asctime (timeinfo)<<endl; 
}

int _tmain(int argc, _TCHAR* argv[])
{
	STARTUPINFO si; 

	PROCESS_INFORMATION pi; //������Ϣ�� 

	coutTime();
	cout<< "argc is "<< argc<<endl;

	if(argc!=2)
	{
		cout<<"eg. >blrun.exe ntcs.ext"<<endl;
		return -1;
	}
	cout<<"blrun "<<argv[1]<<endl;



	ZeroMemory(&si, sizeof(si)); 
	si.cb = sizeof(si); 
	ZeroMemory(&pi, sizeof(pi)); 
	do{ 
		// �����ӽ��̣��ж��Ƿ�ִ�гɹ� 
		if(!CreateProcess( NULL,argv[1],NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)) 
		{ 
			coutTime();
			cout << "��������ʧ��.." << GetLastError() << endl; 
			system("pause"); //���ڲ��� 
			return 0; 
		} 
		//����ִ�гɹ�����ӡ������Ϣ 
		cout << "�������ӽ��̵���Ϣ��" << endl; 
		cout << "����ID pi.dwProcessID: " << pi.dwProcessId << endl; 
		cout << "�߳�ID pi.dwThreadID : " << pi.dwThreadId << endl; 
		// �ȴ�֪���ӽ����˳�... 
		WaitForSingleObject( pi.hProcess, INFINITE);//�������Ƿ�ֹͣ 
		//WaitForSingleObject()�����������״̬�������δȷ������ȴ�����ʱ 
		//�ӽ����˳� 
		coutTime();
		cout << "�ӽ����Ѿ��˳�..." << endl; 
		//�رս��̺;�� 
		CloseHandle(pi.hProcess); 
		CloseHandle(pi.hThread); 
		//system("pause");//ִ����Ϻ�ȴ� 
	}while(true);//��������Ƴ����ٴ�ִ�з��� 
	exit(0); 
	return 0;
}

