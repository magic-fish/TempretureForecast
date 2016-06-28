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
	float per_x = 0;					 //�������ݺ�����x��ƽ��ֵ
	float per_y = 0;                     //��������������y��ƽ��ֵ
	float sumxy = 0;                     //��������xy�˻��ĺ�
	float sumsqrx = 0;                   //��������x��ƽ����

	//�����������ƽ��ֵ���������
	for (int i = 0; i < tempDatas.size(); i++)
	{
		per_x += i;
		per_y += tempDatas[i].m_temperature;
		sumxy += i * tempDatas[i].m_temperature;
		sumsqrx += i * i;
	}
	per_x /= tempDatas.size();
	per_y /= tempDatas.size();

	//����ab����ֵ
	coe_a = (sumxy - tempDatas.size() * per_x * per_y) / (sumsqrx - tempDatas.size() * per_x * per_x);
	coe_b = per_y - coe_a * per_y;

}

vector<CTempData> LinearPredict::GetForecastData(vector<CTempData> tempDatas)
{
	Calculate(tempDatas);
	vector<CTempData>pVec;           //һ�κ���y=ax+b���ص�a��b����
	for (int i = 0; i < tempDatas.size(); i++)
	{
		CTempData pdata;
		GetLocalTime(&pdata.m_date);
		pdata.m_temperature = coe_a*i + coe_b;
		pVec.push_back(pdata);
	}
	return pVec;
}