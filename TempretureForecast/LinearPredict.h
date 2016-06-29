#pragma once
#include "TempData.h"
#include "stdafx.h"
#include<vector>
#include "PredictAlgorithm.h"
using namespace std;

class LinearPredict
	:public CPredictAlgorithm
{
private:
	float coe_a;                     //������a����
	float coe_b;					 //������b����
	//����Ԥ�����ߺ���
	void Calculate(vector<CTempData> tempDatas);
public:
	LinearPredict();
	~LinearPredict();
	//����ʵ��Ԥ����¶���������
	 vector<CTempData> GetForecastData(vector<CTempData> tempDatas);
};

