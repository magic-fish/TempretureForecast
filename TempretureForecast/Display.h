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
	//����������
	void DrawCoordinate(CDC *pDC,CRect rect);
	// �����¶�����ͼ
	void DrawGraph(CDC *pDC);
	vector<CTempData>m_tempData;     //�ɼ�����
	vector<CTempData>m_forecastData; //Ԥ������
	CRect m_codRect;  //����ϵ����
	void AddData(CTempData data);
};

