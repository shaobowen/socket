#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<Windows.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  //�����Ӹ����ļ�����ȻWSAStart�ᱨ��

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
	//��дsocket����
	//����һ��socket �׽���
	SOCKET sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//bind�����ڿͻ������ӵ�����˿�
	sockaddr_in _sin = {}; 
	_sin.sin_family = AF_INET;//��ipv4
	_sin.sin_port = htons(4567);//host to net unsigned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;//�󶨴�����&&INADDR_ANY
	if (SOCKET_ERROR == bind(sock, (sockaddr*)&_sin, sizeof(_sin)))  //Ҫǿ��ת����addr
	{
		cout << "�󶨶˿�ʧ��" << endl;
	}
	else
	{
		cout << "�󶨶˿ڳɹ�" << endl;
	}
	//listen��������˿�
	if (SOCKET_ERROR == listen(sock, 5))
	{
		cout << "�����˿�ʧ��" << endl;
	}
	else
	{
		cout << "�����˿ڳɹ�" << endl;
	}
	//accept �ȴ��ͻ�������
	sockaddr_in clientAddr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;    //��Ч��socket��ַ
	char msgBuf[] = "Hello,I am Server";
	char _recvBuf[128] = {};
	_cSock = accept(sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock)
	{
		cout << "���յ���Ч�ͻ���" << endl;
	}
	cout << "�¿ͻ��˼���: socket=" << _cSock<<" IP=" << inet_ntoa(clientAddr.sin_addr) << endl; //��Ҫ�������#define _WINSOCK_DEPRECATED_NO_WARNINGS
	while (true)
	{
		//���ܿͻ�������
		int nLen = recv(_cSock, _recvBuf, 128, 0);//�����ǿͻ����Ƿ�����Ӧ�ó�����recv������TCP���ӵ���һ�˽�������
		if (nLen <= 0)
		{
			cout << "�ͻ����Ѿ��˳����������" << endl;
			break;
		}
		cout << "�յ�����:" << _recvBuf << endl;
		//��������
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
		if (0 == strcmp(_recvBuf, "getInfo"))
		{
			Datapackage dp = { 80,"����" };
			send(_cSock, (const char*)&dp, sizeof(Datapackage), 0); 
		}
		else
		{
			char msgBuf[] = "?????";
			//��ͻ��˷���һ������
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		
	}
	//�ر�
	closesocket(sock);
	WSACleanup();
	cout << "�Ѿ��˳����������" << endl;
	getchar();
	return 0;
}
