#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<Windows.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  //����Ӹ����ļ�����ȻWSAStart�ᱨ��
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
		else
		{
			//�����˷�������
			send(sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}
		//���շ�������Ϣ recv
		char recvBuf[128] = {};//���ݻ�����,����ƥ��
		int nlen = recv(sock, recvBuf, 256, 0);
		if (nlen > 0)
		{
			Datapackage* dp = (Datapackage*)recvBuf;
			cout << "���յ�����:����="<<dp->age<<",����=" << dp->name << endl;
		}
	}
	//�ر�
	closesocket(sock);
	WSACleanup();
	getchar();
	return 0;
}
	