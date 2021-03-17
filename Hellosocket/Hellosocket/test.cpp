#include<WinSock2.h>
#include<Windows.h>

#pragma comment(lib,"ws2_32.lib")  //得添加个库文件，不然WSAStart会报错
int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//编写socket代码

	//
	WSACleanup();
	return 0;
}