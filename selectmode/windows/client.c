#include<winsock2.h>//winsock2的頭文件
#include<iostream>
using  namespace std;

//勿忘，鏈接dll的lib
#pragma comment(lib, "ws2_32.lib")

int  main()
{
	//加載winsock2的環境
	WSADATA  wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		cout << "WSAStartup  error：" << GetLastError() << endl;
		return 0;
	}

	//1.創建流式套接字
	SOCKET  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		cout << "socket  error：" << GetLastError() << endl;
		return 0;
	}

	//2.連接服務器
	sockaddr_in   addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	int len = sizeof(sockaddr_in);
	if (connect(s, (SOCKADDR*)&addr, len) == SOCKET_ERROR)
	{
		cout << "connect  error：" << GetLastError() << endl;
		return 0;
	}

	//3接收服務端的消息
	char buf[100] = { 0 };
	recv(s, buf, 100, 0);
	cout << buf << endl;

	//3隨時給服務端發消息
	int  ret = 0;
	do
	{
		char buf[100] = { 0 };
		cout << "請輸入聊天內容:";
		cin >> buf;
		ret = send(s, buf, 100, 0);
	} while (ret != SOCKET_ERROR && ret != 0);

	//4.關閉監聽套接字
	closesocket(s);

	//清理winsock2的環境
	WSACleanup();

	return 0;
}