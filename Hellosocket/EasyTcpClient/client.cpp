#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<Windows.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  //得添加个库文件，不然WSAStart会报错
struct Datapackage
{
	int age;
	char name[32];
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
		else
		{
			//向服务端发送请求
			send(sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}
		//接收服务器信息 recv
		char recvBuf[128] = {};//数据缓冲区,长度匹配
		int nlen = recv(sock, recvBuf, 256, 0);
		if (nlen > 0)
		{
			Datapackage* dp = (Datapackage*)recvBuf;
			cout << "接收到数据:年龄="<<dp->age<<",姓名=" << dp->name << endl;
		}
	}
	//关闭
	closesocket(sock);
	WSACleanup();
	getchar();
	return 0;
}
	