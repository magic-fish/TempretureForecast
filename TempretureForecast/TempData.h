#pragma once
//�¶�������
/*
 * author wjy
 * ��ӽӿ��������ݽ���
 */
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
	CString Tostring();
};

