/*
//	utility.h 头文件
//	工具函数接口定义
*/
#ifndef UTILITY_HEAD_CPP
#define UTILITY_HEAD_CPP
#include <vector>
#include <string>

using namespace std;
// 字符串解析类型的接口
// The follow function interface discarded;
#if 0 
float parse(unsigned char* data);
long parse_l(unsigned char* data);
double parse_f(unsigned char* data);
#else
template <typename T>
	T parse(unsigned char* data);
#endif
void FindMin(vector<float>& vArr, float& minV);
void FindMax(vector<float>& vArr, float& maxV);

int StringFind(const char *pSrc, const char *pDst);

//时间函数
string GetTimeDateString();	//年-月-日-时-分-秒
string GetDateString();		//年-月-日

int comstr2int(string str);

//字符串分割函数
void Split(string in, vector<string>& out, char* sep);
void Split(char* in, vector<string>& out, char* sep);


struct APPINFO
{
	string Station;		//站
	string Storage;		//库
	string Gudao;		//股道	
	string TrainType;	//车型
	string TrainNo;		//车号
	string TrainPort;	//端位
	string Gudaoliewei;	//列位
};

APPINFO make_appinfo(string s1, string s2, string s3, string s4, string s5, string s6, string s7 );


#endif
