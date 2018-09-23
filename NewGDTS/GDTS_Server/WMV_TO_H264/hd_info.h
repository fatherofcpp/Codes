#ifndef DEF_HD_H_
#define DEF_HD_H_


typedef struct tagDEV_HD_INFO{
 char		m_dstIp[32];		//目标服务器IP
 char		m_dstPub[64];		//发布名
 LONG		m_dstStream;	//流下标
 LONG		m_userId;//UserID

}DEV_HD_INFO;

typedef struct tagDEV_HD{//航道使用参数结构
 char		m_deviceIp[64];		//设备服务器IP
 LONG		m_devicePort;		//设备服务器端口
 char		m_deviceType[32];	//设备类型
 char		m_userName[32];		//用户名
 char		m_passWord[32];		//密码
 LONG		m_deviceChn;	//设备通道
 LONG		m_subChn;		//副通道
 LONG		m_playChn;		//播放通道

 DEV_HD_INFO m_devInfo;


}DEV_HD_PARAMS;


#endif //DEF_HD_H_