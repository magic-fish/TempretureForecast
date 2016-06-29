#pragma once
#include "PredictAlgorithm.h"
#include "LinearPredict.h"
#include "CurvePredict.h"
class CAlgorithmFactory
{
public:
	CAlgorithmFactory();
	~CAlgorithmFactory();
	//����Ԥ���㷨�Ĺ�����
	static CPredictAlgorithm * CreateAlgorithm(string name);
};

