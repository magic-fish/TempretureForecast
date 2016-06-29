
// TempretureForecastDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TempretureForecast.h"
#include "TempretureForecastDlg.h"
#include "afxdialogex.h"
#include "TempData.h"
#include "FileImpl.h"
#include "LinearPredict.h"
#include "CurvePredict.h"
#include <string>
#include "EncodingUtil.h"
#include "TempretureImpl.h"
#include "SourceWRFactory.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW

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
	datanum = 0;
	buffswitch = 0;
	m_alarmValue = 50;
	isplay = false;
	m_Algorithm = CAlgorithmFactory::CreateAlgorithm("直线拟合");
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
ON_BN_CLICKED(IDC_IMPORTBTN, &CTempretureForecastDlg::OnBnClickedImportbtn)
END_MESSAGE_MAP()


// CTempretureForecastDlg 消息处理程序

BOOL CTempretureForecastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	HANDLE hThread = CreateThread(NULL, 0, DataReceiveInitProc, (LPVOID)this, 0, NULL);
	CloseHandle(hThread);

	m_displayer.Init(m_rect);
	// 以时间为种子来构造随机数生成器   
	srand((unsigned)time(NULL));
	// 启动定时器，ID为1，定时时间为200ms   
	SetTimer(TIMER_1S, 1000 / TIMER_1HZ, NULL);
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
	// 显示当前的最高温度和最低温度
	m_numOfTemp = POINT_COUNT;
	//GetMaxAndMinTemp();
	m_MaxTemp = m_displayer.GetMaxTemp();
	m_MinTemp = m_displayer.GetMinTemp();
	// 显示当前时间和实时温度
	m_date = GetCurTime();
	CTempData pTempdata = m_displayer.m_tempData[m_displayer.m_tempData.size()-1];
	m_curTime.Format(_T("%02d:%02d:%02d"), pTempdata.m_date.wHour, pTempdata.m_date.wMinute, pTempdata.m_date.wSecond);
	m_curTemp.Format(_T("%.0lf ℃"), pTempdata.m_temperature);
	UpdateData(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}



void CTempretureForecastDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	if (buffswitch == 0){
		CDataStorage *pDataStorage = CSourceWRFactory::CreateSourceWR(FILE_WR);
		//将缓存容器I的数据插入到文件中
		pDataStorage->Write(m_DataVecBuffI);
		//切换容器
		m_DataVecBuffI.clear();
		delete pDataStorage;
		pDataStorage = NULL;
	}
	else{
		CDataStorage *pDataStorage = CSourceWRFactory::CreateSourceWR(FILE_WR);
		//将缓存容器II的数据插入到文件中
		pDataStorage->Write(m_DataVecBuffII);

		m_DataVecBuffII.clear();
		delete pDataStorage;
		pDataStorage = NULL;
	}
	m_memDC.DeleteDC();        //删除DC
	m_bmp.DeleteObject();      //删除位图
	KillTimer(m_curTimerID);
	recv.End();
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
	m_collectInterval.AddString(_T("1Hz"));
	m_collectInterval.AddString(_T("2Hz"));
	m_collectInterval.AddString(_T("3Hz"));
	m_collectInterval.AddString(_T("5Hz"));
	m_collectInterval.AddString(_T("10Hz"));
	m_collectInterval.AddString(_T("20Hz"));
	m_collectInterval.AddString(_T("30Hz"));

	m_collectInterval.SetCurSel(0);

	//初始化预测算法
	m_forecastAlgorithm.AddString(_T("直线拟合"));
	m_forecastAlgorithm.AddString(_T("曲线拟合"));

	m_forecastAlgorithm.SetCurSel(0);
}


void CTempretureForecastDlg::OnCbnSelchangeComboAlarm()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strAlarmValue;
	// 获取组合框控件的列表框中选中项的索引   
	int nSel = m_highTempAlarm.GetCurSel();
	// 根据选中项索引获取该项字符串   
	m_highTempAlarm.GetLBText(nSel, strAlarmValue);
	// 将组合框中选中的字符串显示到IDC_SEL_WEB_EDIT编辑框中
	m_alarmValue = nSel + 10;
}


void CTempretureForecastDlg::OnCbnSelchangeComboCollectinterval()
{
	CString strIntervalValue;
	int nSel = m_collectInterval.GetCurSel();
	
	m_collectInterval.GetLBText(nSel, strIntervalValue);
	if (strIntervalValue == HZ1){
		recv.SetFrequency(HZ_1);
	}
	else if (strIntervalValue == HZ2){
		recv.SetFrequency(HZ_2);
	}
	else if (strIntervalValue == HZ3){
		recv.SetFrequency(HZ_3);
	}
	else if (strIntervalValue == HZ5){
		recv.SetFrequency(HZ_5);
	}
	else if (strIntervalValue == HZ10){
		recv.SetFrequency(HZ_10);
	}
	else if (strIntervalValue == HZ20){
		recv.SetFrequency(HZ_20);
	}else{
		recv.SetFrequency(HZ_30);
	}
	recv.Send();
}

//选择预测算法改变的类
void CTempretureForecastDlg::OnCbnSelchangeComboForecastalg()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strAlgValue;
	int nSel = m_forecastAlgorithm.GetCurSel();
	m_forecastAlgorithm.GetLBText(nSel, strAlgValue);
	delete m_Algorithm;

	if (strAlgValue == _T("直线拟合")){
		m_Algorithm = CAlgorithmFactory::CreateAlgorithm(CEncodingUtil::CStringToString(strAlgValue));
	}
	
	if (strAlgValue == _T("曲线拟合")){
		m_Algorithm = CAlgorithmFactory::CreateAlgorithm(CEncodingUtil::CStringToString(strAlgValue));
	}
}


DWORD WINAPI CTempretureForecastDlg::DataReceiveInitProc(LPVOID lpParameter)
{
	CTempretureForecastDlg * pTaskMain = (CTempretureForecastDlg *)lpParameter;   //把this指针传进来
	pTaskMain->recv.Init(pTaskMain);
	pTaskMain->recv.Start();
	pTaskMain->recv.SetFrequency("1");
	return 0;
}
void CTempretureForecastDlg::UpDateTempView(CTempData* data)
{
	//采集的温度数据总数
	datanum++;
	//选择容器，将采集的温度插入到容器中
	if (buffswitch == 1){
		if (datanum % MAX_VEC_SIZE == 0){
			//创建一个线程进行将缓存容器II中的数据保存到文件中
			HANDLE hThread = CreateThread(NULL, 0, FileWriteProc, (LPVOID)this, 0, NULL);
			CloseHandle(hThread);
		}
		m_DataVecBuffII.push_back(data);

	}
	if (buffswitch == 0)
	{
		if (datanum % MAX_VEC_SIZE == 0)
		{
			//创建一个线程进行将缓存容器I中的数据保存到文件中
			HANDLE hThread = CreateThread(NULL, 0, FileWriteProc, (LPVOID)this, 0, NULL);
			CloseHandle(hThread);
			//m_DataVecBuffII.push_back(data);
		}
		m_DataVecBuffI.push_back(data);
	}
	//插入温度数据
	m_displayer.AddData(*data);
	//添加预测的温度数据
	m_displayer.AddForecastData(m_Algorithm->GetForecastData(m_displayer.m_tempData));
	//绘制坐标系
	m_displayer.DrawCoordinate(&m_memDC, m_rect);
	// 绘制波形图   
	m_displayer.DrawGraph(&m_memDC);
	//将缓冲DC画到实际的窗口上
	m_bgPic.GetDC()->BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_memDC, 0, 0, SRCCOPY);
	if (data->m_temperature >= m_alarmValue&&isplay == false)
	{
		//播放预警声音线程
		HANDLE hThread = CreateThread(NULL, 0, PlayProc, (LPVOID)this, 0, NULL);
		CloseHandle(hThread);
	}
	Invalidate(false);
}

DWORD WINAPI CTempretureForecastDlg::FileWriteProc(LPVOID lpParameter)
{
	CTempretureForecastDlg * pTaskMain = (CTempretureForecastDlg *)lpParameter;   //把this指针传进来
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	if (pTaskMain->buffswitch == 0)
	{
		//将缓存容器I的数据插入到文件中
		CDataStorage *pDataStorage = CSourceWRFactory::CreateSourceWR(FILE_WR);
		pDataStorage->Write(pTaskMain->m_DataVecBuffI);
		pTaskMain->CleanBuff();
		//切换容器
		pTaskMain->buffswitch++;

		delete pDataStorage;
		pDataStorage = NULL;
		return 0;
	}
	else{
		CDataStorage *pDataStorage = CSourceWRFactory::CreateSourceWR(FILE_WR);
		pDataStorage->Write(pTaskMain->m_DataVecBuffII);
		pTaskMain->CleanBuff();
		pTaskMain->buffswitch--;

		delete pDataStorage;
		pDataStorage = NULL;
		return 0;
	}
	return 0;
}


void CTempretureForecastDlg::OnBnClickedImportbtn()
{
	CString FilePathName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName(); //文件名保存在了FilePathName里
		filepath = CEncodingUtil::CStringToString(FilePathName);
		HANDLE hThread = CreateThread(NULL, 0, SQLWriteProc, (LPVOID)this, 0, NULL);
		CloseHandle(hThread);
	}
}
DWORD WINAPI CTempretureForecastDlg::SQLWriteProc(LPVOID lpParameter)
{
	CTempretureForecastDlg * pTaskMain = (CTempretureForecastDlg *)lpParameter;   //把this指针传进来
	CDataStorage *pDataStorageFile = CSourceWRFactory::CreateSourceWR(FILE_WR);
	vector<CTempData*> pdata = pDataStorageFile->Read(pTaskMain->filepath);

	delete pDataStorageFile;
	pDataStorageFile = NULL;
	CDataStorage *pDataStorageSql= CSourceWRFactory::CreateSourceWR(MYSQL_WR);

	pDataStorageSql->Write(pdata);

	delete pDataStorageSql;
	pDataStorageSql = NULL;
	AfxMessageBox(_T("导入到数据库成功"));
	return 0;
}

void CTempretureForecastDlg::PlayWav()
{
	HMODULE   hmod = AfxGetResourceHandle();
	HRSRC   hSndResource = FindResource(hmod, MAKEINTRESOURCE(IDR_WAVE1), _T("WAVE"));
	HGLOBAL   hGlobalMem = LoadResource(hmod, hSndResource);
	LPCTSTR   lpMemSound = (LPCTSTR)LockResource(hGlobalMem);
	sndPlaySound(lpMemSound, SND_MEMORY);
	FreeResource(hGlobalMem);
}

DWORD WINAPI CTempretureForecastDlg::PlayProc(LPVOID lpParameter)
{
	CTempretureForecastDlg * pTaskMain = (CTempretureForecastDlg *)lpParameter;   //把this指针传进来
	pTaskMain->isplay = true;
	pTaskMain->PlayWav();
	pTaskMain->isplay = false;
	return 0;
}

void CTempretureForecastDlg::CleanBuff()
{
	if (buffswitch == 0)
	{
		for (int i = 0; i < m_DataVecBuffI.size(); i++){
			CTempData* pdata = m_DataVecBuffI.at(i);
			delete pdata;
			pdata = NULL;
		
		}

		m_DataVecBuffI.clear();
		
	}
	else{

		for (int i = 0; i < m_DataVecBuffII.size(); i++){
			CTempData* pdata = m_DataVecBuffII.at(i);
			delete pdata;
			pdata = NULL;
		}
		m_DataVecBuffII.clear();

	}
}