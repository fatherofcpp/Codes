/*
	APP�Կ��Ƴ���������Զ�����Ϣ����

	{start,��&�ɵ�&����&����&��λ}
	{estop,��&�ɵ�}
	{home,��&�ɵ�}
	{go,��&�ɵ�&x}
	{back,��&�ɵ�&x}
*/

#pragma once
#include "stdafx.h"		

#define	APP_CMD_START	WM_USER + 101	//��ʼ
#define	APP_CMD_ESTOP	WM_USER + 102	//����ֹͣ
#define APP_CMD_HOME	WM_USER + 103	//�ص�ԭ��
#define APP_CMD_GO		WM_USER + 104	//ǰ��
#define APP_CMD_BACK	WM_USER + 105	//����

#define EDITBOX_MSG		WM_USER + 106	//����UI EDITBOX����Ϣ