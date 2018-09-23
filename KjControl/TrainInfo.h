/*
//	TrainInfo.h ͷ�ļ�
//	����ת�����ص����ݽṹ
*/
#ifndef TRAININFO_HEAD_FILE
#define TRAININFO_HEAD_FILE
//#include "stdafx.h"
#include <iostream>
#include <queue>
#include <deque>
#include <stack>
#include <string>
#include <vector>

using namespace std;
typedef pair<float, int>  PAIR_STOP_POSITION;		//ƫ������ʾ�̵��� �ṹ��
typedef struct	_ZXJNODE
{
	string name;									//ת���������
	//vector<PAIR_STOP_POSITION> pair_stpp;			//
	vector<float> stpp;
}ZXJNODE;											//ת��ܽṹ

typedef struct _TRAININFO
{
	string traintypename;							//������
	vector<string>	zxjname;						//ת�����
	vector<ZXJNODE> zxjinfo;						//ת��ܽڵ���Ϣ
}TRAININFO;

//new defined struct data follow- 2017/10/10 //
/* ͣ��λ��Ϣ�ṹ��
	offset : ��ʾ��ǰͣ��λ��ƫ����
	lrobot : ��ʾ�������ʾ�̵���
	rrobot : ��ʾ�һ�����ʾ�̵���
*/
typedef struct STPINFO
{
	
	long offset;
	int	lrobot;
	int rrobot;
	static STPINFO& make_data(long of, int l, int r)
	{
		static STPINFO stpinfo;
		stpinfo.offset = of;
		stpinfo.lrobot = l;
		stpinfo.rrobot = r;
		return stpinfo;
	}
};
/* ת�����Ϣ
	zxjname : ��ʾת������͵����ƣ����� ��D���� ��D0p����etc
	stpdata : ��ʾ��ǰת������е�ͣ��λ��Ϣ
*/
typedef struct ZXJINFO
{
	string zxjname;
	vector<STPINFO> stpdata;
};
/* ������Ϣ
	traintype : ��ʾ��ǰ�������ͣ����硰CRH380B��
	//trainnumber : ��ʾ��ǰ�������ţ� ���硰3706����������Բ�� �������� DISCARD
	zxjdata :   ��ʾ��ǰ��������ת�����Ϣ
*/
typedef struct TRAINMSG
{
	string traintype;
	//string trainnumber;
	vector<ZXJINFO> zxjdata;
	int dzxjh;	//����ת��ܻ����߶�
	int tzxjh;	//�ϳ�ת��ܻ����߶�

	string zjj;	//ת��ܼ�����

	/*TRAINMSG& operator=(TRAINMSG rhs)
	{
		this->traintype = rhs.traintype;
		this->zxjdata.assign(rhs.zxjdata.begin(), rhs.zxjdata.end());
		this->dzxjh = rhs.dzxjh;
		this->tzxjh = rhs.tzxjh;
		this->zjj	= rhs.zjj;
		return *this;
	}*/
	int zxjnum;	//Ҫ���е�ת��ܸ���
};

#define ZEC_TRAIN(x,y)  "{c,type,"+string(##x)+","+ string(##y)+"}";
string GetZec_Train(string cx,string zxjxh);


#endif //TRAININFO_HEAD_FILE

