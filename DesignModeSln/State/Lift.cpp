#include "Lift.h"
#include <iostream>
using std::cout;
using std::endl;

CLift::CLift(void)
{
	this->m_state = 0;
}

CLift::~CLift(void)
{
}

void CLift::SetState(int state)
{
	this->m_state = state;
}

void CLift::Open()
{
	switch(this->m_state)
	{
	case OPENING_STATE:
		break;
	case CLOSING_STATE:
		this->OpenWithoutLogic();
		this->SetState(OPENING_STATE);
		break;
	case RUNNING_STATE:
		break;
	case STOPPING_STATE:
		this->OpenWithoutLogic();
		this->SetState(OPENING_STATE);
		break;
	}
}

void CLift::Close()
{
	switch(this->m_state)
	{
	case OPENING_STATE:
		this->CloseWithoutLogic();
		this->SetState(CLOSING_STATE);
		break;
	case CLOSING_STATE:
		break;
	case RUNNING_STATE:
		break;
	case STOPPING_STATE:
		break;
	}
}

void CLift::Run()
{
	switch(this->m_state)
	{
	case OPENING_STATE:
		break;
	case CLOSING_STATE:
		this->RunWithoutLogic();
		this->SetState(RUNNING_STATE);
		break;
	case RUNNING_STATE:
		break;
	case STOPPING_STATE:
		this->RunWithoutLogic();
		this->SetState(RUNNING_STATE);
		break;
	}
}

void CLift::Stop()
{
	switch(this->m_state)
	{
	case OPENING_STATE:
		break;
	case CLOSING_STATE:
		this->StopWithoutLogic();
		this->SetState(CLOSING_STATE);
		break;
	case RUNNING_STATE:
		this->StopWithoutLogic();
		this->SetState(CLOSING_STATE);
		break;
	case STOPPING_STATE:
		break;
	}
}

void CLift::OpenWithoutLogic()
{
	cout << "�����ſ���..." << endl;
}

void CLift::CloseWithoutLogic()
{
	cout << "�����Źر�..." << endl;
}

void CLift::RunWithoutLogic()
{
	cout << "��������������..." << endl;
}

void CLift::StopWithoutLogic()
{
	cout << "����ֹͣ��..." << endl;
}

