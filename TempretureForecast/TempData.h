#pragma once
//温度数据类
/*
 * author wjy
 * 添加接口利于数据解析
 */
class CTempData
{
public:
	CTempData();
	~CTempData();
	// 温度
	double m_temperature;
	// 时间
	SYSTEMTIME m_date;

	CTempData(double temperature, SYSTEMTIME date);
	//返回温度的字符串
	CString GetTemperature();
	//返回时间的字符串
	CString GetTime();
	// 返回字符串
	CString Tostring();
};

