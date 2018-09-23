/*
//	utility.h ͷ�ļ�
//	���ߺ����ӿڶ���
*/
#ifndef UTILITY_HEAD_CPP
#define UTILITY_HEAD_CPP
#include <vector>
#include <string>

using namespace std;
// �ַ����������͵Ľӿ�
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

//ʱ�亯��
string GetTimeDateString();	//��-��-��-ʱ-��-��
string GetDateString();		//��-��-��

int comstr2int(string str);

//�ַ����ָ��
void Split(string in, vector<string>& out, char* sep);
void Split(char* in, vector<string>& out, char* sep);


struct APPINFO
{
	string Station;		//վ
	string Storage;		//��
	string Gudao;		//�ɵ�	
	string TrainType;	//����
	string TrainNo;		//����
	string TrainPort;	//��λ
	string Gudaoliewei;	//��λ
};

APPINFO make_appinfo(string s1, string s2, string s3, string s4, string s5, string s6, string s7 );


#endif
