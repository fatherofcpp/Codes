/*
//	Algo.h ͷ�ļ�
//	�ᶨλ����㷨
//
*/
#ifndef ALGO_HEAD_FILE
#define ALGO_HEAD_FILE
#include <iostream>
#include <deque>
#include <queue>
#include <string>
#include <stack>
#include <vector>

using namespace std;
typedef pair<string, long> OR_PAIR;		//ԭʼ�ַ���
typedef pair<float, long>  PAIR;		//<�������ǣ� PCI-1751>
typedef pair<int,   long>  PAIR_AXIS;	//<�������ݿ�ȣ� PCI-1751��������>
typedef pair<long,  long>  PAIR_MATCH_ARR;//�����������

typedef struct _AIXS_DETAIL
{
	int left_idx;	//�������
	int right_idx;	//�Ҳ�����
	float left_h;	//�����߶�
	float right_h;	//�Ҳ���߶�
	int   axis_w;	//���
	long  axis_pci;	//�����ĵ�PCIֵ
}AIXS_DETAIL;
typedef struct _tagPAIR_INFO
{
	double dtime;
	int    dlen;
	float  ddata;	//�����ļ������������
	string dsre;
}PAIR_INFO;			// test-struct ADD 20170417

typedef struct _tagPAIR_ARR				//�����������
{
	PAIR_AXIS pLeft;
	PAIR_AXIS pRight;
	_tagPAIR_ARR()
	{
		memset(this, 0, sizeof(_tagPAIR_ARR));
	}
}PAIR_ARR;

void Filter(vector<PAIR>& vArr, vector<PAIR_AXIS>& outArr);
void ZhouDisFilter(vector<PAIR_AXIS>& inArr, vector<PAIR_AXIS>& outArr);

void Filter(vector<PAIR>& vArr, vector<AIXS_DETAIL>& outArr);
void ZhouDisFilter(vector<AIXS_DETAIL>& inArr, vector<AIXS_DETAIL>& outArr);

void TrainBottom(vector<PAIR>& vArr, vector<PAIR>& outArr);

void LunzhouFilter(vector<PAIR>* left_in, vector<PAIR>* right_in, vector<PAIR_AXIS>& left_o, vector<PAIR_AXIS>& right_o);		//����Filter
void LunzhouAcquire(vector<PAIR_AXIS>* left_in, vector<PAIR_AXIS>* right_in, vector<PAIR_AXIS>& left_o, vector<PAIR_AXIS>& right_o);
void LunzhouMatch(vector<PAIR_AXIS>* left_in, vector<PAIR_AXIS>* right_in, vector<PAIR_MATCH_ARR>& match_arr);					//�������
void LunzhouUpdate(vector<PAIR_MATCH_ARR>* arr_in, vector<long>& arr_o);		

int ZhouDisFilter4CRV2(vector<AIXS_DETAIL>& inArr, vector<long>& outArr);

//LOG the data
int LogVec(vector<PAIR>* inArr, string fname);
int LogVec(vector<PAIR_AXIS>* inArr, string fname);
int LogVec(vector<AIXS_DETAIL>* inArr, string fname);
int LogVec(vector<string>* inArr, string fname);
int LogVec(vector<long>* inArr, long headpci, string fname);

int LogVec(shared_ptr<vector<PAIR>> in, string fname);
/// �ڶ������������㷨
/*
	DataBrush function brush the bad data in vector
	Rule:
	for pair value in vector
	1. current pci must<= fore pci, else replace current value use fore pci
	2. current dis must<= 11.5, else repalce current value use fore dis
	1.2 use one for circle'
	3. if current dis >1.0, and fore dis < 1.0 and more than 3 value <1.0 in behind value,then repalce current value use fore value
*/
void Databrush(vector<PAIR>& in, vector<PAIR>& out);

/*
	Findtrainhead function: find train-head pci value
	param:
	in:	input vector 
	headbeg: where begin to search headpci
	slen: how long to search for headpci
	headpci: search result
*/
void Findtrainhead(vector<PAIR>& in, long headbeg, long slen, long& headpci);

/*
	Findzhou function: find train zhou
	param:
	in:	input vector 
	headpci: train head pci
	out: zhou output
*/
void Findzhou(vector<PAIR>& in, long headpci, vector<PAIR>& out);

#define AD_XISHU  0.0165055675093084

void FindAixeCandiate_20171219(vector<PAIR>& in,  vector<long>& out);
void FindFZ50000_20171219(vector<long>& in, vector<long>& out);
#if 0
void FindAixeCandiate_wu(vector<PAIR>& in,  vector<long>& out);
void FindFZ50000_wu(vector<long>& in, vector<long>& out);
#endif
void FindAixeCandiate_wu(vector<PAIR>& in,  vector<PAIR>& out);
void FindFZ50000_wu(vector<PAIR>& in, vector<PAIR>& out);
void FindFZ54000_wu(vector<PAIR>& in, vector<PAIR>& out);
#endif//ALGO_HEAD_FILE