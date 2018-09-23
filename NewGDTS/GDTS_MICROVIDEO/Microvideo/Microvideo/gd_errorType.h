/*
 * Copyright (c) 2003-2014 MicroVideo <www.micorvideo.cn>
 * All rights reserved.
 */
//finished HK;DaHua
//no err:Dfhs;H3CIMOS;H3CIPC;h3cvm;NetPosa;VNMP;NTGJ;Nthd;Ntqy;Rtsp;
//not to do :H3CIMOS;H3CIPC;h3cvm;NetPosa;VNMP;NTGJ;Nthd;Ntqy;Rtsp;

/********************************************Wranning Type**************************************/
#define GD_ANONYMITYMSG			0xFFFFFFFF				//无名错误

//指针错误
#define GD_POINTERISNULL		0x00000001				//指针为空
#define	GD_MEMORYNOTENOUGH		0x00000002				//内存不足

#define GD_INVALIDPARAMETERS	0x00000100				//参数错误

#define GD_STARTTHREADFAILED	0x00000200				//启动线程失败
#define GD_STOPTHREADFAILED		0x00000201				//停止线程失败


#define GD_OPENDECODERFAILED	0x00000300				//打开解码器失败
#define GD_DECODEFAILED			0x00000301				//解码失败
#define GD_OPENENCODERFAILED	0x00000310
#define GD_ENCODERFAILED		0x00000311

//文件部分
#define GD_FILENAMETOOLONG		0x00000400				//文件名太长
#define GD_OPENFILEFAILED		0x00000401				//打开文件失败
#define GD_READFILEFAILED		0x00000402				//读文件失败
#define GD_WRITEFILEFAILED		0x00000403				//写文件失败
#define GD_FILETYPEINCORRECT	0x00000404				//文件类型不正确

//音视频错误
#define GD_VIDEOTYPEINCORRECT	0x00000500				//视频类型不正确
#define GD_AUDIOTYPEINCORRECT	0x00000501				//音频类型不正确

//协议错误
#define GD_URLINCORRECT			0x00000600				//URL格式不正确
#define GD_PROTOCALINCORRECT	0x00000601				//协议不正确
#define GD_INVALIDIPADDRESS		0x00000602				//IP地址不正确
#define GD_INVALIDPORTNUMBER	0x00000603				//端口不正确
#define GD_URLNAMEINCORRECT		0x00000604				//URL流名称不正确

//socket错误
#define GD_CONNECTTIMEOUT		0x00000700				//TCP连接超时
#define GD_RECVTIMEOUT			0x00000701				//接收超时
#define GD_RECVERROR			0x00000702				//接收错误
#define GD_SENDTIMEOUT			0x00000703				//发送超时
#define GD_SENDERROR			0x00000704				//发送错误
#define GD_CONNECTSERVERFAILED	0x00000710				//连接流媒体服务器失败

//库错误
#define GD_LOADDLLFAILED		0x00000800				//加载动态库失败
#define GD_GETDLLFUNCFAILED		0x00000801				//加载动态库中的函数失败
#define GD_NOINIT				0x00000802				//没有进行初始化
#define GD_ALLOCRESOURCEERROR	0x00000803				//SDK资源分配错误
#define GD_INITERROR			0x00000804				//SDK初始化失败
#define GD_UNINITERROR			0x00000805				//SDK清理失败

//流错误
#define GD_STREAMTYPEINCORRECT	0x00000900				//流类型不正确
#define GD_INVALIDPACKETS		0x00000901				//Packet不正确



//云台部分
#define GD_YTCOMMANDINVALID		0x00001000				//云台解码器协议文法不正确或控制命令不正确
#define GD_YTCOMMANDTOOLONG		0x00001001				//云台解码器命令过长
#define GD_YTCONTEXTINVALID		0x00001002				//云台解码器命令文法不正确
#define GD_YTSPEEDISOUT			0x00001003				//云台控制速度超出范围

//用户名权限
#define GD_PASSWORDERROR		0x00001101				//用户名密码错误
#define GD_NOENOUGHPRI			0x00001102				//权限不足
#define GD_USERNOTEXIST			0x00001103				//用户不存在。注册的用户ID已注销或不可用。
#define GD_MAXUSERNUM			0x00001104				//登录设备的用户数达到最大。
#define GD_CHANNELERROR			0x00001105				//通道号错误。设备没有对应的通道号。
#define GD_OPENCHANNELERROR		0x00001106				//开通通道失败
#define GD_CLOSECHANNELERROR	0x00001107				//关闭通道失败

/*************************************Message Type***************************************************/

#define GD_CONNECTTED			0x80000100				//TCP连接成功
#define GD_ENDOFFILE			0x80000200				//文件结束
#define GD_PRIVATEINFO			0x80000300


#define GD_GPSINFO				0x80000400				//GPS信息
#define GD_GPSSTART				0x80000401				//GPS 启动
#define GD_GPSSTOP				0x80000402				//GPS 停止

#define GD_STREAMHEAD			0x80000500				//流信息
#define GD_STREAMBODY			0x80000501				//流数据