#pragma once
#include <vector>
#include"TempData.h"
#include <Windows.h>
#include "PredictAlgorithm.h"
#define EX_SIZE 3
#define EM_SIZE 10
using namespace std;

class CurvePredict
	:public CPredictAlgorithm
{
private:
	double Em[EM_SIZE][EM_SIZE];
	vector<double>m_coefficient;
	double sum(vector<double> Vnum, int n);
	double MutilSum(vector<double> Vx, vector<double> Vy, int n);		
	double RelatePow(vector<double> Vx, int n, int ex);
	double RelateMutiXY(vector<double> Vx, vector<double> Vy, int n, int ex);
	void EMatrix(vector<double> Vx, vector<double> Vy, int n, int ex, double coefficient[]);
	void CalEquation(int exp, double coefficient[]);
	double EmMul(double c[], int l, int m);
	virtual void Calculate(vector<CTempData> tempDatas);
public:
	CurvePredict();
	~CurvePredict();
	//返回预测的数据
	virtual vector<CTempData> GetForecastData(vector<CTempData> tempDatas);
};

