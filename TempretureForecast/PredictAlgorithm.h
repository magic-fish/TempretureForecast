#pragma once
#include "TempData.h"
#include<vector>
using namespace std;
class CPredictAlgorithm
{
public:
	CPredictAlgorithm();
	~CPredictAlgorithm();
	virtual void Calculate(vector<CTempData> tempDatas) =0;
	virtual vector<CTempData> GetForecastData(vector<CTempData> tempDatas) = 0;;
};

