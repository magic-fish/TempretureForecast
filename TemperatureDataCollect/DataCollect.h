#pragma once
#include <string>
#include <WinSock2.h>
#include<iostream>
#pragma comment(lib, "WS2_32.lib")
using namespace std;
#define BUF_SIZE 64
#define PORT 6666
#define PI 3.1415926
class DataCollect
{
private:
	SOCKET m_socket;
	int m_frequency;
	bool m_isSend;
	bool m_isRunning;
	int m_periodCount;
	void Init();
public:
	DataCollect();
	~DataCollect();
	void Start();
	static DWORD WINAPI RunThread(LPVOID lpParm);
	void Run();
	static DWORD WINAPI RecvThread(LPVOID lpParm);
	void Recv();
	void End();
	int GetData();
};

