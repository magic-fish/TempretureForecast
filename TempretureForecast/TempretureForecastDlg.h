
// TempretureForecastDlg.h : ͷ�ļ�
//

#pragma once
#include "Display.h"
#include "afxwin.h"
#define POINT_COUNT 48


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
	CString m_dateAndCurTempreture;         //��ǰʱ��͵�ǰ�¶�
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
};
