/*
概念：允许一个对象在其内部状态改变时改变它的行为。对象看起来似乎修改了它的类。
main()，客户
CLiftState，电梯状态抽象类
CCloseingState，电梯门关闭
COpenningState，电梯门打开
CRunningState，电梯运行
CStoppingState，电梯停止
CContext，电梯的控制面板
说明：CContext保持电梯的状态，并提供操作的接口函数。当函数被调用时，CContext直接调用当前状态的相应函数。由状态的接口函数来确定是否可以执行这个动作，以及修改状态为执行这个动作后的状态。
看代码：第一块是不使用模式的做法，第二块是使用模式的做法，在main()函数里会有调用的方式。
*/

#include "stdafx.h"
#include "ILift.h"
#include "Lift.h"
#include "Context.h"
#include "OpenningState.h"
#include "CloseingState.h"
#include "RunningState.h"
#include "StoppingState.h"
#include <iostream>
using std::cout;
using std::endl;

void DoIt()
{
	//ILift.h, Lift.h, Lift.cpp
	ILift *pLift = new CLift();
	pLift->SetState(ILift::STOPPING_STATE);//电梯的初始条件是停止状态。
	pLift->Open();//首先是电梯门开启，人进去
	pLift->Close();//然后电梯门关闭
	pLift->Run();//再然后，电梯跑起来，向上或者向下
	pLift->Stop();//最后到达目的地，电梯停下来
	delete pLift;
}


void DoNew()
{
	//LiftState.h, LiftState.cpp, OpenningState.h, CloseingState.h, RunningState.h, StoppingState.h
	//Context.h, Context.cpp
	CContext context;
	CCloseingState closeingState;
	context.SetLiftState(&closeingState);
	context.Close();
	context.Open();
	context.Run();
	context.Stop();
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "----------使用模式之前----------" << endl;
	DoIt();
	cout << "----------使用模式之后----------" << endl;
	DoNew();

	return 0;
}