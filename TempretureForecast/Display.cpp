#include "stdafx.h"
#include "Display.h"


CDisplay::CDisplay()
{
	// 创建画笔
	blackPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	bluePen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	grayPen.CreatePen(PS_DOT, 1, RGB(128, 128, 128));
	redPen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	greenPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	brownPen.CreatePen(PS_SOLID, 1, RGB(128, 0, 0));
   //数据初始化
	for (; m_tempData.size() < DATA_SIZE;)
	{
		CTempData data;
		GetLocalTime(&data.m_date);
		data.m_tempreture = 0;
		m_tempData.push_back(data);
		m_forecastData.push_back(data);
	}
	m_maxTemp = 0;
	m_minTemp = 0;

}


CDisplay::~CDisplay()
{
	//删除画笔
	blackPen.DeleteObject();
	bluePen.DeleteObject();
	grayPen.DeleteObject();
	redPen.DeleteObject();
	greenPen.DeleteObject();
	brownPen.DeleteObject();
}


void CDisplay::Init(CRect rect)
{
	//初始化坐标系矩形
	m_codRect.bottom = rect.bottom - 50;
	m_codRect.top = rect.top + 20;
	m_codRect.left = rect.left + 50;
	m_codRect.right = rect.right - 20;
}


//绘制坐标系
void CDisplay::DrawCoordinate(CDC *pDC,CRect rect)
{
	pOldPen = pDC->SelectObject(&blackPen);
	pDC->Rectangle(rect);
	//绘制Y轴   
	pDC->MoveTo(m_codRect.left, m_codRect.bottom);
	pDC->LineTo(m_codRect.left, m_codRect.top);
	//绘制箭头
	pDC->LineTo(m_codRect.left - 4, m_codRect.top + 4);
	pDC->MoveTo(m_codRect.left, m_codRect.top);
	pDC->LineTo(m_codRect.left + 4, m_codRect.top + 4);

	//绘制X轴
	pDC->MoveTo(m_codRect.left, m_codRect.bottom);
	pDC->LineTo(m_codRect.right, m_codRect.bottom);
	//绘制箭头
	pDC->LineTo(m_codRect.right - 4, m_codRect.bottom - 4);
	pDC->MoveTo(m_codRect.right, m_codRect.bottom);
	pDC->LineTo(m_codRect.right - 4, m_codRect.bottom + 4);


	//绘制网格和刻度
    pDC->SelectObject(&grayPen);
	float xavlength = (float)m_codRect.Width() / 2;
	float yavlength = (float)m_codRect.Height() / 10;
	for (int i = 1; i <= 10; i++)
	{
		CString scale;
		scale.Format(_T("%d"), i * 5);
		if (i == 1)
			pDC->TextOutW(m_codRect.left - 20, m_codRect.bottom, _T("0"));
		pDC->TextOutW(m_codRect.left - 20, m_codRect.bottom - yavlength*i, scale);
		pDC->MoveTo(m_codRect.left, m_codRect.bottom - yavlength*i);
		pDC->LineTo(m_codRect.right, m_codRect.bottom - yavlength*i);
	}

	//时间分割线
	pDC->SelectObject(&bluePen);
	pDC->MoveTo(m_codRect.left + xavlength, m_codRect.bottom);
	pDC->LineTo(m_codRect.left + xavlength, m_codRect.top);
	//显示连续两个小时的温度，前一个小时是采集到的，后一个小时是预测的
	/*for (int i = 0; i <2; i++)
	{
		//CString strTime = this->GetCurTime();
		//strTime = strTime.Mid(strTime.GetLength() - 3, 2);
		//int intTime = _ttoi(strTime) + i;
		CString strTime;
		strTime.Format(_T("%d"), i);
		pDC->TextOutW(codrect.left + i*xavlength, codrect.bottom + 10, strTime);
	}
	*/
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString strTime;
	strTime.Format(_T("%2d:%2d:%2d"), time.wMinute,time.wSecond,time.wMilliseconds);
	pDC->TextOutW(m_codRect.left + xavlength, m_codRect.bottom + 10, strTime);

	//绘制X/Y轴标识
	pDC->TextOutW(m_codRect.left - 40, m_codRect.bottom - yavlength * 5, _T("温"));
	pDC->TextOutW(m_codRect.left - 40, m_codRect.bottom - yavlength * 5 + 15, _T("度"));

	pDC->TextOutW(m_codRect.left + xavlength, m_codRect.bottom + 30, _T("时间"));
	// 恢复旧画笔   
	pDC->SelectObject(pOldPen);
}


// 绘制温度曲线图
void CDisplay::DrawGraph(CDC *pDC)
{

	float fDeltaX;     // x轴相邻两个绘图点的坐标距离   
	float fDeltaY;     // y轴每个逻辑单位对应的坐标值   
	int nX;      // 在连线时用于存储绘图点的横坐标   
	int nY;      // 在连线时用于存储绘图点的纵坐标   

	// 计算fDeltaX和fDeltaY   
	fDeltaX = (float)m_codRect.Width() / 2 / (DATA_SIZE-1);
	fDeltaY = (float)m_codRect.Height() / 50;
	SetMaxAndMinTemp();

	pOldPen = pDC->SelectObject(&greenPen);

	// 将当前点移动到绘图控件窗口的左下角，以此为波形的起始点   
	pDC->MoveTo(m_codRect.left, m_codRect.bottom);
	// 计算m_nzValues数组中每个点对应的坐标位置，并依次连接，最终形成曲线   
	for (int i = 0; i<DATA_SIZE; i++)
	{
		nX = m_codRect.left + (int)(i * fDeltaX);
		nY = m_codRect.bottom - (int)(m_tempData.at(i).m_tempreture * fDeltaY);
		if (m_tempData.at(i).m_tempreture == 0)
			pDC->MoveTo(nX, nY);
		else if (i==0)
			pDC->MoveTo(m_codRect.left, nY);
		else
			pDC->LineTo(nX, nY);
	}

	//绘制预测曲线
	pDC->SelectObject(&brownPen);
	for (int i = 0; i<DATA_SIZE; i++)
	{
		nX = m_codRect.left+m_codRect.Width() / 2 + (int)(i * fDeltaX);
		nY = m_codRect.bottom - (int)(i*i/2);
		if (i == 0)
			pDC->MoveTo(m_codRect.left + m_codRect.Width() / 2, nY);
		else
			pDC->LineTo(nX, nY);
	}
	// 恢复旧画笔   
	pDC->SelectObject(pOldPen);
}


void CDisplay::AddData(CTempData data)
{
	if (m_tempData.size() == DATA_SIZE)
		m_tempData.erase(m_tempData.begin());
	m_tempData.push_back(data);
}


// 获得最高温度和最低温度
void CDisplay::GetMinAndMaxTemp(int minTemp, int maxTemp)
{
	m_minTemp = minTemp;
	m_maxTemp = maxTemp;
}


int CDisplay::SetMaxAndMinTemp()
{
	m_maxTemp = m_minTemp = m_tempData.at(0).m_tempreture;
	for (int i = 0; i < m_tempData.size(); i++)
	{
		m_minTemp = min(m_minTemp, m_tempData.at(i).m_tempreture);
		m_maxTemp = max(m_maxTemp, m_tempData.at(i).m_tempreture);
	}
	return 0;
}


int CDisplay::GetMaxTemp()
{
	return m_maxTemp;
}


int CDisplay::GetMinTemp()
{
	return m_minTemp;
}