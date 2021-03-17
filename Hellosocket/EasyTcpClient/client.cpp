#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<Windows.h>
#include <iostream>
#include <cstring>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  //����Ӹ����ļ�����ȻWSAStart�ᱨ��
//Datapackage
enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};
struct Dataheader
{
	short dataLength;//����
	short cmd;//����
};
//Datapackage
struct Login :public Dataheader //�̳�
{
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char password[32];
};
struct Loginresult :public Dataheader
{
	Loginresult()
	{
		dataLength = sizeof(Loginresult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};
struct Logoutresult :public Dataheader
{
	Logoutresult()
	{
		dataLength = sizeof(Logoutresult);
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
	int result;
};
struct Logout :public Dataheader
{
	Logout()
	{
		dataLength = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//��дsocket���룬��socket api��������tcp�ͻ���
	//����һ��socket �׽���
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4,����������
	if (INVALID_SOCKET == sock)
	{
		cout << "����socketʧ��" << endl;
	}
	else
	{
		cout << "����socket�ɹ�" << endl;
	}
	//���ӷ����� connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;//��ipv4
	_sin.sin_port = htons(4567);//host to net unsigned short ���Ӷ˿ں�
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//һ��Ҫָ���������
	int ret= connect(sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		cout << "���ӷ�����ʧ��" << endl;
	}
	else
	{
		cout << "���ӷ������ɹ�" << endl;
	}
	while (true)
	{
		//������������
		char cmdBuf[128] = {};
		cin >> cmdBuf;
		//��������
		if (0 == strcmp(cmdBuf, "exit"))
		{
			cout << "�յ�exit���������" << endl;
			break;
		}
		else if (0 == strcmp(cmdBuf, "login"))
		{
			//�����˷�������
			Login login;
			strcpy(login.userName, "xiaohu");
			strcpy(login.password, "xiaohu66");
			send(sock, (const char*)&login, sizeof(login), 0);//����
			//���շ��������ص�����
			Loginresult loginret = {};
			recv(sock, (char*)&loginret, sizeof(loginret), 0);
			cout << "LoginResult:" << loginret.result << endl;
		}
		else if (0 == strcmp(cmdBuf, "logout"))
		{
			Logout logout;
			strcpy(logout.userName, "xiaohu");
			send(sock, (const char*)&logout, sizeof(logout), 0);//����
			//���շ��������ص�����
			Logoutresult logoutret = {};
			recv(sock, (char*)&logoutret, sizeof(logoutret), 0);
			cout << "LogoutResult:" << logoutret.result << endl;
		}
		else
		{
			cout << "��֧�ֵ������������" << endl;
		}
	}
	//�ر�
	closesocket(sock);
	WSACleanup();
	getchar();
	return 0;
}
	