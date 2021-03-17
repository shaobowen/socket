#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<Windows.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  //����Ӹ����ļ�����ȻWSAStart�ᱨ��

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
struct Login:public Dataheader //�̳�
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
	_cSock = accept(sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock)
	{
		cout << "���յ���Ч�ͻ���" << endl;
	}
	cout << "�¿ͻ��˼���: socket=" << _cSock<<" IP=" << inet_ntoa(clientAddr.sin_addr) << endl; //��Ҫ�������#define _WINSOCK_DEPRECATED_NO_WARNINGS
	while (true)
	{
		Dataheader header{};
		//���ܿͻ�������
		int nLen = recv(_cSock, (char*)&header, sizeof(Dataheader), 0);//�����ǿͻ����Ƿ�����Ӧ�ó�����recv������TCP���ӵ���һ�˽�������
		if (nLen <= 0)
		{
			cout << "�ͻ����Ѿ��˳����������" << endl;
			break;
		}
		
		switch (header.cmd)
		{
		case CMD_LOGIN:
		{
			Login login = {};
			recv(_cSock, (char*)&login+sizeof(Dataheader), sizeof(Login)-sizeof(Dataheader), 0); //�̳У������������ǰ�棬ƫ�ư�ͷ���ݴ�С���Զ�ȡλ�ý���ƫ�ƣ���Ϊhead�Ѿ���ȡ��
			cout << "�յ�����:CMD_LOGIN" << ",���ݳ���:" << login.dataLength<<",username:"<<login.userName<<",password:"<<login.password << endl;
			//�����ж��û������Ƿ���ȷ
			Loginresult ret;
			send(_cSock, (char*)&ret, sizeof(Loginresult), 0);
		}
		break;
		case CMD_LOGOUT:
		{
			Logout logout = {};
			recv(_cSock, (char*)&logout+ sizeof(Dataheader), sizeof(logout)- sizeof(Dataheader), 0);
			cout << "�յ�����:CMD_LOGOUT" << ",���ݳ���:" << logout.dataLength << ",username:" << logout.userName << endl;
			//�����ж��û������Ƿ���ȷ
			Logoutresult ret;
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
		
	}
	//�ر�
	closesocket(sock);
	WSACleanup();
	cout << "�Ѿ��˳����������" << endl;
	getchar();
	return 0;
}

