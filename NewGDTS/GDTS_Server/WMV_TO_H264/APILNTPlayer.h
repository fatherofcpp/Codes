#pragma once
/*********************************************************
	此回调函数是视频数据的回调函数
	ipc:设备的在线识别号(18999999999.ipc)
	datatype:数据类型(H264,YUV420,BMP,JPG等)目前恒为 1 代表标准的H264数据
	buf: 对应数据类型的数据
	len: 数据的长度
	user:自定义数据，目前为句柄
*********************************************************/
typedef void(_stdcall *VDataBack)(const char *ipc,int datatype,const char *buf,unsigned int len,void*user);
/***************************************************************
   目前定义的消息类型
   1      服务器联接是否成功    code 0 成功， 1 失败
   2      请求设备是否在线      code 0 在线   1 不在线  ipc 相应的设备号
   3      与服务联接中断        code 0
****************************************************************
    此回调为交互消息的回调函数
	type:消息类型
	code:消息码(具体参风类型定义)
	msg:消息体内容(无内容时为NULL)
	ipc:设备识别号(无内容时为NULL)
***************************************************************/
typedef void(_stdcall *MsgBack)(int type,int code,const char *msg,const char *ipc);
#if 0
#ifdef __cplusplus 
extern "C" { 
#endif 
/*******************************************************
	DLL初使化函数
*******************************************************/
__declspec(dllexport) void PlayerInit();

/******************************************************
	DLL 释放函数
******************************************************/
__declspec(dllexport) void PlayerFree();

/*****************************************************
	开始播放函数
	返回值：0成功，其它失败
	ipc:在线设备识别号
	Chan:请求视频的通道号，从0开始 如第一通道则为0，第二通道则为1，依此类推
	handle:函数PlayerLogin返回的句柄,用以识别实例
*****************************************************/
__declspec(dllexport) int StartPlayer(const char *ipc,int Chan,void* handle);

/****************************************************
	停止视频播放函数
	handle:函数PlayerLogin返回的句柄,用以识别实例
****************************************************/
__declspec(dllexport) void StopPlayer(void *handle);

/***************************************************
	设置视频数据回调函数
	返回值：0成功 1失败
	handle:函数PlayerLogin返回的句柄,用以识别实例
	func：回调函数地址指针
***************************************************/
//__declspec(dllexport) int SetPlayerFunc(void * handle,VDataBack func);
__declspec(dllexport) int SetPlayerFunc(void * handle,VDataBack func,MsgBack mfunc);

/****************************************************
	视频服务登入函数
	返回值：0成功，1失败
	usr:用户名
	pw:密码
	addr:视频服务地址
	port:视频服务端口
	handle:输入类型(in)返回实例句柄,用以识别实例
****************************************************/
__declspec(dllexport) int PlayerLogin(const char *usr, const char *pw, const char *addr, const char *port,void * &handle);

/***************************************************
	视频服务登出函数
	handle:函数PlayerLogin返回的句柄,用以识别实例
***************************************************/
__declspec(dllexport) void PlayerUnLogin(void *handle);

/**************************************************
	获取在线设备的识别号，及通道信
	返回值：返回在线设备个数

	list: 输入类型(in)返回在线设备的列表及通道号,如（Test8888,ff3)
	FF:   1111 1111
	通道：5678 1234
	通道状态表明了各个通道是否正常，如果某个通道正常，则其对应的bit为1，
	否则为0。如前端设备有8个通道，分别是通道0~7，对应bit 0~7。举例说明
	如下：如果通道0和1正常，通道2和3故障，则对应的通道状态掩码为0x3，
	转换成可打印的ASCII码则为’3’补足则为03；如果8个通道都正常，则对
	应的通道状态掩码为0xff，转换成可打印的ASCII码则为’ff‘

	len:输入类型(in)返回list的长度
	handle:函数PlayerLogin返回的句柄,用以识别实例
**************************************************/
__declspec(dllexport) int GetDevCount(const char* &list,int &len,void *handle);

/****************************************************
	获取版本号，查询所使用的控件版本
	返回值：返回控件版本字符指针
****************************************************/
__declspec(dllexport) const char* GetDllVersion();
#ifdef __cplusplus 
} 
#endif 
#endif
