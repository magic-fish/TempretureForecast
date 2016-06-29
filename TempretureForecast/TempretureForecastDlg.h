
// TempretureForecastDlg.h : ͷ�ļ�
//

#pragma once
#include "Display.h"
#include "afxwin.h"
#include "resource.h"
#include "DataReceive.h"
#include "mmsystem.h"//��������ͷ�ļ�
#include "PredictAlgorithm.h"
#include "AlgorithmFactory.h"
#pragma comment(lib,"winmm.lib")//��������ͷ�ļ���
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
	int m_alarmValue;     //����ֵ
	int m_MaxTemp;        //����¶�
	int m_MinTemp;        //����¶�
	int m_numOfTemp;      //�¶���������
	CDisplay m_displayer; //ͼ����ʾ	
	int m_curTimerID;     //��ǰ�Ķ�ʱ��ID
	DataReceive recv;	  //Tcp��������								
	CPredictAlgorithm* m_Algorithm;									//Ԥ���㷨����
	string filepath;												//ѡ����ļ�·��

	static DWORD WINAPI DataReceiveInitProc(LPVOID lpParameter);	//���²ɼ�Ƶ���̺߳���
	static DWORD WINAPI FileWriteProc(LPVOID lpParameter);			//�ɼ�����д���ļ����̺߳���
	static DWORD WINAPI SQLWriteProc(LPVOID lpParameter);			//���ļ��л�ȡ���ݵ��뵽���ݿ���
	static DWORD WINAPI PlayProc(LPVOID lpParameter);				//�������ֵ��߳�
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
	
public:
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeComboAlarm();
	afx_msg void OnCbnSelchangeComboForecastalg();
	afx_msg void OnCbnSelchangeComboCollectinterval();
	afx_msg void OnBnClickedImportbtn();				//�����ļ����ݵ����ݿ���

	CString GetCurTime();							    //��õ�ǰʱ��
	CString GetCurTemp(int currTemp);
	void InitComboBox();								//��ʼ��Combox

	CComboBox m_highTempAlarm;						    // ���¾����ؼ�
	CComboBox m_collectInterval;						// ���ݲɼ�ʱ�����ؼ�
	
	CComboBox m_forecastAlgorithm;						// Ԥ���㷨
	CString m_curTemp;									// ʵʱ�¶�

	vector<CTempData*> m_DataVecBuffI;					//�¶ȵĲɼ���������I
	vector<CTempData*> m_DataVecBuffII;					//�¶ȵĲɼ���������II
	int  buffswitch;									//�����л��Ŀ���
	int  datanum;
	void UpDateTempView(CTempData* data);				//ʵʱ�ɼ����ݸ��½���

	void PlayWav();									    // ���ű�������
	bool isplay;										//��ǰ�Ƿ����ڲ��ű�������
	void CleanBuff();									//����������ݿ��Ļ�����
};
