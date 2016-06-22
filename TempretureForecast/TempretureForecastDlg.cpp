
// TempretureForecastDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TempretureForecast.h"
#include "TempretureForecastDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define POINT_COUNT 48
#define TIMER_DRAW 1
#define TIMER_COLLECT 2
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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTempretureForecastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BACKGROUND, m_bgPic);
	DDX_Text(pDX, IDC_EDIT_TIMEANDTEMP, m_dateAndCurTempreture);
	DDX_Text(pDX, IDC_EDIT_MAXTEMP, m_MaxTemp);
	DDX_Text(pDX, IDC_EDIT_MINTEMP, m_MinTemp);
}

BEGIN_MESSAGE_MAP(CTempretureForecastDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
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

	//获得客户区
	//m_bgPic.GetClientRect(&m_rect);
	m_bgPic.GetClientRect(m_rect);
	//创建兼容DC
	m_memDC.CreateCompatibleDC(m_bgPic.GetDC());
	//创建兼容bitmap
	m_bmp.CreateCompatibleBitmap(m_bgPic.GetDC(), m_rect.Width(), m_rect.Height());
    m_memDC.SelectObject(&m_bmp);

	// 以时间为种子来构造随机数生成器   
	srand((unsigned)time(NULL));
	// 启动定时器，ID为1，定时时间为200ms   
	SetTimer(TIMER_DRAW, 200, NULL);
	SetTimer(TIMER_COLLECT, 3600000, NULL);

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
	// 将数组中的所有元素前移一个单位，第一个元素丢弃   
	for (int i = 0; i<POINT_COUNT - 1; i++)
	{
		m_waveDataValues[i] = m_waveDataValues[i + 1];
	}
	// 为最后一个元素赋一个50以内的随机数值（整型）   
	m_waveDataValues[POINT_COUNT - 1] = rand() % 50 + 1;

	//绘制坐标系
	DrawCoordinate(&m_memDC, m_rect);
	// 绘制波形图   
	DrawWave(&m_memDC, m_rect);

	// 显示当前的最高温度和最低温度
	m_numOfTemp = POINT_COUNT;
	//GetMaxAndMinTemp();

	// 显示当前时间和实时温度
	m_dateAndCurTempreture = GetCurTime() + GetCurTemp(m_waveDataValues[POINT_COUNT - 1]);
	//将缓冲DC画到实际的窗口上
	m_bgPic.GetDC()->BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_memDC, 0, 0, SRCCOPY);
	UpdateData(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}


void CTempretureForecastDlg::DrawCoordinate(CDC *pDC, CRect& rect)
{
	CPen dotPen;      // 用于创建点线画笔   
	CPen solidPen;     // 用于创建实心画笔
	CPen *pOldPen;     // 用于存放旧画笔     
	CBrush newBrush;   // 用于创建新画刷   
	CBrush *pOldBrush; // 用于存放旧画刷   
	CRect codrect;
	codrect.bottom = rect.bottom - 50;
	codrect.top = rect.top + 20;
	codrect.left = rect.left + 50;
	codrect.right = rect.right - 20;
	// 创建灰色新画刷   
	newBrush.CreateSolidBrush(RGB(240, 240, 240));
	// 选择新画刷，并将旧画刷的指针保存到pOldBrush   
	pOldBrush = pDC->SelectObject(&newBrush);
	// 以黑色画刷为绘图控件填充黑色，形成黑色背景   
	pDC->Rectangle(rect);
	// 恢复旧画刷   
	pDC->SelectObject(pOldBrush);
	// 删除新画刷   
	newBrush.DeleteObject();

	// 创建实心画笔，粗度为2，颜色为黑色   
	solidPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	// 创建点线画笔，粗度为1，颜色为蓝色
	dotPen.CreatePen(PS_DOT, 1, RGB(0, 0, 255));
	// 选择新画笔，并将旧画笔的指针保存到pOldPen   
	pOldPen = pDC->SelectObject(&solidPen);
	//pDC->Rectangle(m_rect);
	//绘制Y轴   
	pDC->MoveTo(codrect.left, codrect.bottom);
	pDC->LineTo(codrect.left, codrect.top);
	//绘制箭头
	pDC->LineTo(codrect.left - 4, codrect.top + 4);
	pDC->MoveTo(codrect.left, codrect.top);
	pDC->LineTo(codrect.left + 4, codrect.top + 4);

	//绘制X轴
	pDC->MoveTo(codrect.left, codrect.bottom);
	pDC->LineTo(codrect.right, codrect.bottom);
	//绘制箭头
	pDC->LineTo(codrect.right - 4, codrect.bottom - 4);
	pDC->MoveTo(codrect.right, codrect.bottom);
	pDC->LineTo(codrect.right - 4, codrect.bottom + 4);


	//绘制网格和刻度
	pOldPen = pDC->SelectObject(&dotPen);
	int xlength = codrect.right - codrect.left;
	int ylength = codrect.bottom - codrect.top;
	int xavlength = (codrect.right - codrect.left) / 2;
	int yavlength = ylength / 10;
	for (int i = 1; i <= 10; i++)
	{
		CString scale;
		scale.Format(_T("%d"), i * 5);
		if (i == 1)
			pDC->TextOutW(codrect.left - 20, codrect.bottom, _T("0"));
		pDC->TextOutW(codrect.left - 20, codrect.bottom - yavlength*i, scale);
		pDC->MoveTo(codrect.left, codrect.bottom - yavlength*i);
		pDC->LineTo(codrect.right, codrect.bottom - yavlength*i);
	}
	//显示连续两个小时的温度，前一个小时是采集到的，后一个小时是预测的
	for (int i = 0; i <2; i++)
	{
		CString strTime = this->GetCurTime();
		strTime = strTime.Mid(strTime.GetLength() - 3, 2);
		int intTime = _ttoi(strTime) + i;
		strTime.Format(_T("%d"), intTime);
		pDC->TextOutW(codrect.left + i*xavlength, codrect.bottom + 10, strTime);
	}

	//绘制X/Y轴标识
	pDC->TextOutW(codrect.left - 40, codrect.bottom - yavlength * 5, _T("温"));
	pDC->TextOutW(codrect.left - 40, codrect.bottom - yavlength * 5 + 15, _T("度"));

	pDC->TextOutW(codrect.left + xavlength, codrect.bottom + 30, _T("时间"));
	//AfxMessageBox(_T("SDS"));
	//InvalidateRect(rectPicture);
	// 恢复旧画笔   
	pDC->SelectObject(pOldPen);
	// 删除新画笔   
	newBrush.DeleteObject();
	solidPen.DeleteObject();
	dotPen.DeleteObject();
}


void CTempretureForecastDlg::DrawWave(CDC *pDC, CRect &rect)
{
	float fDeltaX;     // x轴相邻两个绘图点的坐标距离   
	float fDeltaY;     // y轴每个逻辑单位对应的坐标值   
	int nX;      // 在连线时用于存储绘图点的横坐标   
	int nY;      // 在连线时用于存储绘图点的纵坐标   
	CPen newPen;       // 用于创建新画笔   
	CPen *pOldPen;     // 用于存放旧画笔   

	CRect codrect;    //坐标系矩形区域
	codrect.bottom = rect.bottom - 50;
	codrect.top = rect.top + 20;
	codrect.left = rect.left + 50;
	codrect.right = rect.right - 20;

	// 计算fDeltaX和fDeltaY   
	fDeltaX = (float)codrect.Width() / (POINT_COUNT - 1);
	fDeltaY = (float)codrect.Height() / 50;



	// 创建实心画笔，粗度为1，颜色为绿色   
	newPen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	// 选择新画笔，并将旧画笔的指针保存到pOldPen   
	pOldPen = pDC->SelectObject(&newPen);

	// 将当前点移动到绘图控件窗口的左下角，以此为波形的起始点   
	pDC->MoveTo(codrect.left, codrect.bottom);
	// 计算m_nzValues数组中每个点对应的坐标位置，并依次连接，最终形成曲线   
	for (int i = 0; i<POINT_COUNT; i++)
	{
		nX = codrect.left + (int)(i * fDeltaX);
		nY = codrect.bottom - (int)(m_waveDataValues[i] * fDeltaY);
		if (m_waveDataValues[i] == 0)
			pDC->MoveTo(nX, nY);
		else
			pDC->LineTo(nX, nY);
	}
	// 恢复旧画笔   
	pDC->SelectObject(pOldPen);
	// 删除新画笔   
	newPen.DeleteObject();
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
	KillTimer(TIMER_DRAW);
	KillTimer(TIMER_COLLECT);
	CDialogEx::OnClose();
}


CString CTempretureForecastDlg::GetCurTime()
{
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	CString str;
	str.Format(_T("%02d月%02d日 %02d时"), curTime.wMonth, curTime.wDay,
		curTime.wHour);
	return str;
}


CString CTempretureForecastDlg::GetCurTemp(int currTemp)
{
	CString str;
	str.Format(_T("%d"), currTemp);
	str = _T("实时温度：") + str + _T("℃");
	return str;
}
