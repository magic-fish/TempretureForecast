#include "stdafx.h"
#include "Display.h"


CDisplay::CDisplay()
{
	// ��������
	blackPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	bluePen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	grayPen.CreatePen(PS_DOT, 1, RGB(128, 128, 128));
	redPen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	greenPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	brownPen.CreatePen(PS_SOLID, 1, RGB(128, 0, 0));
   //���ݳ�ʼ��
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
	//ɾ������
	blackPen.DeleteObject();
	bluePen.DeleteObject();
	grayPen.DeleteObject();
	redPen.DeleteObject();
	greenPen.DeleteObject();
	brownPen.DeleteObject();
}


void CDisplay::Init(CRect rect)
{
	//��ʼ������ϵ����
	m_codRect.bottom = rect.bottom - 50;
	m_codRect.top = rect.top + 20;
	m_codRect.left = rect.left + 50;
	m_codRect.right = rect.right - 20;
}


//��������ϵ
void CDisplay::DrawCoordinate(CDC *pDC,CRect rect)
{
	pOldPen = pDC->SelectObject(&blackPen);
	pDC->Rectangle(rect);
	//����Y��   
	pDC->MoveTo(m_codRect.left, m_codRect.bottom);
	pDC->LineTo(m_codRect.left, m_codRect.top);
	//���Ƽ�ͷ
	pDC->LineTo(m_codRect.left - 4, m_codRect.top + 4);
	pDC->MoveTo(m_codRect.left, m_codRect.top);
	pDC->LineTo(m_codRect.left + 4, m_codRect.top + 4);

	//����X��
	pDC->MoveTo(m_codRect.left, m_codRect.bottom);
	pDC->LineTo(m_codRect.right, m_codRect.bottom);
	//���Ƽ�ͷ
	pDC->LineTo(m_codRect.right - 4, m_codRect.bottom - 4);
	pDC->MoveTo(m_codRect.right, m_codRect.bottom);
	pDC->LineTo(m_codRect.right - 4, m_codRect.bottom + 4);


	//��������Ϳ̶�
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

	//ʱ��ָ���
	pDC->SelectObject(&bluePen);
	pDC->MoveTo(m_codRect.left + xavlength, m_codRect.bottom);
	pDC->LineTo(m_codRect.left + xavlength, m_codRect.top);
	//��ʾ��������Сʱ���¶ȣ�ǰһ��Сʱ�ǲɼ����ģ���һ��Сʱ��Ԥ���
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

	//����X/Y���ʶ
	pDC->TextOutW(m_codRect.left - 40, m_codRect.bottom - yavlength * 5, _T("��"));
	pDC->TextOutW(m_codRect.left - 40, m_codRect.bottom - yavlength * 5 + 15, _T("��"));

	pDC->TextOutW(m_codRect.left + xavlength, m_codRect.bottom + 30, _T("ʱ��"));
	// �ָ��ɻ���   
	pDC->SelectObject(pOldPen);
}


// �����¶�����ͼ
void CDisplay::DrawGraph(CDC *pDC)
{

	float fDeltaX;     // x������������ͼ����������   
	float fDeltaY;     // y��ÿ���߼���λ��Ӧ������ֵ   
	int nX;      // ������ʱ���ڴ洢��ͼ��ĺ�����   
	int nY;      // ������ʱ���ڴ洢��ͼ���������   

	// ����fDeltaX��fDeltaY   
	fDeltaX = (float)m_codRect.Width() / 2 / (DATA_SIZE-1);
	fDeltaY = (float)m_codRect.Height() / 50;
	SetMaxAndMinTemp();

	pOldPen = pDC->SelectObject(&greenPen);

	// ����ǰ���ƶ�����ͼ�ؼ����ڵ����½ǣ��Դ�Ϊ���ε���ʼ��   
	pDC->MoveTo(m_codRect.left, m_codRect.bottom);
	// ����m_nzValues������ÿ�����Ӧ������λ�ã����������ӣ������γ�����   
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

	//����Ԥ������
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
	// �ָ��ɻ���   
	pDC->SelectObject(pOldPen);
}


void CDisplay::AddData(CTempData data)
{
	if (m_tempData.size() == DATA_SIZE)
		m_tempData.erase(m_tempData.begin());
	m_tempData.push_back(data);
}


// �������¶Ⱥ�����¶�
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