
// TempretureForecastDlg.h : ͷ�ļ�
//

#pragma once
#include "Display.h"
#include "afxwin.h"
#include "resource.h"
#include "DataReceive.h"
#define POINT_COUNT 48


#define HZ_30 "30"
#define HZ_20 "20"
#define HZ_10 "10"
#define HZ_5  "5"
#define HZ_3  "3"
#define HZ_2  "2"
#define HZ_1  "1"

#define POINT_COUNT 48
#define TIMER_1HZ 1
#define TIMER_2HZ 2
#define TIMER_3HZ 3
#define TIMER_5HZ 5
#define TIMER_10HZ 10
#define TIMER_20HZ 20
#define TIMER_30HZ 30

#define TIMER_1S 0
#define TIMER_10S 1
#define TIMER_30S 2
#define TIMER_1M 3
#define TIMER_10M 4
#define TIMER_30M 5
#define TIMER_1H 6
class DataReceive;
// CTempretureForecastDlg �Ի���
class CTempretureForecastDlg : public CDialogEx
{
// ����
public:
	CTempretureForecastDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEMPRETUREFORECAST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	//int m_waveDataValues[POINT_COUNT];
	CString m_date;       //����
	CString m_curTime;    //��ǰʱ��
	CStatic m_bgPic;      //����
	CDC m_memDC;          //�ڴ�DC
	CBitmap m_bmp;        //����λͼ
	CRect m_rect;         //���οͻ���
	//CDataSave m_optData;//���ݲ�������
	int m_alarmValue;     //����ֵ
	int m_MaxTemp;        //����¶�
	int m_MinTemp;        //����¶�
	int m_numOfTemp;      //�¶���������
	CDisplay m_displayer;   //ͼ����ʾ	
	int m_curTimerID;     //��ǰ�Ķ�ʱ��ID
	DataReceive recv;
	static DWORD WINAPI DataReceiveProc(LPVOID lpParameter);
	static DWORD WINAPI DataReceiveInitProc(LPVOID lpParameter);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	// ��õ�ǰʱ��ε�����¶Ⱥ�����¶�
	void GetMaxAndMinTemp();

	
public:
	afx_msg void OnClose();
	//��õ�ǰʱ��
	CString GetCurTime();
	CString GetCurTemp(int currTemp);
	void InitComboBox();
	// ���¾���
	CComboBox m_highTempAlarm; 
	afx_msg void OnCbnSelchangeComboAlarm();
	// ���ݲɼ�ʱ����
	CComboBox m_collectInterval;
	afx_msg void OnCbnSelchangeComboCollectinterval();
	// Ԥ���㷨
	CComboBox m_forecastAlgorithm;
	afx_msg void OnCbnSelchangeComboForecastalg();
	// ʵʱ�¶�
	CString m_curTemp;
	//�¶ȵ���������
	vector<CTempData*> m_DataVecBuffI;
	vector<CTempData*> m_DataVecBuffII;
	void UpDateTempView(CTempData * data);
};
