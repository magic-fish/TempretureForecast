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
	//����������
	void DrawCoordinate(CDC *pDC,CRect rect);
	// �����¶�����ͼ
	void DrawGraph(CDC *pDC);

	vector<CTempData>m_tempData;     //�ɼ�����
	vector<CTempData>m_forecastData; //Ԥ������
	CRect m_codRect;  //����ϵ����
	void AddData(CTempData data);    //¼������
	void AddForecastData(vector<CTempData>p_forcaseData);//¼��Ԥ������forcaseData
	// �������¶Ⱥ�����¶�
	void GetMinAndMaxTemp(int minTemp, int maxTemp);
	CPen blackPen;      // ���ڴ�����ɫ����   ��������
	CPen bluePen;       // ���ڴ�����ɫ����   ��ʱ����
	CPen grayPen;       // ���ڴ�����ɫ����   ��������
	CPen redPen;        // ���ڴ�����ɫ����   ��Ԥ����
	CPen greenPen;      // ���ڴ�����ɫ����   ��ʵ���¶�����
	CPen brownPen;      // ���ڴ�����ɫ����   ��Ԥ���¶�����
	CPen *pOldPen;      // ���ڴ�žɻ���  
	CBrush redBrush;	
	int SetMaxAndMinTemp();
	int GetMaxTemp();
	int GetMinTemp();
};

