#pragma once
//温度数据类
/*
 * author wjy
 * 添加接口利于数据解析
 */
#define MAX_SIZE 1024
#include <string>
using namespace std;
class CTempData
{
public:
	CTempData();
	~CTempData();

	double m_temperature;									// 温度
	SYSTEMTIME m_date;										// 时间

	CTempData(double temperature, SYSTEMTIME date);
	CString GetTemperature();								//返回温度的字符串
	CString GetTime();									    //返回时间的字符串
	string ToData();										// 返回字符串
};

