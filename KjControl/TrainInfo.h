/*
//	TrainInfo.h 头文件
//	定义转向架相关的数据结构
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
typedef pair<float, int>  PAIR_STOP_POSITION;		//偏移量，示教点数 结构体
typedef struct	_ZXJNODE
{
	string name;									//转向架类型名
	//vector<PAIR_STOP_POSITION> pair_stpp;			//
	vector<float> stpp;
}ZXJNODE;											//转向架结构

typedef struct _TRAININFO
{
	string traintypename;							//车型名
	vector<string>	zxjname;						//转向架名
	vector<ZXJNODE> zxjinfo;						//转向架节点信息
}TRAININFO;

//new defined struct data follow- 2017/10/10 //
/* 停车位信息结构体
	offset : 表示当前停车位的偏移量
	lrobot : 表示左机器人示教点数
	rrobot : 表示右机器人示教点数
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
/* 转向架信息
	zxjname : 表示转向架类型的名称，比如 “D”， “D0p”，etc
	stpdata : 表示当前转向架所有的停车位信息
*/
typedef struct ZXJINFO
{
	string zxjname;
	vector<STPINFO> stpdata;
};
/* 车型信息
	traintype : 表示当前动车车型，比如“CRH380B”
	//trainnumber : 表示当前动车车号， 比如“3706”，此项可以不填， 废弃不用 DISCARD
	zxjdata :   表示当前动车所有转向架信息
*/
typedef struct TRAINMSG
{
	string traintype;
	//string trainnumber;
	vector<ZXJINFO> zxjdata;
	int dzxjh;	//动车转向架基础高度
	int tzxjh;	//拖车转向架基础高度

	string zjj;	//转向架架轴间距

	/*TRAINMSG& operator=(TRAINMSG rhs)
	{
		this->traintype = rhs.traintype;
		this->zxjdata.assign(rhs.zxjdata.begin(), rhs.zxjdata.end());
		this->dzxjh = rhs.dzxjh;
		this->tzxjh = rhs.tzxjh;
		this->zjj	= rhs.zjj;
		return *this;
	}*/
	int zxjnum;	//要运行的转向架个数
};

#define ZEC_TRAIN(x,y)  "{c,type,"+string(##x)+","+ string(##y)+"}";
string GetZec_Train(string cx,string zxjxh);


#endif //TRAININFO_HEAD_FILE

