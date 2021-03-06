#include "stdafx.h"
#include "LinearPredict.h"


LinearPredict::LinearPredict()
{
	 coe_a = 0;                
	 coe_b = 0;				
}


LinearPredict::~LinearPredict()
{
}


void LinearPredict::Calculate(vector<CTempData> tempDatas)
{
	float per_x = 0;					 //所有数据横坐标x的平均值
	float per_y = 0;                     //所有数据纵坐标y的平均值
	float sumxy = 0;                     //所有数据xy乘积的和
	float sumsqrx = 0;                   //所以数据x的平方和

	//计算横纵坐标平均值及求和数据
	for (int i = 0; i < tempDatas.size(); i++)
	{
		per_x += i;
		per_y += tempDatas[i].m_temperature;
		sumxy += i * tempDatas[i].m_temperature;
		sumsqrx += i * i;
	}
	per_x /= tempDatas.size();
	per_y /= tempDatas.size();

	//计算ab参数值
	coe_a = (sumxy - tempDatas.size() * per_x * per_y) / (sumsqrx - tempDatas.size() * per_x * per_x);
	coe_b = per_y - coe_a * per_y;

}

vector<CTempData> LinearPredict::GetForecastData(vector<CTempData> tempDatas)
{
	Calculate(tempDatas);
	vector<CTempData>pVec;           //一次函数y=ax+b返回的a，b参数
	for (int i = 0; i < tempDatas.size(); i++)
	{
		CTempData pdata;
		GetLocalTime(&pdata.m_date);
		pdata.m_temperature = coe_a*i + coe_b;
		pVec.push_back(pdata);
	}
	return pVec;
}