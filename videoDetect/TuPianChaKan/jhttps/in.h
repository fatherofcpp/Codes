#ifndef _IN_H_
#define _IN_H_

typedef struct
{
	char stream[32];	//������ ipcamera
	char id[128];		//cmdId		1BD5AD26-13E4-4ce6-9643-F0375AC50878
	char	time[32];	//��Ƶ��⿪ʼʱ��yyMMddhhmmss
	char picData[32];	//��Ƶ�������yyMMdd
	char outIp[32];		//��������ip
	int outPort;		//�����˿�
	char SerNam[64];	//��������/vqd/service/videodetectphotoService
	int PicInd;			//��Ƭ�±�1-3
	char picPath[128];	//��Ƭ·��
}PicParam;

typedef struct
{
	char picPath[128];
	int locPort;
	char SerNam[64];
	int outsidePort;
}ConfParam;

#endif