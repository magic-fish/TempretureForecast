#pragma once
//�¶�������
/*
 * author wjy
 * ��ӽӿ��������ݽ���
 */
#define MAX_SIZE 1024
#include <string>
using namespace std;
class CTempData
{
public:
	CTempData();
	~CTempData();
	// �¶�
	double m_temperature;
	// ʱ��
	SYSTEMTIME m_date;

	CTempData(double temperature, SYSTEMTIME date);
	//�����¶ȵ��ַ���
	CString GetTemperature();
	//����ʱ����ַ���
	CString GetTime();
	// �����ַ���
	string ToData();
};

