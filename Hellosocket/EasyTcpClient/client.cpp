#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<Windows.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  //得添加个库文件，不然WSAStart会报错
//Datapackage
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
	//编写socket代码，用socket api建立简易tcp客户端
	//建立一个socket 套接字
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4,面向数据流
	if (INVALID_SOCKET == sock)
	{
		cout << "连接socket失败" << endl;
	}
	else
	{
		cout << "连接socket成功" << endl;
	}
	//连接服务器 connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;//做ipv4
	_sin.sin_port = htons(4567);//host to net unsigned short 连接端口号
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//一定要指向有意义的
	int ret= connect(sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		cout << "连接服务器失败" << endl;
	}
	else
	{
		cout << "连接服务器成功" << endl;
	}
	while (true)
	{
		//输入请求命令
		char cmdBuf[128] = {};
		cin >> cmdBuf;
		//处理请求
		if (0 == strcmp(cmdBuf, "exit"))
		{
			cout << "收到exit，任务结束" << endl;
			break;
		}
		else if (0 == strcmp(cmdBuf, "login"))
		{
			//向服务端发送请求
			Login login = { "sbw","sbwsg" };
			Dataheader dh = {sizeof(login),CMD_LOGIN};
			send(sock, (const char*)&dh, sizeof(dh), 0); //包头
			send(sock, (const char*)&login, sizeof(login), 0);//包体
			//接收服务器返回的数据
			Dataheader retheader = {};
			Loginresult loginret = {};
			recv(sock, (char*)&retheader, sizeof(retheader), 0);
			recv(sock, (char*)&loginret, sizeof(loginret), 0);
			cout << "LoginResult:" << loginret.result << endl;
		}
		else if (0 == strcmp(cmdBuf, "logout"))
		{
			Logout logout = { "sbw" };
			Dataheader dh = { sizeof(logout),CMD_LOGIN };
			send(sock, (const char*)&dh, sizeof(dh), 0); //包头
			send(sock, (const char*)&logout, sizeof(logout), 0);//包体
			//接收服务器返回的数据
			Dataheader retheader = {};
			Logoutresult logoutret = {};
			recv(sock, (char*)&retheader, sizeof(retheader), 0);
			recv(sock, (char*)&logoutret, sizeof(logoutret), 0);
			cout << "LogoutResult:" << logoutret.result << endl;
		}
		else
		{
			cout << "不支持的命令，重新输入" << endl;
		}
	}
	//关闭
	closesocket(sock);
	WSACleanup();
	getchar();
	return 0;
}
	