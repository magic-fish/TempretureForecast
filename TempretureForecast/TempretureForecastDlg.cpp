
// TempretureForecastDlg.cpp : ʵ���ļ�
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
	datanum = 0;
	buffswitch = 0;
	m_alarmValue = 50;
	isplay = false;
	m_Algorithm = CAlgorithmFactory::CreateAlgorithm("ֱ�����");
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


// CTempretureForecastDlg ��Ϣ�������

BOOL CTempretureForecastDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	// ��ʾ��ǰ������¶Ⱥ�����¶�
	m_numOfTemp = POINT_COUNT;
	//GetMaxAndMinTemp();
	m_MaxTemp = m_displayer.GetMaxTemp();
	m_MinTemp = m_displayer.GetMinTemp();
	// ��ʾ��ǰʱ���ʵʱ�¶�
	m_date = GetCurTime();
	CTempData pTempdata = m_displayer.m_tempData[m_displayer.m_tempData.size()-1];
	m_curTime.Format(_T("%02d:%02d:%02d"), pTempdata.m_date.wHour, pTempdata.m_date.wMinute, pTempdata.m_date.wSecond);
	m_curTemp.Format(_T("%.0lf ��"), pTempdata.m_temperature);
	UpdateData(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}



void CTempretureForecastDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	if (buffswitch == 0){
		CDataStorage *pDataStorage = CSourceWRFactory::CreateSourceWR(FILE_WR);
		//����������I�����ݲ��뵽�ļ���
		pDataStorage->Write(m_DataVecBuffI);
		//�л�����
		m_DataVecBuffI.clear();
		delete pDataStorage;
		pDataStorage = NULL;
	}
	else{
		CDataStorage *pDataStorage = CSourceWRFactory::CreateSourceWR(FILE_WR);
		//����������II�����ݲ��뵽�ļ���
		pDataStorage->Write(m_DataVecBuffII);

		m_DataVecBuffII.clear();
		delete pDataStorage;
		pDataStorage = NULL;
	}
	m_memDC.DeleteDC();        //ɾ��DC
	m_bmp.DeleteObject();      //ɾ��λͼ
	KillTimer(m_curTimerID);
	recv.End();
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
	m_forecastAlgorithm.AddString(_T("ֱ�����"));
	m_forecastAlgorithm.AddString(_T("�������"));

	m_forecastAlgorithm.SetCurSel(0);
}


void CTempretureForecastDlg::OnCbnSelchangeComboAlarm()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strAlarmValue;
	// ��ȡ��Ͽ�ؼ����б����ѡ���������   
	int nSel = m_highTempAlarm.GetCurSel();
	// ����ѡ����������ȡ�����ַ���   
	m_highTempAlarm.GetLBText(nSel, strAlarmValue);
	// ����Ͽ���ѡ�е��ַ�����ʾ��IDC_SEL_WEB_EDIT�༭����
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

//ѡ��Ԥ���㷨�ı����
void CTempretureForecastDlg::OnCbnSelchangeComboForecastalg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strAlgValue;
	int nSel = m_forecastAlgorithm.GetCurSel();
	m_forecastAlgorithm.GetLBText(nSel, strAlgValue);
	delete m_Algorithm;

	if (strAlgValue == _T("ֱ�����")){
		m_Algorithm = CAlgorithmFactory::CreateAlgorithm(CEncodingUtil::CStringToString(strAlgValue));
	}
	
	if (strAlgValue == _T("�������")){
		m_Algorithm = CAlgorithmFactory::CreateAlgorithm(CEncodingUtil::CStringToString(strAlgValue));
	}
}


DWORD WINAPI CTempretureForecastDlg::DataReceiveInitProc(LPVOID lpParameter)
{
	CTempretureForecastDlg * pTaskMain = (CTempretureForecastDlg *)lpParameter;   //��thisָ�봫����
	pTaskMain->recv.Init(pTaskMain);
	pTaskMain->recv.Start();
	pTaskMain->recv.SetFrequency("1");
	return 0;
}
void CTempretureForecastDlg::UpDateTempView(CTempData* data)
{
	//�ɼ����¶���������
	datanum++;
	//ѡ�����������ɼ����¶Ȳ��뵽������
	if (buffswitch == 1){
		if (datanum % MAX_VEC_SIZE == 0){
			//����һ���߳̽��н���������II�е����ݱ��浽�ļ���
			HANDLE hThread = CreateThread(NULL, 0, FileWriteProc, (LPVOID)this, 0, NULL);
			CloseHandle(hThread);
		}
		m_DataVecBuffII.push_back(data);

	}
	if (buffswitch == 0)
	{
		if (datanum % MAX_VEC_SIZE == 0)
		{
			//����һ���߳̽��н���������I�е����ݱ��浽�ļ���
			HANDLE hThread = CreateThread(NULL, 0, FileWriteProc, (LPVOID)this, 0, NULL);
			CloseHandle(hThread);
			//m_DataVecBuffII.push_back(data);
		}
		m_DataVecBuffI.push_back(data);
	}
	//�����¶�����
	m_displayer.AddData(*data);
	//���Ԥ����¶�����
	m_displayer.AddForecastData(m_Algorithm->GetForecastData(m_displayer.m_tempData));
	//��������ϵ
	m_displayer.DrawCoordinate(&m_memDC, m_rect);
	// ���Ʋ���ͼ   
	m_displayer.DrawGraph(&m_memDC);
	//������DC����ʵ�ʵĴ�����
	m_bgPic.GetDC()->BitBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_memDC, 0, 0, SRCCOPY);
	if (data->m_temperature >= m_alarmValue&&isplay == false)
	{
		//����Ԥ�������߳�
		HANDLE hThread = CreateThread(NULL, 0, PlayProc, (LPVOID)this, 0, NULL);
		CloseHandle(hThread);
	}
	Invalidate(false);
}

DWORD WINAPI CTempretureForecastDlg::FileWriteProc(LPVOID lpParameter)
{
	CTempretureForecastDlg * pTaskMain = (CTempretureForecastDlg *)lpParameter;   //��thisָ�봫����
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	if (pTaskMain->buffswitch == 0)
	{
		//����������I�����ݲ��뵽�ļ���
		CDataStorage *pDataStorage = CSourceWRFactory::CreateSourceWR(FILE_WR);
		pDataStorage->Write(pTaskMain->m_DataVecBuffI);
		pTaskMain->CleanBuff();
		//�л�����
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
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName(); //�ļ�����������FilePathName��
		filepath = CEncodingUtil::CStringToString(FilePathName);
		HANDLE hThread = CreateThread(NULL, 0, SQLWriteProc, (LPVOID)this, 0, NULL);
		CloseHandle(hThread);
	}
}
DWORD WINAPI CTempretureForecastDlg::SQLWriteProc(LPVOID lpParameter)
{
	CTempretureForecastDlg * pTaskMain = (CTempretureForecastDlg *)lpParameter;   //��thisָ�봫����
	CDataStorage *pDataStorageFile = CSourceWRFactory::CreateSourceWR(FILE_WR);
	vector<CTempData*> pdata = pDataStorageFile->Read(pTaskMain->filepath);

	delete pDataStorageFile;
	pDataStorageFile = NULL;
	CDataStorage *pDataStorageSql= CSourceWRFactory::CreateSourceWR(MYSQL_WR);

	pDataStorageSql->Write(pdata);

	delete pDataStorageSql;
	pDataStorageSql = NULL;
	AfxMessageBox(_T("���뵽���ݿ�ɹ�"));
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
	CTempretureForecastDlg * pTaskMain = (CTempretureForecastDlg *)lpParameter;   //��thisָ�봫����
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