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
	float coe_a;                     //函数的a参数
	float coe_b;					 //函数的b参数
	//计算预测曲线函数
	void Calculate(vector<CTempData> tempDatas);
public:
	LinearPredict();
	~LinearPredict();
	//返回实际预测的温度曲线数据
	 vector<CTempData> GetForecastData(vector<CTempData> tempDatas);
};

