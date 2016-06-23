#pragma once
#include<vector>
#include"TempData.h"
#define DATA_SIZE 60

using namespace std;

class CDisplay
{
public:
	CDisplay();
	~CDisplay();
	void Init(CRect rect);
	//绘制坐标轴
	void DrawCoordinate(CDC *pDC,CRect rect);
	// 绘制温度曲线图
	void DrawGraph(CDC *pDC);
	vector<CTempData>m_tempData;     //采集数据
	vector<CTempData>m_forecastData; //预测数据
	CRect m_codRect;  //坐标系矩形
	void AddData(CTempData data);
};

