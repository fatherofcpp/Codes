#pragma once

/*
���Խӿ�
˵����һ�����Էŵ�һ����������õ�ʱ���ҵ�������ң��ӽ������ó�������ʹ�á�
ע�⣺����ֻ�Ǽ򵥵�װ�غ͵��ò��ԣ�������û���߼������Ի��и��������Ȩ�����и�����߼���
*/

class IStrategy
{
public:
	IStrategy(void);
	virtual ~IStrategy(void);
	virtual void Operate(void) = 0;
};

