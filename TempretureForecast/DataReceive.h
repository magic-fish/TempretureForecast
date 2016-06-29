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
	SOCKET m_sServer;												//接收端套接字
	SOCKET m_sClient;												//发送端套接字
	string m_frequency;												//频率
	char m_buf[BUF_SIZE];
	bool m_isSend;
	bool m_isRunning;
	
public:
	void Init(CTempretureForecastDlg * pDlg);						//初始化
	DataReceive();
	~DataReceive();
	void SetFrequency(string frequency);							//设置频率
	static DWORD WINAPI RunThread(LPVOID lpParam);					//运行线程
	void Run();														//运行线程函数
	static DWORD WINAPI SendThread(LPVOID lpParam);					//发送线程
	void Send();													//发送线程函数
	void Start();													//建立连接
	void End();														//断开连接
public:	
	//主对话框窗口指针
	CTempretureForecastDlg * mTaskMainDlg;
};

