#include "stdafx.h"
#include "AlgorithmFactory.h"


CAlgorithmFactory::CAlgorithmFactory()
{
}


CAlgorithmFactory::~CAlgorithmFactory()
{
}

CPredictAlgorithm * CAlgorithmFactory::CreateAlgorithm(string name)
{
	CPredictAlgorithm*pAlgorithm = NULL;
	 if (name == "直线拟合"){
		 pAlgorithm = new LinearPredict();
	 }
	 if (name == "曲线拟合")
	 {
		 pAlgorithm = new CurvePredict();
	 }
	 return pAlgorithm;
}