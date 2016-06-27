#include "stdafx.h"
#include "LinearPredict.h"


LinearPredict::LinearPredict()
{
}


LinearPredict::~LinearPredict()
{
}


vector<double> LinearPredict::Calculate(vector<CTempData> tempDatas)
{
	vector<double>coefficient;           //һ�κ���y=ax+b���ص�a��b����
	float coe_a = 0;                     //������a����
	float coe_b = 0;					 //������b����
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

	coefficient.push_back(coe_a);
	coefficient.push_back(coe_b);
	return coefficient;
}