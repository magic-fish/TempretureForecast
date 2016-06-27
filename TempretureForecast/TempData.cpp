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


// ·µ»Ø×Ö·û´®
CString CTempData::Tostring()
{
	CString data;
	data.Format(_T("%02d:%02d:%02d.%3d  %lf\n"), m_date.wHour, m_date.wMinute, m_date.wSecond, m_date.wMilliseconds, m_temperature);
	return data;
}
