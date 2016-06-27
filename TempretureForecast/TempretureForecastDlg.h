
// TempretureForecastDlg.h : 头文件
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
// CTempretureForecastDlg 对话框
class CTempretureForecastDlg : public CDialogEx
{
// 构造
public:
	CTempretureForecastDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEMPRETUREFORECAST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	//int m_waveDataValues[POINT_COUNT];
	CString m_date;       //日期
	CString m_curTime;    //当前时间
	CStatic m_bgPic;      //背景
	CDC m_memDC;          //内存DC
	CBitmap m_bmp;        //缓存位图
	CRect m_rect;         //矩形客户区
	//CDataSave m_optData;//数据操作对象
	int m_alarmValue;     //警报值
	int m_MaxTemp;        //最高温度
	int m_MinTemp;        //最低温度
	int m_numOfTemp;      //温度数据数量
	CDisplay m_displayer;   //图像显示	
	int m_curTimerID;     //当前的定时器ID
	DataReceive recv;
	static DWORD WINAPI DataReceiveProc(LPVOID lpParameter);
	static DWORD WINAPI DataReceiveInitProc(LPVOID lpParameter);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	// 获得当前时间段的最高温度和最低温度
	void GetMaxAndMinTemp();

	
public:
	afx_msg void OnClose();
	//获得当前时间
	CString GetCurTime();
	CString GetCurTemp(int currTemp);
	void InitComboBox();
	// 高温警报
	CComboBox m_highTempAlarm; 
	afx_msg void OnCbnSelchangeComboAlarm();
	// 数据采集时间间隔
	CComboBox m_collectInterval;
	afx_msg void OnCbnSelchangeComboCollectinterval();
	// 预测算法
	CComboBox m_forecastAlgorithm;
	afx_msg void OnCbnSelchangeComboForecastalg();
	// 实时温度
	CString m_curTemp;
	//温度的数据容器
	vector<CTempData*> m_DataVecBuffI;
	vector<CTempData*> m_DataVecBuffII;
	void UpDateTempView(CTempData * data);
};
