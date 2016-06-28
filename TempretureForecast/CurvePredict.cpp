#include "stdafx.h"
#include "CurvePredict.h"


CurvePredict::CurvePredict()
{
}


CurvePredict::~CurvePredict()
{
}

void CurvePredict::Calculate(vector<CTempData> tempDatas)
{
	double coefficient[EM_SIZE];
	memset(coefficient, 0, sizeof(double)* EM_SIZE);
	vector<double> vx, vy;
	for (int i = 0; i<tempDatas.size(); i++)
	{
		vx.push_back(i);
		vy.push_back(tempDatas[i].m_temperature);
	}
	EMatrix(vx, vy, tempDatas.size(), EX_SIZE, coefficient);
	m_coefficient.push_back(coefficient[3]);
	m_coefficient.push_back(coefficient[2]);
	m_coefficient.push_back(coefficient[1]);
}

//累加  
double CurvePredict::sum(vector<double> Vnum, int n)
{
	double dsum = 0;
	for (int i = 0; i<n; i++)
	{
		dsum += Vnum[i];
	}
	return dsum;
}
//乘积和  
double CurvePredict::MutilSum(vector<double> Vx, vector<double> Vy, int n)
{
	double dMultiSum = 0;
	for (int i = 0; i<n; i++)
	{
		dMultiSum += Vx[i] * Vy[i];
	}
	return dMultiSum;
}
//ex次方和  
double CurvePredict::RelatePow(vector<double> Vx, int n, int ex)
{
	double ReSum = 0;
	for (int i = 0; i<n; i++)
	{
		ReSum += pow(Vx[i], ex);
	}
	return ReSum;
}
//x的ex次方与y的乘积的累加  
double CurvePredict::RelateMutiXY(vector<double> Vx, vector<double> Vy, int n, int ex)
{
	double dReMultiSum = 0;
	for (int i = 0; i<n; i++)
	{
		dReMultiSum += pow(Vx[i], ex)*Vy[i];
	}
	return dReMultiSum;
}
//计算方程组的增广矩阵  
void CurvePredict::EMatrix(vector<double> Vx, vector<double> Vy, int n, int ex, double coefficient[])
{
	for (int i = 1; i <= ex; i++)
	{
		for (int j = 1; j <= ex; j++)
		{
			Em[i][j] = RelatePow(Vx, n, i + j - 2);
		}
		Em[i][ex + 1] = RelateMutiXY(Vx, Vy, n, i - 1);
	}
	Em[1][1] = n;
	CalEquation(ex, coefficient);
}
//求解方程  
void CurvePredict::CalEquation(int exp, double coefficient[])
{
	for (int k = 1; k<exp; k++) //消元过程  
	{
		for (int i = k + 1; i<exp + 1; i++)
		{
			double p1 = 0;

			if (Em[k][k] != 0)
				p1 = Em[i][k] / Em[k][k];

			for (int j = k; j<exp + 2; j++)
				Em[i][j] = Em[i][j] - Em[k][j] * p1;
		}
	}
	coefficient[exp] = Em[exp][exp + 1] / Em[exp][exp];
	for (int l = exp - 1; l >= 1; l--)   //回代求解  
		coefficient[l] = (Em[l][exp + 1] - EmMul(coefficient, l + 1, exp)) / Em[l][l];
}
//供CalEquation函数调用  
double CurvePredict::EmMul(double c[], int l, int m)
{
	double sum = 0;
	for (int i = l; i <= m; i++)
		sum += Em[l - 1][i] * c[i];
	return sum;
}

vector<CTempData> CurvePredict::GetCoefficient(vector<CTempData> tempDatas)
{
	Calculate(tempDatas);
	vector<CTempData>pVec;  
	for (int i = 0; i < tempDatas.size(); i++)
	{
		CTempData pdata;
		GetLocalTime(&pdata.m_date);
		pdata.m_temperature = m_coefficient[0]*i*i+m_coefficient[1]*i+m_coefficient[2];
		pVec.push_back(pdata);
	}
	return pVec;
}