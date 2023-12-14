#include<winsock2.h>
#include<iostream>

using  namespace  std;

#pragma comment(lib,"Ws2_32.lib")


int  main()
{
	//初始化winsock的環境
	WSADATA  wd;
	if (WSAStartup(MAKEWORD(2, 2, ), &wd) == SOCKET_ERROR)
	{
		cout << "WSAStartup  error:" << GetLastError() << endl;
		return 0;
	}

	//1.創建監聽套接字
	SOCKET  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		cout << "socket  error:" << GetLastError() << endl;
		return 0;
	}

	//2.綁定到ip與端口
	sockaddr_in  addr;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(8000);
	addr.sin_family = AF_INET;
	int len = sizeof(sockaddr_in);
	if (bind(sListen, (SOCKADDR*)&addr, len) == SOCKET_ERROR)
	{
		cout << "bind  error:" << GetLastError() << endl;
		return 0;
	}

	//3.監聽套接字
	if (listen(sListen, 5) == SOCKET_ERROR)
	{
		cout << "listen  error:" << GetLastError() << endl;
		return 0;
	}

	//4. select開始了
	fd_set  readSet;//定義一個讀（接受消息）的集合
	FD_ZERO(&readSet);//初始化集合
	FD_SET(sListen, &readSet);

	//不停的select纔可以讀取套接字的狀態改變
	while (true)
	{
		fd_set tmpSet;//定義一個臨時的集合
		FD_ZERO(&tmpSet);//初始化集合
		tmpSet = readSet;// 每次循環都是所有的套接字

		//利用select選擇出集合中可以讀寫的多個套接字，有點像篩選
		int ret = select(0, &tmpSet, NULL, NULL, NULL);//最後一個參數爲NULL，一直等待，直到有數據過來
		if (ret == SOCKET_ERROR)
		{
			continue;
		}

		//成功篩選出來的tmpSet可以發送或者接收的socket
		for (int i = 0; i < tmpSet.fd_count; ++i)
		{
			//獲取到套接字
			SOCKET  s = tmpSet.fd_array[i];

			//接收到客戶端的鏈接
			if (s == sListen)
			{
				SOCKET  c = accept(s, NULL, NULL);
				//fd_set集合最大值爲64
				if (readSet.fd_count < FD_SETSIZE)
				{
					//往集合中添加客戶端套接字
					FD_SET(c, &readSet);
					cout << "歡迎" << c << "進入聊天室！" << endl;

					//給客戶端發送歡迎
					char buf[100] = { 0 };
					sprintf(buf, "歡迎%d進入聊天室！", c);
					send(c, buf, 100, 0);
				}
				else
				{
					cout << "達到客戶端容量上線！" << endl;
				}

			}
			else//一定是客戶端
			{
				//接收客戶端的數據
				char buf[100] = { 0 };
				ret = recv(s, buf, 100, 0);
				if (ret == SOCKET_ERROR || ret == 0)
				{
					closesocket(s);
					FD_CLR(s, &readSet);
					cout << s << "離開聊天室！" << endl;
				}
				else
				{
					cout << s << "說：" << buf << endl;
				}
			}
		}
	}

	//關閉監聽套接字
	closesocket(sListen);

	//清理winsock環境
	WSACleanup();

	return 0;
}