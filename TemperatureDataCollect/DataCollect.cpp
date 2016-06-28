#include "DataCollect.h"


DataCollect::DataCollect()
{
	Init();
}


DataCollect::~DataCollect()
{
	closesocket(m_socket);
	WSACleanup();
}


void DataCollect::Init()
{
	m_isSend = false;
	m_periodCount = 0;
	WSADATA wsd;
	WSAStartup(MAKEWORD(2, 2), &wsd);
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	connect(m_socket, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
}

void DataCollect::Start()
{
	m_isRunning = true;
	CreateThread(NULL, 0, RunThread, this, 0, NULL);
	CreateThread(NULL, 0, RecvThread, this, 0, NULL);
}

DWORD WINAPI DataCollect::RunThread(LPVOID lpParam)
{
	while (((DataCollect*)lpParam)->m_isRunning)
	{
		((DataCollect*)lpParam)->Run();
	}
	return 0;
}

void DataCollect::Run()
{

	char str[BUF_SIZE];
	sprintf_s(str, "%d", GetData());
	if (m_isSend)
	{
		send(m_socket, str, BUF_SIZE, 0);
		cout << str << endl;
		Sleep(1000 / m_frequency);
		//cout << m_frequency << endl;
	}
}

DWORD WINAPI DataCollect::RecvThread(LPVOID lpParam)
{
	while (((DataCollect*)lpParam)->m_isRunning)
	{
		((DataCollect*)lpParam)->Recv();
	}
	return 0;
}

void DataCollect::Recv()
{
	char buf[BUF_SIZE];
	ZeroMemory(buf, BUF_SIZE);
	if (recv(m_socket, buf, BUF_SIZE, 0) > 0)
	{
		m_frequency = atoi(buf);
		m_isSend = true;
	}
}

void DataCollect::End()
{
	m_isRunning = false;
	m_isSend = false;
}


int DataCollect::GetData()
{
	int swing = 20;
	int period = 30;
	int data = swing * sin(2 * PI * (m_periodCount / (period * 1.0))) + swing + rand() % 10;
	m_periodCount++;
	return data;
}
