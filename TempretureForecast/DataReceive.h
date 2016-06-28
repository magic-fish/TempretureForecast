#pragma once

#include <WinSock2.h>
#include <windows.h> 
#include <iostream>
#include <string>
#pragma comment(lib, "WS2_32.lib")
using namespace std;
class CTempretureForecastDlg;
#define BUF_SIZE 64
#define PORT 6666
class DataReceive
{
private:
	WSADATA m_wsd;
	SOCKET m_sServer;
	SOCKET m_sClient;
	string m_frequency;
	char m_buf[BUF_SIZE];
	bool m_isSend;
	bool m_isRunning;
	
public:
	void Init(CTempretureForecastDlg * pDlg);
	DataReceive();
	~DataReceive();
	void SetFrequency(string frequency);
	static DWORD WINAPI RunThread(LPVOID lpParam);
	void Run();
	static DWORD WINAPI SendThread(LPVOID lpParam);
	void Send();
	void Start();
	void End();
public:
	//主对话框窗口指针
	CTempretureForecastDlg * mTaskMainDlg;
};

