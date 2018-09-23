#ifndef MTOOL_HEAD_FILE
#define MTOOL_HEAD_FILE

#include <iostream>
#include <string>
#include <map>

using namespace std;
char hextab[16] = 
{
'0', '1', '2','3','4','5','6','7',
'8', '9', 'A','B','C','D','E','F'
};

string ascii2hex(unsigned char va)
{
	char hex[2] = {0,0};
	int indx = 0;
	indx = (int)va/16;
	hex[0] = hextab[indx];
	indx = (int)va%16;
	hex[1] = hextab[indx];

	string str;
	str.assign(hex,2);
	return str;
}
#endif //MTOOL_HEAD_FILE