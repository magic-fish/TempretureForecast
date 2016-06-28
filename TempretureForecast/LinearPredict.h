#pragma once
#include "TempData.h"
#include "stdafx.h"
#include<vector>
using namespace std;

class LinearPredict
{
private:
	float coe_a;                     //������a����
	float coe_b;					 //������b����
public:
	LinearPredict();
	~LinearPredict();
	 void Calculate(vector<CTempData> tempDatas);
	 vector<CTempData> GetForecastData(vector<CTempData> tempDatas);
};

