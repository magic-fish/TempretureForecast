#pragma once
#include "TempData.h"
#include "stdafx.h"
#include<vector>
using namespace std;

class LinearPredict
{
public:
	LinearPredict();
	~LinearPredict();
	static vector<double> Calculate(vector<CTempData> tempDatas);
};

