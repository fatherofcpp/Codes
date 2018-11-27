win下开机不登陆系统自动运行程序  
Windows NT Resource Kit 提供了2个小工具可以帮助我:  Instrsrv.exe installs and removes system services from Windows NT. Srvany.exe allows any Windows NT application to run as a service.  Instsrv.exe可以给系统安装和删除服务，Srvany.exe可以让程序以服务的方式运行，你可以到我博客里的下载基地下载"Slyar_InstSrv_Srvany.rar"得到它们。下面我来说具体安装过程。

service.rar
 
1、将Instsrv.exe和Srvany.exe放置到某目录下，下面用%path%代替路径 
2、打开CMD，输入以下内容，其中ServerName为你要创建的服务名称 %path%\instsrv ServerName %path%\srvany.exe   如：instsrv RepeaterSrv e:\work\hacServ\srvany.exe   
3、打开regedit注册表编辑器，找到以下目录  HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<ServerName> 如：HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\RepeaterSrv   
4、鼠标右键单击<ServerName>，创建一个"项"，名称为"Parameters"  
5、鼠标左键单击"Parameters"，在右边点击鼠标右键，创建一个"字符串值"(REG_SZ)，名称为"Application"，数值数据里填写你要执行的程序完整路径。  
6.               右键-新建-字符串值AppDirectory 
7.               双击AppDirectory   填入   应用程序的工作目录,如:   D:\workspace\app\
8(可选)、鼠标左键单击"Parameters"，在右边点击鼠标右键，创建一个"字符串值"(REG_SZ)，名称为"DependOnService"，数值数据里填写该服务依存的服务名称。不知道依存是什么的，你可以保持继续不知道，我不解释。  
9、打开services.msc服务控制面板，找到<ServerName>，鼠标右键-属性-登陆，勾选"允许服务与桌面交互"(一般你要运行的程序都是有界面的吧)。
