#include<WinSock2.h>
#include<Windows.h>

#pragma comment(lib,"ws2_32.lib")  //����Ӹ����ļ�����ȻWSAStart�ᱨ��
int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//��дsocket����

	//
	WSACleanup();
	return 0;
}