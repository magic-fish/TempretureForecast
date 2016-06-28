#pragma once
#include "TempData.h"
#include "stdafx.h"
#include<vector>
using namespace std;

class LinearPredict
{
private:
	float coe_a;                     //函数的a参数
	float coe_b;					 //函数的b参数
public:
	LinearPredict();
	~LinearPredict();
	 void Calculate(vector<CTempData> tempDatas);
	 vector<CTempData> GetForecastData(vector<CTempData> tempDatas);
};

