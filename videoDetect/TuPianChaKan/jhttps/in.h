#ifndef _IN_H_
#define _IN_H_

typedef struct
{
	char stream[32];	//流名称 ipcamera
	char id[128];		//cmdId		1BD5AD26-13E4-4ce6-9643-F0375AC50878
	char	time[32];	//视频检测开始时间yyMMddhhmmss
	char picData[32];	//视频检测日期yyMMdd
	char outIp[32];		//本机外网ip
	int outPort;		//本机端口
	char SerNam[64];	//服务器名/vqd/service/videodetectphotoService
	int PicInd;			//照片下标1-3
	char picPath[128];	//照片路径
}PicParam;

typedef struct
{
	char picPath[128];
	int locPort;
	char SerNam[64];
	int outsidePort;
}ConfParam;

#endif