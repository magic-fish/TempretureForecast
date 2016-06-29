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
	 if (name == "ֱ�����"){
		 pAlgorithm = new LinearPredict();
	 }
	 if (name == "�������")
	 {
		 pAlgorithm = new CurvePredict();
	 }
	 return pAlgorithm;
}