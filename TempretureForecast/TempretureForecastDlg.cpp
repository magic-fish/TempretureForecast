
// TempretureForecastDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TempretureForecast.h"
#include "TempretureForecastDlg.h"
#include "afxdialogex.h"
#include "TempData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define POINT_COUNT 48
#define TIMER_1S 0
#define TIMER_10S 1
#define TIMER_30S 2
#define TIMER_1M 3
#define TIMER_10M 4
#define TIMER_30M 5
#define TIMER_1H 6
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTempretureForecastDlg 对话框



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


// CTempretureForecastDlg 消息处理程序

BOOL CTempretureForecastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	InitComboBox();
	//获得客户区
	m_bgPic.GetClientRect(m_rect);
	//创建兼容DC
	m_memDC.CreateCompatibleDC(m_bgPic.GetDC());
	//创建兼容bitmap
	m_bmp.CreateCompatibleBitmap(m_bgPic.GetDC(), m_rect.Width(), m_rect.Height());
    m_memDC.SelectObject(&m_bmp);
 
	m_displayer.Init(m_rect);
	// 以时间为种子来构造随机数生成器   
	srand((unsigned)time(NULL));
	// 启动定时器，ID为1，定时时间为200ms   
	SetTimer(TIMER_1S, 1000, NULL);
	m_curTimerID = TIMER_1S;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTempretureForecastDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTempretureForecastDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTempretureForecastDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值  
	CTempData data;
	GetLocalTime(&data.m_date);
	data.m_tempreture = rand() % 50 + 1;
	m_displayer.AddData(data);
	//绘制坐标系
	m_displayer.DrawCoordinate(&m_memDC,m_rect);
	// 绘制波形图   
	m_displayer.DrawGraph(&m_memDC);
	// 显示当前的最高温度和最低温度
	m_numOfTemp = POINT_COUNT;
	//GetMaxAndMinTemp();
	m_MaxTemp = m_displayer.GetMaxTemp();
	m_MinTemp = m_displayer.GetMinTemp();
	// 显示当前时间和实时温度
	m_date = GetCurTime();
	m_curTime.Format(_T("%02d:%02d:%02d"), data.m_date.wHour, data.m_date.wMinute, data.m_date.wSecond);
	m_curTemp.Format(_T("%.0lf ℃"),data.m_tempreture);
	//将缓冲DC画到实际的窗口上
	m_bgPic.GetDC()->BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_memDC, 0, 0, SRCCOPY);
	//if (data.m_tempreture>=m_alarmValue)
		//AfxMessageBox(_T("高温警报！！"));
	UpdateData(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}


// 获得当前时间段的最高温度和最低温度
void CTempretureForecastDlg::GetMaxAndMinTemp()
{
	//CAlgorithm alg;
	//m_MaxTemp = alg.GetMax(m_numOfTemp, m_waveDataValues);
	//m_MinTemp = alg.GetMin(m_numOfTemp, m_waveDataValues);
}


void CTempretureForecastDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	m_memDC.DeleteDC();        //删除DC
	m_bmp.DeleteObject();      //删除位图
	KillTimer(m_curTimerID);
	CDialogEx::OnClose();
}


CString CTempretureForecastDlg::GetCurTime()
{
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	CString str;
	CString week[8] = {_T(""), _T("周一"), _T("周二"), _T("周三"), _T("周四"), _T("周五"), _T("周六"), _T("周日") };
	str.Format(_T("%04d年%02d月%02d日 ") + week[curTime.wDayOfWeek],curTime.wYear, curTime.wMonth, curTime.wDay);
	return str;
}


CString CTempretureForecastDlg::GetCurTemp(int currTemp)
{
	CString str;
	str.Format(_T("%d"), currTemp);
	str = _T("实时温度：") + str + _T("℃");
	return str;
}


void CTempretureForecastDlg::InitComboBox()
{
	//初始化高温警报
	for (int i = 10; i <= 50; i++)
	{
		CString value;
		value.Format(_T("%d"), i);
		m_highTempAlarm.AddString(value);
	}
	m_highTempAlarm.SetCurSel(40);

	//初始化显示时间间隔
	m_collectInterval.AddString(_T("1秒"));
	m_collectInterval.AddString(_T("10秒"));
	m_collectInterval.AddString(_T("30秒"));
	m_collectInterval.AddString(_T("1分"));
	m_collectInterval.AddString(_T("10分"));
	m_collectInterval.AddString(_T("30分"));
	m_collectInterval.AddString(_T("1小时"));
	m_collectInterval.SetCurSel(0);

	//初始化预测算法
	m_forecastAlgorithm.AddString(_T("最小二乘法"));
	m_forecastAlgorithm.AddString(_T("曲线拟合"));
	m_forecastAlgorithm.SetCurSel(0);
}


void CTempretureForecastDlg::OnCbnSelchangeComboAlarm()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strAlarmValue;
	int nSel;

	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_highTempAlarm.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_highTempAlarm.GetLBText(nSel, strAlarmValue);
	// 将组合框中选中的字符串显示到IDC_SEL_WEB_EDIT编辑框中   
	AfxMessageBox(strAlarmValue);
	m_alarmValue = nSel + 10;
}


void CTempretureForecastDlg::OnCbnSelchangeComboCollectinterval()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strIntervalValue;
	int nSel;
	nSel = m_collectInterval.GetCurSel();
	m_collectInterval.GetLBText(nSel, strIntervalValue);
	AfxMessageBox(strIntervalValue);
	KillTimer(m_curTimerID);
	switch (nSel)
	{
	default:
		break;
	case TIMER_1S:
		SetTimer(TIMER_1S, 1000, NULL);
	case TIMER_10S:
		SetTimer(TIMER_10S, 1000*10, NULL);
	case TIMER_30S:
		SetTimer(TIMER_30S, 1000 * 30, NULL);
	case TIMER_1M:
		SetTimer(TIMER_1M, 1000 * 60, NULL);
	case TIMER_10M:
		SetTimer(TIMER_10M, 1000 * 600, NULL);
	case TIMER_30M:
		SetTimer(TIMER_30M, 1000 * 1800, NULL);
	case TIMER_1H:
		SetTimer(TIMER_1H, 1000 * 3600, NULL);
	}
	m_curTimerID = nSel+1;
}


void CTempretureForecastDlg::OnCbnSelchangeComboForecastalg()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strAlgValue;
	int nSel;
	nSel = m_forecastAlgorithm.GetCurSel();
	m_forecastAlgorithm.GetLBText(nSel, strAlgValue);
	AfxMessageBox(strAlgValue);
}
