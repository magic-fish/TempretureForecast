#include "stdafx.h"
#include "TempData.h"


CTempData::CTempData()
{
}


CTempData::~CTempData()
{
}



CTempData::CTempData(double temperature, SYSTEMTIME date)
{
	this->m_date = date;
	this->m_temperature = temperature;
}

CString CTempData::GetTemperature()
{
	CString data;
	data.Format(_T("%lf\n"), m_temperature);
	return data;
}
CString CTempData::GetTime()
{
	CString data;
	data.Format(_T("%02d:%02d:%02d.%3d\n"), m_date.wHour, m_date.wMinute, m_date.wSecond, m_date.wMilliseconds);
	return data;
}


// �����ַ���
string CTempData::ToData()
{
	char data[MAX_SIZE];
	sprintf_s(data, "%02d:%02d:%02d:%3d:%lf", m_date.wHour, m_date.wMinute, m_date.wSecond, m_date.wMilliseconds, m_temperature);
	string str = data;
	return data;
}
