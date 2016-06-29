
// TempretureForecastDlg.h : 头文件
//

#pragma once
#include "Display.h"
#include "afxwin.h"
#include "resource.h"
#include "DataReceive.h"
#include "mmsystem.h"//导入声音头文件
#include "PredictAlgorithm.h"
#include "AlgorithmFactory.h"
#pragma comment(lib,"winmm.lib")//导入声音头文件库
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

#define HZ30 "30Hz"
#define HZ20 "20Hz"
#define HZ10 "10Hz"
#define HZ5  "5Hz"
#define HZ3  "3Hz"
#define HZ2  "2Hz"
#define HZ1  "1Hz"
#define FILE_WR "File"
#define MYSQL_WR "Mysql"
#define TIMER_1S 0
#define MAX_VEC_SIZE 50
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
	int m_alarmValue;     //警报值
	int m_MaxTemp;        //最高温度
	int m_MinTemp;        //最低温度
	int m_numOfTemp;      //温度数据数量
	CDisplay m_displayer; //图像显示	
	int m_curTimerID;     //当前的定时器ID
	DataReceive recv;	  //Tcp数据连接								
	CPredictAlgorithm* m_Algorithm;									//预测算法变量
	string filepath;												//选择的文件路径

	static DWORD WINAPI DataReceiveInitProc(LPVOID lpParameter);	//更新采集频率线程函数
	static DWORD WINAPI FileWriteProc(LPVOID lpParameter);			//采集数据写入文件的线程函数
	static DWORD WINAPI SQLWriteProc(LPVOID lpParameter);			//从文件中获取数据导入到数据库中
	static DWORD WINAPI PlayProc(LPVOID lpParameter);				//播放音乐的线程
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
	
public:
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboAlarm();
	afx_msg void OnCbnSelchangeComboForecastalg();
	afx_msg void OnCbnSelchangeComboCollectinterval();
	afx_msg void OnBnClickedImportbtn();				//导入文件数据到数据库中

	CString GetCurTime();							    //获得当前时间
	CString GetCurTemp(int currTemp);
	void InitComboBox();								//初始化Combox

	CComboBox m_highTempAlarm;						    // 高温警报控件
	CComboBox m_collectInterval;						// 数据采集时间间隔控件
	
	CComboBox m_forecastAlgorithm;						// 预测算法
	CString m_curTemp;									// 实时温度

	vector<CTempData*> m_DataVecBuffI;					//温度的采集数据容器I
	vector<CTempData*> m_DataVecBuffII;					//温度的采集数据容器II
	int  buffswitch;									//容器切换的开关
	int  datanum;
	void UpDateTempView(CTempData* data);				//实时采集数据更新界面

	void PlayWav();									    // 播放报警声音
	bool isplay;										//当前是否正在播放报警声音
	void CleanBuff();									//清理存入数据库后的缓存区
};
