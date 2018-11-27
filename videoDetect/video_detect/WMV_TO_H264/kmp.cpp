// bbcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "kmp.h"

/*************************************************
　　函数名称:GetNextArray
　　简要描述: 得到字符串的next数组
　　输入: 
		char *s			模式字符串
		int ns			模式串字符串长度
		int next[]	next数组
　　输出: 
		无返回值
　　修改日志:
**************************************************/
void GetNextArray(char *s, int ns, int next[])
{
	// 第一个字符的next值是-1,因为C中的数组是从0开始的
	next[0] = -1;
	for (int i = 0, j = -1; i < ns - 1; ) {
		// i是主串的游标,j是模式串的游标, 这里的主串和模式串都是同一个字符串
		if (-1 == j ||	// 如果模式串游标已经回退到第一个字符
			s[i] == s[j]){	// 如果匹配成功,  两个游标都向前走一步
			++i;
			++j;
			// 存放当前的next值为此时模式串的游标值
			next[i] = j;
		}
		else // 匹配不成功j就回退到上一个next值
			j = next[j];
	}
}

// KMP字符串模式匹配算法
// 输入: S是主串,T是模式串,pos是S中的起始位置
// 输出: 如果匹配成功返回起始位置,否则返回-1
/*************************************************
　　函数名称:KMP
　　简要描述: KMP字符串模式匹配算法
　　输入: 
		char* s						主串 
		int ns						主串长度
		char *t						模式串
		int nt						模式串长度
　　输出: 
		如果匹配成功返回起始位置,否则返回-1
　　修改日志:
**************************************************/
int KMP(char* s, int ns, char *t, int nt)
{
	int *next = (int *)malloc(nt * sizeof(int));
	// 得到模式串的next数组
	GetNextArray(t, nt, next);

	int i, j;
	for (i = 0, j = 0; i < ns && j < nt; ){ // i是主串游标,j是模式串游标
		if (-1 == j ||				// 模式串游标已经回退到第一个位置
			s[i] == t[j]){ // 当前字符匹配成功,满足以上两种情况时两个游标都要向前进一步
			++i;
			++j;
		}
		else //  匹配不成功,模式串游标回退到当前字符的next值
			j = next[j];
	}

	free(next);

	if (j >= nt)// 匹配成功
		return i - nt;
	else // 匹配不成功
		return -1;
}