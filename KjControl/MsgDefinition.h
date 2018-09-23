/*
	APP对控制程序的命令自定义消息定义

	{start,库&股道&车型&车号&端位}
	{estop,库&股道}
	{home,库&股道}
	{go,库&股道&x}
	{back,库&股道&x}
*/

#pragma once
#include "stdafx.h"		

#define	APP_CMD_START	WM_USER + 101	//开始
#define	APP_CMD_ESTOP	WM_USER + 102	//紧急停止
#define APP_CMD_HOME	WM_USER + 103	//回到原点
#define APP_CMD_GO		WM_USER + 104	//前进
#define APP_CMD_BACK	WM_USER + 105	//后退

#define EDITBOX_MSG		WM_USER + 106	//发给UI EDITBOX的消息