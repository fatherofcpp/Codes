#include "StdAfx.h"
#include "Transfer.h"
#include "xmlAnalyze.h"
#include "log.h"
#include "dataConvert.h"

CTransfer gTransf;

CTransfer::CTransfer(void)
{
	transType = UART_TRANSF;
	sendDataLen = 1024 * 128;
	sendData = new char[sendDataLen];

	pCSerialAPI = new CSerialAPI;
	pCNetApi = new CNetApi;
}

CTransfer::~CTransfer(void)
{
	delete[] sendData;
	delete pCNetApi;
	delete pCSerialAPI;
}

void CTransfer::ClearSendData()
{
	memset(sendData, 0, sendDataLen);
}

void CTransfer::SetTransType( e_transfer_type trans_type )
{
	transType = trans_type;
}

int CTransfer::SendReqAndReceiveBody( int send_len,int type)
{
	int rtn = 0;

	SocketLock.Lock();

	//短连接
	gTransf.CloseFd();
	int fd = gTransf.ConnectFd();
	if (fd < 0)
	{
		gLog.Add("[%s][ConnectFd fail\n",__FUNCTION__);
		rtn = -1;
		goto LEAVE;
	}

	MU_INT8* data = NULL;
	int head_len = MESSAGE_BODY_IND;
	int body_len_xml = 0;
	int tmp_len = 0;
	str_communicate_data com_data;
	memset(&com_data, 0, sizeof(com_data));
	/*
	数据包头 (4字节 : 0x68 0x68 0x16 0x16)
	数据长度 (4字节 : 数据域长度)
	数据域   (4字节 : 指令序号 + M字节:指令内容[经过AES加密后的XML文本])
	CRC校验  (2字节 : 只对从"数据包头"到"数据域"进行CRC校验，CRC校验采用CRC-16-CCITT)
	数据包尾 (4字节 : 0x55 0xAA 0x55 0xAA)
	*/

	//保存初次的发生消息
	ClearSendData();
	memcpy(sendData, gXmlAnalyze.pBuff, send_len);

	//3次重发,对发送与接收有效
	const int total_communicate_send_time = 1;//发送总次数，1表示只发送一次
	int retry_time = 0;
	while(1)
	{
		static int total_communicate_time = 0;//通讯总次数
		static int total_communicate_fail_time = 0;//通讯失败总次数
		total_communicate_time ++;
		gLog.Add("[%s][total_communicate_fail_time=%d,total_communicate_time=%d]\n",
			__FUNCTION__, total_communicate_fail_time, total_communicate_time);

		//加密
		rtn = OnSend(sendData, send_len);
		if(rtn == 0)
		{ 
			gLog.Add("[%s]OnSend[head:%02x%02x%02x%02x][len:%02x%02x%02x%02x][sequece:%02x%02x%02x%02x]\n[xml:%s]\n[parity:%02x%02x][tail:%02x%02x%02x%02x]\n", __FUNCTION__,
				(MU_INT8)sendData[0],(MU_INT8)sendData[1],(MU_INT8)sendData[2],
				(MU_INT8)sendData[3],(MU_INT8)sendData[4],(MU_INT8)sendData[5],
				(MU_INT8)sendData[6],(MU_INT8)sendData[7],(MU_INT8)sendData[8],
				(MU_INT8)sendData[9],(MU_INT8)sendData[10],(MU_INT8)sendData[11],
				&sendData[head_len],
				(MU_INT8)sendData[send_len - 6],(MU_INT8)sendData[send_len - 5],
				(MU_INT8)sendData[send_len - 4],(MU_INT8)sendData[send_len - 3],(MU_INT8)sendData[send_len - 2],(MU_INT8)sendData[send_len - 1]);

			gXmlAnalyze.ClearBuff();
			rtn = OnReceive(gXmlAnalyze.pBuff, head_len);
			if(rtn == 0)
			{
				//head
				tmp_len = 0;
				for (int i = 0; i < 4; i++)
				{
					com_data.head[i] = gXmlAnalyze.pBuff[tmp_len++];
				}
				//data_len
				for (int i = 0; i < 4; i++)
				{
					com_data.data_len[i] = gXmlAnalyze.pBuff[tmp_len++];
				}
				//指令序号
				for (int i = 0; i < 4; i++)
				{
					com_data.sequence[i] = gXmlAnalyze.pBuff[tmp_len++];
				}

				//check head
				if (com_data.head[0] == 0x68 &&
					com_data.head[1] == 0x68 && 
					com_data.head[2] == 0x16 && 
					com_data.head[3] == 0x16 )
				{
					//data_len
					body_len_xml = (com_data.data_len[0]) + 
						(com_data.data_len[1] << 8) + 
						(com_data.data_len[2] << 16) + 
						(com_data.data_len[3] << 24) - 4;

					if(body_len_xml >= 0 && body_len_xml < gXmlAnalyze.buffMallocLen)
					{
						gXmlAnalyze.ClearBuff();
						int tail_len = 2 + 4;//crc,tail
						rtn = OnReceive(gXmlAnalyze.pBuff, body_len_xml + tail_len);
						if(rtn == 0)
						{
							gLog.Add("[%s]OnReceive[%s]", __FUNCTION__, gXmlAnalyze.pBuff);
							//crc
							tmp_len = body_len_xml;
							MU_CHAR crc_val_hi = gXmlAnalyze.pBuff[tmp_len++];
							MU_CHAR crc_val_lo = gXmlAnalyze.pBuff[tmp_len++];
							com_data.parity_crc = (crc_val_hi)  + (crc_val_lo<< 8);

							//xml data
							memcpy(com_data.xml_data, gXmlAnalyze.pBuff, body_len_xml);
							com_data.xml_len = body_len_xml;
							//tail 
							for (int i = 0; i < 4; i++)
							{
								com_data.tail[i] = gXmlAnalyze.pBuff[tmp_len++];
							}

							//check crc
							MU_INT16 crc = check_crc_data_communicate_data(com_data);

							if (crc == com_data.parity_crc)
							{
								//check tail
								if (com_data.tail[0] == 0x55 &&
									com_data.tail[1] == 0xAA && 
									com_data.tail[2] == 0x55 && 
									com_data.tail[3] == 0xAA )
								{
									//成功，先去除尾部6个字节
									int clear_ind = body_len_xml;
									for (int i = 0; i < 6; i++)
									{
										gXmlAnalyze.pBuff[clear_ind++] = 0;
									}
									//AES解密
									decrypt_aes((MU_INT8*)gXmlAnalyze.pBuff, (MU_INT32)body_len_xml);//解密

									rtn = 0;
									goto LEAVE;
								}
								else
								{
									gLog.Add("[%s]OnReceive tail check fail[%02x-%02x-%02x-%02x] ,bodyLen=%d", 
										__FUNCTION__, com_data.tail[0],com_data.tail[1],com_data.tail[2],
										com_data.tail[3],body_len_xml);
									rtn = -12;
									goto LEAVE;
								}
							}
							else
							{
								gLog.Add("[%s]OnReceive crc check fail[%02x] ,bodyLen=%d", 
									__FUNCTION__, com_data.parity_crc,body_len_xml);
								rtn = -11;
								goto LEAVE;
							}

						}
						else
						{
							gLog.Add("[%s]OnReceive body fail,bodyLen=%d", 
								__FUNCTION__,body_len_xml);
							rtn = -1;
							goto LEAVE;
						}
					}
					else
					{
						gLog.Add("[%s]OnReceive bodyLen err,[%02x-%02x-%02x-%02x] ,bodyLen=%d", 
							__FUNCTION__, com_data.data_len[0],
							com_data.data_len[1],
							com_data.data_len[2],
							com_data.data_len[3],body_len_xml);
						rtn = -2;
						goto LEAVE;
					}
				}
				else
				{
					gLog.Add("[%s]OnReceive head err[%02x-%02x-%02x-%02x]", 
						__FUNCTION__, com_data.head[0],
						com_data.head[1],
						com_data.head[2],
						com_data.head[3]);
					rtn = -3;
					goto LEAVE;
				}
			}
			else
			{
				total_communicate_fail_time++;
				if (++retry_time >= total_communicate_send_time)
				{
					gLog.Add("[%s]OnReceive head fail", __FUNCTION__);
					rtn = RECEIVE_FAIL;
					goto LEAVE;
				}
				else
				{
					gLog.Add("[%s]OnReceive head fail[total_uart_retry_time=%d,total_uart_com_time=%d]\n",
						__FUNCTION__, total_communicate_fail_time, total_communicate_time);
					continue;
				}
			}
		}
		else
		{
			total_communicate_fail_time++;

			if (++retry_time >= total_communicate_send_time)
			{
				gLog.Add("[%s]OnSend fail[sendLen=%d]\n", __FUNCTION__, send_len);
				rtn = SEND_FAIL;
				goto LEAVE;
			}
			else
			{
				gLog.Add("[%s]OnSend fail[sendLen=%d,total_uart_retry_time=%d,total_uart_com_time=%d]\n",
					__FUNCTION__, send_len, total_communicate_fail_time,total_communicate_time);
				continue;
			}
		}
	}

LEAVE:
	SocketLock.Unlock();
	return rtn;
}

int CTransfer::OnSend( char *p_data, int i_data )
{
	if (transType == UART_TRANSF)
	{
		return pCSerialAPI->OnSend(p_data, i_data);
	}
	else if(transType == NET_TRANSF)
	{
		int send_len = pCNetApi->OnSend(p_data, i_data);

		return (send_len != i_data);
	}
	else
	{
		return -1;
	}
}

int CTransfer::OnReceive( char *p_data, int i_data, bool b_retry /*= true */ )
{
	if (transType == UART_TRANSF)
	{
		return pCSerialAPI->OnReceive(p_data, i_data);
	}
	else if(transType == NET_TRANSF)
	{
		int recv_len = pCNetApi->OnReceive(p_data, i_data, b_retry);
		return (recv_len != i_data);
	}
	return -1;
}

void CTransfer::SetIpPort( st_ip_port_domain& ip_inf)
{
	if (transType == UART_TRANSF)
	{
	}
	else if(transType == NET_TRANSF)
	{
		pCNetApi->SetServerIp(ip_inf.domain);
		pCNetApi->SetServerPort(ip_inf.port);
	}
}

int CTransfer::ConnectFd()
{
	int ret = 0;
	if (transType == UART_TRANSF)
	{
		ret = pCSerialAPI->OpenUart();
	}
	else if(transType == NET_TRANSF)
	{
		ret = pCNetApi->ConnectTCP();
	}

	return ret;
}

void CTransfer::SetReadTimeOut( struct timeval& time_out )
{
	if (transType == UART_TRANSF)
	{
	}
	else if(transType == NET_TRANSF)
	{
		pCNetApi->SetReadTimeOut(time_out);
	}
}

void CTransfer::CloseFd()
{
	SocketLock.Lock();
	if (transType == UART_TRANSF)
	{
		pCSerialAPI->CloseUart();
	}
	else if(transType == NET_TRANSF)
	{
		pCNetApi->NetClose();
	}
	SocketLock.Unlock();
}

void CTransfer::SetSerialConf( st_serial_conf*serial )
{
	if (transType == UART_TRANSF)
	{
		pCSerialAPI->SetSerialConf(serial);
	}
	else if(transType == NET_TRANSF)
	{
	}
}

int CTransfer::GetFd()
{
	int ret = 0;
	if (transType == UART_TRANSF)
	{
		ret = (int)pCSerialAPI->GetIDComDev();
	}
	else if(transType == NET_TRANSF)
	{
		ret = pCNetApi->GetSockFd();
	}
	return ret;
}




