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
	SOCKET m_sServer;												//���ն��׽���
	SOCKET m_sClient;												//���Ͷ��׽���
	string m_frequency;												//Ƶ��
	char m_buf[BUF_SIZE];
	bool m_isSend;
	bool m_isRunning;
	
public:
	void Init(CTempretureForecastDlg * pDlg);						//��ʼ��
	DataReceive();
	~DataReceive();
	void SetFrequency(string frequency);							//����Ƶ��
	static DWORD WINAPI RunThread(LPVOID lpParam);					//�����߳�
	void Run();														//�����̺߳���
	static DWORD WINAPI SendThread(LPVOID lpParam);					//�����߳�
	void Send();													//�����̺߳���
	void Start();													//��������
	void End();														//�Ͽ�����
public:	
	//���Ի��򴰿�ָ��
	CTempretureForecastDlg * mTaskMainDlg;
};

