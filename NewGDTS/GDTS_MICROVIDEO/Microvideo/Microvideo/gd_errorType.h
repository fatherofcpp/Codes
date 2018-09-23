/*
 * Copyright (c) 2003-2014 MicroVideo <www.micorvideo.cn>
 * All rights reserved.
 */
//finished HK;DaHua
//no err:Dfhs;H3CIMOS;H3CIPC;h3cvm;NetPosa;VNMP;NTGJ;Nthd;Ntqy;Rtsp;
//not to do :H3CIMOS;H3CIPC;h3cvm;NetPosa;VNMP;NTGJ;Nthd;Ntqy;Rtsp;

/********************************************Wranning Type**************************************/
#define GD_ANONYMITYMSG			0xFFFFFFFF				//��������

//ָ�����
#define GD_POINTERISNULL		0x00000001				//ָ��Ϊ��
#define	GD_MEMORYNOTENOUGH		0x00000002				//�ڴ治��

#define GD_INVALIDPARAMETERS	0x00000100				//��������

#define GD_STARTTHREADFAILED	0x00000200				//�����߳�ʧ��
#define GD_STOPTHREADFAILED		0x00000201				//ֹͣ�߳�ʧ��


#define GD_OPENDECODERFAILED	0x00000300				//�򿪽�����ʧ��
#define GD_DECODEFAILED			0x00000301				//����ʧ��
#define GD_OPENENCODERFAILED	0x00000310
#define GD_ENCODERFAILED		0x00000311

//�ļ�����
#define GD_FILENAMETOOLONG		0x00000400				//�ļ���̫��
#define GD_OPENFILEFAILED		0x00000401				//���ļ�ʧ��
#define GD_READFILEFAILED		0x00000402				//���ļ�ʧ��
#define GD_WRITEFILEFAILED		0x00000403				//д�ļ�ʧ��
#define GD_FILETYPEINCORRECT	0x00000404				//�ļ����Ͳ���ȷ

//����Ƶ����
#define GD_VIDEOTYPEINCORRECT	0x00000500				//��Ƶ���Ͳ���ȷ
#define GD_AUDIOTYPEINCORRECT	0x00000501				//��Ƶ���Ͳ���ȷ

//Э�����
#define GD_URLINCORRECT			0x00000600				//URL��ʽ����ȷ
#define GD_PROTOCALINCORRECT	0x00000601				//Э�鲻��ȷ
#define GD_INVALIDIPADDRESS		0x00000602				//IP��ַ����ȷ
#define GD_INVALIDPORTNUMBER	0x00000603				//�˿ڲ���ȷ
#define GD_URLNAMEINCORRECT		0x00000604				//URL�����Ʋ���ȷ

//socket����
#define GD_CONNECTTIMEOUT		0x00000700				//TCP���ӳ�ʱ
#define GD_RECVTIMEOUT			0x00000701				//���ճ�ʱ
#define GD_RECVERROR			0x00000702				//���մ���
#define GD_SENDTIMEOUT			0x00000703				//���ͳ�ʱ
#define GD_SENDERROR			0x00000704				//���ʹ���
#define GD_CONNECTSERVERFAILED	0x00000710				//������ý�������ʧ��

//�����
#define GD_LOADDLLFAILED		0x00000800				//���ض�̬��ʧ��
#define GD_GETDLLFUNCFAILED		0x00000801				//���ض�̬���еĺ���ʧ��
#define GD_NOINIT				0x00000802				//û�н��г�ʼ��
#define GD_ALLOCRESOURCEERROR	0x00000803				//SDK��Դ�������
#define GD_INITERROR			0x00000804				//SDK��ʼ��ʧ��
#define GD_UNINITERROR			0x00000805				//SDK����ʧ��

//������
#define GD_STREAMTYPEINCORRECT	0x00000900				//�����Ͳ���ȷ
#define GD_INVALIDPACKETS		0x00000901				//Packet����ȷ



//��̨����
#define GD_YTCOMMANDINVALID		0x00001000				//��̨������Э���ķ�����ȷ����������ȷ
#define GD_YTCOMMANDTOOLONG		0x00001001				//��̨�������������
#define GD_YTCONTEXTINVALID		0x00001002				//��̨�����������ķ�����ȷ
#define GD_YTSPEEDISOUT			0x00001003				//��̨�����ٶȳ�����Χ

//�û���Ȩ��
#define GD_PASSWORDERROR		0x00001101				//�û����������
#define GD_NOENOUGHPRI			0x00001102				//Ȩ�޲���
#define GD_USERNOTEXIST			0x00001103				//�û������ڡ�ע����û�ID��ע���򲻿��á�
#define GD_MAXUSERNUM			0x00001104				//��¼�豸���û����ﵽ���
#define GD_CHANNELERROR			0x00001105				//ͨ���Ŵ����豸û�ж�Ӧ��ͨ���š�
#define GD_OPENCHANNELERROR		0x00001106				//��ͨͨ��ʧ��
#define GD_CLOSECHANNELERROR	0x00001107				//�ر�ͨ��ʧ��

/*************************************Message Type***************************************************/

#define GD_CONNECTTED			0x80000100				//TCP���ӳɹ�
#define GD_ENDOFFILE			0x80000200				//�ļ�����
#define GD_PRIVATEINFO			0x80000300


#define GD_GPSINFO				0x80000400				//GPS��Ϣ
#define GD_GPSSTART				0x80000401				//GPS ����
#define GD_GPSSTOP				0x80000402				//GPS ֹͣ

#define GD_STREAMHEAD			0x80000500				//����Ϣ
#define GD_STREAMBODY			0x80000501				//������