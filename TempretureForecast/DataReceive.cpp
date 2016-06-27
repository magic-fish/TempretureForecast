#include "stdafx.h"
#include "DataReceive.h"
#include "TempretureForecastDlg.h"
DataReceive::DataReceive()
{
	//Init();
}


DataReceive::~DataReceive()
{
	closesocket(m_sClient);
	closesocket(m_sServer);
	WSACleanup();
}

void DataReceive::SetFrequency(string frequency)
{
	m_frequency = frequency;
	m_isSend = true;
}

void DataReceive::Init(CTempretureForecastDlg * pDlg)
{
	mTaskMainDlg = pDlg;
	m_isSend = false;
	WSAStartup(MAKEWORD(2, 2), &m_wsd);
	m_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	bind(m_sServer, (sockaddr*)&addr, sizeof(addr));
	listen(m_sServer, 1);
	cout << "listen" << endl;
	SOCKADDR_IN addrClient;
	int addrlen = sizeof(addr);
	m_sClient = accept(m_sServer, (sockaddr*)&addrClient, &addrlen);
	cout << "accept";
}

void DataReceive::Start()
{
	m_isRunning = true;
	CreateThread(NULL, NULL, RunThread, this, NULL, NULL);
}

DWORD WINAPI DataReceive::RunThread(LPVOID lpParam)
{
	while (((DataReceive*)lpParam)->m_isRunning)
	{
		((DataReceive*)lpParam)->Run();
	}
	return 0;
}

void DataReceive::Run()
{
	if (m_isSend)
	{
		CreateThread(NULL, NULL, SendThread, this, NULL, NULL);
		m_isSend = false;
	}

	ZeroMemory(m_buf, BUF_SIZE);
	int retVal = recv(m_sClient, m_buf, BUF_SIZE, 0);
	if (retVal > 0)
	{
		m_buf[retVal] = 0;
		double  temperature = atof(m_buf);
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		mTaskMainDlg->UpDateTempView(new CTempData(temperature, sys));
	}
}

DWORD WINAPI DataReceive::SendThread(LPVOID lpParam)
{
	((DataReceive*)lpParam)->Send();
	return 0;
}



void DataReceive::Send()
{
	send(m_sClient, m_frequency.c_str(), m_frequency.length(), 0);
}



void DataReceive::End()
{
	m_isRunning = false;
}