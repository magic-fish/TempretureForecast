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

	double m_temperature;									// �¶�
	SYSTEMTIME m_date;										// ʱ��

	CTempData(double temperature, SYSTEMTIME date);
	CString GetTemperature();								//�����¶ȵ��ַ���
	CString GetTime();									    //����ʱ����ַ���
	string ToData();										// �����ַ���
};

