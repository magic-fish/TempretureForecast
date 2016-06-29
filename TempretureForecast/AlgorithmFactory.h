#pragma once
#include "PredictAlgorithm.h"
#include "LinearPredict.h"
#include "CurvePredict.h"
class CAlgorithmFactory
{
public:
	CAlgorithmFactory();
	~CAlgorithmFactory();
	//创建预测算法的工厂类
	static CPredictAlgorithm * CreateAlgorithm(string name);
};

