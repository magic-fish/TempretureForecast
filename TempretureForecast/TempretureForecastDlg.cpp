
// TempretureForecastDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TempretureForecast.h"
#include "TempretureForecastDlg.h"
#include "afxdialogex.h"
#include "TempData.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTempretureForecastDlg �Ի���



CTempretureForecastDlg::CTempretureForecastDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTempretureForecastDlg::IDD, pParent)
	, m_curTemp(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTempretureForecastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BACKGROUND, m_bgPic);
	DDX_Text(pDX, IDC_EDIT_DATE, m_date);
	DDX_Text(pDX, IDC_EDIT_MAXTEMP, m_MaxTemp);
	DDX_Text(pDX, IDC_EDIT_MINTEMP, m_MinTemp);
	DDX_Text(pDX, IDC_EDIT_CURTIME, m_curTime);
	DDX_Text(pDX, IDC_EDIT_CURTEMP, m_curTemp);
	DDX_Control(pDX, IDC_COMBO_ALARM, m_highTempAlarm);
	DDX_Control(pDX, IDC_COMBO_COLLECTINTERVAL, m_collectInterval);
	DDX_Control(pDX, IDC_COMBO_FORECASTALG, m_forecastAlgorithm);

}

BEGIN_MESSAGE_MAP(CTempretureForecastDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
//	ON_WM_COPYDATA()
ON_CBN_SELCHANGE(IDC_COMBO_ALARM, &CTempretureForecastDlg::OnCbnSelchangeComboAlarm)
ON_CBN_SELCHANGE(IDC_COMBO_COLLECTINTERVAL, &CTempretureForecastDlg::OnCbnSelchangeComboCollectinterval)
ON_CBN_SELCHANGE(IDC_COMBO_FORECASTALG, &CTempretureForecastDlg::OnCbnSelchangeComboForecastalg)
END_MESSAGE_MAP()


// CTempretureForecastDlg ��Ϣ�������

BOOL CTempretureForecastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	InitComboBox();
	//��ÿͻ���
	m_bgPic.GetClientRect(m_rect);
	//��������DC
	m_memDC.CreateCompatibleDC(m_bgPic.GetDC());
	//��������bitmap
	m_bmp.CreateCompatibleBitmap(m_bgPic.GetDC(), m_rect.Width(), m_rect.Height());
    m_memDC.SelectObject(&m_bmp);
	HANDLE hThread = CreateThread(NULL, 0, DataReceiveInitProc, (LPVOID)this, 0, NULL);
	CloseHandle(hThread);

	m_displayer.Init(m_rect);
	// ��ʱ��Ϊ���������������������   
	srand((unsigned)time(NULL));
	// ������ʱ����IDΪ1����ʱʱ��Ϊ200ms   
	SetTimer(TIMER_1S, 1000 / TIMER_1HZ, NULL);
	m_curTimerID = TIMER_1S;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTempretureForecastDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTempretureForecastDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTempretureForecastDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTempretureForecastDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ  
	//CTempData data;
	//GetLocalTime(&data.m_date);
	//data.m_tempreture = rand() % 50 + 1;
	//m_displayer.AddData(data);
	////��������ϵ
	//m_displayer.DrawCoordinate(&m_memDC,m_rect);
	//// ���Ʋ���ͼ   
	//m_displayer.DrawGraph(&m_memDC);
	//// ��ʾ��ǰ������¶Ⱥ�����¶�
	//m_numOfTemp = POINT_COUNT;
	////GetMaxAndMinTemp();
	//m_MaxTemp = m_displayer.GetMaxTemp();
	//m_MinTemp = m_displayer.GetMinTemp();
	//// ��ʾ��ǰʱ���ʵʱ�¶�
	//m_date = GetCurTime();
	//m_curTime.Format(_T("%02d:%02d:%02d"), data.m_date.wHour, data.m_date.wMinute, data.m_date.wSecond);
	//m_curTemp.Format(_T("%.0lf ��"),data.m_tempreture);
	////������DC����ʵ�ʵĴ�����
	//m_bgPic.GetDC()->BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_memDC, 0, 0, SRCCOPY);
	////if (data.m_tempreture>=m_alarmValue)
	//	//AfxMessageBox(_T("���¾�������"));
	//UpdateData(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}


// ��õ�ǰʱ��ε�����¶Ⱥ�����¶�
void CTempretureForecastDlg::GetMaxAndMinTemp()
{
	//CAlgorithm alg;
	//m_MaxTemp = alg.GetMax(m_numOfTemp, m_waveDataValues);
	//m_MinTemp = alg.GetMin(m_numOfTemp, m_waveDataValues);
}


void CTempretureForecastDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_memDC.DeleteDC();        //ɾ��DC
	m_bmp.DeleteObject();      //ɾ��λͼ
	KillTimer(m_curTimerID);
	CDialogEx::OnClose();
}


CString CTempretureForecastDlg::GetCurTime()
{
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	CString str;
	CString week[8] = {_T(""), _T("��һ"), _T("�ܶ�"), _T("����"), _T("����"), _T("����"), _T("����"), _T("����") };
	str.Format(_T("%04d��%02d��%02d�� ") + week[curTime.wDayOfWeek],curTime.wYear, curTime.wMonth, curTime.wDay);
	return str;
}


CString CTempretureForecastDlg::GetCurTemp(int currTemp)
{
	CString str;
	str.Format(_T("%d"), currTemp);
	str = _T("ʵʱ�¶ȣ�") + str + _T("��");
	return str;
}


void CTempretureForecastDlg::InitComboBox()
{
	//��ʼ�����¾���
	for (int i = 10; i <= 50; i++)
	{
		CString value;
		value.Format(_T("%d"), i);
		m_highTempAlarm.AddString(value);
	}
	m_highTempAlarm.SetCurSel(40);

	//��ʼ����ʾʱ����
	m_collectInterval.AddString(_T("1Hz"));
	m_collectInterval.AddString(_T("2Hz"));
	m_collectInterval.AddString(_T("3Hz"));
	m_collectInterval.AddString(_T("5Hz"));
	m_collectInterval.AddString(_T("10Hz"));
	m_collectInterval.AddString(_T("20Hz"));
	m_collectInterval.AddString(_T("30Hz"));

	m_collectInterval.SetCurSel(0);

	//��ʼ��Ԥ���㷨
	m_forecastAlgorithm.AddString(_T("��С���˷�"));
	m_forecastAlgorithm.AddString(_T("�������"));
	m_forecastAlgorithm.SetCurSel(0);
}


void CTempretureForecastDlg::OnCbnSelchangeComboAlarm()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strAlarmValue;
	int nSel;

	// ��ȡ��Ͽ�ؼ����б����ѡ���������   
	nSel = m_highTempAlarm.GetCurSel();
	// ����ѡ����������ȡ�����ַ���   
	m_highTempAlarm.GetLBText(nSel, strAlarmValue);
	// ����Ͽ���ѡ�е��ַ�����ʾ��IDC_SEL_WEB_EDIT�༭����   
	AfxMessageBox(strAlarmValue);
	m_alarmValue = nSel + 10;
}


void CTempretureForecastDlg::OnCbnSelchangeComboCollectinterval()
{
	recv.SetFrequency("5");
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strIntervalValue;
	int nSel;
	nSel = m_collectInterval.GetCurSel();
	m_collectInterval.GetLBText(nSel, strIntervalValue);
	//AfxMessageBox(strIntervalValue);
	KillTimer(m_curTimerID);
	switch (nSel)
	{
	
	case TIMER_1S:
		recv.SetFrequency(HZ_1);
		recv.Send();
		SetTimer(TIMER_1S, 1000 / TIMER_1HZ, NULL);
		break;
	case TIMER_10S:
		recv.SetFrequency(HZ_2);
		recv.Send();
		SetTimer(TIMER_10S, 1000 / TIMER_2HZ, NULL);
		break;
	case TIMER_30S:
		recv.SetFrequency(HZ_3);
		recv.Send();
		SetTimer(TIMER_30S, 1000 / TIMER_3HZ, NULL);
		break;
	case TIMER_1M:
		recv.SetFrequency(HZ_5);
		recv.Send();
		SetTimer(TIMER_1M, 1000 / TIMER_5HZ, NULL);
		break;
	case TIMER_10M:
		recv.SetFrequency(HZ_10);
		recv.Send();
		SetTimer(TIMER_10M, 1000 / TIMER_10HZ, NULL);
		break;
	case TIMER_30M:
		recv.SetFrequency(HZ_20);
		recv.Send();
		SetTimer(TIMER_30M, 1000 / TIMER_20HZ, NULL);
		break;
	case TIMER_1H:
		recv.SetFrequency(HZ_30);
		recv.Send();
		SetTimer(TIMER_1H, 1000 / TIMER_30HZ, NULL);
		break;
	default:
		break;
	}
	m_curTimerID = nSel;
}


void CTempretureForecastDlg::OnCbnSelchangeComboForecastalg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strAlgValue;
	int nSel;
	nSel = m_forecastAlgorithm.GetCurSel();
	m_forecastAlgorithm.GetLBText(nSel, strAlgValue);
	AfxMessageBox(strAlgValue);
}


DWORD WINAPI CTempretureForecastDlg::DataReceiveProc(LPVOID lpParameter)
{
	
	return 0;
}

DWORD WINAPI CTempretureForecastDlg::DataReceiveInitProc(LPVOID lpParameter)
{
	CTempretureForecastDlg * pTaskMain = (CTempretureForecastDlg *)lpParameter;   //��thisָ�봫����
	pTaskMain->recv.Init(pTaskMain);
	pTaskMain->recv.Start();
	pTaskMain->recv.SetFrequency("1");
	return 0;
}
void CTempretureForecastDlg::UpDateTempView(CTempData *data)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ  
	//CTempData data;
	//GetLocalTime(&data.m_date);
	//data.m_tempreture = rand() % 50 + 1;
	m_displayer.AddData(*data);
	//��������ϵ
	m_displayer.DrawCoordinate(&m_memDC, m_rect);
	// ���Ʋ���ͼ   
	m_displayer.DrawGraph(&m_memDC);
	// ��ʾ��ǰ������¶Ⱥ�����¶�
	m_numOfTemp = POINT_COUNT;
	//GetMaxAndMinTemp();
	m_MaxTemp = m_displayer.GetMaxTemp();
	m_MinTemp = m_displayer.GetMinTemp();
	// ��ʾ��ǰʱ���ʵʱ�¶�
	m_date = GetCurTime();
	m_curTime.Format(_T("%02d:%02d:%02d"), data->m_date.wHour, data->m_date.wMinute, data->m_date.wSecond);
	m_curTemp.Format(_T("%.0lf ��"), data->m_temperature);
	//������DC����ʵ�ʵĴ�����
	m_bgPic.GetDC()->BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_memDC, 0, 0, SRCCOPY);
	//if (data.m_tempreture>=m_alarmValue)
	//AfxMessageBox(_T("���¾�������"));
	Invalidate(false);
	//UpdateWindow(false);
	//UpdateData(FALSE);
}