#pragma once
#include<vector>
#include"TempData.h"
#define DATA_SIZE 60

using namespace std;

class CDisplay
{
private:
	int m_maxTemp;
	int m_minTemp;
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
	void AddData(CTempData data);    //录入数据
	void AddForecastData(vector<CTempData>p_forcaseData);//录入预测数据forcaseData
	// 获得最高温度和最低温度
	void GetMinAndMaxTemp(int minTemp, int maxTemp);
	CPen blackPen;      // 用于创建黑色画笔   画坐标轴
	CPen bluePen;       // 用于创建蓝色画笔   画时间线
	CPen grayPen;       // 用于创建灰色画笔   画网格线
	CPen redPen;        // 用于创建红色画笔   画预警线
	CPen greenPen;      // 用于创建绿色画笔   画实际温度曲线
	CPen brownPen;      // 用于创建褐色画笔   画预测温度曲线
	CPen *pOldPen;      // 用于存放旧画笔  
	CBrush redBrush;	
	int SetMaxAndMinTemp();
	int GetMaxTemp();
	int GetMinTemp();
};

