#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<Windows.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  //得添加个库文件，不然WSAStart会报错

enum CMD
{
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_ERROR
};
struct Dataheader
{
	short dataLength;//长度
	short cmd;//命令
};
//Datapackage
struct Login
{
	char userName[32];
	char password[32];
};
struct Loginresult
{
	int result;
};
struct Logoutresult
{
	int result;
};
struct Logout
{
	char userName[32];
};
int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//编写socket代码
	//建立一个socket 套接字
	SOCKET sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind绑定用于客户端连接的网络端口
	sockaddr_in _sin = {}; 
	_sin.sin_family = AF_INET;//做ipv4
	_sin.sin_port = htons(4567);//host to net unsigned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;//绑定纯内网&&INADDR_ANY
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&_sin, sizeof(_sin)))  //要强制转换下addr
	{
		cout << "绑定端口失败" << endl;
	}
	else
	{
		cout << "绑定端口成功" << endl;
	}
	//listen监听网络端口
	if (SOCKET_ERROR == listen(sock, 5))
	{
		cout << "监听端口失败" << endl;
	}
	else
	{
		cout << "监听端口成功" << endl;
	}
	//accept 等待客户端连接
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;    //无效的socket地址
	char msgBuf[] = "Hello,I am Server";
	_cSock = accept(sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock)
	{
		cout << "接收到无效客户端" << endl;
	}
	cout << "新客户端加入: socket=" << _cSock<<" IP=" << inet_ntoa(clientAddr.sin_addr) << endl; //需要定义个宏#define _WINSOCK_DEPRECATED_NO_WARNINGS
	while (true)
	{
		Dataheader header{};
		//接受客户端数据
		int nLen = recv(_cSock, (char*)&header, sizeof(Dataheader), 0);//不论是客户还是服务器应用程序都用recv函数从TCP连接的另一端接收数据
		if (nLen <= 0)
		{
			cout << "客户端已经退出，任务结束" << endl;
			break;
		}
		cout << "收到命令:" << header.cmd<<",数据长度:"<<header.dataLength << endl;
		switch (header.cmd)
		{
		case CMD_LOGIN:
		{
			Login login = {};
			recv(_cSock, (char*)&login, sizeof(Login), 0);
			//忽略判断用户密码是否正确
			Loginresult ret = { 1 };
			send(_cSock, (char*)&header, sizeof(Dataheader), 0);
			send(_cSock, (char*)&ret, sizeof(Loginresult), 0);
		}
		break;
		case CMD_LOGOUT:
		{
			Logout logout = {};
			recv(_cSock, (char*)&logout, sizeof(logout), 0);
			//忽略判断用户密码是否正确
			Logoutresult ret = { 1 };
			send(_cSock, (char*)&header, sizeof(header), 0);
			send(_cSock, (char*)&ret, sizeof(ret), 0);
		}
		break;
		default:
		{
			header.cmd = CMD_ERROR;
			header.dataLength = 0;
			send(_cSock, (char*)&header, sizeof(header), 0);
		}
		break;
		}
		//处理请求
		/*if (0 == strcmp(_recvBuf, "getName"))
		{
			char msgBuf[] = "Xiao Hu";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		else if(0==strcmp(_recvBuf,"getAge"))
		{
			char msgBuf[] = "80.";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}*/
		
	}
	//关闭
	closesocket(sock);
	WSACleanup();
	cout << "已经退出，任务结束" << endl;
	getchar();
	return 0;
}

