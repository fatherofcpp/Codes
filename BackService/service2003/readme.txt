win�¿�������½ϵͳ�Զ����г���  
Windows NT Resource Kit �ṩ��2��С���߿��԰�����:  Instrsrv.exe installs and removes system services from Windows NT. Srvany.exe allows any Windows NT application to run as a service.  Instsrv.exe���Ը�ϵͳ��װ��ɾ������Srvany.exe�����ó����Է���ķ�ʽ���У�����Ե��Ҳ���������ػ�������"Slyar_InstSrv_Srvany.rar"�õ����ǡ���������˵���尲װ���̡�

service.rar
 
1����Instsrv.exe��Srvany.exe���õ�ĳĿ¼�£�������%path%����·�� 
2����CMD�������������ݣ�����ServerNameΪ��Ҫ�����ķ������� %path%\instsrv ServerName %path%\srvany.exe   �磺instsrv RepeaterSrv e:\work\hacServ\srvany.exe   
3����regeditע���༭�����ҵ�����Ŀ¼  HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<ServerName> �磺HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\RepeaterSrv   
4������Ҽ�����<ServerName>������һ��"��"������Ϊ"Parameters"  
5������������"Parameters"�����ұߵ������Ҽ�������һ��"�ַ���ֵ"(REG_SZ)������Ϊ"Application"����ֵ��������д��Ҫִ�еĳ�������·����  
6.               �Ҽ�-�½�-�ַ���ֵAppDirectory 
7.               ˫��AppDirectory   ����   Ӧ�ó���Ĺ���Ŀ¼,��:   D:\workspace\app\
8(��ѡ)������������"Parameters"�����ұߵ������Ҽ�������һ��"�ַ���ֵ"(REG_SZ)������Ϊ"DependOnService"����ֵ��������д�÷�������ķ������ơ���֪��������ʲô�ģ�����Ա��ּ�����֪�����Ҳ����͡�  
9����services.msc���������壬�ҵ�<ServerName>������Ҽ�-����-��½����ѡ"������������潻��"(һ����Ҫ���еĳ������н���İ�)��
